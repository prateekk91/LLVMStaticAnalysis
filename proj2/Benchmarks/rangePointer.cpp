# include<stdio.h>
# include<malloc.h>
void hello()
{
	int *arr;
	arr = (int*)malloc(50*sizeof(int));
	int a,b;
	if (a)
		b = 30;
	else
		b = 60;
	arr[b] = 100;
}
