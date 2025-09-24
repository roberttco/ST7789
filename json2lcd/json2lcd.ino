//#include <LinkedList.h>
//#include <CmdBuffer.hpp>
#include <CmdCallback.hpp>
#include <CmdParser.hpp>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <EspMQTTClient.h>

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(X) Serial.print(X)
#define DEBUG_PRINTLN(X) Serial.println(X)
#define DEBUG_DUMPPARAMS(X) dumpParams(X)
#else
#define DEBUG_PRINT(X)
#define DEBUG_PRINTLN(X)
#define DEBUG_DUMPPARAMS(X)
#endif

// ST7789 TFT module connections
#define TFT_DC    D1     // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST   D2     // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS    -1     // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)

// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// command parser stuff
CmdCallback<8> cmdCallbackP;
//CmdBuffer<512> myBuffer;
CmdParser     myParser;

int rotation = 0;
bool connecting = true;
bool waiting = false;

// MQTT
/*
EspMQTTClient client(
  "Singularity",
  "5ms21Od55yy5zgJERTYK",
  "192.168.2.6",  // MQTT Broker server ip
  "7789Cube",     // Client name that uniquely identify your device
  24552              // The MQTT port, default to 1883. this line can be omitted
);
*/
EspMQTTClient client(
  "qsvMIMt8Fm6NV3",
  "UbKNUJakLBLpOh",
  "192.168.2.7",  // MQTT Broker server ip
  "7789Cube",     // Client name that uniquely identify your device
  24552              // The MQTT port, default to 1883. this line can be omitted
);

void setup(void)
{
  Serial.begin(115200);

  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE3);    // Init ST7789 display 240x240 pixel
  tft.setSPISpeed(40000000);

  //
  tft.setRotation(rotation);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);

  ////
  // Enable cmd strings with "" -> SET VAL "HALLO WORLD"
  myParser.setOptIgnoreQuote(false); // default

  ////
  // Set command seperator.
  myParser.setOptSeperator(','); // default

  cmdCallbackP.addCmd("F", &functFill);
  cmdCallbackP.addCmd("S", &functString);
  cmdCallbackP.addCmd("SX", &functStringExtended);
  cmdCallbackP.addCmd("R", &functRectangle);
  cmdCallbackP.addCmd("C", &functCircle);
  cmdCallbackP.addCmd("T", &functTriangle);
  cmdCallbackP.addCmd("P", &functPixel);
  cmdCallbackP.addCmd("L", &functCircle);

  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("7133egyptian/out/display1/state", "Offline");  // You can activate the retain flag by setting the third parameter to true
}

int connectingRadius = 1;
int lastRadius = 1;
void loop(void)
{
  client.loop();

  if (connecting || waiting)
  {
      
      connectingRadius += 8;
      if (connectingRadius > 110)
      {
        connectingRadius = 0;
      }
      
      tft.drawCircle(120,120, lastRadius, ST77XX_BLACK);
      if (waiting)
      {
        tft.drawCircle(120,120, 110-connectingRadius, ST77XX_ORANGE);
        lastRadius = 110-connectingRadius;
      }
      else
      {
        tft.drawCircle(120,120, connectingRadius, ST77XX_WHITE);
        lastRadius = connectingRadius;
      }

      delay(70);
  }
//  else
//  {
//    cmdCallback.updateCmdProcessing(&myParser, &myBuffer, &Serial);
//  }
}

void onConnectionEstablished()
{
  // Subscribe to "7133egyptian/in/display1" and display received message to Serial
  client.subscribe("7133egyptian/in/display1/lcd", [](const String & payload) {
    if (waiting)
    {
      tft.fillScreen(ST77XX_BLACK);
      waiting = false;
    }
    Serial.println(payload);

    char *p = (char *)payload.c_str();
    char *i;
    char *token = strtok_r(p,"|",&i);
    while (token != NULL)
    {
      if (myParser.parseCmd(token) != CMDPARSER_ERROR)
      {
        cmdCallbackP.processCmd(&myParser);
      }

      token = strtok_r(NULL,"|",&i);
    }
  });

  client.subscribe("7133egyptian/in/display1/command", [](const String & payload) {
    if (waiting)
    {
      tft.fillScreen(ST77XX_BLACK);
      waiting = false;
    }
    Serial.println(payload);

    int cmd = payload.toInt();
    switch (cmd)
    {
      case 255: // clear display and home cursor
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(0,0);
        break;
      case 0: // toggle rotation
      case 1: // toggle rotation
      case 2: // toggle rotation
      case 3: // toggle rotation
        rotation = cmd;
        tft.setRotation(rotation);
        break;
      default:
        break; 
    }
  });
  
  // Publish a message to "mytopic/test"
  client.publish("7133egyptian/out/display1/state", "Online"); // You can activate the retain flag by setting the third parameter to true
  
  connecting = false;
  waiting = true;
  //tft.fillScreen(ST77XX_BLACK);
}
