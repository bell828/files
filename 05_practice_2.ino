#define PIN_LED 7
unsigned int count;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  count = 0;
}

void loop() {
  if (count == 0) {
    delay(1000);

    for (int i = 0; i < 6; i++) {
      digitalWrite(PIN_LED, HIGH); 
      delay(100);
      digitalWrite(PIN_LED, LOW);  
      delay(100);
    }

    digitalWrite(PIN_LED, HIGH);
    count = 1;
  }

  while (1) {
    digitalWrite(PIN_LED, HIGH);
  }
}
