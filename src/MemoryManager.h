#pragma once
#include "BuildOptions.h"
#include "Memory.h"

namespace FlexRTE
{
    class MemoryManager
    {
    private:
        char * _MemoryArray;
        unsigned int _ReservedMemory = 0;

        Memory * _Memory;
    public:
        MemoryManager();
        ~MemoryManager();

        char * GetMemoryArray();

        Memory* GetMemory();
    };
}