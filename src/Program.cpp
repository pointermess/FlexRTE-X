
#include "Program.h"
#include "DriverInterfaces/FileIO.h"

char Flex::RTE::Program::ReadByte(unsigned int * stepsTaken)
{
    *stepsTaken += 1;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 1];
#endif
}

short Flex::RTE::Program::ReadWord(unsigned int * stepsTaken)
{
    *stepsTaken += 2;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return ((this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 1] << 8) + (this->_ProgramCode[this->_ProgramCounter + *stepsTaken]));
#endif
}

int Flex::RTE::Program::ReadDWord(unsigned int * stepsTaken)
{
    *stepsTaken += 4;

    unsigned int beginAddr = this->_ProgramCounter + *stepsTaken - 4;

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    return ((((((this->_ProgramCode[beginAddr] << 8) + this->_ProgramCode[beginAddr + 1]) << 8) + this->_ProgramCode[beginAddr + 2]) << 8) + this->_ProgramCode[beginAddr + 3]);
#endif
}

Flex::RTE::Program::Program()
{
}

void Flex::RTE::Program::LoadFromFile(const char * path)
{
    // needs to free last program from ram
    unsigned int size = 0;
#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
    _ProgramCode = Flex::RTE::FileIO::ReadFileBytes(path, &size);
#endif
    _ProgramSize = size;
}

bool Flex::RTE::Program::Step()
{
    return false;
}

void Flex::RTE::Program::SetMemory(Memory & memory)
{
}
