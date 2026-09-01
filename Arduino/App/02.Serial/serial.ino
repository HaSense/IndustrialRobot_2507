void setup() {
  Serial.begin(115200);
}

void loop() {
 char data;
 
 if(Serial.available() > 0){
   data = Serial.read(); // 1byte read
   Serial.write(data); // 1byte send
 }
}
