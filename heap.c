#include "heap.h"
#include "block.h"

t_mem_handler	g_handlers[3] = {0};

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
	h->size = size - HEAP_OFFSET;
	MARK_FREE(h);
	return heap;
}

static
t_heap* create_tiny_heap(size_t size) {
	(void)size;
	return create_heap_impl(TINY_HEAP_SIZE);
}

static
t_heap* create_small_heap(size_t size) {
	(void)size;
	return create_heap_impl(SMALL_HEAP_SIZE);
}

static
t_heap* create_large_heap(size_t size) {
	return create_heap_impl(size);
}

static
t_type_size_pair align_size_to_type(size_t size) {
	t_type_size_pair pair;
	if (size <= TINY_HEAP_SIZE) {
		pair.size = TINY_HEAP_SIZE;
		pair.type = TINY;
	}
	else if (size <= SMALL_HEAP_SIZE) {
		pair.size = SMALL_HEAP_SIZE;
		pair.type = LARGE;
	}
	else {
		pair.size = size;
		pair.type = LARGE;
	}
	return pair;
}

t_heap_type	get_type(size_t size) {
	if (size <= TINY_HEAP_SIZE)
		return TINY;
	else if (size <= SMALL_HEAP_SIZE)
		return SMALL;
	return LARGE;
}

static
t_heap* push_heap(t_heap** list, t_heap* new_heap) {
	if (!*list)
		return *list = new_heap;
	if (!(*list)->next)
		return (*list)->next = new_heap;
	return push_heap(&(*list)->next, new_heap);
}

t_heap*	create_heap(size_t req_size) {
	t_type_size_pair pair = align_size_to_type(req_size);
	t_mem_handler* hnr = &g_handlers[pair.type];
	return push_heap(&hnr->heap_list, hnr->create_heap(pair.size));
}

void	init_handlers() {
	g_handlers[TINY ].heap_list = NULL;
	g_handlers[TINY ].create_heap = create_tiny_heap;
	g_handlers[TINY ].request   = request_tiny_block;
	g_handlers[TINY ].release   = release_tiny_block;
	g_handlers[TINY ].get_type  = get_type;
	g_handlers[SMALL].heap_list = NULL;
	g_handlers[SMALL].request   = NULL;;
	g_handlers[SMALL].release   = NULL;
	g_handlers[SMALL].get_type  = get_type;
	g_handlers[LARGE].heap_list = NULL;;
	g_handlers[LARGE].request   = NULL;
	g_handlers[LARGE].release   = NULL;
	g_handlers[LARGE].get_type  = NULL;
}
