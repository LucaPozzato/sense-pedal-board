# SenseMePedalBoard 🎸🎛️

The **Guitar Arduino Pedals** project combines the flexibility of software-based guitar effects with the intuitive control of hardware. Using an audio interface, Pure Data (Pd), and the **Nicla Sense ME**, this project enables you to build a fully interactive, customizable pedalboard without the need for a DAW. 

This project offers real-time control over pedal parameters and signal flow, letting you focus on playing music while seamlessly managing effects using your feet.

---

## Features 🚀

- **Web-based Pedalboard Management**:
  - Select and rearrange guitar pedals through an intuitive web interface.
  - Dynamically update the signal chain for effects like distortion, delay, reverb, and more.

- **Nicla Sense ME Integration**:
  - Use the **Nicla Sense ME** as a wireless controller to switch between pedals or adjust parameters like gain, feedback, and delay time with foot movements.
  - No need to physically interact with amplifiers or the interface during performance.

- **Pure Data (Pd) Effects Implementation**:
  - Guitar effects are implemented in **Pure Data**, a powerful open-source DSP environment.
  - Avoid the need for a DAW—run everything directly from your computer.

- **Plug and Play Setup**:
  - Connect your guitar to your computer via an audio interface (e.g., Scarlett).
  - Start playing with minimal hardware or software dependencies.

---


## Pedals available 🎛️

The following effects are implemented in **Pure Data**:
1. **Distortion**: Simulates overdrive and fuzz effects.
2. **Delay**: Adds echo effects with adjustable feedback and time.
3. **Reverb**: Simulates different room sizes and depths.
4. **Equalizer**: Allows frequency shaping of the input signal.
5. **Tremolo**: Modulates volume for a dynamic sound.
6. **Chorus**: Creates a thick, lush sound by detuning the signal.

These effects can be easily extended or customized in the `pure_data/` folder.

---

## Gesture available 👟
Once Nicla Sense Me is attached to your foot, these gesture are recognized:
1. **Swipe right**: Allows to move to the next pedal in the selected chain.
2. **Up and down**: Allows to change pedals parameters such as: 

---

## Installation and Setup ⚙️
1. Clone the Repository:
   
   ```bash
   git clone https://github.com/niccolopuccia/GuitarArduinoPedals.git
   cd GuitarArduinoPedals
   ```
3. Set Up the Nicla Sense ME
4. Upload the provided Arduino code (NICLA_MAIN folder) to the Nicla Sense ME using the Arduino IDE.
5. Link Nicla Sense ME to Arduino MKR 1010 shield to enable WI-Fi communication.
6. Upload the provided Arduino code (MKR_MAIN folder) to the shield using the Arduino IDE.
7. Attach the Nicla Sense ME to your foot or another convenient location.
8. Launch the Web Interface:
  - Open the web_interface/index.html file in your browser.
  - Use the interface to select and arrange pedals.
8. Run Pure Data:
  - Open the pure_data/pedalboard.pd file in Pure Data.
  - Start the patch and begin playing.



