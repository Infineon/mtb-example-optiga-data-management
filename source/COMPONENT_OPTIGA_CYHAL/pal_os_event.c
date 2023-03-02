/******************************************************************************
* File Name:   pal_os_event.c
*
* Description: This file contains part of the Platform Abstraction Layer.
*              This is a platform specific file. The functions here are
*              very important and shall be implemted with computation
*              In case a hsot library is always busy (hang) it is likely
*              that function here are not corectly implemented
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2020-2023, Cypress Semiconductor Corporation (an Infineon company) or
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
#include "optiga/pal/pal_os_event.h"
#include "optiga/pal/pal.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

/// @cond hidden
/*******************************************************************************
 * Macros
 ******************************************************************************/
#define PAL_OS_EVENT_INTR_PRIO    (4U)
#define CYHAL_TIMER_SCALING 10

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static pal_os_event_t pal_os_event_0 = {0};
/* Timer object used */
cyhal_timer_t pal_os_event_timer_obj;

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/
 
/* Funciton to initialise the timer for the internal FSM */
void pal_os_event_init(void);

void pal_os_event_start(pal_os_event_t * p_pal_os_event, register_callback callback, void * callback_args)
{
    if (FALSE == p_pal_os_event->is_event_triggered)
    {
        p_pal_os_event->is_event_triggered = TRUE;
        pal_os_event_register_callback_oneshot(p_pal_os_event,callback,callback_args, 1000);
    }
}

void pal_os_event_stop(pal_os_event_t * p_pal_os_event)
{
    //lint --e{714} suppress "The API pal_os_event_stop is not exposed in header file but used as extern in 
    //optiga_cmd.c"
    p_pal_os_event->is_event_triggered = FALSE;
}

pal_os_event_t * pal_os_event_create(register_callback callback, void * callback_args)
{
    pal_os_event_init();

    if (( NULL != callback )&&( NULL != callback_args ))
    {
        pal_os_event_start(&pal_os_event_0,callback,callback_args);
    }
    return (&pal_os_event_0);
}

void pal_os_event_trigger_registered_callback(void)
{
    register_callback callback;

    // !!!OPTIGA_LIB_PORTING_REQUIRED
    // The following steps related to TIMER must be taken care while porting to different platform
    cyhal_timer_stop(&pal_os_event_timer_obj);
    cyhal_timer_reset(&pal_os_event_timer_obj);
    /// If callback_ctx is NULL then callback function will have unexpected behavior 
    if (pal_os_event_0.callback_registered)
    {
        callback = pal_os_event_0.callback_registered;
        callback((void * )pal_os_event_0.callback_ctx);
    }
}
/// @endcond

void pal_os_event_register_callback_oneshot(pal_os_event_t * p_pal_os_event,
                                             register_callback callback,
                                             void * callback_args,
                                             uint32_t time_us)
{
  cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0,                       /* Timer compare value, not used */
        .period = time_us * CYHAL_TIMER_SCALING,  /* Defines the timer period */
        .direction = CYHAL_TIMER_DIR_UP,          /* Timer counts up */
        .is_compare = false,                      /* Don't use compare mode */
        .is_continuous = false,                   /* Run the timer indefinitely */
        .value = 0                                /* Initial value of counter */
    };
    p_pal_os_event->callback_registered = callback;
    p_pal_os_event->callback_ctx = callback_args;

    // !!!OPTIGA_LIB_PORTING_REQUIRED
    // The following steps related to TIMER must be taken care while porting to different platform
    //lint --e{534} suppress "Error handling is not required so return value is not checked"
    cyhal_timer_configure(&pal_os_event_timer_obj, &timer_cfg);
    cyhal_timer_start(&pal_os_event_timer_obj);
}

//lint --e{818,715} suppress "As there is no implementation, pal_os_event is not used"
void pal_os_event_destroy(pal_os_event_t * pal_os_event)
{
    cyhal_timer_free (&pal_os_event_timer_obj);
}

void pal_os_event_init(void)
{
    cy_rslt_t cy_hal_status;
    const cyhal_timer_cfg_t timer_cfg =
    {
        .compare_value = 0,                 /* Timer compare value, not used */
        .period = 1000,                     /* Defines the timer period */
        .direction = CYHAL_TIMER_DIR_UP,    /* Timer counts up */
        .is_compare = false,                /* Don't use compare mode */
        .is_continuous = false,             /* Run the timer indefinitely */
        .value = 0                          /* Initial value of counter */
    };

    do
    {
        /* Initialize the timer object. Does not use pin output ('pin' is NC) and
         * does not use a pre-configured clock source ('clk' is NULL). */
          cy_hal_status = cyhal_timer_init(&pal_os_event_timer_obj, NC, NULL);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
          break;
        }
        /* Apply timer configuration such as period, count direction, run mode, etc. */
        cy_hal_status = cyhal_timer_configure(&pal_os_event_timer_obj, &timer_cfg);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
          break;
        }
        /* Set the frequency of timer to 10^7 Hz */
        cy_hal_status = cyhal_timer_set_frequency(&pal_os_event_timer_obj, 10000000);
        if(CY_RSLT_SUCCESS != cy_hal_status)
        {
          break;
        }
        /* Assign the ISR to execute on timer interrupt */
        cyhal_timer_register_callback(&pal_os_event_timer_obj, 
                                     (cyhal_timer_event_callback_t)pal_os_event_trigger_registered_callback, 
                                      NULL);
        /* Set the event on which timer interrupt occurs and enable it */
        cyhal_timer_enable_event(&pal_os_event_timer_obj, CYHAL_TIMER_IRQ_ALL, PAL_OS_EVENT_INTR_PRIO, true);

    } while (false);
}


/**
* @}
*/
