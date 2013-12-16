#include "memlist.h"

MemList::MemList() {
	/* TODO: Nothing to do? */
}

void MemList::add(MemoryAlloc &alloc) {
	this->list.push_back(alloc);
}

MemoryAlloc MemList::add(void* address, int size) {
	MemoryAlloc alloc(address, size);
	this->list.push_back(alloc);
	return alloc;
}

MemoryAlloc MemList::add(void* address, int size, int fenceSize) {
	MemoryAlloc alloc(address, size, fenceSize);
	this->list.push_back(alloc);
	return alloc;	
}

MemoryAlloc MemList::get(unsigned int index) {
	MemoryAlloc alloc;
	if(index >= 0 && index < this->list.size()) {
		alloc = list.at(index);
	}
	return alloc;
}

bool MemList::removeMatching(void* address) {
	bool success = false;
	int index = containsAddress(address);
	if(index >= 0) {
		success = remove(index);
	}
	return success;
}

bool MemList::removeMatching(MemoryAlloc &alloc) {
	return removeMatching(alloc.getAddress());
}

bool MemList::remove(unsigned int index) {
	bool success = false;
	if(index >= 0 && index < this->list.size()) {
		list.erase(this->list.begin() + index);
		success = true;
	}
	return success;
}

void MemList::clear() {
	this->list.clear();
}

int MemList::size() {
	return this->list.size();
}

bool MemList::isEmpty() {
	return this->list.empty();
}

int MemList::containsAddress(void* address) {
	int index = ERR_NOT_FOUND;
	for(unsigned int i = 0; i < this->list.size(); i++) {
		MemoryAlloc alloc = this->list.at(i);
		void *addr = alloc.getAddress();
		// Check to see if the address is valid
		if(addr == address) {
			/* We found the address, save the location
			and exit the loop. */
			index = i;
			break;
		// Perform a check to see if the address is allocated mid-memory
		} else if(address >= addr && address <= (void*)((char*)addr + alloc.getUserSize() - 1)) {
			// The address was a mid-memory chunk
			index = ERR_MID_CHUNK;
			break;
		// Perform a check to see if the address is allocaed in a memory-fence
		} else if((address >= alloc.getUnderflowFence() && address < addr) || 
			(address >= alloc.getOverflowFence() && address < (void*)((char*)alloc.getOverflowFence() + alloc.getFenceSize()))) {
			// The address was contained in a memory fence
			index = ERR_IN_FENCE;
			break;
		}
	}
	return index;
}
