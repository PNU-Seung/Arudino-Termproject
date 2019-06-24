int LEDpin[10] = {30,31,32,33,34,35,36,37,38,39};
unsigned long LED_prev, LED_current;
unsigned long LEDflag_prev, LEDflag_current;
bool usingLED = false;

int trigPin = 3;
int echoPin = 2;
unsigned long US_prev, US_current;
float distance_prev, distance_current;
bool usingUS = false;



void setLEDandUSsensor() {
  for (int i=0; i<10; i++) {
    pinMode(LEDpin[i], OUTPUT);
    digitalWrite(LEDpin[i], LOW);
  }
  isLEDblinking = wasLEDblinking = false;
  LED_prev = millis();
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  distance_prev = distance_current = getDistance();
  US_prev = millis();
}



void senseApproaching() {
  US_current = millis();
  if (!usingUS && US_current - US_prev > 100) {
      distance_current = getDistance();
      usingUS = true;
  }
  if (US_current - US_prev > 200) {
    /*
    Serial.print("Distance: ");
    Serial.print(distance_current);
    if (isLEDblinking) Serial.println("   \tLED BLINKING!");
    else Serial.println();
    */
    US_prev = US_current;
    distance_prev = distance_current;
    usingUS = false;
  }
  
  if (!isLEDblinking && distance_prev - distance_current > 10) {
    if (distance_prev - distance_current < 20) {
      isLEDblinking = true;
      turn_onLED();
      turn_offSpeaker();
    }
  }
  else if (isLEDblinking && distance_current - distance_prev > 10) {
    if (distance_current - distance_prev < 60)
      turn_offLED();
  }

  if (wasLEDblinking) {
    LEDflag_current = millis();
    if (LEDflag_current - LEDflag_prev > 28000) {
      wasLEDblinking = false;
      turn_offSpeaker();
    }
  }
}

inline float getDistance() {
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  return duration * 340 / 10000 / 2;;
}

inline void turn_onLED() {
  LED_current = millis();
  if (!usingLED && LED_current - LED_prev >= 150) {
    for (int i=0; i<5; i++) {
      digitalWrite(LEDpin[2*i], HIGH);
      digitalWrite(LEDpin[2*i+1], LOW);
    }
    usingLED = true;
  }
  if (LED_current - LED_prev >= 300) {
    for (int i=0; i<5; i++) {
      digitalWrite(LEDpin[2*i], LOW);
      digitalWrite(LEDpin[2*i+1], HIGH);
    }
    usingLED = false;
    LED_prev = LED_current;
  }
}

inline void turn_offLED() {
  isLEDblinking = false;
  wasLEDblinking = true;
  for (int i=0; i<6; i++)
    digitalWrite(LEDpin[i], LOW);
    
  LEDflag_prev = millis();
}
