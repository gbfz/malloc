#include "heap.h"
#include "block.h"

#include <stdio.h>
#include <string.h>

void* ft_malloc(size_t size) {
	t_heap_type type = g_handlers->get_type(size);
	return g_handlers[type].request(size);
}

void ft_free(void* ptr) {
	// t_heap_type type = g_handlers->get_type(size);
	g_handlers[TINY].release(ptr);
}

void test1() {
	char* ptr = ft_malloc(27);
	for (int i = 0; i < 26; ++i)
		ptr[i] = 'a' + i;
	ptr[26] = '\0';
	printf("%s\n", ptr);
	char *o = ft_malloc(33);
	memset(o, 'F', 32);
	o[32] = '\0';
	show_alloc_mem();
	ft_free(ptr);
	printf("%s\n", o);
	ft_free(o);
	show_alloc_mem();
	char *a = ft_malloc(300);
	if (a == NULL)
		return;
	memset(a, '|', 19);
	memset(a + 19, '.', 19);
	a[19 * 2] = '\0';
	printf("%s\n", a);
	ft_free(a);
}

void test2() {
	void* a = ft_malloc(32);
	show_alloc_mem();
	void* b = ft_malloc(319);
	show_alloc_mem();
	ft_free(a);
	ft_free(b);
	show_alloc_mem();
	void* c = ft_malloc(1);
	show_alloc_mem();
	ft_free(c);
}

int main() {
	test2();
}
