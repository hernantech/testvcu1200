#include <stdint.h>

//*****************************************************************************
//
//! The structure used for encapsulating all the items associated with a CAN
//! message object in the CAN controller.
//
//*****************************************************************************
typedef struct
{
    //
    //! The CAN message identifier used for 11 or 29 bit identifiers.
    //
    uint32_t ui32MsgID;

    //
    //! The message identifier mask used when identifier filtering is enabled.
    //
    uint32_t ui32MsgIDMask;

    //
    //! This value holds various status flags and settings specified by
    //! tCANObjFlags.
    //
    uint32_t ui32Flags;

    //
    //! This value is the number of bytes of data in the message object.
    //
    uint32_t ui32MsgLen;

    //
    //! This is a pointer to the message object's data.
    //
    uint8_t *pui8MsgData;
}
tCANMsgObject;