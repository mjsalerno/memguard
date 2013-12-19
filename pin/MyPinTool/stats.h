#ifndef STATS_H
#define STATS_H
#include <cstdio>
#include "memlist.h"
class Stats {
private:
	/* Tracking of memory allocations */
	unsigned int allocCount;
	/* Functions associated with the allocation of memory */
	unsigned int mallocCount;
	unsigned int callocCount;
	unsigned int reallocCount;
	/* Tracking of the deallocation of memory */
	unsigned int freeCount;
	/* Tracking of invalid read to allocated memory */
	unsigned int invalidReadCount;
	/* Types of invalid reads */
	unsigned int readFenceOverflow;
	unsigned int readFenceUnderflow;
	/* Tracking of invalid writes to allocated memory */
	unsigned int invalidWriteCount;
	/* Types of invalid writes */
	unsigned int writeFenceOverflow;
	unsigned int writeFenceUnderflow;
	/* Tracking of invalid free calls */
	unsigned int invalidFreeCount;
	/* Different types of invalid free calls*/
	unsigned int midFreeChunkCount;
	unsigned int freeNullCount;
	unsigned int freeNotFoundCount;
	/* Tracking of invalid return address */
	unsigned int invalidReturnCount;
public:
	Stats();

	/* Getter prototypes */
	unsigned int getAllocCount();
	unsigned int getMallocCount();
	unsigned int getCallocCount();
	unsigned int getReallocCount();

	unsigned int getFreeCount();
	
	unsigned int getInvalidFreeCount();
	unsigned int getMidFreeChunkCount();
	unsigned int getFreeNotFoundCount();
	unsigned int getFreeNullCount();
	
	unsigned int getInvalidReadCount();
	unsigned int getReadFenceOverflow();
	unsigned int getReadFenceUnderflow();

	unsigned int getInvalidWriteCount();
	unsigned int getWriteFenceOverflow();
	unsigned int getWriteFenceUnderflow();

	unsigned int getInvalidReturnCount();	

	/* Setter prototypes */
	void setAllocCount(unsigned int count);
	void setMallocCount(unsigned int count);
	void setCallocCount(unsigned int count);
	void setReallocCount(unsigned int count);

	void setFreeCount(unsigned int count);
	
	void setInvalidFreeCount(unsigned int count);
	void setMidFreeChunkCount(unsigned int count);
	void setFreeNotFoundCount(unsigned int count);
	void setFreeNullCount(unsigned int count);
	
	void setInvalidReadCount(unsigned int count);
	void setReadFenceOverflow(unsigned int count);
	void setReadFenceUnderflow(unsigned int count);

	void setInvalidWriteCount(unsigned int count);
	void setWriteFenceOverflow(unsigned int count);
	void setWriteFenceUnderflow(unsigned int count);

	void setInvalidReturnCount(unsigned int count);
	
	/* Increment prototypes */
	void incAllocCount();
	void incMallocCount();
	void incCallocCount();
	void incReallocCount();

	void incFreeCount();
	
	void incInvalidFreeCount();
	void incMidFreeChunkCount();
	void incFreeNotFoundCount();
	void incFreeNullCount();
	
	void incInvalidReadCount();
	void incReadFenceOverflow();
	void incReadFenceUnderflow();

	void incInvalidWriteCount();
	void incWriteFenceOverflow();
	void incWriteFenceUnderflow();

	void incInvalidReturnCount();
	
	/* Useful functions */
	void reset();

	/**
	 * Displays the results of the running program at exit.
	 * @param memlist List containing all the left over memory allocations.
	 * @param fp File to write the results to. Defaults to stdin if no file is provided.
	 */
	void displayResults(MemList memlist, FILE *fp);
};
#endif
