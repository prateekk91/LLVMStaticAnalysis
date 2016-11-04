# include<stdio.h>
void hello()
{
	int arr[50];
	int a,b;
	if (a)
		b = 30;
	else
		b = 60;
	//The range of b here is the union of 30 and 60 => [30,60]
	//Hence the line below gets a warning
	arr[b] = 100;
}
