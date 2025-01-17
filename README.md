# OPTIGA&trade; Trust M: Data and certificate management

This example demonstrates how to manage data and certificates on an OPTIGA&trade; Trust M secure element using a PSOC&trade; 6 MCU as a bridge. This example has the two following setups:

- **Example mode:** **Figure 1** shows a generic example demonstrated as part of the ModusToolbox&trade; project showing direct API calls such as `optiga_util_read_data`, `optiga_util_write_data`, and `optiga_util_protected_update` to read, write, and securely update the data on the OPTIGA&trade; Trust M device.

- **Provisioning mode:** **Figure 2** shows a special example, where the `optigatrust` Python module can be used to directly access the device and manipulate internal settings. In this case, the host MCU is turned into a bridge to allow Python commands to reach the security solution so the communication occurs in the following sequence:

   **Python Script** <-> **USB** <-> **PSOC&trade; 6 MCU** <-> **I2C** <-> **OPTIGA&trade; Trust M**

**Figure 1. Example mode**

![](images/data_management_setup.png)

<br>
 
**Figure 2. Provisioning mode**

![](images/data_management_setup_python.png)


[View this README on GitHub.](https://github.com/Infineon/mtb-example-optiga-data-management)

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMzM3MzUiLCJTcGVjIE51bWJlciI6IjAwMi0zMzczNSIsIkRvYyBUaXRsZSI6Ik9QVElHQSZ0cmFkZTsgVHJ1c3QgTTogRGF0YSBhbmQgY2VydGlmaWNhdGUgbWFuYWdlbWVudCIsInJpZCI6Inl1c2hldiIsIkRvYyB2ZXJzaW9uIjoiMi4yLjAiLCJEb2MgTGFuZ3VhZ2UiOiJFbmdsaXNoIiwiRG9jIERpdmlzaW9uIjoiTi9BIiwiRG9jIEJVIjoiTi9BIiwiRG9jIEZhbWlseSI6Ik4vQSJ9)


## Requirements

