#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])  
{
	int op1, op2, result;
	printf(argv[1]);
	printf(argv[2]);
	
	op1 = atoi(argv[1]);
	op2 = atoi(argv[2]);
	result = op1 + op2;
	printf("result=%d\n",result);
	
}

