#include "AlaLedRgb.h"

// setup

AlaLedRgb rgbStrip;

int ledPin = 1;
int piezoPin = 11;
int ledCount = 32;

//CUSTOM PALLETTE
AlaColor orangegrad_[6] = { 0x332100, 0x110800, 0x0a0600, 0x030200, 0x010000, 0x000000 };
AlaPalette orangegrad = { 6, orangegrad_ };

AlaColor redgrad_[6] = { 0xff0000, 0xaa0000, 0x660000, 0x220000, 0x110000, 0x000000 };
AlaPalette redgrad = { 6, redgrad_ };

AlaColor purplegrad_[6] = { 0xff00ff, 0xee00cc, 0xcc00aa, 0xaa0088, 0x880066, 0x660044 };
AlaPalette purplegrad = { 6, purplegrad_ };

AlaColor greengrad_[6] = { 0x00ff00, 0x00cc00, 0x00aa00, 0x008800, 0x006600, 0x004400 };
AlaPalette greengrad = { 6, greengrad_ };


// custom fx
AlaSeq pulseSeq1[] = {
  {ALA_FADECOLORS, 200, 200,redgrad},
  { ALA_ENDSEQ,0,0 }
};
AlaSeq pulseSeq2[] = {
  {ALA_FADECOLORS, 200, 200,purplegrad},
  { ALA_ENDSEQ,0,0 }
};
AlaSeq pulseSeq3[] = {
  {ALA_FADECOLORS, 200, 200,orangegrad},
  { ALA_ENDSEQ,0,0 }
};
AlaSeq rainbowSeq[] = {
  {ALA_FADECOLORSLOOP, 2000, 2000,alaPalRainbow},
  { ALA_ENDSEQ,0,0 }
};

AlaSeq seq[] =
{
  { ALA_SPARKLE, 2000, 2000, alaPalRainbow },
  { ALA_SPARKLE2, 2000, 2000, alaPalRainbow },
  //{ ALA_FADECOLORSLOOP, 2000, 2000, alaPalRainbow },  
  { ALA_COMETCOL, 2000, 2000, alaPalRainbow },
  { ALA_BUBBLES, 2000, 2000, alaPalRainbow },
  { ALA_ENDSEQ,0,0 }
};

// classes
class Piezo {
    const byte pin;
    int state;
    int sensorValue; 
    unsigned long buttonDownMs;

  public:
    Piezo(byte attachTo) :
      pin(attachTo)
    {
    }


    void setup() {
      state = LOW;
    }

    void loop() {
      sensorValue = analogRead(pin);
      if (sensorValue >=80) {
        buttonDownMs = millis();
        state = HIGH;
        // low hit
        if(sensorValue < 230){
          rgbStrip.setBrightness(0xaaaaaa);
          rgbStrip.setAnimation(pulseSeq3);
        }
        // medium hit
        else if(sensorValue < 270){
          rgbStrip.setBrightness(0xaaaaaa);
          rgbStrip.setAnimation(pulseSeq1);
        }
        // full hit
        else{
          rgbStrip.setBrightness(0xaaaaaa);
          rgbStrip.setAnimation(pulseSeq2);
        }
      }
      // it's been below threshold
      else if (state == HIGH) {
        if (millis() - buttonDownMs < 150) {
          // ignore this for debounce
        }
        else  {
          rgbStrip.setBrightness(0x111111);
          //rgbStrip.setAnimation(rainbowSeq);
          rgbStrip.setAnimation(seq);
          state = LOW;
        }
      }
      else{
        }
    }
};

Piezo piezo(piezoPin);

void setup() {
  piezo.setup();
  // put your setup code here, to run once:
  rgbStrip.initWS2812(ledCount, ledPin);
  rgbStrip.setBrightness(0x111111);
  rgbStrip.setAnimation(seq);
  //rgbStrip.setAnimation(ALA_FADECOLORSLOOP, 3000,alaPalRainbow);
   pinMode(ledPin, OUTPUT);
}

void loop() {

  piezo.loop();
  // put your main code here, to run repeatedly:
  rgbStrip.runAnimation();
}



