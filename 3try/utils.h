#pragma once

#include "memory.h"

size_t	round_to_eight(size_t n);
int		ptr_cmp(const void* a, const void* b);

void*	get_block_footer(const void* header);
