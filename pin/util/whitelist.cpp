#include "whitelist.h"

WhiteList::Whitelist() {

}

void WhiteList::add(MemoryAlloc &alloc) {

}

void WhiteList::add(void* address, int size) {

}

MemoryAlloc* WhiteList::get(int index) {
	return NULL;
}

bool WhiteList::removeMatching(void* address) {
	return false;
}

bool WhiteList::removingMatching(MemoryAlloc &alloc) {
	return false;
}

void WhiteList::remove(int index) {

}

void WhiteList::clear() {

}

int WhiteList::size() {
	return -1;
}

int WhiteList::containsAddress(void* address) {
	return -1;
}