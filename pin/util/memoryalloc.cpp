#include "memoryalloc.h"

using namespace std;

MemoryAlloc::MemoryAlloc() {
	this->address = NULL;
	this->size = 0;
}

MemoryAlloc::MemoryAlloc(void *address, unsigned int size) {
	this->address = address;
	this->size = size;
}

void* MemoryAlloc::getAddress() {
	return this->address;
}

void MemoryAlloc::setAddress(void *address) {
	this->address = address;
}

unsigned int MemoryAlloc::getSize() {
	return this->size;
}

void MemoryAlloc::setSize(unsigned int size) {
	this->size = size;
}

void MemoryAlloc::print() {
	cout << "MemoryAlloc[address = " << this->address << " : size = " << this->size << " bytes]" << endl;
}