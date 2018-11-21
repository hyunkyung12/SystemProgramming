#include "ku_tsort.h"

int n,m;

int num[MAX_NUM_SIZE];

int *sub[MAX_THREAD_NUM];
int subSize[MAX_THREAD_NUM];

int snum[MAX_NUM_SIZE];

int main(int argc, char *argv[])
{
	int i;

	pthread_t thread[MAX_THREAD_NUM + 1];
	sortArgs args[MAX_THREAD_NUM + 1];

	if (argc < 4)
	{
		printf("프로그램의 매개변수가 부족합니다.\n");
		exit(0);
	}

	// 입력
	m = atoi(argv[1]); //input integer 개수
	n = atoi(argv[2]); //thread 개수

	read(argv[3]);
	if (n > m)
	{
		printf("쓰레드의 개수가 숫자의 개수보다 많을 수 없습니다.\n");
		exit(0);
	}

	// 숫자 배열을 여러 개로 나눔
	split();

	// 나누어진 각각의 숫자 배열을 독립적으로 정렬함
	args[0].arr = sub[0];
	args[0].size = subSize[0];
	sort((void *)&args[0]);

	for (i = 1; i < n; i++)
	{
		args[i].arr = sub[i];
		args[i].size = subSize[i];
		pthread_create(&thread[i], NULL, sort, (void *)&args[i]);
	}

	for (i = 1; i < n; i++)
	{
		pthread_join(thread[i], NULL);
	}

	// 나누어진 숫자 배열들을 다시 합침
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
	
	for(i=0;i<m;i++){
		fscanf(fp,"%d",&num[i]);
	}
	fclose(fp);
}

void split()
{
	int i=0;
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
	subSize[n - 1] = m - ((n - 1) * usize);//마지막 배열엔 남는 수를 고려해
}

void *sort(void *args)
{
	int *arr;
	int size;
	int i, j;
	int temp;

	arr = ((sortArgs *)args)->arr;
	size = ((sortArgs *)args)->size;

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
	int p[MAX_THREAD_NUM] = {0, };
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

