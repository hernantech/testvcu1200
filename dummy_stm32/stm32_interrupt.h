/*
Abstract class for stm32 interrupts

4/20/2024 Sricharan Pamireddy
*/

class dummy_stm32
{
public:
    //*****************************************************************************
    //
    //! Enables the processor interrupt.
    //!
    //! This function allows the processor to respond to interrupts.  This function
    //! does not affect the set of interrupts enabled in the interrupt controller;
    //! it just gates the single interrupt from the controller to the processor.
    //!
    //! \b Example: Enable interrupts to the processor.
    //!
    //! \verbatim
    //! //
    //! // Enable interrupts to the processor.
    //! //
    //! IntMasterEnable();
    //!
    //! \endverbatim
    //!
    //! \return Returns \b true if interrupts were disabled when the function was
    //! called or \b false if they were initially enabled.
    //
    //*****************************************************************************
    static bool enable_processor_interrupt();

    //*****************************************************************************
    //
    //! Disables the processor interrupt.
    //!
    //! This function prevents the processor from receiving interrupts.  This
    //! function does not affect the set of interrupts enabled in the interrupt
    //! controller; it just gates the single interrupt from the controller to the
    //! processor.
    //!
    //! \note Previously, this function had no return value.  As such, it was
    //! possible to include <tt>interrupt.h</tt> and call this function without
    //! having included <tt>hw_types.h</tt>.  Now that the return is a
    //! <tt>bool</tt>, a compiler error occurs in this case.  The solution
    //! is to include <tt>hw_types.h</tt> before including <tt>interrupt.h</tt>.
    //!
    //! \b Example: Disable interrupts to the processor.
    //!
    //! \verbatim
    //! //
    //! // Disable interrupts to the processor.
    //! //
    //! IntMasterDisable();
    //!
    //! \endverbatim
    //!
    //! \return Returns \b true if interrupts were already disabled when the
    //! function was called or \b false if they were initially enabled.
    //
    //*****************************************************************************
    static bool disable_processor_interrupt();
};