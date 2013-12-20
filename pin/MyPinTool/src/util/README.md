# Pin Utility classes
Description: This directory contains the MemList class, the node class MemoryAlloc for tracking memory allocations, and the Stats class.

## Pre-reqs
1. g++ is installed.
2. make is installed.

## Using the MemList class
1. include "memlist.h" in the project you want to use the MemList class in.

## Using the Stats class
1. include "stats.h" in the project you want to use the Stats class in.

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
MemoryAlloc[address = 0xda7010 : underflowFence = 0xda6f10 : overflowFence = 0xda7048 : userSize = 56 : fenceSize = 256 : totalSize = 568]
MemoryAlloc[address = 0xda7050 : underflowFence = 0xda6f50 : overflowFence = 0xda70b6 : userSize = 102 : fenceSize = 256 : totalSize = 614]
=== MemList size() test passed.
=== MemList add(address, size) test passed.
=== MemList remove(int index) and isEmpty() test passed.
=== MemList add(MemoryAlloc alloc) test passed.
=== MemList containsAddress(void *address) test passed.
=== MemList get(int index) test passed.
=== MemList removeMatching(void* address) test passed.
=== MemList removeMatching(MemoryAlloc &alloc) test passed.
=== MemList clear() test passed.
paul@paul-UX32VD:~/Git/cse409/pin/util$ 
```

If something went wrong you will something something like:
```bash
paul@paul-UX32VD:~/Git/cse409/pin/util$ make clean && make
rm -f *.o utest
g++ -g -Wall -Werror -c utest.cpp
g++ -g -Wall -Werror -c memoryalloc.cpp
g++ -g -Wall -Werror -c memlist.cpp	
g++ -g -Wall -Werror -o utest utest.o memoryalloc.o memlist.o
paul@paul-UX32VD:~/Git/cse409/pin/util$ ./utest 
=== MemoryAlloc getSize() test passed.
=== MemoryAlloc getAddress() test passed.
=== Testing the print function
MemoryAlloc[address = 0xda7010 : underflowFence = 0xda6f10 : overflowFence = 0xda7048 : userSize = 56 : fenceSize = 256 : totalSize = 568]
MemoryAlloc[address = 0xda7050 : underflowFence = 0xda6f50 : overflowFence = 0xda70b6 : userSize = 102 : fenceSize = 256 : totalSize = 614]
=== MemList size() test passed.
=== MemList add(address, size) test passed.
=== MemList remove(int index) and isEmpty() test passed.
=== MemList add(MemoryAlloc alloc) test passed.
utest: utest.cpp:75: void memListTests(): Assertion `index != -1' failed.
Aborted (core dumped)
paul@paul-UX32VD:~/Git/cse409/pin/util$ 
```