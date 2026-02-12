#include <U8g2lib.h>
#include <Wire.h>
#include <DHT.h>

// =====================
// PIN DEFINITIONS
// =====================
#define MQ2_PIN 34
#define DHTPIN 4
#define DHTTYPE DHT11

// =====================
// OBJECTS
// =====================
DHT dht(DHTPIN, DHTTYPE);

// For 1.3" OLED (SH1106 – MOST COMMON)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  /* reset=*/ U8X8_PIN_NONE
);

// =====================
// SETUP
// =====================
void setup() {
  dht.begin();

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(18, 35, "ESP32 AIR MONITOR");
  u8g2.sendBuffer();
  delay(2000);
}

// =====================
// DRAW GAS BAR FUNCTION
// =====================
void drawBar(int x, int y, int w, int h, int percent) {
  percent = constrain(percent, 0, 100);
  int fill = map(percent, 0, 100, 0, w - 2);
  u8g2.drawFrame(x, y, w, h);
  u8g2.drawBox(x + 1, y + 1, fill, h - 2);
}

// =====================
// LOOP
// =====================
void loop() {

  // Read sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasRaw = analogRead(MQ2_PIN);
  int gasPercent = map(gasRaw, 0, 4095, 0, 100);

  // Status logic
  const char* status;
  if (gasPercent < 30) status = "SAFE";
  else if (gasPercent < 60) status = "WARNING";
  else status = "DANGER";

  // =====================
  // DISPLAY
  // =====================
  u8g2.clearBuffer();

  // Title
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, 10, "GAS LEVEL");

  // Gas bar
  drawBar(0, 14, 64, 10, gasPercent);

  // Big percentage
  u8g2.setFont(u8g2_font_logisoso24_tf);
  char buf[8];
  sprintf(buf, "%d%%", gasPercent);
  u8g2.drawStr(70, 34, buf);

  // Temperature & Humidity
  u8g2.setFont(u8g2_font_5x8_tf);
  char th[32];
  sprintf(th, "T: %.1fC  H: %.1f%%", temperature, humidity);
  u8g2.drawStr(0, 44, th);

  // Status (BIG & CLEAR)
  u8g2.setFont(u8g2_font_logisoso18_tf);

  // Blink when danger
  if (gasPercent >= 60) {
    if (millis() % 600 < 300)
      u8g2.drawStr(18, 64, status);
  } else {
    u8g2.drawStr(18, 64, status);
  }

  u8g2.sendBuffer();
  delay(1500);
}
