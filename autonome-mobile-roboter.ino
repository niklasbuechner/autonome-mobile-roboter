#include <LiquidCrystal_I2C.h>
#include "WiFi.h"
#include <HTTPClient.h>

const char* SSID = "TODO-SSID-hier";
const char* SSID_PW = "TODO-SSID-PW-hier";

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  Serial.println("setup");

  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }

  lcd.init(); // initialize the lcd
  lcd.backlight(); // Turn on backlight
  lcd.print("Wifi?");// Print a message to the LCD

  WiFi.mode(WIFI_STA);
  delay(100);

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  int networkAmount = WiFi.scanNetworks();
  for (int counter = 0; counter < networkAmount; counter++) {
    Serial.printf("%s %s %d\n", WiFi.SSID(counter), SSID, WiFi.SSID(counter) == String(SSID));
  }

  WiFi.begin(SSID, SSID_PW);
  lcd.setCursor(0, 1);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    lcd.print(".");
    delay(1000);
  }

  lcd.setCursor(0,0);
  lcd.print(WiFi.localIP());
  lcd.print("    ");
}


void loop() {
  Serial.println("Loop");

  // (note: line 1 is the second row, since counting begins with 0):
  // lcd.setCursor(0, 1);// set the cursor to column 0, line 1
  // print the number of seconds since reset:
  // lcd.print("Counter:");
  // lcd.print(millis() / 1000);

  // lcd.setCursor(0, 1);
  // lcd.print("Searching...");

  // int networks = WiFi.scanNetworks();
  // lcd.setCursor(7, 0);
  // lcd.print(networks);

  // for (int counter = 0; counter < networks; counter++) {
  //   printWifiName(networks, counter);
  //   Serial.println(WiFi.SSID(counter));

  //   delay(1000);
  // }

  requestData();

  delay(1000);
}

void requestData() {
  HTTPClient http;
  http.begin("http://172.20.10.9:4567/hello-from-esp");
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);

    lcd.setCursor(0, 1);
    lcd.print(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);

  if (Wire.endTransmission() == 0) {
      return true;
  }

  return false;
}