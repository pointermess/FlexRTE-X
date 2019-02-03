#pragma once


namespace FlexRTE {

    enum MemorySize { msUndefined = 0, msByte = 1, msWord = 2, msDWord = 4 };

    // comma on new line is to fix buggy visual studio formatting......
    enum Register {
        farUnknown = -1,
        // General purpose registers

        // Least significant bytes 000000[00]
        // floor(n / 4) = 0
        farAL = 0, // mod 4 = 0
        farBL = 1, // mod 4 = 1
        farCL = 2, // mod 4 = 2
        farDL = 3 // mod 4 = 3
        ,
        // Significant byte of lower byte-pair 0000[00]00
        // floor(n / 4) = 1
        farAH = 4, // mod 4 = 0
        farBH = 5, // mod 4 = 1
        farCH = 6, // mod 4 = 2
        farDH = 7 // mod 4 = 3
        ,
        // Two most significant bytes [0000]0000
        // floor(n / 4) = 2
        farAX = 8,  // mod 4 = 0
        farBX = 9,  // mod 4 = 1
        farCX = 10, // mod 4 = 2
        farDX = 11 // mod 4 = 3
        ,
        // All four bytes [00000000]
        // floor(n / 4) = 3
        farEAX = 12, // mod 4 = 0
        farEBX = 13, // mod 4 = 1
        farECX = 14, // mod 4 = 2
        farEDX = 15 // mod 4 = 3
        ,
        // Stack registers
        // floor(n / 4) = 4
        farESP = 16, // mod 4 = 0
        farEBP = 17  // mod 4 = 1
    };

    enum Instruction {
        faiUnknown = 0,

        // Move instructions (0xA0 - 0xA5)
        faiMOV_REG_CONST = 0xA0,
        faiMOV_REG_REG = 0xA1,
        faiMOV_REG_ADDR = 0xA2,
        faiMOV_ADDR_CONST = 0xA3,
        faiMOV_ADDR_REG = 0xA4,
        faiMOV_ADDR_ADDR = 0xA5,

        // Stack instructions (0xAC - 0xAF)
        faiPUSH_CONST = 0xAC,
        faiPUSH_REG = 0xAD,
        faiPUSH_ADDR = 0xAE,
        faiPOP_REG = 0xAF,

        // Flowcontrol instructions (0x70 - 0x94)
        faiJMP_LABEL = 0x70,
        faiJMP_ADDR = 0x71,
        faiJO_LABEL = 0x72,
        faiJO_ADDR = 0x73,
        faiJNO_LABEL = 0x74,
        faiJNO_ADDR = 0x75,
        faiJS_LABEL = 0x76,
        faiJS_ADDR = 0x77,
        faiJNS_LABEL = 0x78,
        faiJNS_ADDR = 0x79,
        faiJE_LABEL = 0x7a,
        faiJE_ADDR = 0x7b,
        faiJNE_LABEL = 0x7c,
        faiJNE_ADDR = 0x7d,
        faiJB_LABEL = 0x7e,
        faiJB_ADDR = 0x7f,
        faiJBE_LABEL = 0x80,
        faiJBE_ADDR = 0x81,
        faiJA_LABEL = 0x82,
        faiJA_ADDR = 0x83,
        faiJAE_LABEL = 0x84,
        faiJAE_ADDR = 0x85,
        faiJL_LABEL = 0x86,
        faiJL_ADDR = 0x87,
        faiJLE_LABEL = 0x88,
        faiJLE_ADDR = 0x89,
        faiJG_LABEL = 0x8a,
        faiJG_ADDR = 0x8b,
        faiJGE_LABEL = 0x8c,
        faiJGE_ADDR = 0x8d,
        faiJP_LABEL = 0x8e,
        faiJP_ADDR = 0x8f,
        faiJNP_LABEL = 0x90,
        faiJNP_ADDR = 0x91,

        faiCALL_LABEL = 0x92,
        faiCALL_ADDR = 0x93,
        faiRET = 0x94,

        // System Call instruction (0x98)
        faiINT_CONST = 0x98,

        // Math instructions (0xE0 - 0xEF)
        // Unsigned
        faiADD_REG_CONST = 0xE0,
        faiADD_REG_REG = 0xE1,
        faiSUB_REG_CONST = 0xE2,
        faiSUB_REG_REG = 0xE3,
        faiMUL_REG_CONST = 0xE4,
        faiMUL_REG_REG = 0xE5,
        faiDIV_REG_CONST = 0xE6,
        faiDIV_REG_REG = 0xE7,

        // signed
        faiIADD_REG_CONST = 0xE8,
        faiIADD_REG_REG = 0xE9,
        faiISUB_REG_CONST = 0xEA,
        faiISUB_REG_REG = 0xEB,
        faiIMUL_REG_CONST = 0xEC,
        faiIMUL_REG_REG = 0xED,
        faiIDIV_REG_CONST = 0xEE,
        faiIDIV_REG_REG = 0xEF,

        faiCMP_REG_CONST = 0xF0,
        faiCMP_REG_REG = 0xF1,

        faiDBG_REG = 0xFE,
        faiDBG_ADDR = 0xFF,

        // pseude instructions reserved (0x10 - 0x2F)

        // pseude instructions (0x30 - 0x3F)
        // 1 byte operations (0x30 - 0x34)
        fapiRESB = 0x30, // reserves an amount of bytes
        fapiDB = 0x31  // defines a byte
        ,
        // 2 bytes operations (0x35 - 0x39)
        fapiRESW = 0x35, // reserves an amount of words
        fapiDW = 0x36  // defines a word
        ,
        // 4 bytes operations (0x3A - 0x3F)
        fapiRESD = 0x3A, // reserves an amount of double words
        fapiDD = 0x3B,  // defines a double word

    };

    enum OperationType
    {
        otUndefined = 0,
        otAddition = 1,
        otSubtraction = 2
    };

    enum OperandType
    {
        vtUndefined = 0,
        vtConstantValue = 1,
        vtRegister = 2
    };

    struct BinaryConstant
    {
        MemorySize Size;
        unsigned int Value;
    };

    struct BinaryRegister
    {
        MemorySize Size;
        Register Register;
    };

    struct BinaryAddress
    {
        MemorySize Size;
        unsigned char Struct;

        unsigned int LeftValue;
        unsigned int RightValue;

        unsigned int GetEffectiveAddress();

        OperandType GetLeftOperandType();
        OperandType GetRightOperandType();
        OperationType GetOperationType();
    };

    MemorySize GetRegisterSize(Register reg);
}

