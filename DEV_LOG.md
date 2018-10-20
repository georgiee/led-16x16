## 181020
Working on sketch `sketchbooks/bmp-animation`.

Today I had some soldering fun again as I wanted to extend wy wires from Teensy to the LED panel so I can put the panel on the floor. I also split the wires at the end so I can connect neutral and live wires on the other end of the strip which reduces flickering due to the voltage drop over the 256 LEDs.

I created some new test animations with `gfx2gf.sh` and saved them on the SD card.

Later I refactored the bmp drawer to create the LED buffer internally (accessible throught getBuffer()). This would allow me to create the drawer specific for a size of 16x16 and everything else is created internally. But I had my pointers, structures and array moment. I didn't get it working although I thought I understood the important concept of passing around an array with `(&m_LED[0]);`.

That's how it's done at the moment:

```
CRGB leds[NUM_LEDS];
BmpDrawer drawer(leds);

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, 256);

  drawer.draw();
  FastLED.show();
}

void BmpDrawer::draw() {
  //draw by accessing the led buffer
  m_LED[0] = ...
}

```
But I want it like this:

```
BmpDrawer drawer(16, 16);

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, GRB>(drawer.getBuffer(), 256);

  drawer.draw();
  FastLED.show();
}

// Drawer Class
BmpDrawer::BmpDrawer(uint16_t w, uint16_t h) {
  struct CRGB p_LED[(w * h)];
  m_LED = p_LED;
}

struct CRGB* BmpDrawer::getBuffer() {
  return (&m_LED[0]);
}

void BmpDrawer::draw() {
  //draw by accessing the led buffer
  m_LED[0] = ...
}
```

Anyway it's working with the LED buffer passing around so I keep it like this.
After that I switched from RGB to HSV in this sketch to get a better brightness
and not always the full brightness of an LED liek you get it with RGB.

Next I want some interactive controls. Buttons and the like to switch the animation, the animation speed etc. In parallel I want to think about the wooden frame I want to put around my prototype.

I ended with this tweet today. Good day!
https://twitter.com/deluxee/status/1053661674362814465

## 181015
I tried getting a nice folder based loop working with the SDFat Library. Got stuck here and there because of C. Today I wanted to see again something on my LEDs, so it's time to read some image data. My goal today: Tinker around with the bitmap reading algorithm and create your first C class to help you organizing code. That's totally over engeneering at this point as I could have written everything in my Arduinio Sketch but I really wanted to get some practice writing C classes.

I had some fun with header classes then. Also tried to use Visual Code with `g++` to test some C stuff without relying on the really bad coding experience in the Arduino Sketchbook.

After that I cherry picked some bmw reading code from the GameFrame project and output some pixel values. I also got to the point to shift around a sliding window of 16x16 so I can slide along a huge vertical bitmap which is the default format GameFrame uses to efficiently read bitmap film strips instead of creating single bitmap files which could exhaust the FAT32 max file limit. Okay that's pretty high with 65,517 but maybe there is another good reason. I simply trust it.

I also want to try loading GIF Animations. There is already a library but first keep it simple and something running. The raw bitmap format is perfect to do so.

Stopped after reading in two different frames/windows of the large bmp strip. Next task:
Refactor the code.
+ Set the file and read all metadata once (not on every draw)
+ Define the window size in before hand
+ Slide over the bitmap strip with offsets meaning multiples of the window size.
+ Output which real pixels are being processed — that should help understanding the
upside down structure of the bitmap file format.
+ Maybe write some JS code to get comfy with the BMP file format in a better coding environment (VSCode)