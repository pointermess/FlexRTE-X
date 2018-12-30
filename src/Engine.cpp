#include "Engine.h"

#include <iostream>

void tests(FlexRTE::Program * program)
{
    printf("test from test\n");
}
void tests2(FlexRTE::Program * program)
{
    printf("2222\n");
}
FlexRTE::Engine::Engine()
{
}

FlexRTE::LoadProgramResult FlexRTE::Engine::LoadProgram(Program * program)
{
#if (OPTIONS_RTE_ENGINE_MULTIPROGRAM == 0)
    if (ActiveProgram == nullptr)
    {
        ActiveProgram = program;
    #if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
        program->LookupTable = InstructionLookupTable;
    #endif
        return LoadProgramResult::OK;
    }

    return LoadProgramResult::UnloadFirst;
#else
#endif


}

bool FlexRTE::Engine::UnloadProgram(Program * program)
{
    return false;
}

bool FlexRTE::Engine::Step()
{
    return false;
}
