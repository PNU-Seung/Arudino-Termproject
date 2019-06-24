/*
 * LED가 동작하는 동안 우산꽂이의 무게 변화 감지 (LEDBlink == true)
 * 무게 변화가 감지되면 LED 동작 중단시킴 (turn_offLED())
 * 무게 변화가 감지되지 않는 중 사용자가 멀어지면 부저 작동
 */

#include <pitches.h>
#include "HX711.h"

#define CLK  A0
#define DOUT  A1

HX711 scale(DOUT, CLK);
float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup
float weight_prev, weight_current;
unsigned long Loadcell_prev, Loadcell_current;
bool isUmbrellaTaken = false;
bool usingLoadcell = false;

int SpeakerPin = A3;
int melody[] = {NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_GS4, NOTE_B4, NOTE_C5, 
                NOTE_E4, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_A4};
int noteDuration = 4;
int thisNote = 0;
unsigned long Speaker_prev, Speaker_current;
bool dontUseSpeaker = false;
bool usingSpeaker = false;



void setLoadcell() {
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale...");
  scale.set_scale(calibration_factor);
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading

  Loadcell_prev = millis();
  weight_prev = weight_current = scale.get_units(); // weight(lbs)
  
  Speaker_prev = millis();
}



void senseUmbrella() {
  if (isLEDblinking || usingSpeaker) {
    Loadcell_current = millis();
    if (!usingLoadcell && Loadcell_current - Loadcell_prev > 100) {
        weight_current = scale.get_units();
        usingLoadcell = true;
    }
    if (Loadcell_current - Loadcell_prev > 200) {
      Serial.print("Weight: ");
      Serial.print(weight_current);
      if (isLEDblinking) Serial.print(" \tLED BLINKING!");
      if (usingSpeaker) Serial.print(" \tUSING SPEAKER!");
      Serial.println();
      
      Loadcell_prev = Loadcell_current;
      weight_prev = weight_current;
      usingLoadcell = false;
    }
    if (abs(weight_prev - weight_current) > 1.0) 
      if (abs(weight_prev - weight_current) < 2.0) {
        isUmbrellaTaken = true;
        turn_offLED();
      }
  }
  
  if (isUmbrellaTaken) {
    turn_offSpeaker();
    Loadcell_current = millis();
    if (Loadcell_current - Loadcell_prev > 10000) {
      Loadcell_prev = Loadcell_current;
      isUmbrellaTaken = false;
      wasLEDblinking = false;
      Serial.println("reset isUmbrellaTaken!");
    }
  }
  else if (!isLEDblinking && wasLEDblinking) {
    turn_onSpeaker();
  }
}

inline void turn_onSpeaker() {
  usingSpeaker = true;
  if (!dontUseSpeaker) {
    int noteLength = 1000/noteDuration;
    if (thisNote==8 || thisNote==12 || thisNote==16 || thisNote==26 || thisNote==30)
      noteLength *= 3;
    else if (thisNote == 34) noteLength *= 6;
    
    Speaker_current = millis();
    tone(SpeakerPin, melody[thisNote]);
    if (Speaker_current - Speaker_prev >= noteLength) {
      Speaker_prev = Speaker_current;
      thisNote++;
      usingSpeaker = false;
    }
    if (thisNote >= sizeof(melody) / sizeof(int)) {
      turn_offSpeaker();
    }
  }
}

inline void setSpeakerFlag() {
  if (/*어플에서 스피커 사용 금지시킴*/false) {
    dontUseSpeaker = true;
  }
  else {
    if (isNighttime()) {
      dontUseSpeaker = true;
    }
    else {
      dontUseSpeaker = false;
    }
  }
}

inline void turn_offSpeaker() {
  thisNote = 0;
  usingSpeaker = false;
  noTone(SpeakerPin);
}
