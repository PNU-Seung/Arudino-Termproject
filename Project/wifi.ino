
void setWifi() {
  Serial2.print("AT+CIPSTART=\"TCP\",\"www.kma.go.kr\",80\r\n");
  while(!Serial2.available());
  Serial.println(Serial2.readString());
  Serial2.print("AT+CIPSEND=93\r\n");
  while(!Serial2.available());
  Serial.println(Serial2.readString());
  Serial2.print("GET /wid/queryDFSRSS.jsp?zone=2641058000 HTTP/1.1\r\n");
  Serial2.print("Host: www.kma.go.kr\r\n");
  Serial2.print("Connection: close\r\n\r\n");
  while(!Serial2.available());
} 
