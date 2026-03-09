#include <Servo.h>

Servo gateServo;
int gasSensorPin = A0;
int threshold = 130;
int redLed = 6;
int greenLed = 5;
int buzzer = 7;

int calibrate() {
  Serial.println("Calibrating... keep area clear of gas.");
  long sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(gasSensorPin);
    Serial.print("  Sample ");
    Serial.print(i + 1);
    Serial.print("/100  →  ");
    Serial.println(analogRead(gasSensorPin));
    delay(60000); // 100 samples × 60s = ~100 min warmup built in
  }
  int baseline = sum / 100;
  Serial.print("Baseline: ");
  Serial.println(baseline);
  return baseline + 150;
}

void setup() {
  gateServo.attach(9);
  gateServo.write(0);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  threshold = calibrate();
  Serial.print("Threshold set to: ");
  Serial.println(threshold);
  Serial.println("System armed. Monitoring started.");
}

void loop() {
  int gasLevel = analogRead(gasSensorPin);

  Serial.print("[");
  Serial.print(millis() / 1000);
  Serial.print("s]  Gas=");
  Serial.print(gasLevel);
  Serial.print("  Threshold=");
  Serial.print(threshold);
  Serial.print("  →  ");

  if (gasLevel > threshold) {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(buzzer, HIGH);
    gateServo.write(30);
    Serial.println("DANGER: Gas detected! Closing gate.");
  } else {
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    gateServo.write(0);
    Serial.println("SAFE");
  }

  delay(500);
}
