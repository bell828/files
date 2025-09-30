#define PIN_LED 7

int period_us = 1000;
int duty_percent = 0;

void set_period(int period) {
  if (period < 100) period = 100;
  if (period > 10000) period = 10000;
  period_us = period;
}

void set_duty(int duty) {
  if (duty < 0) duty = 0;
  if (duty > 100) duty = 100;
  duty_percent = duty;
}

void pwm_cycle() {
  int high_time = (period_us * duty_percent) / 100;
  int low_time = period_us - high_time;

  if (high_time > 0) {
    digitalWrite(PIN_LED, HIGH);
    delayMicroseconds(high_time);
  }
  if (low_time > 0) {
    digitalWrite(PIN_LED, LOW);
    delayMicroseconds(low_time);
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  set_period(100);
}

void loop() {
  for (int d = 0; d < 100; d++) {
    set_duty(d);
    unsigned long start = millis();
    while (millis() - start < 5) {
      pwm_cycle();
    }
  }

  for (int d = 100; d > 0; d--) {
    set_duty(d);
    unsigned long start = millis();
    while (millis() - start < 5) {
      pwm_cycle();
    }
  }
}
