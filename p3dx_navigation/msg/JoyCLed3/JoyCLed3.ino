#include "RGBConverter.h"
int rgbhsl[3];
int hslrgb[3];
RGBConverter convt;

//AdafruitNeopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define LED_PIN     4
#define LED_PIN2    6
#define LED_COUNT  144
#define BRIGHTNESS 50
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRBW + NEO_KHZ800);

//ros
#include <stdint.h>
#include <stdlib.h>  
#include <ros.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Joy.h>
#include "geometry_msgs/Twist.h"
#include "p3dx_navigation/AdaNeopixel.h"

uint32_t color;
int LorR = 0;
int func =0;
int brightness=0;
int speed =0;
int r=0;
int g=0;
int b=0;
int w=0;
int startPix=0;
int len = 0;
int count=0;

ros::NodeHandle  nh;

void lightLED(  const p3dx_navigation::AdaNeopixel & msg)
{    
  nh.loginfo("Things Accepted");
  LorR        = msg.ledcommand[0];
  func        = msg.ledcommand[1];
  brightness  = msg.ledcommand[6];
  speed       = msg.ledcommand[9];
  r           = msg.ledcommand[2];
  g           = msg.ledcommand[3];
  b           = msg.ledcommand[4];
  w           = msg.ledcommand[5];
  startPix    = msg.ledcommand[7];
  len         = msg.ledcommand[8];
  
  switch(func){
    case 0:
      while(!Serial.available()){
        if (LorR==2){
          pulseL(r,g,b,w,brightness,startPix,len,speed);
          }
        else if(LorR ==3){
          pulseR(r,g,b,w,brightness,startPix,len,speed);
        }
      }
      clear_all();
      break;
    case 1:
      fillAll(r,g,b,w,brightness);
      break;
    case 2:
      colorWipe(r,g,b,w,brightness);    
      break;
    case 3:
      breath(r,g,b,w,brightness,startPix,len,speed);
      break;
    case 4:
      whiteOverRainbow(brightness,speed,len);
    case 5:
      rainbowFade2White(brightness);
   }          
}

ros::Subscriber<p3dx_navigation::AdaNeopixel> sub("/Neopixel" , lightLED);

void setup()
{   
  //AdafruitNeopixel
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
      clock_prescale_set(clock_div_1);
  #endif
  strip.begin();     // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.begin();
  strip.show();      // Turn OFF all pixels ASAP
  strip2.show();
  nh.initNode();
  nh.subscribe(sub);
  delay(100);
      
}

void loop()   {
  
  nh.spinOnce();
  delay(1);
     
}


void whiteOverRainbow(int brightness, int speed, int whiteLength) {
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  set_brightness(brightness);

  while(!Serial.available()){ 
    for(int i=0; i<strip.numPixels(); i++) {  
      if(((i >= tail) && (i <= head)) ||      
        ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); 
        strip2.setPixelColor(i, strip.Color(0, 0, 0, 255));
      } 
      else{                                             
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        strip2.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
      if (Serial.available()) {     // bail out on sensor detect
            break;
      } 
    }

    show_all(); 

    firstPixelHue += 40; 
    if((millis() - lastTime) > (double)(1.0/speed)) { 
      if(++head >= strip.numPixels()) {      
        head = 0;
      }
      if(++tail >= strip.numPixels()) {      
        tail = 0;
      }
      lastTime = millis();                   
    }
  }
  clear_all();
}

// Set all the dots with a color
void fillAll(int r,int g, int b, int w ,int brightness) {
  set_brightness(brightness);
  color = strip.Color(r, g, b, w);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip2.setPixelColor(i, color);
    if (Serial.available()) {    
      break;
    } 
  }
  show_all();
  while(!Serial.available()){}
  clear_all();
}

// Fill the dots one after the other with a color
void colorWipe(int r,int g, int b, int w ,int brightness) {
  set_brightness(brightness);
  color = strip.Color(r, g, b, w);
  while(!Serial.available()){
    if(count > strip.numPixels() || count > strip2.numPixels()){
      count=0;
      clear_all();
    }
    strip.setPixelColor(count, color);
    strip2.setPixelColor(count, color);
    count += 1;
    show_all();
  }
  clear_all();
}

void breath(int r,int g,int b,int w,int brightness,int startPix,int len,int speed) {
  convt.rgbToHsl(r,g,b,rgbhsl);
  set_brightness(brightness);
  int initialbright = rgbhsl[2];

  while(!Serial.available()){

    for(int i=0; i<initialbright;i+=speed){
      color = hsl(rgbhsl[0], rgbhsl[1], i);
      convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
      if(w!=0){
        w+=speed;
      }
      if(w>255){
        w=255;
      }
      color = strip.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
      strip.fill(color, startPix, len);
      strip2.fill(color, startPix, len);
      show_all();
      if (Serial.available()) {   
        break;
      }  
    }  
  
    for(int i=initialbright; i>0;i-=speed){
      color = hsl(rgbhsl[0], rgbhsl[1], i);
      convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
      if(w!=0 && w>speed){
        w-=speed;
      }
      if(w<=1 && w!=0){
        w=1;
      }
      color = strip.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
      strip.fill(color, startPix, len);
      strip2.fill(color, startPix, len);
      show_all();
      if (Serial.available()) {    
        break;
      } 
    }
  }
  clear_all();  
}

