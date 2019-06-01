#include "Engine.h"

#include <iostream>
#include "Types.h"
#include "ConsoleIO.h"

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

    ConsoleIO::PrintF("[EXCEPTION] Instruction '%u' (0x%02x) at position '%u' (0x%08x) is unknown.\n", instruction, instruction, program->GetProgramCounter(), program->GetProgramCounter());
}

// Pseudo-Instructions
void _FlexRTE_PseudoInstruction_DB(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    unsigned int currentAddress = program->_Memory->GetCurrentStackAddress();

    for (int c = 0; c < size; c++)
    {
        char value = program->ReadByte(steps);

        program->_Memory->Write8(currentAddress, value);

        currentAddress += 1;
    }

    program->_Memory->WriteRegister(farESP, currentAddress);
}

void _FlexRTE_PseudoInstruction_DW(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    unsigned int currentAddress = program->_Memory->GetCurrentStackAddress();

    for (int c = 0; c < size; c++)
    {
        short value = program->ReadWord(steps);

        program->_Memory->Write16(currentAddress, value);

        currentAddress += 2;
    }

    program->_Memory->WriteRegister(farESP, currentAddress);
}

void _FlexRTE_PseudoInstruction_DD(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    unsigned int currentAddress = program->_Memory->GetCurrentStackAddress();

    for (int c = 0; c < size; c++)
    {
        int value = program->ReadDWord(steps);

        program->_Memory->Write32(currentAddress, value);

        currentAddress += 4;
    }

    program->_Memory->WriteRegister(farESP, currentAddress);
}

void _FlexRTE_PseudoInstruction_RESB(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    program->_Memory->WriteRegister(farESP, program->_Memory->ReadRegister(farESP) + size);
}

void _FlexRTE_PseudoInstruction_RESW(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    program->_Memory->WriteRegister(farESP, program->_Memory->ReadRegister(farESP) + size * 2);
}

void _FlexRTE_PseudoInstruction_RESD(FlexRTE::Program * program, unsigned int * steps)
{
    unsigned int size = program->ReadDWord(steps);

    program->_Memory->WriteRegister(farESP, program->_Memory->ReadRegister(farESP) + size * 4);
}

// Instructions 
// MOV REG CONST
void _FlexRTE_Instruction_MOV_REG_CONST(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister  arg1 = program->ReadRegister(steps);
    BinaryConstant arg2 = program->ReadConstant(steps);

    program->_Memory->WriteRegister(arg1.Register, arg2.Value);
}

// MOV REG REG
void _FlexRTE_Instruction_MOV_REG_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);
    BinaryRegister arg2 = program->ReadRegister(steps);

    program->_Memory->Write(
        arg1.Size, Memory::RegisterLookupTable[arg1.Register],
        program->_Memory->Read(arg2.Size, Memory::RegisterLookupTable[arg2.Register])
    );
}

// MOV REG ADDR
void _FlexRTE_Instruction_MOV_REG_ADDR(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);
    BinaryAddress arg2 = program->ReadAddress(steps);


     program->_Memory->Write(arg1.Size, Memory::RegisterLookupTable[arg1.Register], program->_Memory->Read(arg2.Size, arg2.GetEffectiveAddress()));
}

// LEA REG ADDR
void _FlexRTE_Instruction_LEA_REG_ADDR(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);
    BinaryAddress arg2 = program->ReadAddress(steps);

    program->_Memory->Write(arg1.Size, Memory::RegisterLookupTable[arg1.Register], arg2.GetEffectiveAddress());
}

// PUSH CONST
void _FlexRTE_Instruction_PUSH_CONST(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    program->_Memory->Push(arg1.Value);
}

// PUSH REG
void _FlexRTE_Instruction_PUSH_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);

    program->_Memory->Push(program->_Memory->ReadRegister(arg1.Register));
}

// POP REG
void _FlexRTE_Instruction_POP_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);

    unsigned int value = program->_Memory->Pop();

    program->_Memory->WriteRegister(arg1.Register, value);
}

