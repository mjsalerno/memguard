CPP = g++
CPPFLAGS = -Wall -Werror

all: utest

utest: utest.o memoryalloc.o whitelist.o
	$(CPP) $(CPPFLAGS) -o utest utest.o memoryalloc.o whitelist.o

utest.o: utest.cpp memoryalloc.h whitelist.h
	$(CPP) $(CPPFLAGS) -c utest.cpp

memoryalloc.o: memoryalloc.cpp memoryalloc.h
	$(CPP) $(CPPFLAGS) -c memoryalloc.cpp

whitelist.o: whitelist.cpp whitelist.h
	$(CPP) $(CPPFLAGS) -c whitelist.cpp	

clean:
	rm -f *.o utest
