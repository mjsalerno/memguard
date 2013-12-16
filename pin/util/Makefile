CPP = g++
CPPFLAGS = -g -Wall -Werror

all: utest

utest: utest.o memoryalloc.o memlist.o
	$(CPP) $(CPPFLAGS) -o utest utest.o memoryalloc.o memlist.o

utest.o: utest.cpp memoryalloc.h memlist.h
	$(CPP) $(CPPFLAGS) -c utest.cpp

memoryalloc.o: memoryalloc.cpp memoryalloc.h
	$(CPP) $(CPPFLAGS) -c memoryalloc.cpp

whitelist.o: memlist.cpp memlist.h
	$(CPP) $(CPPFLAGS) -c memlist.cpp	

clean:
	rm -f *.o utest
