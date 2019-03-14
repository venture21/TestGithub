#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>

int main (void)
{
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

	printf ("lines %d\n", ws.ws_row);
	printf ("columns %d\n", ws.ws_col);

	return 0;
}
