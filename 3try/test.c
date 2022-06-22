#include "memory.h"
#include <string.h>

void*	ft_malloc(size_t size)
{
	return g_handlers[TINY].request(size);
}

void	ft_free(void* ptr)
{
	g_handlers[TINY].release(ptr);
}

void test1()
{
	char* a = ft_malloc(27);
	for (int i = 0; i < 26; ++i)
		a[i] = 'a' + i;
	a[26] = '\0';
	printf("%s\n", a);
	// show_alloc_mem();
	int* b = ft_malloc(50 * sizeof(int));
	for (int i = 0; i < 50; ++i)
		b[i] = i;
	for (int i = 0; i < 50; ++i)
		printf("%d ", b[i]);
	printf("\n");
	show_alloc_mem();
	// char* c = ft_malloc(12000);
	// ft_free(a);
	// ft_free(b);
}

int main()
{
	test1();
}