void pulseL(int r,int g,int b,int w,int brightness,int startPix,int len,int speed) {    
  
  convt.rgbToHsl(r,g,b,rgbhsl);
  strip.setBrightness(brightness);
  int initialbright = rgbhsl[2];
 
  for(int i=0; i<initialbright;i+=speed){
    color = hsl(rgbhsl[0], rgbhsl[1], i);
    convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
    if(w!=0){
      w+=speed;
    }
    if(w>255){
      w=255;
    }
    color = strip.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
    strip.fill(color, startPix, len);
    strip.fill(color, 144-len, 144);
    strip.show();
    if (Serial.available()) {     
      break;
    }  
  }  

  for(int i=initialbright; i>0;i-=speed){
    color = hsl(rgbhsl[0], rgbhsl[1], i);
    convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
    if(w!=0 && w>speed){
      w-=speed;
    }
    if(w<=1 && w!=0){
      w=1;
    }
    color = strip.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
    strip.fill(color, startPix, len);
    strip.fill(color, 144-len, 144);
    strip.show();
    if (Serial.available()) {    
      break;
    } 
  }    
}

void pulseR(int r,int g,int b,int w,int brightness,int startPix,int len,int speed) {    
  
  convt.rgbToHsl(r,g,b,rgbhsl);
  strip2.setBrightness(brightness);
  int initialbright = rgbhsl[2];
  
  for(int i=0; i<initialbright;i+=speed){
    color = hsl(rgbhsl[0], rgbhsl[1], i);
    convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
    if(w!=0){
      w+=speed;
    }
    if(w>255){
      w=255;
    }
    color = strip2.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
    strip2.fill(color, startPix, len);
    strip2.fill(color, 144-len, 144);
    strip2.show();
    if (Serial.available()) {     
      break;
    } 
  }  

  for(int i=initialbright; i>0;i-=speed){
    color = hsl(rgbhsl[0], rgbhsl[1], i);
    convt.hslToRgb(rgbhsl[0],rgbhsl[1],i,hslrgb);
    if(w!=0 && w>speed){
      w-=speed;
    }
    if(w<=1 && w!=0){
      w=1;
    }
    color = strip2.Color(hslrgb[0], hslrgb[1], hslrgb[2], w);
    strip2.fill(color, startPix, len);
    strip2.fill(color, 144-len, 144);
    strip2.show(); 
    if (Serial.available()) {     // bail out on sensor detect
      break;
    } 
  }    
}

void rainbowFade2White(int brightness ) {
  set_brightness(brightness);
  int rainbowLoops =3;
  int fadeVal=0, fadeMax=100;

  while(!Serial.available()){
    for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
      firstPixelHue += 256) {
  
        for(int i=0; i<strip.numPixels(); i++) { 
    
          uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
            255 )));
          strip2.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
            255 )));
            if (Serial.available()) {     t
              break;
            } 
        }
        show_all();
    
        if (Serial.available()) {     
          break;
        } 
    }
  }
  clear_all();
}

void set_brightness(int brightness){
    strip.setBrightness(brightness);
    strip2.setBrightness(brightness);
}

void show_all(){
    strip.show();
    strip2.show();
}

void clear_all(){
    strip.clear();
    strip.show();
    strip2.clear();
    strip2.show();
}


uint32_t hsl(uint16_t ih, uint8_t is, uint8_t il) {

  float h, s, l, t1, t2, tr, tg, tb;
  uint8_t r, g, b;

  h = (ih % 360) / 360.0;
  s = constrain(is, 0, 100) / 100.0;
  l = constrain(il, 0, 100) / 100.0;

  if ( s == 0 ) { 
    r = g = b = 255 * l;
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  } 
  
  if ( l < 0.5 ) t1 = l * (1.0 + s);
  else t1 = l + s - l * s;
  
  t2 = 2 * l - t1;
  tr = h + 1/3.0;
  tg = h;
  tb = h - 1/3.0;

  r = hsl_convert(tr, t1, t2);
  g = hsl_convert(tg, t1, t2);
  b = hsl_convert(tb, t1, t2);

  // NeoPixel packed RGB color
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

uint8_t hsl_convert(float c, float t1, float t2) {

  if ( c < 0 ) c+=1; 
  else if ( c > 1 ) c-=1;

  if ( 6 * c < 1 ) c = t2 + ( t1 - t2 ) * 6 * c;
  else if ( 2 * c < 1 ) c = t1;
  else if ( 3 * c < 2 ) c = t2 + ( t1 - t2 ) * ( 2/3.0 - c ) * 6;
  else c = t2;
  
  return (uint8_t)(c*255); 
}
