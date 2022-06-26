#include "memory.h"

static inline
void	show_alloc_mem_impl(const t_heap* heap)
{
	if (heap == NULL)
		return;
	const t_header* heap_end = (t_header*)heap + heap->size;
	const t_header* block = (t_header*)heap + sizeof(t_heap);
	while (ptr_cmp(block, heap_end) < 0 && block->size) {
		if (is_allocated(block)) {
			const t_header* block_end = block + size_mask(block->size);
			printf("%p - %p : %zu\n", block, block_end, block_end - block);
		}
		block += size_mask(block->size);
	}
	show_alloc_mem_impl(heap->next);
}

void	show_alloc_mem()
{
	printf("TINY : %p\n", g_handlers[TINY].heap_list);
	show_alloc_mem_impl(g_handlers[TINY].heap_list);
	// printf("SMALL : %p\n", g_handlers[SMALL].heap_list);
	// show_alloc_mem_impl(g_handlers[SMALL].heap_list);
	// printf("LARGE : %p\n", g_handlers[LARGE].heap_list);
	// show_alloc_mem_impl(g_handlers[LARGE].heap_list);
}
