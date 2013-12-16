#!/usr/bin/env bash

if [ -d "obj-intel64" ] ; then
	OBJDIR="obj-intel64"
else
	OBJDIR="obj-ia32"
fi


if test "$1" = "clean"  ; then
	make PIN_ROOT=pin clean
else
	# Just ignore the flag and do the normal build 
	make PIN_ROOT=pin
	gcc test.c -o test
	./pin/pin -t "$OBJDIR"/mallocmem.so -- ./test	
fi
