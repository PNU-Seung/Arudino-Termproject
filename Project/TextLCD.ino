#include <LiquidCrystal.h>

LiquidCrystal lcd(44, 45, 46, 47, 48, 49); // RS  E  DB4 DB5 DB6 DB7
int buttonPin = 7;
int displayState = 0;



void setLCD() {
  lcd.begin(16, 2); // (column, row)
  lcd.clear();
  pinMode(buttonPin, INPUT);
}



void displayData() { // 시간, 날씨, 기온, 강수확률
  if (digitalRead(buttonPin))
    displayState++;
    
  displayState %= 4;
  lcd.clear();
  lcd.setCursor(0,0);
  if (displayState == 0) { 
    // 현재 날씨
  }
  else if (displayState == 1) { 
    // 3시간후
  }
  else if (displayState == 2) { 
    // 6시간후
  }
  else { 
    // 9시간후
  }
}
