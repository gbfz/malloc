#include "memory.h"
#include "utils.h"

static inline
t_heap*	create_heap_impl(size_t size)
{
	t_heap* heap = mmap(NULL, size,
						PROT_READ | PROT_WRITE,
						MAP_PRIVATE | MAP_ANONYMOUS,
						-1, 0);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	heap->next = NULL;
	heap->size = size;
	t_header* free_mem = (t_header*)heap + HEAP_OFFSET;
	free_mem->size = heap->size - HEAP_OFFSET;
	MARK_FREE(free_mem);
	t_header* heap_end = (t_header*)heap + (heap->size / sizeof(heap->size)) - sizeof(t_header);
	heap_end->size = 0;
	MARK_END(heap_end);
	return heap;
}

static inline
t_heap*	push_heap(t_heap** list, t_heap* heap)
{
	heap->next = *list;
	*list = heap;
	return heap;
}

static inline
t_heap*	create_tiny_heap(UNUSED size_t size)
{
	printf("Creating tiny heap...\n");
	t_heap* heap = create_heap_impl(TINY_HEAP_SIZE);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	return push_heap(&g_handlers[TINY].heap_list, heap);
}

static inline
t_heap*	create_small_heap(UNUSED size_t size)
{
	printf("Creating small heap...\n");
	t_heap* heap = create_heap_impl(SMALL_HEAP_SIZE);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	return push_heap(&g_handlers[SMALL].heap_list, heap);
}

static inline
t_heap*	create_large_heap(size_t size)
{
	size = round_to_eight(size);
	t_heap* heap = create_heap_impl(size);
	if (heap == MAP_FAILED)
		return MAP_FAILED;
	return push_heap(&g_handlers[LARGE].heap_list, heap);
}

static inline
void	delete_heap_impl(t_heap** list, t_heap* heap)
{
	if (!*list)
		munmap(NULL, 0xDEAD); // crash ?
	if (*list == heap) {
		munmap(heap, heap->size);
		*list = NULL;
	}
	else delete_heap_impl(&(*list)->next, heap);
}

static inline
void	delete_tiny_heap(t_heap* heap)
{
	delete_heap_impl(&g_handlers[TINY].heap_list, heap);
}

static inline
void	delete_small_heap(t_heap* heap)
{
	delete_heap_impl(&g_handlers[SMALL].heap_list, heap);
}

static inline
void	delete_large_heap(t_heap* heap)
{
	delete_heap_impl(&g_handlers[LARGE].heap_list, heap);
}

/*
t_heap_type	get_heap_type(size_t size)
{
	if (size <= TINY_HEAP_SIZE)
		return TINY;
	if (size <= SMALL_HEAP_SIZE)
		return SMALL;
	return LARGE;
}
*/

t_mem_handler	g_handlers[3] = {0};

void	init_heap_handlers()
{
	t_mem_handler*	tiny  = &g_handlers[TINY];
	UNUSED /* YET */ t_mem_handler*	small = &g_handlers[SMALL];
	UNUSED /* YET */ t_mem_handler*	large = &g_handlers[LARGE];

	tiny->heap_list = NULL;
	tiny->heap_size_type = TINY_HEAP_SIZE;
	tiny->create_heap = create_tiny_heap;
	tiny->delete_heap = delete_tiny_heap;

	small->heap_list = NULL;
	small->heap_size_type = SMALL_HEAP_SIZE;
	small->create_heap = create_small_heap;
	small->delete_heap = delete_small_heap;

	large->heap_list = NULL;
	large->heap_size_type = LARGE_HEAP_SIZE;
	large->create_heap = create_large_heap;
	large->delete_heap = delete_large_heap;
	large->request = (void* (*)(size_t))create_large_heap; //
	large->release = (void (*)(void*))delete_large_heap;   // I should really test whether that's fine...
}
