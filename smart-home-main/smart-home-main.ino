#define BLYNK_TEMPLATE_ID "TMPL6e0Yhgw39"
#define BLYNK_TEMPLATE_NAME "IoT"
#define BLYNK_AUTH_TOKEN "apNH0lk3Kq05f2fZaY5wfniDWTZmo90S"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <DHT.h>

#define DHT_PIN 23      // Pin untuk DHT11 (GPIO23)
#define DHT_TYPE DHT11  // Tipe sensor DHT yang digunakan

// Pin untuk MQ-2 (pin analog)
#define MQ2_PIN 34  // Pin analog untuk sensor MQ-2 (GPIO4)
#define BUZZER_PIN 5
#define LED_PIN 21
#define SERVO_PIN 19

DHT dht(DHT_PIN, DHT_TYPE);  // Inisialisasi objek DHT11

Servo servo;

// Koneksi WiFi
const char* ssid = "Setrika";
const char* password = "11223344";
unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(115200);  // Inisialisasi komunikasi serial pada baudrate 115200

  analogReadResolution(12);           // Resolusi 12-bit (0-4095)
  analogSetAttenuation(ADC_11db); 
  dht.begin();           // Inisialisasi sensor DHT11
  

  // Set pin MQ-2 untuk input (untuk pembacaan analog)
  pinMode(MQ2_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

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
  // Membaca suhu dan kelembapan dari DHT11
  float temperature = dht.readTemperature();  // Membaca suhu dalam Celcius
  float humidity = dht.readHumidity();        // Membaca kelembapan dalam persen

  // Cek apakah pembacaan DHT11 berhasil
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca dari sensor DHT11");
  } else {
    // Menampilkan suhu dan kelembapan di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(temperature);
    Serial.print(" °C  ");
    Serial.print("Kelembapan: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // Membaca nilai sensor MQ-2 (pada pin analog)
  int mq2Value = analogRead(MQ2_PIN);

  if (mq2Value >= 900) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.logEvent("bahaya", "Terlalu banyak karbon monoksida, beri tindakan!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Menampilkan nilai pembacaan sensor MQ-2
  Serial.print("Nilai Sensor MQ-2: ");
  Serial.println(mq2Value);

  // Kirim data ke Blynk
  Blynk.virtualWrite(V4, mq2Value);
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  // Delay 2 detik sebelum pembacaan berikutnya
  delay(2000);
}
