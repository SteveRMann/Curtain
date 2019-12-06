// ============================= Connect the ESP to the router =============================
void setup_wifi() {
  Serial.println(F("\n"));
  Serial.print(F("Connecting to "));
  Serial.println(my_ssid);

  //Connect to WiFi network so we can reach the MQTT broker and publish messages to topics.
  // Make sure you include at the start of the sketch:
  // #include "ESP8266WiFi.h"   // Not needed if also using the Arduino OTA Library...
  // #include "D:\River Documents\Arduino\libraries\Kaywinnet.h"

  WiFi.mode(WIFI_STA);
  WiFi.enableInsecureWEP();
  WiFi.begin(my_ssid, my_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(WiFi.status()); Serial.print(F(" "));
  }
  Serial.print(F("\nWiFi connected, "));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());

}
