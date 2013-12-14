#include <cstdlib>
#include <cassert>
#include <iostream>
#include "whitelist.h"

using namespace std;

void memoryAllocTests();
void whiteListTests();

int main(int argc, char *argv[]) {
	/* Test nodes */
	memoryAllocTests();
	/* if node tests pass, test the list */
	whiteListTests();
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
	assert(ma1.getSize() == 56);
	assert(ma2.getSize() == 102);
	assert(ma1.getSize() != ma2.getSize());
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

void whiteListTests() {
	WhiteList wlist;
	int size1 = 1337;
	char *s1 = (char*) malloc(size1);
	// Check the initial size;
	assert(wlist.size() == 0);
	cout << "=== WhiteList size() test passed." << endl;
	// Create a new node
	wlist.add(s1, size1);
	// Check the size
	assert(wlist.size() == 1);
	cout << "=== WhiteList add(address, size) test passed." << endl;
	// Remove the node
	bool success = wlist.remove(0);
	assert(success == true);
	assert(wlist.size() == 0);
	assert(wlist.isEmpty());
	cout << "=== WhiteList remove(int index) and isEmpty() test passed." << endl;
	// Add the node back
	MemoryAlloc ma1(s1, size1);
	wlist.add(ma1);
	assert(wlist.size() == 1);
	cout << "=== WhiteList add(MemoryAlloc alloc) test passed." << endl;
	// Check the containsAddress function
	int index = wlist.containsAddress(ma1.getAddress());
	assert(index >= 0);
	index = wlist.containsAddress(s1);
	assert(index >= 0);
	assert(index == 0); // Only item inserted so it must be at index 0 right?
	index = wlist.containsAddress(NULL);
	assert(index == ERR_NOT_FOUND);
	index = wlist.containsAddress((void*)((char*)ma1.getAddress() + 1));
	assert(index == ERR_MID_CHUNK);
	cout << "=== WhiteList containsAddress(void *address) test passed." << endl;
	// Check the get function
	index = wlist.containsAddress(s1);
	MemoryAlloc g = wlist.get(index); 
	assert(g.getAddress() == ma1.getAddress());
	assert(g.getSize() == ma1.getSize());
	cout << "=== WhiteList get(int index) test passed." << endl;
	// Check removeMatching(void* address)
	bool removed = wlist.removeMatching(s1);
	assert(removed);
	removed = wlist.removeMatching(NULL);
	assert(!removed);
	cout << "=== WhiteList removeMatching(void* address) test passed." << endl;
	// Check removeMatching(MemoryAlloc &alloc)
	wlist.add(ma1);
	removed = wlist.removeMatching(ma1);
	assert(removed);
	removed = wlist.removeMatching(ma1);
	assert(!removed);
	cout << "=== WhiteList removeMatching(MemoryAlloc &alloc) test passed." << endl;
	// Check clear()
	assert(wlist.isEmpty());
	wlist.add(ma1);
	assert(!wlist.isEmpty());
	wlist.clear();
	assert(wlist.isEmpty());
	cout << "=== WhiteList clear() test passed." << endl;
	// Free allocated memory
	free(s1);
}
