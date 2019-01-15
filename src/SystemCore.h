#pragma once
namespace FlexRTE
{
    namespace DriverInterface
    {
        class SystemCore
        {
        private:
        public:
            static unsigned int StartTime;
            static unsigned int Clock();
            static void Initialize();
        };
    }
}