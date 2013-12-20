#include "mallocmem.h"
/* Import the std namespace for libc++ namespace */
using namespace std;

/* pintool variables */
FILE * trace;
MemList ml;
Stats stats;
stack<ADDRINT> addrStack;
stack<ADDRINT *> addrOfReturnStack;
static REG scratchReg;
static bool forceNoRAD = false;

/**
 * Checks and marks bad memory reads to any heap memory
 * allocated throughout the life of the program.
 */
void RecordHeapMemRead(ADDRINT ip, void* addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_FENCE_UNDERFLOW || rtn == ERR_FENCE_OVERFLOW) {
        // General Counter
        stats.incInvalidReadCount();
        switch(rtn) {
            case ERR_FENCE_UNDERFLOW:
                stats.incReadFenceUnderflow();
                fprintf(trace,"##########BAD READ - UNDERFLOW: %p \n", addr);
                RecordAddrSource(ip, "BAD READ - UNDERFLOW");
                break;
            case ERR_FENCE_OVERFLOW:
                stats.incReadFenceOverflow();
                fprintf(trace,"##########BAD READ - OVERFLOW: %p \n", addr);
                RecordAddrSource(ip, "BAD READ - OVERFLOW");
                break;    
        }
    }
}

/**
 * Checks and marks bad memory writes to any heap memory
 * allocated throughout the life of the program.
 */
void RecordHeapMemWrite(ADDRINT ip, void* addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_FENCE_UNDERFLOW || rtn == ERR_FENCE_OVERFLOW) {
        // General Counter
        stats.incInvalidWriteCount();
        switch(rtn) {
            case ERR_FENCE_UNDERFLOW:
                stats.incWriteFenceUnderflow();
                fprintf(trace,"##########BAD WRITE - UNDERFLOW: %p \n", addr);
                RecordAddrSource(ip, "BAD WRITE - UNDERFLOW");
                break;
            case ERR_FENCE_OVERFLOW:
                stats.incWriteFenceOverflow();
                fprintf(trace,"##########BAD WRITE - OVERFLOW: %p \n", addr);
                RecordAddrSource(ip, "BAD WRITE - OVERFLOW");
                break;    
        }
    }
}

/**
 * Gets the sourcefile, source line, and function name and logs a message.
 * see http://software.intel.com/sites/landingpage/pintool/docs/58423/Pin/html/group__DEBUG__API.html 
 */
void RecordAddrSource(ADDRINT address, string message) {
	INT32 column = 0;   // column number within the file.
	INT32 line = 0;     // line number within the file.
	string filename;    // source file name.
	PIN_LockClient();
	PIN_GetSourceLocation(address, &column, &line, &filename);
	PIN_UnlockClient();

    string rtnname = RTN_FindNameByAddress(address);
    // print only if source was found.
    if (!filename.empty()){
		cout << BOLD << filename << ":" << dec << line << ":" << column << RESET << endl;
        cout << BOLD << "Address: 0x" << hex << address << ": In function: ";
		cout << "'" << rtnname << "': " << RESET;
	}
	// Print Error message
	cout << BOLD_RED << "Error: " << BOLD << message << RESET << endl << endl;
}

/**
 * Check for a matching pattern at the end of a string.
 */
