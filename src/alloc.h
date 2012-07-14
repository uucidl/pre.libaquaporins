#ifndef aqp_alloc_h
#define aqp_alloc_h

extern void* aqp_malloc(int size);
extern void* aqp_calloc(int count, int size);
extern void* aqp_realloc(void* ptr, int size);
extern void aqp_free(void* ptr);

#endif
