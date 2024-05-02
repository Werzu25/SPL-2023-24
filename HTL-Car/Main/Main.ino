int motor1Pin1 = 3;
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

void motorControll(int pwmPin, int nonPwmPin, int speed, bool direction)
{
    if (direction) {
        digitalWrite(nonPwmPin, direction);
        analogWrite(pwmPin, speed);
    } else {
        digitalWrite(nonPwmPin, direction);
        analogWrite(pwmPin, (255 - speed));
    }
}