/******************************************************************************
 *
 * Default Linker Command file for the Texas Instruments TM4C123BH6PGE
 *
 * This is derived from revision 15071 of the TivaWare Library.
 *
 *****************************************************************************/

--retain=g_pfnVectors

#include "configuration.h"

MEMORY
{
	FLASH_INTVECS	: origin = APP_BASE, length = 0x00000300
	FLASH_CODE (RX) : origin = APP_BASE + 0x0300, length = 0x00000004, fill = 0xABCD1200
    FLASH (RX) : origin = APP_BASE + 0x0304, length = 0x0003AC00
    SRAM (RWX) : origin = 0x20000000, length = 0x00008000
}

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M4_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > APP_BASE
    .text   :   > FLASH
    .const  :   > FLASH
    .cinit  :   > FLASH
    .pinit  :   > FLASH
    .init_array : > FLASH

    .vtable :   > 0x20000000
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM
}

__STACK_TOP = __stack + 512;
