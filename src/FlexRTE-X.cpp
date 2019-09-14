

#include "ConsoleIO.h"
#include "Program.h"
#include "Engine.h"
#include "DriverInterfaces\FileIO.h"
#include "Helper\HexFileExport.h"
#include <iostream>

using namespace FlexRTE;
using namespace FlexRTE::DriverInterface;

int main()
{
    Engine * engine = new Engine();

    Program * testProgram = new Program();
    const char* path;
    path = "C:/Users/Milos/Desktop/FlexASM Compiler/Debug/pctest.fae";
    testProgram->LoadFromFile(path);
    if (testProgram->Initialize())
    {
        if (engine->LoadProgram(testProgram))
        {
            engine->Execute();

        }
        else
        {
            ConsoleIO::PrintF("Could not load program in to engine");
        }
    }
    else
    {
        ConsoleIO::PrintF("Could not initialize program");
    }
    engine->PrintExecutionReport();
    HexFileExporter::ExportArrayToFile(engine->MemoryManager->GetMemoryArray(), OPTIONS_RTE_MEMORY_FULLSIZE, "C:/Users/Milos/Desktop/FlexASM Compiler/Debug/flextestres.txt");
}

