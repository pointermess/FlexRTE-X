#pragma once
#include "BuildOptions.h"
namespace FlexRTE
{
    class Memory
    {
    private:
        char * _Memory;
        unsigned int _Size;
    public:
        Memory(char * memory, unsigned int size) { _Memory = memory; _Size = size; };


        // Basic read & write methods
        const unsigned int Read(const unsigned int position);
        inline const unsigned char Read8(const unsigned int position);
        inline const unsigned short Read16(const unsigned int position);
        inline const unsigned int Read32(const unsigned int position);

        void Write(const unsigned int position);
        inline void Write8(const unsigned int position, const unsigned char value);
        inline void Write16(const unsigned int position, const unsigned short value);
        inline void Write32(const unsigned int position, const unsigned int value);

        // Memory read & write functions

        // Stack methods
        void Push();

        // Heap methods
        const unsigned int FindFreeHeapMemory(const unsigned int size);
        const unsigned int AllocateHeapMemory(const unsigned int size);
        void FreeHeapMemory(const unsigned int address);
    };
}