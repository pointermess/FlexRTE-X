#pragma once
#include "..\BuildOptions.h"

namespace FlexRTE
{
    namespace DriverInterface
    {
        class FileIO
        {
        public:
            static char* ReadFileBytes(const char *name, unsigned int *size);
        };
    }
}