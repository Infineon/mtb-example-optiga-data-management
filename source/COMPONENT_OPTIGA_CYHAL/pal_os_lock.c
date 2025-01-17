/******************************************************************************
* File Name:   pal_os_lock.c
*
* Description: This file contains part of the Platform Abstraction Layer.
*              This is a platform specific file. The function here are not 
*              really used by the host library (mw) itself
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
#include "include/pal/pal_os_lock.h"

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/
void pal_os_lock_create(pal_os_lock_t * p_lock, uint8_t lock_type)
{
    p_lock->type = lock_type;
    p_lock->lock = 0;
}

//lint --e{715} suppress "p_lock is not used here as it is placeholder for future." 
//lint --e{818} suppress "Not declared as pointer as nothing needs to be updated in the pointer."
void pal_os_lock_destroy(pal_os_lock_t * p_lock)
{
    
}

pal_status_t pal_os_lock_acquire(pal_os_lock_t * p_lock)
{
    pal_status_t return_status = PAL_STATUS_FAILURE;

    if (!(p_lock->lock))
    {
        p_lock->lock++;
        if (1 != p_lock->lock)
        {
            p_lock->lock--;
        }
        return_status = PAL_STATUS_SUCCESS;
    }
    return return_status;
}

void pal_os_lock_release(pal_os_lock_t * p_lock)
{
    if (0 != p_lock->lock)
    {
        p_lock->lock--;
    }
}

void pal_os_lock_enter_critical_section()
{
}

void pal_os_lock_exit_critical_section()
{
}

/**
* @}
*/
