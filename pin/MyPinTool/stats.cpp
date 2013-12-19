#include "stats.h"

Stats::Stats() {
	this->reset();
}

void Stats::reset() {
	/* Allocation Stats */
	this->allocCount = 0;
	this->mallocCount = 0;
	this->callocCount = 0;
	this->reallocCount = 0;
	/* Deallocation Stats */
	this->freeCount = 0;
	/* Invalid Free Stats*/
	this->invalidFreeCount = 0;
	this->midFreeChunkCount = 0;
	this->freeNullCount = 0;
	/* Invalid Read Stats */
	this->invalidReadCount = 0;
	this->readFenceOverflow = 0;
	this->readFenceUnderflow = 0;
	/* Invalid Write Stats */
	this->invalidWriteCount = 0;
	this->writeFenceOverflow = 0;
	this->writeFenceUnderflow = 0;
	/* Stack Smashing Stats */
	this->invalidReturnCount = 0;
}
/* Getters */
unsigned int Stats::getAllocCount() {
	return this->allocCount;
}

unsigned int Stats::getMallocCount() {
	return this->mallocCount;
}

unsigned int Stats::getCallocCount() {
	return this->callocCount;
}

unsigned int Stats::getReallocCount() {
	return this->reallocCount;
}

unsigned int Stats::getFreeCount() {
	return this->freeCount;
}
unsigned int Stats::getInvalidReadCount() {
	return this->invalidReadCount;
}

unsigned int Stats::getReadFenceOverflow() {
	return this->readFenceOverflow;
}

unsigned int Stats::getReadFenceUnderflow() {
	return this->readFenceUnderflow;
}

unsigned int Stats::getInvalidWriteCount() {
	return this->invalidWriteCount;
}

unsigned int Stats::getWriteFenceOverflow() {
	return this->writeFenceOverflow;
}

unsigned int Stats::getWriteFenceUnderflow() {
	return this->writeFenceUnderflow;
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

unsigned int Stats::getFreeNotFoundCount() {
	return this->freeNotFoundCount;
}

unsigned int Stats::getInvalidReturnCount() {
	return this->invalidReturnCount;
}

/* Setters */

void Stats::setAllocCount(unsigned int count) {
	this->allocCount = count;
}

void Stats::setMallocCount(unsigned int count) {
	this->allocCount = count;
}

void Stats::setCallocCount(unsigned int count) {
	this->allocCount = count;
}

void Stats::setReallocCount(unsigned int count) {
	this->allocCount = count;
}

void Stats::setFreeCount(unsigned int count) {
	this->freeCount = count;
}

void Stats::setInvalidReadCount(unsigned int count) {
	this->invalidReadCount = count;
}

void Stats::setReadFenceOverflow(unsigned int count) {
	this->readFenceOverflow = count;
}

void Stats::setReadFenceUnderflow(unsigned int count) {
	this->readFenceUnderflow = count;
}

void Stats::setInvalidWriteCount(unsigned int count) {
	this->invalidWriteCount = count;
}

void Stats::setWriteFenceOverflow(unsigned int count) {
	this->writeFenceOverflow = count;
}

void Stats::setWriteFenceUnderflow(unsigned int count) {
	this->writeFenceUnderflow = count;
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

void Stats::setFreeNotFoundCount(unsigned int count) {
	this->freeNotFoundCount = count;
}

/* Increment Count Functions */

void Stats::incAllocCount() {
	this->allocCount++;
}

void Stats::incMallocCount() {
	this->mallocCount++;
}

void Stats::incCallocCount() {
	this->callocCount++;
}

void Stats::incReallocCount() {
	this->reallocCount++;
}

void Stats::incFreeCount() {
	this->freeCount++;
}

void Stats::incInvalidReadCount() {
	this->invalidReadCount++;
}

void Stats::incReadFenceOverflow() {
	this->readFenceOverflow++;
}

void Stats::incReadFenceUnderflow() {
	this->readFenceUnderflow++;
}

void Stats::incInvalidWriteCount() {
	this->invalidWriteCount++;
}

void Stats::incWriteFenceOverflow() {
	this->writeFenceOverflow++;
}

void Stats::incWriteFenceUnderflow() {
	this->writeFenceUnderflow++;
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

void Stats::incFreeNotFoundCount() {
	this->freeNotFoundCount++;
}

void Stats::incInvalidReturnCount() {
	this->invalidReturnCount++;
}

/**
 * Display All information tracked by the pintool.
 */
void Stats::displayResults(MemList memlist, FILE *fp) {
	if(fp == NULL) {
		fp = stdin;
	}
	// Print out the initial object
	fprintf(fp, "==================================\n");
	fprintf(fp, "%-20s %s\n", "ACTIONS ", "COUNT");
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "allocations: ", this->allocCount);
	fprintf(fp, "%-20s %d\n", "    malloc", this->mallocCount);
	fprintf(fp, "%-20s %d\n", "    calloc", this->callocCount);
	fprintf(fp, "%-20s %d\n", "    realloc", this->reallocCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "deallocations: ", this->freeCount);
	fprintf(fp, "%-20s %d\n", "    free ", this->freeCount);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid reads: ", this->invalidReadCount);
	fprintf(fp, "%-20s %d\n", "    underflow: ", this->readFenceUnderflow);
	fprintf(fp, "%-20s %d\n", "    overflow: ", this->readFenceOverflow);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid writes: ", this->invalidWriteCount);
	fprintf(fp, "%-20s %d\n", "    underflow: ", this->writeFenceUnderflow);
	fprintf(fp, "%-20s %d\n", "    overflow: ", this->writeFenceOverflow);
	fprintf(fp, "----------------------------------\n");
	fprintf(fp, "%-20s %d\n", "invalid frees: ", this->invalidFreeCount);
	fprintf(fp, "%-20s %d\n", "    not found: ", this->freeNotFoundCount);
	fprintf(fp, "%-20s %d\n", "    null free: ", this->freeNullCount);
	fprintf(fp, "%-20s %d\n", "    midchunk free: ", this->midFreeChunkCount);
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
