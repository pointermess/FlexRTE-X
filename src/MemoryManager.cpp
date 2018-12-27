#pragma once
#include "MemoryManager.h"
#include "BuildOptions.h"
// Platform specific includes
#if (OPTIONS_TARGET_BASED_WINDOWS)
#include <stdio.h>
#include <stdlib.h>
#endif


Flex::RTE::MemoryManager::MemoryManager()
{
    _MemoryArray = new char[OPTIONS_RTE_MEMORY_FULLSIZE];
    _Memory = new Memory(_MemoryArray, OPTIONS_RTE_MEMORY_FULLSIZE);
}

Flex::RTE::MemoryManager::~MemoryManager()
{
    delete _MemoryArray;
    free(_Memory);
}
