#include <Adafruit_NeoPixel.h>
#define anzMuster 4
#define LED_PIN 4
#define LED_COUNT 30

int bntPin = 2;
int richtung = 1;
int musterNr = 0;
int vorherigesMuster = 0;
bool altStatus = 0;
int anzLeuchtLeds = 4;
unsigned long zeitLetzteFlanke = millis();

int counter = 0;
unsigned long vorherigesMillis = millis();

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pinMode(bntPin, INPUT);
    Serial.begin(9600);
    pixels.begin();
    pixels.show();
}

void loop()
{
    tasteAbfrage();
    musterWechsel();
    switch (musterNr)
    {
    case 0:
        allesEin();
        break;
    case 1:
        symetrisch(200);
        break;
    case 2:
        lauflicht(200);
        break;
    case 3:
        hinUndHer(200);
        break;
    }
    vorherigesMuster = musterNr;
}
void tasteAbfrage()
{
    bool aktStatus = digitalRead(bntPin);
    bool steigFlanke = aktStatus && !altStatus;
    bool fallFlake = !aktStatus && altStatus;
    altStatus = aktStatus;

    if (steigFlanke && (millis() > zeitLetzteFlanke + 20))
    {
        musterNr = (musterNr + 1) % anzMuster;
        Serial.println(musterNr);
    }
    if (steigFlanke || fallFlake)
    {
        zeitLetzteFlanke = millis();
    }
}
void allesEin()
{
    pixels.clear();
    for (int i = 0; i < LED_COUNT; i++)
    {
        pixels.setPixelColor(i, Wheel(i, LED_COUNT));
    }
    pixels.show();
}
void lauflicht(unsigned long intervall)
{
    if ((vorherigesMillis + intervall) <= millis())
    {
        pixels.clear();
        pixels.setPixelColor(counter, Wheel(counter, LED_COUNT));
        for (int i = counter; i <= counter + anzLeuchtLeds - 1; i++)
        {
            pixels.setPixelColor((i % LED_COUNT), Wheel(i, LED_COUNT));
        }
        vorherigesMillis = millis();
        counter = (counter + 1) % LED_COUNT;
    }
    pixels.show();
};

void hinUndHer(unsigned long intervall)
{
    if ((vorherigesMillis + intervall) <= millis())
    {
        pixels.clear();
        pixels.setPixelColor(counter, Wheel(counter, LED_COUNT));
        for (int i = counter; i <= counter + anzLeuchtLeds - 1; i++)
        {
            pixels.setPixelColor((i % LED_COUNT), Wheel(i, LED_COUNT));
        }
        counter = (counter + richtung) % LED_COUNT;
        if ((counter == 0) || (counter == LED_COUNT - anzLeuchtLeds))
        {
            richtung = -richtung;
        }
        vorherigesMillis = millis();
    }
    pixels.show();
}
void symetrisch(unsigned long intervall)
{
    if ((vorherigesMillis + intervall) <= millis())
    {
        pixels.clear();
        pixels.setPixelColor(counter, Wheel(counter, LED_COUNT / 2));
        pixels.setPixelColor(LED_COUNT - 1 - counter, Wheel(counter, LED_COUNT / 2));
        for (int i = counter; i <= counter + anzLeuchtLeds - 1; i++)
        {
            pixels.setPixelColor((i % LED_COUNT), Wheel(i, LED_COUNT / 2));
            pixels.setPixelColor(LED_COUNT - 1 - (i % LED_COUNT), Wheel(i, LED_COUNT / 2));
        }
        vorherigesMillis = millis();
        counter = (counter + richtung) % (LED_COUNT / 2);
        if ((counter == 0) || (counter == (LED_COUNT / 2) - anzLeuchtLeds))
        {
            richtung = -richtung;
        }
    }
    pixels.show();
}
uint32_t Wheel(int i, int pixles)
{
    byte WheelPos = i * 256 / pixles;
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void musterWechsel()
{
    if (musterNr != vorherigesMuster)
    {
        counter = 0;
        richtung = 1;
    }
}
