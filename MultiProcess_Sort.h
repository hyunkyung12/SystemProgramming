#include<stdlib.h>
#include<stdio.h>
#include<mqueue.h>
#include <fcntl.h>

#define MAX_NUM_SIZE 100000
#define MAX_PROC_NUM 100000
#define INF 2147483647
#define NAME_POSIX "/my_ass"

void read(char *path);
void split();
void sort(int *arr, int size);
void merge();
void write(char *path);

