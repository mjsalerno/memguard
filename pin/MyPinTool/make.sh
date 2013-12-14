make PIN_ROOT=pin
gcc test.c -o test
./pin/pin -t obj-intel64/mallocmem.so -- ./test
