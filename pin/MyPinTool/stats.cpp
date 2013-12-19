#include "stats.h"

Stats::Stats() {
	this->mallocCount = 0;
	this->freeCount = 0;
	this->invalidReadCount = 0;
	this->invalidWriteCount = 0;
	this->fenceHitCount = 0;
	this->invalidFreeCount = 0;
	this->midFreeChunkCount = 0;
	this->freeNullCount = 0;
	this->invalidReturnCount = 0;
	this->fenceOverflowHitCount = 0;
	this->fenceUnderflowHitCount = 0;
}

void Stats::reset() {
	this->mallocCount = 0;
	this->freeCount = 0;
	this->invalidReadCount = 0;
	this->invalidWriteCount = 0;
	this->fenceHitCount = 0;
	this->invalidFreeCount = 0;
	this->midFreeChunkCount = 0;
	this->freeNullCount = 0;
	this->invalidReturnCount = 0;
	this->fenceOverflowHitCount = 0;
	this->fenceUnderflowHitCount = 0;
}

unsigned int Stats::getMallocCount() {
	return this->mallocCount;
}
unsigned int Stats::getFreeCount() {
	return this->freeCount;
}
unsigned int Stats::getInvalidReadCount() {
	return this->invalidReadCount;
}

unsigned int Stats::getInvalidWriteCount() {
	return this->invalidWriteCount;
}

unsigned int Stats::getFenceHitCount() {
	return this->fenceHitCount;
}

unsigned int Stats::getInvalidFreeCount() {
	return this->invalidFreeCount;
}

unsigned int Stats::getMidFreeChunkCount() {
	return this->midFreeChunkCount;
}

unsigned int Stats::getFreeNullCount() {
	return this->freeNullCount;
}

unsigned int Stats::getInvalidReturnCount() {
	return this->invalidReturnCount;
}

unsigned int Stats::getFenceOverflowHitCount() {
	return this->fenceOverflowHitCount;
}

unsigned int Stats::getFenceUnderflowHitCount() {
	return this->fenceUnderflowHitCount;
}

void Stats::setMallocCount(unsigned int count) {
	this->mallocCount = count;
}

void Stats::setFreeCount(unsigned int count) {
	this->freeCount = count;
}

void Stats::setInvalidReadCount(unsigned int count) {
	this->invalidReadCount = count;
}

void Stats::setInvalidWriteCount(unsigned int count) {
	this->invalidWriteCount = count;
}

void Stats::setFenceHitCount(unsigned int count) {
	this->fenceHitCount = count;
}

void Stats::setInvalidFreeCount(unsigned int count) {
	this->invalidFreeCount = count;
}

void Stats::setMidFreeChunkCount(unsigned int count) {
	this->midFreeChunkCount = count;
}

void Stats::setFreeNullCount(unsigned int count) {
	this->freeNullCount = count;
}

void Stats::setInvalidReturnCount(unsigned int count) {
	this->invalidReturnCount = count;
}

void Stats::setFenceOverflowHitCount(unsigned int count) {
	this->fenceOverflowHitCount = count;
}

void Stats::setFenceUnderflowHitCount(unsigned int count) {
	this->fenceUnderflowHitCount = count;
}

void Stats::incMallocCount() {
	this->mallocCount++;
}

void Stats::incFreeCount() {
	this->freeCount++;
}

void Stats::incInvalidReadCount() {
	this->invalidReadCount++;
}

void Stats::incInvalidWriteCount() {
	this->invalidWriteCount++;
}

void Stats::incFenceHitCount() {
	this->fenceHitCount++;
}

void Stats::incInvalidFreeCount() {
	this->invalidFreeCount++;
}

void Stats::incMidFreeChunkCount() {
	this->midFreeChunkCount++;
}

void Stats::incFreeNullCount() {
	this->freeNullCount++;
}

void Stats::incInvalidReturnCount() {
	this->invalidReturnCount++;
}

void Stats::incFenceOverflowHitCount() {
	this->fenceOverflowHitCount++;
}

void Stats::incFenceUnderflowHitCount() {
	this->fenceUnderflowHitCount++;
}

void Stats::displayResults(MemList memlist, FILE *fp) {
	if(fp == NULL) {
		fp = stdin;
	}
	// Print out the initial object
	fprintf(fp, "==================================\n");
	fprintf(fp, "%-20s %s\n", "ACTIONS ", "COUNT");
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "allocations: ", this->mallocCount);
	fprintf(fp, "%-20s %d\n", "deallocations: ", this->freeCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid reads: ", this->invalidReadCount);
	fprintf(fp, "%-20s %d\n", "invalid writes: ", this->invalidWriteCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid frees: ", this->invalidFreeCount);
	fprintf(fp, "%-20s %d\n", "    null free: ", this->freeNullCount);
	fprintf(fp, "%-20s %d\n", "    midchunk free: ", this->midFreeChunkCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "fence hit: ", this->fenceHitCount);
	fprintf(fp, "%-20s %d\n", "    underflow: ", this->fenceUnderflowHitCount);
	fprintf(fp, "%-20s %d\n", "    overflow: ", this->fenceOverflowHitCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid returns: ", this->invalidReturnCount);
	fprintf(fp, "==================================\n");
	// Print out the contents of memlist if it was provided.
	fprintf(fp, "%-20s\n", "MEMLIST CONTENTS");
	char buffer[2048];
	int lostMemory = 0;
	for(int i = 0; i < memlist.size(); i++) {
		MemoryAlloc alloc = memlist.get(i); 
		fprintf(fp, "%s\n", alloc.toString(buffer, 2048));
		lostMemory += alloc.getTotalSize();
	}
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "list size: ", memlist.size());
	fprintf(fp, "%-20s %d %s\n", "total loss: ", lostMemory, "bytes");
	fprintf(fp, "==================================\n");
}
