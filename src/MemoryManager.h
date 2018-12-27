#include "BuildOptions.h"
#include "Memory.h"

namespace Flex
{
    namespace RTE
    {
        class MemoryManager
        {
        private:
            char * _MemoryArray;
            unsigned int _ReservedMemory = 0;

            Memory * _Memory;
        public:
            MemoryManager();
            ~MemoryManager();
        };
    }
}