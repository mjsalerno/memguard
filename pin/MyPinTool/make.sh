#!/usr/bin/env bash

if test "$1" = "clean"  ; then
	make PIN_ROOT=pin clean
	rm -f test 
else
	# Just ignore the flag and do the normal build 
	make PIN_ROOT=pin
	# Check to see if pin was successful
	if [ "$?" = "1" ]
		echo "pintool make failed."
		exit 1
	fi
	# compile the test program
	gcc test.c -o test
	# Check to see if the test program compiled successfully
	if [ "$?" = "1" ]
		echo "test program failed to compile."
		exit 1
	fi
	# Check for 32 or 64 bit system 
	if [ -d "obj-intel64" ] ; then
		OBJDIR="obj-intel64"
	else
		OBJDIR="obj-ia32"
	fi
	# Run Pin
	./pin/pin -t "$OBJDIR"/mallocmem.so -- ./test	
fi
