import speech_recognition as sr
import serial
import time

# Connect to Arduino
arduino = serial.Serial('COM8', 9600)  # Replace 'COM8' with your Arduino's COM port
time.sleep(2)  # Wait for the connection to establish

# Initialize the speech recognizer
recognizer = sr.Recognizer()

def recognize_speech():
    """Capture speech from the microphone and return it as text."""
    with sr.Microphone() as source:
        print("Speak a command:")
        try:
            audio = recognizer.listen(source, timeout=5)
            text = recognizer.recognize_google(audio)
            print(f"You said: {text}")
            return text
        except sr.UnknownValueError:
            print("Sorry, I could not understand the audio.")
            return None
        except sr.RequestError as e:
            print(f"Could not request results; {e}")
            return None

def wait_for_acknowledgment():
    """Wait for acknowledgment from Arduino that Morse code has been displayed."""
    while True:
        if arduino.in_waiting > 0:  # Check if Arduino has sent any data
            ack = arduino.readline().decode('utf-8').strip()  # Read the acknowledgment message
            if ack == "DONE":  # Assume Arduino sends "DONE" when Morse code display is complete
                print("Arduino completed Morse code display.")
                break

# Continuously capture and send speech input to Arduino
try:
    while True:
        command = recognize_speech()
        if command:
            # Send the captured input directly to the Arduino
            arduino.write((command + '\n').encode('utf-8'))
            print(f"Sent to Arduino: {command}")

            # Wait for Arduino to complete the Morse code display
            wait_for_acknowledgment()

        # Optional: Use 'exit' command to terminate the script
        if command and "exit" in command.lower():
            print("Exiting the script.")
            break
except KeyboardInterrupt:
    print("Program interrupted by user.")

# Close the connection to Arduino
arduino.close()
print("Connection to Arduino closed.")
