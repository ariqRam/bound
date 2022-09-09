#define DEVICE_ID 3
#define DEVICE_NAME "bound3" // nida
#define TOKEN "~987502_47A6hKBbFe9QAg4NIvzxzoOQ"
#define PIN 2
#define NUMPIXELS 12

#define key1 13
#define key2 15
#define key3 14
#define key4 12

// RemoteMe
#include <RemoteMe.h>
#include <RemoteMeSocketConnector.h>
// Hardware
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
// WiFi
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

RemoteMe& remoteMe = RemoteMe::getInstance(TOKEN, DEVICE_ID);

inline void setLight(int i) { remoteMe.getVariables()-> setInteger("light" , i); }
void ICACHE_RAM_ATTR checkInterrupt1();
void ICACHE_RAM_ATTR checkInterrupt3();
void ICACHE_RAM_ATTR checkInterrupt4();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const uint32_t COLOR_1 = strip.Color(102, 0, 102); // ariq
const uint32_t COLOR_2 = strip.Color(249, 1, 63); // isna
const uint32_t COLOR_3 = strip.Color(255, 80, 0); // nida
const uint32_t COLOR_4 = strip.Color(0, 255, 255); // firda
const uint32_t COLOR_5 = strip.Color(0, 200, 20); // totti
const uint32_t COLOR_6 = strip.Color(102, 1, 30); // azhar
const uint32_t COLOR_7 = strip.Color(0, 0, 255); // havidz

int light = 0;

void onLightChange(int i) {
  Serial.printf("onLightChange: i: %d\n",i);
  light = i;
  switch (light){
    case 1:
      colorWipe(COLOR_1, 50, 4);
      setLight(0);
      break;
    case 2:
      colorWipe(COLOR_2, 50, 4);
      setLight(0);
      break;
    case 3:
      colorWipe(COLOR_3, 50, 4);
      setLight(0);
      break;
    case 4:
      colorWipe(COLOR_4, 50, 4);
      setLight(0);
      break;
    case 5:
      colorWipe(COLOR_5, 50, 4);
      setLight(0);
      break;
//    case 7:
//      colorWipe(COLOR_7, 50, 4);
//      setLight(0);
//      break;
  }
}

void setup(){
  WiFiManager wifiManager;
  Serial.begin(9600);
  pinMode(key1, INPUT_PULLUP);// set pin as input
  pinMode(key2, INPUT_PULLUP);// set pin as input
  pinMode(key3, INPUT_PULLUP);// set pin as input
  pinMode(key4, INPUT_PULLUP);// set pin as input

  attachInterrupt(digitalPinToInterrupt(key1), checkInterrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(key3), checkInterrupt3, RISING);
  attachInterrupt(digitalPinToInterrupt(key4), checkInterrupt4, FALLING);


  pinMode(PIN, OUTPUT);
  // connect to Wi-Fi
  wifiManager.autoConnect("BOUND-0", "ramdhany");
  WiFi.softAPdisconnect (true);

  remoteMe.getVariables()->observeInteger("light", onLightChange);

  remoteMe.setConnector(new RemoteMeSocketConnector());
  remoteMe.sendRegisterDeviceMessage(DEVICE_NAME);

  strip.begin();
  colorWipe(strip.Color(255, 255, 255), 50, 1);
}

void loop(){
  // main loop
  remoteMe.loop();
  switch(light) {
    case 13:
      light = 0;
      Serial.println(light);
      setLight(DEVICE_ID);
      break;
    case 11:
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(171, 71, 0)); // night lamp
        strip.setBrightness(30);
        strip.show();
        delay(50);
      }
      break;
    case 0:
      for (uint16_t i=0; i<strip.numPixels(); i++){
        strip.setPixelColor(i,0);
        strip.show();
        delay(50);
      }
      break;
    case 12:
      rainbow(20);
      break;
  }
}

void colorWipe(uint32_t c, uint8_t wait, uint16_t n_loop) {
  for(uint16_t j=0; j<n_loop; j++){
  // Turning pixels on one by one
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  // Turning pixels off one by one
    for (uint16_t i=0; i<strip.numPixels(); i++){
      strip.setPixelColor(i,0);
      strip.show();
      delay(wait);
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void checkInterrupt1() {
//  setLight(DEVICE_ID);
  light = 13;
  Serial.println("interrupt1");
}

void checkInterrupt3() {
  light = light==11?0:11;
  Serial.println("interrupt3");
}

void checkInterrupt4() {
  light = light==12?0:12;
  Serial.print("interrupt4");
  Serial.println(light);
}
