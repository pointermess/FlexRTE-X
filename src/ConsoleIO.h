#pragma once
#include <stdarg.h>
#include <stdio.h>

class ConsoleIO
{
public:
    static void PrintF(const char * format, ...);
};

