#include <limits.h>
#include "memlist.h"
#include "memoryalloc.h"
#include "gtest/gtest.h"

// Tests positive input.
TEST(MemList, All) {
	MemList mlist;
	int size1 = 1337;
	int size2 = 777;
	char *s1 = (char*) malloc(size1);
	char *s2 = (char*) malloc(size2);
	// Check the initial size;
	ASSERT_TRUE(mlist.size() == 0);
	// Create a new node
	mlist.add(s1, size1);
	// Check the size
	ASSERT_TRUE(mlist.size() == 1);
	// Remove the node
	bool success = mlist.remove(0);
	ASSERT_TRUE(success == true);
	ASSERT_TRUE(mlist.size() == 0);
	ASSERT_TRUE(mlist.isEmpty());
	// Add the node back
	MemoryAlloc ma1(s1, size1);
	mlist.add(ma1);
	ASSERT_TRUE(mlist.size() == 1);
	// Use add(void*, unsigned int, unsigned int)
	mlist.add(s2, size2, DEFAULT_FENCE_SIZE);
	ASSERT_TRUE(mlist.size() == 2);
	success = mlist.remove(1);
	ASSERT_TRUE(success);
	// Check the containsAddress function
	int index = mlist.containsAddress(ma1.getAddress());
	ASSERT_TRUE(index >= 0);
	index = mlist.containsAddress(ma1.getAddress());
	ASSERT_TRUE(index >= 0);
	ASSERT_TRUE(index == 0); // Only item inserted so it must be at index 0 right?
	index = mlist.containsAddress(NULL);
	ASSERT_TRUE(index == ERR_NOT_FOUND);
	index = mlist.containsAddress((void*)((char*)ma1.getAddress() + 1));
	ASSERT_TRUE(index == ERR_MID_CHUNK);
	index = mlist.containsAddress(s1); // Check the underflow fence check
	ASSERT_TRUE(index == ERR_FENCE_UNDERFLOW);
	index = mlist.containsAddress((char*)ma1.getAddress() + ma1.getUserSize()); // Check the overflow fence check
	ASSERT_TRUE(index == ERR_FENCE_OVERFLOW);
	// Check the get function
	index = mlist.containsAddress(ma1.getAddress());
	MemoryAlloc g = mlist.get(index); 
	ASSERT_TRUE(g.getAddress() == ma1.getAddress());
	ASSERT_TRUE(g.getUserSize() == ma1.getUserSize());
	// Check removeMatching(void* address)
	bool removed = mlist.removeMatching(ma1.getAddress());
	ASSERT_TRUE(removed);
	removed = mlist.removeMatching(NULL);
	ASSERT_TRUE(!removed);
	// Check removeMatching(MemoryAlloc &alloc)
	mlist.add(ma1);
	removed = mlist.removeMatching(ma1);
	ASSERT_TRUE(removed);
	removed = mlist.removeMatching(ma1);
	ASSERT_TRUE(!removed);
	// Check clear()
	ASSERT_TRUE(mlist.isEmpty());
	mlist.add(ma1);
	ASSERT_TRUE(!mlist.isEmpty());
	mlist.clear();
	ASSERT_TRUE(mlist.isEmpty());
	// Perform some tests to hit certain branches in the code (and increase coverage :D)
	bool failed = mlist.remove(1024);
	ASSERT_TRUE(!failed);
	MemoryAlloc f = mlist.get(1024); // this function does not return a pointer ....
	// ASSERT_TRUE();
	failed = mlist.removeMatching(NULL);
	ASSERT_TRUE(!failed);
	// Free allocated memory
	free(s1);
	free(s2);
}