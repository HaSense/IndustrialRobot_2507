int pinGnd = 4;
int pinVcc = 3;
int pinTilt = 2;

void setup() {
  
  Serial.begin(115200);
  pinMode(pinGnd, OUTPUT);
  pinMode(pinVcc, OUTPUT);
  pinMode(pinTilt, INPUT);
  digitalWrite(pinGnd, LOW);
  digitalWrite(pinVcc, HIGH);
}

void loop() {
  //기울기 위치는 센서에 따라 변경하라
  if(!digitalRead(pinTilt)){
    Serial.println("Tilt OFF"); // LOW 
  }else{
    Serial.println("Tilt ON"); // HIGH
  }
  delay(500);
}
