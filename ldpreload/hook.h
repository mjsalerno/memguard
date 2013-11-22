#ifndef HOOK_H
#define HOOK_H
/* Location to store the original malloc function pointer */
void* (*libc_malloc)(size_t size) = NULL;
/* Location to store the original free function pointer */
void (*libc_free)(void* ptr) = NULL;
/* Function for initializing and storing the function pointer of the original malloc */
static void mallocInit(void);
/* Function for initializing and storing the function pointer of the original free */
static void freeInit(void);
/* Custom wrapper function for malloc */
void* malloc(size_t size);
void free(void* ptr);
#endif