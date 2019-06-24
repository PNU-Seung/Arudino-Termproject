String BTStr = "";



void BTCommunication()
{
  while (Serial3.available()) {       
    char myChar = Serial3.read();
    BTStr += myChar;
    delay(5); //수신 문자열 끊김 방지
  }
  if (!BTStr.equals("")) {
    Serial.println("Entered String -> "+BTStr);
    BTStr = "";
  }
  while (Serial.available()) {
    char myChar = Serial.read();
    Serial3.write(myChar);  //시리얼 모니터 내용을 블루추스 측에 WRITE
    Serial.write(myChar);
  }
}
