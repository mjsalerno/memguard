#define _GNU_SOURCE 
#include <dlfcn.h> /* dlsym() */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* mprotect(2) */
#include <malloc.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
/* function prototypes */
#include "hook.h"

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct sigaction sa;
int pagesize;

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
	// Set up mprotect
	sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        handle_error("sigaction");
    pagesize = sysconf(_SC_PAGE_SIZE);
    if (pagesize == -1)
        handle_error("sysconf");
}

/**
 * Function for handling violating page reads
 */
static void handler(int sig, siginfo_t *si, void *unused) {
    printf("Got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
    // exit(EXIT_FAILURE);
    /*if(mprotect(si->si_addr, pagesize, PROT_READ | PROT_WRITE) == -1)
        printf("An error occurred while trying to mark the page R+W\n");*/
    printf("R/W Allowed\n");
}


/**
 * Custom wrapper for malloc. 
 * Will track all allocated pointers and log lost memory.
 * @param size Amount of bytes to allocate.
 * @return Returns memory allocated by malloc if possible, else NULL.
 */
void *malloc(size_t size) {
	/* Calculate the number of pages to store */
	int pages = (size / pagesize);
	pages = pages > 0 ? pages : 1;
	/* calculate the total pages to allocate */
	// int total_pages = pages + 2;
	/* Total Bytes to allocate */
	int total_bytes = pages * pagesize;
	/* allocate the desired memory */
	void *ptr = memalign(pagesize, total_bytes);
	if (ptr == NULL)
		handle_error("memalign"); 
	/* Print out original malloc area */
	printf("--- Desired malloc(%zu bytes; %d pages) = %p\n", size, pages, ptr);
	/* Print out actual malloc information */ 
	printf("--- Actual malloc(%d bytes; %d pages) = %p\n", total_bytes, pages, ptr);
	/* Add protection to the start memory */
	if(mprotect(ptr, pagesize * pages, PROT_NONE) == -1)
        handle_error("PROT_NONE");
    /* Adjust the pointer */
    ptr = ptr + PADDING;
    /* increment the pointer to the correct location */ 
	printf("--- User starting address == %p\n", ptr);
	printf("--- User ending address == %p\n", ptr + size);
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
		/* TODO: This value is hard coded fix it! */
		ptr -= (PADDING - 8);
		printf("--- Freeing memory at address %p\n", ptr);
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
