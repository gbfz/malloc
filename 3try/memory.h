#pragma once

#include <sys/mman.h>
#include <stddef.h>
#include "macros.h"

#include <stdio.h> // TODO: remove later 

typedef enum e_heap_type
{
	TINY,
	SMALL,
	LARGE
}	t_heap_type;

typedef struct /*ALIGN_AS(8)*/ s_header
{
	size_t size;
}	t_header;

typedef struct /*ALIGN_AS(8)*/ s_footer
{
	size_t prev_size;
}	t_footer;

typedef struct /*ALIGN_AS(8)*/ s_heap
{
	size_t			size;
	struct s_heap*	next;
}	t_heap;

typedef struct s_mem_handler
{
	size_t		heap_size_type;
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

t_heap_type	get_heap_type(size_t size);

void	show_alloc_mem();
