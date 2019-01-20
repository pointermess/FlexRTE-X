#pragma once
#include "BuildOptions.h"
#include "Types.h"
namespace FlexRTE
{
    class Memory
    {
    private:
        char * _Memory;
        unsigned int _Size;
    public:
        Memory(char * memory, unsigned int size);


        // Basic read & write methods
        unsigned int Read(const MemorySize size, const unsigned int position);
        inline const unsigned char Read8(const unsigned int position);
        inline const unsigned short Read16(const unsigned int position);
        inline const unsigned int Read32(const unsigned int position);

        void Write(const MemorySize size, const unsigned int position, const unsigned int value);
        inline void Write8(const unsigned int position, const unsigned char value);
        inline void Write16(const unsigned int position, const unsigned short value);
        inline void Write32(const unsigned int position, const unsigned int value);

        // Memory read & write functions

        // Stack methods
        void Push();

        // Heap methods
        const unsigned int FindAvailableHeapMemory(const unsigned int size);
        const unsigned int AllocateHeapMemory(const unsigned int size);
        void FreeHeapMemory(const unsigned int address);


        static char RegisterLookupTable[18];
        static char MemoryStartPosition;
        static void InitializeLookupTable();
    };
}