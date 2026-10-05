#include <stdio.h>
#include <string.h>
#include <unistd.h>

int sum(int x, int y)
{
	return (x + y);
}

int main(void)
{
	int i;

	i = strlen("TEST");
	printf("%d\n", i);
	write(1, "TEST\n", 5);
	printf("%d\n", sum(2, 5));
	return 0;
}
