#ifndef HEAP_H
# define HEAP_H

# include <sys/mman.h>
# include <stddef.h>

# ifndef MAP_ANONYMOUS // Fuck Ale
#  define MAP_ANONYMOUS 0
# endif

int getpagesize();

# define PAGE_SIZE getpagesize()
# define TINY_HEAP_SIZE 4 * PAGE_SIZE
# define SMALL_HEAP_SIZE 16 * PAGE_SIZE

typedef enum heap_type {
	TINY,
	SMALL,
	LARGE
} t_heap_type;

typedef struct s_heap {
	struct s_heap	*next;
	int		size;
	// t_heap_type	type;
	// int		chunk_count;
} t_heap;

typedef struct s_type_size_pair {
	size_t size;
	t_heap_type type;
} t_type_size_pair;

typedef struct s_mem_request_handler {
	t_heap*	heap_list;
	// function pointer to look for properly sized free block
	// function pointer to create heap if ^ not found
} t_mem_request_handler;

/*
typedef struct s_heap_anchor {
	t_heap*	tiny;
	t_heap*	small;
	t_heap*	large;
} t_heap_anchor;

extern t_heap_anchor	g_heap_anchor;
*/

extern t_heap*	g_heap_anchor[3];

void	init_heap_anchor() __attribute__ ((constructor));

// # IDEA
// i really should think about the
// __attribute__s constructor and destructor...

// # STATIC
// t_heap*	create_heap(int size);
// t_heap*	append_heap(t_heap* heap);
// t_heap_type adjust_size_to_type(int* size);

// t_heap*	request_heap(int memory_request_size);
// int	release_heap(t_heap* heap);

#endif
