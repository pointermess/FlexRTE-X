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
    case (msByte):
        return Read8(position);
        break;
    case (msWord):
        return Read16(position);
        break;
    case (msDWord):
        return Read32(position);
        break;
    }
    return 0;

}

const unsigned char FlexRTE::Memory::Read8(const unsigned int position)
{
    return _Memory[position];

    unsigned char * memory = (unsigned char*)(_Memory + position);
    return *memory;
}

const unsigned short FlexRTE::Memory::Read16(const unsigned int position)
{
    return ((_Memory[position] << 8) + (_Memory[position + 1]));

    unsigned short * memory = (unsigned short*)(_Memory + position);
    return *memory;
}

const unsigned int FlexRTE::Memory::Read32(const unsigned int position)
{
    return ((((((_Memory[position] << 8) + _Memory[position + 1]) << 8) + _Memory[position + 2]) << 8) + _Memory[position + 3]);

    unsigned int * memory = (unsigned int*)(_Memory + position);
    return *memory;
}

void FlexRTE::Memory::Write(const MemorySize size, const unsigned int position, const unsigned int value)
{
    switch (size)
    {
    case (msByte):
        Write8(position, value);
        break;
    case (msWord):
        Write16(position, value);
        break;
    case (msDWord):
        Write32(position, value);
        break;
    }
}

inline void FlexRTE::Memory::Write8(const unsigned int position, const unsigned char value)
{
    _Memory[position] = value;

    return;
    unsigned char * memory = (unsigned char*)(_Memory + position);
    *memory = value;
}

inline void FlexRTE::Memory::Write16(const unsigned int position, const unsigned short value)
{
    _Memory[position] = value >> 8;
    _Memory[position + 1] = value;
    return;

    unsigned short * memory = (unsigned short*)(_Memory + position);
    *memory = value;
}

inline void FlexRTE::Memory::Write32(const unsigned int position, const unsigned int value)
{
    _Memory[position] = value >> 24;
    _Memory[position + 1] = value >> 16;
    _Memory[position + 2] = value >> 8;
    _Memory[position + 3] = value;
    return;
    unsigned int * memory = (unsigned int*)(_Memory + position);
    *memory = value;
}

///
const unsigned int FlexRTE::Memory::FindAvailableHeapMemory(const unsigned int size)
{
    unsigned int address = _Size - 6;
    bool first = true;

    while (true)
    {
        unsigned char heapUsed = Read8(address + 5);
        unsigned int heapSize = Read32(address + 1);


        if (heapUsed == 0)
        {
            if (heapSize == 0 || heapSize >= size)
                break;
        }
        if (address - heapSize > address) return 0;
        address -= (heapSize + 6);
        

        
        /*
        if (heapUsed == 0)
        {
            if (heapSize == 0 || heapSize >= size)
            {
                // current address was never allocated or has enough space
                // so it can be (re)allocated
                return address - 5;
            }
            address -= heapSize - 5;
        }
        else if (heapUsed == 1)
        {
            if (address - heapSize - 5 > address) break;
            address -= heapSize - 5;
        }
        
        
        if (address - 5 > address) break;
        address -= 5;
        */
    }

    return address;
}

const unsigned int FlexRTE::Memory::AllocateHeapMemory(const unsigned int size)
{
    unsigned int address = FindAvailableHeapMemory(size);

    // as the heap works by "hiding" its prepended informations
    // from the caller, we need to add them accordingly.
    Write8(address + 5, 1);
    Write32(address + 1, size);


    return address;
}

void FlexRTE::Memory::FreeHeapMemory(const unsigned int address)
{
    unsigned char heapUsed = Read8(address + 5);
    unsigned char heapSize = Read32(address + 1);

    if (heapUsed == 1)
    {
        Write8(address + 5, 0);
    }
}

void FlexRTE::Memory::InitializeLookupTable()
{
#if (OPTIONS_RTE_MEMORY_REGISTERLOOKUPTABLE)
    // Initialize RegisterLookupTable.
    // Fills in the absolute memory position of each register.
    //
    // This is the new variant, it requires 36 bytes but has almost no overhead
    // and is a lot faster than the method used in the first version of FlexRTE.
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
#endif
    // Defines the first available byte after all registers.
    MemoryStartPosition = 24;
}
