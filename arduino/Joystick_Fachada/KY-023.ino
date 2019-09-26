// https://arduinomodules.info/ky-023-joystick-dual-axis-module/
// mapping Pin for KY-023 with ESP8266
// VRx -> None , VRy -> A0, SW -> D1 , GND -> GND, 5V -> 3.3V

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
const int digitalInPin = D1; //SW pin
bool bLeftActive = false;
bool bRightActive = false;
int defaultJoystickValueX = 544;
bool bFirstLoop = true;

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

  if (valueAnalogX_ky023 > defaultJoystickValueX + 10) {
    if (bDebugButtons)Serial.print("X:");
    if (bDebugButtons)Serial.print(valueAnalogX_ky023, DEC);

    mapAnalogX_ky023_left = mapfloat(valueAnalogX_ky023, defaultJoystickValueX, 1024, 0.0f, -1.0f);

    if (bDebugButtons)Serial.print("Map Left X:");
    if (bDebugButtons)Serial.println(mapAnalogX_ky023_left, 2);
    bLeftActive = true;
    bRightActive = false;
  }
  else if (valueAnalogX_ky023 < defaultJoystickValueX - 10) {
    if (bDebugButtons)Serial.print("X:");
    if (bDebugButtons)Serial.print(valueAnalogX_ky023, DEC);
    mapAnalogX_ky023_right = mapfloat(valueAnalogX_ky023, defaultJoystickValueX, 9, 0.0f, 1.0f); // 9 better than 0... KY023 is not perfect arround 0
    if(mapAnalogX_ky023_right>1.0)mapAnalogX_ky023_right = 1.0;//parche

    if (bDebugButtons)Serial.print("Map Right or Center X:");
    if (bDebugButtons)Serial.println(mapAnalogX_ky023_right, 2);
    bLeftActive = false;
    bRightActive = true;
  }
  else {
    bLeftActive = false;
    bRightActive = false;
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
  //if (valueAnalogX_ky023 != last_valueAnalogX_ky023) {

    if (bLeftActive) {
      sendLeftWebSockets();
      bLeftActive = false;
      bRightActive = false;
      Serial.println("send left!" + String(mapAnalogX_ky023_left, 2));
    }
    else if (bRightActive) {
      //then is Right or 0
      sendRightWebSockets();
      bRightActive = false;
      bLeftActive = false;
      Serial.println("send Right!" + String(mapAnalogX_ky023_right, 2));
    }
    else {
      Serial.println("send NoLeftNoRight -> !" + String(mapAnalogX_ky023_right, 2));
      sendNoLeftNoRightWebSockets();//TODO Avoid to send everytime
    }
 // }

  // }

  //Memo last Frame value
  last_valueAnalogX_ky023 = valueAnalogX_ky023;
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
