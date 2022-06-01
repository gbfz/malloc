#ifndef BLOCK_H
# define BLOCK_H

/* These macros are to be used
 * 	with the [block_size] field of the structs below.
 * The least significant bit
 * 	designates whether the block is free or allocated.
 * The second least significant bit designates ???????????????????
 * 	the type of heap that the block belongs to.
 */
// # define FREE_BLOCK(x) ((x & 1) == 0)
// # define ALLOCATED_BLOCK(x) ((x & 1) == 1)

// # define TINY_BLOCK(x)  ((x & 2) == 0)
// # define SMALL_BLOCK(x) ((x & 2) == 1)

# define IS_FREE_BLOCK(x) ((x->block_size & 1) == 0)
# define IS_ALLOCATED_BLOCK(x) ((x->block_size & 1) == 1)

struct __header {
	int prev_block_size;
	int block_size;
};
typedef struct __header* header;
typedef struct __header* alloc_header;

struct __free_tiny_header {
	int prev_block_size;
	int block_size;
	struct __free_tiny_header* prev;
	struct __free_tiny_header* next;
};
typedef struct __free_tiny_header* free_tiny_header;

struct __free_small_header {
	int prev_block_size;
	int block_size;
	struct __free_small_header* left;
	struct __free_small_header* right;
	struct __free_small_header* mom;
};
typedef struct __free_small_header* free_small_header;

/* Is this unreadable?
 * I'm sort of hiding the structs themselves from clients.
 * Only the pointers are to be used.
 */

#endif
