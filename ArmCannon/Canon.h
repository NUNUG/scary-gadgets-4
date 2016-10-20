#ifndef Canon_h
#define Canon_h

#include <Adafruit_NeoPixel.h>
typedef Adafruit_NeoPixel Pixel;

#define PIXEL_SETTINGS NEO_GRB + NEO_KHZ800

class Canon
{
protected:
  Pixel* Strips;
  int NumStrips;
  int LedsPerStrip;

  Pixel* Cannon;
  int NumCannonStrips;
  int LedsPerCannonStrip;

  void SetPixelColor(int pixel, uint32_t color)
  {
	for(int s = 0; s < NumStrips; ++s)
	  Strips[s].setPixelColor(pixel, color);
  }

  void Show()
  {
	for(int s = 0; s < NumStrips; ++s)
	  Strips[s].show();
  }
  
public:
  Canon()
  {
	
  }

  ~Canon()
  {
	delete[] Strips;
  }

  void Init(int *stripPins, int numStrips, int ledsPerStrip, int* cannonPins, int numCannonStrips, int ledsPerCannon)
  {
	NumStrips = numStrips;
	LedsPerStrip = ledsPerStrip;
	Strips = new Pixel[NumStrips];
	
	for(int i = 0; i < NumStrips; ++i)
	{
	  // Parameter 1 = number of pixels in strip
	  // Parameter 2 = Arduino pin number (most are valid)
	  // Parameter 3 = pixel type flags, add together as needed:
	  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
	  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
	  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
	  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
	  Strips[i] = Adafruit_NeoPixel(LedsPerStrip, stripPins[i], PIXEL_SETTINGS);
	  Strips[i].begin();
	  Strips[i].show();
	}

	NumCannonStrips = numCannonStrips;
	LedsPerCannonStrip = ledsPerCannon;
	Cannon = new Pixel[NumCannonStrips];

	for (int i = 0; i < NumCannonStrips; ++i)
	{
		Strips[i] = Pixel(LedsPerCannonStrip, cannonPins[i], PIXEL_SETTINGS);
	}
  }

  // Could use a switch to choose which animation to show (chase, rainbow, etc.)
  // For the switch, a bit mask like idea could work. Would need a big switch, though
  // Potentiometer will choose the color
  void ChargeCanon_Chase(unsigned int startWait, unsigned int endWait, unsigned int loops, uint32_t color)
  {
	unsigned int waitChange = (startWait - endWait) / loops;

	for(int i = 0; i < loops + 1; ++i)
	{
	  for(int q = 0; q < 3; ++q)
	  {
		for(int p = 0; p < LedsPerStrip; p += 3)
		  SetPixelColor(p + q, color);

		Show();

		delay(startWait);

		for(int p = 0; p < LedsPerStrip; ++p)
		  SetPixelColor(p + q, 0);
	  }

	  startWait -= waitChange;
	}
  }
  
  void FireCanon_ChaseRainbow()
  {
	for(int j = 0; j < 256; ++j)
	{
	  for(int q = 0; q < 3; q++)
	  {
		for(int i = 0; i < LedsPerStrip; i += 3)
		  SetPixelColor(i + q, Wheel((i + j) % 255));

		Show();
		delay(20);

		for(int i = 0; i < LedsPerStrip; i += 3)
		  SetPixelColor(i + q, 0);
	  }
	}
  }

  void FireCanon_RainbowCycle() {
	uint16_t i, j, k;
	const int numCycles = 10;
	
	for(j = 0; j < 256 * numCycles; ++j) { // 5 cycles of all colors on wheel
	  for(i=0, k = LedsPerStrip - 1; i < LedsPerStrip; ++i, --k)
		SetPixelColor(k, Wheel(((i * 256 / LedsPerStrip) + j) & 255));
	  
	  Show();
	  delay(1);
	}
  }
  
  void FireCanon()
  {
	FireCanon_RainbowCycle();
	Clear();
  }
  
  void Clear()
  {
	for(int i = 0; i < LedsPerStrip; ++i)
	  SetPixelColor(i, 0);

	Show();
  }

  uint32_t Color(int r, int g, int b)
  {
	return Strips[0].Color(r, g, b);
  }
  
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) {
	  return Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if(WheelPos < 170) {
	  WheelPos -= 85;
	  return Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
};



// Fill the dots one after the other with a color
//void colorWipe(Adafruit_NeoPixel& strip, uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}
//
//void rainbow(Adafruit_NeoPixel& strip, uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(Adafruit_NeoPixel& strip, uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
////Theatre-style crawling lights.
//void theaterChase(Adafruit_NeoPixel& strip, uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(Adafruit_NeoPixel& strip, uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel(strip, (i+j) % 255));    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (int i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
#endif
