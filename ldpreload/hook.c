#define _GNU_SOURCE 
#include <dlfcn.h> /* dlsym() */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hook.h"

/**
 * Hook the _init function.
 */
void _init(void) {
    if(libc_malloc == NULL) {
		mallocInit();
	}
	if(libc_free == NULL) {
		freeInit();
	}
}

/**
 * Custom wrapper for malloc. 
 * Will track all allocated pointers and log lost memory.
 * @param size Amount of bytes to allocate.
 * @return Returns memory allocated by malloc if possible, else NULL.
 */
void *malloc(size_t size) {
	void *ptr = libc_malloc(size);
	printf("malloc(%zu) = %p\n", size, ptr);
	return ptr;
}

/**
 * Custom wrapper for free.
 * Can detect free on null, unaligned free, free of an
 * already freed pointer (double free), and free of stack memory.
 * @param ptr The pointer to be deallocated.
 */
void free(void *ptr) {
	if(ptr != NULL) {
		printf("Freeing memory at address %p\n", ptr);
		libc_free(ptr);
	}
	/* Hooking free via LD_PRELOAD hooks NULL frees from unknown source*/
	/* Currently nothing is done to handle this */
}

/**
 * Stores the address of malloc into the function pointer.
 */
static void mallocInit(void) {
	printf("=== Attempting to hook `malloc`\n");
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    if (libc_malloc == NULL) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    } else {
    	printf("=== `malloc` was successfully hooked.\n");
    }
}

/**
 * Stores the address of free into the function pointer.
 */
static void freeInit(void) {
	printf("=== Attempting to hook `free`\n");
	libc_free = dlsym(RTLD_NEXT, "free");
	if(libc_free == NULL) {
		fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
	} else {
		printf("=== `free` was successfully hooked.\n");
	}
}
