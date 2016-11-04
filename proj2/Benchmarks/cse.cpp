//Benchmark to describe capabilities and possible improvements in CSE
# include<stdio.h>
int helloCSE()
{
	int a = 10, b = 20, c = 30;
	int d = a+b;
	int x = a+b;	//gets replaced by d
	int e = b+a;	//Does not get replaced. Ideally should have
	int f = e+10;
	int y = x+20;
	int g = c+a+b;	//Gets broken down into temp = c+a, g = temp+b. Since neither of them are already availble expressions, this does not get optimised
	return 0;
}
