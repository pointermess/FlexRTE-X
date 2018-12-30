#pragma once
#include "MemoryManager.h"
#include "BuildOptions.h"
// Platform specific includes
#if (OPTIONS_TARGET_BASED_WINDOWS)
#include <stdio.h>
#include <stdlib.h>
#endif


FlexRTE::MemoryManager::MemoryManager()
{
    _MemoryArray = new char[OPTIONS_RTE_MEMORY_FULLSIZE];
    _Memory = new Memory(_MemoryArray, OPTIONS_RTE_MEMORY_FULLSIZE);
}

FlexRTE::MemoryManager::~MemoryManager()
{
    delete _MemoryArray;
    free(_Memory);
}
