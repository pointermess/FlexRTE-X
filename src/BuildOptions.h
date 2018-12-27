// Build options for Target Platform
// Operating System
// 1 = Windows
// 20 = Teensy 3.6
#define OPTIONS_TARGET 1

// Based on:
#define OPTIONS_TARGET_BASED_WINDOWS true
#define OPTIONS_TARGET_BASED_UNIX false
#define OPTIONS_TARGET_BASED_ARDUINO false
#define OPTIONS_TARGET_BASED_TEENSY false
#define OPTIONS_TARGET_BASED_ESP false

// Build options for optimizations
// Interpreter
// High Performance Mode
// 0 = Slow but virtually no memory usage (switch-statement)
// 1 = Very fast but requires ~1.2KB of memory usage (pointermess)
#define OPTIONS_RTE_INTERPRETER_HIGHPERFORMANCE 1

// Program
// Keep program code in memory
// 0 = Reads program code from source each instruction (very slow)
// 1 = Keeps program code in memory (utilizes dynamic memory allocation at startup but is faster than option 0)
// 2 = Cached program code (combination of the above, to be done)
#define OPTIONS_RTE_PROGRAM_KEEPCODEINMEMORY 1

// Memory
// Sizes for engine and apps
#define OPTIONS_RTE_MEMORY_FULLSIZE 128000
#define OPTIONS_RTE_MEMORY_APPSIZE 8000
#define OPTIONS_RTE_MEMORY_ALLOWDYNAMICAPPSIZE true