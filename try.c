#include <stdio.h>
#include <stdlib.h>

void	bad_malloc()
{
	void* a = malloc(13);
	a += 2;
	free(a);
}

int main()
{
	// int a = 1;
	// printf("%d\n", a ?: 4);
	bad_malloc();
}
