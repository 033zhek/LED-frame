#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <FastLED.h>          // библиотека для работы с лентой

#define LED_COUNT 13          // число светодиодов в кольце/ленте
#define LED_DT 4             // пин, куда подключен DIN ленты
#define END_OF_SSID_ADDRESS 32
#define END_OF_PASSWORD_ADDRESS 96
#define LED_MODE_ADDRESS 97
#define EEPROM_SIZE 98

const char main_page[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
  <style>
  </style>
  <body>
    <h1>Choose led mode</h1>
    <a href="/1" >1</a><br>
    <a href="/2" >2</a><br>
    <a href="/3" >3</a><br>
    <a href="/4" >4</a><br>
    <a href="/5" >5</a><br>
    <a href="/6" >6</a><br>
    <a href="/7" >7</a><br>
    <a href="/8" >8</a><br>
    <a href="/wifi_page">Set home wifi ssid and password</a>
  </body>
</html>)=====";
//style="width:20px; height:10px; border:2px solid black; border-radius:10px"
const char wifi_page[] PROGMEM = R"=====(
<html>
  <style>
  </style>
  <body>
    <form action="/post">
      SSID:     <input type="text" name="SSID"><br>
      Password: <input type="text" name="Password"><br>
      <input type="submit" value="Submit">
    </form><br>
    <a href="/"><button class="button">Go to LED page</button></a>
  </body>
</html>)=====";

ESP8266WebServer server(80);

int max_bright = 51;

// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int TOP_INDEX = int(LED_COUNT / 2);
int EVENODD = LED_COUNT % 2;
struct CRGB leds[LED_COUNT];
int ledsX[LED_COUNT][3];     //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY (FOR CELL-AUTOMATA, MARCH, ETC)

int thisdelay = 20;          //-FX LOOPS DELAY VAR
int thisstep = 10;           //-FX LOOPS DELAY VAR
int thishue = 0;             //-FX LOOPS DELAY VAR
int thissat = 255;           //-FX LOOPS DELAY VAR

int thisindex = 0;
int thisRED = 0;
int thisGRN = 0;
int thisBLU = 0;

int idex = 0;                //-LED INDEX (0 to LED_COUNT-1
int ihue = 0;                //-HUE (0-255)
int ibright = 0;             //-BRIGHTNESS (0-255)
int isat = 0;                //-SATURATION (0-255)
int bouncedirection = 0;     //-SWITCH FOR COLOR BOUNCE (0-1)
float tcount = 0.0;          //-INC VAR FOR SIN LOOPS
int lcount = 0;              //-ANOTHER COUNTING VAR
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}

void one_color_allHSV(int ahue) {    //-SET ALL LEDS TO ONE COLOR (HSV)
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(ahue, thissat, 255);
  }
}

// Set these to your desired credentials.
byte LED_mode = 3;
const char *my_ssid = "LED_frame";
const char *my_password = "LEaDtoyourbrightmemories";

IPAddress local_IP(192, 168, 1, 125);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);


