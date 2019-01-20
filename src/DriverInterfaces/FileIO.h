#pragma once
#include "..\BuildOptions.h"

#if (OPTIONS_TARGET_BASED_WINDOWS)
#include <stdarg.h>
#include <stdio.h>
#endif
namespace FlexRTE
{
    namespace DriverInterface
    {
        class FileIO
        {
        public:
            static unsigned char* ReadFileBytes(const char *name, unsigned int *size);
        };


        class FileStream
        {
        private:
#if (OPTIONS_TARGET_BASED_WINDOWS)
            FILE * _File;
#endif
        public:
            FileStream(const char * path);
            ~FileStream();
            bool IsOpen();
            void PrintF(const char * format, ...);

            void Close();
        };
    }
}