// ADD REG CONST
void _FlexRTE_Instruction_ADD_REG_CONST(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister  arg1 = program->ReadRegister(steps);
    BinaryConstant arg2 = program->ReadConstant(steps);


    program->_Memory->WriteRegister(arg1.Register, program->_Memory->ReadRegister(arg1.Register) + arg2.Value);
}

// CMP REG CONST
void _FlexRTE_Instruction_CMP_REG_CONST(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister  arg1 = program->ReadRegister(steps);
    BinaryConstant arg2 = program->ReadConstant(steps);

    unsigned int val0 = program->_Memory->ReadRegister(arg1.Register);
    unsigned int val1 = arg2.Value;


    program->_Memory->WriteFlag((val0 == val1) | (val0 > val1) << 1);
}

// CMP REG REG
void _FlexRTE_Instruction_CMP_REG_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister  arg1 = program->ReadRegister(steps);
    BinaryRegister arg2 = program->ReadRegister(steps);

    unsigned int val0 = program->_Memory->ReadRegister(arg1.Register);
    unsigned int val1 = program->_Memory->ReadRegister(arg2.Register);


    program->_Memory->WriteFlag((val0 == val1) | (val0 > val1) << 1);
}

// JMP LABEL
void _FlexRTE_Instruction_JMP_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    program->SetProgramCounter(arg1.Value);
}

// JE LABEL
void _FlexRTE_Instruction_JE_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (program->_Memory->ReadFlag() & 0x1)
        program->SetProgramCounter(arg1.Value);
} 

void _FlexRTE_Instruction_JNE_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (!(program->_Memory->ReadFlag() & 0x1))
        program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_JG_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (program->_Memory->ReadFlag() & 0x2)
        program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_JGE_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (program->_Memory->ReadFlag() & 0x3)
        program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_JL_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (!(program->_Memory->ReadFlag() & 0x3))
        program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_JLE_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    if (!(program->_Memory->ReadFlag() & 0x2))
        program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_CALL_LABEL(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryConstant arg1 = program->ReadConstant(steps);

    unsigned int programCounter = program->GetProgramCounter() + *steps;

    program->_Memory->Push(programCounter);
    
    program->SetProgramCounter(arg1.Value);
}

void _FlexRTE_Instruction_CALL_ADDR(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryAddress arg1 = program->ReadAddress(steps);

    unsigned int programCounter = program->GetProgramCounter() + *steps;

    program->_Memory->Push(programCounter);

    programCounter = program->_Memory->Read32(arg1.GetEffectiveAddress());

    program->SetProgramCounter(programCounter);
}

void _FlexRTE_Instruction_RET(FlexRTE::Program * program, unsigned int * steps)
{
    program->SetProgramCounter(program->_Memory->Pop());
}


// DBG REG
void _FlexRTE_Instruction_DBG_REG(FlexRTE::Program * program, unsigned int * steps)
{
    BinaryRegister arg1 = program->ReadRegister(steps);


    ConsoleIO::PrintF("Debug Value: %u\n", arg1);
}


#endif

