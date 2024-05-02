#include <FahrzeugLib.h>
#include <SoftwareSerial.h>

#define triggerpinsL 6
#define triggerpinsM 8
#define triggerpinsR 11

#define echopinL 7
#define echopinM 9
#define echopinR 12

CMotor MotorLinks(5, 4);
CMotor MotorRechts(3, 2);
CAntrieb Antrieb(&MotorLinks, &MotorRechts);

enum Direction
{
    FORWARD = true,
    BACKWARD = false,
};

unsigned long previousMillis = 0;

Direction direction = FORWARD;
float velocity = 250;
float angle = 0;
bool stoped = false;
bool turn = false;

double distance = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(triggerpinsL, OUTPUT);
    pinMode(triggerpinsM, OUTPUT);
    pinMode(triggerpinsR, OUTPUT);
    pinMode(echopinL, INPUT);
    pinMode(echopinM, INPUT);
    pinMode(echopinR, INPUT);
}

void loop()
{
    double distanceR = calcDistance(triggerpinsR, echopinR);
    double distanceL = calcDistance(triggerpinsL, echopinL);
    double distanceM = calcDistance(triggerpinsM, echopinM);

    if (distanceM < 75)
    {
        if (distanceL < 10 || distanceR < 10 || distanceM < 10)
        {
            if (distanceL > distanceR)
            {
                Antrieb.Wende(false, velocity);
            }
            else if (distanceL < distanceR)
            {
                Antrieb.Wende(true, velocity);
            }
        }
        if (distanceL > distanceR)
        {
            angle = map(distanceL, 0, 30, 0, 200) / 100.0;
        }
        else if (distanceL < distanceR)
        {
            angle = map(distanceR, 0, 30, 0, -200) / 100.0;
        }
    }
    else
    {
        angle = 0;
    }
    Antrieb.Kurve(angle, velocity, direction);
}

double calcDistance(int triggerPin, int echoPin)
{
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);

    double duration = pulseIn(echoPin, HIGH);
    distance = duration / 58;
    return distance;
}