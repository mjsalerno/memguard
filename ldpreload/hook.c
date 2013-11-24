#define _GNU_SOURCE 
#include <dlfcn.h> /* dlsym() */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <execinfo.h>
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
		libc_free(ptr);
	}
	/* Hooking free via LD_PRELOAD hooks NULL frees from unknown source*/
	/* Currently nothing is done to handle this */

	/* temp backtrace calls */
	void *buffer[100];
    char **strings;
    int j;
	int nptrs = backtrace(buffer, 100);
    // printf("backtrace() returned %d addresses\n", nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

   for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);
   libc_free(strings);
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
