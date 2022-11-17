int EMGPin = A0;
// int EMGPin1 = A2;
int EMGVal = 0;
// int EMGVal1 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  EMGVal = analogRead(EMGPin);
  // EMGVal1 = analogRead(EMGPin1);
  // Serial.print("Gastrocnemio Vasto medial\t");
  // Serial.print(EMGVal1); Serial.print("\t");
  Serial.println(EMGVal);
  delay(1);
}
