#pragma once

#include "BuildOptions.h"
#include "Memory.h"

namespace FlexRTE
{
#pragma once
    class Program;
    typedef void(*ExecuteInstructionMethod)(Program * program);
    enum ProgramPriority
    {
        VeryLow,
        Low,
        Normal,
        High,
        VeryHigh
    };

    class Program
    {
    private:
        Memory * _Memory;


#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
        char* _ProgramCode;
#endif
        unsigned int _ProgramSize = 0;
        unsigned int _ProgramCounter = 0;

        // Read from program code
        // Basic types
        char ReadByte(unsigned int * stepsTaken);
        short ReadWord(unsigned int * stepsTaken);
        int ReadDWord(unsigned int * stepsTaken);

        // Complex types
        char ReadConstant(unsigned int * stepsTaken);
        char ReadRegister(unsigned int * stepsTaken);
        char ReadAddress(unsigned int * stepsTaken);


    public:

#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
        ExecuteInstructionMethod * LookupTable;
#endif
        ProgramPriority Priority = ProgramPriority::Normal;

        Program();

        bool Initialize();

        void LoadFromFile(const char * path);

        unsigned int GetProgramCounter() { unsigned int test = 0; return ReadDWord(&test); };

        bool Step();

        void SetMemory(Memory& memory);
        Memory * GetMemory();
    };
}