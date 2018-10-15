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