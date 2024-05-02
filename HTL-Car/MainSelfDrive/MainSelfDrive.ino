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

SoftwareSerial bt(11, 10);


enum Direction
{
    FORWARD = true,
    BACKWARD = false,
};

unsigned long previousMillis = 0;

Direction direction = FORWARD;
float velocity = 255;
float angle = 0;
bool stoped = false;
bool turn = false;

double distance = 0;

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


    bt.print("DistanceL: ");
    bt.print(distanceL);
    
    bt.print(" DistanceM: ");
    bt.print(distanceM);

    bt.print(" DistanceR: ");
    bt.println(distanceR);

    if (distanceM < 65 && distanceM > 18)
    {
        if (distanceL > distanceR)
        {
            angle = map(distanceL, 0, 45, 0, 200) / 100.0;
        }
        else if (distanceL < distanceR)
        {
            angle = map(distanceR, 0, 45, 0, -200) / 100.0;
        }
        Antrieb.Kurve(angle, velocity, direction);
    }
    else if (distanceM > 65)
    {
        angle = 0;
        Antrieb.Kurve(angle, velocity, direction);
    }
    else if ((distanceM < 18 || distanceL < 18 || distanceR < 18 )|| (distanceM < 790 || distanceL < 790 || distanceR < 790))
    {
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
        delay(500);
        direction = FORWARD;
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