#include "memory.h"

inline
int	is_allocated(const t_header* header)
{
	return header->size & 1;
}

inline
int	is_free(const t_header* header)
{
	return !is_allocated(header);
}

inline
void mark_allocated(t_header* header)
{
	header->size |= 1;
}

inline
void mark_free(t_header* header)
{
	if (is_free(header))
		return;
	header->size &= ~1;
	t_header* end = header + header->size - sizeof(t_header);
	end->size = header->size;
}

inline
size_t size_mask(size_t s)
{
	return s & ~1;
}

inline
int	ptr_cmp(const void* a, const void* b)
{
	if (a < b)
		return -1;
	return a > b;
}

inline
size_t	round_to_eight(size_t s)
{
	return (s + 7) & -8;
}
