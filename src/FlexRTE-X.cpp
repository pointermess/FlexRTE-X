

#include "Program.h"
#include "Engine.h"
#include <iostream>

using namespace FlexRTE;

int main()
{
    Engine * engine = new Engine();

    Program * testProgram = new Program();
    const char* path;
    path = "C:/Users/Milos/Desktop/FlexASM Compiler/Debug/test.fae";
    testProgram->LoadFromFile(path);
    if (testProgram->Initialize())
    {
        if (engine->LoadProgram(testProgram))
        {
            engine->Execute();
        }
        else
        {
            std::cout << "Could not load program in to engine";
        }
    }
    else
    {
        std::cout << "Could not initialize program";
    }
}

