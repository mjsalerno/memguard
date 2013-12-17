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
#include <set>
#include "memlist.h"
#include "memoryalloc.h"
#include "stats.h"
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

bool inMain = false;
FILE * trace;
MemList ml;
Stats stats;
// Library Counts 
int mallocNumber = -1;
int freeNumber = -1;

// Print a memory read record
VOID RecordHeapMemRead(VOID * ip, VOID * addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_IN_FENCE) {
        fprintf(trace,"##########BAD WRITE: %p \n", addr);
        cout << "BAD READ" << endl;
        stats.incInvalidReadCount();
    }
    //printf("heap read: %p\n", addr);
}

// Print a memory write record
VOID RecordHeapMemWrite(VOID * ip, VOID * addr) {
    int rtn = ml.containsAddress(addr);
    if(rtn == ERR_IN_FENCE) {
        fprintf(trace,"##########BAD WRITE: %p \n", addr);
        cout << "BAD WRITE" << endl;
        stats.incInvalidWriteCount();
    }    
    //printf("heap write: %p\n", addr);
}

VOID RecordStackMemRead(VOID * ip, VOID * addr) {    
    //printf("stack read: %p\n", addr);    
}

// Print a memory write record
VOID RecordStackMemWrite(VOID * ip, VOID * addr) {
    //printf("stack write: %p\n", addr); 
}

// Is called for every instruction and instruments reads and writes
VOID Instruction(INS ins, VOID *v) {
    if(!inMain || (mallocNumber <= 0 && freeNumber <=0))
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
VOID* NewMalloc(FP_MALLOC orgFuncptr, UINT32 arg0, ADDRINT returnIp) {
    // Call the relocated entry point of the original (replaced) routine.
    void* v = orgFuncptr(arg0 + (2 * DEFAULT_FENCE_SIZE));
    stats.incMallocCount();
    MemoryAlloc ma = ml.add(v, arg0, DEFAULT_FENCE_SIZE);
    fprintf(trace, "ADDED: %p %d \n", v, DEFAULT_FENCE_SIZE);
    return ma.getAddress();
}

void* NewCalloc(FP_CALLOC orgFuncptr, UINT32 arg0, UINT32 arg1, ADDRINT returnIp) {
    printf("New Calloc was called\n");
    return orgFuncptr(arg0, arg1);
}

void* NewRealloc(FP_REALLOC orgFuncptr, void* arg0, UINT32 arg1, ADDRINT returnIp) {
    printf("New Realloc was called\n");
    return orgFuncptr(arg0, arg1);
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
            // Dump all memory currently stored
            char buffer[1024];
            for(int i = 0; i < ml.size(); i++) {
                fprintf(trace, "%s\n", ml.get(i).toString(buffer, 1024));
            }
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
    if(!inMain) {
        inMain = IMG_IsMainExecutable(img);
        return;
    }
    // Hook Functions
    HookMalloc(img);
    HookFree(img);
    // HookCalloc(img);
    // HookRealloc(img);
}

void HookFree(IMG img) {
    RTN rtn = RTN_FindByName(img, "free");
    // TODO: Hack for skipping hooks to /lib64/ld-linux-x86-64.so.2
    if(RTN_Valid(rtn)) {
        freeNumber++;
    }
    if(RTN_Valid(rtn) && freeNumber > 0) {
        cout << "Replacing free in " << IMG_Name(img) << endl;
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
        mallocNumber++;
    }
    if(RTN_Valid(rtnMalloc) && mallocNumber > 0) {
        cout << "Replacing malloc in " << IMG_Name(img) << endl;   
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
        
    }
    if(RTN_Valid(rtn)) {
        cout << "Replacing calloc in " << IMG_Name(img) << endl;   
        PROTO proto = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT,
            "calloc", PIN_PARG(size_t), PIN_PARG(size_t), PIN_PARG_END());
        RTN_ReplaceSignature(rtn, AFUNPTR(NewCalloc),
            IARG_PROTOTYPE, proto,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
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
        
    }
    if(RTN_Valid(rtn)) {
        cout << "Replacing realloc in " << IMG_Name(img) << endl;   
        PROTO proto = PROTO_Allocate(PIN_PARG(void *), CALLINGSTD_DEFAULT,
            "realloc", PIN_PARG(void*), PIN_PARG(size_t), PIN_PARG_END());
        RTN_ReplaceSignature(rtn, AFUNPTR(NewRealloc),
            IARG_PROTOTYPE, proto,
            IARG_ORIG_FUNCPTR,
            IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
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
