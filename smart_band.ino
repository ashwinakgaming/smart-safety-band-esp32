#include <Wire.h>
#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "MPU6050.h"
// ---------- GPS ----------
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
// ---------- MPU6050 ----------
MPU6050 mpu;
// ---------- Pins ----------
#define TOUCH_PIN 4 // change if needed
// ---------- Variables ----------
float ax, ay, az;
bool fallDetected = false;
// ---------- Setup ----------
void setup() {
 Serial.begin(115200);
 // GPS setup (UART1)
 gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX
 // MPU6050 setup
 Wire.begin(21, 22); // SDA, SCL
 mpu.initialize();
 if (!mpu.testConnection()) {
 Serial.println("MPU6050 not connected!");
 } else {
 Serial.println("MPU6050 connected");
 }
 pinMode(TOUCH_PIN, INPUT);
 Serial.println("System Ready...");
}
// ---------- Loop ----------
void loop() {
 // ===== GPS Reading =====
 while (gpsSerial.available()) {
 gps.encode(gpsSerial.read());
 }
 if (gps.location.isUpdated()) {
 Serial.print("Latitude: ");
 Serial.println(gps.location.lat(), 6);
 Serial.print("Longitude: ");
 Serial.println(gps.location.lng(), 6);
 }
 // ===== MPU6050 Reading =====
 int16_t ax_raw, ay_raw, az_raw;
 mpu.getAcceleration(&ax_raw, &ay_raw, &az_raw);
 ax = ax_raw / 16384.0;
 ay = ay_raw / 16384.0;
 az = az_raw / 16384.0;
 float totalAcc = sqrt(ax * ax + ay * ay + az * az);
 // ===== Fall Detection Logic =====
 if (totalAcc > 2.5) { // threshold (tune if needed)
 fallDetected = true;
 Serial.println("⚠ FALL DETECTED!");
 }
 // ===== Touch SOS =====
 if (digitalRead(TOUCH_PIN) == HIGH) {
 Serial.println(" SOS TRIGGERED!");
 }
 delay(500);
}