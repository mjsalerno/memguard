CC=gcc
CFLAGS= -Wall -Werror -g 

all: libhook.so sample

sample: sample.c
	$(CC) $(CFLAGS) sample.c -o sample

libhook.so: hook.o
	ld -shared -o libhook.so hook.o -ldl

hook.o: hook.c hook.h 
	$(CC) $(CFLAGS) -fPIC -DPIC -rdynamic -c hook.c

clean:
	rm -f *.o sample libhook.so

run: all
	LD_PRELOAD=./libhook.so ./sample

run-nohook: sample
	./sample