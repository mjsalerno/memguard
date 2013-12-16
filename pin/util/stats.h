#ifndef STATS_H
#define STATS_H
#include <cstdio>
#include "memlist.h"
class Stats {
private:
	unsigned int mallocCount;
	unsigned int freeCount;
	unsigned int invalidReadCount;
	unsigned int invalidWriteCount;
	unsigned int invalidFreeCount;
	unsigned int midFreeChunkCount;
	unsigned int freeNullCount;
	unsigned int fenceHitCount;
public:
	Stats();
	unsigned int getMallocCount();
	unsigned int getFreeCount();
	unsigned int getInvalidReadCount();
	unsigned int getInvalidWriteCount();
	unsigned int getFenceHitCount();
	unsigned int getInvalidFreeCount();
	unsigned int getMidFreeChunkCount();
	unsigned int getFreeNullCount();

	void setMallocCount(unsigned int count);
	void setFreeCount(unsigned int count);
	void setInvalidReadCount(unsigned int count);
	void setInvalidWriteCount(unsigned int count);
	void setFenceHitCount(unsigned int count);
	void setInvalidFreeCount(unsigned int count);
	void setMidFreeChunkCount(unsigned int count);
	void setFreeNullCount(unsigned int count);


	void incMallocCount();
	void incFreeCount();
	void incInvalidReadCount();
	void incInvalidWriteCount();
	void incFenceHitCount();
	void incInvalidFreeCount();
	void incMidFreeChunkCount();
	void incFreeNullCount();

	void reset();

	/**
	 * Displays the results of the running program at exit.
	 * @param memlist List containing all the left over memory allocations.
	 * @param fp File to write the results to. Defaults to stdin if no file is provided.
	 */
	void displayResults(MemList memlist, FILE *fp);
};
#endif