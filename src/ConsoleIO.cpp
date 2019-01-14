#include "ConsoleIO.h"


void ConsoleIO::PrintF(const char * format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
}
