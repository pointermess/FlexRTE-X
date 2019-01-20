

#include "FileIO.h"


// Platform specific includes
#if (OPTIONS_TARGET_BASED_WINDOWS)
    #include <stdio.h>
    #include <stdlib.h>
#endif

unsigned char* FlexRTE::DriverInterface::FileIO::ReadFileBytes(const char *name, unsigned int * size)
{
    // Platform specific branches
#if (OPTIONS_TARGET_BASED_WINDOWS)
    FILE *fl;
    fopen_s(&fl, name, "rb");
    if (fl == NULL) {
        printf("Error while opening %s", name);
        exit(EXIT_FAILURE);
    }
    fseek(fl, 0, SEEK_END);
    long len = ftell(fl);
    *size = len;
    unsigned char *ret = (unsigned char*)malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
#endif
}

FlexRTE::DriverInterface::FileStream::FileStream(const char * path)
{
#if (OPTIONS_TARGET_BASED_WINDOWS)
    fopen_s(&_File, path, "w+");
#endif
}

FlexRTE::DriverInterface::FileStream::~FileStream()
{
#if (OPTIONS_TARGET_BASED_WINDOWS)
    fclose(_File);
#endif
}

bool FlexRTE::DriverInterface::FileStream::IsOpen()
{
#if (OPTIONS_TARGET_BASED_WINDOWS)
    return true;
#endif
}

void FlexRTE::DriverInterface::FileStream::PrintF(const char * format, ...)
{
#if (OPTIONS_TARGET_BASED_WINDOWS)
    va_list arglist;
    va_start(arglist, format);
    vfprintf(_File, format, arglist);
    va_end(arglist);
#endif
}

void FlexRTE::DriverInterface::FileStream::Close()
{
#if (OPTIONS_TARGET_BASED_WINDOWS)
    fclose(_File);
#endif
}
