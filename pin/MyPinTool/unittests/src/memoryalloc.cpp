#include "memoryalloc.h"

using namespace std;

MemoryAlloc::MemoryAlloc() {
	this->address = NULL;
	this->underflowFence = NULL;
	this->overflowFence = NULL;
	this->userSize = 0;
	this->fenceSize = 0;
	this->totalSize = 0;
}

MemoryAlloc::MemoryAlloc(void *address, unsigned int userSize, unsigned int fenceSize) {
	this->underflowFence = address;
	this->address = (char*)address + fenceSize;
	this->overflowFence = (char*)this->address + userSize; 
	// Calculate Sizes
	this->userSize = userSize;
	this->fenceSize = fenceSize;
	this->totalSize = userSize + (2 * fenceSize);
}

MemoryAlloc::MemoryAlloc(void *address, unsigned int userSize) {
	this->underflowFence = address;
	this->address = (char*)address + DEFAULT_FENCE_SIZE;
	this->overflowFence = (char*)this->address + userSize;
	// Calculate Sizes
	this->userSize = userSize;
	this->fenceSize = DEFAULT_FENCE_SIZE;
	this->totalSize = userSize + (2 * DEFAULT_FENCE_SIZE);
}

void* MemoryAlloc::getAddress() {
	return this->address;
}

void MemoryAlloc::setAddress(void *address) {
	this->address = address;
}

void* MemoryAlloc::getUnderflowFence() {
	return this->underflowFence;
}

void MemoryAlloc::setUnderflowFence(void *address) {
	this->underflowFence = address;
}

void* MemoryAlloc::getOverflowFence() {
	return this->overflowFence;
}

void MemoryAlloc::setOverflowFence(void* address) {
	this->overflowFence = address;
}

unsigned int MemoryAlloc::getUserSize() {
	return this->userSize;
}

void MemoryAlloc::setUserSize(unsigned int userSize) {
	this->userSize = userSize;
}

unsigned int MemoryAlloc::getFenceSize() {
	return this->fenceSize;
}

void MemoryAlloc::setFenceSize(unsigned int fenceSize) {
	this->fenceSize = fenceSize;
}

unsigned int MemoryAlloc::getTotalSize() {
	return this->totalSize;
}

void MemoryAlloc::setTotalSize(unsigned int totalSize) {
	this->totalSize = totalSize;
}

void MemoryAlloc::print() {
	printf("MemoryAlloc[address = %p : underflowFence = %p : overflowFence = %p : userSize = %u : fenceSize = %u : totalSize = %u]\n", 
		this->address, this->underflowFence, this->overflowFence, this->userSize, this->fenceSize, this->totalSize);
}

char* MemoryAlloc::toString(char *buffer, int size) {
	snprintf(buffer, size, "MemoryAlloc[address = %p : underflowFence = %p : overflowFence = %p : userSize = %u : fenceSize = %u : totalSize = %u]", 
		this->address, this->underflowFence, this->overflowFence, this->userSize, this->fenceSize, this->totalSize);
	return buffer;
}
