#include <cstdlib>
#include <cassert>
#include <iostream>
#include "memoryalloc.h"

using namespace std;

int main(int argc, char *argv[]) {
	// Allocate Some pointers
	char *string = (char*) malloc(56);
	char *s2 = (char*) malloc(102);
	// Allocate one memory alloc and test the print function
	MemoryAlloc ma1(string, 56);
	MemoryAlloc ma2(s2, 102);
	// Check to make sure the correct values are being returned
	assert(ma1.getSize() == 56);
	assert(ma2.getSize() == 102);
	assert(ma1.getSize() != ma2.getSize());
	cout << "=== Size test passed." << endl;
	assert(ma1.getAddress() == string);
	assert(ma2.getAddress() != string);
	assert(ma1.getAddress() != ma2.getAddress());
	assert(ma2.getAddress() == s2);
	cout << "=== Address test passed." << endl;
	// Test The print feature
	cout << "=== Testing the print function" << endl;
	ma1.print();
	ma2.print();
	// Free allocated memory
	free(string);
	free(s2);
	return EXIT_SUCCESS;
}