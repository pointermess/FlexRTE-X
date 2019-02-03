#include "Memory.h"

using namespace FlexRTE;

char Memory::RegisterLookupTable[18];
char Memory::MemoryStartPosition;

Memory::Memory(char * memory, unsigned int size)
{
    _Memory = memory;
    _Size = size;
}

unsigned int Memory::Read(const MemorySize size, const unsigned int position)
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

const unsigned char Memory::Read8(const unsigned int position)
{
    return _Memory[position];
}

const unsigned short Memory::Read16(const unsigned int position)
{
    return ((unsigned char)_Memory[position] << 8) + (unsigned char)_Memory[position + 1] << 16;
}

const unsigned int Memory::Read32(const unsigned int position)
{
    return ((unsigned char)_Memory[position] << 24) + ((unsigned char)_Memory[position + 1] << 16) + ((unsigned char)_Memory[position + 2] << 8) + (unsigned char)_Memory[position + 3];
}

void Memory::Write(const MemorySize size, const unsigned int position, const unsigned int value)
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

inline void Memory::Write8(const unsigned int position, const unsigned char value)
{
    _Memory[position] = value;
}

inline void Memory::Write16(const unsigned int position, const unsigned short value)
{
    _Memory[position] = value >> 8;
    _Memory[position + 1] = value;
}

inline void Memory::Write32(const unsigned int position, const unsigned int value)
{
    _Memory[position] = value >> 24;
    _Memory[position + 1] = value >> 16;
    _Memory[position + 2] = value >> 8;
    _Memory[position + 3] = value;
}

unsigned int FlexRTE::Memory::GetRegisterAddres(Register reg)
{
    return RegisterLookupTable[reg];
}

unsigned int FlexRTE::Memory::ReadRegister(Register reg)
{
    return Read(GetRegisterSize(reg), GetRegisterAddres(reg));
}

void FlexRTE::Memory::WriteRegister(Register reg, const unsigned int value)
{
    Write(GetRegisterSize(reg), GetRegisterAddres(reg), value);
}

unsigned char FlexRTE::Memory::ReadFlag()
{
    return Read8(MemoryFlagPosition);
}

void FlexRTE::Memory::WriteFlag(unsigned char value)
{
    Write8(MemoryFlagPosition, value);
}

unsigned int FlexRTE::Memory::GetEffectiveAddress(BinaryAddress binaryAddress)
{
    return 0;
}

///
const unsigned int Memory::FindAvailableHeapMemory(const unsigned int size)
{
    unsigned int address = _Size - 6;
    bool first = true;

    // loop until found (will break if found, or return 0 when end conditions are met)
    while (true)
    {
        // parse current heap section
        unsigned char heapUsed = Read8(address + 5);
        unsigned int heapSize = Read32(address + 1);


        if (heapUsed == 0)
        {
            if (heapSize == 0 || heapSize >= size)
                break;
        }

        // check if next section would be out of memory
        // if not, then decrese address to next heap section
        if (address - heapSize > address) return 0;
        address -= (heapSize + 6);
    }

    return address;
}

const unsigned int Memory::AllocateHeapMemory(const unsigned int size)
{
    unsigned int address = FindAvailableHeapMemory(size);

    // as the heap works by "hiding" its prepended informations
    // from the caller, we need to add them accordingly.
    Write8(address + 5, 1);
    Write32(address + 1, size);


    return address;
}

void Memory::FreeHeapMemory(const unsigned int address)
{
    unsigned char heapUsed = Read8(address + 5);
    unsigned char heapSize = Read32(address + 1);

    if (heapUsed == 1)
    {
        Write8(address + 5, 0);
    }
}

void Memory::InitializeLookupTable()
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

    // Defines the first available byte after all registers and flags.
    // 0  : eax (4 bytes)
    // 4  : ebx (4 bytes)
    // 8  : ecx (4 bytes)
    // 12 : edx (4 bytes)
    // 16 : esp (4 bytes)
    // 20 : ebp (4 bytes)
    // 24 : esi (4 bytes)
    // 28 : edi (4 bytes)
    // 32 : flags (1 byte)
    // 33 : MEMORY BEGIN
    MemoryStartPosition = 33;
}
