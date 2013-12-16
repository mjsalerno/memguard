#include <cstdlib>
#include <cassert>
#include <iostream>
#include "memlist.h"

using namespace std;

void memoryAllocTests();
void memListTests();

int main(int argc, char *argv[]) {
	/* Test nodes */
	memoryAllocTests();
	/* if node tests pass, test the list */
	memListTests();
	return EXIT_SUCCESS;
}

void memoryAllocTests() {
	// Allocate Some pointers
	char *string = (char*) malloc(56);
	char *s2 = (char*) malloc(102);
	// Allocate one memory alloc and test the print function
	MemoryAlloc ma1(string, 56);
	MemoryAlloc ma2(s2, 102);
	// Check to make sure the correct values are being returned
	assert(ma1.getUserSize() == 56);
	assert(ma2.getUserSize() == 102);
	assert(ma1.getUserSize() != ma2.getUserSize());
	cout << "=== MemoryAlloc getSize() test passed." << endl;
	assert(ma1.getAddress() == string);
	assert(ma2.getAddress() != string);
	assert(ma1.getAddress() != ma2.getAddress());
	assert(ma2.getAddress() == s2);
	cout << "=== MemoryAlloc getAddress() test passed." << endl;
	// Test The print feature
	cout << "=== Testing the print function" << endl;
	ma1.print();
	ma2.print();
	// Free allocated memory
	free(string);
	free(s2);
}

void memListTests() {
	MemList mlist;
	int size1 = 1337;
	char *s1 = (char*) malloc(size1);
	// Check the initial size;
	assert(mlist.size() == 0);
	cout << "=== MemList size() test passed." << endl;
	// Create a new node
	mlist.add(s1, size1);
	// Check the size
	assert(mlist.size() == 1);
	cout << "=== MemList add(address, size) test passed." << endl;
	// Remove the node
	bool success = mlist.remove(0);
	assert(success == true);
	assert(mlist.size() == 0);
	assert(mlist.isEmpty());
	cout << "=== MemList remove(int index) and isEmpty() test passed." << endl;
	// Add the node back
	MemoryAlloc ma1(s1, size1);
	mlist.add(ma1);
	assert(mlist.size() == 1);
	cout << "=== MemList add(MemoryAlloc alloc) test passed." << endl;
	// Check the containsAddress function
	int index = mlist.containsAddress(ma1.getAddress());
	assert(index >= 0);
	index = mlist.containsAddress(s1);
	assert(index >= 0);
	assert(index == 0); // Only item inserted so it must be at index 0 right?
	index = mlist.containsAddress(NULL);
	assert(index == ERR_NOT_FOUND);
	index = mlist.containsAddress((void*)((char*)ma1.getAddress() + 1));
	assert(index == ERR_MID_CHUNK);
	cout << "=== MemList containsAddress(void *address) test passed." << endl;
	// Check the get function
	index = mlist.containsAddress(s1);
	MemoryAlloc g = mlist.get(index); 
	assert(g.getAddress() == ma1.getAddress());
	assert(g.getUserSize() == ma1.getUserSize());
	cout << "=== MemList get(int index) test passed." << endl;
	// Check removeMatching(void* address)
	bool removed = mlist.removeMatching(s1);
	assert(removed);
	removed = mlist.removeMatching(NULL);
	assert(!removed);
	cout << "=== MemList removeMatching(void* address) test passed." << endl;
	// Check removeMatching(MemoryAlloc &alloc)
	mlist.add(ma1);
	removed = mlist.removeMatching(ma1);
	assert(removed);
	removed = mlist.removeMatching(ma1);
	assert(!removed);
	cout << "=== MemList removeMatching(MemoryAlloc &alloc) test passed." << endl;
	// Check clear()
	assert(mlist.isEmpty());
	mlist.add(ma1);
	assert(!mlist.isEmpty());
	mlist.clear();
	assert(mlist.isEmpty());
	cout << "=== MemList clear() test passed." << endl;
	// Free allocated memory
	free(s1);
}
