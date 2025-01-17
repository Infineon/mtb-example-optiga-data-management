/******************************************************************************
* File Name:   example_optiga_util_update_count.c
*
* Description: This file contains an example code to update the counter 
*              object value in OPTIGA using #optiga_util_update_count.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2024, Cypress Semiconductor Corporation (an Infineon company) or
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
#include "include/optiga_util.h"
#include "optiga_example.h"


/*******************************************************************************
 * Global variables
 ******************************************************************************/

/**
 * Initialize the counter object with a threshold value 0x0A
 */
static const uint8_t initial_counter_object_data [] =
{
    //Initial counter value
    0x00, 0x00, 0x00, 0x00,
    //Threshold value
    0x00, 0x00, 0x00, 0x0A,
};

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

/**
 * The below example demonstrates update counter functionalities
 *
 * Example for #optiga_util_update_count
 *
 */
void example_optiga_util_update_count(void)
{
    uint32_t time_taken = 0;
    uint16_t optiga_counter_oid;
    uint8_t offset;

    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    optiga_util_t * me = NULL;
    
    do
    {
        
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        example_optiga_init();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        
        OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
        /**
         * 1. Create OPTIGA Util Instance
         */
        me = optiga_util_create(0, optiga_util_callback, NULL);
        if (NULL == me)
        {
            break;
        }
        
        /**
         * Pre-condition
         * Any data object can be converted to counter data object by changing metadata as mentioned below:
         * - As precondition  write access should be always or LCSO < OPERATIONAL
         * - Write metadata as "0xD0, 0x01, 0x01" using #optiga_util_write_metadata
        */

        /**
         * Write default count and threshold value to counter data object (e.g. E120)
         * using optiga_util_write_data.
         *
         * Use Erase and Write (OPTIGA_UTIL_ERASE_AND_WRITE) option,
         * in order to correctly update the used length of the object.
         */
        optiga_counter_oid = 0xE120;
        offset = 0x00;
        optiga_lib_status = OPTIGA_LIB_BUSY;
        
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = optiga_util_write_data(me,
                                               optiga_counter_oid,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               offset,
                                               initial_counter_object_data,
                                               sizeof(initial_counter_object_data));

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        // In this example, the counter is update by 5 and the final count would be 15
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_update_count(me,
                                                 optiga_counter_oid,
                                                 0x05);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = OPTIGA_LIB_SUCCESS;
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken, return_status);
    
    if (me)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
}

/**
* @}
*/
