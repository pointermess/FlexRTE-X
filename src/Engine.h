#include "BuildOptions.h"
#include "Program.h"
#include "MemoryManager.h"
#include "LookupTable.h"
#include "SystemCore.h"

#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
#endif

namespace FlexRTE
{
#pragma once

    typedef void(*ExecuteInstructionMethod)(Program * program, unsigned int * steps);
    enum LoadProgramResult
    {
        Error,
        OK,
        UnloadFirst
    };

    class Engine
    {
    private:
        Program * ActiveProgram;
#if (OPTIONS_RTE_ENGINE_MULTIPROGRAM == 0)
#else   
        Program * Programs;
        unsigned char ProgramIndex = 0;
        unsigned char ProgramCount = 0;
#endif


#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
        ExecuteInstructionMethod * InstructionLookupTable = new ExecuteInstructionMethod[256];
#endif
    public:
        MemoryManager * MemoryManager;
        Engine();
        LoadProgramResult LoadProgram(Program* program);
        bool UnloadProgram(Program * program);

        void PrintExecutionReport();

        bool Step();
        void Execute();
    };
}