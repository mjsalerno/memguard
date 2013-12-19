#include <limits.h>
#include "sample1.h"
#include "gtest/gtest.h"

// Tests positive input.
TEST(IsPrimeTest, Positive) {
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
	// Test The print feature
	// Free allocated memory
	free(string);
	free(s2);
}
