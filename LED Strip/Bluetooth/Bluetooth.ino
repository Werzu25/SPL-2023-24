#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#include "HardwareSerial.h"

#define ledPin 6
#define buttonPin 2

SoftwareSerial bt(10, 11);
uint32_t color = Adafruit_NeoPixel::Color(255, 255, 255);
int patternNr = 0;
bool oldState = false;
unsigned long edgeTimeout = millis();
unsigned long eepromTimeout = millis();
unsigned long timeout = millis();
int activeLeds = 5;
int ledCount = 30;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(255, ledPin, NEO_GRB + NEO_KHZ800);
char btChar = ' ';
char btId = ' ';
char btIdTemp = ' ';
bool btIdExpected = true;
bool btNewReading = false;
String btValueString = "";
float btValue;
int direction = 1;
int currentLed = 0;
const int pattern = 4;
const int timer = 100;

void initialize() {  // Initialisierung
    direction = 1;
    currentLed = 0;
}

void printMusterNr() {  // Muster Nummer ausgeben
    Serial.print("Muster: ");
    Serial.println(patternNr);
}

void buttonQuery() {  // Button abfragen
    bool state = !digitalRead(buttonPin);
    bool risingEdge = state && !oldState;
    bool fallingEdge = !state && oldState;

    oldState = state;

    if (risingEdge && (millis() > edgeTimeout + 20)) {
        initialize();
        patternNr = (patternNr + 1) % pattern;
        printMusterNr();
    }
    if (risingEdge || fallingEdge) {
        edgeTimeout = millis();
    }
}

void btQuery() {  // Bluetooth abfragen
    btNewReading = false;
    if (bt.available()) {
        btChar = bt.read();
        Serial.write(btChar);
        if (btIdExpected) {
            btIdTemp = btChar;
            btIdExpected = false;
            btValueString = "";
        } else {
            if ((btChar != ' ') && (btChar != '\n')) btValueString += btChar;
            if (btChar == '\n') {
                btId = btIdTemp;
                btValue = btValueString.toFloat();
                btValueString = "";
                btIdExpected = true;
                btNewReading = true;
            }
        }
    }
}

void btSetValue() {  // Bluetooth Signal verarbeiten
    switch (btId) {
        case 'N':
            initialize();
            activeLeds = (ledCount * btValue) / 100;
            break;
        case 'A':
            if (btNewReading) {
                initialize();
                patternNr = (patternNr + 1) % pattern;
                printMusterNr();
            }
            break;
        case 'S':
            initialize();
            ledCount = btValue;
            break;
        case 'C':
            if (btNewReading) {
                int tmp = btValue;
                switch (tmp) {
                    case 0:
                        color = Adafruit_NeoPixel::Color(255, 255, 255);
                        break;
                    case 1:
                        color = Adafruit_NeoPixel::Color(255, 0, 0);
                        break;
                    case 2:
                        color = Adafruit_NeoPixel::Color(0, 255, 0);
                        break;
                    case 3:
                        color = Adafruit_NeoPixel::Color(0, 0, 255);
                        break;
                }
            }
            break;
    }
}

void showTimeout() {  // fix strip.show() weil libary nicht funktioniert
    if ((timeout + 50) < millis()) {
        strip.show();
        timeout = millis();
    }
}

void loadFromEeprom() {         // Laden aus EEPROM
    if (EEPROM.read(0) == 0) {  // default Werte
        patternNr = 0;
        activeLeds = 5;
        ledCount = 30;
    } else {
        patternNr = EEPROM.read(0);
        activeLeds = EEPROM.read(1);
        ledCount = EEPROM.read(2);

        Serial.print("patternNr: ");
        Serial.println(patternNr);
        Serial.print("activeLeds: ");
        Serial.println(activeLeds);
        Serial.print("ledCount: ");
        Serial.println(ledCount);
    }
}

void saveToEeprom() {  // Speichern in EEPROM
    EEPROM.write(0, patternNr);
    EEPROM.write(1, activeLeds);
    EEPROM.write(2, ledCount);
}

//--------------MUSTER---------------
void pattern0() {  // alle in einer Farbe
    strip.clear();
    for (int i = 0; i < activeLeds; i++) {
        strip.setPixelColor(i, color);
    }
    showTimeout();
}

void pattern1() {  // lauflicht
    if ((timeout + timer) < millis()) {
        strip.clear();
        for (int i = currentLed; i <= currentLed + activeLeds - 1; ++i) {
            strip.setPixelColor(i % ledCount, color);
        }
        currentLed = (currentLed + 1) % ledCount;
        showTimeout();

        timeout = millis();
    }
}

void pattern2() {  // hin & her Lauflicht
    if ((timeout + timer) < millis()) {
        strip.clear();
        for (int i = currentLed; i <= currentLed + activeLeds - 1; i++) {
            strip.setPixelColor(i, color);
        }
        currentLed += direction;
        if ((currentLed == 0) || (currentLed == ledCount - activeLeds)) {
            direction = -direction;
        }
        showTimeout();

        timeout = millis();
    }
}

void pattern3() {  // symmetrisch
    if ((timeout + timer) < millis()) {
        strip.clear();
        for (int i = currentLed; i <= currentLed + (activeLeds / 2) - 1; i++) {
            strip.setPixelColor(i, color);
        }
        for (int i = ledCount - currentLed - (activeLeds / 2);
             i <= ledCount - currentLed - 1; i++) {
            strip.setPixelColor(i, color);
        }
        currentLed += direction;
        if ((currentLed == 0) ||
            (currentLed == (ledCount / 2) - (activeLeds / 2))) {
            direction = -direction;
        }
        showTimeout();

        timeout = millis();
    }
}
//-----------------------------------

void setup() {
    pinMode(buttonPin, INPUT_PULLUP);
    Serial.begin(9600);
    bt.begin(9600);
    strip.begin();

    loadFromEeprom();
}

void loop() {
    // Bluetooth
    btQuery();
    btSetValue();

    // Button
    buttonQuery();

    // save to EEPROM
    if (millis() > (eepromTimeout + 500)) {
        saveToEeprom();
        eepromTimeout = millis();
    }

    // Muster
    switch (patternNr) {
        case 0:
            pattern0();
            break;
        case 1:
            pattern1();
            break;
        case 2:
            pattern2();
            break;
        case 3:
            pattern3();
            break;
    }
}