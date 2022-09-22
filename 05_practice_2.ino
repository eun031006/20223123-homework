#define PIN_LED 7

void setup() {
  pinMode(PIN_LED, OUTPUT);

}

void loop() {
  digitalWrite(PIN_LED, LOW);
  delay(1000);
  int i;
  i=1;
  while(i<=5){
    digitalWrite(PIN_LED, HIGH);
    delay(200);
    digitalWrite(PIN_LED, LOW);
    delay(200);
    i=i+1;
  }
  digitalWrite(PIN_LED, HIGH);
  while(1){
    
  }

}
