#include <stdint.h>

class stm32_gpio
{
public:
    static uint32_t GPIO_PORTB_BASE;
    static uint32_t GPIO_PIN_1;

    //*****************************************************************************
    //
    //! Writes a value to the specified pin(s).
    //!
    //! \param ui32Port is the base address of the GPIO port.
    //! \param ui8Pins is the bit-packed representation of the pin(s).
    //! \param ui8Val is the value to write to the pin(s).
    //!
    //! Writes the corresponding bit values to the output pin(s) specified by
    //! \e ui8Pins.  Writing to a pin configured as an input pin has no effect.
    //!
    //! The pin(s) are specified using a bit-packed byte, where each bit that is
    //! set identifies the pin to be accessed, and where bit 0 of the byte
    //! represents GPIO port pin 0, bit 1 represents GPIO port pin 1, and so on.
    //!
    //! \return None.
    //
    //*****************************************************************************
    static void gpio_write(uint32_t gpio_address, uint8_t gpio_pin, uint8_t value);
};