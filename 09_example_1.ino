#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100
#define _DIST_MAX 300

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL)

#define N 30   // median filter window size (change to 3, 10, or 30)

unsigned long last_sampling_time;
float samples[N];
int index_sample = 0;
bool buffer_full = false;

float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}

float getMedian(float arr[], int size) {
  float temp[size];
  for (int i = 0; i < size; i++) temp[i] = arr[i];
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (temp[j] > temp[j + 1]) {
        float t = temp[j];
        temp[j] = temp[j + 1];
        temp[j + 1] = t;
      }
    }
  }
  if (size % 2 == 1) return temp[size / 2];
  else return (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.begin(57600);
}

void loop() {
  if (millis() < last_sampling_time + INTERVAL) return;

  float dist_raw = USS_measure(PIN_TRIG, PIN_ECHO);
  samples[index_sample++] = dist_raw;

  if (index_sample >= N) {
    index_sample = 0;
    buffer_full = true;
  }

  int valid_size = buffer_full ? N : index_sample;
  float dist_median = getMedian(samples, valid_size);

  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(dist_raw);
  Serial.print(",ema:"); Serial.print(0); // not used here
  Serial.print(",median:"); Serial.print(dist_median);
  Serial.print(",Max:"); Serial.print(_DIST_MAX);
  Serial.println("");

  if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
    digitalWrite(PIN_LED, 1);
  else
    digitalWrite(PIN_LED, 0);

  last_sampling_time += INTERVAL;
}
