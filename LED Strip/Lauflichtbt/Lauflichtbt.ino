#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define anzMuster 4
#define LED_PIN 6
#define LED_COUNT 307
char BT_Char=' ';
char BT_ID=' ';
char BT_ID_temp= ' ';
bool BT_ID_expected=true;
bool BT_newReading=false;
String BT_ValueStr="";
float BT_Value;
int bntPin = 2;
int richtung = 1;
int musterNr = 0;
int vorherigesMuster = 0;
bool altStatus = 0;
int anzLeuchtLeds = 4;
unsigned long zeitLetzteFlanke = millis();
unsigned long lastShow = millis();

int counter = 0;
unsigned long vorherigesMillis = millis();

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
SoftwareSerial btSerial(10, 11); // RX, TX

void setup()
{
    pinMode(bntPin, INPUT);
    Serial.begin(9600);
    btSerial.begin(9600);
    pixels.begin();
    pixels.show();
}

void loop()
{
    musterWechsel();
    tasteAbfrage();
    btAbfrage();
    BT_check();
    showLeds();
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

void btAbfrage()
{
    musterWechsel();
    if (btSerial.available())
    {
        char zeichen = btSerial.read();
        Serial.print(zeichen);
        if (zeichen == 'E')
        {
            musterNr = (musterNr + 1) % anzMuster;
        }
    }
}
void showLeds()
{
    if ((lastShow + 15) <= millis())
    {
        pixels.show();
        lastShow = millis();
    }
}
void BT_setValue()
{
    switch (BT_ID)
    {
    case 'N':
        Serial.println("test");
        break;
    }
}
void BT_check()
{
    BT_newReading = false;
    if (btSerial.available())
    {
        BT_Char = btSerial.read();
        Serial.write(BT_Char);
        if (BT_ID_expected)
        {
            BT_ID_temp = BT_Char;
            BT_ID_expected = false;
            BT_ValueStr = "";
        }
        else
        {
            if ((BT_Char != ' ') && (BT_Char != '\n'))
                BT_ValueStr += BT_Char;
            if (BT_Char == '\n')
            {
                BT_ID = BT_ID_temp;
                BT_Value = BT_ValueStr.toFloat();
                BT_ValueStr = "";
                BT_ID_expected = true;
                BT_newReading = true;
            }
        }
    }
}
