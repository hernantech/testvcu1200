/*
Abstract class for stm32 interrupts

4/20/2024 Sricharan Pamireddy
*/

#include <stdint.h>

class stm32_interrupts
{
public:
    // see driverLib/interrupt.c::IntMasterEnable()
    static bool enable_processor_interrupt();

    // see driverLib/interrupt.c::IntMasterDisable()
    static bool disable_processor_interrupt();

    // see driverLib/interrupt.c::IntEnable()
    static void enable_specified_interrupt(uint32_t);

    // see driverLib/interrupt.c::IntDisable()
    static void disable_specified_interrupt(uint32_t);
};