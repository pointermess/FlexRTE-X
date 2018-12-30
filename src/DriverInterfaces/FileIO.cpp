

#include "FileIO.h"


// Platform specific includes
#if (OPTIONS_TARGET_BASED_WINDOWS)
    #include <stdio.h>
    #include <stdlib.h>
#endif

char* FlexRTE::DriverInterface::FileIO::ReadFileBytes(const char *name, unsigned int * size)
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
    char *ret = (char*)malloc(len);
    fseek(fl, 0, SEEK_SET);
    fread(ret, 1, len, fl);
    fclose(fl);
    return ret;
#endif
}
