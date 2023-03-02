# Table of Content:

+ [Setup requirements](#setup-requirements)
+ [Provisioning mode: write a custom certificate and a private key](#provisioning-mode-write-a-custom-certificate-and-a-private-key)
    - [ECC NISTP-256](#ecc-nistp-256)
        - [Generate a new X.509 PEM formatted certificate with `modus-shell` and openssl](#generate-a-new-x509-pem-formatted-certificate-with-modus-shell-and-openssl)
        - [**Step 1.** Write the certificate into the Trust M](#step-1-write-the-certificate-into-the-trust-m)
        - [**Step 2.** Prepare a manifest and fragments](#step-2-prepare-a-manifest-and-fragments)
        - [**Step 3.** Prepare target board](#step-3-prepare-target-board)
        - [**Step 4.** Run the protected update routine](#step-4-run-the-protected-update-routine)
    - [RSA 2048](#rsa-2048)
        - [Generate a new X.509 PEM formatted certificate with `modus-shell` and openssl](generate-a-new-x509-pem-formatted-certificate-with-modus-shell-and-openssl-1)
        - [**Step 1.** Write the certificate into the Trust M](#step-1-write-the-certificate-into-the-trust-m-1)
        - [**Step 2.** Prepare a manifest and fragments](#step-2-prepare-a-manifest-and-fragments-1)
        - [**Step 3.** Prepare target board](#step-3-prepare-target-board-1)
        - [**Step 4.** Run the protected update routine](#step-4-run-the-protected-update-routine-1)
+ [How to run a custom protected data/key/metadata update in example mode](#how-to-run-a-custom-protected-datakeymetadata-update-in-example-mode)
    - [**Step 1.** Prepare a manifest and fragments](#step-1-prepare-a-manifest-and-fragments)
    - [**Step 2.** Paste the result into target headers](#step-2-paste-the-result-into-target-headers)
    - [**Step 3.** Run the protected update routine](#step-3-run-the-protected-update-routine)


# Setup requirements

1. A PSoC board flashed with provisioning mode example from the generic [README.md](README.md) description connected to the laptop
2. Opened `modus-shell` tool which comes as part of the ModusToolbox™ package
3. Installed `optigatrust` python module
    - open `modus-shell` and execute the following command `pip install optigatrust` 


# Provisioning mode: write a custom certificate and a private key

This example demonstrates a protected data/key/metadata update routine done through the command line interface. All the comands above are executed using a `modus-shell` tool which is part of the installation package.

## ECC NISTP-256

### Generate a new X.509 PEM formatted certificate with `modus-shell` and openssl

* generate a private key for a curve

    ```
    openssl ecparam -name prime256v1 -genkey -noout -out my-nistp256-key.pem
    ```

* generate corresponding public key

    ```
    openssl ec -in my-nistp256-key.pem -pubout -out my-nistp256-public-key.pem
    ```


* optional: create a self-signed certificate

    ```
    openssl req -new -x509 -key my-nistp256-key.pem -out my-nistp256-cert.pem -days 360
    ```

At the end of this excercise you should make a note of two important files: your certificate `my-nistp256-cert.pem` and a corresponding private key `my-nistp256-key.pem`, you need them for the following steps.

### **Step 1.** Write the certificate into the Trust M

* Write the certificate

    ```
    optigatrust object --id 0xe0e1 --in my-nistp256-cert.pem
    ```

* Make sure everything written is correct

    ```
    optigatrust object --id 0xe0e1 --outform PEM
    ```

* Compare with the original certificate 

    ```
    cat my-nistp256-cert.pem
    ```

### **Step 2.** Prepare a manifest and fragments

To update the key a protected key update is required, as by default it is prohibited to write a private key into a key object

Using the [manual](protected_update_data_set/README.md) and the set of sample commands for [data](protected_update_data_set/samples/protected_update_data_set/samples/gen_data_update_data_set.bat), [metadata](protected_update_data_set/samples/gen_metadata_update_data_set.bat), and [key](protected_update_data_set/samples/gen_key_update_data_set.bat), prepare a manifest and fragments. You can try with various options of the tool and view examples in the scripts mentioned above.

* Copy the **private** key to a new place

    ```
    cp my-nistp256-key.pem <path_to_the_repo>/mtb-example-optiga-data-management/protected_update_data_set/samples/
    ```

* Change the working directory 

    ```
    cd mtb-example-optiga-data-management/protected_update_data_set/samples
    ```
* Generate a manifest and required fragments

    ```
    ../bin/protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0F1 sign_algo=ES_256 priv_key=../samples/integrity/sample_ec_256_priv.pem payload_type=key key_algo=03 key_usage=10 key_data=my-nistp256-key.pem > E0F1.txt
    ```
<details>

**<summary>An output of the command</summary>**
```
user@pc ~/git/mtb-example-optiga-data-management/protected_update_data_set/samples
$ cat E0F1.txt
Tool Version : 3.00.2490
Info : Default values are set
Info : User provided inputs
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0F1
        Signature Algorithm  : ES_256
        Private key          : ../samples/integrity/sample_ec_256_priv.pem
        Type of Payload      : key
        Key Usage            : 20
        Key algorithm        : 03
        Key Data             : my-nistp256-key.pem

Info : Setting value for data formatter
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0F1
        Digest algorithm     : 29
        Signature Algorithm  : FFFFFFF9
        Type of Payload      : FFFFFFFD
Manifest Data , size : [139]
        uint8_t manifest_data[] =
        {
        0x84, 0x43, 0xA1, 0x01, 0x26, 0xA1, 0x04, 0x42, 0xE0, 0xE3, 0x58, 0x3D, 0x86, 0x01, 0xF6, 0xF6,
        0x84, 0x22, 0x18, 0x66, 0x03, 0x82, 0x03, 0x18, 0x20, 0x82, 0x82, 0x20, 0x58, 0x25, 0x82, 0x18,
        0x29, 0x58, 0x20, 0x88, 0x3B, 0x49, 0x6B, 0xF6, 0xB1, 0x2D, 0xDE, 0x9A, 0xE2, 0x0C, 0x7D, 0x13,
        0xEE, 0x13, 0x5D, 0x82, 0xB6, 0x5E, 0xD7, 0x5D, 0x49, 0xDA, 0x19, 0x1E, 0xDE, 0xF7, 0x19, 0x7C,
        0xF0, 0xC0, 0x69, 0xF6, 0x82, 0x40, 0x42, 0xE0, 0xF1, 0x58, 0x40, 0xD4, 0x96, 0x08, 0x2F, 0x7C,
        0x66, 0xB3, 0xC8, 0xA8, 0x61, 0x0A, 0x12, 0x9E, 0x74, 0x72, 0x24, 0x50, 0x43, 0x6A, 0x3C, 0xA8,
        0x8C, 0xB5, 0x5A, 0x70, 0x44, 0xDD, 0xD6, 0x17, 0xC2, 0x1D, 0x49, 0xFA, 0xF8, 0xFD, 0xB0, 0xAA,
        0x62, 0xCE, 0xA1, 0x9D, 0x88, 0x65, 0x01, 0x0F, 0xE6, 0xAA, 0xE7, 0x70, 0x43, 0x70, 0x2C, 0x21,
        0x2A, 0xF8, 0x63, 0x41, 0xA1, 0xA3, 0x0A, 0x3C, 0xFB, 0xE6, 0x16,
        };

Fragment number:[01], size:[102]
        uint8_t fragment_01[] =
        {
        0x01, 0x00, 0x20, 0xA5, 0xE5, 0x80, 0x79, 0xDF, 0x9B, 0x7A, 0x21, 0x77, 0xD0, 0x36, 0x42, 0xD7,
        0x2F, 0x93, 0x76, 0x32, 0x58, 0x94, 0xBA, 0x4D, 0xC1, 0xEA, 0x01, 0x9B, 0x51, 0x46, 0x83, 0x3A,
        0x26, 0x5A, 0xF4, 0x02, 0x00, 0x40, 0xDE, 0x44, 0xEC, 0x94, 0x41, 0x57, 0x54, 0x31, 0x7C, 0xFA,
        0x97, 0x42, 0x02, 0x28, 0x87, 0x6B, 0x95, 0x4D, 0x99, 0xCA, 0xEA, 0xD5, 0x25, 0x1D, 0xF0, 0x12,
        0x4B, 0xFF, 0x6B, 0x25, 0xE6, 0x0B, 0xDC, 0x60, 0x48, 0xD8, 0xCC, 0x7B, 0xA1, 0x0C, 0x73, 0x94,
        0x4A, 0x39, 0xAA, 0x98, 0x91, 0x0C, 0x82, 0xC3, 0x4A, 0x04, 0x1E, 0x58, 0x26, 0x68, 0xEA, 0x1A,
        0x76, 0xC7, 0x8C, 0x73, 0x43, 0x7D,
        };
```
</details>

<details>

**<summary>Command parameters explained</summary>**
The following exact parameters are used for data set generation:

Parameter             | Description
----------------------|------------------------------
`payload_version=3` | Payload version of the data to update. Default: 3. In case you try to programm the same data into the object id second time, you need to increase the version.
`trust_anchor_oid=E0E8` | To sign a manifest data, a private key that corresponds to a certain Certificate A is used. Trust Anchor OID is a data object ID where that certificate in RAW format (with no OPTIGA&trade; identity tags) must be stored; in the case of this application, it is 0xE0E3. That object will have "Trust Anchor" data type. In this case, the secret is written in advance as part of the ModusToolbox&trade; code example.
`target_oid=E0E3`    | Target object ID to be updated; in the case of this example, it is a data object ID 0xE0E3.
`sign_algo=ES_256`   | Signature algorithm that must be used to sign the manifest.
`priv_key=..\samples\integrity\sample_ec_256_priv.pem` | Path to the private key that corresponds to the certificate stored in the Trust Anchor Object ID (in this case, 0xE0E3).
`payload_type=data` | Type of the payload to send to the OPTIGA&trade; Trust M device. Here, the 0xE0F1 object (which is a key object) is being updated.
`offset=00`      | Offset in bytes. It gives an instruciton to the chip whether an offset required during writing data into the object (see the solution reference manual).
`write_type=1`     | Write type (1 - Write, 2 - EraseAndWrite) (see the solution reference manual).
`in_data_format="ascii"` | Format of teh data either `ascii` or `hex`
`data=../samples/payload/data/ascii_data.txt` | Data source


See [samples](protected_update_data_set/samples) for more variants.

</details>

<br>

### **Step 3.** Prepare target board


```
user@pc ~/git/mtb-example-optiga-data-management/protected_update_data_set/samples
$ optigatrust update-wizard --target-id 0xe0f1 --int-id 0xe0e3 --int-file integrity/sample_ec_256_cert.pem
Loaded: liboptigatrust-uart-win-i686.dll
[0/7]: To run the protected data/metadata update, a host MCU needs to send (forward) a manifest and
payload (in fragments). This data might be only signed, or signed and encrypted.
This wizard helps you to configure the target Object on the Trust M.
This configuration should match the settings set during protected payload generation on the remote server.
NB: If your target Object already has an entry in its metadata; e.g. it has a 'reset_type' already defined
 it is not possible to remove this entry anymore even after during the protected metadata update
[1/7]: Integrity protection selected. Trust Anchor [0xe0e8] is used to verify the signature of
the incoming payload during the protected update. If you leave it unlocked it can be modified in the future.
[Question]: Do you want to lock the 0xe0e8 Object? [y/N]: N
[Info]: Certificate integrity/sample_ec_256_cert.pem in DER Form has been writen into the 0xe0e8 Object ID
-----BEGIN CERTIFICATE-----
MIICWDCCAf+gAwIBAgIBLzAKBggqhkjOPQQDAjBWMQswCQYDVQQGEwJJTjENMAsG
A1UECgwESUZJTjEMMAoGA1UECwwDQ0NTMRMwEQYDVQQDDApJbnRDQSBQMjU2MRUw
EwYDVQQuEwxUcnVzdCBBbmNob3IwHhcNMTYwNTI2MDgwMTM3WhcNMTcwNjA1MDgw
MTM3WjBaMQswCQYDVQQGEwJJTjENMAsGA1UECgwESUZJTjEMMAoGA1UECwwDQ0NT
MRcwFQYDVQQDDA5lbmRFbnRpdHkgUDI1NjEVMBMGA1UELhMMVHJ1c3QgQW5jaG9y
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEGbWyFw31mF7U2XIW72E5PxRYr1wC
eAfKSI8q45C5A6HSRiAJIVKY3I6IhGeOg9HeDxzlGR0MdGBBWFs2Vfg9q6OBuTCB
tjAJBgNVHRMEAjAAMB0GA1UdDgQWBBS1l/2rNhqgoiOnaCUl+4JV0E/PuDB6BgNV
HSMEczBxgBQau1ZEZYxNT80poj9Mxryoi6QK2qFWpFQwUjELMAkGA1UEBhMCSU4x
DTALBgNVBAoMBElGSU4xDDAKBgNVBAsMA0NDUzEPMA0GA1UEAwwGUm9vdENBMRUw
EwYDVQQuEwxUcnVzdCBBbmNob3KCAS4wDgYDVR0PAQH/BAQDAgCBMAoGCCqGSM49
BAMCA0cAMEQCIGj9nI81Mwu4MoyvHIFOQSkmy7cQoHX8ia7FHZKOcu9cAiB9wetY
IfH9+17X3gbJtP9ZjTeMekjNLZl0d1idlVGPXQ==
-----END CERTIFICATE-----
[Info]: Object ID 0xe0e3 has now 'trust_anchor' data type and Execute Access Condition set to 'always'
[2/7]: Confidentiality protection is when a secret used to encrypt the protected payload prepared on the remote server. If selected it requires to know the secret so that this wizard can write it on the chip
[Question]: Do you want to enable Confidentiality protection? [y/N]: N
[3/7]: Skipped.
[4/7]: Skipped.
[5/7]: Skipped.
[6/7]: You need to select what should be updated.
[Question]: Type 1 for data, 2 for metadata, or 3 for both: 1
[Info]: target Object ID 0xe0f1 has now the following metadata:
{
    "lcso": "creation",
    "change": [
        "lcso",
        "<",
        "operational",
        "||",
        "int",
        "0xe0",
        "0xe3"
    ],
    "execute": "always"
}
```

### **Step 4.** Run the protected update routine

Open the `modus-shell` and change the working directory to the one where the E0E3.txt file from the previous step has been generated

```
optigatrust update --id 0xe0f1 --file E0F1.txt
```

## RSA 2048

### Generate a new X.509 PEM formatted certificate with `modus-shell` and openssl

* generate a private key for a curve

    ```
    openssl genrsa -out my-rsa2k-key.pem 2048
    ```

* generate corresponding public key

    ```
    openssl rsa -in my-rsa2k-key.pem -pubout -out my-rsa2k-public-key.pem
    ```


* optional: create a self-signed certificate

    ```
    openssl req -new -x509 -key my-rsa2k-key.pem -out my-rsa2k-cert.pem -days 360
    ```

At the end of this excercise you should make a note of two important files: your certificate `my-rsa2k-cert.pem` and a corresponding private key `my-rsa2k-key.pem`, you need them for the following steps.

### **Step 1.** Write the certificate into the Trust M

* Write the certificate

    ```
    optigatrust object --id 0xe0e2 --in my-rsa2k-cert.pem
    ```

* Make sure everything written is correct

    ```
    optigatrust object --id 0xe0e2 --outform PEM
    ```

* Compare with the original certificate 

    ```
    cat my-rsa2k-cert.pem
    ```

### **Step 2.** Prepare a manifest and fragments

To update the key a protected key update is required, as by default it is prohibited to write a private key into a key object

Using the [manual](protected_update_data_set/README.md) and the set of sample commands for [data](protected_update_data_set/samples/protected_update_data_set/samples/gen_data_update_data_set.bat), [metadata](protected_update_data_set/samples/gen_metadata_update_data_set.bat), and [key](protected_update_data_set/samples/gen_key_update_data_set.bat), prepare a manifest and fragments. You can try with various options of the tool and view examples in the scripts mentioned above.

* Copy the **private** key to a new place

    ```
    cp my-rsa2k-key.pem <path_to_the_repo>/mtb-example-optiga-data-management/protected_update_data_set/samples/
    ```

* Change the working directory 

    ```
    cd mtb-example-optiga-data-management/protected_update_data_set/samples
    ```
* Generate a manifest and required fragments

    ```
    ../bin/protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0FC sign_algo=ES_256 priv_key=../samples/integrity/sample_ec_256_priv.pem payload_type=key key_algo=66 key_usage=20 key_data=my-rsa2k-key.pem > E0FC.txt
    ```
<details>

**<summary>An output of the command</summary>**
```
user@pc ~/git/mtb-example-optiga-data-management/protected_update_data_set/samples
$ cat E0FC.txt
Tool Version : 3.00.2490
Info : Default values are set
Info : User provided inputs
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0FC
        Signature Algorithm  : ES_256
        Private key          : ../samples/integrity/sample_ec_256_priv.pem
        Type of Payload      : key
        Key Usage            : 20
        Key algorithm        : 66
        Key Data             : my-rsa2k-key.pem

Info : Setting value for data formatter
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0FC
        Digest algorithm     : 29
        Signature Algorithm  : FFFFFFF9
        Type of Payload      : FFFFFFFD
Manifest Data , size : [141]
        uint8_t manifest_data[] =
        {
        0x84, 0x43, 0xA1, 0x01, 0x26, 0xA1, 0x04, 0x42, 0xE0, 0xE3, 0x58, 0x3F, 0x86, 0x01, 0xF6, 0xF6,
        0x84, 0x22, 0x19, 0x02, 0x0D, 0x03, 0x82, 0x18, 0x42, 0x18, 0x20, 0x82, 0x82, 0x20, 0x58, 0x25,
        0x82, 0x18, 0x29, 0x58, 0x20, 0xC6, 0x54, 0x45, 0x7B, 0xF4, 0x07, 0x66, 0x5C, 0xF9, 0xA9, 0x51,
        0x62, 0x80, 0xE4, 0x86, 0xD5, 0xFE, 0x95, 0x98, 0x57, 0x5C, 0x0B, 0x20, 0xBE, 0x97, 0x35, 0x16,
        0x55, 0xD4, 0x11, 0x1C, 0x00, 0xF6, 0x82, 0x40, 0x42, 0xE0, 0xFC, 0x58, 0x40, 0xC1, 0x2D, 0x73,
        0x55, 0xF6, 0xE0, 0x88, 0x62, 0x62, 0xB6, 0x3F, 0x2E, 0x1E, 0x92, 0xBA, 0xEE, 0x96, 0x04, 0x4A,
        0xD8, 0x7F, 0x23, 0x8E, 0xED, 0x11, 0x80, 0x90, 0xF7, 0xF8, 0x86, 0xC1, 0x1A, 0xBF, 0xCD, 0x80,
        0x25, 0xBB, 0xD6, 0xDE, 0x52, 0x60, 0x93, 0x1B, 0xDE, 0xD0, 0x3F, 0x39, 0x73, 0x85, 0x03, 0xDC,
        0x04, 0x64, 0xF8, 0x8F, 0x26, 0x06, 0x99, 0x51, 0xDE, 0xF7, 0x99, 0x15, 0x1D,
        };

Fragment number:[01], size:[525]
        uint8_t fragment_01[] =
        {
        0x01, 0x01, 0x00, 0x87, 0x7D, 0x18, 0xD2, 0x82, 0x9B, 0x86, 0x38, 0x3F, 0xF7, 0xD8, 0xBE, 0xFA,
        0xEB, 0x3E, 0x9A, 0xC3, 0xF2, 0xBE, 0x6A, 0x6C, 0x50, 0x73, 0xB9, 0xE9, 0x76, 0x40, 0x49, 0xBE,
        0x57, 0x89, 0xA1, 0x1E, 0x80, 0x81, 0x7A, 0x7C, 0x03, 0xD5, 0x8A, 0x35, 0x56, 0x10, 0x00, 0x25,
        0xB6, 0xCF, 0x3B, 0x0F, 0xC4, 0x26, 0xB6, 0x3D, 0xB9, 0x5A, 0x73, 0x0D, 0x57, 0x32, 0x74, 0xD1,
        0x66, 0xAF, 0x3D, 0x12, 0xC5, 0x43, 0xA2, 0x31, 0xFD, 0x30, 0xAC, 0x13, 0x26, 0xA5, 0x9B, 0x06,
        0xFD, 0x40, 0x61, 0x9B, 0x81, 0x73, 0x26, 0xAC, 0x3C, 0x78, 0x51, 0x67, 0xC6, 0xDF, 0x43, 0x50,
        0x73, 0xBA, 0x8F, 0x13, 0xC5, 0x36, 0x58, 0x9F, 0x9D, 0x54, 0x22, 0x06, 0xA9, 0x4D, 0xE6, 0x6F,
        0x27, 0x49, 0x3B, 0x52, 0x82, 0xBD, 0x4D, 0x50, 0x5C, 0x86, 0x2D, 0x46, 0x2A, 0xD0, 0xA2, 0x18,
        0xEB, 0x53, 0x8B, 0x44, 0xFC, 0x9C, 0x88, 0xAE, 0xF5, 0x6D, 0xD9, 0xBF, 0x65, 0x33, 0x43, 0x02,
        0x33, 0x67, 0xF6, 0x6F, 0x5D, 0x96, 0x2C, 0x16, 0x7E, 0x67, 0x9A, 0x00, 0xF9, 0xE7, 0xE3, 0xF4,
        0xC4, 0xE8, 0x1B, 0x43, 0xD6, 0x9F, 0x64, 0xC9, 0xE8, 0xAF, 0x00, 0xB9, 0x5D, 0x49, 0xFF, 0x97,
        0xEE, 0xB8, 0x66, 0x57, 0x23, 0x68, 0x9A, 0x65, 0x04, 0xA3, 0xE6, 0x73, 0x11, 0xDF, 0x6C, 0x0F,
        0x32, 0x7E, 0xFB, 0xED, 0x3E, 0x34, 0x15, 0xEE, 0xA2, 0xFF, 0x23, 0x34, 0x69, 0xA1, 0xCE, 0xF2,
        0x5D, 0x1A, 0xD4, 0xD4, 0x92, 0xC2, 0x90, 0x5F, 0x93, 0x3B, 0x16, 0x3B, 0xCB, 0xD3, 0xC1, 0xCC,
        0xF8, 0x06, 0x95, 0xC3, 0x0B, 0xB4, 0xB9, 0xC2, 0xF0, 0x58, 0x4F, 0x6B, 0xC0, 0x2A, 0x46, 0x7D,
        0xB6, 0x7C, 0x3E, 0x10, 0x2B, 0xC1, 0xDF, 0x0E, 0x56, 0x65, 0x4D, 0x40, 0xAE, 0xC7, 0xFA, 0x40,
        0x2D, 0xEF, 0x71, 0x02, 0x01, 0x00, 0xBC, 0x52, 0x28, 0xA9, 0xEF, 0xA0, 0x0E, 0x87, 0x1A, 0xF7,
        0xDA, 0xB9, 0xF9, 0xC4, 0xAF, 0xE1, 0x00, 0x56, 0xE1, 0xF7, 0x35, 0x59, 0x27, 0xE4, 0x3A, 0xD5,
        0x5F, 0x69, 0x03, 0x45, 0x1E, 0xFF, 0xF3, 0xAC, 0x5D, 0xAF, 0x03, 0x49, 0x57, 0xC6, 0xDA, 0xF2,
        0x30, 0x84, 0x98, 0x2B, 0x28, 0x60, 0xA3, 0x23, 0x25, 0x99, 0xB9, 0x57, 0x0C, 0xC9, 0xD1, 0xB8,
        0xB0, 0x8A, 0x94, 0x0E, 0x5B, 0xD4, 0xBA, 0x5E, 0xC6, 0xF6, 0x2A, 0x55, 0xB8, 0xEF, 0xBC, 0xF3,
        0x85, 0xA1, 0x96, 0x29, 0x55, 0xF4, 0xFA, 0x4F, 0x72, 0x2F, 0x86, 0x7C, 0x74, 0x75, 0x8E, 0xA0,
        0x20, 0xE4, 0x61, 0x17, 0xF4, 0x54, 0xCC, 0x30, 0x6B, 0x72, 0x3D, 0x33, 0x15, 0x61, 0xFC, 0xF1,
        0xEF, 0x81, 0x11, 0x31, 0x7A, 0xDA, 0x82, 0x42, 0xFC, 0x8A, 0xB2, 0x5D, 0x58, 0x35, 0xB5, 0x8A,
        0xCB, 0x92, 0xD5, 0xB2, 0x5A, 0x0C, 0xF2, 0x2C, 0xC6, 0xAE, 0x68, 0xB5, 0x5D, 0xCF, 0x23, 0x72,
        0x58, 0xA1, 0xFF, 0xF0, 0x16, 0x84, 0xFF, 0xDF, 0x88, 0x4B, 0x31, 0xED, 0xF4, 0x46, 0x7E, 0xB1,
        0x10, 0x70, 0x0F, 0xE3, 0x17, 0x13, 0xDB, 0x79, 0xBD, 0x54, 0x43, 0xEA, 0xEC, 0x7D, 0xA1, 0x98,
        0x06, 0x29, 0x3D, 0x83, 0x87, 0x2C, 0xFB, 0xDE, 0x05, 0x0F, 0xA1, 0xFF, 0x55, 0x9E, 0x44, 0x8E,
        0xDF, 0x8B, 0x84, 0x66, 0xB3, 0x5C, 0x35, 0x4A, 0x1B, 0xF2, 0x06, 0xBD, 0x9E, 0x4A, 0xA7, 0xCB,
        0x88, 0x6C, 0xF0, 0x5F, 0xC2, 0x61, 0x25, 0xE4, 0xF7, 0x4F, 0xF8, 0x74, 0xA8, 0x16, 0x36, 0x4E,
        0x69, 0x38, 0xAC, 0x98, 0x29, 0x97, 0x39, 0xAA, 0x4A, 0x68, 0xDA, 0x0C, 0xF1, 0x30, 0x6F, 0x58,
        0x92, 0x2A, 0xE0, 0xC0, 0xF1, 0xCA, 0xCF, 0x64, 0x7D, 0xC1, 0x3A, 0xBD, 0xA5, 0x9D, 0x40, 0x41,
        0x25, 0x6A, 0xF8, 0x9C, 0x67, 0x83, 0x03, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01,
        };
```
</details>

<details>

**<summary>Command parameters explained</summary>**
The following exact parameters are used for data set generation:

Parameter             | Description
----------------------|------------------------------
`payload_version=3` | Payload version of the data to update. Default: 3. In case you try to programm the same data into the object id second time, you need to increase the version.
`trust_anchor_oid=E0E8` | To sign a manifest data, a private key that corresponds to a certain Certificate A is used. Trust Anchor OID is a data object ID where that certificate in RAW format (with no OPTIGA&trade; identity tags) must be stored; in the case of this application, it is 0xE0E3. That object will have "Trust Anchor" data type. In this case, the secret is written in advance as part of the ModusToolbox&trade; code example.
`target_oid=E0FC`    | Target object ID to be updated; in the case of this example, it is a data object ID 0xE0E3.
`sign_algo=ES_256`   | Signature algorithm that must be used to sign the manifest.
`priv_key=..\samples\integrity\sample_ec_256_priv.pem` | Path to the private key that corresponds to the certificate stored in the Trust Anchor Object ID (in this case, 0xE0E3).
`payload_type=key` | Type of the payload to send to the OPTIGA&trade; Trust M device. Here, the 0xE0FC object (which is a key object) is being updated.
`key_algo=66`      | Key algorithm which is being written. 66 Stands for RSA 2048
`key_usage=10`     | Key Usage (see the solution reference manual).
`key_data=my-rsa2k-key.pem` | Data source


See [samples](protected_update_data_set/samples) for more variants.

</details>

<br>

### **Step 3.** Prepare target board


```
user@pc ~/git/mtb-example-optiga-data-management/protected_update_data_set/samples
$ optigatrust update-wizard --target-id 0xe0fc --int-id 0xe0e3 --int-file integrity/sample_ec_256_cert.pem
Loaded: liboptigatrust-uart-win-i686.dll
Error: 0x204
Error: 0x204
Error: 0x204
[0/7]: To run the protected data/metadata update, a host MCU needs to send (forward) a manifest and
payload (in fragments). This data might be only signed, or signed and encrypted.
This wizard helps you to configure the target Object on the Trust M.
This configuration should match the settings set during protected payload generation on the remote server.
NB: If your target Object already has an entry in its metadata; e.g. it has a 'reset_type' already defined
 it is not possible to remove this entry anymore even after during the protected metadata update
[1/7]: Integrity protection selected. Trust Anchor [0xe0e3] is used to verify the signature of
the incoming payload during the protected update. If you leave it unlocked it can be modified in the future.
[Question]: Do you want to lock the 0xe0e3 Object? [y/N]: N
Error: 0x204
[Info]: Certificate integrity/sample_ec_256_cert.pem in DER Form has been writen into the 0xe0e3 Object ID
-----BEGIN CERTIFICATE-----
MIICWDCCAf+gAwIBAgIBLzAKBggqhkjOPQQDAjBWMQswCQYDVQQGEwJJTjENMAsG
A1UECgwESUZJTjEMMAoGA1UECwwDQ0NTMRMwEQYDVQQDDApJbnRDQSBQMjU2MRUw
EwYDVQQuEwxUcnVzdCBBbmNob3IwHhcNMTYwNTI2MDgwMTM3WhcNMTcwNjA1MDgw
MTM3WjBaMQswCQYDVQQGEwJJTjENMAsGA1UECgwESUZJTjEMMAoGA1UECwwDQ0NT
MRcwFQYDVQQDDA5lbmRFbnRpdHkgUDI1NjEVMBMGA1UELhMMVHJ1c3QgQW5jaG9y
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEGbWyFw31mF7U2XIW72E5PxRYr1wC
eAfKSI8q45C5A6HSRiAJIVKY3I6IhGeOg9HeDxzlGR0MdGBBWFs2Vfg9q6OBuTCB
tjAJBgNVHRMEAjAAMB0GA1UdDgQWBBS1l/2rNhqgoiOnaCUl+4JV0E/PuDB6BgNV
HSMEczBxgBQau1ZEZYxNT80poj9Mxryoi6QK2qFWpFQwUjELMAkGA1UEBhMCSU4x
DTALBgNVBAoMBElGSU4xDDAKBgNVBAsMA0NDUzEPMA0GA1UEAwwGUm9vdENBMRUw
EwYDVQQuEwxUcnVzdCBBbmNob3KCAS4wDgYDVR0PAQH/BAQDAgCBMAoGCCqGSM49
BAMCA0cAMEQCIGj9nI81Mwu4MoyvHIFOQSkmy7cQoHX8ia7FHZKOcu9cAiB9wetY
IfH9+17X3gbJtP9ZjTeMekjNLZl0d1idlVGPXQ==
-----END CERTIFICATE-----
[Info]: Object ID 0xe0e3 has now 'trust_anchor' data type and Execute Access Condition set to 'always'
[2/7]: Confidentiality protection is when a secret used to encrypt the protected payload prepared on the remote server. If selected it requires to know the secret so that this wizard can write it on the chip
[Question]: Do you want to enable Confidentiality protection? [y/N]: N
[3/7]: Skipped.
[4/7]: Skipped.
[5/7]: Skipped.
[6/7]: You need to select what should be updated.
[Question]: Type 1 for data, 2 for metadata, or 3 for both: 1
[Info]: target Object ID 0xe0fc has now the following metadata:
{
    "lcso": "creation",
    "change": [
        "lcso",
        "<",
        "operational",
        "||",
        "int",
        "0xe0",
        "0xe3"
    ],
    "execute": "always"
}
```

### **Step 4.** Run the protected update routine

Open the `modus-shell` and change the working directory to the one where the E0E3.txt file from the previous step has been generated

```
optigatrust update --id 0xe0fc --file E0FC.txt
```

# How to run a custom protected data/key/metadata update in example mode

To learn more about the protected data/key/metadata update procedure, see the solution reference manual or the [Wiki page](https://github.com/Infineon/optiga-trust-m/wiki/Protected-Update-for-Data-Objects).
For this exercise you need to open a Windows Comamnder


## **Step 1.** Prepare a manifest and fragments

Using the [manual](protected_update_data_set/README.md) and the set of sample commands for [data](protected_update_data_set/samples/protected_update_data_set/samples/gen_data_update_data_set.bat), [metadata](protected_update_data_set/samples/gen_metadata_update_data_set.bat), and [key](protected_update_data_set/samples/gen_key_update_data_set.bat), prepare a manifest and fragments. You can try with various options of the tool and view examples in the scripts mentioned above. The output is as shown below:

<details>

**<summary>Manifest and fragments sample for the protected data update procedure</summary>**

```
mtb-example-optiga-data-management\protected_update_data_set\samples>..\bin\protected_update_data_set.exe payload_version=3 trust_anchor_oid=E0E3 target_oid=E0F1 sign_algo=ES_256 priv_key=..\samples\integrity\sample_ec_256_priv.pem payload_type=key key_algo=03 key_usage=10 key_data=..\samples\payload\key\ecc_secp256r1_test.pem secret=..\samples\confidentiality\secret.txt label="test" seed_length=64 enc_algo="AES-CCM-16-64-128" secret_oid=F1D1
Tool Version : 3.00.2490
Info : Default values are set
Info : User provided inputs
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0F1
        Signature Algorithm  : ES_256
        Private key          : ..\samples\integrity\sample_ec_256_priv.pem
        Type of Payload      : key
        Shared secret        : ..\samples\confidentiality\secret.txt
        Shared secret OID    : F1D1
        Label                : test
        Encryption algorithm : AES-CCM-16-64-128
        Length of seed       : 64
        Key Usage            : 10
        Key algorithm        : 03
        Key Data             : ..\samples\payload\key\ecc_secp256r1_test.pem

Info : Setting value for data formatter
        Payload version      : 3
        Trust anchor oid     : E0E3
        Target oid           : E0F1
        Digest algorithm     : 29
        Signature Algorithm  : FFFFFFF9
        Type of Payload      : FFFFFFFD
        Length of seed       : 40
        Label                : test
        Shared secret OID    : F1D1
0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6, 0xD4, 0xC5, 0x4F, 0x5B, 0x12, 0xC5, 0x7E, 0xDB, 0x27,
        0xCE, 0xD2, 0x24, 0x04, 0x8F, 0x25, 0x48, 0x2A, 0xA1, 0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6,
        0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6, 0xD4, 0xC5, 0x4F, 0x5B, 0x12, 0xC5, 0x7E, 0xDB, 0x27,
        0xCE, 0xD2, 0x24, 0x04, 0x8F, 0x25, 0x48, 0x2A, 0xA1, 0x49, 0xC9, 0xF4, 0x92, 0xA9, 0x92, 0xF6,
        Encryption algorithm : A
        Key Derivation Function : FFFEFF48
Manifest Data , size : [234]
        uint8_t manifest_data[] =
        {
        0x84, 0x43, 0xA1, 0x01, 0x26, 0xA1, 0x04, 0x42, 0xE0, 0xE3, 0x58, 0x9C, 0x86, 0x01, 0xF6, 0xF6,
        0x84, 0x22, 0x18, 0x66, 0x03, 0x82, 0x03, 0x10, 0x82, 0x82, 0x20, 0x58, 0x25, 0x82, 0x18, 0x29,
        0x58, 0x20, 0xDB, 0xBE, 0x5F, 0x01, 0x99, 0xEB, 0x44, 0x15, 0xA4, 0x4B, 0xFE, 0xC7, 0x7F, 0x9C,
        0xAB, 0x25, 0x71, 0x87, 0x22, 0xFA, 0x18, 0xB5, 0x4D, 0x5F, 0x0B, 0xC2, 0xF8, 0x7A, 0x1A, 0x80,
        0xAF, 0xD5, 0x82, 0x01, 0x83, 0x43, 0xA1, 0x01, 0x0A, 0x81, 0x82, 0x58, 0x54, 0xA3, 0x04, 0x42,
        0xF1, 0xD1, 0x01, 0x3A, 0x00, 0x01, 0x00, 0xB7, 0x05, 0x82, 0x44, 0x74, 0x65, 0x73, 0x74, 0x58,
        0x40, 0xD8, 0x57, 0xEB, 0xCD, 0x8E, 0x6B, 0x14, 0xBE, 0xBC, 0xFE, 0xB9, 0x13, 0x0E, 0xA3, 0x90,
        0xDB, 0xBB, 0xA6, 0xF8, 0xF5, 0x48, 0x27, 0x01, 0xE4, 0xDB, 0xB5, 0xBE, 0xE6, 0x46, 0x1F, 0x5B,
        0x02, 0xA3, 0xB0, 0x10, 0xC4, 0xC8, 0x4C, 0x5F, 0x0D, 0xBC, 0x93, 0x1D, 0x83, 0x2A, 0x59, 0xC8,
        0xDC, 0xC8, 0x4C, 0x40, 0x49, 0x50, 0x4D, 0xEA, 0x1E, 0xBE, 0x09, 0xFD, 0x7F, 0x1C, 0x83, 0x13,
        0x5B, 0xF6, 0xF6, 0x82, 0x40, 0x42, 0xE0, 0xF1, 0x58, 0x40, 0x1E, 0x64, 0xAA, 0x6D, 0xC6, 0xAF,
        0xFD, 0x89, 0x67, 0x20, 0x21, 0x16, 0x07, 0xD0, 0x0A, 0x2C, 0x58, 0xBA, 0x6A, 0xA9, 0x97, 0x1E,
        0x2C, 0x32, 0xF9, 0x39, 0xD5, 0x6E, 0x77, 0x9A, 0x54, 0x45, 0x2B, 0x37, 0xD8, 0x84, 0x3D, 0x07,
        0x98, 0xB9, 0x1C, 0xFB, 0xEA, 0x11, 0xB8, 0x94, 0x3C, 0x2A, 0xA5, 0x41, 0x59, 0x76, 0x46, 0x47,
        0xA8, 0xC6, 0x38, 0x9E, 0x76, 0x64, 0xF4, 0x07, 0x81, 0xB4,
        };

Fragment number:[01], size:[110]
        uint8_t fragment_01[] =
        {
        0x43, 0xA8, 0xF0, 0x47, 0x06, 0xFE, 0xDB, 0x36, 0x91, 0x98, 0x7F, 0x94, 0xFA, 0xB8, 0x82, 0xAD,
        0x1B, 0x1C, 0x92, 0xC0, 0x30, 0x1B, 0x1D, 0x16, 0x54, 0x23, 0x2D, 0x67, 0x78, 0xB5, 0x51, 0x3A,
        0x30, 0x0C, 0xC0, 0x9B, 0xA9, 0x58, 0x4D, 0xF9, 0xFB, 0x53, 0x4F, 0x66, 0xE5, 0xF3, 0xF3, 0x43,
        0xA3, 0x20, 0xCD, 0xA7, 0x4D, 0x54, 0x0D, 0xAB, 0x45, 0x51, 0x0F, 0xB5, 0x28, 0xE2, 0x20, 0x89,
        0x50, 0xB2, 0xA1, 0x6E, 0xF1, 0xA5, 0x86, 0x59, 0xA7, 0x8E, 0x84, 0xE0, 0x74, 0xC5, 0x84, 0xFF,
        0xE0, 0x69, 0xC8, 0xC0, 0x25, 0xF6, 0x2C, 0x64, 0x1F, 0xBE, 0xD9, 0x7F, 0x4A, 0x15, 0x6C, 0x21,
        0x4C, 0xCB, 0xDE, 0x4F, 0x74, 0x66, 0x6D, 0x9E, 0x81, 0x56, 0x77, 0x4F, 0x33, 0x70,
        };
```


The following exact parameters are used for data set generation:

Parameter             | Description
----------------------|------------------------------
`payload_version=3` | Payload version of the data to update. Default: 3.
`trust_anchor_oid=E0E3` | To sign a manifest data, a private key that corresponds to a certain Certificate A is used. Trust Anchor OID is a data object ID where that certificate in RAW format (with no OPTIGA&trade; identity tags) must be stored; in the case of this application, it is 0xE0E3. That object will have "Trust Anchor" data type. In this case, the secret is written in advance as part of the ModusToolbox&trade; code example.
`target_oid=E0F1`    | Target object ID to be updated; in the case of this example, it is a key object ID 0xE0F1.
`sign_algo=ES_256`   | Signature algorithm that must be used to sign the manifest.
`priv_key=..\samples\integrity\sample_ec_256_priv.pem` | Path to the private key that corresponds to the certificate stored in the Trust Anchor Object ID (in this case, 0xE0E3).
`payload_type=key` | Type of the payload to send to the OPTIGA&trade; Trust M device. Here, the 0xE0F1 object (which is a key object) is being updated.
`key_algo=03`      | Target key algorithm (see the solution reference manual).
`key_usage=10`     | Target key usage (see the solution reference manual).
`key_data=..\samples\payload\key\ecc_secp256r1_test.pem` | Final payload that must be written into the target object ID. In this case, it is 0xE0F1.
`secret=..\samples\confidentiality\secret.txt` | Fragments (split payload) will be encrypted; a defined secret is used.
`label="test"` | Label for encryption in CCM mode.
`seed_length=64` |
`enc_algo="AES-CCM-16-64-128"` | Encryption algorithm.
`secret_oid=F1D1` | OPTIGA&trade; Trust M must be able to decrypt the payload. The manifest defines from where the security solution must take a secret to do that. In this case, the secret is written in advance as part of the ModusToolbox&trade; code example.

See [samples](protected_update_data_set/samples) for more variants.

</details>

<br>

## **Step 2.** Paste the result into target headers

The resulting C language structures will be replaced with the default values in the code. For example, the content of the `uint8_t manifest_data[] =` buffer will be inserted into the [`const uint8_t manifest_ecc_key[]`](config/ecc_key_object_confidentiality.h#L43) line, because the ECC key is the target type of the update. The ModusToolbox&trade; code example will extract the manifest from that buffer and use it for the protected key update procedure. The content of `uint8_t fragment_01[] =` will be replaced with a content of the `const uint8_t ecc_key_final_fragment_array[] =` buffer.

By controlling the macros in the [`config/example_optiga_util_protected_update.h`](config/example_optiga_util_protected_update.h#L53-70) file, you can control the routine to be executed during the update. For example, to execute only the ECC key update with manifest/fragments protected for confidentiality and integrity, ensure that the following macros are **commented out**:

- `AES_KEY_UPDATE`
- `RSA_KEY_UPDATE`
- `EXAMPLE_OPTIGA_UTIL_PROTECTED_UPDATE_OBJECT_METADATA_ENABLED`


## **Step 3.** Run the protected update routine

Run the example as described in the sections above.
