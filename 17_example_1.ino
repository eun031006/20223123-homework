#include <Servo.h>

// Arduino pin assignment
#define PIN_LED 9
#define PIN_POTENTIOMETER 3
#define PIN_IRSENSOR 0
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)
#define _DIST_MIN 100.0
#define _DIST_MAX 250.0

#define _EMA_ALPHA 0.5

Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist_ema, dist_prev = _DIST_MAX; 

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);

  dist_prev = _DIST_MIN;
  
  Serial.begin(2000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  float dist;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IRSENSOR);
  dist = (6762.0/(a_value-9)-4.0)*10.0-60.0;

  if (dist < _DIST_MIN) {
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);
    myservo.writeMicroseconds(_DUTY_MIN);
  } else if (dist > _DIST_MAX) {
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);
    myservo.writeMicroseconds(_DUTY_MAX);
  } else {
    digitalWrite(PIN_LED, 0);
    dist_prev = dist;
    myservo.writeMicroseconds(553+(dist-100.0)*184.6/15);
  }
  
  dist_ema = _EMA_ALPHA*dist + (1-_EMA_ALPHA)*dist_ema;

  // map distance into duty
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:");    Serial.print(_DIST_MIN);
  Serial.print(",IR:");    Serial.print(a_value);
  Serial.print(",dist:");  Serial.print(dist);
  Serial.print(",ema:");   Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.print(",MAX:");   Serial.print(_DIST_MAX);
  Serial.println("");
}
