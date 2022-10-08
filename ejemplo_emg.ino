int EMGPin = A1;
int EMGVal = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  EMGVal = analogRead(EMGPin);
  Serial.println(EMGVal);
  delay(100);
}
