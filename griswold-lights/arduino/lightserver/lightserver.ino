// External LED on a PWM pin.
int led = 9;
// Serial messaging
String text = "";

void setup() {

  //Set up serial connection.
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  // Read the serial port.
  if(Serial.available()>0) {
    char inputChar = (char)Serial.read();
    //Serial.write(inputChar);
    text += inputChar;
    if(inputChar == '\n') {
      Serial.print("Command: ");
      Serial.print(text);
      Serial.print("\n"); 
      executeCommand(text);
      text = "";
    }
  }
}

void setIntensity(int i) {
  analogWrite(led, i);
}

void executeLightsOn()
{
  setIntensity(255);
}

void executeLightsOff()
{
  setIntensity(0);
}

void executeCommand(String text) {
  if(text == "lights on\n") {
    executeLightsOn();
    Serial.print("OK\n");
  } else if(text == "lights off\n") {
    executeLightsOff();
    Serial.print("OK\n");
  } else if(text.startsWith("setintensity:")) {
    if ((text.length() > 13) && (text.length() <= 17)) { // 12chars, :,  3 chars, \n = 17
      String s = text.substring(13);
      int ssize = s.length();
      int i = s.substring(0,ssize-2).toInt(); // Remove trailing \n.
      if ((i == 0) && (s != "0\n"))
      {
        Serial.print("Invalid value ");
        Serial.print(s);
        Serial.print("\n");
      }
      else
      {
        setIntensity(i);
        Serial.print("OK\n");
      }
    }
    else {
      Serial.print("Bad command\n");
    }
  }
}

