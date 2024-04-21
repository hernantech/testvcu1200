#include <stdint.h>

class stm32_sysctl
{
public:
    // see driverLib/sysctl.c::SysCtlPeripheralEnable()
    static void peripheralEnable(uint32_t);

    // see driverLib/sysctl.c::SysCtlDelay()
    static void delay(int);

    // see driverLib/sysctl.c::SysCtlClockGet()
    static uint32_t getClock();
};