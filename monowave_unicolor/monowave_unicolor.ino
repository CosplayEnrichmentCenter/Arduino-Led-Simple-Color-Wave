#include <Adafruit_NeoPixel.h>
#include <FastLED.h>


/********BASIC SETTINGS********/

// the data pin for the NeoPixels
#define PIN 6

// How many LEDs we will be using, charge according to your needs
#define NB_PIXELS 60

//time for a full loop in milliseconds. e.g : 5000ms is 5s.
#define TIME_LOOP 7500

//size of the wave
//e.g : NB_PIXELS to have a wave of the size of your LED strip that you already defined
//20 (or anything else) to have a shorter wave than the length of your LED strip
#define waveLength 20

/*
 * Define the color of the wave. You can use RGB, HEX or HSV color.
 * RGB values must be between 0 & 255.
 * e.g for red color:
 * RGB : const CRGB defaultColor = CRGB(255,0,0);
 * HEX : const CRGB defaultColor = CRGB(0xFF0000);
 * HSV : const CRGB defaultColor = CHSV(0,255,255);
 */
const CRGB defaultColor = CRGB(255, 128, 0);


/********ADVANCED SETTINGS********/

/*
 * HALF_WAVE_SIZE : size ratio of the first part of the wave. Second part would be the remaining space.
 * Used to determine the size and also the easing for the start and the end of the size.
 * Must be between 0 & 1
 */
#define HALF_WAVE_SIZE 0.5

/*
 * Define the Wave look, choose one of the following choice.
 * SOFT : Smooth wave wave, recommended
 * HARD : Smooth with a deeper curve
 * LINEAR : Spike effect
 */
#define WAVE_TYPE SOFT


/******************CODE*****************/
/**************DO NOT TOUCH*************/
/*********unless you really need********/


#if WAVE_TYPE == SOFT
  #define WAVE_EASING CubicEaseInOut
#elif WAVE_TYPE == HARD
  #define WAVE_EASING QuinticEaseInOut
#else
  #define WAVE_EASING Linear
#endif

// Instatiate the NeoPixel from the ibrary
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NB_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

//time variable
unsigned long time;

//half wave size, used for a smooth effect later
const int halfWaveSize = waveLength * HALF_WAVE_SIZE ;

void setup() {
  strip.begin();  // initialize the strip
  strip.show();   // make sure it is visible
  strip.clear();  // Initialize all pixels to 'off'
  //strip.setBrightness(60);
  //Serial.begin(9600);
}

void loop() {
  renderLEDs();
}

void renderLEDs() {
  time = millis();

  for (int i = 0; i < NB_PIXELS; i++) {
    float delta = ((float)(time % TIME_LOOP) / TIME_LOOP) * NB_PIXELS;
    float v = getPixelValue(i, delta);

    int r = defaultColor.r * v;
    int g = defaultColor.g * v;
    int b = defaultColor.b * v;


    //used to prevent weird corlor variation.
    if (r == 0 && defaultColor.r > 0 && v>0)
      r = 1;
    if (g == 0 && defaultColor.g > 0 && v>0)
      g = 1;
    if (b == 0 && defaultColor.b > 0 && v>0)
      b = 1;

    strip.setPixelColor(i, r , g , b);
  }

  strip.show();

}
float getPixelValue(int index, float deltaI) {
  float position = (index - deltaI);
  if (position < 0) {
    position = position + NB_PIXELS;
  }
  if (position >= 0 && position < waveLength) {
    if (position < (float)halfWaveSize) {

      float p = position / (halfWaveSize);
      return WAVE_EASING(p);
    }
    else if (position < waveLength) {
      float p = 1 - (position - halfWaveSize) / (halfWaveSize);
      return WAVE_EASING(p);
    }
  }
  else {
    return 0;
  }
}


/******************EASING FUNCTIONS*****************/
float Linear(float p){
  return p;
}
float CubicEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 4 * p * p * p;
  }
  else
  {
    float f = ((2 * p) - 2);
    return 0.5 * f * f * f + 1;
  }
}

float QuinticEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 16 * p * p * p * p * p;
  }
  else
  {
    float f = ((2 * p) - 2);
    return  0.5 * f * f * f * f * f + 1;
  }
}
