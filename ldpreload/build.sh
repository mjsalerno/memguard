#!/bin/bash
gcc sample.c -o sample
gcc -shared -fPIC -rdynamic hook.c -ldl -o libhook.so
LD_PRELOAD=./libhook.so ./sample
