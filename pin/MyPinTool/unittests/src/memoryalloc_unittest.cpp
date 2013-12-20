#include <limits.h>
#include "memoryalloc.h"
#include "gtest/gtest.h"

// Tests positive input.
TEST(MemoryAlloc, All) {
  // Allocate Some pointers
	char *string = (char*) malloc(56);
	char *s2 = (char*) malloc(102);
	// Allocate one memory alloc and test the print function
	MemoryAlloc ma1(string, 56);
	MemoryAlloc ma2(s2, 102);
	// Check to make sure the correct values are being returned
	ASSERT_TRUE(ma1.getUserSize() == 56);
	ASSERT_TRUE(ma2.getUserSize() == 102);
	ASSERT_TRUE(ma1.getUserSize() != ma2.getUserSize());
	ASSERT_TRUE(ma1.getAddress() == (char*)string + DEFAULT_FENCE_SIZE);
	ASSERT_TRUE(ma2.getAddress() != (char*)string + DEFAULT_FENCE_SIZE);
	ASSERT_TRUE(ma1.getAddress() != ma2.getAddress());
	ASSERT_TRUE(ma2.getAddress() == (char*)s2 + DEFAULT_FENCE_SIZE);
	// Change some values
	ma2.setAddress((char*)s2 + DEFAULT_FENCE_SIZE);
	ma2.setUnderflowFence(s2);
	ma2.setOverflowFence((char*)ma2.getAddress() + 102);
	ma2.setUserSize(4);
	ma2.setFenceSize(1024);
	ma2.setTotalSize(77777);
	// Test new values that were set
	ASSERT_TRUE(ma2.getAddress() == (char*)s2 + DEFAULT_FENCE_SIZE);
	ASSERT_TRUE(ma2.getUnderflowFence() == s2);
	ASSERT_TRUE(ma2.getOverflowFence() == (char*)ma2.getAddress() + 102);
	ASSERT_TRUE(ma2.getUserSize() == 4);
	ASSERT_TRUE(ma2.getFenceSize() == 1024);
	ASSERT_TRUE(ma2.getTotalSize() == 77777);
	// Free allocated memory
	free(string);
	free(s2);
}