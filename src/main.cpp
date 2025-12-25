#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

// 240x240 RGB565 buffer (~115 KB)
GFXcanvas16 canvas(240, 240);


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
  // ---- LED sanity blink ----
  pinMode(LED_PIN, OUTPUT);
  flashLed(5, 80);

  Serial.begin(115200);
  delay(200);

  // ---- SPI ----
  SPI.begin(tft_scl, -1, tft_sda);

  // ---- TFT ----
  tft.begin();
  tft.setRotation(0);
}

void loop() {
  static float angle = 0;
  static float pulse = 0;

  const int cx = 120;
  const int cy = 120;
  const int maxR = 118;

  // ---- draw to offscreen buffer ----
  canvas.fillScreen(GC9A01A_BLACK);

  // breathing ring (thicker so it's visible)
  int ringR = maxR - 6 + sin(pulse) * 4;
  for (int w = 0; w < 3; w++) {
    canvas.drawCircle(cx, cy, ringR - w, GC9A01A_BLUE);
  }

  // orbiting dots
  for (int i = 0; i < 3; i++) {
    float a = angle + i * TWO_PI / 3;
    int x = cx + cos(a) * (maxR - 12);
    int y = cy + sin(a) * (maxR - 12);
    canvas.fillCircle(x, y, 5, GC9A01A_GREEN);
  }

  // center dot
  canvas.fillCircle(cx, cy, 4, GC9A01A_RED);

  // ---- blit buffer to display (single transfer) ----
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 240);

  angle += 0.045;
  pulse += 0.035;

  delay(20); // ~50 FPS, very stable
}


