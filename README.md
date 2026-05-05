# MorseCodeMachine: Advanced Hardware Encoder & Decoder

An engineering-focused communication tool designed for high-reliability environments where digital infrastructure is unavailable. This project features high-speed dual-LED signaling, hardware-level performance tracking, and an experimental voice-automation interface.  

---

## 🚀 Core Features (Stable)

*   **The "Spacepin" Indicator:** A dedicated **Green LED** pulses after every character completion, solving the common timing ambiguity in manual Morse decryption.  
*   **Fast Mode (Dual-LED Signaling):** A high-speed interface using a **Red LED** for dots and a **Blue LED** for dashes, allowing for near-instant signal recognition without waiting for long pulse durations.  
*   **Real-time Performance Analytics:** Automatically calculates and displays the "Time Elapsed" for every message, enabling operators to track their Words Per Minute (WPM) and signal efficiency.  
*   **Dynamic Speed Management:** Supports custom timing parameters via Serial input (e.g., `DOT:100` for high-speed expert use or `DASH:800` for slow-speed distress signals).  
*   **Dual-Operation Mode:** A physical hardware switch toggles the device between an automated **Encoder** and a manual **Decoder** with dedicated input buttons.  

---

## 🧪 Experimental Features (In Development)

### **Voice-to-Morse Bridge**
A Python-based module that utilizes the Google Speech Recognition API to translate spoken words into text and forward them to the Arduino via the Serial port.  

> **Note:** This feature is currently in its alpha stage. Accuracy is highly dependent on ambient noise levels and microphone quality. It serves as a proof-of-concept for hands-free Morse transmission.

---

## 📐 System Architecture & Hardware

The system is built on the Arduino Uno platform, utilizing I2C for the display interface to minimize pin usage while maintaining a rich signaling array.  

[Morse Code Schematic](Schematic/Morse-Code_Schematic.pdf)
*Circuit Diagram*  

---

## 📦 Setup & Usage

### 1. Arduino Installation
*   Load `Morse-Arduino-Code/Morse-Arduino-Code.ino` into the Arduino IDE.  
*   Install the `LiquidCrystal_I2C` library.  
*   Connect your hardware as per the provided [Schematic](Schematic/Morse-Code-Schematic.pdf).  

### 2. Running the Speech Interface
*   Navigate to the `scripts/` folder.
*   Install dependencies: `pip install speechrecognition pyserial`.
*   Run `python morseup.py`.

---

## 🗺️ Project Roadmap & Future Scope

*   **[Development]** Refining the Python Speech-to-Text algorithm with noise-cancellation filters to improve accuracy.
*   **[Hardware]** Integrating a 433MHz RF module for true wireless long-range communication between two units.
*   **[Accessibility]** Exploring applications for users with visual or hearing impairments based on existing research in assistive technology.  

---

Since you mentioned the speech recognition isn't perfect, you might want to add a `try-except` block around the specific line `text = recognizer.recognize_google(audio)` in your Python script to catch `sr.WaitTimeoutError`. This prevents the script from crashing if it doesn't hear anything.

Do you have any other projects on GitHub already, or is this the first one you're adding to your portfolio?
