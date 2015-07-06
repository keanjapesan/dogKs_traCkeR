#define ALARM 12
#define LED 23
#define LED1 9
#define LED2 8
#define LED3 22
#define LED4 24

String dogID = "190077761F07";
String ownerID = "190025460872";
String incomingData;   // for incoming serial data
char stream;

int irSensor1Pin = A0;
int irSensor2Pin = A1;
int irSensor1Value = 0; // variable to store the value coming from the sensor
int irSensor2Value = 0; // variable to store the value coming from the sensor
int first; // 1 -> irSensor1, 2 -> irSensor2

boolean ir_sensor_1 = false;
boolean ir_sensor_2 = false;
boolean rfid_sensor = false;
boolean stop_alarm = false;

void setup() {
  Serial1.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  pinMode(ALARM, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void loop() {
  // If after some time some of the values are true but not the others, then false all of them
  // to avoid weird timing issues related to old values.

  irSensor1Value = analogRead(irSensor1Pin);
  irSensor2Value = analogRead(irSensor2Pin);
  String stringOutPut = "IR 1: " + String(irSensor1Value) + " IR 2: " + String(irSensor2Value);
  Serial.println(stringOutPut);

  if (irSensor1Value >= 970 && !ir_sensor_1) {
    ir_sensor_1 = true;
    if (ir_sensor_2 == false){ first = 1; }
    digitalWrite(LED1, HIGH);
  }
  if (irSensor2Value >= 970 && !ir_sensor_2) {
    ir_sensor_2 = true;
    if (ir_sensor_1 == false){ first = 2; }
    digitalWrite(LED2, HIGH);
  }

  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    stream = Serial1.read();
    incomingData += stream;
  }

  if (incomingData.length() > 13) {
    Serial.print("I received: ");
    Serial.println(incomingData);
    if(incomingData.indexOf(dogID)>=0 && !rfid_sensor){
      rfid_sensor = true;
      digitalWrite(LED3, HIGH);
    }else{
      if(incomingData.indexOf(ownerID)>=0 && !stop_alarm){
        stop_alarm = true;
        digitalWrite(LED4, HIGH);
      }
    }
    incomingData = "";
    flushReceive();
  }

  takeDecision();
  //delay(5000);
}

void takeDecision(){
  if (ir_sensor_1 && ir_sensor_2 && rfid_sensor){
    if(first == 2){
      Serial.println("Starting Alarm!!!");
      alarm();
      Serial.println("Stopped Alarm");
    }
    cleanUp();
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

  while (!stop_alarm) {
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
    authorize();
  }

  noTone(ALARM);
}

void flushReceive(){
  while(Serial1.available()){ Serial1.read(); }
}

void authorize(){
  if (Serial1.available() > 0) {
    stream = Serial1.read();
    incomingData += stream;
  }

  if (incomingData.length() > 13) {
    if(incomingData.indexOf(ownerID)>=0){
      digitalWrite(LED4, HIGH);
      stop_alarm = true;
    }
    incomingData = "";
    flushReceive();
  }
}

void cleanUp(){
  ir_sensor_1 = ir_sensor_2 = rfid_sensor = stop_alarm = false;
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  delay(2000);
  incomingData = "";
  flushReceive();
}

