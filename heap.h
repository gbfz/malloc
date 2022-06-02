#ifndef HEAP_H
# define HEAP_H

# include <sys/mman.h>
# include <stddef.h>

# ifndef MAP_ANONYMOUS // Fuck Ale
#  define MAP_ANONYMOUS 0
# endif

int getpagesize();

# define PAGE_SIZE getpagesize()
# define TINY_HEAP_SIZE PAGE_SIZE
# define SMALL_HEAP_SIZE 16 * PAGE_SIZE

typedef enum heap_type {
	TINY, SMALL, LARGE
} t_heap_type;

typedef struct s_heap {
	struct s_heap	*next;
	size_t		size;
} t_heap;

typedef struct s_type_size_pair {
	size_t size;
	t_heap_type type;
} t_type_size_pair;

typedef struct s_mem_request_handler {
	t_heap*	heap_list;
	void*	(*request)(size_t);
	void	(*release)(void*);
	// func pointer to create heap
} t_mem_handler;

# define HEAP_OFFSET sizeof(t_heap)

extern t_mem_handler	g_handlers[3];

t_heap*	create_heap(size_t req_size);
t_heap* get_heap(size_t req_size);

void	init_handlers() __attribute__ ((constructor));
// munmaping destructor

#endif
