#include <SoftwareSerial.h>;
#include <ArduinoJson.h>
#include <Servo.h>
#include <Wire.h>
int motorIzq=6;
int motorDer=5;
bool espera=0, confirmacion=0;
Servo motoIzq;
Servo motoDer;
SoftwareSerial mibt(10,11);
int controlMotorIzq(int value){
  motoIzq.write(map(value,-255,255,1000,2000));
}
int controlMotorDer(int value){
  motoDer.write(map(value,-255,255,1000,2000));
}

/*
identificador i2c
mcu 8
arduinoSensores 9
arduinoMotor maestr
arduinoPantalla 10

*/

void mensajeI2C(char mensaje, int esclavo){
  Wire.beginTransmission(esclavo);
  Wire.write(mensaje);
  Wire.endTransmission();
}

void drive(int X, int Y){
  if(X > 60 && X < 40){
    if(Y<40){ //ARRIBA
      controlMotorDer(map(Y, 52, 100, 0 , -255));
      controlMotorIzq(map(Y, 52, 100, 0 , -255));
    }
    if(Y>60){//abajo
      controlMotorDer(map(Y, 48, 0, 0 , 255));
      controlMotorIzq(map(Y, 48, 0, 0 , 255));
      
    }
  }
  if(Y > 60 && Y < 40){
    if(X<40){ //Izquierda
      controlMotorDer(map(X, 48, 0, 0 , 255));
      controlMotorIzq(map(X, 48, 0, 0 , -255));
      }
      if(X>60){//derecha
      controlMotorDer(map(X, 52, 100, 0 , -255));
      controlMotorIzq(map(X, 52, 100, 0 , 255));
      }
  }
  else {	
    controlMotorIzq(0);	
    controlMotorDer(0);
  } 
}

void setup() 
{
  //iniciador
  Serial.begin(9600); 
  Wire.begin();        
  Serial.println("Serialprendido");
  mibt.begin(38400);
  //definir pines

  //motores
  pinMode(motorIzq, OUTPUT);
  pinMode(motorDer, OUTPUT);   
  motoIzq.attach(motorIzq);
  motoDer.attach(motorDer);
  delay(1000);
  if(mibt.available()){
    mensajeI2C(mibt.read(),8);
  }
  int i=0;
  char lectura;
  do{
    Serial.println("Esperando confirmacion");
    Serial.print("Contadore: ");
    Serial.println(i++);
    Wire.requestFrom(2, 6);    
    while(Wire.available()) {
      lectura=Wire.read();
    }
    if(lectura==1){
      espera=1;
    }
  }while(!espera);
}
void loop()
{
  if(mibt.available()){
    if(espera){// si se confirma los datos adafruit
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, mibt.readStringUntil('?'));
      int X= doc["x"];
      int Y= map(doc["y"],100,0,0,100);
      drive(X,Y);
    }
    else{
      Serial.println("No confirmacion");
    }
  }
}


