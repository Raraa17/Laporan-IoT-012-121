#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// ThingSpeak API
String apiKey = "API_KEY_KAMU";  
const char* server = "http://api.thingspeak.com/update";

// Pin sensor & output
#define TRIG_PIN 18
#define ECHO_PIN 5
#define LED_PIN 25
#define BUZZER_PIN 26

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  // Kirim pulsa trig
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Hitung durasi echo
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kontrol LED & buzzer
  if (distance > 0 && distance < 10) {
    digitalWrite(LED_PIN, HIGH);   // LED nyala
    digitalWrite(BUZZER_PIN, LOW); // buzzer nyala
  } else {
    digitalWrite(LED_PIN, LOW);    // LED mati
    digitalWrite(BUZZER_PIN, HIGH);// buzzer mati
  }

  // Kirim data ke ThingSpeak setiap 15 detik
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(distance);
    
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("Data terkirim ke ThingSpeak");
    } else {
      Serial.println("Gagal kirim data");
    }
    http.end();
  }

  delay(15000); // ThingSpeak limit: 15 detik per update
}