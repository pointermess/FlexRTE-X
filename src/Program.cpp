
#include "Program.h"
#include "Types.h"
#include "DriverInterfaces/FileIO.h"

using namespace FlexRTE;

inline  char FlexRTE::Program::ReadByte(unsigned int * stepsTaken)
{
    *stepsTaken += 1;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 1];
#endif
}

short FlexRTE::Program::ReadWord(unsigned int * stepsTaken)
{
    *stepsTaken += 2;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return ((this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 1] << 8) + (this->_ProgramCode[this->_ProgramCounter + *stepsTaken]));
#endif
}

int FlexRTE::Program::ReadDWord(unsigned int * stepsTaken)
{
    *stepsTaken += 4;

    unsigned int beginAddr = this->_ProgramCounter + *stepsTaken - 4;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return ((((((this->_ProgramCode[beginAddr] << 8) + this->_ProgramCode[beginAddr + 1]) << 8) + this->_ProgramCode[beginAddr + 2]) << 8) + this->_ProgramCode[beginAddr + 3]);
#endif
}

FlexRTE::BinaryConstant FlexRTE::Program::ReadConstant(unsigned int * stepsTaken)
{
    BinaryConstant result;
    result.Size = (MemorySize)ReadByte(stepsTaken);

    switch (result.Size)
    {
    case msByte: result.Value = ReadByte(stepsTaken);
    case msWord: result.Value = ReadWord(stepsTaken);
    case msDWord: result.Value = ReadDWord(stepsTaken);
    }

    return result;
}

BinaryRegister FlexRTE::Program::ReadRegister(unsigned int * stepsTaken)
{
    return { msDWord, (Register)ReadByte(stepsTaken) };
}

FlexRTE::Program::Program()
{
}

bool FlexRTE::Program::Initialize()
{
    unsigned int steps = 0;

    int prefix = ReadDWord(&steps);

    // Check for "FAE" + 0x00 type information heaeder.
    if (prefix == 0x46414500)
    {
        char version = ReadByte(&steps);

        int headerSize = ReadDWord(&steps);
        int dataSize = ReadDWord(&steps);
        int textSize = ReadDWord(&steps);

        _ProgramCounter += steps;

        return true;
    }

    return false;
}

void FlexRTE::Program::LoadFromFile(const char * path)
{
    // needs to free last program from ram
    unsigned int size = 0;
#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    _ProgramCode = DriverInterface::FileIO::ReadFileBytes(path, &size);
#endif
    _ProgramSize = size;
}

bool FlexRTE::Program::Step()
{
    unsigned int steps = 1;

#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    int instruction = this->_ProgramCode[this->_ProgramCounter];
    LookupTable[instruction](this, &steps);
#endif
    return false;
}

void FlexRTE::Program::SetMemory(Memory & memory)
{
}

FlexRTE::Memory * FlexRTE::Program::GetMemory()
{
    return _Memory;
}
