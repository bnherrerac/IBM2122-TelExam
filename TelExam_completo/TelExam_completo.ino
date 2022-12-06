#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <WiFiEsp.h>
#include <stdio.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"
//Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "Dpto910"
#define WIFI_PASSWORD "Bm910019"

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

//Nombre o IP del servidor mosquitto
char server[50] = "192.168.100.7";

//Inicializamos el objeto de cliente esp
WiFiEspClient espClient;

//Iniciamos el objeto subscriptor del cliente con el objeto del cliente
PubSubClient client(espClient);

//Conexión serial para el esp con una comunicación serial, pines 2: rx y 3: tx
SoftwareSerial soft(8, 9);

//Contador para el envio de datos
unsigned long lastSend;

int status = WL_IDLE_STATUS;

int cont = 0;


void setup() {
  // put your setup code here, to run once:
  //Inicializamos la comunicación serial para el log
  Serial.begin(38400);
  //Iniciamos la conexión a la red WiFi
  InitWiFi();
  //Colocamos la referencia del servidor y el puerto
  client.setServer( server, 1883 );
  lastSend = 0;
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor
  sensor2.initialize();

  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  if (sensor2.testConnection()) Serial.println("Sensor2 iniciado correctamente");
  else Serial.println("Error al iniciar el sensor2");
}

void loop() {

  status = WiFi.status();
  //Validamos si esta la conexión del servidor
  if(!client.connected() ) {
      //Si falla reintentamos la conexión
      reconnectClient();
  }

  
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

  Serial.print(ang_x); Serial.print(",");
  Serial.print(ang_y); Serial.print(",");
  Serial.print(ang_z); Serial.print(",");

  Serial.print(ang_x2); Serial.print(",");
  Serial.print(ang_y2); Serial.print(",");
  Serial.print(ang_z2); Serial.print(",");
  
  EMGVal_vm = analogRead(EMGPin_vm);
  EMGVal_gc = analogRead(EMGPin_gc);
  Serial.print(EMGVal_vm); Serial.print(",");
  Serial.print(EMGVal_gc); 
  Serial.println("\n");
  String base = ((String)EMGVal_vm + "," + (String)EMGVal_gc + "," + (String)ang_x + "," + (String)ang_y + "," + (String)ang_z + "," + (String)ang_x2 + "," + (String)ang_y2 + "," + (String)ang_z2);

  sendDataTopic(base, 8);
  /*
  sendDataTopic(EMGVal_vm, 0);
  sendDataTopic(EMGVal_gc, 1);
  
  sendDataTopic(ang_x, 2);
  sendDataTopic(ang_y, 3);
  sendDataTopic(ang_z, 4);
  
  sendDataTopic(ang_x2, 5);
  sendDataTopic(ang_y2, 6);
  sendDataTopic(ang_z2, 7);
  */  
  client.loop();
  delay(10);
}




void sendDataTopic(String msg, int option)
{
    //Serial.println("sendDataTopic");
    // Prepare a JSON payload string
    String payload = (String)msg;
    char attributes[10];
    payload.toCharArray( attributes, 10 );
    switch(option){
      case 0:
        client.publish( "EMG1", attributes );
        break;
      case 1:
        client.publish( "EMG2", attributes );
        break;
      case 2:
        client.publish( "ANGX", attributes );
        break;
      case 3:
        client.publish( "ANGY", attributes );
        break;
      case 4:
        client.publish( "ANGZ", attributes );
        break;
      case 5:
        client.publish( "ANGX2", attributes );
        break;
      case 6:
        client.publish( "ANGY2", attributes );
        break;
      case 7:
        client.publish( "ANGZ2", attributes );
        break;
      case 8:
        client.publish( "ALL", attributes);
        break;
    }
    
}

void InitWiFi()
{
    //Inicializamos el puerto serial
    soft.begin(38400);
    //Iniciamos la conexión wifi
    WiFi.init(&soft);
    //Verificamos si se pudo realizar la conexión al wifi
    //si obtenemos un error, lo mostramos por log y denememos el programa
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("El modulo WiFi no esta presente");
        while (true);
    }
    reconnectWifi();
}


void reconnectWifi() {
    Serial.println("Iniciar conección a la red WIFI");
    while(status != WL_CONNECTED) {
        Serial.print("Intentando conectarse a WPA SSID: ");
        Serial.println(WIFI_AP);
        //Conectar a red WPA/WPA2
        status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
        delay(500);
    }
    Serial.println("Conectado a la red WIFI");
}

void reconnectClient() {
    //Creamos un loop en donde intentamos hacer la conexión
    while(!client.connected()) {
        Serial.print("Conectando a: ");
        Serial.println(server);
        //Creamos una nueva cadena de conexión para el servidor
        //e intentamos realizar la conexión nueva
        //si requiere usuario y contraseña la enviamos connect(clientId, username, password)
        String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
        if(client.connect(clientId.c_str())) {
            Serial.println("[DONE]");
        } else {
            Serial.print( "[FAILED] [ rc = " );
            Serial.print( client.state() );
            Serial.println( " : retrying in 5 seconds]" );
            delay( 500000 );
        }
    }
}
