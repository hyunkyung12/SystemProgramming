#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUM_SIZE 100000
#define MAX_THREAD_NUM 100000
#define INF 2147483647

typedef struct
{
	int *arr;
	int size;
} sortArgs;

void read(char *path);
void split();
void *sort(void *args);
void merge();
void write(char *path);

