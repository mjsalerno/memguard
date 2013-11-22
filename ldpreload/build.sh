#!/bin/bash
# gcc -Wall -shared -fPIC -DPIC hook.c -ldl -o libhook.so
# gcc -Wall sample.c -o sample

# Create the shared library
gcc -Wall -fPIC -DPIC -rdynamic -c hook.c
ld -shared -o libhook.so hook.o -ldl
# Build the sample program
gcc -Wall -o sample sample.c
# Run the program with LD_PRELOAD
LD_PRELOAD=./libhook.so ./sample
