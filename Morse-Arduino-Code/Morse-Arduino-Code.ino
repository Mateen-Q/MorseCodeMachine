#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Morse code for A-Z
String morseCodeAlphabet[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..",
    "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int MonoledPin = 7;
const int RedPin = 5;
const int BluePin = 4;
const int spacepin = 6;
const int spacepin2 = 3;
const int encryptionread = A3;
const int decoderModePin = A1;
const int dotButton = 8;
const int dashButton = 9;

const int defaultDotDuration = 200;
const int defaultDashDuration = 600;

int dotDuration = defaultDotDuration;
int dashDuration = defaultDashDuration;

String inputSequence = ""; // For decoding input
bool decodingMode = false;

void setup() {
    pinMode(MonoledPin, OUTPUT);
    pinMode(RedPin, OUTPUT);
    pinMode(BluePin, OUTPUT);
    pinMode(spacepin, OUTPUT);
    pinMode(spacepin2, OUTPUT);
    pinMode(encryptionread, INPUT);
    pinMode(decoderModePin, INPUT);
    pinMode(dotButton, INPUT_PULLUP);
    pinMode(dashButton, INPUT_PULLUP);

    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.init();
    lcd.backlight();
    lcd.print("Encoder-Decoder");
    Serial.print("Encoder-Decoder");
}

void loop() {
    decodingMode = digitalRead(decoderModePin) == HIGH; // Check if decoder mode is active

    if (decodingMode) {
        lcd.setCursor(0, 1);
        lcd.print("Decoder Mode   ");
        handleDecoder();
    } else {
        lcd.setCursor(0, 1);
        lcd.print("Encoder Mode   ");
        handleEncoder();
    }
}

void handleEncoder() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input.startsWith("DOT:")) {
            dotDuration = input.substring(4).toInt();
            if (dotDuration < 100) dotDuration = 100;
            Serial.println("Dot duration set to: " + String(dotDuration) + " ms");
            return;
        }

        if (input.startsWith("DASH:")) {
            dashDuration = input.substring(5).toInt();
            if (dashDuration < dotDuration) dashDuration = dotDuration + 400;
            Serial.println("Dash duration set to: " + String(dashDuration) + " ms");
            return;
        }

        if (input.equalsIgnoreCase("DEFAULT")) {
            dotDuration = defaultDotDuration;
            dashDuration = defaultDashDuration;
            Serial.println("Durations reset to default values.");
            return;
        }

        if (input.equalsIgnoreCase("SOS")) {
            lcd.setCursor(0, 1);
            lcd.print("SOS ALERT!      ");
            triggerSOS();
            return;
        }

        input.toUpperCase();
        for (int i = 0; i < input.length(); i++) {
            char c = input[i];
            if (c >= 'A' && c <= 'Z') {
                String morse = morseCodeAlphabet[(int)(c - 'A')];
                lcd.setCursor(0, 1);
                lcd.print(c);
                lcd.setCursor(1, 1);
                lcd.print(":");
                lcd.setCursor(2, 1);
                lcd.print(morse);
                lcd.print("            ");
                displayMorse(morse);
            } else if (c == ' ') {
                delay(dotDuration * 4);
            }
        }
        lcd.setCursor(0, 1);
        lcd.print(" :)              ");
        displayElapsedTime();
    }
}
/*
void handleDecoder() {
    if (digitalRead(dotButton) == LOW) {
        inputSequence += '.';
        lcd.setCursor(0, 1);
        lcd.print(inputSequence + "        ");
        delay(200);
    }

    if (digitalRead(dashButton) == LOW) {
        inputSequence += '-';
        lcd.setCursor(0, 1);
        lcd.print(inputSequence + "        ");
        delay(200);
    }

    if (inputSequence.length() > 0 && millis() % 2000 < 50) { // After a pause
        decodeMorse(inputSequence);
        inputSequence = ""; // Reset sequence
    }
}
*/
void handleDecoder() {
    static unsigned long lastInputTime = 0;

    if (digitalRead(dotButton) == LOW) {
        inputSequence += '.';
        lcd.setCursor(0, 1);
        lcd.print(inputSequence + "        ");
        delay(30); // Debounce delay
        lastInputTime = millis();
    }

    if (digitalRead(dashButton) == LOW) {
        inputSequence += '-';
        lcd.setCursor(0, 1);
        lcd.print(inputSequence + "        ");
        delay(30); // Debounce delay
        lastInputTime = millis();
    }

    // Decode after 500 ms of inactivity
  if (inputSequence.length() > 0 && millis() - lastInputTime > 150){
    //  lcd.clear(); // Clear the LCD before displaying decoded result
        decodeMorse(inputSequence);
        inputSequence = ""; // Reset sequence
    }
}


