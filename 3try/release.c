#include "memory.h"
#include <stdbool.h>

static inline
void	release_impl(const t_mem_handler* handler, void* ptr)
{
	(void)handler; (void)ptr;
	/*
	if (ptr == NULL)
		return;
	t_header* block = ptr;
	t_footer* footer = &block[-1];
	t_header* next_block = &block[block->size];
	if (IS_FREE(next_block)) {
	}
	*/
}

static inline
void	tiny_release(void* ptr)
{
	release_impl(&g_handlers[TINY], ptr);
}

static inline
void	small_release(void* ptr)
{
	release_impl(&g_handlers[SMALL], ptr);
}

static inline
void	large_release(void* ptr)
{
	g_handlers[LARGE].delete_heap(ptr);
}

void	init_release_handlers()
{
	g_handlers[TINY ].release = tiny_release;
	g_handlers[SMALL].release = small_release;
	g_handlers[LARGE].release = large_release;
}
