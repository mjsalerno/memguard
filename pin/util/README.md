# Pin Utility classes
Description: This directory contains the WhiteList class and the node class MemoryAlloc for tracking memory allocations.

## Pre-reqs
1. g++ is installed.
2. make is installed.

## Using the MemList class
1. include "memlist.h" in the project you want to use the MemList class in.

## Verifying MemList works
1. Navigate to cse409/pin/util
2. type ``` make clean && make ```
3. type ``` ./utest ```

If all went correctly then you should see something similar to the following:

```bash
paul@paul-UX32VD:~/Git/cse409/pin/util$ ./utest 
=== MemoryAlloc getSize() test passed.
=== MemoryAlloc getAddress() test passed.
=== Testing the print function
MemoryAlloc[address = 0x1d31010 : size = 56 bytes]
MemoryAlloc[address = 0x1d31050 : size = 102 bytes]
=== WhiteList size() test passed.
=== WhiteList add(address, size) test passed.
=== WhiteList remove(int index) and isEmpty() test passed.
=== WhiteList add(MemoryAlloc alloc) test passed.
=== WhiteList containsAddress(void *address) test passed.
=== WhiteList get(int index) test passed.
=== WhiteList removeMatching(void* address) test passed.
=== WhiteList removeMatching(MemoryAlloc &alloc) test passed.
=== WhiteList clear() test passed.
paul@paul-UX32VD:~/Git/cse409/pin/util$ 
```

If something went wrong you will something something like:
```bash
paul@paul-UX32VD:~/Git/cse409/pin/util$ make clean && make
rm -f *.o utest
g++ -g -Wall -Werror -c utest.cpp
g++ -g -Wall -Werror -c memoryalloc.cpp
g++ -g -Wall -Werror -c whitelist.cpp	
g++ -g -Wall -Werror -o utest utest.o memoryalloc.o whitelist.o
paul@paul-UX32VD:~/Git/cse409/pin/util$ ./utest 
=== MemoryAlloc getSize() test passed.
=== MemoryAlloc getAddress() test passed.
=== Testing the print function
MemoryAlloc[address = 0x15d9010 : size = 56 bytes]
MemoryAlloc[address = 0x15d9050 : size = 102 bytes]
=== WhiteList size() test passed.
=== WhiteList add(address, size) test passed.
=== WhiteList remove(int index) and isEmpty() test passed.
=== WhiteList add(MemoryAlloc alloc) test passed.
utest: utest.cpp:75: void whiteListTests(): Assertion `index != -1' failed.
Aborted (core dumped)
paul@paul-UX32VD:~/Git/cse409/pin/util$ 
```