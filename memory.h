#pragma once

#include <sys/mman.h>
#include <stddef.h>

#include <stdio.h> // TODO: delete later 

#define TINY	0
#define	SMALL	1
#define	LARGE	2
#define TINY_HEAP_SIZE 4 * getpagesize()
#define SMALL_HEAP_SIZE 16 * getpagesize()
#define LARGE_HEAP_SIZE 0
// #define TINY_HEAP_SIZE getpagesize()
// #define SMALL_HEAP_SIZE TINY_HEAP_SIZE * 4
// #define LARGE_HEAP_SIZE 0

#define UNUSED	__attribute__ (( unused ))

// fuck ale 
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS 0xab0ba
#endif
int getpagesize();

typedef struct s_header {
	size_t size;
}	t_header;

typedef struct s_heap {
	size_t			size;
	struct s_heap*	next;
}	t_heap;

typedef struct s_mem_handler {
	t_heap*		heap_list;
	t_heap*		(*create_heap)(size_t);
	void		(*delete_heap)(t_heap*);
	void*		(*request)(size_t);
	void		(*release)(void*);
}	t_mem_handler;

extern t_mem_handler g_handlers[3];

void	init_heap_handlers()	__attribute__ (( constructor (101) ));
void	init_request_handlers()	__attribute__ (( constructor (102) ));
void	init_release_handlers()	__attribute__ (( constructor (103) ));

void	show_alloc_mem();

int		is_allocated(const t_header* header);
int		is_free(const t_header* header);
void	mark_allocated(t_header* header);
void	mark_free(t_header* header);
size_t	size_mask(size_t s);
size_t	round_to_eight(size_t s);
int		ptr_cmp(const void* a, const void* b);