bool hasEnding (string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

/**
 * Detects any reads made to stack memory.
 * TODO: Currently does nothing.
 */
void RecordStackMemRead(void *ip, void *addr) {    
    //printf("stack read: %p\n", addr);    
}

/**
 * Detects any writes made to stack memory.
 * TODO: Currently does nothing.
 */
void RecordStackMemWrite(void *ip, void* addr) {
    //printf("stack write: %p\n", addr); 
}

/**
 * Saves the call instruction address.
 */
void RecordCallIns(ADDRINT nextip, ADDRINT target) {
	addrStack.push(nextip);
}

/**
 * Checks the return address with one previously saved.
 */
void RecordReturnIns(ADDRINT ip, ADDRINT retip) {
	ADDRINT originval;
	if (addrStack.empty()) {
		RecordAddrSource(ip, "TOO MANY RETURNS");
		stats.incInvalidReturnCount();
		fprintf(trace, "ERROR: TOO MANY RETURNS\n");
	} else {
		originval = addrStack.top();
		addrStack.pop();
		if (originval != retip) {
			char errstr[128];
			snprintf(errstr, 128, "RETURN ADDRESS CHANGED: expected target %p, actual return target %p", (void *)originval, (void *)retip);
			RecordAddrSource(ip, (string)errstr);
			stats.incInvalidReturnCount();
			fprintf(trace, "ERROR: %s\n", errstr);
		}
	}
}
ADDRINT EmuCall(ADDRINT nextip, ADDRINT tgtip, ADDRINT *rsp)
{	
	//Save the address where the matching ret instruction should go
	addrStack.push(nextip);
    //*rsp = EmuPushValue(*rsp, nextip);
    
    ADDRINT rspVal = *rsp;
    
    rspVal = rspVal - sizeof(ADDRINT);
    ADDRINT *psp = (ADDRINT *)rspVal;
	addrOfReturnStack.push(psp);
    *psp = nextip;
    *rsp = rspVal;


    return tgtip;
}

ADDRINT EmuRet(ADDRINT ip, ADDRINT *rsp, UINT32 framesize)
{
    ADDRINT retval = 1;

    ADDRINT rspVal = *rsp;
    //ADDRINT *psp = (ADDRINT *)rspVal;
    //retval = *psp;
    *rsp = rspVal + sizeof(ADDRINT);
    
    *rsp += framesize;

	// Check the return address to make sure it is the one we saved
	if (addrStack.empty()) {
		RecordAddrSource(ip, "TOO MANY RETURNS");
		stats.incInvalidReturnCount();
		fprintf(trace, "ERROR: TOO MANY RETURNS\n");
	} else {
		ADDRINT originval = addrStack.top();
		ADDRINT *ptrtoRA = addrOfReturnStack.top();
		// if (ptrtoRA != psp)
			// cout << "NOT EQUALS" << endl;
		// else
			// cout << "EQUALS" << endl;
		addrStack.pop();
		addrOfReturnStack.pop();
		
		retval = *ptrtoRA;
		if (originval != retval) {
			char errstr[128];
			snprintf(errstr, 128, "RETURN ADDRESS CHANGED: expected target %p, actual return target %p", (void *)originval, (void *)retval);
			RecordAddrSource(ip, (string)errstr);
			stats.incInvalidReturnCount();
			fprintf(trace, "ERROR: %s\n", errstr);
		}
	}
	cout << "*rsp = " << hex << *rsp << "retip = " << hex << retval << endl;
    return retval;
}

/**
 * Is called for every instruction and instruments reads and writes
 */
void Instruction(INS ins, void *v) {
	// RETURN ADDRESS DEFENDER
	if (!forceNoRAD) {
		if (INS_IsCall(ins)) {
		    INS_InsertCall(ins, IPOINT_BEFORE,
		        AFUNPTR(EmuCall),
		        IARG_ADDRINT, INS_NextAddress(ins),
		        IARG_BRANCH_TARGET_ADDR,
		        IARG_REG_REFERENCE, REG_STACK_PTR,
		        IARG_RETURN_REGS, scratchReg, IARG_END);

		    INS_InsertIndirectJump(ins, IPOINT_AFTER, scratchReg);

		    INS_Delete(ins);
		} else if (INS_IsRet(ins)) {
		    UINT64 imm = 0;
		    if (INS_OperandCount(ins) > 0 && INS_OperandIsImmediate(ins, 0)) {
		        imm = INS_OperandImmediate(ins, 0);
		    }
		    INS_InsertCall(ins, IPOINT_BEFORE,
		        AFUNPTR(EmuRet),
		        IARG_CALL_ORDER, CALL_ORDER_FIRST,
				IARG_INST_PTR,
		        IARG_REG_REFERENCE, REG_STACK_PTR,
		        IARG_ADDRINT, (ADDRINT)imm,
		        IARG_RETURN_REGS, scratchReg, IARG_END);

		    INS_InsertIndirectJump(ins, IPOINT_AFTER, scratchReg);

		    INS_Delete(ins);
		}
	}
    // Instruments memory accesses using a predicated call, i.e.
    // the instrumentation is called iff the instruction will actually be executed.
    //
    // On the IA-32 and Intel(R) 64 architectures conditional moves and REP 
    // prefixed instructions appear as predicated instructions in Pin.
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
        //if (INS_MemoryOperandIsRead(ins, memOp)) {
        if (INS_MemoryOperandIsRead(ins, memOp) && !INS_IsStackRead(ins)) {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordHeapMemRead,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        } else if (INS_MemoryOperandIsRead(ins, memOp)) {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordStackMemRead,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
        // Note that in some architectures a single memory operand can be 
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        //if (INS_MemoryOperandIsWritten(ins, memOp)) {
        if (INS_MemoryOperandIsWritten(ins, memOp) && !INS_IsStackWrite(ins)) {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordHeapMemWrite,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        } else if (INS_MemoryOperandIsWritten(ins, memOp)) {
            INS_InsertPredicatedCall(
                ins, IPOINT_BEFORE, (AFUNPTR)RecordStackMemWrite,
                IARG_INST_PTR,
                IARG_MEMORYOP_EA, memOp,
                IARG_END);
        }
    }
}

/**
 * Before the program exits this function makes
 * prints out the results and closes the logging file.
 */
void Fini(INT32 code, void *v) {
    printf("Check %s %s %s for the results of this execution.\n", BOLDGREEN, OUTPUT_LOG, RESET);
    fprintf(trace, "#eof \n");
    // Display the stats
    stats.displayResults(ml, trace);
	if (!forceNoRAD) {
		//print the return address stack
		fprintf(trace, "TOP Address Stack:\n");
		while (!addrStack.empty()) {
			ADDRINT leftover = addrStack.top();
			fprintf(trace, "%p\n", (void *)leftover);
			IMG img = IMG_FindByAddress(leftover);
			if (IMG_Valid(img)) {
				cout << IMG_Name(img) << endl;
			}
			addrStack.pop();
		}
		fprintf(trace, "BOTTOM Address Stack:\n");
	}
    fclose(trace);
}

/**
 * This is the function that replaces the glibc malloc call.
 */
void* NewMalloc(FP_MALLOC orgFuncptr, size_t arg0, ADDRINT returnIp) {
	// Pop the call for this function
	if (!addrStack.empty()) {
		addrStack.pop();
		addrOfReturnStack.pop();
	}
    // Call the relocated entry point of the original (replaced) routine.
    void* v = orgFuncptr(arg0 + (2 * DEFAULT_FENCE_SIZE));
    stats.incMallocCount();
    stats.incAllocCount();
    MemoryAlloc ma = ml.add(v, arg0, DEFAULT_FENCE_SIZE);
    fprintf(trace, "ADDED: %p %d \n", v, DEFAULT_FENCE_SIZE);
    return ma.getAddress();
}

/**
 * This is the function that replaces the glibc calloc call.
 */
void* NewCalloc(FP_CALLOC libc_calloc, size_t arg0, size_t arg1, ADDRINT returnIp) {
	// Pop the call for this function
	if (!addrStack.empty()) {
		addrStack.pop();
	}
    // Calculate the size in bytes
    size_t bytes = (arg0 * arg1);
    size_t totalSize = bytes + (2 * DEFAULT_FENCE_SIZE);
	void *ptr = libc_calloc(totalSize, 1);
    // Create a new MemoryAlloc
    MemoryAlloc ma = ml.add(ptr, bytes, DEFAULT_FENCE_SIZE);
    // Add the information to the trace file
    fprintf(trace, "ADDED: %p %d \n", ptr, DEFAULT_FENCE_SIZE);
    stats.incAllocCount();
    stats.incCallocCount();
    return ma.getAddress();
}

/**
 * This is the function that replaces the glibc realloc call.
 */
void* NewRealloc(FP_REALLOC orgFuncptr, void* arg0, size_t arg1, ADDRINT returnIp) {
	// Pop the call for this function
	if (!addrStack.empty()) {
		addrStack.pop();
	}
	// First, find MemoryAlloc of arg0
	if(arg0 != NULL) {
        // Check the MemList
        int index  = ml.containsAddress(arg0);
        if(index >= 0) {
            MemoryAlloc alloc = ml.get(index);
			// Call the original realloc 
			void *newptr = orgFuncptr(alloc.getUnderflowFence(), arg1 + (2 * DEFAULT_FENCE_SIZE));
			if (!newptr) {
				// original realloc failed, so arg0 is untouched
				// just return NULL?
				fprintf(trace, "realloc failed for address = %p, size = %zu.\n", arg0, arg1);
				return NULL;
			}
			// Remove the MemoryAlloc for arg0
            ml.removeMatching(alloc);
			stats.incFreeCount();
			// Add MemoryAlloc for newptr
			MemoryAlloc ma = ml.add(newptr, arg1, DEFAULT_FENCE_SIZE);
			stats.incAllocCount();
            stats.incReallocCount();
    		fprintf(trace, "REALLOCED: %p, TOTAL SIZE: %zu\n", newptr, arg1 +(2 * DEFAULT_FENCE_SIZE));
    		return ma.getAddress();           
        } else if(index == ERR_NOT_FOUND) {
            // This currently gets hit since a blacklist is being used 
            fprintf(trace, "Address = %p not found. Bad address in realloc.\n", arg0);
        } else if(index == ERR_MID_CHUNK) {
            fprintf(trace, "Mid-chunk memory in realloc @ %p\n", arg0);
			stats.incMidFreeChunkCount();
        } else if(index == ERR_FENCE_UNDERFLOW) {
            fprintf(trace, "Memory Fence Hit in realloc - Underflow @ %p\n", arg0);
            // stats.incFenceHitCount();
            // stats.incFenceUnderflowHitCount();
        } else if(index == ERR_FENCE_OVERFLOW) {
            fprintf(trace, "Memory Fence Hit in realloc - Overflow @ %p\n", arg0);
            // stats.incFenceHitCount();
            // stats.incFenceOverflowHitCount();
        } else {
            // This should Never happen...
            fprintf(trace, "index = %d : Unable to realloc the memory @ %p\n", index, arg0);
        }
    } else {
        // arg0 == NULL so realloc acts as malloc(arg1)
		void* newptr = orgFuncptr(arg0, arg1 + (2 * DEFAULT_FENCE_SIZE));
		stats.incAllocCount();
		MemoryAlloc ma = ml.add(newptr, arg1, DEFAULT_FENCE_SIZE);
		fprintf(trace, "ADDED: %p, TOTAL SIZE: %zu \n", newptr, arg1 +(2 * DEFAULT_FENCE_SIZE));
		return ma.getAddress();
    }
	return NULL;
}

/**
 * This is the function that replaces the glibc free call.
 */
void NewFree(FP_FREE orgFuncptr, void* ptr, ADDRINT returnIp) {
	// Pop the call for this function
	if (!addrStack.empty()) {
		addrStack.pop();
	}
    if(ptr != NULL) {
        // Check the MemList
        int index  = ml.containsAddress(ptr);
        if(index >= 0) {
            MemoryAlloc alloc = ml.get(index);
            // Remove the space
            orgFuncptr(alloc.getUnderflowFence());
            fprintf(trace, "Freed the memory @ address %p successfully.\n", alloc.getUnderflowFence());
            // Remove the element from the list
            ml.removeMatching(alloc);
            stats.incFreeCount();
        } else {
            // Track the Invalid Free and the type of invalid free
            stats.incInvalidFreeCount();
            if(index == ERR_NOT_FOUND) {
                fprintf(trace, "Address = %p not found. Bad address or stack address used.\n", ptr);
                RecordAddrSource(returnIp - 4, "INVALID FREE - ERR_NOT_FOUND");
                stats.incFreeNotFoundCount();
            } else if(index == ERR_MID_CHUNK) {
                fprintf(trace, "Mid-chunk memory deallocation @ %p\n", ptr);
                RecordAddrSource(returnIp - 4, "INVALID FREE - ERR_MID_CHUNK");
                stats.incMidFreeChunkCount();
            } else if(index == ERR_FENCE_UNDERFLOW) {
                fprintf(trace, "Memory Fence Hit - Underflow @ %p\n", ptr);
                RecordAddrSource(returnIp - 4, "INVALID FREE - ERR_FENCE_UNDERFLOW");
                // stats.incFenceHitCount();
                // stats.incFenceUnderflowHitCount();
            } else if(index == ERR_FENCE_OVERFLOW) {
                fprintf(trace, "Memory Fence Hit - Overflow @ %p\n", ptr);
                RecordAddrSource(returnIp - 4, "INVALID FREE - ERR_FENCE_OVERFLOW");
                // stats.incFenceHitCount();
                // stats.incFenceOverflowHitCount();
            } else {
                // This should Never happen...
                fprintf(trace, "index = %d : Unable to deallocate the memory @ %p\n", index, ptr);
            }
        }
    } else {
        fprintf(trace, "Attempted to free NULL\n");
        RecordAddrSource(returnIp - 4, "INVALID FREE - NULL FREE");
        stats.incInvalidFreeCount();
        stats.incFreeNullCount();
    } 
}

/**
 * Pin calls this function every time a new img is loaded.
 * It is best to do probe replacement when the image is loaded,
 * because only one thread knows about the image at this time.
 */
void ImageLoad(IMG img, void *v) {
    if(!hasEnding(IMG_Name(img), "libc.so.6")) {
        return;
    }
    // Hook Functions
    HookMalloc(img);
    HookFree(img);
  	HookCalloc(img);
    HookRealloc(img);
}

/**
 * Helper method for hooking free during the ImageLoad.
 */
void HookFree(IMG img) {
    RTN rtn = RTN_FindByName(img, "free");
    if(RTN_Valid(rtn)) {
        fprintf(trace, "Replacing free in %s\n", IMG_Name(img).c_str());
        // Return type, cstype, function name, arguments...
        PROTO proto = PROTO_Allocate(PIN_PARG(void), CALLINGSTD_DEFAULT, "free", PIN_PARG(void*), PIN_PARG_END());
        // Replace free
        RTN_ReplaceSignature(rtn, AFUNPTR(NewFree),
            IARG_PROTOTYPE, 
            proto,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0, 
            IARG_RETURN_IP,
            IARG_END);
        // Free the function prototype
        PROTO_Free(proto);
    }
}

/**
 * Helper method for hooking malloc during the ImageLoad.
 */
void HookMalloc(IMG img) {
    // See if malloc() is present in the image.  If so, replace it.
    RTN rtnMalloc = RTN_FindByName(img, "malloc");
    
    if(RTN_Valid(rtnMalloc)) {
        fprintf(trace, "Replacing malloc in %s\n", IMG_Name(img).c_str());   
        PROTO proto_malloc = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT,
            "malloc", PIN_PARG(int), PIN_PARG_END());
        RTN_ReplaceSignature(rtnMalloc, AFUNPTR(NewMalloc),
            IARG_PROTOTYPE, proto_malloc,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
            IARG_RETURN_IP,
            IARG_END);
        // Free the function prototype.
        PROTO_Free(proto_malloc);
    }
}

