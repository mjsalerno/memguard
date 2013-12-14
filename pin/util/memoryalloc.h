#ifndef MEMORY_ALLOC_H
#define MEMORY_ALLOC_H
#include <iostream>
#include <string>
#include <cstddef>
class MemoryAlloc {
private:
	/**
	 * Starting address of this MemoryAlloc.
	 */
	void *address;
	
	/**
	 * Size of this MemoryAlloc in bytes.
	 */
	unsigned int size;
public:
	/**
	 * Default Constructor.
	 * Sets the address and size to zero;
	 */
	MemoryAlloc();

	/**
	 * Complete Constructor.
	 * @param address Starting address of this memory segment.
	 * @param size Size of the memory allocated in bytes.
	 */
	MemoryAlloc(void *address, unsigned int size);
	
	/**
	 * Gets the starting address of this MemoryAlloc.
	 * @return Returns the address of this MemoryAlloc.
	 */
	void* getAddress();

	/**
	 * Sets the address of this MemoryAlloc.
	 * @param address Address of this MemoryAlloc.
	 */
	void setAddress(void *address);

	/**
	 * Gets the size in bytes of this MemoryAlloc.
	 * @return Returns the size of this MemoryAlloc in bytes.
	 */
	unsigned int getSize();

	/**
	 * Sets the size in bytes of this MemoryAlloc.
	 * @param Size in bytes of this MemoryAlloc.
	 */
	void setSize(unsigned int size);

	/**
	 * Prints a textual representation of this MemoryAlloc. [address ~ size]
	 */
	void print();
};
#endif
