CPP = g++
CPPFLAGS = -g -Wall -Werror

all: utest

utest: utest.o memoryalloc.o memlist.o stats.o
	$(CPP) $(CPPFLAGS) -o utest utest.o memoryalloc.o memlist.o stats.o

utest.o: utest.cpp memoryalloc.h memlist.h stats.h
	$(CPP) $(CPPFLAGS) -c utest.cpp

memoryalloc.o: memoryalloc.cpp memoryalloc.h
	$(CPP) $(CPPFLAGS) -c memoryalloc.cpp

memlist.o: memlist.cpp memlist.h
	$(CPP) $(CPPFLAGS) -c memlist.cpp	

stats.o: stats.cpp stats.h
	$(CPP) $(CPPFLAGS) -c stats.cpp

clean:
	rm -f *.o utest
