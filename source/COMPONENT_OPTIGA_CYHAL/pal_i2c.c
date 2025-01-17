/******************************************************************************
* File Name:   pal_i2c.c
*
* Description: This file contains part of the Platform Abstraction Layer.
*              This is a platform specific file. This file implements 
*              platform abstraction layer configurations for ifx i2c protocol.
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
#include "include/pal/pal_i2c.h"
#include "pal_psoc_i2c_mapping.h"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cyhal_scb_common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define PAL_I2C_MASTER_MAX_BITRATE  (400U)
#define PAL_I2C_MASTER_INTR_PRIO    (3U)
/// @cond hidden

/*******************************************************************************
 * Static Variables
 ******************************************************************************/
_STATIC_H volatile uint32_t g_entry_count = 0;
_STATIC_H const pal_i2c_t * gp_pal_i2c_current_ctx;
_STATIC_H uint8_t g_pal_i2c_init_flag = 0;

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/
//lint --e{715} suppress "This is implemented for overall completion of API"
_STATIC_H pal_status_t pal_i2c_acquire(const void * p_i2c_context)
{
    if (0 == g_entry_count)
    {
        g_entry_count++;
        if (1 == g_entry_count)
        {
            return PAL_STATUS_SUCCESS;
        }
    }
    return PAL_STATUS_FAILURE;
}

//lint --e{715} suppress "The unused p_i2c_context variable is kept for future enhancements"
_STATIC_H void pal_i2c_release(const void * p_i2c_context)
{
    g_entry_count = 0;
}

_STATIC_H void invoke_upper_layer_callback (const pal_i2c_t * p_pal_i2c_ctx,
                                        optiga_lib_status_t event)
{
    upper_layer_callback_t upper_layer_handler;
    //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
    upper_layer_handler = (upper_layer_callback_t)p_pal_i2c_ctx->upper_layer_event_handler;

    upper_layer_handler(p_pal_i2c_ctx->p_upper_layer_ctx, event);

    //Release I2C Bus
    pal_i2c_release(p_pal_i2c_ctx->p_upper_layer_ctx);
}

_STATIC_H void i2c_master_error_detected_callback(const pal_i2c_t * p_pal_i2c_ctx)
{
    cyhal_i2c_abort_async(((pal_psoc_i2c_t *)(p_pal_i2c_ctx->p_i2c_hw_config))->i2c_master_channel);
    invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_ERROR);
}

/* Defining master callback handler */
void i2c_master_event_handler(void *callback_arg, cyhal_i2c_event_t event)
{
    if (0UL != (CYHAL_I2C_MASTER_ERR_EVENT & event))
    {
        /* In case of error abort transfer */
        i2c_master_error_detected_callback(gp_pal_i2c_current_ctx);
    }
    /* Check write complete event */
    else if (0UL != (CYHAL_I2C_MASTER_WR_CMPLT_EVENT & event))
    {
        /* Perform the required functions */
        invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_SUCCESS);
    }
    /* Check read complete event */
    else if (0UL != (CYHAL_I2C_MASTER_RD_CMPLT_EVENT & event))
    {
        /* Perform the required functions */
        invoke_upper_layer_callback(gp_pal_i2c_current_ctx, PAL_I2C_EVENT_SUCCESS);
    }
}

pal_status_t pal_i2c_init(const pal_i2c_t * p_i2c_context)
{
    cy_rslt_t cy_hal_status;
    /* Define the I2C master configuration structure */
    cyhal_i2c_cfg_t i2c_master_config = {false,    // is slave?
                                         0,        // address of this resource; set to 0 for master
                                         400000    // bus frequency in Hz
                                        };
    
    do
    {
        if (g_pal_i2c_init_flag == 0)
        {
            // Init I2C driver
            cy_hal_status = cyhal_i2c_init(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                        ((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->sda_port_num,
                        ((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->sca_port_num,
                        NULL);

            if (CY_RSLT_SUCCESS != cy_hal_status)
            {
                break;
            }

            //Configure the I2C resource to be master
            cy_hal_status = cyhal_i2c_configure(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                                                            &i2c_master_config);
            if (CY_RSLT_SUCCESS != cy_hal_status)
            {
                break;
            }

            // Register i2c master callback
            cyhal_i2c_register_callback(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                          (cyhal_i2c_event_callback_t) i2c_master_event_handler,
                          NULL);

            // Enable interrupts for I2C master
            cyhal_i2c_enable_event(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                         (cyhal_i2c_event_t)(CYHAL_I2C_MASTER_WR_CMPLT_EVENT \
                         | CYHAL_I2C_MASTER_RD_CMPLT_EVENT \
                         | CYHAL_I2C_MASTER_ERR_EVENT),    \
                         PAL_I2C_MASTER_INTR_PRIO ,
                         true);
            g_pal_i2c_init_flag = 1;
        }
        else
        {
            cy_hal_status = (pal_status_t)PAL_STATUS_SUCCESS;
        }
    } while (FALSE);

    return (pal_status_t)cy_hal_status;
}

pal_status_t pal_i2c_deinit(const pal_i2c_t * p_i2c_context)
{
    g_pal_i2c_init_flag = 0;
    cyhal_i2c_free(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel);
    return (PAL_STATUS_SUCCESS);
}

pal_status_t pal_i2c_write(const pal_i2c_t * p_i2c_context, uint8_t * p_data, uint16_t length)
{
    pal_status_t status = PAL_STATUS_FAILURE;

    //Acquire the I2C bus before read/write
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context))
    {
        gp_pal_i2c_current_ctx = p_i2c_context;
        //Invoke the low level i2c master driver API to write to the bus
        if (CY_RSLT_SUCCESS != cyhal_i2c_master_transfer_async(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                                                                p_i2c_context->slave_address,
                                                                p_data,
                                                                length,
                                                                NULL,
                                                                0))
        {
            //If I2C Master fails to invoke the write operation, invoke upper layer event handler with error.
            //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))
                                                       (p_i2c_context->p_upper_layer_ctx , PAL_I2C_EVENT_ERROR);
            
            //Release I2C Bus
            pal_i2c_release((void * )p_i2c_context);
        }
        else
        {
            status = PAL_STATUS_SUCCESS;
        }
    }
    else
    {
        status = PAL_STATUS_I2C_BUSY;
        //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
        ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))
                                                        (p_i2c_context->p_upper_layer_ctx , PAL_I2C_EVENT_BUSY);
    }
    return (status);
}

