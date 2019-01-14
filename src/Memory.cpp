#include "Memory.h"

char FlexRTE::Memory::RegisterLookupTable[18];
char FlexRTE::Memory::MemoryStartPosition;

FlexRTE::Memory::Memory(char * memory, unsigned int size)
{
    _Memory = memory;
    _Size = size;
}

unsigned int FlexRTE::Memory::Read(const MemorySize size, const unsigned int position)
{
    switch (size)
    {
    case (msByte): return Read8(position); break;
    case (msWord): return Read16(position); break;
    case (msDWord): return Read32(position); break;
    }
    return 0;

}

const unsigned char FlexRTE::Memory::Read8(const unsigned int position)
{
    return _Memory[position];
}

const unsigned short FlexRTE::Memory::Read16(const unsigned int position)
{
    return ((_Memory[position] << 8) + (_Memory[position + 1]));
}

const unsigned int FlexRTE::Memory::Read32(const unsigned int position)
{
    return ((((((_Memory[position] << 8) + _Memory[position + 1]) << 8) + _Memory[position + 2]) << 8) + _Memory[position + 3]);
}

void FlexRTE::Memory::Write(const MemorySize size, const unsigned int position, const unsigned int value)
{
    switch (size)
    {
    case (msDWord):
        Write32(position, value);
        break;
    }
}

inline void FlexRTE::Memory::Write32(const unsigned int position, const unsigned int value)
{
    _Memory[position] = value >> 24;
    _Memory[position + 1] = value >> 16;
    _Memory[position + 2] = value >> 8;
    _Memory[position + 3] = value;
}

void FlexRTE::Memory::InitializeLookupTable()
{
    // Initialize RegisterLookupTable.
    // Fills in the absolute memory position of each register.
    //
    // This is the new variant, it uses 36 bytes but has almost no overhead
    // and is a lot faster than the mothod used in the first version of FlexRTE.
    RegisterLookupTable[0] = 3;
    RegisterLookupTable[1] = 7;
    RegisterLookupTable[2] = 11;
    RegisterLookupTable[3] = 15;
    RegisterLookupTable[4] = 2;
    RegisterLookupTable[5] = 6;
    RegisterLookupTable[6] = 10;
    RegisterLookupTable[7] = 14;
    RegisterLookupTable[8] = 0;
    RegisterLookupTable[9] = 4;
    RegisterLookupTable[10] = 8;
    RegisterLookupTable[11] = 12;
    RegisterLookupTable[12] = 0;
    RegisterLookupTable[13] = 4;
    RegisterLookupTable[14] = 8;
    RegisterLookupTable[15] = 12;
    RegisterLookupTable[16] = 16;
    RegisterLookupTable[17] = 20;

    // Defines the first available byte after all registers.
    MemoryStartPosition = 24;
}