/**
 * Helper method for hooking calloc during the ImageLoad.
 */
void HookCalloc(IMG img) {
    // See if calloc() is present in the image.  If so, replace it.
    RTN rtn = RTN_FindByName(img, "calloc");
    if(RTN_Valid(rtn)) {
        fprintf(trace, "Replacing calloc in %s\n", IMG_Name(img).c_str());   
        PROTO proto = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT,
            "calloc", PIN_PARG(size_t), PIN_PARG(size_t), PIN_PARG_END());
        RTN_ReplaceSignature(rtn, AFUNPTR(NewCalloc),
            IARG_PROTOTYPE, proto,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
			IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
            IARG_RETURN_IP,
            IARG_END);
        // Free the function prototype.
        PROTO_Free(proto);
    }
}

/**
 * Helper method for hooking realloc during the ImageLoad.
 */
void HookRealloc(IMG img) {
    // See if calloc() is present in the image.  If so, replace it.
    RTN rtn = RTN_FindByName(img, "realloc");
    if(RTN_Valid(rtn)) {
        fprintf(trace, "Replacing realloc in %s\n", IMG_Name(img).c_str());   
        PROTO proto = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT,
            "realloc", PIN_PARG(void*), PIN_PARG(size_t), PIN_PARG_END());
        RTN_ReplaceSignature(rtn, AFUNPTR(NewRealloc),
            IARG_PROTOTYPE, proto,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
			IARG_FUNCARG_ENTRYPOINT_VALUE, 1,
            IARG_RETURN_IP,
            IARG_END);
        // Free the function prototype.
        PROTO_Free(proto);
    }
}

