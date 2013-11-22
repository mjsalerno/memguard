#define _GNU_SOURCE 
#include <dlfcn.h> /* dlsym() */
#include <stdio.h>
#include <stdint.h>
#include "hook.h"

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

/**
 * Custom wrapper for malloc. 
 * Will track all allocated pointers and log lost memory.
 * @param size Amount of bytes to allocate.
 * @return Returns memory allocated by malloc if possible, else NULL.
 */
void *malloc(size_t size) {
	if(libc_malloc == NULL) {
		mallocInit();
	}
	void *p = libc_malloc(size);
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
	if(libc_free == NULL) {
		freeInit();
	}
	if(ptr != NULL) {
		printf("Free memory ------------- address @ %p\n", ptr);
	} else {
		printf("Attempting to free NULL!\n");
	}
}

/**
 * Hook the init function.
 */
void _init(void) {
  printf("=== Hooking init ===\n");
}