#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

// ---- TFT pin names (as you requested) ----
#define tft_rst  4   // D2 / GPIO4
#define tft_cs   5   // D3 / GPIO5
#define tft_dc   6   // D4 / GPIO6
#define tft_scl  8   // D8 / GPIO8
#define tft_sda  10  // D10 / GPIO10

Adafruit_GC9A01A tft(tft_cs, tft_dc, tft_rst);

void setup() {
  Serial.begin(115200);
  delay(200);

  // SPI on ESP32-C3 (SCK, MISO unused, MOSI)
  SPI.begin(tft_scl, -1, tft_sda);

  tft.begin();
  tft.fillScreen(GC9A01A_BLACK);

  // Crosshair
  tft.drawLine(0, 120, 239, 120, GC9A01A_RED);
  tft.drawLine(120, 0, 120, 239, GC9A01A_RED);

  // Concentric circles
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
