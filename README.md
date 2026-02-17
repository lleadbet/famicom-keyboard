# Famicom Keyboard USB Adapter

QMK firmware for the Family BASIC keyboard using an Adafruit KB2040. This was a personal project of mine to prove it was possible, as well as making it feasible to repeat on any other Family BASIC keyboards in case others would like to do so as well.

## Credits

A huge thanks to CircuitRewind for creating the [original project](https://github.com/circuitrewind/famicom-keyboard) and doing the heavy lifting of reverse-engineering and implementing the Family BASIC keyboard's matrix scanning protocol. Their initial work with the Arduino implementation provided the foundation for understanding the vintage hardware's timing requirements and the custom scanning logic that makes this adapter possible. This QMK port builds upon that excellent groundwork to bring modern keyboard features.

## Table of Contents

- [Famicom Keyboard USB Adapter](#famicom-keyboard-usb-adapter)
  - [Credits](#credits)
  - [Table of Contents](#table-of-contents)
  - [Before You Get Started](#before-you-get-started)
  - [Bill of Materials (BOM)](#bill-of-materials-bom)
  - [Wiring Diagram](#wiring-diagram)
    - [New Build Wiring (JST PH Connector)](#new-build-wiring-jst-ph-connector)
    - [Original DA15 Cable Reference](#original-da15-cable-reference)
  - [Building the QMK Firmware](#building-the-qmk-firmware)
    - [Prerequisites](#prerequisites)
    - [Build Steps](#build-steps)
  - [Flashing the Firmware](#flashing-the-firmware)
    - [Initial Flash (First Time)](#initial-flash-first-time)
    - [Reflashing After Initial Setup](#reflashing-after-initial-setup)
    - [Setting Up VIA (Key Remapping)](#setting-up-via-key-remapping)
  - [How It Works](#how-it-works)
    - [Scanning Process](#scanning-process)
  - [Troubleshooting \& Tuning](#troubleshooting--tuning)
    - [Timing Parameters](#timing-parameters)
      - [1. Debounce Threshold (Line 33)](#1-debounce-threshold-line-33)
      - [2. Hardware Delay (Line 46)](#2-hardware-delay-line-46)
      - [Making Changes](#making-changes)
  - [Notes \& Resources](#notes--resources)
    - [Hardware Details](#hardware-details)
    - [Famicom Keyboard Port Pinout](#famicom-keyboard-port-pinout)
    - [Resources](#resources)

## Before You Get Started

Before you consider doing this yourself, I want to let you know of a few things: 

1. The keyboard is over 40 years old at this point and is entirely plastic. When unscrewing the case, you will need to be careful as to not break the plastic posts
2. The keyboard is a traditional membrane-style keyboard, so don't expect superb tactility or key feel
3. The keycaps are not MX-compatible, so you can't take them and swap them to another keyboard
4. The keycaps are also a nonstandard size, so even if you were to replace the PCB entirely, it's unlikely you could fit the exact layout in the plastic as a result
5. N-Key rollover isn't possible due to a lack of diodes, so you can maybe get 2 or 3 keys at once before the board begins to act wonky. As a result, the usefulness can be a little limited if you want to become a pro gamer

**I would strongly consider the keyboard as a conversation starter, not a daily driver.**

## Bill of Materials (BOM)

I used the following on my own USB Family BASIC Keyboard:

| Component | Description | Link/Notes |
|-----------|-------------|------------|
| Adafruit KB2040 | RP2040 microcontroller in Pro Micro form factor | [Adafruit](https://www.adafruit.com/product/5302) 
| 11-pin single-row JST PH connector w/ pre-attached cables | Used to replace the DA15 connector | [Amazon link (non-affiliate)](https://amazon.com/dp/B0DMZM59QS) |
| Extra wire | 1 wire for going from GND to the metal chassis | 22-26 AWG solid or stranded |
| Heat Shrink/Tape | For insulating unused DA15 Pins 1 and 13, or for bundling the connector wires together for easier routing | Electrical tape or heat shrink tubing |
| 6ft silicone USB-A to USB-C cable | For a pre-attached USB cable without modifying the case. | |


## Wiring Diagram

This project uses an Adafruit KB2040.

### New Build Wiring (JST PH Connector)

Wiring order matches physical connector pin order (left to right):

| DA15 Pin | KB2040 Socket | GPIO  | Function        | Wire Color (New) |
|----------|---------------|-------|-----------------|------------------|
| Chassis  | GND           | GND   | Chassis Ground  | Extra 26 AWG wire (solder to GND pad) |
| 15       | 3.3V          | 3V3   | Power (VCC)     | Red              |
| 13       | —             | —     | Not Connected   | White (tape off) |
| 12       | D2            | GP2   | OUT0 (Reset)    | Yellow           |
| 11       | D3            | GP3   | OUT1 (Col Sel)  | Orange           |
| 10       | D4            | GP4   | OUT2 (Enable)   | Green            |
| 07       | A0            | GP26  | Data Pin 1      | Blue             |
| 06       | A2            | GP28  | Data Pin 3      | Purple           |
| 05       | A1            | GP27  | Data Pin 2      | Grey             |
| 04       | A3            | GP29  | Data Pin 4      | Brown            |
| 01       | GND           | GND   | Not Connected   | Black            |

**Note: If using the suggested JST PH connector, the connector has black wires on both ends. The first black wire (pin 1) needs to be taped off, which would align with the old connector's light green wire. The second black wire (chassis ground) is soldered directly to the KB2040's GND pad. DA15 Pin 13 (white wire) also needs to be taped off.**

### Original DA15 Cable Reference

For reference if you have the original DA15 connector - this shows the original cable's wire colors and their functions (physical order, left to right):

| Original Color | DA15 Pin | Function |
|----------------|----------|----------|
| Lt. Green      | Chassis  | Chassis Ground (bypassed DA15 connector) |
| Black          | 15       | Power (VCC) |
| White          | 13       | Extra Data (unused) |
| Gray           | 12       | OUT0 (Reset) |
| Purple         | 11       | OUT1 (Col Sel) |
| Blue           | 10       | OUT2 (Enable) |
| Green          | 07       | Data 1 |
| Yellow         | 06       | Data 3 |
| Orange         | 05       | Data 2 |
| Red            | 04       | Data 4 |
| Brown          | 01       | Logic Ground |

## Building the QMK Firmware

### Prerequisites

A configured QMK environment, following QMK's own Getting Started Guide: https://docs.qmk.fm/newbs_getting_started. 

### Build Steps

1. Copy the `qmk/keyboards/famikey` directory from this repository into your QMK installation:
```bash
cp -r /path/to/this/repo/qmk/keyboards/famikey keyboards/
```

2. Set up QMK (first time only):
```bash
qmk setup
```

3. Build the firmware with VIA support (enables graphical key remapping):
```bash
qmk compile -kb famikey -km via
```

The compiled `.uf2` file will be in the `qmk_firmware/.build/` directory.

**Note:** This build uses VIA for real-time key remapping and layer configuration without recompiling firmware. You'll need to sideload the keyboard definition (see below).

## Flashing the Firmware

### Initial Flash (First Time)

1. Hold the BOOT button on your KB2040
2. Plug in the USB cable while holding BOOT 
3. The board will appear as a USB mass storage device
4. Copy the compiled (or the one in the releases tab) `.uf2` file to the device
5. The board will automatically reboot with the new firmware

Your keyboard is now ready to use!

### Reflashing After Initial Setup

Once QMK firmware is installed, you can enter bootloader mode using the QMK Boot key combination:

1. Unplug the keyboard
2. Hold down KANA + F8
3. The board enters bootloader mode
4. Release the keys and copy the new `.uf2` file
5. The keyboard automatically reboots with updated firmware

**Note: The KANA + F8 key combo is based on the key codes, meaning that if you remap the KANA key, it _will not_ work for starting the keyboard into the QMK Boot mode any longer. You will need to map the KANA key again to another key to enable the boot mode key combo.**

### Setting Up VIA (Key Remapping)

To use VIA for graphical key remapping:

1. Download and install [VIA](https://caniusevia.com/) (or use the web version at [usevia.app](https://usevia.app/))
2. Plug in your keyboard with the VIA-enabled firmware
3. In VIA, go to the **Settings** tab and enable **"Show Design tab"**
4. Navigate to the **Design** tab
5. Click **"Load Draft Definition"**
6. Select the `via.json` file from `qmk/keymaps/via/via.json`
7. VIA will now recognize your keyboard and you can remap keys in real-time

## How It Works

The Family BASIC keyboard uses a custom matrix scanning protocol (originally created by `circuitrewind`, as noted above) that differs from modern keyboards. 

### Scanning Process

1. The microcontroller resets the keyboard's internal column counter
2. For each of the 18 columns:
   - Read 4 data pins to get the state of all rows in that column
   - Toggle the Column Select pin to advance to the next column
3. After all columns are scanned, reset the counter and repeat

This happens continuously to detect key presses and releases.

## Troubleshooting & Tuning

### Timing Parameters

If you experience issues like double-typing, missed keypresses, or erratic behavior, you can adjust two timing parameters in the firmware. Both parameters are in the file:

**`qmk/matrix.c`**

#### 1. Debounce Threshold (Line 33)

```c
#define DEBOUNCE_THRESHOLD 8
```

**Purpose:** Prevents membrane contact bounce from registering multiple keypresses. The value represents the number of consecutive matrix scans a key must maintain the same state before registering as pressed or released.

**Current value:** `8` scans

**Recommended range:** `4` to `16` scans

**Symptoms requiring adjustment:**
- **Too low (< 4)**: Double-typing - pressing a key once registers multiple characters
- **Too high (> 16)**: Missed keypresses or sluggish response

**How to adjust:** If you're experiencing double-typing, increase the value. If keys feel unresponsive or miss presses, decrease the value. Change in increments of 2-4.

#### 2. Hardware Delay (Line 46)

```c
wait_us(50);  // 50 microsecond delay for vintage hardware
```

**Purpose:** Delay after writing to control pins to allow the vintage CMOS chips time to settle and respond reliably at 3.3V operation.

**Current value:** `50` microseconds (μs)

**Recommended range:** `30` to `100` microseconds

**Symptoms requiring adjustment:**
- **Too low (< 30)**: Erratic behavior - keys not registering, random keypresses, inconsistent scanning
- **Too high (> 100)**: Slower scan rate, potentially sluggish or delayed keypress detection

**How to adjust:** The current value of 50μs was found through testing to be the reliable minimum for consistent operation. Only decrease if you need faster scanning and are willing to risk instability. Increase if experiencing erratic reads.

#### Making Changes

After editing these values in `matrix.c`:

1. Recompile the firmware:
   ```bash
   qmk compile -kb famikey -km via
   ```

2. Reflash to your keyboard using either:
   - Initial flash method (BOOT button + copy `.uf2`)
   - QMK Boot key combination (KANA + F8)

3. Test the keyboard and observe behavior. You may need to iterate to find the optimal values for your specific keyboard's condition.

## Notes & Resources

### Hardware Details

The Famicom Keyboard was designed to work with the Famicom's 5V power and signalling. The keyboard is comprised of just 3 ICs (Integrated Circuits / Computer Chips): a 4017 *(Counter/Divider)*, a 4019 *(Quad AND/OR Select Gate)*, and a 4069 *(Hex Inverter)*. Each of these chips are designed to work at 5V, but have a minimum supply of 3V. The KB2040 runs at 3.3V, which is within the acceptable range for these vintage CMOS chips. Thanks to this, we can run the entire circuit at 3.3V without the need for any logic level converters.

### Famicom Keyboard Port Pinout

Source: https://www.nesdev.org/wiki/Expansion_port
```
      Female DA-15 on peripheral
                 /\
                |   \
         Gnd -- | 01  \
                |   09 | -> /OE for joypad 2 ($4017 read strobe)
       SOUND <- | 02   |
                |   10 | -> OUT2 ($4016 write data, bit 2)
        /IRQ ?? | 03   |
                |   11 | -> OUT1 ($4016 write data, bit 1)
joypad 2 /D4 -> | 04   |
                |   12 | -> OUT0 ($4016 write data, bit 0, strobe on pads)
joypad 2 /D3 -> | 05   |
                |   13 | <- joypad 1 /D1
joypad 2 /D2 -> | 06   |
                |   14 | -> /OE for joypad 1 ($4016 read strobe)
joypad 2 /D1 -> | 07   |
                |   15 | -- +5V
joypad 2 /D0 ?? | 08  /
                |   /
                 \/
```

### Resources
* [Keyboard Schematic](https://ia800502.us.archive.org/5/items/fc-factory-cpu-schematic-scan/FC%20Family%20Basic%20Keyboard%20and%20Sound%20Circuit%20Schematic.png)
* [Keyboard Matrix](https://ia800502.us.archive.org/5/items/fc-factory-cpu-schematic-scan/FC%20Keyboard%20Key%20Schematic.png)
* [4017 Datasheet](https://www.ti.com/lit/ds/symlink/cd4017b.pdf) - Counter/Divider
* [4019 Datasheet](https://www.ti.com/lit/ds/symlink/cd4019b.pdf) - Quad AND/OR Select Gate
* [4069 Datasheet](https://www.ti.com/lit/ds/symlink/cd4069ub.pdf) - Hex Inverter
* [QMK Documentation](https://docs.qmk.fm/)
* [QMK Firmware Repository](https://github.com/qmk/qmk_firmware)

