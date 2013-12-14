CPP = g++
CPPFLAGS = -Wall -Werror

all: utest

utest: utest.o memoryalloc.o
	$(CPP) $(CPPFLAGS) -o utest utest.o memoryalloc.o

utest.o: utest.cpp memoryalloc.h
	$(CPP) $(CPPFLAGS) -c utest.cpp

memoryalloc.o: memoryalloc.cpp memoryalloc.h
	$(CPP) $(CPPFLAGS) -c memoryalloc.cpp

clean:
	rm -f *.o utest
