#include "BuildOptions.h"
#include "Program.h"
#include "MemoryManager.h"


namespace Flex
{
    namespace RTE
    {
        enum LoadProgramResult
        {
            Error,
            OK,
            UnloadFirst
        };

        class Engine
        {
        private:
#if (OPTIONS_RTE_ENGINE_MULTIPROGRAM == 0)
            Program * ActiveProgram;
#else
            Program * Programs;
            unsigned char ProgramIndex = 0;
            unsigned char ProgramCount = 0;
#endif
        public:

            LoadProgramResult LoadProgram(Program* program);
            bool UnloadProgram(Program * program);

            bool Step();
        };
    }
}