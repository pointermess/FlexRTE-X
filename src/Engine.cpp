#include "Engine.h"


Flex::RTE::LoadProgramResult Flex::RTE::Engine::LoadProgram(Program * program)
{
#if (OPTIONS_RTE_ENGINE_MULTIPROGRAM == 0)
    if (ActiveProgram == nullptr)
    {
        ActiveProgram = program;
        return LoadProgramResult::OK;
    }

    return LoadProgramResult::UnloadFirst;
#else
#endif
}

bool Flex::RTE::Engine::UnloadProgram(Program * program)
{
    return false;
}

bool Flex::RTE::Engine::Step()
{
    return false;
}
