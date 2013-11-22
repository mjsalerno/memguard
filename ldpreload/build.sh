#!/bin/bash
# gcc -Wall -shared -fPIC -DPIC hook.c -ldl -o libhook.so
# gcc -Wall sample.c -o sample

gcc -Wall -fPIC -DPIC -c hook.c
ld -shared -o libhook.so hook.o -ldl

gcc -Wall -o sample sample.c

# LD_PRELOAD=./libhook.so ./sample
LD_PRELOAD=./libhook.so ./sample
