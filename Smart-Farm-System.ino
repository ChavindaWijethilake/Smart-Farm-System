#include <DHT.h>

#define shadeOn 5
#define shadeOff 6
#define shadeSpeed 3
#define shadeValue 240
#define water 4

#define soilSensor A3
#define rainSensor A1
#define DHTPIN 7
#define DHTTYPE DHT11
int shadeStatus = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  pinMode(shadeOn, OUTPUT);
  pinMode(shadeOff, OUTPUT);
  pinMode(shadeSpeed, OUTPUT);
  pinMode(water, OUTPUT);

  digitalWrite(shadeOn, LOW);
  analogWrite(shadeSpeed, shadeValue);

  pinMode(soilSensor, INPUT);
  pinMode(rainSensor, INPUT);

  dht.begin();

  Serial.begin(115200);
}

double soil = 0.00;
double temp = 0.00;
double hum = 0.00;
int rain = 0;

void readSensorValues(){
  soil = analogRead(soilSensor);
  soil = map(soil, 450, 250, 0, 100);
  rain = analogRead(rainSensor);
  if (rain < 600){
    rain = 1;
  }
  else{
    rain = 0;
  }
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  constructString();
}

void getMoisture(){
  soil = analogRead(soilSensor);
  soil = map(soil, 450, 250, 0, 100);
}

void putShadeOn(){
  digitalWrite(shadeOn, HIGH);
  digitalWrite(shadeOff, LOW);
  delay(5500);
  digitalWrite(shadeOn, LOW);
  shadeStatus = 1;
}

void takeShadeOff(){
  digitalWrite(shadeOff, HIGH);
  digitalWrite(shadeOn, LOW);
  delay(4500);
  digitalWrite(shadeOff, LOW);
  shadeStatus = 0;
}

void constructString(){
  double valuesList[4] = {0, 0, 0, 0};

  valuesList[0] = soil;
  valuesList[1] = temp;
  valuesList[2] = hum;
  valuesList[3] = rain;

  String message = "";
  for (int i = 0; i < 4; i++){
    message += String(valuesList[i]) + ',';
  }
  Serial.write(message.c_str());
}

void loop() {
  
  readSensorValues();

  if (soil < 30){
    while (soil < 30 ){
      digitalWrite(water, HIGH);
      delay(2000);
      digitalWrite(water, LOW);
      delay(5000);
      getMoisture();
    }
  }

  if (rain == 1 and shadeStatus == 0){
    putShadeOn();
  }
  else if (rain == 0 and shadeStatus == 1){
    takeShadeOff();
  }
  
  delay(1000);
}
