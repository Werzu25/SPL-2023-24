//Einbinden der Fahrzeug-Library
#include <FahrzeugLib.h>

/*
 * geschw ist ein Wert von 0 bis 255
 * form gibt den Kurvenradius an: form = 0 -> geradeaus
 *                                form = -1 ->int motor1Pin1 = 3;
int motor1Pin2 = 2;
int motor2Pin1 = 5;
int motor2Pin2 = 4;

void setup() {
    pinMode(motor1Pin1,OUTPUT);
    pinMode(motor1Pin2,OUTPUT);
    pinMode(motor2Pin1,OUTPUT);
    pinMode(motor2Pin1,OUTPUT);
    Serial.begin(9600);
}

void loop() {

}
root
void motorControll(int pwmPin, int nonPwmPin, int speed, bool direction)
{
    if (direction) {
        digitalWrite(nonPwmPin, direction);
        analogWrite(pwmPin, speed);
    } else {
        digitalWrite(nonPwmPin, direction);
        analogWrite(pwmPin, (255 - speed));

    }
} Linkskurve, linkes Rad ist halb so schnell wie rechtes
 *                                form = 1 -> Rechtskurve, rechtes Rad ist halb so schnell wie linkes
 *                                
 * Methoden für Klasse CMotor:
    CMotor(int pinPWM, int pinNichtPWM);
    void Bewegen(bool vorwaerts, int geschw);
    void Stop();

 * Methoden für Klasse CAntrieb:
    CAntrieb(CMotor *motor1, CMotor *motor2);
    void Gerade(bool vorwaerts, int geschw);
    void Kurve(double form, int geschw, bool vorwaerts);
    void Stop();
    void Wende(bool links, int geschw);
 */

//Erstellen von zwei Objekten vom Typ "C-Motor"
CMotor MotorLinks(5,4);  // Der Pin, der im ersten Parameter angegeben ist, muss PWM-fähig sein, der zweite nicht unbedingt
CMotor MotorRechts(3,2);

//Erstellen eines Objektes vom Type "CAntrieb"
CAntrieb Antrieb(&MotorLinks, &MotorRechts);

void setup() {
  MotorLinks.Bewegen(true,200);
  MotorRechts.Bewegen(true,200);
  delay(1000);
  MotorLinks.Stop();
  MotorRechts.Stop();
  delay(1000);
}

void loop() {
delay(1000);
Antrieb.Kurve(-2,200,true);
delay(5000);
Antrieb.Stop();
}
