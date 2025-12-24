#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

// ---- LED ----
#define LED_PIN 2   // D0 / GPIO2 / physical pin 1

// ---- TFT pin names ----
#define tft_rst  4   // D2 / GPIO4 / pin 3
#define tft_cs   5   // D3 / GPIO5 / pin 4
#define tft_dc   6   // D4 / GPIO6 / pin 5
#define tft_scl  8   // D8 / GPIO8 / pin 9
#define tft_sda  10  // D10 / GPIO10 / pin 11

Adafruit_GC9A01A tft(tft_cs, tft_dc, tft_rst);

void flashLed(uint8_t times, uint16_t delayMs) {
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}

void setup() {
  // ---- LED power-on indicator ----
  pinMode(LED_PIN, OUTPUT);
  flashLed(5, 80);   // rapid 5x blink

  Serial.begin(115200);
  delay(200);

  // ---- SPI ----
  SPI.begin(tft_scl, -1, tft_sda);

  // ---- TFT ----
  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);

  // Crosshair
  tft.drawLine(0, 120, 239, 120, GC9A01A_RED);
  tft.drawLine(120, 0, 120, 239, GC9A01A_RED);

  // Circles
  for (int r = 20; r <= 120; r += 20) {
    tft.drawCircle(120, 120, r, GC9A01A_GREEN);
  }

  // Label
  tft.setTextColor(GC9A01A_WHITE);
  tft.setTextSize(2);
  tft.setCursor(55, 110);
  tft.print("GC9A01");
}

void loop() {
  // nothing
}
