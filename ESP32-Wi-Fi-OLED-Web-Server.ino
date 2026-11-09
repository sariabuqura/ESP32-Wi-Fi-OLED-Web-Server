#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFiManager.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);
WebServer server(80);
String displayText = "Hello";
String htmlPage() {
  return "<!DOCTYPE html><html>"
         "<head><title>ESP32 OLED</title></head>"
         "<body style='text-align:center;'>"
         "<h2>Send Text to OLED</h2>"
         "<form action='/send'>"
         "<input type='text' name='msg' style='font-size:20px;'/>"
         "<br><br>"
         "<input type='submit' value='Send' style='font-size:20px;'/>"
         "</form>"
         "</body></html>";
}
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}
void handleSend() {
  if (server.hasArg("msg")) {
    displayText = server.arg("msg");
    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 10);
    display.println(WiFi.SSID());   
    display.println(WiFi.localIP());
    display.println("-----------------");
    display.setCursor(0, 40);
    display.println(displayText);
    display.display();
  }
  server.send(200, "text/html", htmlPage());
}
void configModeCallback(WiFiManager *myWiFiManager) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("AP Mode");
  display.println("SSID: IOT-ESP32");
  display.println(WiFi.softAPIP());  
  display.display();
}
void setup() {
  Serial.begin(115200);
  if (!display.begin(0x3C, true)) {
    Serial.println("OLED failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.println("Booting...");
  display.display();
  delay(1000);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);

  display.clearDisplay();
  display.setCursor(0, 10);
  display.println("Connecting WiFi...");
  display.display();

  wm.resetSettings();

  bool res = wm.autoConnect("IOT-ESP32");
  if (!res) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("WiFi Failed!");
    display.display();
    delay(3000);
    ESP.restart();
  }
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(WiFi.SSID());  
  display.println(WiFi.localIP());
  display.display();

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
}
void loop() {
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("Reconnecting...");
    display.display();
    delay(2000);
  }}