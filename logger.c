#include "logger.h"

static inline
void	show_tiny_heap_usage_impl(const t_heap* heap)
{
	if (!heap)
		return;
	size_t distance = 0;
	t_header header = (t_header)(heap + HEAP_OFFSET);
	while (distance < heap->size) {
		if (IS_ALLOCATED_BLOCK(header)) {
			t_header end_of_block = header + header->size - 1;
			printf("%p - %p : %zu bytes\n", header, end_of_block, end_of_block - header);
		}
		distance += header->size + 1;
		header += header->size + 1;
	}
	show_tiny_heap_usage_impl(heap->next);
}

static inline
void	show_tiny_heap_usage(const t_heap* heap)
{
	printf("TINY:\n");
	show_tiny_heap_usage_impl(heap);
}

void	show_alloc_mem(void)
{
	show_tiny_heap_usage(g_handlers[TINY].heap_list);
}
