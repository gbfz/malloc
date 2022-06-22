#include "memory.h"
#include "utils.h"

static inline
void	show_alloc_mem_impl(const t_heap* heap)
{
	if (!heap)
		return;
	const t_heap* heap_end = heap + heap->size;
	const t_header* block = (t_header*)heap + HEAP_OFFSET;
	while (ptr_cmp(block, heap_end) == -1 && block->size) {
		if (IS_ALLOCATED(block)) {
			const t_header* block_end = block + SIZE_MASK(block->size);
			/* XXX i can't use printf, right? */
			// printf("block size : %zu\n", SIZE_MASK(block->size));
			printf("%p - %p : %zu\n", block, block_end, block_end - block);
		}
		block += SIZE_MASK(block->size);
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
