// https://arduinomodules.info/ky-023-joystick-dual-axis-module/
// mapping Pin for KY-023 with ESP8266
// VRx -> None , VRy -> A0, SW -> D1 , GND -> GND, 5V -> 3.3V

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int digitalInPin = D1; //SW pin
bool bXActive = false;
int defaultJoystickValueX = 544;
bool bFirstLoop = true;
int thresholdDefaultValue = 5;

void setup_ky023() {

  pinMode(digitalInPin, INPUT_PULLUP);

  // initialize serial communication at 115200
  Serial.begin(115200);
}

//---------------------------------------------------
void loop_ky023() {

  valueButton_ky023 = digitalRead(digitalInPin);
  valueAnalogX_ky023 = analogRead(analogInPin); // read X axis value [0..1023]

  if (bFirstLoop) {
    defaultJoystickValueX = valueAnalogX_ky023;
    bFirstLoop = false;
  }

  if ((valueAnalogX_ky023 > defaultJoystickValueX + thresholdDefaultValue) || (valueAnalogX_ky023 < defaultJoystickValueX - thresholdDefaultValue)) {
    if (bDebugButtons)Serial.print("X:");
    if (bDebugButtons)Serial.print(valueAnalogX_ky023, DEC);

    mapAnalogX_ky023 = mapfloat(valueAnalogX_ky023, 9, 1024, -1.0f, +1.0f);
    if(mapAnalogX_ky023<-1.0)mapAnalogX_ky023 = -1.0;//parche

    if (bDebugButtons)Serial.print("Map X:");
    if (bDebugButtons)Serial.println(mapAnalogX_ky023, 2);
    bXActive = true;
  }
  else {
    bXActive = false;
  }

  //TODO Add multiplexor
  //  value = analogRead(A1); // read Y axis value [0..1023]
  //  Serial.print(" | Y:");
  //  Serial.print(value, DEC);

  valueButton_ky023 = digitalRead(digitalInPin); // read Button state [0,1]
  if (false)Serial.print(" | Button:");
  if (false)Serial.println(valueButton_ky023);


}

//----------------------------------------------------
void loop_evaluate_send_LeftRight_ky023() {

    if (bXActive) {
      sendXWebSockets();
      bXActive = false;
      if(bDebugPrint)Serial.println("send X!" + String(mapAnalogX_ky023, 2));
    }
    else {
      if(bDebugPrint)Serial.println("send NoLeftNoRight -> !" + String(mapAnalogX_ky023, 2));
      sendNoLeftNoRightWebSockets();
    }

  //Memo last Frame value // Not used
  //last_valueAnalogX_ky023 = valueAnalogX_ky023;
}

//----------------------------------------------------
void loop_evaluate_send_Click_ky023() {
  if (valueButton_ky023 == 0 && last_valueButton_ky023 != valueButton_ky023) {

    //goNextGameStatus();
    sendClickWebSockets();

    if (bDebugButtons) {
      Serial.println(" ");
      Serial.println("valueButton_ky023 = " + String(valueButton_ky023));
      Serial.println("new statusGame = " + String(statusGame));
    }
  }

  //Memo last Frame value
  last_valueButton_ky023 = valueButton_ky023;
}
