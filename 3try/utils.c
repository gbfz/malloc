#include "utils.h"

int	ptr_cmp(const void* a, const void* b)
{
	if (a < b)
		return -1;
	return a > b;
}

size_t	round_to_eight(size_t n)
{
	return (n + 7) & -8;
}

void*	get_block_footer(const void* h_ptr)
{
	const t_header* header = h_ptr;
	const size_t offset = header->size / sizeof(header->size);
	return (t_footer*)header + offset - sizeof(t_footer);
}
