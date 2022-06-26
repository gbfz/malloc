#include "memory.h"

static inline
t_header*	fragment_block(t_header* block, size_t request_size)
{
	t_header* next_block = block + block->size;
	size_t old_size = block->size;

	block->size = request_size + sizeof(t_header);
	t_header* remainder = block + block->size;
	mark_allocated(block);

	if (ptr_cmp(remainder, next_block) < 0) {
		remainder->size = old_size - request_size;
		if (is_free(next_block))
			remainder->size += next_block->size;
		mark_free(remainder);
	}
	return block;
}

static inline
t_header*	seek_first_fit(const t_heap* heap, size_t request_size)
{
	t_header* heap_end = (t_header*)heap + heap->size;
	t_header* block = (t_header*)heap + sizeof(t_heap);
	while (ptr_cmp(block, heap_end) < 0 && block->size) {
		if (is_free(block)) {
			if (block->size + sizeof(t_header) >= request_size) {
				return block;
			}
		}
		block += size_mask(block->size);
	}
	return NULL;
}

static inline
t_header*	get_fit_block(t_heap* heap, size_t size)
{
	if (heap == NULL)
		return NULL;
	t_header* fit_block = seek_first_fit(heap, size);
	if (fit_block == NULL)
		return get_fit_block(heap->next, size);
	return fit_block;
}

static inline
void*	request_impl(t_mem_handler* handler, size_t size)
{
	size = round_to_eight(size);
	if (handler->heap_list == NULL) {
		if (handler->create_heap(size) == MAP_FAILED)
			return NULL;
	}
	t_header* fit_block = get_fit_block(handler->heap_list, size);
	if (fit_block == NULL) {
		if (handler->create_heap(size) == MAP_FAILED)
			return NULL;
		return request_impl(handler, size);
	}
	return fragment_block(fit_block, size) + sizeof(t_header);
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
