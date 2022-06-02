#include "heap.h"
#include "block.h"

t_mem_handler	g_handlers[3];

static
t_heap*	create_heap_impl(size_t size) {
	t_heap* heap = mmap(NULL, size,
			    PROT_READ | PROT_WRITE,
			    MAP_PRIVATE | MAP_ANONYMOUS,
			    -1, 0);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	heap->next = NULL;
	heap->size = size;
	header h = (header)heap + HEAP_OFFSET;
	h->prev_block_size = 0;
	h->block_size = size - HEAP_OFFSET;
	MARK_FREE(h);
	return heap;
}

static
t_type_size_pair align_size_to_type(size_t size) {
	t_type_size_pair pair;

	if (size <= TINY_HEAP_SIZE) {
		pair.size = TINY_HEAP_SIZE;
		pair.type = TINY;
	} else if (size <= SMALL_HEAP_SIZE) {
		pair.size = SMALL_HEAP_SIZE;
		pair.type = LARGE;
	} else {
		pair.size = size;
		pair.type = LARGE;
	}
	return pair;
}

t_heap*	create_heap(size_t req_size) {
	t_type_size_pair pair = align_size_to_type(req_size);
	t_heap* ptr = g_handlers[pair.type].heap_list;
	if (!ptr) {
		g_handlers[pair.type].heap_list =
			create_heap_impl(TINY_HEAP_SIZE);
		return g_handlers[pair.type].heap_list;
	}
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = create_heap_impl(pair.size);
	return ptr->next;
}

void	init_handlers() {
	g_handlers[TINY].heap_list = NULL;
	g_handlers[TINY].request = request_tiny_block;
	g_handlers[TINY].release = release_tiny_block;
	g_handlers[SMALL].heap_list = NULL;
	g_handlers[LARGE].heap_list = NULL;
}
