/******************************************************************************
* File Name:   example_pair_host_and_optiga_using_pre_shared_secret.c
*
* Description: This file contains an example code to demonstarte how to pair
*              a hsot MCU with the OPTIGA Trust M to use a shielded connection
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
#include "include/optiga_crypt.h"

#include "include/pal/pal_os_datastore.h"
#include "optiga_example.h"

#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION 

/*******************************************************************************
 * Macros
 ******************************************************************************/
  
// Value of Operational state
#define LCSO_STATE_CREATION       (0x01)
// Value of Operational state
#define LCSO_STATE_OPERATIONAL    (0x07)

//Currently set to Creation state(defualt value). At the real time/customer side this needs to be LCSO_STATE_OPERATIONAL (0x07)
#define FINAL_LCSO_STATE          (LCSO_STATE_CREATION)

/*******************************************************************************
 * Global variables
 ******************************************************************************/
uint32_t time_taken_for_pairing = 0;

/* Platform Binding Shared Secret (0xE140) Metadata to be updated */

const uint8_t platform_binding_shared_secret_metadata_final [] = {
    //Metadata to be updated
    0x20, 0x17,
        // LcsO
        0xC0, 0x01,
                    FINAL_LCSO_STATE,       // Refer Macro to see the value or some more notes
        // Change/Write Access tag
        0xD0, 0x07,
                    // This allows updating the binding secret during the runtime using shielded connection
                    // If not required to update the secret over the runtime, set this to NEV and
                    // update Metadata length accordingly
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
                    0xFE,
                    0x20, 0xE1, 0x40,
        // Read Access tag
        0xD1, 0x03,
                    0xE1, 0xFC, LCSO_STATE_OPERATIONAL,   // LcsO < Operational state
        // Execute Access tag
        0xD3, 0x01,
                    0x00,   // Always
        // Data object Type
        0xE8, 0x01,
                    0x22,   // Platform binding secret type
};

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

/**
 * Callback when optiga_util_xxxx/optiga_crypt_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

optiga_lib_status_t pair_host_and_optiga_using_pre_shared_secret(void)
{
    uint16_t bytes_to_read;
    uint8_t platform_binding_secret[64];
    uint8_t platform_binding_secret_metadata[44];
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    pal_status_t pal_return_status;
    optiga_util_t * me_util = NULL;
    optiga_crypt_t * me_crypt = NULL;

    do
    {
        /**
         * 1. Create OPTIGA Util and Crypt Instances
         */
        me_util = optiga_util_create(0, optiga_lib_callback, NULL);
        if (NULL == me_util)
        {
            break;
        }

        me_crypt = optiga_crypt_create(0, optiga_lib_callback, NULL);
        if (NULL == me_crypt)
        {
            break;
        }

        /**
         * 2. Initialize the protection level and protocol version for the instances
         */
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_UTIL_SET_COMMS_PROTOCOL_VERSION(me_util,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

        OPTIGA_CRYPT_SET_COMMS_PROTECTION_LEVEL(me_crypt,OPTIGA_COMMS_NO_PROTECTION);
        OPTIGA_CRYPT_SET_COMMS_PROTOCOL_VERSION(me_crypt,OPTIGA_COMMS_PROTOCOL_VERSION_PRE_SHARED_SECRET);

        /**
         * 3. Read Platform Binding Shared secret (0xE140) data object metadata from OPTIGA
         *    using optiga_util_read_metadata.
         */
        bytes_to_read = sizeof(platform_binding_secret_metadata);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        
        START_PERFORMANCE_MEASUREMENT(time_taken_for_pairing);
        
        return_status = optiga_util_read_metadata(me_util,
                                                  0xE140,
                                                  platform_binding_secret_metadata,
                                                  &bytes_to_read);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 4. Validate LcsO in the metadata.
         *    Skip the rest of the procedure if LcsO is greater than or equal to operational state(0x07)
         */
        if (platform_binding_secret_metadata[4] >= LCSO_STATE_OPERATIONAL)
        {
            // The LcsO is already greater than or equal to operational state
            break;
        }

        /**
         * 5. Generate Random using optiga_crypt_random
         *       - Specify the Random type as TRNG
         *    a. The maximum supported size of secret is 64 bytes.
         *       The minimum recommended is 32 bytes.
         *    b. If the host platform doesn't support random generation,
         *       use OPTIGA to generate the maximum size chosen.
         *       else choose the appropriate length of random to be generated by OPTIGA
         *
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_random(me_crypt,
                                            OPTIGA_RNG_TYPE_TRNG,
                                            platform_binding_secret,
                                            sizeof(platform_binding_secret));
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 6. Generate random on Host
         *    If the host platform doesn't support, skip this step
         */


        /**
         * 7. Write random(secret) to OPTIGA platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = optiga_util_write_data(me_util,
                                               0xE140,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0,
                                               platform_binding_secret,
                                               sizeof(platform_binding_secret));
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 8. Write/store the random(secret) on the Host platform
         *
         */
        pal_return_status = pal_os_datastore_write(OPTIGA_PLATFORM_BINDING_SHARED_SECRET_ID,
                                                   platform_binding_secret,
                                                   sizeof(platform_binding_secret));

        if (PAL_STATUS_SUCCESS != pal_return_status)
        {
            //Storing of Pre-shared secret on Host failed.
            optiga_lib_status = pal_return_status;
            break;
        }



        /**
         * 9. Update metadata of OPTIGA Platform Binding shared secret data object (0xE140)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        OPTIGA_UTIL_SET_COMMS_PROTECTION_LEVEL(me_util,OPTIGA_COMMS_NO_PROTECTION);
        return_status = optiga_util_write_metadata(me_util,
                                                   0xE140,
                                                   platform_binding_shared_secret_metadata_final,
                                                   sizeof(platform_binding_shared_secret_metadata_final));

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken_for_pairing);
        
        return_status = OPTIGA_LIB_SUCCESS;

    } while(FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
    if(me_util)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me_util);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    if(me_crypt)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_crypt_destroy(me_crypt);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    return return_status;
}

/**
 * The below example demonstrates pairing the Host and OPTIGA using a preshared secret for the first time.
 *
 * Note:
 *   1) If the below example is executed once, the LcsO of Platform Binding shared secret is set to Initialization.
 *      The LcsO can't be reverted to previous states.
 *
 *      Please ensure the access conditions and other required settings in the metadata must be accordingly
 *      before setting the LcsO to Initialization state.
 *
 *   2) The metadata gets written in this example is just an example. The user must update this to the respective
 *      needs including LcsO state and access conditions
 *
 * Preconditions: The optiga_util_open_application must be executed before invoking the below example.
 *
 */
void example_pair_host_and_optiga_using_pre_shared_secret(void)
{
    optiga_lib_status_t return_status;
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Open the application on OPTIGA which is a precondition to perform any other operations
     * using optiga_util_open_application
     */
    example_optiga_init();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
    
    return_status = pair_host_and_optiga_using_pre_shared_secret();
     
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif //OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken_for_pairing,return_status);
    
}
#endif
