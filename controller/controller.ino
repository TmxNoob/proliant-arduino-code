unsigned long lastCommand;

unsigned long commandTimeout = 1000000; //60s
unsigned char minSpeed = 15;
unsigned char maxSpeed = 255;
unsigned char defaultSpeed = 40;

String serialBuffer = "";
bool stringComplete = false;

char fanPins[] = {3,5,6,9,10,11};
char numFans = sizeof(fanPins);

void setupFans() {
  for(char c = 0;c<numFans;c++){
    pinMode(fanPins[c],OUTPUT);
    setFan(c,defaultSpeed);
  }
}
void setFan(char fan,unsigned char spd){
 unsigned char spdPwm = 255-(max(minSpeed,min(spd,maxSpeed))); //make sure speed is between max and min and hp proliant fans use inversed pwm
 analogWrite(fanPins[fan],spdPwm);
}

void resetSerialBuffer(){
  serialBuffer = "";
  stringComplete = false;
}

void setup(){
  setupFans(); //set fan pwm pins as outputs
  resetSerialBuffer();

  //change arduino clocks or smth to set pwm frequency to like 32khz or smth like that for fans xd dont care aslong as it works
  TCCR0B = _BV(CS00);
  TCCR1B = _BV(CS00);
  TCCR2B = _BV(CS00);
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);

  Serial.begin(9600);
  Serial.setTimeout(100000);
  lastCommand = millis();
}

void loop(){
  if(millis()-lastCommand > commandTimeout){
    Serial.println("Resetting fans");
    setupFans();
    lastCommand = millis();
  }
  if(stringComplete){
    //process serial event
    char cmdbuf[64];
    serialBuffer.getBytes(cmdbuf,64);
    switch(cmdbuf[0]){
      case 's':
        Serial.println("fan number:");
        char cmdFanNum = Serial.parseInt();
        Serial.println("fan speed:");
        char cmdFanSpeed = Serial.parseInt();
        if(cmdFanNum > 0 && cmdFanNum <= numFans){
          setFan(cmdFanNum-1,cmdFanSpeed);
          Serial.println("OK");
        }else{
          Serial.println("fan out of range");
        }
        break;
      case 'g':
        Serial.println("fan speed is");
        Serial.println(millis());
        Serial.println(lastCommand);
        break;
      case 'c':
        Serial.println("config edited");
        break;
      default:
        Serial.println("unknown command");
        break;
    }
    lastCommand = millis();
    resetSerialBuffer();
  }
}

void serialEvent() {
  if(stringComplete) return; //skip if current string isnt processed yet
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    serialBuffer += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