void decodeMorse(String morse) {
    for (int i = 0; i < 26; i++) {
        if (morseCodeAlphabet[i] == morse) {
            char decodedChar = 'A' + i;
          lcd.clear();
          lcd.setCursor(0,0);
         lcd.print("Encoder-Decoder");
            lcd.setCursor(0, 1);
            lcd.print("Decoded: ");
            lcd.print(decodedChar);
            Serial.println("Decoded: " + String(decodedChar));
            delay(2000);
            return;
        }
    }
    lcd.setCursor(0, 1);
    lcd.print("Invalid Morse    ");
    Serial.println("Invalid Morse Code");
    delay(2000);
}

void triggerSOS() {
    String sosMorse = "... --- ...";
    for (int repeat = 0; repeat < 3; repeat++) {
        displayMorse(sosMorse);
    }
    lcd.setCursor(0, 1);
    lcd.print("HELP SENT!      ");
    delay(2000);
}

void displayElapsedTime() {
    unsigned long totalMillis = millis();
    unsigned long seconds = (totalMillis / 1000) % 60;
    unsigned long minutes = (totalMillis / (1000 * 60)) % 60;
    unsigned long hours = (totalMillis / (1000 * 60 * 60)) % 24;

    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(hours);
    lcd.print("h ");
    lcd.print(minutes);
    lcd.print("m ");
    lcd.print(seconds);
    lcd.print("s    ");
    delay(2000);
}

void displayMorse(String morse) {
   if (analogRead(encryptionread) >= 500) { // Normal mode
        for (int j = 0; j < morse.length(); j++) {
        if (morse[j] == '.') {
            digitalWrite(MonoledPin, HIGH); // Turn LED on for a dot
            delay(dotDuration);        // Dot duration
            digitalWrite(MonoledPin, LOW); // Turn LED off
        } else if (morse[j] == '-') {
            digitalWrite(MonoledPin, HIGH); // Turn LED on for a dash
            delay(dashDuration);        // Dash duration
            digitalWrite(MonoledPin, LOW);  // Turn LED off
        }
        delay(dotDuration); // Delay between parts of a letter
    }
        digitalWrite(spacepin, HIGH);
        delay(500); // Delay between letters
        digitalWrite(spacepin, LOW);
    }
     else if (analogRead(encryptionread) <= 500) { // Encryption mode
        for (int j = 0; j < morse.length(); j++) {
            if (morse[j] == '.') {
                digitalWrite(RedPin, HIGH); // Turn LED on for a dot
                delay(200);                // Dot duration
                digitalWrite(RedPin, LOW); // Turn LED off
            } else if (morse[j] == '-') {
                digitalWrite(BluePin, HIGH); // Turn LED on for a dash
                delay(200);                // Dash duration
                digitalWrite(BluePin, LOW); // Turn LED off
            }
            delay(200); // Delay between parts of a letter
        }
        digitalWrite(spacepin2, HIGH);
        delay(500); // Delay between letters
        digitalWrite(spacepin2, LOW);
    }
}
