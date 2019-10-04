/*
   WebSocketClientSocketIO.ino

    Created on: 06.06.2016

*/

#define LED D4            //  (D0)in V2 && D4 in Lolin V3

bool doConectRoom = true;
bool bDebugSockets = true;
bool bConnected = false;
bool bNotConnectedToChanel = true;

#define USE_SERIAL Serial
//#define HAS_SSL true

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      USE_SERIAL.printf("[IOc] Disconnected!\n");
      bConnected = false;
      bNotConnectedToChanel = true;
      break;
    case sIOtype_CONNECT:
      USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
      bConnected = true;
      break;
    case sIOtype_EVENT:
      USE_SERIAL.printf("[IOc] get event: %s\n", payload);
      //Do something with this event
      USE_SERIAL.printf("doBlikingLED");
      doBlikingLED(1);//seconds blinking
      break;
    case sIOtype_ACK:
      USE_SERIAL.printf("[IOc] get ack: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      USE_SERIAL.printf("[IOc] get error: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:
      USE_SERIAL.printf("[IOc] get binary: %u\n", length);
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
      hexdump(payload, length);
      break;
  }
}

//----------------------------------------------
void wifiSetup() {
  // Connection to wireless network
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.print("Connecting to ");
  USE_SERIAL.println(ssid);

  //Configuring the WI-FI with the specified static IP.
  WiFi.config(ip, gateway, subnet);

  //Start the WI-FI connection with specified ACCESS-POINT
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address in the serial monitor.
  Serial.print("Type this address in URL to connect: ");
  Serial.print("http://");
  Serial.println(ip);
  Serial.println("/");

  Serial.print("Local IP = ");
  Serial.println(WiFi.localIP());
}

void setup_webSockets() {

  USE_SERIAL.println("Hello WbsocketsIO");

  //USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  ////////////////////
  wifiSetup();


  // server address, port and URL
  socketIO.begin("192.168.200.102", 8004);// 192.168.1.171 or URL //192.168.43.244 //

  // event handler
  socketIO.onEvent(socketIOEvent);

  //LED response
  pinMode(LED, OUTPUT);   // Defining pin as output

  //COUNTER TIMER
  messageTimestamp = millis();

}

//---------------------------------------------------
//seconds blinking
void doBlikingLED(int _seconds) {

  USE_SERIAL.println("doBlikingLED Init");
  long auxSince = millis() + 1000 * _seconds;

  while (auxSince > millis()) {

    //    USE_SERIAL.println("auxSince = " + String(auxSince));
    //    USE_SERIAL.println("auxNow = " + String(millis()));

    digitalWrite(LED, LOW); // turn the LED on.
    delay(100);
    digitalWrite(LED, HIGH);// turn the LED off.(Note that LOW is the voltage level but actually
    //the LED is on; this is because it is acive low on the ESP8266.
    delay(10);
  }
}

//----------------------------------------------------
void sendMessageRoom(String _message2Send1, String _message2Send2) {
  ////////////////////////////////////////////////
  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('event_name', ....
  array.add(_message2Send1);
  array.add(_message2Send2);

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  if (bDebugSockets)USE_SERIAL.println(output);
}

//----------------------------------------------------
void sendMessage(String _message2Send1, String _message2Send2, String _from) {

  // creat JSON message for Socket.IO (event)
  DynamicJsonDocument doc(1024);
  JsonArray array = doc.to<JsonArray>();

  // add evnet name
  // Hint: socket.on('event_name', ....
  array.add(_message2Send1);

  // add payload (parameters) for the event
  JsonObject param1 = array.createNestedObject();
  param1["sender"] = _from;//"4";//from
  param1["message"] = _message2Send2;

  // JSON to String (serializion)
  String output;
  serializeJson(doc, output);

  // Send event
  socketIO.sendEVENT(output);

  // Print JSON for debugging
  if (bDebugSockets)USE_SERIAL.println(output);
}


//------------------------------------------------------

void loop_webSockets() {
  socketIO.loop();
  update_serialInput();

  uint64_t now = millis();
  float ellapsedTime = now - messageTimestamp;
  //USE_SERIAL.print("ellapsedTime = ");
  //USE_SERIAL.println(ellapsedTime);

  if (bConnected) {

    if (bNotConnectedToChanel && doConectRoom) {
      USE_SERIAL.println("Enter enter to this Room!");
      sendMessageRoom("addJoystick", "/8");//4:4
      doConectRoom = false;
    }
    else if (/*ellapsedTime > 1000 &&*/ sendUDPOnce == true) {
      //sendMessageRoom("addJoystick", "/8");//Entering in Room #8
      if (sendUDPOnce) {
        if (bSendNoLeftNoRightMessage) {
          sendMessage("toServer", "X/" + String(0.0), String(idJoystick)); // String(mapAnalogX_ky023_right)
          USE_SERIAL.println("bSendRightMessage!");
          bSendNoLeftNoRightMessage = false;
        }
        else {
          if (bSendLeftMessage) {
            sendMessage("toServer", "X/" + String(mapAnalogX_ky023_left), String(idJoystick)); //String(mapAnalogX_ky023_left)
            USE_SERIAL.println("bSendLeftMessage!");
            bSendLeftMessage = false;
          }
          if (bSendRightMessage) {
            sendMessage("toServer", "X/" + String(mapAnalogX_ky023_right), String(idJoystick)); // String(mapAnalogX_ky023_right)
            USE_SERIAL.println("bSendRightMessage!");
            bSendRightMessage = false;
          }
        }

        if (bSendClickMessage) {

          //TODO if pressed
          sendMessage("toServer", "Click/" + String(1.0), String(idJoystick)); // "Click is just one Click"
          bSendClickMessage = false;
          USE_SERIAL.println("bSendClickMessage!");

          //TODO if released
        }
      }
      sendUDPOnce = false;
    }
  }


}

//-----------------------------------------------
void sendNoLeftNoRightWebSockets() {
  bSendNoLeftNoRightMessage = true;
  sendUDPOnce = true;
}

//-----------------------------------------------
void sendRightWebSockets() {
  bSendRightMessage = true;
  sendUDPOnce = true;
}
//-----------------------------------------------
void sendLeftWebSockets() {
  bSendLeftMessage = true;
  sendUDPOnce = true;
}
//-------------------------------------------------
void sendClickWebSockets() {
  bSendClickMessage = true;
  sendUDPOnce = true;
}

//-------------------------------------------------
void update_serialInput() {

  String myTextLeft = "left\n";
  String myTextRight = "right\n";
  String myTextClick = "click\n";

  String mySerialReceivedString = "";
  if (USE_SERIAL.available() > 0) //Checks is there any data in buffer
  {
    //USE_SERIAL.print("We got:");
    //USE_SERIAL.println(char(USE_SERIAL.read()));  //Read serial data byte and send back to serial monitor
    mySerialReceivedString = Serial.readString();
    USE_SERIAL.print(mySerialReceivedString);

    if (mySerialReceivedString == myTextLeft) {
      USE_SERIAL.println("We send LEFT command");
      bSendLeftMessage = true;
    }

    if (mySerialReceivedString == myTextRight) {
      USE_SERIAL.print("We send RIGHT command");
      bSendRightMessage = true;
    }

    if (mySerialReceivedString == myTextClick) {
      USE_SERIAL.print("We send Click command");
      bSendClickMessage = true;
    }

    sendUDPOnce = true;
    messageTimestamp = millis();
  }
  else
  {
    //    USE_SERIAL.println("Hello World..."); //Print Hello word every one second
    //    delay(1000);                      // Wait for a second
  }
}
