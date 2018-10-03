# LED 16x16 Grid
256 WS2812B LEDs in a 16x16 grid, controlled by FastLED to display images and animations. This repository holds everything from inspiration, to hardware planning and Arduino Sketchbooks.

---
## Goal & Idea
Being a Frontend developer I always wanted to get in touch with electronics. I started learning electronics from scratch without any Arduino involved using the book *Make: Electronics*. I'm far from being done with this book but I got the idea of building a 16x16 LED grid when watching some random videos about the hardware. Well and what's better to learn than an actual project? So I shifted from the book to this project.

It's about building a 16x16 LED grid to display pixel images and animations.

## Prototype
First steps was to create a prototype of my LED matrix. I bought 5m of WS2812B with a density of 60 LEDs per meter. The full stripe contains 300 LEDS. Together with them I purchased solderless adapter to connect the rows of LEDs in order to create my matrix.

### 1. LED + Solderless Connector
1. Cut 16 rows each containing 16 LEDs.
2. Connect them by using the solderless connectors
3. Win
4. No Fail. Connection was good for most of them but I got like 6 or 7 connection problems. Sometimes the data pin, sometimes the voltage. I switched back and forth adapters and led strips. My goal was to create a quick prototype but these connectors got in my way. So I changed my plans.

### 2. LED + soldering wires (solid core)
Next Idea will help me getting better in soldering (which I'm still learning as part of my electronics journey ). I want to solder three wires (solid not stranded) to connect each contact of a row with the next row. That sounds professional and it's a lot of work. That's the way to go. I prepar all rows, solder wires to the copper contacts and connect them with the next row. Then I carefully tested each row. Looks good!

The problems came soon after. My choice of using solid core wires apparently caused the copper contacts to get lifted from the strip. The solid core wires put an enormous tension on the contacts while I was moving and turning the stripes. Now I had far more problems than in the first round. I had to debug all stripes and when I found one problem I had to decide if the soldering was the problem or if the copper contact was ripped away. When the copper contact was loose I cut one LED away and soldered another single LED to the existing stripe by soldering all three copper pads together.

The more I interacted with this setup the more defective pads I got. I gave up.

### 3. LED + JST connectors.
Next round: Switch over to stranded wire for more flexibility and use JST connectors to easily connect and disconnect each row in case I have problems during assembly. I desoldered all solid core wires from the last try. Cleaned the pads from the remaining solder and started applying solder to connect the wires from the JST adapters.
This worked flawless. Not a single problem during assembly and now I hold a first prototype in my hands mounted on a large sheet of Plexiglas.

After that I ordered a laser cut plexiglas with 16x16 holes to make a solid base where I can put the actual pixel grid to separate the different LEDS on top later. Creatign the template for this was an interesting journey. I chose to use JavaScript to create the SVG and this worked pretty nice (hardware/lasercut-template). The actual linens are so fine that you nearly can't see them.

Now I'm ready to finally begin tinkering with code.

## Coding
I use the FastLED library and currently I'm reading the docs the get all basics right. My goal is to draw an image from a byte array. I have example code for this (examples/brainy-bits) but I want to write it myself.

Start with cloning the FastLED sources just to have them at hand when you want to look up something. For examples I wanted to know what keywords like NEOPIXEL and WS2812B are available and what's there difference. Looks like both types mentioned are creating a `WS2812Controller800Khz`. NEOPIXEL is the marketing name for Adafruit's WS2812B stripes so they are the same of course.

I tried the hello world to display a red led. It was green. RGB is mixed up for my LEDs. I quickly looked into the data sheet (well the ebay description) and it's mentioned

> Color Ordering: GRB

+ Wrote a converter (tools/hex) in JS to get a hex array from any image
+ Tested with some 16x16 shapes I saved from the web and also with custom shapes from Photoshop
+ Got a mess of pixels. My converter needed to reverse of every second row. Now it looks good.
+ tested an animation by iterating over multiples images with ha delay
+ Tested a logo. The background is dark but it's pretty light in the matrix compared to the other parts. Trying to convert to HSV (spectrum vs rgb conversion)
+ Lowered max brightness to 100, some blue values are red now. Guess `value` translates directly to LED brightness. Which makes totally sense. So back to max 255.
+ Rewrote my web application to output hsv values in hex format. I parse them back to single HSV values before passing to FastLED. Looks so much better, because darker colors have darker lights.
+ Just bought Pixen 4 as I want to try an animation. Did this for an hour.
+ Found Glediator. COM Ports missing. Quickly found out about RXTX. All the manuals telling you to put the files in ~/Library/Java/Extensions are wrong. Just put the files in the same folder as Glediator. Start with `java -jar Glediator_V2.jar`. If you see an error like "java.lang.UnsatisfiedLinkError:" the file `librxtxSerial.jnilib` is misisng or wrong for your architecture. Mien was wrong (http://rxtx.qbang.org/wiki/index.php/Download) got working ones from http://jlog.org/rxtx-mac.html.
+ Rabbit hole: Reading about the UART baud rate prescaler as under OSX librxtxSerial.jnilib can't provide a baud rate of 1.000.000 as it's configured in the Glediator Arduino sketch (the receibing "firmware").
+ To get something else done: I will work on getting my Teensy 3.2 connected to a breadboard. I'm going to solder header pins to it and will test the SD Card board I bought with it.
+ Now install Teensyduino to get board support in Arduino.
+ Yeah just run SD Card Info example and got all infos in the Serial Monitor together with: *Initializing SD card...Wiring is correct and a card is present.*
+ Back to topic: Loading BPM to use them with fastled. I leave my Teensy 3 connected.
+ Looking to sources of GameFrame the commercial 16x16 led box. It's using SdFat together with a custom BPM parser
+ gfx2gf to convert a sample image and put it on the formated SD Card.
## Resources
+ https://www.amazon.com/Make-Electronics-Learning-Through-Discovery/dp/1680450263
+ https://www.brainy-bits.com/arduino-16x16-matrix-frame/
+ https://ledseq.com/product/game-frame/
+ https://github.com/FastLED/FastLED/wiki/Basic-usage
+ [ WS2812B 5050 SMD ](https://www.ebay.de/itm/222192610445)
+ [ 3Pin-Solderless-DIY ](https://www.ebay.de/itm/222544533606)
+ [ JST 3pin connectors ](https://www.ebay.de/itm/362437551331)
+ https://www.pjrc.com/teensy/td_download.html
+ https://github.com/Jerware/GameFrame