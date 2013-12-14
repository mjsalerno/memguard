#ifndef WHITE_LIST_H
#define WHITE_LIST_H
#include <cstdlib>
#include <vector>
#include "memoryalloc.h"

/* Define error code Constants */
#define ERR_NOT_FOUND -1
#define ERR_MID_CHUNK -2 

using namespace std;

class WhiteList {
private:
	vector<MemoryAlloc> list;
public:
	/**
	 * Default Constructor.
	 * Creates an empty WhiteList.
	 */
	WhiteList();

	/**
	 * Adds the provided memory alloc to this WhiteList.
	 * @param alloc MemoryAlloc to add to this WhiteList.
	 */
	void add(MemoryAlloc &alloc);

	/**
	 * Creates a new MemoryAlloc with the provided values
	 * and adds it to this WhiteList.
	 */
	void add(void* address, int size);

	/**
	 * Gets the MemoryAlloc at the provided index.
	 * @return Returns the MemoryAlloc at the index if it is valid, otherwise NULL.
	 */
	MemoryAlloc get(unsigned int index);

	/**
	 * Searces this WhiteList for a matching address in a MemoryAlloc
	 * element and removes it.
	 * @return Returns true if the element was found and removed, else false.
	 */
	bool removeMatching(void* address);

	/**
	 * Searches this WhiteList for the provided MemoryAlloc and removes it.
	 * @param alloc MemoryAlloc to be removed. 
	 * @return Returns true if the element was found and removed, else false.
	 */
	bool removingMatching(MemoryAlloc &alloc);

	/**
	 * Removes the element at the specified index.
	 * @param index Index of the item to remove from the list.
	 * @return Returns true if the index was valid, else false.
	 */
	bool remove(unsigned int index);

	/**
	 * Removes all elements from this WhiteList.
	 */
	void clear();

	/**
	 * Gets the size of this WhiteList.
	 * @return Returns the size of this WhiteList.
	 */
	int size();

	/**
	 * Determines if this WhiteList is empty or not.
	 * @return Returns true if empty, else false.
	 */
	bool isEmpty();

	/**
	 * Checks to see if the provided address is contained within the whitelist.
	 * The address provided should be the start of the memory allocation.
	 * @param address Address to search this WhiteList for.
	 * @return Returns the index of the item in the list if it is found and correct, ERR_NOT_FOUND 
	 * if the item is not found, and ERR_MID_CHUNK if the address is found but contained within 
	 * a block.
	 */
	int containsAddress(void* address); 
};
#endif
