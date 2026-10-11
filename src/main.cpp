#include <Arduino.h>
#include <adafruit_NeoPixel.h>
#include <adafruit_ssd1306.h>
#include <Wire.h>

#define LED_PIN_ROW1 0
#define LED_PIN_ROW2 1
#define LED_PIN_ROW3 2
#define LEDS_PER_ROW 10
#define LED_TYPE NEO_GRB

#define SLIDER_1 3
#define SLIDER_2 4
#define SLIDER_3 5

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

struct Config {
  String slider1App;
  String slider2App;
  String slider3App;
} config = {"Chrome", "Spotify", "Discord"};

Adafruit_NeoPixel ledRow1(LEDS_PER_ROW, LED_PIN_ROW1, LED_TYPE + NEO_KHZ800);
Adafruit_NeoPixel ledRow2(LEDS_PER_ROW, LED_PIN_ROW2, LED_TYPE + NEO_KHZ800);
Adafruit_NeoPixel ledRow3(LEDS_PER_ROW, LED_PIN_ROW3, LED_TYPE + NEO_KHZ800);

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void SetRowLEDCount(int row, int count, uint32_t color);
void setRowColor(int row, uint32_t color);
void setRowBrightness(int row, uint8_t brightness);
void updateAllLEDS();
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
int readSlider1();
int readSlider2();
int readSlider3();
void parseSerialConfig(String data);
void handleSerialData();
void displayConfig();

void SetRowLEDCount(int row, int count, uint32_t color) {
  if (count < 0) count = 0;
  if (count > LEDS_PER_ROW) count = LEDS_PER_ROW;

  if (row == 1) {
    for (int i=0; i < LEDS_PER_ROW; i++) {
      if (i < count) {
        ledRow1.setPixelColor(i, color);
      } else {
        ledRow1.setPixelColor(i, 0);
      }
    }
  }
  else if (row == 2) {
    for (int i=0; i < LEDS_PER_ROW; i++) {
      if (i < count) {
        ledRow2.setPixelColor(i, color);
      } else {
        ledRow2.setPixelColor(i, 0);
      }
    }
  }
  else if (row == 3) {
    for (int i=0; i < LEDS_PER_ROW; i++) {
      if (i < count) {
        ledRow3.setPixelColor(i, color);
      } else {
        ledRow3.setPixelColor(i, 0);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ledRow1.begin();
  ledRow2.begin();
  ledRow3.begin();

  ledRow1.setBrightness(50);
  ledRow2.setBrightness(50);
  ledRow3.setBrightness(50);

  ledRow1.clear();
  ledRow2.clear();
  ledRow3.clear();
  ledRow1.show();
  ledRow2.show();
  ledRow3.show();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("spectre ready!");
  display.display();
  pinMode(SLIDER_1, INPUT);
  pinMode(SLIDER_2, INPUT);
  pinMode(SLIDER_3, INPUT);

  delay(2000);
  Serial.println("Setup Complete");
}

void loop() {
  handleSerialData();

  int slider1 = readSlider1();
  int slider2 = readSlider2();
  int slider3 = readSlider3();

  int ledCount1 = map(slider1, 0, 4095, 0, LEDS_PER_ROW);
  int ledCount2 = map(slider2, 0, 4095, 0, LEDS_PER_ROW);
  int ledCount3 = map(slider3, 0, 4095, 0, LEDS_PER_ROW);

  SetRowLEDCount(1, ledCount1, makeColor(255, 0, 0));
  SetRowLEDCount(2, ledCount2, makeColor(0, 255, 0));
  SetRowLEDCount(3, ledCount3, makeColor(0, 0, 255));
  updateAllLEDS();

  displayConfig();

  delay(200);
}

void setRowColor(int row, uint32_t color) {
  if (row == 1 ) {
    for (int i=0; i< LEDS_PER_ROW; i++) {
      ledRow1.setPixelColor(i, color);
    }
  }
  else if (row ==2 ){
    for (int i=0; i< LEDS_PER_ROW; i++) {
      ledRow2.setPixelColor(i, color);
    }
  }
  else if (row ==3 ) {
    for (int i=0; i<LEDS_PER_ROW; i++) {
      ledRow3.setPixelColor(i, color);
    }
  }
}

void SetRowBrightness(int row, uint8_t brightness) {
  if (row == 1) {
    ledRow1.setBrightness(brightness);
  }
  else if (row == 2 ) {
    ledRow2.setBrightness(brightness);
  }
  else if (row == 3) {
    ledRow3.setBrightness(brightness);
  }
}

void updateAllLEDS() {
  ledRow1.show();
  ledRow2.show();
  ledRow3.show();
}

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b){
  return ((uint32_t)r <<16) | ((uint32_t)g << 8) | b;
}

int readSlider1() {
  return analogRead(SLIDER_1);
}

int readSlider2() {
  return analogRead(SLIDER_2);
}

int readSlider3() {
  return analogRead(SLIDER_3);
}

void parseSerialConfig(String data) {
  data.trim();

  int pos1 = data.indexOf("S1:");
  int pos2 = data.indexOf("|S2:");
  int pos3 = data.indexOf("|S3:");
  int posEnd = data.length();

  if (pos1 != -1 && pos2 != -1 && pos3 != -1) {
    config.slider1App = data.substring(pos1 + 3, pos2);
    config.slider2App = data.substring(pos2 + 4, pos3);
    config.slider3App = data.substring(pos3 + 4, posEnd);

    Serial.print("Config updated: S1=");
    Serial.print(config.slider1App);
    Serial.print(" S2=");
    Serial.print(config.slider2App);
    Serial.print(" S3=");
    Serial.println(config.slider3App);
  }
}

void handleSerialData() {
  static String serialBuffer = "";

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      if (serialBuffer.length() > 0) {
        parseSerialConfig(serialBuffer);
        serialBuffer = "";
      }
    } else if (c != '\r') {
      serialBuffer += c;
    }
  }
}

void displayConfig() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("=== SPECTRE MIXER ===");
  display.println();

  display.print("S1: ");
  display.println(config.slider1App);
  display.print("S2: ");
  display.println(config.slider2App);
  display.print("S3: ");
  display.println(config.slider3App);

  display.display();
}