# include<stdio.h>
void hello()
{
	int a = 10;
	int b = a+5;
	int c = b * 10;
	int d = c/0;
	printf("%d\n", d);
}
