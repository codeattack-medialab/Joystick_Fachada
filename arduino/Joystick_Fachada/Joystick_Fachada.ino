
//////////////////////////////////////////////
//WIFI and Websockets
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>
//////////////////////////////////////////////
//ID Joystick
int idJoystick = 2;
//Enter the SSID and Password of the Access Point or Hotspot.
const char* ssid = "Orange-BC0A";//"Medialab-Prado";//"Orange-BC0A"; //MPF
const char* password = "NmyRdSqc";//"visualizar";//"NmyRdSqc"; //Fachad4MP
//Enter the static ip that you want to set
IPAddress ip(192, 168, 1, 111);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
bool bDebugPrint = false;

/////////////////////////////////////////////
SocketIOclient socketIO;

//Control Vars
bool bSendNoLeftNoRightMessage = false;
bool bSendXMessage = false;
bool bSendClickMessage = false;
bool sendUDPOnce = true;
unsigned long messageTimestamp = 0;

//////////////////////////////////////////////////
//DISPLAY ePaper
// include library, include base class, make path known
#include <GxEPD.h>
// select the display class to use, only one
#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w
#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#if defined(ESP8266)
GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // default selection of D4(=2), D2(=4)
#endif

////////////////////////////////////////////////
//Include Images
#include "IMG_0001.h"

//////////////////////////////////////////////////
//CodeAttack Jostick
int valueAnalogX_ky023 = 0;
int last_valueAnalogX_ky023 = 0;
float mapAnalogX_ky023 = 0.0f;

int last_valueButton_ky023 = 1;
int valueButton_ky023 = 1;

int statusGame = 0; // 0(waiting),1(instructions),2(playing),3(gameOver)
int timer = 0;
bool bRefreshScreen = true;

bool bDebugButtons = true;
//-----------------------------------

void resetStatus() {
  statusGame = 0;
}
//-----------------------------------
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");

  display.init(115200); // enable diagnostic output on Serial

  Serial.println("setup ky023");
  setup_ky023();

  //////////
  Serial.println("setup webSockets And Internet");
  setup_webSockets();

  Serial.println("setup done");
}

//-----------------------------------
void goNextGameStatus() {
  statusGame++;
  if (statusGame > 3)statusGame = 0;
  bRefreshScreen = true;
}

//-----------------------------------
void loop()
{
  //UPdate timer
  timer = millis();

  ///////////////////////////////////////////
  //Loop values Joystick - ky023
  loop_ky023();
  loop_evaluate_send_Click_ky023();
  loop_evaluate_send_LeftRight_ky023();

  ////////
  loop_webSockets();

  /////
  loopStatus();



  /////////////////////////////////////////////////////7
  delay(100);

}





///////////////////////////////////////////
void loopStatus() {

  //Status Loop ePaper Screeen

  int x = 0;//float(296*0.5);
  Serial.println("Pos X Image is " + String (x));
  int y = 0;
  uint16_t forward = GxEPD::bm_invert | GxEPD::bm_flip_x;
  uint16_t reverse = GxEPD::bm_invert | GxEPD::bm_flip_x | GxEPD::bm_flip_y;

  if (statusGame == 0) {
    if (bRefreshScreen) {
      Serial.println("Display Waiting");
      //Waiting. Refresh Quick bitmap image
//      display.setRotation(1);
//      display.fillScreen(GxEPD_WHITE);
//      display.drawExampleBitmap(myBitmap_madpong, x, y, 296, 128, GxEPD_BLACK, reverse);
//      display.update();
    }

    //do not refresh until changes. Time for full refresh is 2s
    bRefreshScreen = false;
  }
  else if (statusGame == 1) {
    if (bRefreshScreen) {
      Serial.println("Display Instrucctions");
      //Instructions. Draw Some Text. 3..2..1 Start!
//      display.setRotation(1);
//      display.drawPaged(showFontCallback_Instructions);
//      delay(2000);
    }
    //do not refresh until changes. Time for full refresh is 2s
    bRefreshScreen = false;
  }
  else if (statusGame == 2) {
    if (bRefreshScreen) {
      Serial.println("Display Playing ");
//      display.setRotation(0);
//      display.fillScreen(GxEPD_WHITE);
//      display.drawExampleBitmap(gImage_IMG_0001, x, y, 64, 180, GxEPD_BLACK);
//      display.update();
    }
    //do not refresh until changes. Time for full refresh is 2s
    bRefreshScreen = false;
  }
  else if (statusGame == 3) {
    //GameOver
    if (bRefreshScreen) {
      Serial.println("Display gameOver ");
      //display.drawCornerTest();
      //Idem by hand
      uint8_t rotation = display.getRotation();
      for (uint16_t r = 0; r < 4; r++)
      {
//        display.setRotation(r);
//        display.fillScreen(GxEPD_WHITE);
//        display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
//        display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
//       display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
//        display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);
//        display.update();
//        display.setRotation(rotation);
      }
    }
    //do not refresh until changes. Time for full refresh is 2s
    bRefreshScreen = false;
  }

}
