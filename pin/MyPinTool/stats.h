#ifndef STATS_H
#define STATS_H
#include <cstdio>
#include "memlist.h"
class Stats {
private:
	unsigned int allocCount;
	unsigned int freeCount;
	
	unsigned int invalidReadCount;
	unsigned int invalidWriteCount;
	
	unsigned int invalidFreeCount;
	unsigned int midFreeChunkCount;
	unsigned int freeNullCount;
	
	unsigned int freeNotFoundCount;


	unsigned int fenceHitCount;
	unsigned int fenceOverflowHitCount;
	unsigned int fenceUnderflowHitCount;
	
	unsigned int invalidReturnCount;
public:
	Stats();
	unsigned int getAllocCount();
	unsigned int getFreeCount();
	unsigned int getInvalidReadCount();
	unsigned int getInvalidWriteCount();
	unsigned int getFenceHitCount();
	unsigned int getInvalidFreeCount();
	unsigned int getMidFreeChunkCount();
	unsigned int getFreeNotFoundCount();
	unsigned int getFreeNullCount();
	unsigned int getInvalidReturnCount();
	unsigned int getFenceOverflowHitCount();
	unsigned int getFenceUnderflowHitCount();

	void setAllocCount(unsigned int count);
	void setFreeCount(unsigned int count);
	void setInvalidReadCount(unsigned int count);
	void setInvalidWriteCount(unsigned int count);
	void setFenceHitCount(unsigned int count);
	void setInvalidFreeCount(unsigned int count);
	void setMidFreeChunkCount(unsigned int count);
	void setFreeNotFoundCount(unsigned int count);
	void setFreeNullCount(unsigned int count);
	void setInvalidReturnCount(unsigned int count);
	void setFenceOverflowHitCount(unsigned int count);
	void setFenceUnderflowHitCount(unsigned int count);

	void incAllocCount();
	void incFreeCount();
	void incInvalidReadCount();
	void incInvalidWriteCount();
	void incFenceHitCount();
	void incInvalidFreeCount();
	void incMidFreeChunkCount();
	void incFreeNotFoundCount();
	void incFreeNullCount();
	void incInvalidReturnCount();
	void incFenceOverflowHitCount();
	void incFenceUnderflowHitCount();

	void reset();

	/**
	 * Displays the results of the running program at exit.
	 * @param memlist List containing all the left over memory allocations.
	 * @param fp File to write the results to. Defaults to stdin if no file is provided.
	 */
	void displayResults(MemList memlist, FILE *fp);
};
#endif
