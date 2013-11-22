#!/bin/bash
gcc -g -Wall -O0 sample.c -o sample
gcc -g -Wall -O0 -shared -fPIC -rdynamic hook.c -ldl -o libhook.so
LD_PRELOAD=./libhook.so ./sample
