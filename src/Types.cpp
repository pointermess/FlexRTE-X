#include "Types.h"
using namespace FlexRTE;
unsigned int FlexRTE::BinaryAddress::GetEffectiveAddress()
{
    OperationType opType = GetOperationType();

    if (opType == otAddition)
    {
        return LeftValue + RightValue;
    }
    else if (opType == otSubtraction)
    {
        return LeftValue - RightValue;
    }
    else
    {
        return LeftValue;
    }
}
OperandType FlexRTE::BinaryAddress::GetLeftOperandType()
{
    return (OperandType)(unsigned char)(this->Struct >> 5);
}

OperandType FlexRTE::BinaryAddress::GetRightOperandType()
{
    return (OperandType)(unsigned char)((unsigned char)(this->Struct << 5) >> 5);
}

OperationType FlexRTE::BinaryAddress::GetOperationType()
{
    return (OperationType)(unsigned char)((unsigned char)(this->Struct << 3) >> 6);
}

MemorySize FlexRTE::GetRegisterSize(Register reg)
{
    if (reg >= 0 && reg <= 7)
        return msByte;
    if (reg >= 8 && reg <= 11)
        return msWord;
    if (reg >= 12)
        return msDWord;
}
