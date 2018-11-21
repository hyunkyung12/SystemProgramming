#include "ku_psort.h"

int m,n;

int num[MAX_NUM_SIZE];
int *sub[MAX_PROC_NUM];
int subSize[MAX_PROC_NUM];
int buf[100]={0,};
int snum[MAX_NUM_SIZE]={0,};

int main(int argc, char *argv[])
{

	int i,j;
	int pid[MAX_PROC_NUM + 1];
	struct mq_attr attr;
	unsigned int prio;
	mqd_t mqdes;

	attr.mq_maxmsg=10;
	attr.mq_msgsize=100;

	if (argc != 5)
	{	
		printf("프로그램의 매개변수의 수가 다릅니다.\n");
		exit(0);
	}
	if (n > m)
	{
		printf("process의 수가 input의 수보다 많습니다.\n");
		exit(0);
	}

	// 입력
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	read(argv[3]);//배열 num에 input integer 저장

	// 숫자 배열을 여러 개로 나눔
	split();

	mqdes = mq_open(NAME_POSIX, O_CREAT|O_RDWR,0666,&attr);
	if(mqdes ==(mqd_t)-1) perror("mqopen fail: \n");

	// 나누어진 각각의 수열을 독립적으로 정렬함
	sort(sub[0], subSize[0]); //부모가 먼저 자신이 맡은 배열 정렬
	
	for (i = 1; i < n; i++)
	{	
		pid[i] = fork();
		if (pid[i] == 0)
		{
			sort(sub[i], subSize[i]);
			if(mq_send(mqdes,(char*)&sub[i][0],subSize[i]*4,prio)==-1) perror("sub send fail\n");//각 child가 정렬한 배열을 부모로 보냄
			exit(0);
		}
		if(mq_receive(mqdes,(char*)&sub[i][0],100,&prio)==-1) perror("sub receive fail\n");
	}

	for (i = 1; i < n; i++)
	{
		wait(NULL);
	}
	mq_close(mqdes);
	mq_unlink(NAME_POSIX);

	// 나누어진 숫자 배열을 다시 합침
	merge();

	// 출력
	write(argv[4]);

	return 0;
}

void read(char *path)
{
	FILE *fp;
	int i=0;

	fp = fopen(path, "r");

	for(i=0;i<m;i++)
	{
		fscanf(fp, "%d", &num[i]);
	}

	fclose(fp);
}

void split()
{
	int i;
	int usize;

	// 나누어질 숫자 배열의 크기를 구함
	usize = m / n;
	if (m % n > 0)
	{
		usize++;
	}

	// 숫자 배열을 n개의 부분으로 나눔
	for (i = 0; i < n; i++)
	{
		sub[i] = &num[i * usize];
		subSize[i] = usize;
	}
	subSize[n - 1] = m - ((n - 1) * usize);
}

void sort(int *arr, int size)
{
	int i, j;
	int temp;

	for (i = 0; i < size; i++)
	{
		for (j = i; j < size; j++)
		{
			if (arr[i] > arr[j])
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

void merge()
{
	int i, j;
	int p[MAX_PROC_NUM] = {0, };
	int min;
	int minId;

	for (i = 0; i < m; i++)
	{
		min = INF;
		for (j = 0; j < n; j++)
		{
			if (p[j] < subSize[j] && min > sub[j][p[j]])
			{
				min = sub[j][p[j]];
				minId = j;
			}
		}

		snum[i] = min;
		p[minId]++;
	}
}

void write(char *path)
{
	FILE *fp;
	int i;

	fp = fopen(path, "w");

	for (i = 0; i < m; i++)
	{
		fprintf(fp, "%d\n", snum[i]);
	}

	fclose(fp);
}

