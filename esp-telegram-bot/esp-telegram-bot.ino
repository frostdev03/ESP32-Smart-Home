#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <ESP32Servo.h>

// Informasi WiFi
const char* ssid = "Setrika";       // Ganti dengan nama WiFi Anda
const char* password = "11223344";  // Ganti dengan password WiFi Anda

// Token dan Chat ID
const char* botToken = "8192486991:AAF69R-I3bQbhY7Jypc2cTXdyG07NHZ8UYI";  // Token bot Anda
const char* chatID = "7274640131";                                        // Chat ID Anda

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// Pin dan Komponen
#define DHTPIN 23
#define MQ_PIN 23
#define BUZZER_PIN 5
#define LED_PIN 21
#define SERVO_PIN 19

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo servo;

// Status dan Variabel
bool lampStatus = false;    // Status LED
bool garageStatus = false;  // Status servo garasi

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    // Hanya merespons pesan dari chat ID yang terdaftar
    if (chat_id != chatID) continue;

    String response;
    if (text == "/start") {
      response = "Selamat datang di Bot Kontrol Rumah!\n\n"
                 "/cek - Cek suhu, kelembapan dan kualitas udara, status lampu dan garasi\n"
                 "/lh - Hidupkan lampu\n"
                 "/lm - Matikan lampu\n"
                 "/ls - Cek keadaan lampu\n"
                 "/bg - Buka garasi\n"
                 "/tg - Tutup garasi\n"
                 "/gs - Cek status garasi\n"
                 "/cs - Cek suhu dan kelembapan ruangan\n"
                 "/cu - Cek kualitas udara\n"
                 "/on - Hidupkan lampu dan buka garasi\n"
                 "/off - Matikan lampu dan tutup garasi";
    } else if (text == "/cek") {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      int airQuality = analogRead(MQ_PIN);

      String tempHumidity;
      if (isnan(temperature) || isnan(humidity)) {
        tempHumidity = "Suhu dan kelembapan tidak dapat dibaca.\n";
      } else {
        tempHumidity = "Suhu: " + String(temperature, 2) + "°C\n"
                                                           "Kelembapan: "
                       + String(humidity, 2) + "%\n";
      }

      String lampStatusStr = lampStatus ? "Lampu: Hidup\n" : "Lampu: Mati\n";
      String garageStatusStr = garageStatus ? "Garasi: Terbuka\n" : "Garasi: Tertutup\n";
      String airQualityStr = "Kualitas Udara (PPM): " + String(airQuality) + "\n";

      response = "Status Rumah:\n" + tempHumidity + lampStatusStr + garageStatusStr + airQualityStr;

    } else if (text == "/lh") {
      digitalWrite(LED_PIN, HIGH);
      lampStatus = true;
      response = "Lampu telah dihidupkan.";
    } else if (text == "/lm") {
      digitalWrite(LED_PIN, LOW);
      lampStatus = false;
      response = "Lampu telah dimatikan.";
    } else if (text == "/ls") {
      response = lampStatus ? "Lampu saat ini hidup." : "Lampu saat ini mati.";
    } else if (text == "/bg") {
      servo.write(90);  // Buka garasi
      garageStatus = true;
      response = "Garasi telah dibuka.";
    } else if (text == "/tg") {
      servo.write(0);  // Tutup garasi
      garageStatus = false;
      response = "Garasi telah ditutup.";
    } else if (text == "/gs") {
      response = garageStatus ? "Garasi saat ini terbuka." : "Garasi saat ini tertutup.";
    } else if (text == "/cs") {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      if (isnan(temperature) || isnan(humidity)) {
        response = "Gagal membaca suhu atau kelembapan. Pastikan sensor terhubung dengan benar.";
      } else {
        response = "Suhu ruangan: " + String(temperature, 2) + "°C\n"
                                                               "Kelembapan: "
                   + String(humidity, 2) + "%";
      }
    } else if (text == "/cu") {
      int airQuality = analogRead(MQ_PIN);
      response = "Kualitas udara (PPM): " + String(airQuality);
    } else if (text == "/on") {
      digitalWrite(LED_PIN, HIGH);
      servo.write(90);  // Buka garasi
      lampStatus = true;
      garageStatus = true;
      response = "Lampu telah dihidupkan dan garasi telah dibuka.";
    } else if (text == "/off") {
      digitalWrite(LED_PIN, LOW);
      servo.write(0);  // Buka garasi
      lampStatus = false;
      garageStatus = false;
      response = "Lampu telah dimatikan dan garasi telah ditutup.";
    } else {
      response = "Perintah tidak dikenal.";
    }

    bot.sendMessage(chat_id, response, "");
  }
}

void setup() {
  Serial.begin(115200);

  // Inisialisasi Pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ_PIN, INPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Inisialisasi Servo
  servo.attach(SERVO_PIN);
  servo.write(0);  // Garasi ditutup

  // Inisialisasi Sensor
  dht.begin();

  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("Terhubung ke WiFi.");

  // Mengatur koneksi SSL untuk Telegram Bot
  client.setInsecure();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
  }
  delay(1000);  // Interval untuk mengecek pesan baru
}
