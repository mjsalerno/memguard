/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2013 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */

#include "pin.H"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include "memlist.h"
#include "memoryalloc.h"
#include "stats.h"
#include <stack>
using namespace std;

// Prototypes ; TODO: Move to Seperate Header file?
void HookFree(IMG img);
void HookMalloc(IMG img);
void HookCalloc(IMG img);
void HookRealloc(IMG img);

typedef VOID* (*FP_MALLOC)(size_t);
typedef void (*FP_FREE)(void*);
typedef void* (*FP_CALLOC)(size_t, size_t);
typedef void* (*FP_REALLOC)(void*, size_t);

bool hasEnding (std::string const &fullString, std::string const &ending);
void RecordAddrSource(ADDRINT address, string message);

bool inMain = false;
FILE * trace;
MemList ml;
Stats stats;

stack<ADDRINT> addrStack;

// Print a memory read record
VOID RecordHeapMemRead(ADDRINT ip, VOID * addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_IN_FENCE) {
        fprintf(trace,"##########BAD READ: %p \n", addr);
		RecordAddrSource(ip, "BAD READ");
        stats.incInvalidReadCount();
    }
}

// Print a memory write record
VOID RecordHeapMemWrite(ADDRINT ip, VOID * addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_IN_FENCE) {
        fprintf(trace,"##########BAD WRITE: %p \n", addr);
		RecordAddrSource(ip, "BAD WRITE");
        stats.incInvalidWriteCount();
    }    
}

// see http://software.intel.com/sites/landingpage/pintool/docs/58423/Pin/html/group__DEBUG__API.html
void RecordAddrSource(ADDRINT address, string message){
	INT32 column = 0;   // column number within the file.
	INT32 line = 0;     // line number within the file.
	string filename;    // source file name.
	PIN_LockClient();
	PIN_GetSourceLocation(address, &column, &line, &filename);
	PIN_UnlockClient();

    string rtnname = RTN_FindNameByAddress(address);
    // print only if source was found.
    if (!filename.empty()){
		cout << filename << ":" << dec << line << ":" << column << endl;
        cout << "Address: 0x" << hex << address << ": In function: ";
		cout << "'" << rtnname << "': ";
	}
	// Print Error message
	cout << "Error: " << message << endl << endl;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

VOID RecordStackMemRead(VOID * ip, VOID * addr) {    
    //printf("stack read: %p\n", addr);    
}

// Print a memory write record
VOID RecordStackMemWrite(VOID * ip, VOID * addr) {
    //printf("stack write: %p\n", addr); 
}


VOID RecordCallIns(ADDRINT nextip)
{
	addrStack.push(nextip);
	//cout << "Calling  : 0x" << hex << nextip << endl;
}

VOID RecordReturnIns(ADDRINT ip, ADDRINT retip)
{
	ADDRINT originval;
	if(addrStack.empty()){
		RecordAddrSource(ip, "TOO MANY RETURNS");
		stats.incInvalidReturnCount();
		fprintf(trace, "ERROR: TOO MANY RETURNS\n");
	}
	else {
		originval = addrStack.top();
		addrStack.pop();
		if(originval != retip){
			char errstr[128];
			snprintf(errstr, 128, "RETURN ADDRESS CHANGED: expected target %p, actual return target %p", (void *)originval, (void *)retip);
			RecordAddrSource(ip, (string)errstr);
			stats.incInvalidReturnCount();
			fprintf(trace, "ERROR: %s\n", errstr);
		}
	}	
	//cout << "Returning: 0x" << hex << retip << endl;
}

VOID SetInMain(void){
	inMain = true;
    fprintf(trace, "Main execution started\n");
}

// Is called for every instruction and instruments reads and writes
VOID Instruction(INS ins, VOID *v) {
	// RETURN ADDRESS DEFENDER
	if (INS_IsCall(ins)) {
		INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
			AFUNPTR(RecordCallIns),
			IARG_ADDRINT, INS_NextAddress(ins),
			IARG_END);
	} else if (INS_IsRet(ins)) {
		INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
			AFUNPTR(RecordReturnIns),
            IARG_INST_PTR,
			IARG_RETURN_IP,
			IARG_END);
	}
    if(!inMain)
        return;
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

VOID Fini(INT32 code, VOID *v) {
    fprintf(trace, "#eof \n");
    // Display the stats
    stats.displayResults(ml, trace);
    fclose(trace);
}

// This is the replacement routine.
VOID* NewMalloc(FP_MALLOC orgFuncptr, size_t arg0, ADDRINT returnIp) {
    // Call the relocated entry point of the original (replaced) routine.
    void* v = orgFuncptr(arg0 + (2 * DEFAULT_FENCE_SIZE));
    stats.incMallocCount();
    MemoryAlloc ma = ml.add(v, arg0, DEFAULT_FENCE_SIZE);
    fprintf(trace, "ADDED: %p %d \n", v, DEFAULT_FENCE_SIZE);
    return ma.getAddress();
}

