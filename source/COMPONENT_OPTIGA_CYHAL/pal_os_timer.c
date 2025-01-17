/******************************************************************************
* File Name:   pal_os_timer.c
*
* Description: This file contains part of the Platform Abstraction Layer.
*              This is a platform specific file and shall be changed in case
*              base board is changed. this file shall implement only functions
*              which can return timestamps
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "include/pal/pal_os_timer.h"
#include "include/pal/pal.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

/// @cond hidden

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define PAL_OS_TIMER_INTR_PRIO    (6U)

/*******************************************************************************
 * Local Variables
 ******************************************************************************/
static volatile uint32_t g_tick_count = 0;
/* Timer object used */
static cyhal_timer_t pal_os_timer_obj;

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/
static void pal_os_timer_callback_event(void)
{
    g_tick_count += 1U;
    cyhal_timer_reset (&pal_os_timer_obj);
}

uint32_t pal_os_timer_get_time_in_microseconds(void)
{
    // !!!OPTIGA_LIB_PORTING_REQUIRED
    // This API is needed to support optiga cmd scheduler. 
    static uint32_t count = 0;
    // The implementation must ensure that every invocation of this API returns a unique value.
    return (count++);
}

uint32_t pal_os_timer_get_time_in_milliseconds(void)
{
    return (g_tick_count);
}

void pal_os_timer_delay_in_milliseconds(uint16_t milliseconds)
{
    cyhal_system_delay_ms (milliseconds);
}

//lint --e{714} suppress "This is implemented for overall completion of API"
pal_status_t pal_timer_init(void)
{
    cy_rslt_t cy_hal_status;
    pal_status_t pal_timer_init_status = PAL_STATUS_FAILURE;

    const cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0,                 /* Timer compare value, not used */
        .period = 10000,                    /* Defines the timer period */
        .direction = CYHAL_TIMER_DIR_UP,    /* Timer counts up */
        .is_compare = false,                /* Don't use compare mode */
        .is_continuous = true,              /* Run the timer indefinitely */
        .value = 0                          /* Initial value of counter */
    };
    
    do
    {
        /* Initialize the timer object. Does not use pin output ('pin' is NC) and
         * does not use a pre-configured clock source ('clk' is NULL). */
        cy_hal_status = cyhal_timer_init(&pal_os_timer_obj, NC, NULL);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
            break;
        }
          
        /* Apply timer configuration such as period, count direction, run mode, etc. */
        cy_hal_status = cyhal_timer_configure(&pal_os_timer_obj, &timer_cfg);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
            break;
        }
        cy_hal_status = cyhal_timer_set_frequency(&pal_os_timer_obj, 10000000);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
            break;
        }
        /* Assign the ISR to execute on timer interrupt */
        cyhal_timer_register_callback(&pal_os_timer_obj, (cyhal_timer_event_callback_t)pal_os_timer_callback_event, NULL);
        /* Set the event on which timer interrupt occurs and enable it */
        cyhal_timer_enable_event(&pal_os_timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, PAL_OS_TIMER_INTR_PRIO, true);
        /* Start the timer with the configured settings */
        cy_hal_status = cyhal_timer_start(&pal_os_timer_obj);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
            break;
        }
        pal_timer_init_status = PAL_STATUS_SUCCESS;
        
    } while (false);
    return (pal_timer_init_status);
}

//lint --e{714} suppress "This is implemented for overall completion of API"
pal_status_t pal_timer_deinit(void)
{
    cyhal_timer_free(&pal_os_timer_obj);
    return (PAL_STATUS_SUCCESS);
}
