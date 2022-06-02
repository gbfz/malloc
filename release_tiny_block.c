#include "heap.h"
#include "block.h"

static inline
free_tiny_header	locate_ptr_in_heap(const t_heap* heap, const void* ptr) {
	header h = (header)(heap + HEAP_OFFSET);
	ptr -= TINY_HEADER_OFFSET;
	while (ptr != h)
		h += h->block_size;
	return (free_tiny_header)h;
}

static inline
void	merge_with_next_chunk(free_tiny_header chunk) {
	if (chunk->next == NULL)
		return;
	free_tiny_header next_chunk = chunk->next;
	chunk->block_size += next_chunk->block_size;
	chunk->next = next_chunk->next;
	if (next_chunk->next)
		next_chunk->next->prev = chunk;
}

static inline
void	merge_with_prev_chunk(free_tiny_header chunk) {
	free_tiny_header prev_chunk = chunk->next;
	if (prev_chunk == NULL)
		return;
	prev_chunk->block_size += chunk->block_size;
	prev_chunk->next = chunk->next;
	if (chunk->next)
		chunk->next->prev = prev_chunk;
}

static inline
void	release_tiny_block_impl(t_heap* heap, void *ptr) {
	free_tiny_header chunk = locate_ptr_in_heap(heap, ptr);
	merge_with_next_chunk(chunk);
	merge_with_prev_chunk(chunk);
	MARK_FREE(chunk);
}

static inline
int	is_in_address_space(const t_heap* heap, const void *_ptr) {
	t_heap* ptr = (t_heap*)_ptr;
	return ptr >= heap && ptr <= (heap + heap->size);
}

inline
void	release_tiny_block(void* ptr) {
	t_heap* heap = g_handlers[TINY].heap_list;
	if (ptr == NULL || heap == NULL)
		return;
	while (heap != NULL && !is_in_address_space(heap, ptr))
		heap = heap->next;
	if (heap == NULL)
		return; // XXX BAD THING HAPPENED CRASH NOW!
	release_tiny_block_impl(heap, ptr);
}
