#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;
MPU6050 sensor2(0x69);

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

int ax2, ay2, az2;
int gx2, gy2, gz2;

// EMG
int EMGPin_vm = A0;
int EMGVal_vm = 0;
int EMGPin_gc = A1;
int EMGVal_gc = 0;

// Variables para integrar
long tiempo_prev;
float dt;
float ang_x, ang_y, ang_z;
float ang_x_prev, ang_y_prev, ang_z_prev;

long tiempo_prev2;
float dt2;
float ang_x2, ang_y2, ang_z2;
float ang_x_prev2, ang_y_prev2, ang_z_prev2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);    //Iniciando puerto serial
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor
  sensor2.initialize();
  /*
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  if (sensor2.testConnection()) Serial.println("Sensor2 iniciado correctamente");
  else Serial.println("Error al iniciar el sensor2");
  */
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);

  sensor2.getAcceleration(&ax2, &ay2, &az2);
  sensor2.getRotation(&gx2, &gy2, &gz2);

  dt = (millis()-tiempo_prev)/1000.0;
  tiempo_prev=millis();

  dt2 = (millis()-tiempo_prev2)/1000.0;
  tiempo_prev2=millis();

  // Calcular los ángulos con acelerometro
  float accel_ang_x=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(-ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_z=atan(az/sqrt(pow(ax,2)+pow(ay,2)))*(180.0/3.14);

  float accel_ang_x2=atan(ay2/sqrt(pow(ax2,2) + pow(az2,2)))*(180.0/3.14);
  float accel_ang_y2=atan(-ax2/sqrt(pow(ay2,2) + pow(az2,2)))*(180.0/3.14);
  float accel_ang_z2=atan(az2/sqrt(pow(ax2,2)+pow(ay2,2)))*(180.0/3.14);
  
  //Calcular angulo de rotación con giroscopio y filtro complemento  
  ang_x = 0.98*(ang_x_prev+(gx/131)*dt) + 0.02*accel_ang_x;
  ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*accel_ang_y;
  ang_z = 0.98*(ang_z_prev+(gz/131)*dt) + 0.02*accel_ang_z;

  
  //Calcular angulo de rotación con giroscopio y filtro complemento  
  ang_x2 = 0.98*(ang_x_prev2+(gx2/131)*dt) + 0.02*accel_ang_x2;
  ang_y2 = 0.98*(ang_y_prev2+(gy2/131)*dt) + 0.02*accel_ang_y2;
  ang_z2 = 0.98*(ang_z_prev2+(gz2/131)*dt) + 0.02*accel_ang_z2;
  
  ang_x_prev=ang_x;
  ang_y_prev=ang_y;
  ang_z_prev=ang_z;

  ang_x_prev2=ang_x2;
  ang_y_prev2=ang_y2;
  ang_z_prev2=ang_z2;

  //Mostrar los angulos separadas por un [tab]

  //Serial.print("Rotacion en X:  ");
  //Serial.print(ang_x); Serial.print(",");
  //Serial.print("Rotacion en Y: ");
  //Serial.print(ang_y); Serial.print(",");
  //Serial.print("Rotacion en Z: ");
  //Serial.print(ang_z); Serial.print(",");

  //Serial.print("Rotacion en X2:  ");
  //Serial.print(ang_x2); Serial.print(",");
  // Serial.print("Rotacion en Y2: ");
  //Serial.print(ang_y2); Serial.print(",");
  //Serial.print("Rotacion en Z2: ");
  Serial.print(ang_z2-ang_z); Serial.print(",");

  
  EMGVal_vm = analogRead(EMGPin_vm);
  EMGVal_gc = analogRead(EMGPin_gc);

  //Mostrar las lecturas separadas por un [tab]
  //Serial.print("EMG");
  Serial.print(EMGVal_vm); Serial.print(",");
  Serial.print(EMGVal_gc); 
  Serial.println("\n");
  delay(1);
}
