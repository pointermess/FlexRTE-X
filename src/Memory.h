#pragma once
#include "BuildOptions.h"
namespace Flex
{
    namespace RTE
    {
        class Memory
        {
        private:
            char * _Memory;
            unsigned int _Size;
        public:
            Memory(char * memory, unsigned int size) { _Memory = memory; _Size = size; };

        };
    }
}