#pragma once
#include "..\BuildOptions.h"

namespace FlexRTE
{
    namespace DriverInterface
    {
        class FileIO
        {
        public:
            static unsigned char* ReadFileBytes(const char *name, unsigned int *size);
        };
    }
}