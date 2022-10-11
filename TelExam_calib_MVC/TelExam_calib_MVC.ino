// https://blog.mdurance.eu/academia/la-importancia-de-la-maxima-contraccion-voluntaria-en-emg/
// Calibración de sensor EMG

// EMG
int EMGPin = A1;
int EMGVal = 0;

int delay_time = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);    //Iniciando puerto serial
}

void loop() {  
  EMGVal = analogRead(EMGPin);
  Serial.print(EMGVal);  Serial.println("\n");  
  delay(delay_time);
}
