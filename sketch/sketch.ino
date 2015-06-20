#define ALARM 12
#define LED 13
String authorizedID = "190077761F07";
String incomingData;   // for incoming serial data
char stream;

void setup() {
  Serial1.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  pinMode(ALARM, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop() {

  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    stream = Serial1.read();
    incomingData += stream;
  }

  if (incomingData.length() > 13) {
    Serial.print("I received: ");
    Serial.println(incomingData);
    if(incomingData.indexOf(authorizedID)>=0){
      Serial.println("Starting Alarm!!!");
      alarm();
      //delay(5000);
      Serial.println("Stopped Alarm");
    }
    incomingData = "";
    flushReceive(); //Serial1.flush();
  }
}

void soundAlarm(){
  float sinVal;
  int toneVal;

  for (int i=0; i<30; i++) {
    for (int x=0; x<180; x++) {
      // convert degrees to radians then obtain sin value
      sinVal = (sin(x*(3.1412/180)));
      // generate a frequency from the sin value
      toneVal = 2000+(int(sinVal*1000));
      tone(ALARM, toneVal);
    }
    delay(100);
  }

  noTone(ALARM);
}

void lightAlarm() {
  for(int i=0; i<15; i++){
    digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
    delay(100);              // wait for a second
  }
}

void alarm(){
  float sinVal;
  int toneVal;

  for (int i=0; i<30; i++) {
    for (int x=0; x<180; x++) {
      // convert degrees to radians then obtain sin value
      sinVal = (sin(x*(3.1412/180)));
      // generate a frequency from the sin value
      toneVal = 2000+(int(sinVal*1000));
      tone(ALARM, toneVal);
    }
    digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  }

  noTone(ALARM);
}

void flushReceive(){
  while(Serial1.available()){ Serial1.read(); }
}

