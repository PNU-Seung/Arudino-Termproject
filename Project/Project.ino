bool isLEDblinking, wasLEDblinking;

// 날씨 정보용 변수
  String setLocation = "/wid/queryDFSRSS.jsp?zone=2641058000";
  int index = 0;
  String temp;
  String wfEn;
  String reh;
  String tmp_str;

class WeatherInfo{
  public:
    int hour; //12, 15..
    float temp; //24.0 19.0,,
    bool rain;  //1이면 비옴, 0이면 x
    String SkyInfo; //Cloudy, Mostly Cloudy, Clear
    int Probability;
};
void updateWeather(String location);
WeatherInfo weather_forecast[10];

void setup() {
  Serial.begin(9600);   //시리얼모니터
  Serial2.begin(9600);  //WIFI 모듈
  Serial3.begin(9600);  //블루투스 시리얼
  
  delay(500);
  updateWeather(setLocation);

  
  setMotor();
  setLEDandUSsensor();
  setLoadcell();
  setRTC();
  setLCD();
  //setWifi();
  Serial.println("");
}

void loop() {
  //기상청에서 날씨 불러 오기
  while(Serial2.available()){
    String line = Serial2.readStringUntil('\n');
    //시간
    int temp11= line.indexOf("</hour>");
    if(temp11>0) {
      String tmp_str="<hour>";
      String wt_hour = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp11);
      if(wt_hour.length() == 1)
        Serial.print("0");
      Serial.print(wt_hour);
      Serial.print(":00  ");
      if(index<10)
        weather_forecast[index].hour = wt_hour.toInt();
    }
    
    //온도
    int temp= line.indexOf("</temp>");
    if(temp>0) {
      String tmp_str="<temp>";
      String wt_temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),temp);
      Serial.print(wt_temp);
      Serial.print("C  ");
        
      if(index<10)
        weather_forecast[index].temp = wt_temp.toFloat();
    }
      
    //날씨 정보
    int wfEn= line.indexOf("</wfEn>");
    if(wfEn>0) {
      String tmp_str="<wfEn>";
      String wt_twfEn = line.substring(line.indexOf(tmp_str)+tmp_str.length(),wfEn);
      
      Serial.print(wt_twfEn);
      if(wt_twfEn == String("Cloudy"))
        Serial.print("       ");
      else if(wt_twfEn == String("Clear"))
        Serial.print("        ");
      Serial.print("  ");
      
      if(index<10)
        weather_forecast[index].SkyInfo = wt_twfEn;
    }
    //강수 상태
    int rain = line.indexOf("</pty>");
    if(rain>0) {
      String tmp_str="<pty>";
      String wt_temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),rain);
      if(wt_temp.toInt())
        Serial.print("Rain  ");
      else
        Serial.print("No Rain  ");
        if(index<10)
          weather_forecast[index].rain = wt_temp.toInt();
      }
      //강수 확률
      int probabilityRain = line.indexOf("</pop>");
      if(probabilityRain > 0) {
        String tmp_str="<pop>";
        String wt_temp = line.substring(line.indexOf(tmp_str)+tmp_str.length(),probabilityRain);
        Serial.print(wt_temp);
        Serial.println("%");
        if(index<10){
          weather_forecast[index].Probability = wt_temp.toInt();
          index++;
        }
      }
  }
  // 날씨 끝.
    
  BTCommunication();
  setSpeakerFlag();
  displayData();
  if (/*현재 강수확률 > 기준 || 3시간후 강수확률 > 기준*/true) {
    senseApproaching();
    senseUmbrella();
    if (isLEDblinking) {
      turn_onLED();
    }
  }
  Motor();
}
void updateWeather(String location){
   Serial2.print("AT+CIPSTART=\"TCP\",\"www.kma.go.kr\",80\r\n");
  while(!Serial2.available());
  Serial.println(Serial2.readString());
  Serial2.print("AT+CIPSEND=93\r\n");
  while(!Serial2.available());
  Serial.println(Serial2.readString());
  Serial2.print("GET "+ location +" HTTP/1.1\r\n");
  Serial2.print("Host: www.kma.go.kr\r\n");
  Serial2.print("Connection: close\r\n\r\n");
  while(!Serial2.available());

}
/*
 * Bluetooth -> 지역코드 할당 대도시 위주로
 */
