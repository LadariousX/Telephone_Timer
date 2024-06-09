#include <env_build.h>


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


void buildDisplay() {

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.clearDisplay();
    display.display();
}

void buildWifi(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2, 2);
  display.println("Connecting to ");
  display.print(WIFI_SSID);
  display.display();

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }
}

void buildClocks(){
  display.clearDisplay();

  timeClient.begin();
  timeClient.setTimeOffset(-18000); // CDT offset from UTC
  timeClient.update();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    display.println("Couldn't est RTC!");
    display.display();
    Serial.flush();
    while (1) delay(10);
  }

  time_t epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime((time_t *)&epochTime);
  
  int X_yr = ptm->tm_year + 1900;
  int X_mo = ptm->tm_mon + 1;
  int X_day = ptm->tm_mday;
  int X_hr = timeClient.getHours();
  int X_min = timeClient.getMinutes();
  int X_sec = timeClient.getSeconds();

  rtc.adjust(DateTime(X_yr, X_mo, X_day, X_hr, X_min, X_sec));
}