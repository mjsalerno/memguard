# Function hooking via LD_PRELOAD
Description: This directory contains some sample code for hooking a function is libc
## Settings up the project
### Pre-reqs
1. Linux OS
2. gcc is installed
⋅⋅ using ubuntu you can install gcc using sudo apt-get install gcc or on fedora sudo yum install gcc
### Instructions
Once all the prereqs are aquired just run 
```bash
./build.sh
```
This script will build `hook.so` and the executable `sample`
It will then run the program as so: `LD_PRELOAD=./libhook.so ./sample`

You should see some output similar to the block below:

```
paul@paul-UX32VD:~/Documents/school/fall2013/cse409/project$ ./build.sh 
Calling from main...
inside shared object..
malloc(12) = 0x18b1010 
displaying memory leaks.. 
returning from shared object.. 
returning to main.. 
freeing memory..
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


