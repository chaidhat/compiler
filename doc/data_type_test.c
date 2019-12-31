#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	char a = 1;
	int b = 1;
	int c = 0x2;
	int d = 02;
	unsigned int e = 2;
	printf("%d, %d, %x, %o, %u", a, b, c, d, e);
	return 1;
}
