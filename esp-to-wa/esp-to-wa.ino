#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Kipas Angin";
const char* password = "11223344";

String phoneNumber = "+6289601485849";
String apiKey = "8192384";

void sendMessage(String message) {
  // Data untuk dikirim menggunakan HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;

  http.begin(url);

  // Tentukan header jenis konten
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

  // Hentikan koneksi HTTP
  http.end();
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Koneksi ke WiFi
  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan ke WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Terhubung ke jaringan WiFi dengan IP: ");
  Serial.println(WiFi.localIP());

  // Kirim pesan ke WhatsApp sebagai test
  sendMessage("Hello from ESP32!");
}

void loop() {
  // Membaca kelembaban dan suhu dari sensor DHT
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // Menampilkan hasil di Serial Monitor
  Serial.print("Kelembaban: ");
  Serial.println(hum);
  Serial.print("Suhu: ");
  Serial.println(temp);

  // Mengirimkan pesan WhatsApp
  String T = String(temp);
  String H = String(hum);
  sendMessage("Temp : " + T + " Hum : " + H);

  delay(3000); // Tunggu 3 detik sebelum mengirim data lagi
}
