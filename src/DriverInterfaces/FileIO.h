#pragma once
#include "..\BuildOptions.h"

namespace Flex
{
    namespace RTE
    {
        class FileIO
        {
        public:
            static char* ReadFileBytes(const char *name, unsigned int *size);
        };
    }
}