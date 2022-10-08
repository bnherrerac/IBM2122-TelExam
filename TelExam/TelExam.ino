#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

int EMGPin = A1;
int EMGVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  EMGVal = analogRead(EMGPin);

  //Mostrar las lecturas separadas por un [tab]
  //Serial.print("EMG, rot[x y z], accel_mod:\t");
  Serial.print(EMGVal);  Serial.print(",");
  Serial.print(ax/250);  Serial.print(",");
  Serial.print(ay/250);  Serial.print(",");
  Serial.print(az/250);  Serial.print(",");

  Serial.print(gx/1000); Serial.print(",");
  Serial.print(gy/1000); Serial.print(",");
  Serial.print(gz/1000); Serial.println(".");
  delay(10);
}
