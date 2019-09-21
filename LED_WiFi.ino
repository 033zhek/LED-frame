#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 32
#define EEPROM_SIZE 96

const char main_page[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
  <style>
  </style>
  <body>
    <h1>Choose led mode</h1>
    <p><a href="/1"><button class=\"button\">1</button></a></p>
    <p><a href="/2"><button class=\"button\">2</button></a></p>
    <p><a href="/3"><button class=\"button\">3</button></a></p>
    <p><a href="/wifi_page"><button class=\"button\">Set home wifi ssid and password</button></a></p>
  </body>
</html>)=====";

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
    <p><a href="/"><button class=\"button\">Go to LED page</button></a></p>
  </body>
</html>)=====";

WebServer server(80);

// Set these to your desired credentials.
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
void handleFirstMode() {}
void handleSecondMode() {}
void handleThirdMode() {}

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  delay(3000);
  // You can remove the password parameter if you want the AP to be open.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(my_ssid, my_password);
  Serial.println("AP established");
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
  delay(1);
}
