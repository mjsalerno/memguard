#define _GNU_SOURCE 
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h> /* dlsym() */
#include "hook.h"

/**
 * Stores the address of malloc into the function pointer.
 */
static void mallocInit(void) {
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    if (original_malloc == NULL) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    }
}

/**
 * Stores the address of free into the function pointer.
 */
static void freeInit(void) {
	original_free = dlsym(RTLD_NEXT, "free");
	if(original_free == NULL) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	}
}

/**
 * Custom wrapper for malloc. 
 * Will track all allocated pointers and log lost memory.
 * @param size Amount of bytes to allocate.
 * @return Returns memory allocated by malloc if possible, else NULL.
 */
void *malloc(size_t size) {
	if(original_malloc == NULL) {
		mallocInit();
	}
	void *p = original_malloc(size);
	printf("malloc(%zd) = %p\n", size, p);
	return p;
}

/**
 * Custom wrapper for free.
 * Can detect free on null, unaligned free, free of an
 * already freed pointer (double free), and free of stack memory.
 * @param ptr The pointer to be deallocated.
 */
void free(void *ptr) {
	if(original_free == NULL) {
		freeInit();
	}
	original_free(ptr);
	printf("free(%p)", ptr);
}