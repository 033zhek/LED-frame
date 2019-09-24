#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 32
#define LED_MODE_ADDRESS 97
#define EEPROM_SIZE 97

const char main_page[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
  <style>
    body {width: 100%;
          padding: 100px;
          margin: auto;
          background-color: white;
          text-align: center;
    }
    button {
          margin: auto;
          border: 2px solid #1E90FF;
          border-radius = 3px;
          background-color:#6495ED;
    }
    link {
          background-color:#4169E1; 
    }
  </style>
  <body>
    <h1>Choose led mode</h1>
    <p><a href="/1"><button class="button">1</button></a></p>
    <p><a href="/2"><button class="button">2</button></a></p>
    <p><a href="/3"><button class="button">3</button></a></p>
    <p><a href="/wifi_page"><button class="button link">Set home wifi ssid and password</button></a></p>
  </body>
</html>)=====";

const char wifi_page[] PROGMEM = R"=====(
<html>
  <style>
    body {width: %;
          margin: auto;
          background-color: white;
          text-align: center;
    }
    form {
          width: 100%;
          margin: auto;
    }
    button {
          border: 2px solid #1E90FF;
          border-radius = 3px;
          background-color:#6495ED;
    }
    link {
          background-color:#4169E1;
    }
  </style>
  <body>
    <form action="/post">
      SSID:     <input type="text" name="SSID"><br>
      Password: <input type="text" name="Password"><br>
      <input type="submit" value="Submit">
    </form><br>
    <p><a href="/"><button class="button">Go to LED page</button></a></p>
  </body>
</html>)=====";

WebServer server(80);

// Set these to your desired credentials.
uint8_t LED_mode = 0;
const char *my_ssid = "LED_frame";
const char *my_password = "LEaDtoyourbrightmemories";
String ssid;
String password;

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
    String buf_ssid = server.arg("SSID");
    if (buf_ssid[0] != '\0') {
      EEPROM.writeString(SSID_ADDRESS, buf_ssid);
      String buf_password = server.arg("Password");
      EEPROM.writeString(PASSWORD_ADDRESS, buf_password);
      EEPROM.commit();
    }
  }
  String s = main_page;
  server.send(200, "text/html", s);
}
void handleFirstMode() {
  EEPROM.writeByte(LED_MODE_ADDRESS, 1);
  LED_mode = 1;
  String s = main_page;
  server.send(200, "text/html", s);
  }
void handleSecondMode() {
  EEPROM.writeByte(LED_MODE_ADDRESS, 2);
  LED_mode = 2;
  String s = main_page;
  server.send(200, "text/html", s);
  }
void handleThirdMode() {
  EEPROM.writeByte(LED_MODE_ADDRESS, 3);
  LED_mode = 3;
  String s = main_page;
  server.send(200, "text/html", s);
  }

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...")
  delay(3000);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(my_ssid, my_password);
  Serial.println("AP established");
  LED_mode = EEPROM.readByte(LED_MODE_ADDRESS);
  ssid = EEPROM.readString(SSID_ADDRESS);
  Serial.println("SSID got from eeprom");
  Serial.println(ssid);

  if (ssid[0] != '\0') {
    Serial.println("SSID isn't empty");
    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }
    password = EEPROM.readString(PASSWORD_ADDRESS);
    if (password[0] != '\0') WiFi.begin(ssid.c_str(), password.c_str());
    else WiFi.begin(ssid.c_str());
    uint16_t wifi_connection_timeout = millis();
    while (WiFi.waitForConnectResult() != WL_CONNECTED && millis() - wifi_connection_timeout < 10000) {
      Serial.print(".");
      delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
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
  server.on("/post", handleInput);
  server.begin();//

  Serial.println("Server started");
}

void loop() {
  server.handleClient();
  switch(LED_mode){
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    }
    
  delay(1);
}
