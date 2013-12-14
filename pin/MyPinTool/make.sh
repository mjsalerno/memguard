make PIN_ROOT=pin
gcc text.c -o test
./pin/pin -t obj-intel64/mallocmem.so -- ./test
