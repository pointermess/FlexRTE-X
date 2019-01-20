
#include "Program.h"
#include "Types.h"
#include "DriverInterfaces/FileIO.h"
#include "SystemCore.h"

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
    return ((this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 2] << 8) + (this->_ProgramCode[this->_ProgramCounter + *stepsTaken - 1]));
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
    case msByte:
        result.Value = ReadByte(stepsTaken);
        break;
    case msWord:
        result.Value = ReadWord(stepsTaken);
        break;
    case msDWord:
        result.Value = ReadDWord(stepsTaken);
        break;
    }

    return result;
}

BinaryRegister FlexRTE::Program::ReadRegister(unsigned int * stepsTaken)
{
    return { (MemorySize)ReadByte(stepsTaken), (Register)ReadByte(stepsTaken) };
}

void FlexRTE::Program::PrintExecutionReport()
{
    ConsoleIO::PrintF("------------------------------\n");
    unsigned int time = DriverInterface::SystemCore::Clock() - DriverInterface::SystemCore::StartTime;
    float diff = ((float)(time) / 1000000.0F) * 1000;
    ConsoleIO::PrintF("Program finished...\nExecution time: %.3f s\n------------------------------\n", diff);

    //ConsoleIO::PrintF("Memory Size: %u bytes\n\n", FPMemory_GetMemorySize(AProcessor->Memory));

    ConsoleIO::PrintF("Registers Data: \n");
    unsigned int ebx = GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farEBX]);
    ConsoleIO::PrintF("EAX Register: 0x%08x\n", GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farEAX]));
    ConsoleIO::PrintF("EBX Register: 0x%08x\n", ebx);
    ConsoleIO::PrintF("ECX Register: 0x%08x\n", GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farECX]));
    ConsoleIO::PrintF("EDX Register: 0x%08x\n", GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farEDX]));
    ConsoleIO::PrintF("ESP Register: 0x%08x\n", GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farESP]));
    ConsoleIO::PrintF("EBP Register: 0x%08x\n", GetMemory()->Read(msDWord, Memory::RegisterLookupTable[farEBP]));


    printf("\n");

    printf("Program Counter: %u\n", _ProgramCounter);

    printf("------------------------------\n");
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

    _ProgramCounter += steps;


    return _ProgramCounter >= _ProgramSize;
}

void FlexRTE::Program::SetMemory(Memory & memory)
{
}

FlexRTE::Memory * FlexRTE::Program::GetMemory()
{
    return _Memory;
}
