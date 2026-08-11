# STM32-NUCLEO-F446RE-baremetal-basics

This educational repository contains examples of configuration and programs for the STM32-chip in bare-metal mode.

## Table of Contents

- [Quick Start & Building](#quick-start--building)
  - [1. [HW] Connection STM32-Board](#1-hw-connection-stm32-board)
  - [2. [SW] Tools Installation](#2-sw-tools-installation)
  - [3. [SW] ST-LINK Configuration](#3-sw-st-link-configuration)
  - [4. [SW] VS Code Environment](#4-sw-vscode-environment)
    - [4.1 Required Extensions](#41-required-extensions)
    - [4.2 Clone the Repository](#42-clone-the-repository)
    - [4.3 Build & Debug via VS Code](#43-build--debug-via-vscode)
- [1st-minimal](#1st-minimal)
- [2nd-led-blink](#2nd-led-blink)
- [Directory Structure](#directory-structure)
- [References](#references)
- [License](#license)


## Quick Start & Building

### 1. [HW] Connection STM32-Board

- Board: STM32 NUCLEO-F446RE.
- Connection: Connect the board to your computer using a USB Mini-B cable connected to the onboard ST-LINK port (CN1, top of the board).
- Power Verification: Confirm that the green power LED (PWR / LD3) is turned on solid.

---

### 2. [SW] Tools Installation

To compile bare-metal C code and communicate with the board, install the ARM GCC cross-compiler, make, and openocd.

**macOS (via Homebrew)**

```bash
    brew install --cask gcc-arm-embedded
    brew install make openocd stlink
```

**Linux (Ubuntu/Debian)**

```bash
    sudo apt update
    sudo apt install gcc-arm-none-eabi build-essential openocd stlink-tools
    sudo ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
```

**Tools Verification**

```bash
    arm-none-eabi-gcc --version
    make --version
    openocd --version
    st-info --version
```

---

### 3. [SW] ST-LINK Configuration
Before debugging, verify that your machine detects the onboard ST-LINK programmer and can communicate with the STM32 chip.

```bash
    st-info --probe
```

> Expected output: Information regarding the ST-LINK version, SRAM size (131072 bytes), and Flash size (524288 bytes).

**Verify OpenOCD Connection**
Test OpenOCD communication using the target config files:

```bash
    openocd -f interface/stlink-v2-1.cfg -f target/stm32f4x.cfg
```

**Debugging Configuration Breakdown** (launch.json)
In VS Code (`.vscode/launch.json`), the debugger utilizes the following settings:

```json
    "device": "STM32F446RE",
    "configFiles": [
        "interface/stlink-v2-1.cfg",
        "target/stm32f4x.cfg"
    ]
```

- `"device": "STM32F446RE"`: Informs the `Cortex-Debug` Visual Studio Code extension of the specific MCU model for register mapping and SVD file selection.
- `"interface/stlink-v2-1.cfg"`: Selects the hardware interface config for the onboard **ST-LINK V2.1** programmer present on Nucleo boards.
- `"target/stm32f4x.cfg"`: Specifies the **STM32F4** target architecture config file for OpenOCD to handle flash algorithms and hardware reset routines.

> Expected output: OpenOCD should recognize the MCU and report [stm32f4x.cpu] Hardware has `6 breakpoints`, `4 watchpoints`. Press `Ctrl + C` to exit.

---

### 4. [SW] VSCode Environment

#### 4.1 Required Extensions

    - Cortex-Debug (`marus25.cortex-debug`) — ARM Cortex-M hardware debugging support.    
    - C/C++ (`ms-vscode.cpptools`) — IntelliSense and syntax highlighting.

#### 4.2 Clone the Repository

```bash
    git clone https://github.com/meugenom/stm32-nucleo-f446re-baremetal-basics.git
    cd stm32-nucleo-f446re-baremetal-basics
```

#### 4.3 Build & Debug via VSCode

- Open the workspace root folder in VS Code.
- Open the debug panel and select the desired configuration (e.g., `Debug: 1st-minimal`).
- VS Code will automatically run the preLaunchTask (executing make), compile the binary into build/firmware.elf, flash the MCU, and halt execution at main().
- Debugging can be performed using breakpoints, watch expressions, and the debug console.

--- 

## 1st-minimal

Code demonstrates basic step counting (1, 2, 3, ...) and cumulative sum for GDB/Watch Expressions during debugging.
Run with `Debug: 1st-minimal` by selecting it from the debug configuration dropdown.

## 2nd-led-blink

Code demonstrates GPIO configuration and toggling of the onboard LED (LD2) on the Nucleo board.
Run with `Debug: 2nd-led-blink` by selecting it from the debug configuration dropdown.

## Directory Structure

```txt
stm32f446-baremetal-basics/
├── README.md                     <-- Main README file
├── .gitignore                    <-- Garbage that should NOT be committed to Git
├── assets/                       <-- Assets for the README.md file
│
├── common/                       <-- Common files for all examples (optional)
│   └── f446.ld                   <-- Common Linker Script
│
└── examples/                     <-- Folder with all examples
    ├── 01-minimal/
    │   ├── Makefile              <-- Makefile for the example>
    │   └── src/                  
    │       └── main.c            <-- Source code for the example>
    │
    ├── 02-led-blink/
    │   ├── Makefile
    │   └── src/
    │       └── main.c
    │
    └── 03-example/
        ├── Makefile
        └── src/
            └── main.c
```

# References


# License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details