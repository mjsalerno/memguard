#!/usr/bin/env bash

if test "$1" = "clean"  ; then
	make PIN_ROOT=pin clean
else
	# Just ignore the flag and do the normal build 
	make PIN_ROOT=pin
	gcc test.c -o test
	./pin/pin -t obj-intel64/mallocmem.so -- ./test	
fi
