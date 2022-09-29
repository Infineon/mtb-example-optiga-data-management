# OPTIGA™ Trust M: Data and certificates management

This example demonstrates how you can manage data and certificates on a OPTIGA™ Trust M V3 security solution using a PSoC™ 6 MCU as a bridge. This example has the following two setups:

- **Example mode:** Figure 1 shows a generic example demonstrated as part of the ModusToolbox&trade; project showing direct API calls such as `optiga_util_read_data`, `optiga_util_write_data`, and `optiga_util_protected_update` to read, write, and securely update the data on the OPTIGA™ Trust M device.

- **Provisioning mode:** Figure 2 shows a special example, where the `optigatrust` Python module can be used to directly access the device and manipulate internal settings. In this case, the host MCU is turned into a bridge to allow Python commands reach the security solution so that the communication occurs in the following sequence:

   **Python Script <-> USB <-> PSoC™ 6 MCU <-> I2C <-> OPTIGA™ Trust M**

**Figure 1. Example mode**

![](images/data_management_setup.png)

<br>

**Figure 2. Provisioning mode**

![](images/data_management_setup_python.png)


[View this README on GitHub.](https://github.com/Infineon/mtb-example-optiga-data-management)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzM3MzUiLCJTcGVjIE51bWJlciI6IjAwMi0zMzczNSIsIkRvYyBUaXRsZSI6Ik9QVElHQeKEoiBUcnVzdCBNOiBEYXRhIGFuZCBjZXJ0aWZpY2F0ZXMgbWFuYWdlbWVudCIsInJpZCI6Inl1c2hldiIsIkRvYyB2ZXJzaW9uIjoiMS4wLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTi9BIiwiRG9jIEJVIjoiTi9BIiwiRG9jIEZhbWlseSI6Ik4vQSJ9)

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox) v2.4
- Board Support Package (BSP) minimum required version: 2.0.0
  > **Important Note:** The `optiga-trust-m` middleware supports only BSP versions 2.x.
