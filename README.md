# Spectre Audio Mixer

A USB audio mixer PCB with per-app volume control. Link any three applications to physical sliders and control their volume on the fly without tabbing out.

## Features

- Link any three apps to individual volume sliders
- Seamless app-to-slider mapping via desktop controller
- LED level indicators (1-10) showing current volume per slider
- OLED display showing active app mappings
- Reactive LED indicators synced to system volume
- Works in games without alt-tabbing

## Hardware Design

KiCad files available in `hardware/kicad/`:
- Schematic: `Spectre.kicad_sch`
- PCB Layout: `Spectre.kicad_pcb`

![Spectre PCB final rendering](Hardware\Spectre.png)

**Microcontroller:**
- ESP32-C3-DevKitM-1

**Sensors & Controls:**
- 3x Potentiometer sliders (10kΩ)
- GPIO pins 3, 4, 5 for analog input

**Output:**
- 30x SK6812MINI-E addressable RGB LEDs (10 per slider)
- SSD1306 OLED display (128x64) via I2C
- 5V and 3.3V power rails with 220µF decoupling capacitor
- 360Ω series resistors on LED data lines

**Power:**
- USB-C (5V from ESP32)
- Separate 5V rail for LEDs, 3.3V for logic


## Software

**Desktop Controller (Python):**
- Tkinter GUI for app selection
- Serial communication at 115200 baud
- Sends config format: `S1:Chrome|S2:Spotify|S3:Discord`

**Firmware (ESP32-C3):**
- Arduino framework with PlatformIO
- Adafruit NeoPixel library for LED control
- Adafruit SSD1306 library for OLED display
- Serial parsing for configuration updates

## Setup

### Hardware Assembly
1. Solder SK6812MINI-E LEDs to GPIO 0, 1, 2 (10 each)
2. Connect potentiometers to GPIO 3, 4, 5
3. Wire SSD1306 OLED via I2C (SDA, SCL)
4. Add 220µF capacitor across 5V/GND rails
5. Add 360Ω resistors on LED data lines

### Firmware
1. Install PlatformIO
2. Build and upload to ESP32-C3
3. Connect via USB (COM3 on Windows)

### Desktop App
```bash
pip install pyserial psutil
python spectre_controller.py
```

## Usage

1. Run desktop controller
2. Select apps from dropdown menus (Slider 1, 2, 3)
3. Click "Save Configuration"
4. OLED displays active app mappings
5. Move sliders to control individual app volumes
6. LED bars show volume level (1-10 LEDs lit)

## Status

- Firmware: Complete (Steps 1-4 done)
- Desktop Controller: BETA (app selection refinement in progress)
- Hardware: PCB designed, awaiting fabrication