FlexRTE::Engine::Engine()
{

    #if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    // Prefill lookup table with exception method
    for (int c = 0; c < 256; c++)
        InstructionLookupTable[c] = _FlexRTE_EXCEPTION_UNKNOWN_INSTRUCTION;


    InstructionLookupTable[fapiDB] = _FlexRTE_PseudoInstruction_DB;
    InstructionLookupTable[fapiDW] = _FlexRTE_PseudoInstruction_DW;
    InstructionLookupTable[fapiDD] = _FlexRTE_PseudoInstruction_DD;
    InstructionLookupTable[fapiRESB] = _FlexRTE_PseudoInstruction_RESB;
    InstructionLookupTable[fapiRESW] = _FlexRTE_PseudoInstruction_RESW;
    InstructionLookupTable[fapiRESD] = _FlexRTE_PseudoInstruction_RESD;

    InstructionLookupTable[faiMOV_REG_CONST] = _FlexRTE_Instruction_MOV_REG_CONST;
    InstructionLookupTable[faiMOV_REG_REG] = _FlexRTE_Instruction_MOV_REG_REG;
    InstructionLookupTable[faiMOV_REG_ADDR] = _FlexRTE_Instruction_MOV_REG_ADDR;

    InstructionLookupTable[faiLEA_REG_ADDR] =  _FlexRTE_Instruction_LEA_REG_ADDR;

    InstructionLookupTable[faiPUSH_CONST] = _FlexRTE_Instruction_PUSH_CONST;
    InstructionLookupTable[faiPUSH_REG] = _FlexRTE_Instruction_PUSH_REG;
    InstructionLookupTable[faiPOP_REG] = _FlexRTE_Instruction_POP_REG;

    InstructionLookupTable[faiADD_REG_CONST] = _FlexRTE_Instruction_ADD_REG_CONST;

    InstructionLookupTable[faiCMP_REG_CONST] = _FlexRTE_Instruction_CMP_REG_CONST;
    InstructionLookupTable[faiCMP_REG_REG] = _FlexRTE_Instruction_CMP_REG_REG;

    InstructionLookupTable[faiJMP_LABEL] = _FlexRTE_Instruction_JMP_LABEL;
    InstructionLookupTable[faiJE_LABEL] = _FlexRTE_Instruction_JE_LABEL;
    InstructionLookupTable[faiJNE_LABEL] = _FlexRTE_Instruction_JNE_LABEL;
    InstructionLookupTable[faiJL_LABEL] = _FlexRTE_Instruction_JL_LABEL;
    InstructionLookupTable[faiJLE_LABEL] = _FlexRTE_Instruction_JLE_LABEL;
    InstructionLookupTable[faiJG_LABEL] = _FlexRTE_Instruction_JG_LABEL;
    InstructionLookupTable[faiJGE_LABEL] = _FlexRTE_Instruction_JGE_LABEL;


    InstructionLookupTable[faiCALL_LABEL] = _FlexRTE_Instruction_CALL_LABEL;
    InstructionLookupTable[faiCALL_ADDR] = _FlexRTE_Instruction_CALL_ADDR;
    InstructionLookupTable[faiRET] = _FlexRTE_Instruction_RET;

    InstructionLookupTable[faiDBG_REG] = _FlexRTE_Instruction_DBG_REG;
    #endif

    MemoryManager = new FlexRTE::MemoryManager();
}

FlexRTE::LoadProgramResult FlexRTE::Engine::LoadProgram(Program * program)
{
    // Prepare high performance build option
#if (OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE == 1)
    program->LookupTable = InstructionLookupTable;
#endif

    // Branching for multiple programs build option
#if (!OPTIONS_RTE_ENGINE_MULTIPROGRAM)
    if (ActiveProgram == nullptr)
    {
        // Set it as active program
        ActiveProgram = program;
        

        unsigned int address = MemoryManager->GetMemory()->AllocateHeapMemory(OPTIONS_RTE_MEMORY_APPSIZE);
        program->_Memory = new Memory(MemoryManager->GetMemoryArray() + address - size, OPTIONS_RTE_MEMORY_APPSIZE);

        return LoadProgramResult::OK;
    }

    return LoadProgramResult::UnloadFirst;
#else
    Programs[ProgramCount] = program;
    ActiveProgram = program;
    ProgramCount += 1;

    unsigned int address = MemoryManager->GetMemory()->AllocateHeapMemory(OPTIONS_RTE_MEMORY_APPSIZE);
    program->_Memory = new Memory(MemoryManager->GetMemoryArray() + address - OPTIONS_RTE_MEMORY_APPSIZE, OPTIONS_RTE_MEMORY_APPSIZE);
    program->_Memory->WriteRegister(farESP, Memory::MemoryStartPosition);

    return LoadProgramResult::OK;
#endif


}