- [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) v3.1 or later (tested with v3.1)
- Board support package (BSP) minimum required version: 4.0.0
- Programming language: C
- Associated parts:
  - All [PSOC&trade; 6 MCU](https://www.infineon.com/PSOC6) parts with SDIO
  - [OPTIGA&trade; Trust M security solution](https://www.infineon.com/optiga-trust-m)


## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm&reg; Embedded Compiler v11.3.1 (`GCC_ARM`) – Default value of `TOOLCHAIN`
- Arm&reg; Compiler v6.16 (`ARM`)
- IAR C/C++ Compiler v9.30.1 (`IAR`)


## Supported kits (make variable 'TARGET')

- [OPTIGA&trade; Trust IoT Security Development Kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-m-iot-kit/) (`CYSBSYSKIT-DEV-01`) – Default value of `TARGET`
- [PSOC&trade; 62S2 Evaluation Kit](https://www.infineon.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2`, `CY8CEVAL-062S2-LAI-43439M2`, `CY8CEVAL-062S2-LAI-4373M2`, `CY8CEVAL-062S2-MUR-43439M2`, `CY8CEVAL-062S2-MUR-4373EM2`)
- [PSOC&trade; 62S2 Pioneer Kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-43012/) in combination with [OPTIGA&trade; Trust Adapter](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-adapter/) (`CY8CKIT-062S2-43012`)


## Hardware setup

This example uses the board's default configuration for all the supported kits except [CY8CKIT-062S2-43012](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062s2-43012/). This kit requires the [OPTIGA&trade; Trust Adapter](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-adapter/) and any OPTIGA&trade; Trust M Shield (e.g. [OPTIGA&trade; Trust M Express Shield](https://www.infineon.com/cms/en/product/evaluation-boards/trust-m-express-shield/)) to be plugged on it. See the kit user guide to ensure that the board is configured correctly.



> **Note:** The PSOC&trade; 6 Bluetooth&reg; LE Pioneer Kit (CY8CKIT-062-BLE) and the PSOC&trade; 6 Wi-Fi Bluetooth&reg; Pioneer Kit (CY8CKIT-062-WIFI-BT) ship with KitProg2 installed. ModusToolbox&trade; requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/Infineon/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error like "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".


## Software setup

See the [ModusToolbox&trade; tools package installation guide](https://www.infineon.com/ModusToolboxInstallguide) for information about installing and configuring the tools package.



- **Example mode:** Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://teratermproject.github.io/index-en.html).

- **Provisioning mode:**

   1. Install an **optigatrust** Python module.

   2. Open the **modus-shell** tool (locally available at *{ModusToolbox&trade; install directory}/tools_2.X)*.

   3. In the terminal emulator, execute this command:

       ```
       pip install optigatrust
       ```

This example requires no additional software or tools.


## Using the code example

### Create the project

The ModusToolbox&trade; tools package provides the Project Creator as both a GUI tool and a command line tool.

<details><summary><b>Use Project Creator GUI</b></summary>

1. Open the Project Creator GUI tool.

   There are several ways to do this, including launching it from the dashboard or from inside the Eclipse IDE. For more details, see the [Project Creator user guide](https://www.infineon.com/ModusToolboxProjectCreator) (locally available at *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/docs/project-creator.pdf*).

2. On the **Choose Board Support Package (BSP)** page, select a kit supported by this code example. See [Supported kits](#supported-kits-make-variable-target).

   > **Note:** To use this code example for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. On the **Select Application** page:

   a. Select the **Applications(s) Root Path** and the **Target IDE**.

   > **Note:** Depending on how you open the Project Creator tool, these fields may be pre-selected for you.

   b.	Select this code example from the list by enabling its check box.

   > **Note:** You can narrow the list of displayed examples by typing in the filter box.

   c. (Optional) Change the suggested **New Application Name** and **New BSP Name**.

   d. Click **Create** to complete the application creation process.

</details>

<details><summary><b>Use Project Creator CLI</b></summary>

The 'project-creator-cli' tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the 'project-creator-cli' tool. On Windows, use the command-line 'modus-shell' program provided in the ModusToolbox&trade; installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing "modus-shell" in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

The following example clones the "[mtb-example-optiga-data-management](https://github.com/Infineon/mtb-example-optiga-power-management)" application with the desired name "OptigaData" configured for the *CYSBSYSKIT-DEV-01* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CY8CEVAL-062S2 --app-id mtb-example-optiga-data-management --user-app-name OptigaData --target-dir "C:/mtb_projects"
   ```

The 'project-creator-cli' tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the <id> field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the <id> field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

> **Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at {ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf).

</details>


### Open the project

After the project has been created, you can open it in your preferred development environment.


<details><summary><b>Eclipse IDE</b></summary>

If you opened the Project Creator tool from the included Eclipse IDE, the project will open in Eclipse automatically.

For more details, see the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_ide_user_guide.pdf*).

</details>


<details><summary><b>Visual Studio (VS) Code</b></summary>

Launch VS Code manually, and then open the generated *{project-name}.code-workspace* file located in the project directory.

For more details, see the [Visual Studio Code for ModusToolbox&trade; user guide](https://www.infineon.com/MTBVSCodeUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_vscode_user_guide.pdf*).

</details>


<details><summary><b>Keil µVision</b></summary>

Double-click the generated *{project-name}.cprj* file to launch the Keil µVision IDE.

For more details, see the [Keil µVision for ModusToolbox&trade; user guide](https://www.infineon.com/MTBuVisionUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_uvision_user_guide.pdf*).

</details>


<details><summary><b>IAR Embedded Workbench</b></summary>

Open IAR Embedded Workbench manually, and create a new project. Then select the generated *{project-name}.ipcf* file located in the project directory.

For more details, see the [IAR Embedded Workbench for ModusToolbox&trade; user guide](https://www.infineon.com/MTBIARUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mt_iar_user_guide.pdf*).

</details>


<details><summary><b>Command line</b></summary>

If you prefer to use the CLI, open the appropriate terminal, and navigate to the project directory. On Windows, use the command-line 'modus-shell' program; on Linux and macOS, you can use any terminal application. From there, you can run various `make` commands.

For more details, see the [ModusToolbox&trade; tools package user guide](https://www.infineon.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation


### Note on OPTIGA&trade; Trust M Express and OPTIGA&trade; Trust M MTR configurations

This example works for all currently available OPTIGA&trade; Trust M configurations. (for a list, see [here](https://github.com/Infineon/optiga-trust-m-overview?tab=readme-ov-file#provisioning-configurations))
However, OPTIGA&trade; Trust M Express and OPTIGA&trade; Trust M MTR come with a pre-provisioned platform-binding secret, which enables the shielded connection.

The shielded connection is used at some points in this example. A specific subsection might fail with the Error Code `0x0107`.

In this case, you have two options:

1. Disable the shielded connection by undefining the `#define OPTIGA_COMMS_SHIELDED_CONNECTION` macro in *config/optiga_lib_config_mtb.h*.

2. Enable the shielded connection and write the same shared secret into the example by modifying the value of the `uint8_t optiga_platform_binding_shared_secret` variable in *COMPONENT_OPTIGA_CYHAL/pal_os_datastore.c*.

Depending on the configuration, use either [Example mode](#example-mode) or [Provisioning mode](#provisioning-mode).


### Example mode

The examples are presented in the form of various API calls.

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

3. Open the *Makefile* in any text editor, find the line `PROVISIONING_MODE` and change the value from '1' to '0', and save the change.

    ```c
    # Define whether the example should be compiled in provisioning mode
    PROVISIONING_MODE=0
    ```

4. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```

5. After programming, connect the serial terminal and see the output there.


### Provisioning mode

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Open the *Makefile* in any text editor, find the line `PROVISIONING_MODE` and change the value from '0' to '1', and save the change.

    ```c
    # Define whether the example should be compiled in provisioning mode
    PROVISIONING_MODE=1
    ```

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3_MiniProg4)**.
   </details>


   <details><summary><b>In other IDEs</b></summary>

   Follow the instructions in your preferred IDE.
   </details>


   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain is specified in the application's Makefile but you can override this value manually:
      ```
      make program TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TOOLCHAIN=GCC_ARM
      ```
   </details>

4. After programming, unplug the board from the USB port and reconnect it. Make sure all serial communication to that port (for example, working Tera Term sessions) are closed.

5. Open the **modus-shell** tool again as described in the [Software setup](#software-setup) section.

6. To try out the connection with the security solution, execute the following command:
   ```
   optigatrust object --id 0xe0e0 --outform PEM
   ```

   > **Note:** if you see a message like the one below, try to repeat the command, as sometimes the standard output or input isn't cleaned and some rest information remains.
   
   ```
   $ optigatrust object --id 0xe0e0
   No Start Sequence found
   Error: 0x202
   Could not find module 'C:\Users\user\ModusToolbox\tools_3.0\python\lib\site-packages\optigatrust\csrc\lib\liboptigatrust-i2c-win-amd64.dll' (or one of its dependencies). Try using the full path with constructor syntax.
   libusb: Failed to connect
   uart: Failed to connect
   i2c: Failed to find library liboptigatrust-i2c-win-amd64.dll in C:\Users\user\ModusToolbox\tools_3.0\python\lib\site-packages\optigatrust\csrc\lib
   ```
   
7. To see the metadata associated with that object, execute the following command:
   ```
   optigatrust object --id 0xe0e0 --meta
   ```

For more examples, see the [*optigatrust* command line interface](#optigatrust-command-line-interface) section.

To use the command-line interface to inject or import custom credentials, follow [this guide](ImportCustomCredentials.md).


## Debugging

You can debug the example to step through the code.


<details><summary><b>In Eclipse IDE</b></summary>

Use the **\<Application Name> Debug (KitProg3_MiniProg4)** configuration in the **Quick Panel**. For details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; user guide](https://www.infineon.com/MTBEclipseIDEUserGuide).

> **Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice – once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.infineon.com/docs/DOC-21143) to learn about this and for the workaround.

</details>


<details><summary><b>In other IDEs</b></summary>

Follow the instructions in your preferred IDE.
</details>


## Design and implementation

### Provisioning mode design

The general idea is to link the **optigatrust** Python module and an OPTIGA&trade; Trust M security solution through I2C. For this, the PSOC&trade; 6 board is flashed with a dedicated firmware, which turns it into a bridge device translating the commands from the Python library to the security solution. In this case, the setup looks similar to the following sequence:

 **optigatrust Python modules** <-> **USB (UART)** <-> **PSOC&trade; 6 MCU** <-> **I2C** <-> **OPTIGA&trade; Trust M**

The firmware implements a very rudimentary finite state machine (the *provisioning_sm.c* file) with the following four states:

- `INIT_STATE` – Initialization state to open the Infineon I2C protocol with the security solution. Afterwards, a transition into the `RX_STATE` is triggered.

- `RX_STATE` – A state where the host (PSOC&trade; 6 MCU) is waiting for a start sequence over the UART interface:
    ```
    uint8_t start_seq[] = {0xbe, 0xef, 0xde, 0xad}`)
    ```
    Afterwards, the APDU from the Python script is forwarded over the Infineon I2C protocol to the OPTIGA&trade; Trust chip, at the same time expecting an answer either from the security solution or an error code. Afterwards, a transition into the `TX_STATE` is triggered.

- `TX_STATE` – Prepares the answer from the OPTIGA&trade; Trust M to send over the UART interface back to the host system. Afterwards, the state is again in `RX_STATE`.
- `ERROR_STATE`


### Example mode design

The main file executes several sub-examples:

- Reads out the data from the OPTIGA&trade; Trust M using `example_optiga_util_read_data()`.
- Writes data to the OPTIGA&trade; Trust M using `example_optiga_util_write_data()`.
- Demonstrates how to work with built-in counters using `example_optiga_util_update_count()`.
- Runs a protected update routine for key objects, data objects, and metadata associated with the objects or keys using `example_optiga_util_protected_update()`. For more information, see [How to run a custom protected data/key/metadata update](#ImportCustomCredentials.md).
- Reads the coprocessor unique ID using `example_read_coprocessor_id()`. It is a demonstration of the `optiga_util_read_data()` function call using a coprocessor unique ID object.


### *optigatrust* command-line interface

 Command            | Note
 ------------------ | --------------------
 `optigatrust object --help` | Displays the usage information for the command `object`
 `optigatrust object --id 0xe0e0` | Reads the content of the 0xe0e0 object (to get the [objects map](https://infineon.github.io/python-optiga-trust/metadata.html)).
 `optigatrust object --id 0xe0e0 --out certificate.pem --outform PEM` | Reads the content of the object 0xe0e0, tries to convert it into a PEM-formatted X.509 certificate, and stores the result into a *certificate.pem* text file.
 `optigatrust object --id 0xe0e0 --meta` | Reads the metadata of the 0xe0e0 object. 
 `optigatrust object --in data_metadata.json` | Writes to the OPTIGA&trade; Trust data and metadata from JSON format.
 `optigatrust object --export-otc` | Reads all the objects from the OPTIGA&trade; Trust and stores them into a format compatible with OPTIGA&trade; Trust configurator.
 `optigatrust object --export-json` | Reads all objects from the OPTIGA&trade; Trust and stores them into the JSON format*.
 `optigatrust create-keys --help` | Displays the usage information for the command `create-keys`.
 `optigatrust create-keys --id 0xe0f1` | Generates an ECC NISTP256 public/private key-pair and stores the private component in the given key [object ID](https://infineon.github.io/python-optiga-trust/metadata.html); the private key usage is set to **signature** by default.
 `optigatrust create-keys --id 0xe0f1 --pubout public.pkey --privout private.key` | Generates an ECC NISTP256 public/private key-pair and stores them in the corresponding files; the output is PEM-formatted.
 `optigatrust create-keys --id 0xe0f1 --curve secp384r1` | Generates an ECC NISTP384 public/private key-pair.
 `optigatrust create-keys --id 0xe0f1 --key-usage key_agreement --key-usage signature` | Generates an ECC NISTP256 public/private key-pair and sets the private key usage to be **KeyAgreement** and **Signature**.
 `optigatrust create-keys --id 0xe0fc --rsa --key_size 2048` | Generates an RSA public/private key-pair with a key size of 2048 bits; private key usage is set to **Signature** by default.

For other commands, see the [`python-optiga-trust`](https://github.com/Infineon/python-optiga-trust) repository.

To use the command-line interface to inject or import custom credentials, follow [this guide](ImportCustomCredentials.md).

<br>

<details><summary><b>The format of the <i>data_metadata.json</i> can look like the following:</b></summary>


```json
{
    "e0e1":
    {
        "pretty_metadata":
        {
            "change": ['int','0xe0','0xe9','&&','conf','0xe2','0x00'],
        }
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

Resources  | Links
-----------|----------------------------------
Application notes  | [AN228571](https://www.infineon.com/AN228571) – Getting started with PSOC&trade; 6 MCU on ModusToolbox&trade; <br>  [AN215656](https://www.infineon.com/AN215656) – PSOC&trade; 6 MCU: Dual-CPU system design
Code examples  | [Using ModusToolbox&trade;](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) on GitHub
Device documentation | [PSOC&trade; 6 MCU datasheets](https://documentation.infineon.com/html/psoc6/bnm1651211483724.html) <br> [PSOC&trade; 6 technical reference manuals](https://documentation.infineon.com/html/psoc6/zrs1651212645947.html) <br> [OPTIGA&trade; Trust M datasheets](https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-m-sls32aia/#!?fileId=5546d4626c1f3dc3016c853c271a7e4a)
Development kits | Select your kits from the [Evaluation Board Finder](https://www.infineon.com/cms/en/design-support/finder-selection-tools/product-finder/evaluation-board) page.
Libraries on GitHub  | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – PSOC&trade; 6 Peripheral Driver Library (PDL)  <br> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware Abstraction Layer (HAL) library <br> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port
Middleware on GitHub  | [optiga-trust-m](https://github.com/Infineon/optiga-trust-m) – OPTIGA&trade; Trust M library and documents <br> [capsense](https://github.com/Infineon/capsense) – CAPSENSE&trade; library and documents <br> [psoc6-middleware](https://github.com/Infineon/modustoolbox-software#psoc-6-middleware-libraries) – Links to all PSOC&trade; 6 MCU middleware
Tools  | [ModusToolbox&trade;](https://www.infineon.com/modustoolbox) – ModusToolbox&trade; software is a collection of easy-to-use libraries and tools enabling rapid development with Infineon MCUs for applications ranging from wireless and cloud-connected systems, edge AI/ML, embedded sense and control, to wired USB connectivity using PSOC&trade; Industrial/IoT MCUs, AIROC&trade; Wi-Fi and Bluetooth&reg; connectivity devices, XMC&trade; Industrial MCUs, and EZ-USB&trade;/EZ-PD&trade; wired connectivity controllers. ModusToolbox&trade; incorporates a comprehensive set of BSPs, HAL, libraries, configuration tools, and provides support for industry-standard IDEs to fast-track your embedded application development.


## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com) to help you select the right device, and quickly and effectively integrate it into your design.

For PSOC&trade; 6 MCU devices, see [How to design with PSOC&trade; 6 MCU - KBA223067](https://community.infineon.com/t5/Knowledge-Base-Articles/How-to-Design-with-PSOC-6-MCU-KBA223067/ta-p/248857) in the Infineon Developer Community.


## Document history

Document title: *CE233735* – *OPTIGA&trade; Trust M: Data and certificate management*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example
 2.0.0   | Updated to use the latest optiga-trust-m middleware release
 2.1.0   | Added support for new kits
 2.2.0   | Added support for the OPTIGA&trade; Trust M Express and OPTIGA&trade; Trust M MTR chips
<br>



All referenced product or service names and trademarks are the property of their respective owners.

The Bluetooth&reg; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.


---------------------------------------------------------

© Cypress Semiconductor Corporation, 2023-2024. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, ModusToolbox, PSOC, CAPSENSE, EZ-USB, F-RAM, and TRAVEO are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.
