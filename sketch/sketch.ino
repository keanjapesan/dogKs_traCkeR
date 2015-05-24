#define ALARM 12

void setup() {
  pinMode(ALARM, OUTPUT);
}

void loop() {
  if(false){
    alarm();
  }
  delay(10000);
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
    delay(100);
  }

  noTone(ALARM);
}
