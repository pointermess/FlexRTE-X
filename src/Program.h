#pragma once

#include "BuildOptions.h"
#include "Memory.h"
#include "Types.h"
#include "ConsoleIO.h"

#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
#include "DriverInterfaces/FileIO.h"
#endif
namespace FlexRTE
{
#pragma once
    class Program;
    typedef void(*ExecuteInstructionMethod)(Program * program, unsigned int * steps);
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


#if (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 0)
        DriverInterface::FileStream * _ProgramFileStream;
#elif (OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY == 1)
        unsigned char* _ProgramCode;
#endif
        unsigned int _ProgramSize = 0;
        unsigned int _ProgramCounter = 0;

        bool _ProgramJumped = false;


    public:
        Memory * _Memory;
#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
        ExecuteInstructionMethod * LookupTable = new ExecuteInstructionMethod[255];
#endif
        ProgramPriority Priority = ProgramPriority::Normal;

        Program();

        bool Initialize();

        void LoadFromFile(const char * path);

        unsigned int GetProgramCounter() { return _ProgramCounter; };
        void SetProgramCounter(unsigned int programCounter) { _ProgramCounter = programCounter; _ProgramJumped = true; };

        bool Step();

        void SetMemory(Memory& memory);
        Memory * GetMemory();

        // Read from program code
        // Basic types
        inline char ReadByte(unsigned int * stepsTaken);
        inline short ReadWord(unsigned int * stepsTaken);
        inline int ReadDWord(unsigned int * stepsTaken);

        // Complex types
        BinaryConstant ReadConstant(unsigned int * stepsTaken);
        BinaryRegister ReadRegister(unsigned int * stepsTaken);
        BinaryAddress ReadAddress(unsigned int * stepsTaken);
        //char ReadAddress(unsigned int * stepsTaken);

        void PrintExecutionReport();
    };
}