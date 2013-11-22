#define _GNU_SOURCE 
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h> /* dlsym() */

void *malloc(size_t size) {
	static void *(*my_malloc)(size_t) = NULL;
	printf("inside shared object..\n");
	if(!my_malloc) {
		my_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	void *p = my_malloc(size);
	printf("malloc(%zd) = %p\n", size, p);
	lcheck();
	printf("returning from shared object..\n");
	return p;
}