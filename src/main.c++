#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// WiFi Credentials
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Firebase Credentials
#define API_KEY "AIzaSyD1ysbuA3eZAv10qVjiUJCeSdpGIQ-XVT0"
#define DATABASE_URL "https://tes1-31669-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "nCfH0zulObn9Q2pJ13gXmFuMCdaRJ5uP19jqq6PH"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Sensor Pins
#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ2PIN 34
#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 32
#define BUZZER_PIN 26
#define LED_PIN 27

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

bool tutupTerbuka = false;
unsigned long waktuBuka = 0;
bool sedangTampilStatus = false;
unsigned long waktuTampilanStatus = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();
  myServo.attach(SERVO_PIN);
  myServo.write(90); // posisi awal tutup

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ2PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Smart Trash Can");
  delay(2000);
  lcd.clear();

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Terhubung!");

  // Firebase Config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET; // Tambahkan ini

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase Terhubung!");
}

void loop() {
  float humidity = dht.readHumidity();
  int mq2Value = analogRead(MQ2PIN);
  bool gasDetected = mq2Value > 1500;

  // Ultrasonic
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  // Buka tutup otomatis
  if (distance < 15 && !tutupTerbuka) {
    tutupTerbuka = true;
    myServo.write(0);
    waktuBuka = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMART TRASH CAN");
    lcd.setCursor(0, 1);
    lcd.print("TERBUKA");
    sedangTampilStatus = true;
    waktuTampilanStatus = millis();
  }

  if (tutupTerbuka && millis() - waktuBuka > 5000) {
    tutupTerbuka = false;
    myServo.write(90);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SMART TRASH CAN");
    lcd.setCursor(0, 1);
    lcd.print("TERTUTUP");
    sedangTampilStatus = true;
    waktuTampilanStatus = millis();
  }

  if (sedangTampilStatus && millis() - waktuTampilanStatus > 2000) {
    sedangTampilStatus = false;
    lcd.clear();
  }

  if (!sedangTampilStatus) {
    lcd.setCursor(0, 0);
    if (gasDetected) {
      lcd.print("GAS TERDETEKSI! ");
      tone(BUZZER_PIN, 1000);
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
      }
      noTone(BUZZER_PIN);
    } else {
      lcd.print("    GAS AMAN    ");
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN);
    }

    lcd.setCursor(0, 1);
    lcd.print("HUMIDITY: ");
    lcd.print(humidity, 1);
    lcd.print("% ");
  }

  // Serial monitor
  Serial.println("===== MONITORING =====");
  Serial.print("Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Gas: ");
  Serial.println(gasDetected ? "TERDETEKSI" : "AMAN");
  Serial.print("Nilai MQ-2: ");
  Serial.println(mq2Value);
  Serial.print("Status: ");
  Serial.println(tutupTerbuka ? "TERBUKA" : "TERTUTUP");
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("=======================\n");

  // Kirim data ke Firebase
  if (Firebase.setFloat(fbdo, "/DATA/humidity", humidity)) {
    Serial.println("Humidity updated!");
  } else {
    Serial.print("Humidity update failed: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.setInt(fbdo, "/DATA/mq2Value", mq2Value)) {
    Serial.println("MQ2 value updated!");
  } else {
    Serial.print("MQ2 update failed: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.setBool(fbdo, "/DATA/gasDetected", gasDetected)) {
    Serial.println("Gas status updated!");
  } else {
    Serial.print("Gas status update failed: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.setFloat(fbdo, "/DATA/distance", distance)) {
    Serial.println("Distance updated!");
  } else {
    Serial.print("Distance update failed: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.setBool(fbdo, "/DATA/tutupTerbuka", tutupTerbuka)) {
    Serial.println("Tutup status updated!");
  } else {
    Serial.print("Tutup status update failed: ");
    Serial.println(fbdo.errorReason());
  }

  delay(100);
}