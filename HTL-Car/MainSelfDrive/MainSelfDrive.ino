#include <FahrzeugLib.h>
#include <SoftwareSerial.h>
#include "./Audio/Audio.cpp"

#define triggerpinsL 6
#define triggerpinsM 8
#define triggerpinsR 11

#define echopinL 7
#define echopinM 9
#define echopinR 12

unsigned long previousMillis = 0;

Audio audio(13);
CMotor MotorLinks(5, 4);
CMotor MotorRechts(3, 2);
CAntrieb Antrieb(&MotorLinks, &MotorRechts);

SoftwareSerial bt(11, 10);

enum Direction
{
    FORWARD = true,
    BACKWARD = false,
};

Direction direction = FORWARD;
float velocity = 255;
float angle = 0;
bool stoped = false;
bool turn = false;

double distance = 0;

int numTurnRepeats = 0;

void setup()
{
    Serial.begin(115200);
    bt.begin(9600);
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

    if (distanceM <= 75 && distanceM > 18)
    {
        numTurnRepeats = 0;
        if (distanceL > distanceR)
        {
            angle = map(distanceL, 0, 50, 0, 200) / 100.0;
        }
        else if (distanceL < distanceR)
        {
            angle = map(distanceR, 0, 50, 0, -200) / 100.0;
        }
        Antrieb.Kurve(angle, velocity, direction);
    }
    else if (distanceM > 75)
    {
        angle = 0;
        Antrieb.Kurve(angle, velocity, direction);
    }
    else if ((distanceM < 18 || distanceL < 18 || distanceR < 18) || (distanceM < 790 || distanceL < 790 || distanceR < 790))
    {
        audio.startAudio(5000);
        if (distanceL > distanceR)
        {
            angle = map(distanceL, 0, 18, 0, -100) / 100.0;
        }
        else if (distanceL < distanceR)
        {
            angle = map(distanceR, 0, 18, 0, 100) / 100.0;
        }
        direction = BACKWARD;
        Antrieb.Kurve(angle, velocity, direction);
        numTurnRepeats++;
        delay(500 + (numTurnRepeats * 100));
        direction = FORWARD;
        audio.stopAudio();
    }
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