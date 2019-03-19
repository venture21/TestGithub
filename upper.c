#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
		int ch;
		while((ch = getchar()) != EOF) {
				putchar(toupper(ch));
		}
		exit(0);
}
