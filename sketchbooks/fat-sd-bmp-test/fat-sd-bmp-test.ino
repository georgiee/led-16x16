
#include <SPI.h>
#include "SdFat.h"
#include "Finder.h"

#define SD_CS_PIN SS
#define BUTTON_NEXT_PIN 14

SdFat sd; // set filesystem
SdFile file; // set filesystem


Finder finder;

int folderIndex = 0;

boolean buttonPressed = false; // control button check
  
void setup() {
  pinMode(BUTTON_NEXT_PIN, INPUT); 
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!sd.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  findNextImage();
}

void findNextImage() {
  char folderName[9];
  int i;
  
  for (i = 0; i< 30; i++) {
    folderIndex = i;
    
    Serial.println(i);
    file.open(sd.vwd(), folderIndex, O_READ);
    file.getName(folderName, sizeof(folderName));  
    
    
    
    
    file.close();
  }    
  
}

void nextFolder() {
  if(file.isDir() && !file.isHidden()) {
      Serial.print("Found a folder: "); file.printName(&Serial);
      Serial.println();
    }
}


void loop() { 
  
  if(digitalRead(BUTTON_NEXT_PIN) == LOW && buttonPressed == false) {
    Serial.println("button pressed");
    buttonPressed = true;
    findNextImage();
    
  } else if(digitalRead(BUTTON_NEXT_PIN) == HIGH && buttonPressed == true) {
    Serial.println("button released");
    buttonPressed = false;
  } 
}
