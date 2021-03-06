#!/usr/bin/env bash
PIN_ROOT="../pin"

if test "$1" = "clean"  ; then
	make PIN_ROOT="$PIN_ROOT" clean
	rm -f test sample stacksmash control
elif test -z "$1" ; then
	# Just ignore the flag and do the normal build 
	make PIN_ROOT="$PIN_ROOT"
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
	# compile the program that uses all the functions correctly 
	gcc -g samples/control.c -o control
	if [[ $? -ne 0 ]]; then
		echo "control program failed to compile."
		exit 1
	fi
	# Check for 32 or 64 bit system 
	if [ -d "obj-intel64" ] ; then
		OBJDIR="obj-intel64"
	else
		OBJDIR="obj-ia32"
	fi
	# Run Pin
	echo -e "\n### Running contorl program test on ./control ###\n"
	"$PIN_ROOT"/pin.sh -t "$OBJDIR"/memguard.so -- ./control
	echo -e "\n### Appending control.log ###\n" > control.log
	cat stats.log >> control.log

	echo -e "\n### Running stack smashing detection test on ./stacksmash ###\n"
	"$PIN_ROOT"/pin.sh -t "$OBJDIR"/memguard.so -- ./stacksmash
	echo -e "\n### Appending stacksmash.log ###\n" > stacksmash.log
	cat stats.log >> stacksmash.log
	
	echo -e "\n### Running memory check detection test on ./test ###\n"
	"$PIN_ROOT"/pin.sh -t "$OBJDIR"/memguard.so -- ./test

else
	make PIN_ROOT="$PIN_ROOT"
	# Check to see if pin was successful
	if [[ $? -ne 0 ]]; then
		echo "pintool make failed."
		exit 1
	fi
	# Check for 32 or 64 bit system 
	if [ -d "obj-intel64" ] ; then
		OBJDIR="obj-intel64"
	else
		OBJDIR="obj-ia32"
	fi 
	"$PIN_ROOT"/pin.sh -t "$OBJDIR"/memguard.so -- $1
fi
