/******************************************************************************
* File Name:   aes_key_object_confidentiality.h
*
* Description: This file provides the data set for protected update of aes 
*              key object with confidentiality.
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

#include <stdint.h>

/**
 * Manifest for AES key update with integrity protected and confidentiality
 */
const uint8_t manifest_aes_key[] = 
{
    // COSE Sign1 Trust
    0x84,
        // Protected signed header trust
        0x43,
            // Trust Sign Algorithm
            0xA1, 
            // ECDSA
            0x01, 0x26, 
        // Unprotected signed header trust
        0xA1, 
            // Root of trust
            0x04, 0x42, 
            // Trust Anchor OID
            0xE0, 0xE3, 
        // Payload info Byte string of single byte length
        0x58, 
            // Byte string length of manifest            
            0x9C, 
            // Trust manifest, array of 6 entries
            0x86, 
            // Version(Major Type 0)
            0x01,
            // NULL
            0xF6,
            // NULL
            0xF6,
            // Resources, array of 4 entries
            0x84, 
                // Trust Payload Type
                0x22,
                // Payload Length 
                0x13,
                // Trust Payload version
                0x03, 
                // Trust Add info data 
                0x82, 
                    // key algorithm
                    0x18, 0x81,
                    // key usage
                    0x02, 
                // Trust Processors, array of 2 entries 
                0x82, 
                    // Processing step integrity, array of 2 entries
                    0x82, 
                        // Process( Major Type 1)
                        0x20, 
                        // Parameters, byte string with single byte length
                        0x58, 
                            // Byte string length 
                            0x25, 
                            // IFX Digest info, array of 2 entries
                            0x82, 
                                // Digest Algorithm
                                0x18,
                                    // SHA-256
                                    0x29, 
                                // Digest
                                0x58, 
                                    // Byte string length
                                    0x20, 
                                        // Digest data
                                        0x78, 0x46, 0xBB, 0x13, 0xC0, 0x22, 0x9B, 0x30,
                                        0x65, 0x32, 0x39, 0x10, 0x07, 0x7E, 0xDE, 0xB0,
                                        0xD9, 0x7B, 0x0F, 0x23, 0xEB, 0xC8, 0x51, 0x1E,
                                        0x79, 0x5B, 0xAC, 0x6F, 0xBF, 0x1D, 0x0E, 0x3B,
                    // Processing step decrypt, array of 2 entries
                    0x82,
                        // Process( Major Type 0)
                        0x01,
                        // COSE_Encrypt_Trust array of 3 entries
                        0x83,
                            // protected-encrypt-header-Trust
                            0x43,
                                // Map 
                                0xA1,
                                    // Key 
                                    0x01,
                                    // AES-CCM-16-64-128, 128-bit key, 64-bit Authentication(MAC) tag, 13-byte nonce
                                    0x0A,
                            // recipients array of 1
                            0x81,
                                // COSE_Recipient_Trust array of 2
                                0x82,
                                    // protected-recipient-header-Trust
                                    0x58,
                                        // byte str
                                        0x54,
                                            // Map of 3
                                            0xA3,
                                                // Key
                                                0x04,
                                                // Value
                                                0x42,
                                                    // Shared secret OID
                                                    0xF1, 0xD1,
                                                // Key
                                                0x01,
                                                // Value (KeyDerivationAlgorithms)
                                                0x3A, 0x00, 0x01, 0x00, 0xB7,
                                                // Key
                                                0x05,
                                                // Value array of 2
                                                0x82,
                                                    0x44,
                                                        // label(4)
                                                        0x74, 0x65, 0x73, 0x74,
                                                    0x58,
                                                        0x40,
                                                        // seed(64)
                                                        0x2D, 0x93, 0xB2, 0xA2, 0x81, 0x72, 0xC2, 0x42,
                                                        0x78, 0xAA, 0x5F, 0x8B, 0x59, 0x66, 0x8D, 0x93,
                                                        0xEF, 0x49, 0x60, 0x2A, 0x3D, 0xDD, 0x1D, 0xDE,
                                                        0xCB, 0x46, 0xB4, 0x5F, 0x8F, 0x61, 0x9C, 0xF2,
                                                        0xAD, 0x60, 0x69, 0xFB, 0xA8, 0xDC, 0x16, 0xC9,
                                                        0xA8, 0xB0, 0x86, 0x59, 0xB7, 0xFB, 0x80, 0x24,
                                                        0xD2, 0x6F, 0xC3, 0xCB, 0x4D, 0x26, 0xBA, 0x56,
                                                        0x6D, 0xA0, 0x8D, 0xFC, 0x39, 0xE7, 0xFF, 0xFD,
                                    // ciphertext-recipient-header-Trust
                                    0xF6,
                            // AdditionalInfo
                            0xF6,
                // Trust Target
                0x82, 
                    // Component identifier(0)
                    0x40,
                    // Storage identifier
                    0x42, 
                    // Optiga target OID
                    0xE2, 0x00, 
        // Signature info, byte string of single byte length
        0x58, 
            // Byte string length for ECC 256
            0x40, 
            // Signature data
            0x91, 0xAF, 0xD6, 0x8F, 0x3A, 0x03, 0x15, 0x94, 0xE1, 0x6A, 0xD9, 0x27, 0x8B, 0xAF, 0x3D, 0x2D,
            0x80, 0x10, 0xFF, 0x52, 0x66, 0x4C, 0x27, 0x4A, 0x8E, 0x36, 0xDD, 0xC0, 0x6F, 0xA9, 0xC4, 0x79,
            0x8B, 0x11, 0x3C, 0x38, 0x1F, 0x90, 0x0F, 0x48, 0xF8, 0x34, 0xD7, 0x74, 0x9C, 0x1D, 0x00, 0xE5,
            0x65, 0xBA, 0x51, 0xFF, 0x41, 0xE9, 0xFB, 0xBF, 0x88, 0xFF, 0x0B, 0xDE, 0xBC, 0x30, 0xE8, 0x2F,
};

/**
 * Fragment array for final with 19 bytes of payload for AES key update
 */
const uint8_t aes_key_final_fragment_array[] = 
{
    // Payload of 27 bytes (16 bytes AES key,3 bytes for TL and 8 byte MAC)
  0x5B, 0x4A, 0x81, 0x30, 0xEF, 0x28, 0x05, 0x14, 0x6C, 0x53, 0xBA, 0xCD, 0xAB, 0x75, 0x53, 0x89,
  0x28, 0x16, 0x23, 0x89, 0xB7, 0xD0, 0xF4, 0xCE, 0xFC, 0x12, 0x3E,
};

/**
* @}
*/
