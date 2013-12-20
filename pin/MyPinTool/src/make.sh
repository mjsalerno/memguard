#!/usr/bin/env bash
if test "$1" = "clean"  ; then
	make PIN_ROOT=pintool clean
	rm -f test sample
else
	# Just ignore the flag and do the normal build 
	make PIN_ROOT=pintool
	# Check to see if pin was successful
	if [[ $? -ne 0 ]]; then
		echo "pintool make failed."
		exit 1
	fi
	# compile the test program
	gcc -g samples/test.c -o test
	# Check to see if the test program compiled successfully
	if [[ $? -ne 0 ]]; then
		echo "test program failed to compile."
		exit 1
	fi
	# compile the stack smashing example
	gcc -g -fno-stack-protector samples/stacksmash.c -o stacksmash
	if [[ $? -ne 0 ]]; then
		echo "stacksmash program failed to compile."
		exit 1
	fi
	# Check for 32 or 64 bit system 
	if [ -d "obj-intel64" ] ; then
		OBJDIR="obj-intel64"
	else
		OBJDIR="obj-ia32"
	fi
	# Run Pin
	echo -e "\n### Running stack smashing detection test on ./stacksmash ###\n"
	./pintool/pin.sh -t "$OBJDIR"/mallocmem.so -- ./stacksmash
	echo -e "\n### Appending stats.log ###\n" > stacksmash.log
	cat stats.log >> stacksmash.log
	echo -e "\n### Running memory check detection test on ./test ###\n"
	./pintool/pin.sh -t "$OBJDIR"/mallocmem.so -- ./test	
fi
