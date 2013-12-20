#ifndef MEMORY_ALLOC_H
#define MEMORY_ALLOC_H
#include <cstdio>
#include <string>
#include <cstddef>

#define DEFAULT_FENCE_SIZE 256

class MemoryAlloc {
private:
	/**
	 * Starting address of this MemoryAlloc.
	 */
	void *address;
	
	/**
	 * Size of this MemoryAlloc in bytes.
	 */
	unsigned int userSize;

	/**
	 * Size of the memory fence in bytes.
	 */
	unsigned int fenceSize;

	/**
	 * Total size in bytes including the space
	 * the user wanted to allocate and the underflow
	 * and overflow fence.
	 */
	unsigned int totalSize;

	/**
	 * Starting address of the underflow fence. 
	 */
	void *underflowFence;

	/**
	 * Starting address of the overflow fence.
	 */
	void *overflowFence;
public:
	/**
	 * Default Constructor.
	 * Sets the address, user_size, and fence_size to zero;
	 */
	MemoryAlloc();

	/**
	 * Complete Constructor.
	 * @param address Starting address of this memory segment.
	 * @param userSize Size of the memory the user wanted to allocate in bytes.
	 * @param fenceSize Size of the memory fences to be created in bytes.
	 */
	MemoryAlloc(void *address, unsigned int userSize, unsigned int fenceSize);
	
	/**
	 * Creates a MemoryAlloc node with a default fence_size of DEFAULT_FENCE_SIZE.
	 * @param address Starting address of this memory segment.
	 * @param user_size Size of the memory the user wanted to allocate in bytes.
	 */
	MemoryAlloc(void *address, unsigned int userSize);


	/**
	 * Gets the starting address of this MemoryAlloc.
	 * @return Returns the address of this MemoryAlloc.
	 */
	void* getAddress();

	/**
	 * Gets the starting address of the underflow fence.
	 * @return Returns the starting address of the underflow fence.
	 */
	void* getUnderflowFence();

	/**
	 * Gets the starting address of the overflow fence.
	 * @return Returns the starting address of the overflow fence.
	 */
	void* getOverflowFence();

	/**
	 * Sets the address of this MemoryAlloc.
	 * @param address Address of this MemoryAlloc.
	 */
	void setAddress(void *address);

	/**
	 * Sets the underflowAddress of this MemoryAlloc.
	 * @param address Starting address of the underflow fence.
	 */
	void setUnderflowFence(void *address);

	/**
	 * Sets the overflowAddress of this MemoryAlloc.
	 * @param address Starting address of the overflow fence.
	 */
	void setOverflowFence(void *address);

	/**
	 * Gets the size in bytes of this MemoryAlloc.
	 * @return Returns the size of this MemoryAlloc in bytes.
	 */
	unsigned int getUserSize();

	/**
	 * Sets the size in bytes of this MemoryAlloc.
	 * @param userSize Size in bytes of this MemoryAlloc.
	 */
	void setUserSize(unsigned int userSize);

	/**
	 * Sets the size in bytes of the memory fences used in this MemoryAlloc.
	 * @param fenceSize Size in bytes of the memory fences in this MemoryAlloc.
	 */
	void setFenceSize(unsigned int fenceSize);

	/**
	 * Gets the size in bytes of the memory fences in this MemoryAlloc.
	 * @return Returns the size of the memory fences of this MemoryAlloc in bytes.
	 */
	unsigned int getFenceSize();

	/**
	 * Sets the size in bytes of this MemoryAlloc.
	 * @param totalSize Size in bytes of this MemoryAlloc including memory
	 * fences and user demanded space.
	 */
	void setTotalSize(unsigned int totalSize);

	/**
	 * Gets the total size of this MemoryAlloc.
	 * This value is the sum of userSize and 2 * fenceSize.
	 * @return Returns the total size of this MemoryAlloc.
	 */
	unsigned int getTotalSize();

	/**
	 * Prints a textual representation of this MemoryAlloc. 
	 * MemoryAlloc[address = addr : size = n bytes]
	 */
	void print();

	/**
	 * Gets the string representation of this MemoryAlloc.
	 * @param buffer Pointer to the buffer to store the string in.
	 * @param size Size of the buffer.
	 * @returns Returns a C string representing this MemoryAlloc.
	 */
	char *toString(char *buffer, int size);
};
#endif
