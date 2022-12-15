//importing of wire library to talk to accelerometer
#include <Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//library for smoothing values of accelerometer
#include <Ewma.h>
int famount = 0.1;
Ewma faX(famount);
Ewma faY(famount);
Ewma faZ(famount);
Ewma fgX(famount);
Ewma fgY(famount);
Ewma fgZ(famount);
float aX,aY,aZ,gX,gY,gZ;

//importing of Adafruit dht library and creating of dht object
#include <Adafruit_Sensor.h>
#include <DHT.h>
DHT dht(4, DHT11);

//defining pins and variables for ultrasonic distance sensor
#define echo 2
#define trig 3
long dur;
int dist;

void setup() {
  //starting and initialising accelerometer
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  
  //initialising dht sensor
  dht.begin();
  
  //opening up the serial port wit ha baudt rate of 115200
  Serial.begin(115200);
  Serial.println("Hellos");
  
  //initialising pins for ultrasonic sensor
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  digitalWrite(trig, 0);
}

void loop(){
  //reading accelerometer using I²C
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  //smoothing of values from accelerometer 
  //TODO!
  aX = faX.filter(AcX);



  //reading temperature and humidity from dht sensor
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  if(isnan(temp)||isnan(humi)) return;
  
  //measuring distance of ultrasonic sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  dur = pulseIn(echo, HIGH);
  dist = dur * 0.034 / 2;
  
  //outputing resulting values to the serial port
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);

  Serial.print("Thermohygrometer: ");
  Serial.print("T = "); Serial.print(temp);
  Serial.print("°C | H = "); Serial.print(humi);
  Serial.println("%");

  Serial.print("Ultrasonic Distance: ");
  Serial.print("D = "); Serial.print(dist);
  Serial.println("cm");

  Serial.println(" ");
  Serial.println(" ");
  
  //wait a second between cycles
  delay(1000);
}
