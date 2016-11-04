# include<stdio.h>
void hello()
{
	int a;
	int x = 10;
	int y = 20;
	int z = 30;
	if (y>0)
	{
		a = z - y;
	}
	else
	{
		a = y - x;
	}
	//In constant propagation,
	//Even though z-y = y-x = 10, a does not get 10, even after applying flow function
	//Because the values are no longer available after merging when we reach the PHI node
	//Improving this is something we can look in the future
	printf("%d\n", a);
}
