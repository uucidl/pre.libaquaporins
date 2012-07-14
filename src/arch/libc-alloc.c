#include <stdlib.h>

#include "../alloc.h"

extern void* aqp_malloc(int size)
{
	return malloc (size);
}

extern void* aqp_calloc(int count, int size)
{
	return calloc (count, size);
}

extern void* aqp_realloc(void* ptr, int size)
{
	return realloc (ptr, size);
}

extern void aqp_free(void* ptr)
{
	free (ptr);
}