- Programming language: C
- Associated parts:
  - All [PSoC&trade; 6 MCU](http://www.cypress.com/PSoC6) parts
  - [OPTIGA™ Trust M V3](https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/)


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® Embedded Compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm Compiler v6.13 (`ARM`)
- IAR C/C++ Compiler v8.42.2 (`IAR`)


## Supported kits (make variable 'TARGET')

- [OPTIGA™ Trust IoT security development kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-m-iot-kit/) (`CYSBSYSKIT-DEV-01`) – Default value of `TARGET`
- [PSoC™ 62S2 evaluation kit](https://www.infineon.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2`)
- [PSoC™ 62S2 evaluation kit with the Sterling-LWB5+ M.2 radio module](https://www.infineon.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2-LAI-4373M2`)


## Software setup

- **Example mode:** Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

- **Provisioning mode:** Do the following:
   1. Install an `optigatrust` Python module.

   2. Open the `modus-shell` tool (locally available at *{ModusToolbox&trade; install directory}/tools_2.X)*.

   3. In the terminal emulator, execute the following command:

       ```
       pip install optigatrust
       ```

## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox&trade; Application**). This launches the [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. (Optional) Change the suggested **New Application Name**.

5. The **Application(s) Root Path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) Root Path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the Project Creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[Hello World](https://github.com/Infineon/mtb-example-psoc6-hello-world)" application with the desired name "MyHelloWorld" configured for the *CYSBSYSKIT-DEV-01* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CYSBSYSKIT-DEV-01 --app-id mtb-example-psoc6-hello-world --user-app-name MyHelloWorld --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [Project Creator](https://www.cypress.com/ModusToolboxProjectCreator) tool:**

   1. Launch Project Creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose Board Support Package** screen, select the BSP, and click **Next**.

   3. In the **Select Application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>



## Operation

Depending on the configuration, use either [Example mode](#example-mode) or [Provisioning mode](#provisioning-mode).


### Example mode

The examples are presented in the form of various API calls.

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

3. Open the *Makefile* in any text editor, find the line `PROVISIONING_MODE` and change the value from '1' to '0', and then save the change.

    ```c
    # Define whether the example should be compiled in provisioning mode
    PROVISIONING_MODE=0
    ```

4. Program the board.

   - **Using Eclipse IDE for ModusToolbox&trade;:**

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.

   - **Using CLI:**

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. You can specify a target and toolchain manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CYSBSYSKIT-DEV-01 TOOLCHAIN=GCC_ARM
      ```

5. After programming, connect the serial terminal and see the output there.


### Provisioning mode

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

3. Open the *Makefile* in any text editor, find the line `PROVISIONING_MODE` and change the value from '0' to '1', and then save the change.

    ```c
    # Define whether the example should be compiled in provisioning mode
    PROVISIONING_MODE=1
    ```

4. Program the board.

   - **Using Eclipse IDE for ModusToolbox&trade;:**

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.

   - **Using CLI:**

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. You can specify a target and toolchain manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CYSBSYSKIT-DEV-01 TOOLCHAIN=GCC_ARM
      ```

5. After programming, unplug the board from the USB port and reconnect it again. Make sure that all serial communication to that port (for example, working Tera Term sessions) are closed.

6. Open the `modus-shell` tool again as described in the [Software setup](#software-setup) section.

7. To try out the connection with the security solution, execute the following command:
   ```
   optigatrust object --id 0xe0e0 --outform PEM
   ```
8. To see the metadata associated with that object, execute the following command:
   ```
   optigatrust object --id 0xe0e0 --meta
   ```

For more examples, see the [*optigatrust* command line interface](#optigatrust-command-line-interface) section.


## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For more details, see the "Program and Debug" section in the [Eclipse IDE for ModusToolbox User Guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

**Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice - once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.infineon.com/docs/DOC-21143) to learn about this and for the workaround.


## Design and implementation

### Provisioning mode design

The general idea is to link the `optigatrust` Python module and an OPTIGA™ Trust M security solution through I2C. For this, the PSoC&trade; 6 board is flashed with a dedicated firmware, which turns it into a bridge device translating the commands from the Python library to the security solution. In this case, the setup looks similar to the following sequence:

 **optigatrust Python modules <-> USB (UART) <-> PSoC&trade; 6 MCU <-> I2C <-> OPTIGA™ Trust M**

The firmware implements a very rudimentary finite state machine (the *provisioning_sm.c* file) with the following four states:

- `INIT_STATE` – Initialization state to open the Infineon I2C protocol with the security solution. Afterwards, a transition into the `RX_STATE` is triggered.
- `RX_STATE` – A state in which the host (PSoC&trade; 6 MCU) is waiting for a start sequence over the UART interface:
    ```
    uint8_t start_seq[] = {0xbe, 0xef, 0xde, 0xad}`)
    ```
    Afterwards, the APDU from the Python script is forwarded over the Infineon I2C protocol to the OPTIGA™ Trust chip, at the same time expecting an answer either from the security solution or an error code. Afterwards, a transition into the `TX_STATE` is triggered.
- `TX_STATE` – A state which prepares the answer from the OPTIGA™ Trust M to send over the UART interface back to the host system. Afterwards, the state is again in `RX_STATE`.
- `ERROR_STATE`


### Example mode design

The main file executes several sub-examples:

- Read out the data from the OPTIGA™ Trust M using `example_optiga_util_read_data()`
- Write data to the OPTIGA™ Trust M using `example_optiga_util_write_data()`
- Demonstrate how to work with build-in counters using `example_optiga_util_update_count()`
- Run a protected update routine for key objects, data objects, and metadata associated with the objects/keys using `example_optiga_util_protected_update()`. For more information, see [How to run a custom protected data/key/metadata update](#how-to-run-a-custom-protected-datakeymetadata-update).
- Read the coprocessor unique ID using `example_read_coprocessor_id()`. This is a demonstration of the `optiga_util_read_data()` function call using a coprocessor unique ID object.


### How to run a custom protected data/key/metadata update

To learn more about the protected data/key/metadata update procedure, see the solution reference manual or the [Wiki page](https://github.com/Infineon/optiga-trust-m/wiki/Protected-Update-for-Data-Objects).


#### **Step 1.** Prepare a manifest and fragments

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

#### **Step 2.** Paste the result into target headers

The resulting C language structures will be replaced with the default values in the code. For example, the content of the `uint8_t manifest_data[] =` buffer will be inserted into the [`const uint8_t manifest_ecc_key[]`](config/ecc_key_object_confidentiality.h#L43) line, because the ECC key is the target type of the update. The ModusToolbox&trade; code example will extract the manifest from that buffer and use it for the protected key update procedure. The content of `uint8_t fragment_01[] =` will be replaced with a content of the `const uint8_t ecc_key_final_fragment_array[] =` buffer.

By controlling the macros in the [`config/example_optiga_util_protected_update.h`](config/example_optiga_util_protected_update.h#L53-70) file, you can control the routine to be executed during the update. For example, to execute only the ECC key update with manifest/fragments protected for confidentiality and integrity, ensure that the following macros are **commented out**:

- `AES_KEY_UPDATE`
- `RSA_KEY_UPDATE`
- `EXAMPLE_OPTIGA_UTIL_PROTECTED_UPDATE_OBJECT_METADATA_ENABLED`


#### **Step 3.** Run the protected update routine

Run the example as described in the sections above.

### *optigatrust* command-line interface

| Command            | Note                |
| ------------------ | -------------------- |
| `optigatrust object --help` | Displays the usage information for the command `object`|
| `optigatrust object --id 0xe0e0` | Read the content of the object 0xe0e0 (to get the [objects map](https://infineon.github.io/python-optiga-trust/metadata.html)) |
| `optigatrust object --id 0xe0e0 --out certificate.pem --outform PEM` | Read the content of the object 0xe0e0, try to convert it into a PEM-formatted X.509 certificate, and store the result into a *certificate.pem* text file |
| `optigatrust object --id 0xe0e0 --meta` | Read the metadata of the object 0xe0e0 |
| `optigatrust object --in data_metadata.json` | Write to the OPTIGA™ Trust data and metadata from JSON format  |
| `optigatrust object --export-otc` | Read all the objects from the OPTIGA™ Trust and store them into a format compatible with OPTIGA&trade; Trust configurator |
| `optigatrust object --export-json` | Read all objects from the OPTIGA™ Trust and store them into JSON format* |
| `optigatrust create-keys --help` | Displays the usage information for the command `create-keys`  |
| `optigatrust create-keys --id 0xe0f1` | Generate an ECC NISTP256 public/private key-pair and store the private component in the given key [object ID](https://infineon.github.io/python-optiga-trust/metadata.html). Private key usage is set to *signature* by default.  |
| `optigatrust create-keys --id 0xe0f1 --pubout public.pkey --privout private.key` | Generate an ECC NISTP256 public/private key-pair and store them in the corresponding files. The output is PEM-formatted.    |
| `optigatrust create-keys --id 0xe0f1 --curve secp384r1` | Generate an ECC NISTP384 public/private key-pair |
| `optigatrust create-keys --id 0xe0f1 --key-usage key_agreement --key-usage signature` | Generate an ECC NISTP256 public/private key-pair and put the private key usage to be *KeyAgreement* and *Signature* |
| `optigatrust create-keys --id 0xe0fc --rsa --key_size 2048` | Generate an RSA public/private key-pair with a key size of 2048 bits. Private key usage is set to *Signature* by default. |

For other commands, see the [`python-optiga-trust`](https://github.com/Infineon/python-optiga-trust) repository.

<br>

<details><summary><b>The format can look like the following:</b></summary>


```json
{
    "e0e1":
    {
        "pretty_metadata":
        {
            "change": ['int','0xe0','0xe9','&&','conf','0xe2','0x00'],
        },
        "data":"c002010001fe0001fb308201f73082017ca00302010202045d7367cc300a06082a8648ce3d0403033072310b30090603550406130244453121301f060355040a0c18496e66696e656f6e20546563686e6f6c6f6769657320414731133011060355040b0c0a4f505449474128544d29312b302906035504030c22496e66696e656f6e204f505449474128544d29205472757374204d20434120333030301e170d3230303931303131343033325a170d3430303931303131343033325a301a3118301606035504030c0f496e66696e656f6e496f544e6f64653059301306072a8648ce3d020106082a8648ce3d030107034200047b3cee887c798f84177b53bfdeffa0fa31fff5e2642c348a3f137ae8fd52e93fe6307af175a0774243f8841d2f06afa86d8e6d35123ae8baa29a173566938645a3583056300e0603551d0f0101ff040403020080300c0603551d130101ff0402300030150603551d20040e300c300a06082a82140044011401301f0603551d23041830168014b383e1ac56940659afd8af57217845748e0c4999300a06082a8648ce3d0403030369003066023100cf2a005a492e515746970168db44490b483b662d823e2d8076e26b01c448890a47a364c4382055741d681ccf75d2c2c3023100ec2862eead1d79abcc190976d4937677b35fc004f992238ef0c62174c4ae9233557e0f9686785fef989affdc8d935450"
    },
    "e0f1":
    {
        "pretty_metadata":
        {
            "change": ['int','0xe0','0xe9','&&','conf','0xe2','0x00'],
        }
    }
}

```

</details>

<br>

## Related resources


Resources | Links
----------|------
Application notes |[AN228571](https://www.cypress.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; software <br> [AN215656](https://www.cypress.com/AN215656) – PSoC&trade; 6 MCU: Dual-CPU system design
Code examples on GitHub| [Using ModusToolbox&trade; software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software)
Device documentation | [PSoC&trade; 6 MCU datasheets](https://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A575&f[2]=field_related_products%3A114026) <br> [PSoC&trade; 6 technical reference manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583) <br> [OPTIGA&trade; Trust M datasheet](https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embed]ded-security-solutions/optiga-trust/optiga-trust-m-sls32aia/#!?fileId=5546d4626c1f3dc3016c853c271a7e4a)
Development kits | Select your kits from the [Evaluation Board Finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page.
Libraries on GitHub  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – PSoC&trade; 6 peripheral driver library (PDL)  <br> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware abstraction layer (HAL) library <br> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Middleware on GitHub  | [optiga-trust-m](https://github.com/Infineon/optiga-trust-m) – OPTIGA™ Trust M library and documents <br> [capsense](https://github.com/Infineon/capsense) – CAPSENSE&trade; library and documents <br> [psoc6-middleware](https://github.com/Infineon/modustoolbox-software#psoc-6-middleware-libraries) – Links to all PSoC&trade; 6 MCU middleware
Tools  | [Eclipse IDE for ModusToolbox&trade; software](https://www.cypress.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC&trade; Wi-Fi and Bluetooth® connectivity devices.


## Other resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC&trade; 6 MCU – KBA223067](https://community.infineon.com/t5/Knowledge-Base-Articles/How-to-Design-with-PSoC-6-MCU-KBA223067/ta-p/248857) in the Infineon community.


## Document history

Document title: *CE233735* – *OPTIGA™ Trust M: Data and certificates management*

| Version | Description of change |
| ------- | --------------------- |
| 1.0.0   | New code example      |


All other trademarks or registered trademarks referenced herein are the property of their respective owners.

-----------------

© Cypress Semiconductor Corporation, 2022. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product.  CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications.  To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document.  Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolBox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries.  For a more complete list of Cypress trademarks, visit cypress.com.  Other names and brands may be claimed as property of their respective owners.

-------------------------------------------------------------------------------
