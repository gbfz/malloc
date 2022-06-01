#include "heap.h"

t_heap* g_heap_anchor[3];

static
t_heap*	create_heap_impl(size_t size) {
	t_heap* heap = mmap(NULL, size,
			    PROT_READ | PROT_WRITE,
			    MAP_PRIVATE | MAP_ANONYMOUS,
			    -1, 0);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	heap->next = NULL;
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

static
t_heap*	create_heap(t_type_size_pair pair) {
	// t_type_size_pair pair = align_size_to_type(size);
	t_heap* ptr = g_heap_anchor[pair.type];
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = create_heap_impl(pair.size);
	return ptr->next;
}

void	init_heap_anchor() {
	g_heap_anchor[TINY] = NULL;
	g_heap_anchor[SMALL] = NULL;
	g_heap_anchor[LARGE] = NULL;
}

/*
t_heap_anchor g_heap_anchor;

static
t_heap*	create_heap(size_t size) {
	t_heap* heap = mmap(NULL, size,
			    PROT_READ | PROT_WRITE,
			    MAP_PRIVATE | MAP_ANONYMOUS,
			    -1, 0);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	heap->next = NULL;
	return heap;
}

static
t_heap*	append_heap(t_heap_type type) {
	t_heap* typed_heap_ptr =
		(t_heap *)(&g_heap_anchor) + ((type - 1) * sizeof(t_heap*));
}

void	init_heap_anchor() {
	g_heap_anchor.tiny = NULL;
	g_heap_anchor.small = NULL;
	g_heap_anchor.large = NULL;
}
*/

/*
static
t_heap* append_heap(size_t size) {
	t_heap* typed_heap_ptr;
	if (size == TINY_HEAP_SIZE)
		typed_heap_ptr = g_heap_anchor.tiny;
	else if (size == SMALL_HEAP_SIZE)
		typed_heap_ptr = g_heap_anchor.small;
	else typed_heap_ptr = g_heap_anchor.large;
}
*/

/*
static t_heap*	create_tiny_heap() {
	t_heap* heap = mmap(NULL, TINY_HEAP_SIZE,
			    PROT_READ | PROT_WRITE,
			    MAP_PRIVATE | MAP_ANONYMOUS,
			    -1, 0);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	heap->next = NULL;
	return heap;
}

static t_heap*	create_small_heap() {
}

t_heap*	create_heap(size_t size) {
	if (size <= TINY_HEAP_SIZE)
		return create_tiny_heap();
	if (size <= SMALL_HEAP_SIZE)
		return create_small_heap();
}
*/

