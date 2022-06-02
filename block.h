#ifndef BLOCK_H
# define BLOCK_H

# include <stddef.h>

/* These macros are to be used
 * 	with the [block_size] field of the structs below.
 * The least significant bit
 * 	designates whether the block is free or allocated.
 * The second least significant bit designates ???????????????????
 * 	the type of heap that the block belongs to.
 */

// # define TINY_BLOCK(x)  ((x & 2) == 0)
// # define SMALL_BLOCK(x) ((x & 2) == 1)

# define IS_ALLOCATED_BLOCK(x) ((x->block_size & 1) == 1)
# define IS_FREE_BLOCK(x) ((x->block_size & 1) == 0)

# define MARK_ALLOCATED(x) (x->block_size |= 1)
# define MARK_FREE(x) (x->block_size &= ~1)

struct __header {
	size_t prev_block_size;
	size_t block_size;
};

struct __free_tiny_header {
	size_t prev_block_size;
	size_t block_size;
	struct __free_tiny_header* prev;
	struct __free_tiny_header* next;
};

struct __free_small_header {
	size_t prev_block_size;
	size_t block_size;
	struct __free_small_header* left;
	struct __free_small_header* right;
	struct __free_small_header* mom;
};

typedef struct __header* header;
typedef struct __header* alloc_header;
typedef struct __free_tiny_header* free_tiny_header;
typedef struct __free_small_header* free_small_header;

# define HEADER_OFFSET sizeof(struct __header)
# define TINY_HEADER_OFFSET sizeof(struct __free_tiny_header)
# define SMALL_HEADER_OFFSET sizeof(struct __free_small_header)

void*	request_tiny_block(size_t req_size);
void	release_tiny_block(void* ptr);



#endif
