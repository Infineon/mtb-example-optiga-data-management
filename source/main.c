/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC 6 MCU and OPTIGA Trust M
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
* The MIT License
*
* Copyright (c) 2022 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "optiga_example.h"
#include "include/pal/pal_logger.h"
#include "include/pal/pal.h"
#include "provisioning_sm.h"
#include "optiga_lib_config.h"

extern pal_logger_t logger_console;

/* If you would like to enable the provisioning mode don't do this from here,
 * but rather open the makefile and change the DEFINES macro*/
#ifndef PROVISIONING_MODE

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
extern void example_optiga_init(void);
extern void example_optiga_util_read_data(void);
extern void example_optiga_util_write_data(void);
extern void example_optiga_util_update_count(void);
extern void example_optiga_util_protected_update(void);
extern void example_read_coprocessor_id(void);

#endif

/*******************************************************************************
* Global Variables
*******************************************************************************/


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It sets up a timer to trigger a 
* periodic interrupt. The main while loop checks for the status of a flag set 
* by the interrupt and toggles an LED at 1Hz to create an LED blinky. The 
* while loop also checks whether the 'Enter' key was pressed and 
* stops/restarts LED blinking.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    
    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, 115200);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

#ifndef PROVISIONING_MODE
    /*
      Initialise the chip with this function
    */
    example_optiga_init();

    example_optiga_util_read_data();
    example_optiga_util_write_data();
    example_optiga_util_update_count();
    //example_optiga_util_protected_update();
    example_read_coprocessor_id();
#else
    while(1)
    {
        provisioning_state_machine(&logger_console);
    }
#endif
}

/* [] END OF FILE */
