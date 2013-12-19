#ifndef MALLOC_MEM_H
#define MALLOC_MEM_H
/* pintool includes */
#include "pin.H"
/* libc++ headers */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <stack>
/* Utility Class Headers */
#include "memlist.h"
#include "memoryalloc.h"
#include "stats.h"
/* Define colors used for our error print statements */
#define BOLD_RED "\033[1m\033[31m"
#define BOLD_WHITE "\033[1m\033[37m"
#define RESET "\033[0m"
/* Typedefs for hooked functions */
typedef void* (*FP_MALLOC)(size_t);
typedef void (*FP_FREE)(void*);
typedef void* (*FP_CALLOC)(size_t, size_t);
typedef void* (*FP_REALLOC)(void*, size_t);
/* Hooking Functions */
void HookFree(IMG img);
void HookMalloc(IMG img);
void HookCalloc(IMG img);
void HookRealloc(IMG img);
/* Helper Functions */
bool hasEnding(string const &fullString, string const &ending);
void RecordAddrSource(ADDRINT address, string message);
#endif 