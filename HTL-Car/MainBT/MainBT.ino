#include <FahrzeugLib.h>
#include <SoftwareSerial.h>

CMotor MotorLinks(5, 4);
CMotor MotorRechts(3, 2);

CAntrieb Antrieb(&MotorLinks, &MotorRechts);

SoftwareSerial bt(10, 11);
char btChar = ' ';
char btId = ' ';
char btIdTemp = ' ';
bool btIdExpected = true;
bool btNewReading = false;
String btValueString = "";
float btValue;

float velocity = 0;
float angle = 0;
bool direction = true;
bool stoped = false;

void setup()
{
    bt.begin(9600);
    Serial.begin(9600);
}

void loop()
{
    btQuery();
    btSetValue();
    if (!stoped)
    {
        Antrieb.Kurve(angle, velocity, direction);
    }
    else
    {
        Antrieb.Stop();
    }
    Serial.println(btValue);
}

void btQuery()
{
    btNewReading = false;
    if (bt.available())
    {
        btChar = bt.read();
        Serial.write(btChar);
        if (btIdExpected)
        {
            btIdTemp = btChar;
            btIdExpected = false;
            btValueString = "";
        }
        else
        {
            if ((btChar != ' ') && (btChar != '\n'))
            {
                btValueString += btChar;
            }
            if (btChar == '\n')
            {
                btId = btIdTemp;
                btValue = btValueString.toFloat();
                btValueString = "";
                btIdExpected = true;
                btNewReading = true;
            }
        }
    }
}

void btSetValue()
{
    switch (btId)
    {
    case 'S':
        stoped = true;
        break;
    case 'F':
        stoped = false;
        direction = true;
        break;
    case 'B':
        stoped = false;
        direction = false;
        break;
    case 'V':
        velocity = btValue;
        break;
    case 'Y':
        angle = -btValue;
        break;
    }
}