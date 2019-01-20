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
        finished = Programs[0]->Step();
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
    BinaryRegister  arg1 = program->ReadRegister(steps);
    BinaryConstant arg2 = program->ReadConstant(steps);

    program->_Memory->Write(arg1.Size, Memory::RegisterLookupTable[arg1.Register], arg2.Value);
}

// MOV REG REG
void _FlexRTE_Instruction_MOV_REG_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);
    BinaryRegister arg2 = program->ReadRegister(steps);

    program->_Memory->Write(arg1.Size, Memory::RegisterLookupTable[arg1.Register], program->_Memory->Read(arg2.Size, Memory::RegisterLookupTable[arg2.Register]));
}
#endif

FlexRTE::Engine::Engine()
{

    #if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    // Prefill lookup table with exception method
    for (int c = 0; c < 256; c++)
        InstructionLookupTable[c] = _FlexRTE_EXCEPTION_UNKNOWN_INSTRUCTION;

    InstructionLookupTable[faiMOV_REG_CONST] = _FlexRTE_Instruction_MOV_REG_CONST;
    InstructionLookupTable[faiMOV_REG_REG] = _FlexRTE_Instruction_MOV_REG_REG;
    #endif

    MemoryManager = new FlexRTE::MemoryManager();
    
}

FlexRTE::LoadProgramResult FlexRTE::Engine::LoadProgram(Program * program)
{
    // Prepare high performance build option
#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    program->LookupTable = InstructionLookupTable;
#endif

    #if (!OPTIONS_RTE_ENGINE_MULTIPROGRAM)
    if (ActiveProgram == nullptr)
    {
        // Set it as active program
        ActiveProgram = program;
        

        unsigned int size = 1024;
        unsigned int address = MemoryManager->GetMemory()->AllocateHeapMemory(size);

        program->_Memory = new Memory(MemoryManager->GetMemoryArray() + address - size, 1024);

        return LoadProgramResult::OK;
    }

    return LoadProgramResult::UnloadFirst;
    #else
    Programs[ProgramCount] = program;
    ActiveProgram = program;
    ProgramCount += 1;

    unsigned int size = 1024;
    unsigned int address = MemoryManager->GetMemory()->AllocateHeapMemory(size);

    program->_Memory = new Memory(MemoryManager->GetMemoryArray() + address - size, 1024);

    return LoadProgramResult::OK;
    #endif


}
