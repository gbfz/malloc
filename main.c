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

int main() {
	char* ptr = ft_malloc(27);
	for (int i = 0; i < 26; ++i)
		ptr[i] = 'a' + i;
	ptr[26] = '\0';
	printf("%s\n", ptr);
	char *o = ft_malloc(33);
	memset(o, 'F', 32);
	o[32] = '\0';
	ft_free(ptr);
	printf("%s\n", o);
	ft_free(o);
	char *a = ft_malloc(300);
	if (a == NULL)
		return 1;
	memset(a, '|', 19);
	memset(a + 19, '.', 19);
	a[19 * 2] = '\0';
	printf("%s\n", a);
	ft_free(a);
}
