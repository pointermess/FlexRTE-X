#include "SystemCore.h"

#include <ctime>


using namespace FlexRTE;
using namespace FlexRTE::DriverInterface;

unsigned int SystemCore::StartTime;

unsigned int FlexRTE::DriverInterface::SystemCore::Clock()
{
    return clock();
}
void SystemCore::Initialize()
{
    SystemCore::StartTime = clock();
}
