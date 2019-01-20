#pragma once
#include "MemoryManager.h"
#include "BuildOptions.h"
// Platform specific includes
#if (OPTIONS_TARGET_BASED_WINDOWS)
#include <stdio.h>
#include <stdlib.h>
#endif

using namespace FlexRTE;

MemoryManager::MemoryManager()
{
    Memory::InitializeLookupTable();
    _MemoryArray = new char[OPTIONS_RTE_MEMORY_FULLSIZE];
    _Memory = new Memory(_MemoryArray, OPTIONS_RTE_MEMORY_FULLSIZE);

    for (int c = 0; c < OPTIONS_RTE_MEMORY_FULLSIZE; c++)
    {
        _MemoryArray[c] = 0;
    }
}

MemoryManager::~MemoryManager()
{
    delete _MemoryArray;
    free(_Memory);
}

char * FlexRTE::MemoryManager::GetMemoryArray()
{
    return _MemoryArray;
}

Memory * FlexRTE::MemoryManager::GetMemory()
{
    return _Memory;
}
