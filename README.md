# SenseMePedalBoard 🎸🎛️

The **Guitar Arduino Pedals** project combines the flexibility of software-based guitar effects with the intuitive control of hardware. Using an audio interface, Pure Data (Pd), and the **Nicla Sense ME**, this project enables you to build a fully interactive, customizable pedalboard without the need for a DAW.

This project offers real-time control over pedal parameters and signal flow, letting you focus on playing music while seamlessly managing effects switching between them using foot gestures.

---

## Features 

- **Web-based Pedalboard Management**:

  - Select and rearrange guitar pedals through an intuitive web interface.
  - Dynamically update the signal chain for effects like distortion, delay, reverb, and more.

- **Nicla Sense ME Integration**:

  - Use the **Nicla Sense ME** as a wireless controller to switch between pedals or adjust parameters like gain, feedback, and delay time with foot movements.
  - No need to physically interact with amplifiers or the interface during performance.

- **Pure Data (Pd) Effects Implementation**:

  - Guitar effects are implemented in **Pure Data**, a powerful open-source DSP environment.
  - Avoid the need of an amplifier. Run everything directly from your computer.
  - Avoid the need for a DAW (Digital Audio Workstation) and low-level customize your own effects. 

- **Plug and Play Setup**:
  - Connect your guitar to your computer via an audio interface.
  - Start playing with minimal hardware or software dependencies.

---

## Pedals Available

The following effects are implemented in **Pure Data**:

0. **Clean**: Provides an unaltered, clear signal.
1. **Fuzz**: Creates a heavily distorted and aggressive sound.
2. **Overdrive**: Simulates warm tube amplifier overdrive.
3. **Wah**: Produces a sweeping frequency effect for expressive tones.
4. **Delay**: Adds echo effects with adjustable feedback and time.
5. **Reverb**: Simulates different room sizes and depths for spatial effects.
6. **Equalizer**: Allows frequency shaping of the input signal.
7. **Tremolo**: Modulates volume for a dynamic, pulsating effect.

These effects can be easily extended or customized in the `puredata_scripts` folder.

---

## Gestures Available

Once the Nicla Sense Me is attached to your foot and the pedal order is configured by the user via the web application, the following gestures are recognized:

1. **Swipe right**: Moves to the next pedal on the pedalboard.
2. **Swipe up and down**: Modulates the selected pedal's parameters, adjusting the intensity of the effect as if you were turning a knob on an amplifier.

---

## Installation and Setup

1. Clone the Repository:

   ```bash
   git clone https://github.com/niccolopuccia/GuitarArduinoPedals.git
   cd GuitarArduinoPedals
   ```

2. Set Up the Nicla Sense ME
3. Upload the provided Arduino code (NICLA_MAIN folder) to the Nicla Sense ME using the Arduino IDE.
4. Link Nicla Sense ME to Arduino MKR 1010 shield to enable WI-Fi communication.
5. Upload the provided Arduino code (MKR_MAIN folder) to the shield using the Arduino IDE.
6. Attach the Nicla Sense ME to your foot.
7. Run Pure Data:

- Open the pure_data/pedalboard.pd file in Pure Data and run it.

8. Launch the Web Interface:

- Open the web_interface/index.html file in your browser.
- Use the interface to select and arrange pedals.
  
9. Begin playing
