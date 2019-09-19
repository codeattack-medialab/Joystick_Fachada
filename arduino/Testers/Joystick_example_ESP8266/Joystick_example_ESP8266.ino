int valueAnalogX = 0;
int valueButton = 0;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int digitalInPin = D1;

void setup() {

  pinMode(digitalInPin, INPUT_PULLUP);

  // initialize serial communication at 115200
  Serial.begin(115200);
}

void loop() {

  valueButton = digitalRead(digitalInPin);
  valueAnalogX = analogRead(analogInPin); // read X axis value [0..1023]
  Serial.print("X:");
  Serial.print(valueAnalogX, DEC);

  //TODO Add multiplexor
  //  value = analogRead(A1); // read Y axis value [0..1023]
  //  Serial.print(" | Y:");
  //  Serial.print(value, DEC);

  valueButton = digitalRead(digitalInPin); // read Button state [0,1]
  Serial.print(" | Button:");
  //Serial.println(valueButton, DEC);
  Serial.println(valueButton);

  delay(100);
}
