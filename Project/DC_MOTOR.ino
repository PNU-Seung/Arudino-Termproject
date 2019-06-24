#include <DHT.h>
#define DHTPIN 5      // DHT핀을 5번으로 정의한다(DATA핀)
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다

DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털5, dht11)
int EnableMotor = 28;
bool usingMotor = false;



void setMotor() {
  pinMode(EnableMotor, OUTPUT);
  digitalWrite(EnableMotor, LOW);
}



inline void Motor() {
  if (!usingMotor && dht.readHumidity() > 85)
    digitalWrite(EnableMotor, HIGH);
  else if (usingMotor)
    digitalWrite(EnableMotor, LOW);
}
