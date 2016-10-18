// On-board LED
int led = 13;
// Serial messaging
String text = "";

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  //Set up serial connection.
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  // Read the serial port.
  if(Serial.available()>0) {
    char inputChar = (char)Serial.read();
    Serial.write(inputChar);
    text += inputChar;
    if(inputChar == '\n') {
      Serial.print("Executing command: ");
      Serial.print(text);
      Serial.print("\n");
      executeCommand(text);
      text = "";
    }
  }
}


void executeLightsOn()
{
  digitalWrite(led, HIGH);
}

void executeLightsOff()
{
  digitalWrite(led, LOW);
}

void executeCommand(String text) {
  if(text == "lights on\n") {
    executeLightsOn();
  } else if(text == "lights off\n") {
    executeLightsOff();
  }
}

