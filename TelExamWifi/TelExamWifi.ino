#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"
//Conexión a la red wifi: nombre de la red y contraseña
#define WIFI_AP "Dpto910"
#define WIFI_PASSWORD "Bm910019"

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

void setup() {
    //Inicializamos la comunicación serial para el log
    Serial.begin(115200);
    //Iniciamos la conexión a la red WiFi
    InitWiFi();
    //Colocamos la referencia del servidor y el puerto
    client.setServer( server, 1883 );
    lastSend = 0;
    reconnectWifi();
}

void loop() {
    //Validamos si el modulo WiFi aun esta conectado a la red
    // status = WiFi.status();

    //Validamos si esta la conexión del servidor
    if(!client.connected() ) {
        //Si falla reintentamos la conexión
        reconnectClient();
    }
    String payload = "25";
    char attributes[5];
    payload.toCharArray( attributes, 5 );
    client.publish( "outTopic", attributes );
    client.loop();
    delay(10);
}

void sendDataTopic()
{
    //Serial.println("sendDataTopic");
    // Prepare a JSON payload string
    String payload = "25";

    // Send payload
    char attributes[10];
    payload.toCharArray( attributes, 10 );
    client.publish( "outTopic", attributes );
    //Serial.println( attributes );
}

//Inicializamos la conexión a la red wifi
void InitWiFi()
{
    //Inicializamos el puerto serial
    soft.begin(115200);
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
            delay( 5000 );
        }
    }
}
