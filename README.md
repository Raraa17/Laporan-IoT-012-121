# Laporan-IoT-012-121


| No  | Nama                   | NRP        |
| --- | ---------------------- | ---------- |
| 1   | Adiwidya Budi Pratama  | 5027241012 |
| 2   | Zahra Hafizhah         | 5027241121 |

## Pendahuluan
Tugasnya adalah membuat alat IoT dengan Board ESP-32  dengan menggunakan sensor ultrasonic, lampu LED, dan buzzer. 
-Sensor Ultrasonik > mengukur jarak
Semakin dekat jarak yang diukur maka lampu LED dan buzzer akan menyala, sedangkan makin jauh atau jika lebih dari 20 cm maka lampu LED akan amti dan buzzer juga tidak berbunyi.

## Penjelasan Kode

1. Import Library
```
#include <WiFi.h>
#include <HTTPClient.h>
```
- Wifi.h → digunakan untuk menghubungkan ESP32 ke jaringan WIFI
- HTTP CLient.h → digunakan untuk melakukan request HTTP (mengirim data ke ThingSpeak)

2. Deklarasi WiFi dan ThingSpeak
```
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

String apiKey = "API_KEY_KAMU";  
const char* server = "http://api.thingspeak.com/update";
```
- Ssid dan password → kredensial WiFi agar ESP32 bisa terhubung ke internet.
- ApiKey → API key dari ThingSpeak channel yang digunakan untuk autentikasi.
- Server → URL endpoint ThingSpeak untuk mengirim data.

3. Definisi Pin
```
#define TRIG_PIN 18
#define ECHO_PIN 5
#define LED_PIN 25
#define BUZZER_PIN 26
```
- TRIG_PIN → pin ESP32 untuk trigger sensor ultrasonik (HC-SR04).
- ECHO_PIN → pin ESP32 untuk menerima pantulan sinyal ultrasonik.
- LED_PIN → LED indikator.
- BUZZER_PIN → buzzer untuk alarm.

4. Fungsi setup()
```
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
```
- Serial.begin(115200) → memulai komunikasi serial dengan baudrate 115200 (untuk debugging).
- PinMode() → mengatur mode pin (INPUT/OUTPUT).
- Koneksi WiFi → mencoba menghubungkan ke jaringan WiFi hingga status WL_CONNECTED

5. Fungsi loop()
```
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

long duration = pulseIn(ECHO_PIN, HIGH);
int distance = duration * 0.034 / 2;

```
- TRIG_PIN dikirim pulsa selama 10 µs → memicu sensor untuk memancarkan gelombang ultrasonik.
- pulseIn() menghitung waktu pantulan diterima di ECHO_PIN.
- Rumus konversi waktu ke jarak:
  ```
  Jarak (cm) = (durasi × 0.034) / 2
  ```
Menampilkan hasil jarak
```
Serial.print("Jarak: ");
Serial.print(distance);
Serial.println(" cm");
```
Kontrol LED dan Buzzer
```
if (distance > 0 && distance < 10) {
  digitalWrite(LED_PIN, HIGH);   // LED nyala
  digitalWrite(BUZZER_PIN, LOW); // buzzer nyala
} else {
  digitalWrite(LED_PIN, LOW);    // LED mati
  digitalWrite(BUZZER_PIN, HIGH);// buzzer mati
}
```
Mengirim data ke ThingSpeak
```
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
```
Membuat URL untuk request HTTP Get ke ThingSpeak 
```
http://api.thingspeak.com/update?api_key=API_KEY&field1=distance
```
Delay antar update
```
delay(15000); // ThingSpeak limit: 15 detik per update
```

## Dokumentasi
![WhatsApp Image 2025-10-03 at 12 43 07_322eed15](https://github.com/user-attachments/assets/e0554e59-3840-4b8a-81b5-8a5f9c892a45)


https://github.com/user-attachments/assets/1116f03b-ea49-4737-9b92-6ac8a59d5923




