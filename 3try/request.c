#include "memory.h"
#include "utils.h"

static inline
void*	fragment_block(t_header* block, size_t req_size)
{
	size_t old_block_size = SIZE_MASK(block->size);
	t_header* next_block = (t_header*)block + old_block_size;

	block->size = req_size;
	MARK_ALLOCATED(block);

	/*
	t_footer* block_footer = (t_footer*)block + req_size - FOOTER_OFFSET;
	block_footer->prev_size = req_size;

	t_header* rem = (t_header*)block_footer + FOOTER_OFFSET;
	if (ptr_cmp(rem + HEADER_OFFSET, next_block) == -1) {
		rem->size = old_block_size - req_size;
		MARK_FREE(rem);
		t_footer* rem_footer = (t_footer*)rem + rem->size - FOOTER_OFFSET;
		rem_footer->prev_size = rem->size;
	}
	*/
	return block;
}

static inline
void*	seek_first_fit(const t_heap* heap, size_t size)
{
	t_header* block = (t_header*)heap + HEAP_OFFSET;
	t_footer* end_of_heap = (t_footer*)heap + heap->size;
	while (ptr_cmp(block, end_of_heap) == -1 && block->size) {
		if (IS_FREE(block)) {
			size_t block_size = SIZE_MASK(block->size);
			if (block_size + HEADER_OFFSET + FOOTER_OFFSET >= size)
				return block;
		}
		block += SIZE_MASK(block->size);
	}
	return NULL;
}

static inline
void*	request_impl(t_mem_handler* handler, size_t size)
{
	size = round_to_eight(size);
	if (handler->heap_list == NULL) {
		if (MAP_FAILED == handler->create_heap(handler->heap_size_type ?: size))
			return NULL;
	}
	t_heap* heap = handler->heap_list;
	t_header* fitting_block = seek_first_fit(heap, size);
	while (!fitting_block && heap != NULL) {
		fitting_block = seek_first_fit(heap, size);
		heap = heap->next;
	}
	if (!fitting_block) {
		if (MAP_FAILED == handler->create_heap(handler->heap_size_type ?: size))
			return NULL;
		return request_impl(handler, size);
	}
	fitting_block = fragment_block(fitting_block, size);
	return fitting_block + HEADER_OFFSET;
}

static inline
void*	tiny_request(size_t size)
{
	return request_impl(&g_handlers[TINY], size);
}

static inline
void*	small_request(size_t size)
{
	return request_impl(&g_handlers[SMALL], size);
}

static inline
void*	large_request(size_t size)
{
	return g_handlers[LARGE].create_heap(size);
}

void	init_request_handlers()
{
	g_handlers[TINY].request  = tiny_request;
	g_handlers[SMALL].request = small_request;
	g_handlers[LARGE].request = large_request;
}
