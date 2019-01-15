#include "Engine.h"

#include <iostream>
#include "Types.h"

using namespace FlexRTE;

bool FlexRTE::Engine::UnloadProgram(Program * program)
{
    return false;
}

void FlexRTE::Engine::PrintExecutionReport()
{
    ActiveProgram->PrintExecutionReport();
}

bool FlexRTE::Engine::Step()
{
    return false;
}

void FlexRTE::Engine::Execute()
{
    DriverInterface::SystemCore::Initialize();
    bool finished = false;

    while (!finished)
    {
    finished = ActiveProgram->Step();
    }
}

#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
// Exceptions
void _FlexRTE_EXCEPTION_UNKNOWN_INSTRUCTION(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned char instruction = program->ReadByte(steps);

    std::cout << "[EXCEPTION] Instruction '" << (int)instruction << "' unknown." << std::endl;
}

// Instructions 
// MOV REG CONST
void _FlexRTE_Instruction_MOV_REG_CONST(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister  reg = program->ReadRegister(steps);
    BinaryConstant con = program->ReadConstant(steps);

    program->_Memory->Write(reg.Size, Memory::RegisterLookupTable[reg.Register], con.Value);
}
#endif

FlexRTE::Engine::Engine()
{

    #if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    // Prefill lookup table with exception method
    for (int c = 0; c < 256; c++)
        InstructionLookupTable[c] = _FlexRTE_EXCEPTION_UNKNOWN_INSTRUCTION;

    InstructionLookupTable[faiMOV_REG_CONST] = _FlexRTE_Instruction_MOV_REG_CONST;
    #endif

    MemoryManager = new FlexRTE::MemoryManager();
    
}

FlexRTE::LoadProgramResult FlexRTE::Engine::LoadProgram(Program * program)
{
    #if (OPTIONS_RTE_ENGINE_MULTIPROGRAM == 0)
    if (ActiveProgram == nullptr)
    {
        // Set it as active program
        ActiveProgram = program;
        
        // Prepare high performance build option
    #if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
        program->LookupTable = InstructionLookupTable;
    #endif

        program->_Memory = new Memory(MemoryManager->GetMemoryArray() + 64, 1024);

        return LoadProgramResult::OK;
    }

    return LoadProgramResult::UnloadFirst;
    #else
    #endif


}
