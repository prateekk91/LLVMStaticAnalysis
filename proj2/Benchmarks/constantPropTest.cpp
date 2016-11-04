//This is a basic constant propagation test
//to show that our analysis is capable of folding
//basic arithmetic operations.
# include<stdio.h>
void hello()
{
	int a = 10;
	int b = a+5;
	int c = b*20;
	int d = c-b;
	int e = d/a;
	printf("%d\n", e);
}
