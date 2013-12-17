#!/usr/bin/env bash

if test "$1" = "clean"  ; then
	make PIN_ROOT=pin clean
	rm -f test 
else
	# Just ignore the flag and do the normal build 
	make PIN_ROOT=pin
	gcc test.c -o test
	# Check for 32 or 64 bit system 
	if [ -d "obj-intel64" ] ; then
		OBJDIR="obj-intel64"
	else
		OBJDIR="obj-ia32"
	fi
	# Run Pin
	./pin/pin -t "$OBJDIR"/mallocmem.so -- ./test	
fi
