# Function hooking via LD_PRELOAD
Description: This directory contains some sample code for hooking a function is libc
## Settings up the project
### Pre-reqs
1. Linux OS
2. gcc is installed
3. make installed
  using ubuntu you can install gcc using sudo apt-get install gcc or on fedora sudo yum install gcc
### Instructions
Once all the prereqs are aquired just run 
```bash
make run
```
This script will build `hook.so` and the executable `sample`
It will then run the program as so: `LD_PRELOAD=./libhook.so ./sample`

You should see some output similar to the block below:

```
paul@paul-UX32VD:~/Git/cse409/ldpreload$ make run
gcc -Wall -Werror -g  -fPIC -DPIC -rdynamic -c hook.c
ld -shared -o libhook.so hook.o -ldl
gcc -Wall -Werror -g  sample.c -o sample
LD_PRELOAD=./libhook.so ./sample
=== Attempting to hook `malloc`
=== `malloc` was successfully hooked.
=== Attempting to hook `free`
=== `free` was successfully hooked.
malloc(4) = 0x177c010
Freeing memory at address 0x177c010
```

## Viewing information about a binary
* `objdump -T libhook.so` or `objdump -T sample`
```
paul@paul-UX32VD:~/Git/cse409/ldpreload$ objdump -T libhook.so 

libhook.so:     file format elf64-x86-64

DYNAMIC SYMBOL TABLE:
00000000000004d0 l    d  .text	0000000000000000              .text
0000000000000000      D  *UND*	0000000000000000              printf
0000000000200a40 g    DO .bss	0000000000000008  Base        libc_malloc
0000000000000000      D  *UND*	0000000000000000              puts
00000000000005ba g    DF .text	0000000000000059  Base        malloc
0000000000200a48 g    DO .bss	0000000000000008  Base        libc_free
0000000000000662 g    DF .text	0000000000000012  Base        _init
0000000000000000      D  *UND*	0000000000000000              fprintf
0000000000200a40 g    D  .bss	0000000000000000  Base        __bss_start
0000000000000000      D  *UND*	0000000000000000              stderr
0000000000000000      DF *UND*	0000000000000000  GLIBC_2.2.5 dlerror
0000000000000000      DF *UND*	0000000000000000  GLIBC_2.2.5 dlsym
0000000000200a40 g    D  .got.plt	0000000000000000  Base        _edata
0000000000200a50 g    D  .bss	0000000000000000  Base        _end
0000000000000613 g    DF .text	000000000000004f  Base        free

```

* `nm libhook.so` or `nm sample`
```
paul@paul-UX32VD:~/Git/cse409/ldpreload$ nm libhook.so 
0000000000200a88 B __bss_start
                 U dlerror@@GLIBC_2.2.5
                 U dlsym@@GLIBC_2.2.5
00000000002008c0 d _DYNAMIC
0000000000200a88 D _edata
0000000000200a98 B _end
                 U fprintf
0000000000000613 T free
0000000000000545 t freeInit
0000000000200a48 d _GLOBAL_OFFSET_TABLE_
0000000000000662 T _init
0000000000200a90 B libc_free
0000000000200a88 B libc_malloc
00000000000005ba T malloc
00000000000004d0 t mallocInit
                 U printf
                 U puts
                 U stderr
```

* `ldd libhook.so` or `ldd sample`
```
paul@paul-UX32VD:~/Git/cse409/ldpreload$ ldd libhook.so 
	linux-vdso.so.1 =>  (0x00007fff8f1e4000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007fac49f6d000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fac49ba5000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fac4a38e000)
```

## Sources and References
1. [Let’s Hook a Library Function](http://www.linuxforu.com/2011/08/lets-hook-a-library-function/)
2. [Tutorial: Function Interposition in Linux](http://www.jayconrod.com/cgi/view_post.py?23)
3. [LD_PRELOAD for real world heap access tracking](http://blog.oakbits.com/index.php?post/2012/04/11/LD_PRELOAD-for-real-usage-on-memory-allocation)
4. [LD PRELOAD notes](http://www.noah.org/wiki/LD_PRELOAD_notes)
5. [dlsym(3)](http://linux.die.net/man/3/dlsym)
6. [What is the LD_PRELOAD trick?](http://stackoverflow.com/questions/426230/what-is-the-ld-preload-trick)
7. [3.2.2.10 Memory Allocation Hooks](http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html)
8. [Overriding 'malloc' using the LD_PRELOAD mechanism](http://stackoverflow.com/questions/6083337/overriding-malloc-using-the-ld-preload-mechanism)
