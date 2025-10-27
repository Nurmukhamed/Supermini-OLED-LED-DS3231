#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <WiFi.h>
#include "time.h"

#include "RTClib.h"

#include <Adafruit_NeoPixel.h>

#include <ESP32Ping.h>

#include <secrets.h>

#define SCREEN_WIDTH 128 // OLED дисплей Ширина
#define SCREEN_HEIGHT 64 // OLED дисплей Высота

const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Создаем объект RTCLib
RTC_DS3231 rtc;

// Инициализация I2C и дисплея
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define PIN 3  //Pin for the Neopixel ring

bool LEDstates[12];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

uint32_t ledColor;
int currentLED = 0;
uint8_t LEDmode = 0;

int secondTimer = 0;

String processor(const String& var) {
  if (var == "STARTCOLOR") {
    return "#00ff4c";
  }
  return String();
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void printOLED(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(message);
  display.display();
}

void initWiFi() {
  WiFi.mode(WIFI_STA);

  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);

  WiFi.setHostname(hostname);

  WiFi.begin(ssid, password);

  //https://forum.arduino.cc/t/no-wifi-connect-with-esp32-c3-super-mini/1324046/12
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  
  printOLED("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    printOLED("Connecting to WiFi ..");
    delay(1000);
    printOLED(WiFi.macAddress());
   delay(1000);
  }

  printOLED(WiFi.localIP().toString());
}

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Адрес 0x3C для 128x64 дисплея
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Бесконечный цикл при ошибке
  }
  // Очистка буфера дисплея
  display.clearDisplay();
  display.display();

  if (! rtc.begin()) {
    while (1);
  }

  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100);
  Wire.endTransmission();
   
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  initWiFi();

  strip.begin();
  strip.setBrightness(100);

}

void testServer() {
  bool success = Ping.ping(frigate_ip, 3);

  if (success) {
    // Fill along the length of the strip in various colors...
    colorWipe(strip.Color(  0, 255,   0), 20); // Green
  } else {
    // Fill along the length of the strip in various colors...
    colorWipe(strip.Color(255,   0,   0), 40); // Red
  }

  theaterChase(strip.Color(127, 127, 127), 20); // White, half brightness
  colorWipe(strip.Color(  0, 0,   0), 20); // Green
}

void loop() {
  DateTime now = rtc.now();

  // Clear the OLED display buffer
  display.clearDisplay();

  // Display the date
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Date: ");
  display.print(now.year());
  display.print("-");
  if (now.month() < 10) display.print("0");
  display.print(now.month());
  display.print("-");
  if (now.day() < 10) display.print("0");
  display.print(now.day());

  // Display the time
  display.setCursor(0, 15);
  display.print("Time: ");
  if (now.hour() < 10) display.print("0");
  display.print(now.hour());
  display.print(":");
  if (now.minute() < 10) display.print("0");
  display.print(now.minute());
  display.print(":");
  if (now.second() < 10) display.print("0");
  display.print(now.second());

  // Display the day of the week
  display.setCursor(0, 30);
  display.print("Day: ");
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);

  // Display the temperature from DS3231
  display.setCursor(0, 45);
  display.print("Temp: ");
  display.print(rtc.getTemperature());
  display.cp437(true);
  display.write(167);
  display.print("C");

  // Display everything on the OLED
  display.display();

  if (secondTimer > 60) {
    testServer();
    secondTimer = 0;
  }

  secondTimer++;
  delay(1000);

}

