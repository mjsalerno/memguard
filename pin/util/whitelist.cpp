#include "whitelist.h"

WhiteList::WhiteList() {
	/* TODO: Nothing to do? */
}

void WhiteList::add(MemoryAlloc &alloc) {
	this->list.push_back(alloc);
}

void WhiteList::add(void* address, int size) {
	MemoryAlloc alloc(address, size);
	this->list.push_back(alloc);
}

MemoryAlloc WhiteList::get(unsigned int index) {
	MemoryAlloc alloc;
	if(index >= 0 && index <= this->list.size()) {
		alloc = list.at(index);
	}
	return alloc;
}

bool WhiteList::removeMatching(void* address) {
	bool success = false;
	int index = containsAddress(address);
	if(index >= 0) {
		success = remove(index);
	}
	return success;
}

bool WhiteList::removeMatching(MemoryAlloc &alloc) {
	return removeMatching(alloc.getAddress());
}

bool WhiteList::remove(unsigned int index) {
	bool success = false;
	if(index >= 0 && index <= this->list.size()) {
		list.erase(this->list.begin() + index);
		success = true;
	}
	return success;
}

void WhiteList::clear() {
	this->list.clear();
}

int WhiteList::size() {
	return this->list.size();
}

bool WhiteList::isEmpty() {
	return this->list.empty();
}

int WhiteList::containsAddress(void* address) {
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
		// Perform a check to see if it the address is allocated mid-memory
		} else if(address >= addr && address <= (void*)((char*)addr + alloc.getSize())) {
			// The address was a mid-memory chunk
			index = ERR_MID_CHUNK;
			break;
		}
	}
	return index;
}