void handleRoot() {
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleWiFiSettings() {
  String s = wifi_page;
  server.send(200, "text/html", s);
}
void handleInput() {
  if (server.hasArg("SSID") && server.hasArg("Password")) {
    String qssid = server.arg("SSID");
    String qpass = server.arg("Password");
    for (int i = 0; i < qssid.length(); ++i)
    {
      EEPROM.write(i, qssid[i]);
      Serial.print("Wrote: ");
      Serial.println(qssid[i]);
    }
    Serial.println("writing eeprom pass:");
    for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32 + i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]);
    }
    EEPROM.commit();
  }
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleFirstMode() {
  EEPROM.write(LED_MODE_ADDRESS, 1);
  EEPROM.commit();
  LED_mode = 1;
  Serial.println(LED_mode);
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleSecondMode() {
  EEPROM.write(LED_MODE_ADDRESS, 2);
  EEPROM.commit();
  LED_mode = 2;
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleThirdMode() {
  EEPROM.write(LED_MODE_ADDRESS, 3);
  EEPROM.commit();
  LED_mode = 3;
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleFourthMode() {
  EEPROM.write(LED_MODE_ADDRESS, 4);
  EEPROM.commit();
  LED_mode = 4;
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleFifthMode() {
  EEPROM.write(LED_MODE_ADDRESS, 5);
  EEPROM.commit();
  LED_mode = 5;
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleSixthMode() {
  EEPROM.write(LED_MODE_ADDRESS, 6);
  EEPROM.commit();
  LED_mode = 6;
  String s = main_page;
  server.send(200, "text/html", s);
} void handleSeventhMode() {
  EEPROM.write(LED_MODE_ADDRESS, 7);
  EEPROM.commit();
  LED_mode = 7;
  String s = main_page;
  server.send(200, "text/html", s);
} void handleEighthMode() {
  EEPROM.write(LED_MODE_ADDRESS, 8);
  EEPROM.commit();
  LED_mode = 8;
  String s = main_page;
  server.send(200, "text/html", s);
}

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  
  LEDS.setBrightness(max_bright);
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);
  one_color_all(0, 0, 0);
  LEDS.show();
  
  Serial.println();
  Serial.println("Configuring access point...");
  delay(1000);
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Configured");
  delay(1000);
  WiFi.softAP(my_ssid, my_password);
  Serial.println("AP established");
  delay(1000);
  LED_mode = EEPROM.read(LED_MODE_ADDRESS);
  Serial.println(LED_mode);
  String essid = "";
  for (int i = 0; i < END_OF_SSID_ADDRESS; ++i)
  {
    essid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(essid);
  Serial.println("Reading EEPROM pass");
  String epass = "";
  for (int i = END_OF_SSID_ADDRESS; i < END_OF_PASSWORD_ADDRESS; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  delay(1000);
  if (essid != "") {
    Serial.println("SSID isn't empty");
    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }
    if (epass != "") WiFi.begin(essid, epass);
    else WiFi.begin(essid);
    uint16_t wifi_connection_timeout = millis();
    while (WiFi.waitForConnectResult() != WL_CONNECTED && millis() - wifi_connection_timeout < 10000) {
      Serial.print(".");
      delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(essid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Error with connection to wifi");
    }

  }
  server.on("/", handleRoot);
  server.on("/wifi_page", handleWiFiSettings);
  server.on("/1", handleFirstMode);
  server.on("/2", handleSecondMode);
  server.on("/3", handleThirdMode);
  server.on("/4", handleFourthMode);
  server.on("/5", handleFifthMode);
  server.on("/6", handleSixthMode);
  server.on("/7", handleSeventhMode);
  server.on("/8", handleEighthMode);
  server.on("/post", handleInput);
  server.begin();//

  Serial.println("Server started");
}

void loop() {
  server.handleClient();
  switch (LED_mode) {
    case 1: 
      rainbow_fade();
      break;            // плавная смена цветов всей ленты
    case 2: 
      random_burst();
      break;            // случайная смена цветов
    case 3: 
      pulse_one_color_all_rev(); 
      break; // пульсация со сменой цветов
    case 4: 
      random_march();
      break;            // безумие случайных цветов 200
    case 5:
      rgb_propeller();
      break;           // RGB пропеллер
    case 6:
      new_rainbow_loop();
      break;        // крутая плавная вращающаяся радуга 100
    case 7:
      rainbowCycle(20);
      break;                                        // очень плавная вращающаяся радуга 20
    case 8:
      demo_modeA();             // длинное демо
  }
  yield();
}

void rainbow_fade() {                         //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;
  if (ihue > 255) {
    ihue = 0;
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(ihue, thissat, 255);
  }
  LEDS.show();
  delay(thisdelay);
}

void random_burst() {                         //-m4-RANDOM INDEX/COLOR
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  leds[idex] = CHSV(ihue, thissat, 255);
  LEDS.show();
  delay(thisdelay*2);
}

void pulse_one_color_all_rev() {           //-m11-PULSE SATURATION ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0) {
    isat++;
    if (isat >= 255) {
      bouncedirection = 1;
    }
  }
  if (bouncedirection == 1) {
    isat = isat - 1;
    if (isat <= 1) {
      bouncedirection = 0;
    }
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(thishue, isat, 255);
  }
  LEDS.show();
  delay(thisdelay);
}

void random_march() {                   //-m14-RANDOM MARCH CCW
  copy_led_array();
  int iCCW;
  leds[0] = CHSV(random(0, 255), 255, 255);
  for (int idex = 1; idex < LED_COUNT ; idex++ ) {
    iCCW = adjacent_ccw(idex);
    leds[idex].r = ledsX[iCCW][0];
    leds[idex].g = ledsX[iCCW][1];
    leds[idex].b = ledsX[iCCW][2];
  }
  LEDS.show();
  delay(thisdelay*8);
}

void rgb_propeller() {                           //-m27-RGB PROPELLER
  idex++;
  int ghue = (thishue + 80) % 255;
  int bhue = (thishue + 160) % 255;
  int N3  = int(LED_COUNT / 3);
  int N6  = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);
  for (int i = 0; i < N3; i++ ) {
    int j0 = (idex + i + LED_COUNT - N12) % LED_COUNT;
    int j1 = (j0 + N3) % LED_COUNT;
    int j2 = (j1 + N3) % LED_COUNT;
    leds[j0] = CHSV(thishue, thissat, 255);
    leds[j1] = CHSV(ghue, thissat, 255);
    leds[j2] = CHSV(bhue, thissat, 255);
  }
  LEDS.show();
  delay(thisdelay*6);
}

void new_rainbow_loop() {                      //-m88-RAINBOW FADE FROM FAST_SPI2
  ihue -= 1;
  fill_rainbow( leds, LED_COUNT, ihue );
  LEDS.show();
  delay(thisdelay);
}
//--------------------------------------------------------------------------------------
void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < LED_COUNT; i++) {
      c = Wheel(((i * 256 / LED_COUNT) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    FastLED.show();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}
//--------------------------------------------------------------------------------------
void demo_modeA() {
  int r = 10;
  thisdelay = 20; thisstep = 10; thishue = 0; thissat = 255;
  one_color_all(255, 255, 255); LEDS.show(); delay(1200);
  for (int i = 0; i < r * 25; i++) {
    rainbow_fade();
  }
  
  for (int i = 0; i < r * 20; i++) {
    random_burst();
  }
  one_color_all(0, 0, 0); LEDS.show();
  thisdelay = 15; thishue = 0; thissat = 255;
  for (int i = 0; i < r * 40; i++) {
    pulse_one_color_all_rev();
  }
  thisdelay = 40;
  for (int i = 0; i < r * 8; i++) {
    random_march();
  }
  thisdelay = 50;
  for (int i = 0; i < r * 10; i++) {
    rgb_propeller();
  }
  thisdelay = 5;
  for (int i = 0; i < r * 120; i++) {
    new_rainbow_loop();
  }
  one_color_all(255, 0, 0); LEDS.show(); delay(1200);
  one_color_all(0, 255, 0); LEDS.show(); delay(1200);
  one_color_all(0, 0, 255); LEDS.show(); delay(1200);
  one_color_all(255, 255, 0); LEDS.show(); delay(1200);
  one_color_all(0, 255, 255); LEDS.show(); delay(1200);
  one_color_all(255, 0, 255); LEDS.show(); delay(1200);
}

//-----------------------------------------------------------------------------------------------------------------

int adjacent_ccw(int i) {
  int r;
  if (i > 0) {
    r = i - 1;
  }
  else {
    r = LED_COUNT - 1;
  }
  return r;
}

void copy_led_array() {
  for (int i = 0; i < LED_COUNT; i++ ) {
    ledsX[i][0] = leds[i].r;
    ledsX[i][1] = leds[i].g;
    ledsX[i][2] = leds[i].b;
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}
