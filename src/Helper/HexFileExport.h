#pragma once

#include "..\DriverInterfaces\FileIO.h"

class HexFileExporter
{
public:
    static void ExportArrayToFile(char * arrayPtr, unsigned int arrayLength, const char * path);
    static void ExportArrayToBinaryFile(char * arrayPtr, unsigned int arrayLength, const char * path);
};