void* NewCalloc(FP_CALLOC libc_calloc, size_t arg0, size_t arg1, ADDRINT returnIp) {
    // Calculate the size in bytes
    size_t bytes = (arg0 * arg1);
    size_t totalSize = bytes + (2 * DEFAULT_FENCE_SIZE);
	void *ptr = libc_calloc(totalSize, 1);
    // Create a new MemoryAlloc
    MemoryAlloc ma = ml.add(ptr, bytes, DEFAULT_FENCE_SIZE);
    // Add the information to the trace file
    fprintf(trace, "ADDED: %p %d \n", ptr, DEFAULT_FENCE_SIZE);
    stats.incMallocCount();
    return ma.getAddress();
}

void* NewRealloc(FP_REALLOC orgFuncptr, void* arg0, size_t arg1, ADDRINT returnIp) {
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
			stats.incMallocCount();
    		fprintf(trace, "REALLOCED: %p, TOTAL SIZE: %zu\n", newptr, arg1 +(2 * DEFAULT_FENCE_SIZE));
    		return ma.getAddress();           
        } else if(index == ERR_NOT_FOUND) {
            // This currently gets hit since a blacklist is being used 
            fprintf(trace, "Address = %p not found. Bad address in realloc.\n", arg0);
        } else if(index == ERR_MID_CHUNK) {
            fprintf(trace, "Mid-chunk memory in realloc @ %p\n", arg0);
			stats.incMidFreeChunkCount();
        } else if(index == ERR_IN_FENCE) {
            fprintf(trace, "Memory Fence Hit in realloc @ %p\n", arg0);
            stats.incFenceHitCount();
        } else {
            // This should Never happen...
            fprintf(trace, "index = %d : Unable to realloc the memory @ %p\n", index, arg0);
        }
    } else {
        // arg0 == NULL so realloc acts as malloc(arg1)
		void* newptr = orgFuncptr(arg0, arg1 + (2 * DEFAULT_FENCE_SIZE));
		stats.incMallocCount();
		MemoryAlloc ma = ml.add(newptr, arg1, DEFAULT_FENCE_SIZE);
		fprintf(trace, "ADDED: %p, TOTAL SIZE: %zu \n", newptr, arg1 +(2 * DEFAULT_FENCE_SIZE));
		return ma.getAddress();
    }
	return NULL;
}

void NewFree(FP_FREE orgFuncptr, void* ptr, ADDRINT returnIp) {
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
        } else if(index == ERR_NOT_FOUND) {
            // This currently gets hit since a blacklist is being used 
            fprintf(trace, "Address = %p not found. Bad address or stack address used.\n", ptr);
            stats.incInvalidFreeCount();
            /*
            // Dump all memory currently stored
            char buffer[1024];
            for(int i = 0; i < ml.size(); i++) {
                fprintf(trace, "%s\n", ml.get(i).toString(buffer, 1024));
            }
            */
        } else if(index == ERR_MID_CHUNK) {
            fprintf(trace, "Mid-chunk memory deallocation @ %p\n", ptr);
            stats.incMidFreeChunkCount();
        } else if(index == ERR_IN_FENCE) {
            fprintf(trace, "Memory Fence Hit @ %p\n", ptr);
            stats.incFenceHitCount();
        } else {
            // This should Never happen...
            fprintf(trace, "index = %d : Unable to deallocate the memory @ %p\n", index, ptr);
        }
    } else {
        fprintf(trace, "Attempted to free NULL\n");
        stats.incFreeNullCount();
    }
}


// Pin calls this function every time a new img is loaded.
// It is best to do probe replacement when the image is loaded,
// because only one thread knows about the image at this time.
VOID ImageLoad(IMG img, VOID *v) {
    if(IMG_IsMainExecutable(img)) {
		RTN rtn = RTN_FindByName(img, "main");
        RTN_Open(rtn);//Must open RTN API before examining instructions
		INS ins = RTN_InsHead(rtn);
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)SetInMain, IARG_END);
        RTN_Close(rtn);
    }

    if(!hasEnding(IMG_Name(img), "libc.so.6")) {
        return;
    }

    // Hook Functions
    HookMalloc(img);
    HookFree(img);
  	HookCalloc(img);
    HookRealloc(img);
}

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
            IARG_FUNCARG_ENTRYPOINT_VALUE, 
            0, 
            IARG_RETURN_IP,
            IARG_END);
        // Free the function prototype
        PROTO_Free(proto);
    }
}

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

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage() {
    cerr << "This tool demonstrates how to replace an original" << endl;
    cerr << " function with a custom function defined in the tool " << endl;
    cerr << " using probes.  The replacement function has a different " << endl;
    cerr << " signature from that of the original replaced function." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main: Initialize and start Pin in Probe mode.                         */
/* ===================================================================== */

int main(INT32 argc, CHAR *argv[]) {
    // Initialize symbol processing
    PIN_InitSymbols();
    // Initialize pin
    if (PIN_Init(argc, argv)) 
        return Usage();
    // Open up the trace file
    trace = fopen("zzz.out", "w");
    // Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction(ImageLoad, 0);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();
    return 0;
}
