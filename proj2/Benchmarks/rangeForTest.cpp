//This benchmark shows that even though the value of x is
//monotonically increasing in every iteration of the worklist,
//our analysis will terminate after marking x as a full set.
# include<stdio.h>
void helloRange()
{
	int arr[100];
	int i;
	int x = 0;
	for (i=0;i<100;i++)
	{
		arr[i] = 50;
		arr[x] = 100;
		x = x + i;
	}
	arr[i] = 10;
}
