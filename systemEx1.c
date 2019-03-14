#include <stdlib.h>
#include <stdio.h>

int main()
{
	printf("Running ps with system\n");
	system("ps -ef");
	printf("Done.\n");
	exit(0);
}
