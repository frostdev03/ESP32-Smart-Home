#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <ESP32Servo.h>
#include "DHT.h"

#define DHTPIN 23
#define MQ_PIN 18
#define BUZZER_PIN 5
#define LED_PIN 21
// #define FAN_PIN 10
#define SERVO_PIN 19

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Koneksi WiFi
const char* ssid = "Setrika";
const char* password = "11223344";

// WhatsApp API
String phoneNumber = "+6289601485849";
String apiKey = "8192384";

// Variabel status
int fanSpeed = 0;  // 0 = Off, 1 = Slow, 2 = Fast
bool ledState = false;
bool servoState = false;

// Fungsi untuk mengirim pesan WhatsApp
void sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Kirim HTTP POST request
  int httpResponseCode = http.POST("");
  if (httpResponseCode == 200) {
    Serial.println("Pesan berhasil dikirim");
  } else {
    Serial.println("Gagal mengirim pesan");
    Serial.print("Kode respons HTTP: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  delay(3000);
}

// Fungsi untuk mengontrol kipas berdasarkan suhu
// void controlFan(float temp) {
//   if (temp < 25) {
//     fanSpeed = 0;  // Off
//     analogWrite(FAN_PIN, 0);
//   } else if (temp < 30) {
//     fanSpeed = 1;               // Slow
//     analogWrite(FAN_PIN, 128);  // Setengah kecepatan
//   } else {
//     fanSpeed = 2;               // Fast
//     analogWrite(FAN_PIN, 255);  // Kecepatan penuh
//   }
// }

// // Fungsi untuk membaca perintah dari WhatsApp
// void parseCommand(String command) {
//   if (command == "cek suhu") {
//     float hum = dht.readHumidity();
//     float temp = dht.readTemperature();
//     sendMessage("Suhu: " + String(temp) + "°C, Kelembapan: " + String(hum) + "%");
//   } else if (command == "lampu nyala") {
//     ledState = true;
//     digitalWrite(LED_PIN, HIGH);
//     sendMessage("Lampu dinyalakan.");
//   } else if (command == "lampu mati") {
//     ledState = false;
//     digitalWrite(LED_PIN, LOW);
//     sendMessage("Lampu dimatikan.");
//   } else if (command == "servo nyala") {
//     servoState = true;
//     digitalWrite(SERVO_PIN, HIGH);
//     sendMessage("Servo dihidupkan.");
//   } else if (command == "servo mati") {
//     servoState = false;
//     digitalWrite(SERVO_PIN, LOW);
//     sendMessage("Servo dimatikan.");
//   }
// }

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan ke WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Terhubung ke WiFi dengan IP: " + WiFi.localIP());

  // Setup pin
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // pinMode(FAN_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);

  // Matikan semua perangkat awal
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  // digitalWrite(FAN_PIN, LOW);
  digitalWrite(SERVO_PIN, LOW);
}

void loop() {

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // Kontrol kipas berdasarkan suhu
  // controlFan(temp);

  // Membaca nilai dari sensor MQ
  int mqValue = analogRead(MQ_PIN);

  // if (mqValue > 200) {  // Kategori terburuk
  //   digitalWrite(BUZZER_PIN, HIGH);
  // } else if (mqValue < 200) {
  //   digitalWrite(BUZZER_PIN, HIGH);
  // } else {
  //   digitalWrite(BUZZER_PIN, LOW);
  // }

  // Simulasikan menerima pesan WhatsApp
  // String receivedCommand = "";  // Ganti ini dengan fungsi untuk mendapatkan perintah dari WhatsApp
  // if (receivedCommand != "") {
  //   parseCommand(receivedCommand);
  // }

  Serial.print("Suhu ");
  Serial.print(temp);
  Serial.print(" || Kelembapan ");
  Serial.print(hum);
  Serial.print(" || Kualitas Udara ");
  Serial.println(mqValue);

  delay(3000);  // Delay 1 detik
}
