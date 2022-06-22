#pragma once

#define ALIGN_AS(x)	__attribute__ (( aligned (x) ))
#define UNUSED		__attribute__ (( unused ))

#define HEAP_OFFSET	  sizeof(t_heap)
#define HEADER_OFFSET sizeof(t_header)
#define FOOTER_OFFSET sizeof(t_footer)

#define SIZE_MASK(x) (x & ~1)

#define IS_FREE(x)				((x->size & 1) == 0)
#define IS_ALLOCATED(x)			((x->size & 1) == 1)
#define IS_PREV_FREE(x)			((x->size & 2) == 0)
#define IS_PREV_ALLOCATED(x)	((x->size & 2) == 1)

#define MARK_ALLOCATED(x)		(x->size |= 1)
#define MARK_FREE(x)			(x->size &= ~1)
#define MARK_PREV_ALLOCATED(x)	(x->size |= 2)
#define MARK_PREV_FREE(x)		(x->size &= ~2)
// #define MARK_END(x)			(x->size |= 3)

// fuck ale 
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS 0xBAD
#endif
int getpagesize();

#define TINY_HEAP_SIZE 4 * getpagesize()
#define SMALL_HEAP_SIZE 16 * getpagesize()
#define LARGE_HEAP_SIZE 0

#define GET_FOOTER_ADDRESS(x) (void*)x + x->size - FOOTER_OFFSET;