pal_status_t pal_i2c_read(const pal_i2c_t * p_i2c_context, uint8_t * p_data, uint16_t length)
{
    pal_status_t status = PAL_STATUS_FAILURE;

    //Acquire the I2C bus before read/write
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context))
    {
        gp_pal_i2c_current_ctx = p_i2c_context;

        //Invoke the low level i2c master driver API to read from the bus
        if (CY_RSLT_SUCCESS != cyhal_i2c_master_transfer_async(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel,
                                                                p_i2c_context->slave_address,
                                                                NULL,
                                                                0,
                                                                p_data,
                                                                length))
        {
            //If I2C Master fails to invoke the read operation, invoke upper layer event handler with error.
            //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
            ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))
                                                       (p_i2c_context->p_upper_layer_ctx , PAL_I2C_EVENT_ERROR);

            //Release I2C Bus
            pal_i2c_release((void * )p_i2c_context);
        }
        else
        {
            status = PAL_STATUS_SUCCESS;
        }
    }
    else
    {
        status = PAL_STATUS_I2C_BUSY;
        //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
        ((upper_layer_callback_t)(p_i2c_context->upper_layer_event_handler))
                                                        (p_i2c_context->p_upper_layer_ctx , PAL_I2C_EVENT_BUSY);
    }
    return (status);
}

pal_status_t pal_i2c_set_bitrate(const pal_i2c_t * p_i2c_context, uint16_t bitrate)
{

    cyhal_i2c_t * i2cObj = (cyhal_i2c_t * )(((pal_psoc_i2c_t *)(p_i2c_context->p_i2c_hw_config))->i2c_master_channel);
    pal_status_t return_status = PAL_STATUS_FAILURE;
    optiga_lib_status_t event = PAL_I2C_EVENT_ERROR;
    uint32_t setDataRate;

    //Acquire the I2C bus before setting the bitrate
    if (PAL_STATUS_SUCCESS == pal_i2c_acquire(p_i2c_context))
    {
        // If the user provided bitrate is greater than the I2C master hardware maximum supported value,
        // set the I2C master to its maximum supported value.
        if (bitrate > PAL_I2C_MASTER_MAX_BITRATE)
        {
            bitrate = PAL_I2C_MASTER_MAX_BITRATE;
        }

        setDataRate = _cyhal_i2c_set_peri_divider(i2cObj, 1, (bitrate * 1000), false);
        if (0 == setDataRate)
        {

             return_status = PAL_STATUS_FAILURE;
        }
        else
        {
             return_status = PAL_STATUS_SUCCESS;
             event = PAL_I2C_EVENT_SUCCESS;
        }
    }
    else
    {
        return_status = PAL_STATUS_I2C_BUSY;
        event = PAL_I2C_EVENT_BUSY;
    }

    if (0 != p_i2c_context->upper_layer_event_handler)
    {
        //lint --e{611} suppress "void* function pointer is type casted to upper_layer_callback_t type"
        ((callback_handler_t)(p_i2c_context->upper_layer_event_handler))(p_i2c_context->p_upper_layer_ctx , event);
    }
    //Release I2C Bus if its acquired 
    if (PAL_STATUS_I2C_BUSY != return_status)
    {
        pal_i2c_release((void * )p_i2c_context);
    }

    return (return_status);
}