/**
 * Prints out the usuage of this tool incase someone trys to compile and run this program,
 * instead of linking it with pin as a .so.
 */
INT32 Usage() {
    cerr << "This tool hooks and tracks the following glibc functions:" << endl;
    cerr << "malloc, calloc, realloc, and free." << endl;
    cerr << "Stats are collected about these memory allocations and usage." << endl;
    cerr << "Ths tool also detects stack smashing attempts, and logs those as well." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/** 
 * Sanity check that the tool isn't being run in threaded code.
 */
static VOID CheckThreadCount(THREADID threadIndex, CONTEXT *, INT32, VOID *)
{
    ASSERT (threadIndex==0, "This tool does not handle multiple threads\n");
}

/**
 * Main Function. 
 * Initializes pin, and starts the instrumented program.
 */
int main(INT32 argc, CHAR *argv[]) {
    // Initialize symbol processing
    PIN_InitSymbols();
    // Initialize pin
    if (PIN_Init(argc, argv)) 
        return Usage();
	// Register used for 	   
	scratchReg = PIN_ClaimToolRegister();
    if (!REG_valid(scratchReg)) {
        fprintf (stderr, "Cannot allocate a scratch register.\n");
        return 1;
    }
	// Check if we're running 32 or 64
	if (sizeof(size_t) != 8) {
		forceNoRAD = true;
		fprintf (stderr, "Disabling return address defender: does not yet work on 32 bit machines.\n");
	}
    // Open up the trace file
    trace = fopen(OUTPUT_LOG, "w");
    // Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction(ImageLoad, 0);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);
	PIN_AddThreadStartFunction(CheckThreadCount, 0);
    PIN_StartProgram();
    return 0;
}
