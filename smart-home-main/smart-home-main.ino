#define BLYNK_TEMPLATE_ID "TMPL6e0Yhgw39"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "apNH0lk3Kq05f2fZaY5wfniDWTZmo90S"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include "DHT.h"

#define DHTPIN 23
#define MQ_PIN 22
#define BUZZER_PIN 5
#define LED_PIN 21
// #define FAN_PIN 10
#define SERVO_PIN 19

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo servo;

// Koneksi WiFi
const char* ssid = "Kipas Angin";
const char* password = "11223344";

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(MQ_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // pinMode(FAN_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  // digitalWrite(FAN_PIN, LOW);

  servo.attach(SERVO_PIN);
  servo.write(0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

BLYNK_WRITE(V3) {
  int switchValue = param.asInt();
  if (switchValue == 1) {
    servo.write(90);
    Serial.println("Servo diaktifkan, posisi 90 derajat");
  } else {
    servo.write(0);
    Serial.println("Servo dimatikan, posisi 0 derajat");
  }
}

BLYNK_WRITE(V2) {
  int switchValue = param.asInt();
  digitalWrite(LED_PIN, switchValue);
  Serial.print("LED ");
  Serial.println(switchValue ? "ON" : "OFF");
}

void loop() {
  Blynk.run();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int mqVal = analogRead(MQ_PIN);

  if (mqVal > 200) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("bahaya", "Terlalu banyak karbon monoksida, beri tindakan!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V4, mqVal);

  Serial.print("Suhu: ");
  Serial.print(temp);
  Serial.print(" || Kelembapan: ");
  Serial.print(hum);
  Serial.print(" || MQ: ");
  Serial.println(mqVal);

  delay(1000);
}
