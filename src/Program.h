#pragma once

#include "BuildOptions.h"
#include "Memory.h"

namespace Flex
{
    namespace RTE
    {
        enum ProgramPriority
        {
            Default,
            VeryLow,
            Low,
            Medium,
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
            char ReadRegister(unsigned int * stepsTaken);
            char ReadAddress(unsigned int * stepsTaken);


        public:
            ProgramPriority Priority = ProgramPriority::Default;

            Program();

            void LoadFromFile(const char * path);

            unsigned int GetProgramCounter() { unsigned int test = 0; return ReadDWord(&test); };

            bool Step();

            void SetMemory(Memory& memory);
        };
    }
}