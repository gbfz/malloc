#include "heap.h"
#include "block.h"

#include <stdio.h> // XXX

// request block
// release block
// merge block

struct s_heap;
typedef struct s_heap t_heap;

static
void*	request_block_from_heap(t_heap* heap, int req_size) {
	header hd = (header)heap;
	int distance = 0;
	while (distance < heap->size && IS_ALLOCATED_BLOCK(hd)) {
		distance += hd->block_size;
		hd += hd->block_size;
	}
	if (distance == heap->size)
		return NULL;
	free_tiny_header thd = (free_tiny_header)hd;
	while (thd != NULL && thd->block_size < req_size)
		thd = thd->next;
	if (thd == NULL)
		return NULL;
	/* split, return */ return NULL;
}

void* request_block(int req_size) {
	t_heap* heap = /* request heap */ (t_heap *) 0xAB0BA;
}
