#include <Arduino.h>
#include <adafruit_NeoPixel.h>
#include <adafruit_ssd1306.h>
#include <Wire.h>
//LED setup
#define LED_pin_row1 0
#define LED_pin_row2 1
#define LED_pin_row3 2
#define LEDS_Per_row 10
#define LED_TYPE NEO_GRB

//Slider setup
#define Slider_1 3
#define Slider_2 4
#define Slider_3 5

//OLED config
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define Oled_ADDR 0x03c

// LED Strips for each row that exists
Adafruit_NeoPixel ledRow1(LEDS_Per_row, LED_pin_row1, LED_TYPE + NEO_KHZ800);
Adafruit_NeoPixel ledRow2(LEDS_Per_row, LED_pin_row2, LED_TYPE + NEO_KHZ800);
Adafruit_NeoPixel ledRow3(LEDS_Per_row, LED_pin_row3, LED_TYPE + NEO_KHZ800);

// OLED display
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

void SetRowLEDCount(int row, int count, uint32_t color);
void setRowColor(int row, uint32_t color);
void setRowBrightness(int row, uint8_t brightness);
void updateAllLEDS();
uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b);
int readSlider1();
int readSlider2();
int readSlider3();


void SetRowLEDCount(int row, int count, uint32_t color) {
  if (count < 0) count = 0;
  if (count > LEDS_Per_row) count = LEDS_Per_row;

  if (row == 1) {
    for (int i=0; i < LEDS_Per_row; i++) {
      if (i < count) {
        ledRow1.setPixelColor(i, color);
      } else {
        ledRow1.setPixelColor(i, 0); // Turn off the remaining LEDs
      }
    }
  }
  else if (row == 2) {
    for (int i=0; i < LEDS_Per_row; i++) {
      if (i < count) {
        ledRow2.setPixelColor(i, color);
      } else {
        ledRow2.setPixelColor(i, 0); // Turn off the remaining LEDs
      }
    }
  }
  else if (row == 3) {
    for (int i=0; i < LEDS_Per_row; i++) {
      if (i < count) {
        ledRow3.setPixelColor(i, color);
      } else {
        ledRow3.setPixelColor(i, 0); // Turn off the remaining LEDs
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

  if (!display.begin(SSD1306_SWITCHCAPVCC, Oled_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("spectre ready!");
  display.display();
  pinMode(Slider_1, INPUT);
  pinMode(Slider_2, INPUT);
  pinMode(Slider_3, INPUT);
  Serial.println("Setup Complete");
  
}

void loop() {

  int slider1 = readSlider1();
  int slider2 = readSlider2();
  int slider3 = readSlider3();
  //maps the slider values to the brighness, a littel test but close enough to waht i really want
  int ledCount1 = map(slider1, 0, 4095, 0, LEDS_Per_row ); // Adjust the mapping range as needed
  int ledCount2 = map(slider2,0, 4095, 0, LEDS_Per_row);// Adjust the mapping range as needed
  int ledCount3 = map(slider3,0, 4095, 0, LEDS_Per_row);// Adjust the mapping range as needed

  SetRowLEDCount(1, ledCount1, makeColor(255,0,0));
  SetRowLEDCount(2, ledCount2, makeColor(0,255,0));
  SetRowLEDCount(3, ledCount3, makeColor(0,0,255));
  updateAllLEDS();


  //display on the OLED screen the values of the sliders
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Audio Mixer:");
  display.println("slider 1: " + String(slider1));
  display.println("slider 2: " + String(slider2));
  display.println("slider 3: " + String(slider3));
  display.display();

  delay(200);
}

void setRowColor(int row, uint32_t color) {
  if (row == 1 ) {
    for (int i=0; i< LEDS_Per_row; i++) {
      ledRow1.setPixelColor(i, color);
    }
  }
 else if (row ==2 ){
   for (int i=0; i< LEDS_Per_row; i++) {
      ledRow2.setPixelColor(i, color);
   }
 }
 else if (row ==3 ) {
    for (int i=0; i<LEDS_Per_row; i++) {
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
  return analogRead(Slider_1);
}
int readSlider2() {
  return analogRead(Slider_2);
}
int readSlider3() {
  return analogRead(Slider_3);

}