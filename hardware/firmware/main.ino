#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "$WIFI_SSID"
#define WIFI_PASSWORD "$WIFI_PASSWORD"
#define DATABASE "scrooge-mcduck-b.firebaseio.com"
#define STREAM "/display"
#define OLED_I2C_ADDR 0x3C

#if (SSD1306_LCDHEIGHT != 64)
# error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define OLED_RESET LED_BUILTIN

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
  display.clearDisplay();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());
  Firebase.begin(DATABASE);
  Firebase.stream(STREAM);
}

void loop() {
  if (Firebase.failed()) {
    Serial.print("Streaming error: ");
    Serial.println(Firebase.error());
  }

  if (!WiFi.isConnected()) {
    WiFi.reconnect();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    return;
  }

  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    auto eventType = event.getString("type");
    eventType.toLowerCase();
    if (eventType == "put") {
      auto root = event.getJsonVariant("data");

      auto payment = root["payment"].as<String>();
      Serial.print("Payment: ");
      Serial.println(payment);

      auto transfer = root["transfer"].as<String>();
      Serial.print("Transfer: ");
      Serial.println(transfer);

      auto withdraw = root["withdraw"].as<String>();
      Serial.print("Withdraw: ");
      Serial.println(withdraw);

      auto updated_on = root["updated_on"].as<String>();
      Serial.print("Updated On: ");
      Serial.println(updated_on);

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.setTextColor(WHITE);

      display.print("P: ");
      display.println(payment);
      display.print("T: ");
      display.println(transfer);
      display.print("W: ");
      display.println(withdraw);

      display.setTextSize(1);
      display.setCursor(4, 56);
      display.println(updated_on);

      display.display();
      Serial.println();
    }
  }
}
