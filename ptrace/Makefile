CC=gcc
CFLAGS= -g -Wall -Werror

all: sample sample2 hw

sample: sample.o
	$(CC) $(CFLAGS) sample.o -o sample

sample.o: sample.c 
	$(CC) $(CFLAGS) -c sample.c

sample2: sample2.o
	$(CC) $(CFLAGS) sample2.o -o sample2

sample2.o:
	$(CC) $(CFLAGS) -c sample2.c

hw: hw.o
	ld hw.o -o hw

hw.o: hw.S
	gcc -c hw.S

clean:
	rm -f *.o sample sample2 hw
