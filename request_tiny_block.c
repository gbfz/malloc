#include "heap.h"
#include "block.h"

static inline
void*	fragment_block(t_free_tiny_header allocated_chunk, size_t req_size)
{
	t_free_tiny_header chunk = ((void*)allocated_chunk + req_size);
	chunk->prev_block_size = req_size;
	chunk->size = allocated_chunk->size - req_size;
	chunk->next = allocated_chunk->next;
	chunk->prev = allocated_chunk->prev;
	if (chunk->prev)
		chunk->prev->next = chunk;
	MARK_FREE(chunk);
	allocated_chunk->size = req_size;
	MARK_ALLOCATED(allocated_chunk);
	return allocated_chunk;
}

static inline
void*	request_tiny_block_impl(const t_heap* heap, size_t req_size)
{
	t_header chunk = (t_header)heap + HEAP_OFFSET;
	size_t distance = HEAP_OFFSET;
	while (distance < heap->size && IS_ALLOCATED_BLOCK(chunk)) {
		distance += chunk->size;
		chunk += chunk->size;
	}
	if (distance == heap->size)
		return NULL;
	t_free_tiny_header free_chunk = (t_free_tiny_header)chunk;
	while (free_chunk != NULL && free_chunk->size < req_size)
		free_chunk = free_chunk->next;
	if (!free_chunk)
		return NULL;
	void *ptr = fragment_block(free_chunk, req_size);
	return ptr + TINY_HEADER_OFFSET;
}

inline
void*	request_tiny_block(size_t req_size)
{
	ALIGN_8(req_size);
	t_heap*	heap = g_handlers[TINY].heap_list;
	void* mem = NULL;
	while (!mem && heap) {
		mem = request_tiny_block_impl(heap, req_size);
		heap = heap->next;
	}
	if (mem)
		return mem;
	t_heap* new_heap = create_heap(req_size);
	if (!new_heap)
		return NULL;
	return request_tiny_block_impl(new_heap, req_size);
}
