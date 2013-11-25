#ifndef HOOK_H
#define HOOK_H
#define PADDING 256
/* Location to store the original malloc function pointer */
static void* (*libc_malloc)(size_t size) = NULL;
/* Location to store the original free function pointer */
static void (*libc_free)(void* ptr) = NULL;
/* Function for initializing and storing the function pointer of the original malloc */
static void mallocInit(void);
/* Function for initializing and storing the function pointer of the original free */
static void freeInit(void);
/* Custom wrapper function for malloc */
void* malloc(size_t size);
void free(void* ptr);
/* mprotect */
static void handler(int sig, siginfo_t *si, void *unused);
#endif