#include "../Thread.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

__thread int x = 0;

void print()
{
	printf("pid=%d tid=%d x=%d\n", getpid(), fnet::tid(), x);
}

int main()
{
	printf("parent %d\n", getpid());
	print();
	x = 1;
	print();
	pid_t p = fork();

	if (p == 0)
	{
		printf("child %d\n", getpid());
		//child
		print();
		x = 2;
		print();
	}
	else
	{
		//parent
		print();
	}
}
