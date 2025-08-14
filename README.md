# SensePedalBoard

SensePedalBoard is a software–hardware system that integrates guitar effects into a customizable digital pedalboard controlled via a wearable device.  
The project combines **Pure Data (Pd)** for real-time audio processing, a **web-based pedalboard manager** for configuration, and the **Nicla Sense ME** for gesture-based wireless control.

## Overview

Traditional pedalboards require multiple physical devices, cables, and amplifiers. SensePedalBoard reduces this setup to a computer-based environment and a single wireless wearable controller.  
This approach enables guitarists to design, arrange, and control audio effects dynamically with minimal hardware.

## Features

- **Web-Based Pedalboard Management**

  - Add, remove, and rearrange pedals through an intuitive browser interface.
  - Dynamically configure the signal chain.

- **Nicla Sense ME Integration**

  - Wearable controller attached to the shoe.
  - Gesture recognition for switching pedals or modulating effect parameters.

- **Pure Data (Pd) Effects**

  - Effects implemented in Pure Data without the need for a DAW.
  - Includes standard effects (fuzz, overdrive, delay, reverb, wah, tremolo, equalizer).
  - Extendable via the `puredata_scripts` directory.

- **Plug and Play**
  - Compatible with any amplified instrument.
  - Simple setup with an audio interface and a computer.

## Available Pedals

- Clean (unaltered signal)
- Fuzz (heavy distortion)
- Overdrive (tube-like saturation)
- Wah (frequency sweep)
- Delay (echo with adjustable feedback/time)
- Reverb (spatial depth simulation)
- Equalizer (frequency shaping)
- Tremolo (amplitude modulation)

Additional effects can be added or modified.

## Gesture Controls

- **Swipe right** → switch to the next pedal.
- **Tilt foot up/down** → modulate effect parameters in real time.
