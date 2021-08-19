/*
  Project name:Tabletop ornament by esp8266
  Time: 2020-03
  Author: Kelevc(skyqin1999)
  Aim: Gifts for good friends
  To-do:
    - Weather warning information
    - Countdown function
  About project:
    - tech blog: https://www.write-down.tech/junior/tabletop-ornament.html
    - github:
  Some Tool Links:
      Display library: https://github.com/olikraus/u8g2/wiki/u8g2reference
      Taichi Chinese ESP8266 Doc: http://www.taichi-maker.com/homepage/iot-development/iot-dev-reference/esp8266-c-plus-plus-reference/
      Arduinojson Library: https://arduinojson.org/v6/assistant/
      An API for obtaining geographic information from IP: https://ipstack.com/
      An API for weather: https://caiyunapp.com/
      My tech site to offers updates: https://www.write-down.tech/iot/
  README:
    (Because Chinese may be garbled, most of the program notes are written in English and translated by Baidu)
    Due to the limitation of memory size, it is impossible to store all the astronaut animation image information in the static area of the program, so I used an extra program to save the image in flash of esp8266.
    If you need to use astronaut boot animation, you need to use additional programs to upload images. After uploading, you can set ANI to true to use boot animation, otherwise boot is a simple loading effect.

    由于内存大小的限制，无法将所有的太空人动画图片信息存储到程序的静态区中，所以我是用了额外的程序将图片存到esp8266的flash中
    如果你需要使用太空人开机动画，你需要使用额外的程序上传图像，完成上传后你可以通过设置ANI为真来使用开机动画，否则开机就是简单的加载效果
*/

#include "IMG.h"

#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
//do not use this library: <NTPClient.h>, it has a bug when it forceupdate from sever!!!
//(Of course, you can also try to use the library function of esp8266)
#include <NTPClient_Generic.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//bin info
bool ANI = false;
bool C_F = 1;
char VERSION[10] = "b-0.1";
unsigned long BTN_COUNTER;
wl_status_t CONNECTED = WL_CONNECTED;

//server url and api part
char TIME_SERVER[] = "ntp1.aliyun.com";
char IOTAPI_URL[] = "www.write-down.tech";
char CAIYUNAPI_URL[] = "api.caiyunapp.com";
char IPSTACKAPI_URL[] = "api.ipstack.com";
char BILIBILIAPI_URL[] = "api.bilibili.com";
String BILIBILI_ID = "50488155";

String USR = "yezi";
String CAIYUNAPI_KEY = "b3b42YJU3A1F0W17";
String IPSTACKAPI_KEY = "6625799c241ebae3db45fc90fee6723a";

//slogan part
unsigned long TIME = 0;
unsigned long LAST_TIME = 0;
char MSG[127] = "The best preparation for tomorrow is doing your best today.";

// followers part

int FOLLOWERS = 0;

//time part
uint16_t SECS_SHOW_WEATHER = 50;
uint16_t SECS_FRESH_WEATHER = 55;
uint16_t SECS_SET_CONTRAST = 59;
uint16_t SECS_SHOW_NOTE = 5;
uint16_t TIMES_SHOW_NOTE = 8;

//clock part
unsigned long T = 0;
int HOURS = 8;
int MINS = 0;
int SECS = 0;
uint8_t PAGE = 1;

//show part
uint32_t TPF = 30;//Time Per Frames
int CONTRAST = 255;
#define ERR 0
#define WARN 1
#define INFO 2
#define TEXT 3
int16_t OFFSET = -(int16_t)u8g2.getDisplayWidth();

//weather part
#define SUN 0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4
unsigned long FRESH_TIME = 0;
int WIND_DIRECTION = 36;
int AQI = 100;
float TEMPERATURE = 15;
float TEMPERATURE_A = 13;
float HUMIDITY = 0.26;
float VISIBILITY = 7.3;
float PRESSURE = 101215.6;
float WIND_SPEED = 5.2;
float PROBABILITY[4] = {0, 0, 0, 0};
char SKYCON[20] = "LIGHT_RAIN";
char LIFE_ULT[10] = "Weak";
char LIFE_COM[10] = "Cool";
char AQI_DES[10] = "moderate";

char city[16] = "Beijing";
double latitude = 40.057;
double longitude = 116.184;
char DESCRIPTION[100] = "Drizzling outside, bring your umbrella";

//ntp time part
#define TIME_ZONE_OFFSET_HRS            (8)
#define NTP_UPDATE_INTERVAL_MS          60000L
WiFiUDP NTP_UDP;
NTPClient TIME_CLIENT(NTP_UDP, TIME_SERVER, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

// It has a bug if we use function WiFi.status() with some of the functions in the ESPhttpUpdate library, such as function ESPhttpUpdate.onProgress(),
// So I rewrote this function as follows
// This bug will cause a big delay in WiFi status update
wl_status_t wifi_status() {
  wl_status_t s = WiFi.status();
  if (s == WL_CONNECTED) {
    if (C_F) { //refresh
      WiFiClient c;
      HTTPClient http;
      String urlDate = "";
      Serial.println("Test Wifi_Status");
      http.begin(c, IOTAPI_URL, 80, urlDate);
      http.setTimeout(500);
      int httpCode = http.GET(); // send the request
      http.end();
      C_F = 0;
      if (httpCode > 0) {
        CONNECTED = WL_CONNECTED;
        return WL_CONNECTED;
      } else {
        CONNECTED = WL_DISCONNECTED;
        return WL_DISCONNECTED;
      }
    } else
      return CONNECTED;
  } else {
    return s;
  }
}


// basic draw function
void draw_progress(const char *title, const char *info, int percent) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_mf);

  u8g2.drawRFrame(8, 0, 112, 64, 4);
  u8g2.drawStr(50, 10, title);
  u8g2.drawLine(8, 13, 119, 13);
  u8g2.drawLine(8, 55, 119, 55);
  u8g2.drawFilledEllipse(14, 6, 2, 2, U8G2_DRAW_ALL);
  u8g2.drawFilledEllipse(22, 6, 2, 2, U8G2_DRAW_ALL);
  u8g2.drawFilledEllipse(30, 6, 2, 2, U8G2_DRAW_ALL);

  u8g2.drawRFrame(28, 24, 72, 16, 2);
  int p = 0.72 * percent;
  u8g2.drawBox(28, 25, p, 14);
  u8g2.drawStr(34, 50, info);
  u8g2.sendBuffer();

  return;
}

void draw_windows(int type, const char *s ) {
  u8g2.clearBuffer();
  u8g2.setContrast(250);

  Serial.println(s);
  int len = strlen(s), perline = 0;//14 char per line;18 char perline
  char *str =  (char*)s;
  char line[4][20] = {"", "", "", ""};
  if (len < 42) {
    perline = 14;
    u8g2.drawLine(8, 13, 119, 13);
    u8g2.drawLine(8, 55, 119, 55);
    u8g2.drawFilledEllipse(14, 6, 2, 2, U8G2_DRAW_ALL);
    u8g2.drawFilledEllipse(22, 6, 2, 2, U8G2_DRAW_ALL);
    u8g2.drawFilledEllipse(30, 6, 2, 2, U8G2_DRAW_ALL);
    u8g2.drawRFrame(8, 0, 112, 64, 4);
    u8g2.setFont(u8g2_font_6x10_mf);
    if (type == 1) {
      u8g2.drawStr(45, 10, "WARNING!");
      u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
      u8g2.drawGlyph(14, 43, 71);//WARNING iconic
    }
    else if (type == 2) {
      u8g2.drawStr(50, 10, "INFO");
      u8g2.setFont(u8g2_font_open_iconic_embedded_2x_t);
      u8g2.drawGlyph(14, 43, 65);//info iconic
    }
    else if (type == 0) {
      u8g2.drawStr(50, 10, "ERROR!!!");
      u8g2.setFont(u8g2_font_open_iconic_check_2x_t);
      u8g2.drawGlyph(14, 43, 66);//error iconic
    }

  } else {
    perline = 18;
    u8g2.drawRFrame(0, 0, 127, 63, 4);
    u8g2.drawRFrame(2, 2, 123, 58, 3);
  }


  if (len > 0 && len < 73) {
    u8g2.setFont(u8g2_font_6x10_mf);
    for (int i = 0; i < 4; i++) line[i][perline] = 0;
    for (int i = 0; i < perline; i++) line[0][i] = *(str++);
    Serial.println(line[0]);
    if (perline == 18)
      u8g2.drawStr(10, 20, line[0]);
    else
      u8g2.drawStr(33, 27, line[0]);
    if (len > perline) {
      for (int i = 0; i < perline; i++) line[1][i] = *(str++);
      Serial.println(line[1]);
      if (perline == 18)
        u8g2.drawStr(10, 20 + 12, line[1]);
      else
        u8g2.drawStr(33, 27 + 12, line[1]);
    }
    if (len > 2 * perline) {
      for (int i = 0; i < perline; i++) line[2][i] = *(str++);
      Serial.println(line[2]);
      if (perline == 18)
        u8g2.drawStr(10, 20 + 24, line[2]);
      else
        u8g2.drawStr(33, 27 + 24, line[2]);
    }
    if (len > 3 * perline) {
      Serial.println(line[3]);
      for (int i = 0; i < perline; i++) line[3][i] = *(str++);
      u8g2.drawStr(10, 20 + 36, line[3]);
    }


  }

  u8g2.sendBuffer();
  u8g2.setContrast(CONTRAST);

  return;
}


//OTA update function
void update_started() {
  draw_windows(INFO, "              WILL UPDATE!");
  delay(500);
  Serial.println("CALLBACK:  HTTP update process started");

  return;
}

void update_finished() {
  draw_windows(INFO, "               UPADATE DONE");
  delay(500);
  draw_windows(INFO, "               RESTART....");
  delay(500);
  u8g2.clearBuffer();
  u8g2.setContrast(10);
  u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);
  u8g2.drawGlyph(30, 60, 87);//info iconic
  u8g2.sendBuffer();
  Serial.println("CALLBACK:  HTTP update process finished");

  return;
}

void update_progress(int cur, int total) {
  String info = String(cur / 1024) + "k/" + String(total / 1024) + "k";
  draw_progress("UPDATE", info.c_str(), (int)(100 * cur / total));
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);

  return;
}

void update_error(int err) {
  draw_windows(ERR, "UPADATE ERR");
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);

  return;
}

void update() {

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    ESPhttpUpdate.onStart(update_started);
    ESPhttpUpdate.onEnd(update_finished);
    ESPhttpUpdate.onProgress(update_progress);
    ESPhttpUpdate.onError(update_error);
    String url = "http://skyqin1999.oss-cn-beijing.aliyuncs.com/IOT/";
    url += VERSION;
    url += ".bin";
    Serial.print("固件地址：");
    Serial.println(url);


    t_httpUpdate_return ret = ESPhttpUpdate.update(client, url.c_str());

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }

  return;
}

void vertify_update() {
  if (get_version()) {
    Serial.println("ready to update...");
    draw_windows(INFO, "FIND UPDATE   & WILL UPDATE");
    delay(1000);
    for (int i = 12; i > 0; i--) {
      String s = "PRESS TO OTA  MUST UPDATE   WITH CHARGE ";
      s += i / 2;
      Serial.println(s.c_str());
      draw_windows(INFO, s.c_str());
      if (digitalRead(0) == LOW) update();
      delay(500);
    }
  }

  return;
}

//esp8266HTTPClient.h
int get_slogan() {
  if (wifi_status() == WL_CONNECTED) {
    WiFiClient c;
    HTTPClient http; //Declare an object of class HTTPClient

    String urlDate = "/iot/slogan/slogan.php?name=" + USR;

    Serial.println("start refresh ");
    http.begin(c, IOTAPI_URL, 80, urlDate);
    http.setTimeout(500);
    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      Serial.println("httpcode ok");
      String input = http.getString(); //Get the request response input
      http.end();
      Serial.println(input);
      StaticJsonDocument<128> doc;
      DeserializationError error = deserializeJson(doc, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());

        return 0;
      }
      LAST_TIME = TIME;
      TIME = doc["time"]; // 1614248446
      strcpy(MSG, doc["msg"]); // "There is only one heroism in the world: to see the world as it is and ...
      if (LAST_TIME == TIME)
        return 0;
      else
        return 1;
    } else {
      http.end();
      Serial.println("httpcode bad");
      return 0;
    }
  } else return 0;
}

int get_follower_num() {
  if (wifi_status() == WL_CONNECTED) {
    WiFiClient c;
    HTTPClient http; //Declare an object of class HTTPClient

    String urlDate = "/x/relation/stat?vmid=" + BILIBILI_ID;

    Serial.println("start get follower numbers");
    http.begin(c, BILIBILIAPI_URL, 80, urlDate);
    http.setTimeout(500);
    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      Serial.println("httpcode ok");
      String input = http.getString(); //Get the request response input
      http.end();
      Serial.println(input);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }

      JsonObject data = doc["data"];
      FOLLOWERS = data["follower"];
      return 1;
    } else {
      http.end();
      Serial.println("httpcode bad");
      return 0;
    }
  } else return 0;
}

int get_version() {
  WiFiClient c;
  HTTPClient http; //Declare an object of class HTTPClient

  String urlDate = "/iot/version/version.php?name=" + USR;

  Serial.println("vertify update ");
  http.begin(c, IOTAPI_URL, 80, urlDate);
  http.setTimeout(500);
  int httpCode = http.GET(); // send the request

  if (httpCode > 0) // check the returning code
  {
    Serial.println("httpcode ok");
    String input = http.getString(); //Get the request response input
    http.end();
    Serial.println(input);

    StaticJsonDocument<32> doc;

    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return 0;
    }
    const char *version = doc["version"];
    Serial.print("doc[version] = ");
    Serial.println(version);
    if (strcmp(version, VERSION) != 0) {
      strcpy(VERSION, doc["version"]);
      return 1;
    }
    else
      return 0;


  } else {
    http.end();
    Serial.println("httpcode bad");
    return 0;
  }
}

int get_weather_prediction() {
  if (wifi_status() == WL_CONNECTED) {
    WiFiClient c;
    HTTPClient http; //Declare an object of class HTTPClient

    String urlDate = "/v2.5/" + CAIYUNAPI_KEY + "/" + String(longitude) + "," + String(latitude) + "/minutely.json?lang=en_US";

    Serial.println("start get predition weather info ");
    http.begin(c, CAIYUNAPI_URL, 80, urlDate);
    http.setTimeout(500);
    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      Serial.println("httpcode ok");
      String input = http.getString(); //Get the request response input
      http.end();
      Serial.println(input);
      // Stream& input;
      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }
      FRESH_TIME = doc["server_time"]; // 1616167610
      FRESH_TIME += 8 * 3600;//timeoffset
      Serial.print("sever time : ");
      Serial.println(FRESH_TIME);

      JsonObject result = doc["result"];
      JsonObject result_minutely = result["minutely"];
      JsonArray result_minutely_probability = result_minutely["probability"];
      PROBABILITY[0] = result_minutely_probability[0]; // 0
      PROBABILITY[1] = result_minutely_probability[1]; // 0
      PROBABILITY[2] = result_minutely_probability[2]; // 0
      PROBABILITY[3] = result_minutely_probability[3]; // 0
      strcpy(DESCRIPTION, result_minutely["description"]); // "Cloudy here but it's ...
      return 1;
    } else {
      http.end();
      Serial.println("httpcode bad");
      return 0;
    }
  } else return 0;
}

int get_weather_realtime() {
  if (wifi_status() == WL_CONNECTED) {
    WiFiClient c;
    HTTPClient http; //Declare an object of class HTTPClient

    String urlDate = "/v2.5/" + CAIYUNAPI_KEY + "/" + String(longitude) + "," + String(latitude) + "/realtime.json?lang=en_US";

    Serial.println("start get realtime weather info ");
    http.begin(c, CAIYUNAPI_URL, 80, urlDate);
    http.setTimeout(500);
    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      Serial.println("httpcode ok");
      String input = http.getString(); //Get the request response input
      http.end();
      Serial.println(input);
      // String input;
      DynamicJsonDocument doc(1600);
      DeserializationError error = deserializeJson(doc, input);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }

      if (strcmp(doc["status"], "ok") == 0) {
        JsonObject result_realtime = doc["result"]["realtime"];
        TEMPERATURE = result_realtime["temperature"]; // 14.79
        HUMIDITY = result_realtime["humidity"]; // 0.3
        strcpy(SKYCON, result_realtime["skycon"]); // "LIGHT_HAZE"
        VISIBILITY = result_realtime["visibility"]; // 7.9
        WIND_SPEED = result_realtime["wind"]["speed"]; // 2.52
        WIND_DIRECTION = result_realtime["wind"]["direction"]; // 36
        PRESSURE = result_realtime["pressure"]; // 101215.6
        TEMPERATURE_A = result_realtime["apparent_temperature"]; // 13.2
        JsonObject result_realtime_air_quality = result_realtime["air_quality"];
        AQI = result_realtime_air_quality["aqi"]["chn"]; // 132
        strcpy(AQI_DES, result_realtime_air_quality["description"]["chn"]);//moderate
        strcpy(LIFE_ULT, result_realtime["life_index"]["ultraviolet"]["desc"]);//Weak
        strcpy(LIFE_COM, result_realtime["life_index"]["comfort"]["desc"]);//Cool
      }
      return 1;
    } else {
      http.end();
      Serial.println("httpcode bad");
      return 0;
    }
  } else return 0;
}

int get_location() {
  if (wifi_status() == WL_CONNECTED) {
    WiFiClient c;
    HTTPClient http; //Declare an object of class HTTPClient

    String urlDate = "/check?access_key=" + IPSTACKAPI_KEY;

    Serial.println("start get location from ip ");
    http.begin(c, IPSTACKAPI_URL, 80, urlDate);
    http.setTimeout(500);
    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      Serial.println("httpcode ok");
      String input = http.getString(); //Get the request response input
      http.end();
      Serial.println(input);
      // Stream& input;

      StaticJsonDocument<1540> doc;

      DeserializationError error = deserializeJson(doc, input);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return 0;
      }
      strcpy(city, doc["city"]); // "Beijing"
      latitude = doc["latitude"]; // 39.91176223754883
      longitude = doc["longitude"]; // 116.37923431396484

      return 1;
    } else {
      http.end();
      Serial.println("httpcode bad");
      return 0;
    }
  }
  else return 0;
}


//draw function
void drawScrollString(int x_offset, int16_t offset, const char *s) {
  static char buf[36];  // should for screen with up to 256 pixel width
  size_t len;
  size_t char_offset = 0;
  u8g2_uint_t dx = 0;
  size_t visible = 0;
  len = strlen(s);
  if ( offset < 0 )
  {
    char_offset = (-offset) / 6;
    dx = offset + char_offset * 6;
    if ( char_offset >= u8g2.getDisplayWidth() / 6 )
      return;
    visible = u8g2.getDisplayWidth() / 6 - char_offset + 1;
    strncpy(buf, s, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.drawStr(char_offset * 6 - dx + x_offset, 7, buf);
  }
  else
  {
    char_offset = offset / 6;
    if ( char_offset >= len )
      return; // nothing visible
    dx = offset - char_offset * 6;
    visible = len - char_offset;
    if ( visible > u8g2.getDisplayWidth() / 6 + 1 )
      visible = u8g2.getDisplayWidth() / 6 + 1;
    strncpy(buf, s + char_offset, visible);
    buf[visible] = '\0';
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.drawStr(-dx + x_offset, 7, buf);

  }

}

void draw_state_scroll() {
  drawScrollString(16, OFFSET, MSG);
  int16_t len = strlen(MSG);
  OFFSET += 2;
  if ( OFFSET > len * 8 + 1 ) {
    C_F = 1;
    get_slogan();
    OFFSET = -(int16_t)u8g2.getDisplayWidth();
  }

}

void draw_precipitation() {
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(16, 12, "2H precipitation");

  u8g2.drawStr(3, 22, "00");
  u8g2.drawRFrame(28, 18, 90, 8, 2);
  u8g2.drawBox(28, 18, PROBABILITY[0] * 90, 8);

  u8g2.drawStr(3, 22 + 12, "30");
  u8g2.drawRFrame(28, 18 + 12, 90, 8, 2);
  u8g2.drawBox(28, 18 + 12, PROBABILITY[1] * 90, 8);

  u8g2.drawStr(3, 22 + 24, "60");
  u8g2.drawRFrame(28, 18 + 24, 90, 8, 2);
  u8g2.drawBox(28, 18 + 24, PROBABILITY[2] * 90, 8);

  u8g2.drawStr(3, 22 + 36, "90");
  u8g2.drawRFrame(28, 18 + 36, 90, 8, 2);
  u8g2.drawBox(28, 18 + 36, PROBABILITY[3] * 90, 8);


}

void draw_weather() {
  Serial.println("des: ");
  Serial.println(DESCRIPTION);
  if (TIME_CLIENT.getEpochMillis() % 8000 < 6000 || !(PROBABILITY[0] || PROBABILITY[1] || PROBABILITY[2] || PROBABILITY[3])) {
    int flag = 0;
    if ((TIME_CLIENT.getEpochMillis() % 2000) < 1000)
      flag = 0;
    else flag = 1;

    if (strcmp(SKYCON, "CLEAR_DAY")  == 0) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "CLEAR~");
      else u8g2.drawStr(45, 15, "(OvO)");
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 69);
    } else if (strcmp(SKYCON, "CLEAR_NIGHT") == 0) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "CLEAR~");
      else u8g2.drawStr(45, 15, "(OvO)");
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 66);
    } else if (strcmp(SKYCON, "PARTLY_CLOUDY_DAY") == 0 || strcmp(SKYCON, "PARTLY_CLOUDY_NIGHT") == 0) {
      u8g2.setFont(u8g2_font_7x13_mf);
      if (flag)u8g2.drawStr(20, 15, "PARTLY_CLOUDY");
      else u8g2.drawStr(20, 15, "(QUQ)");
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 65);
    } else if (strcmp(SKYCON, "CLOUDY") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "CLOUDY~");
      else u8g2.drawStr(45, 15, "(Q-Q)");
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 64);
    } else if (strcmp(SKYCON, "LIGHT_HAZE") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(20, 15, "LIGHT_HAZE!");
      else u8g2.drawStr(20, 15, "(TOT)");
      u8g2.setFont(u8g2_font_open_iconic_other_2x_t);
      u8g2.drawGlyph(2, 20, 67);
    } else if (strcmp(SKYCON, "MODERATE_HAZE") == 0 || strcmp(SKYCON, "HEAVY_HAZE") == 0) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(20, 15, "HEAVY_HAZE!!");
      else u8g2.drawStr(20, 15, "(TAT)");
      u8g2.setFont(u8g2_font_open_iconic_other_2x_t);
      u8g2.drawGlyph(2, 20, 68);
    } else if (strcmp(SKYCON, "LIGHT_RAIN") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(20, 15, "LIGHT_RAIN!");
      else u8g2.drawStr(20, 15, "(Q-Q)");
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 67);
    } else if (strcmp(SKYCON, "MODERATE_RAIN") == 0 || strcmp(SKYCON, "HEAVY_RAIN") == 0 || strcmp(SKYCON, "STORM_RAIN") == 0) {
      u8g2.setFont(u8g2_font_open_iconic_weather_2x_t);
      u8g2.drawGlyph(2, 20, 67);
      u8g2.drawGlyph(30, 20, 67);
      u8g2.drawGlyph(50, 20, 67);
    } else if (strcmp(SKYCON, "LIGHT_SNOW") == 0 ) {
      u8g2.setFont(u8g2_font_7x13_mf);
      if (flag)u8g2.drawStr(20, 15, "LIGHT_SNOW~");
      else u8g2.drawStr(20, 15, "(OvO)");
      u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
      u8g2.drawGlyph(2, 20, 78);
    } else if (strcmp(SKYCON, "MODERATE_SNOW") == 0 || strcmp(SKYCON, "HEAVY_SNOW") == 0 || strcmp(SKYCON, "STORM_SNOW") == 0) {
      u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
      u8g2.drawGlyph(2, 20, 78);
      u8g2.drawGlyph(30, 20, 78);
      u8g2.drawGlyph(50, 20, 78);
    } else if (strcmp(SKYCON, "FOG") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "FOG~");
      else u8g2.drawStr(45, 15, "(Q-Q)");
      u8g2.setFont(u8g2_font_open_iconic_other_2x_t);
      u8g2.drawGlyph(2, 20, 69);
    } else if (strcmp(SKYCON, "WIND") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "WIND");
      else u8g2.drawStr(45, 15, "(Q-Q)");
      u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
      u8g2.drawGlyph(2, 20, 65);
    } else if (strcmp(SKYCON, "DUST") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "DUST!");
      else u8g2.drawStr(45, 15, "(Q-Q)");
      u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
      u8g2.drawGlyph(2, 20, 78);
    } else if (strcmp(SKYCON, "SAND") == 0 ) {
      u8g2.setFont(u8g2_font_9x15_mf);
      if (flag)u8g2.drawStr(45, 15, "SAND!!!");
      else u8g2.drawStr(45, 15, "(T-T)");
      u8g2.setFont(u8g2_font_open_iconic_text_2x_t);
      u8g2.drawGlyph(2, 20, 71);
    }
    u8g2.setFont(u8g2_font_7x13_mf);
    String fl = "";
    if (flag)
      fl = String("T:") + (int)TEMPERATURE + String("°C  H:") + (int)(HUMIDITY * 100) + "%";
    else
      fl = String("t:") + (int)TEMPERATURE_A + String("°C  H:") + (int)(HUMIDITY * 100) + "%";
    u8g2.setCursor(2, 38);
    u8g2.print(fl.c_str());

    u8g2.setFont(u8g2_font_6x10_mf);
    String sl = "";
    if (flag)sl = String("AQI:") + AQI + String("    VIS:") + VISIBILITY + "Km" ;
    else sl = String("AQI:") + AQI + String("    BP:") + (int)PRESSURE + "pa";
    u8g2.drawStr(2, 52, sl.c_str());
    String tl = String("ULT:") + LIFE_ULT + String("  ") + LIFE_COM + " day~";
    u8g2.drawStr(2, 62, tl.c_str());
  } else {
    draw_precipitation();
  }


  // u8g2.setCursor(48+3, 44);
  // u8g2.print(degree);
  // u8g2.print("°C");   // requires enableUTF8Print()
}

void draw_State() {
  u8g2.setFont(u8g2_font_open_iconic_www_1x_t);
  if (wifi_status() == WL_CONNECTED) {
    u8g2.drawGlyph(0, 8, 72);
  } else if (wifi_status() == WL_DISCONNECTED ) {
    if ((TIME_CLIENT.getEpochMillis() % 1000) > 500)
      u8g2.drawGlyph(0, 8, 69);
  } else if (wifi_status() == WL_NO_SSID_AVAIL ) {
    if ((TIME_CLIENT.getEpochMillis() % 1000) > 500)
      u8g2.drawGlyph(0, 8, 79);
  } else if (wifi_status() == WL_IDLE_STATUS ) {
    if ((TIME_CLIENT.getEpochMillis() % 1000) > 500)
      u8g2.drawGlyph(0, 8, 81);
  }

  //show weather state

}

void draw_foot(bool location) {
  u8g2.setFont(u8g2_font_6x10_mf);
  if (location) {
    u8g2.drawStr(2, 64, (String(TIME_CLIENT.getMonth()) + "/" + TIME_CLIENT.getDay() ).c_str());
    u8g2.drawStr(30, 64, TIME_CLIENT.getDoW().c_str());
    u8g2.drawStr(66, 62, city);
    u8g2.setFont(u8g2_font_open_iconic_www_1x_t);
    u8g2.drawGlyph(56, 63, 64);
  } else {
    u8g2.drawStr(12, 64, (String(TIME_CLIENT.getMonth()) + "/" + TIME_CLIENT.getDay() ).c_str());
    u8g2.drawStr(90, 64, TIME_CLIENT.getDoW().c_str());
  }


}

void draw_clock() {
  if (wifi_status() == WL_CONNECTED) {
    TIME_CLIENT.update();
  }

  HOURS = TIME_CLIENT.getHours();
  MINS =  TIME_CLIENT.getMinutes();
  SECS =  TIME_CLIENT.getSeconds();


  u8g2.setFont(u8g2_font_logisoso30_tn);
  u8g2.drawStr(0, 46, TIME_CLIENT.getStrHours().c_str());
  u8g2.drawStr(45, 46, TIME_CLIENT.getStrMinutes().c_str());
  u8g2.drawStr(88, 46, TIME_CLIENT.getStrSeconds().c_str());

  if ((TIME_CLIENT.getEpochMillis() % 1000) < 333) {
    u8g2.drawStr(36, 46, ":");
    u8g2.drawStr(80, 46, " ");
  } else if ((TIME_CLIENT.getEpochMillis() % 1000) < 666) {
    u8g2.drawStr(36, 46, " ");
    u8g2.drawStr(80, 46, ":");
  }
  else {
    u8g2.drawStr(80, 46, " ");
    u8g2.drawStr(80, 46, " ");
  }



}

void draw_big_clock() {
  if (wifi_status() == WL_CONNECTED) {
    TIME_CLIENT.update();
  }

  HOURS = TIME_CLIENT.getHours();
  MINS =  TIME_CLIENT.getMinutes();
  SECS =  TIME_CLIENT.getSeconds();
  u8g2.setFont(u8g2_font_logisoso42_tn);
  u8g2.drawStr(0, 53, TIME_CLIENT.getStrHours().c_str());
  if ((TIME_CLIENT.getEpochMillis() % 1000) > 500)
    u8g2.drawStr(57, 53, " ");
  else
    u8g2.drawStr(57, 53, ":");
  u8g2.drawStr(74, 53, TIME_CLIENT.getStrMinutes().c_str());
}

void draw_followers() {
  u8g2.setFont(u8g2_font_logisoso30_tn);
  String f = String("F:") + FOLLOWERS;
  u8g2.drawStr(0, 53, f.c_str());
}

void draw_note() {
  if (SECS == SECS_SHOW_NOTE ) {
    if ( wifi_status() != WL_CONNECTED && PAGE < 3 ) {
      switch (wifi_status()) {
        case WL_IDLE_STATUS: draw_windows(INFO, "     WIFI       CONNECTING  "); break; //正在连接
        case WL_NO_SSID_AVAIL: draw_windows(ERR, "     WIFI        DISAPPER"); break;
        case WL_CONNECT_FAILED: draw_windows(ERR, "WIFI INFO     HAS CHANGED!!"); break; //连接失败
        case WL_DISCONNECTED: draw_windows(INFO, "NO USEFUL WIFITO CONNECT"); break;
        default: break;
      }
      delay(1000 * TIMES_SHOW_NOTE);
    }
    else if (MINS % 1 == 0) {
      Serial.print("---draw_note--->");
      Serial.println(DESCRIPTION);
      draw_windows(INFO, DESCRIPTION);
      Serial.println(PROBABILITY[0]);
      Serial.println(PROBABILITY[1]);
      Serial.println(PROBABILITY[2]);
      Serial.println(PROBABILITY[3]);

      if (PROBABILITY[0] || PROBABILITY[1] || PROBABILITY[2] || PROBABILITY[3]) {
        delay(800 * TIMES_SHOW_NOTE);
        u8g2.clearBuffer();
        draw_precipitation();
        u8g2.sendBuffer();
        delay(200 * TIMES_SHOW_NOTE);
      } else
        delay(1000 * TIMES_SHOW_NOTE);



    }

  }
}

unsigned char * load_img(int index, unsigned char * str) {
  EEPROMClass MYROM_0(2 * index + SECTOR_START);
  MYROM_0.begin(3008);
  for (int i = 0 ; i < 3008; i++) {
    str[i] = MYROM_0.read(i);
  }
  MYROM_0.end();

  EEPROMClass MYROM_1(2 * index + 1 + SECTOR_START);
  MYROM_1.begin(3008);
  for (int i = 0 ; i < 3008; i++) {
    str[ i + 3008] = MYROM_1.read(i);
  }
  MYROM_1.end();
  return str;
}

void loading_wifi(int i) {
  u8g2.clearBuffer();
  u8g2.drawXBMP(17, 0, 94, 64, load_img(IMG_PAGE, SPACEMAN));
  if (IMG_PAGE < 44)IMG_PAGE++;
  else IMG_PAGE = 0;
  u8g2.drawBox(0, 62, (int)i * 1.28, 2);
  u8g2.sendBuffer();
  delay(10);
}

//config wifi function
bool autoConfig() {
  WiFi.begin();
  //bool r = analogRead(A0)%2;
  for (int i = 0; i < 300; i++)
  {
    if (!ANI) {
      draw_progress("Set WiFi", "  LOADING", i / 3);
      delay(50);
    }
    else loading_wifi(i / 3);
    if (WiFi.status() == WL_CONNECTED)
    {
      if (!ANI)draw_progress("Set WiFi", "  LOADING", i / 3 + ((100 - i / 3) / 2));
      else loading_wifi(i / 3);
      delay(40);
      if (!ANI)draw_progress("Set WiFi", "  LOADING", 100);
      else loading_wifi(100);
      delay(100);

      draw_windows(INFO, "              wifi is ok");
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
    }
    else
    {
      Serial.println("AutoConfig Waiting......");

    }
  }
  Serial.println("AutoConfig Faild!" );
  return false;
  //WiFi.printDiag(Serial);
}

bool smartConfig() {
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig等待连接");
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 62, 62, QRCODE);
  u8g2.setFont(u8g2_font_8x13_mf);
  u8g2.drawStr(65, 15, "wx scan");

  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(1000);
  // 等待配网
  WiFi.beginSmartConfig();

  int times = 0;
  while (1)
  {
    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, 62, 62, QRCODE);
    if (times % 2)
      u8g2.drawXBMP(76, 26, 28, 32, PANDA_2); //28,32
    else
      u8g2.drawXBMP(76, 26, 29, 31, PANDA_1); //29,31

    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr(65, 15, "wx scan");
    u8g2.sendBuffer();
    Serial.println("...");

    if (WiFi.smartConfigDone()) {

      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      if (WiFi.getAutoConnect() == true) {
        Serial.println("已设置自连接");
      }
      else
        Serial.println("未设置连接");

      WiFi.setAutoConnect(true);//启用自动连接模式
      delay(50);//启用自动连接后再检查一次，确定设置变化

      Serial.println("再次检查自动连接设置状态");
      if (WiFi.getAutoConnect() == true) {
        Serial.println("已设置自连接");
      }
      else
        Serial.println("未设置连接");


      u8g2.clearBuffer();
      u8g2.sendBuffer();
      int flag = 1;
      while (flag) {
        switch (WiFi.status()) {
          case WL_IDLE_STATUS: draw_windows(INFO, "              CONNECTING"); break; //正在连接
          case WL_CONNECTED: draw_windows(INFO, "               WIFI DONE"); flag = 0; break; //连接
          case WL_CONNECT_FAILED:
            draw_windows(ERR, "               PWD ERROR");
            delay(2000);
            WiFi.stopSmartConfig();
            draw_windows(INFO, "              Scan Again");
            delay(2000);
            WiFi.beginSmartConfig();
            flag = 0;
            break;//连接失败
          case WL_DISCONNECTED: draw_windows(INFO, "              WAITING.."); break;
          default: break;
        }
        delay(100);
      }


      if (WiFi.status() == WL_CONNECTED)
        break;
      else
        times = 0;
    }
    delay(500);
    times++;
    if (times > 180) {
      Serial.println("SmartConfig Timeout!!!");
      draw_windows(ERR, "              TIME OUT!");
      delay(1000);
      draw_windows(INFO, "  START WITH   NO WIFI...");
      delay(1000);
      return false;
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

//setup function
bool wifi_setup() {
  if (!autoConfig())
  {
    Serial.println("Start smartConfig");
    return smartConfig();
  }
  else
    return true;
}

void ntp_setup() {
  TIME_CLIENT.begin();
  TIME_CLIENT.update();
  while (!TIME_CLIENT.updated()) {
    Serial.println("******NOT UPDATED******");
    delay(50);
    Serial.println("******UPDATED TIME******");
    TIME_CLIENT.update();
  }


}

void system_set() {
  //switch contrast
  if (MINS == 0 && SECS == SECS_SET_CONTRAST && TIME_CLIENT.getEpochMillis() % 1000 < 100) {
    Serial.println("---system_set--->change system contrast!!");
    switch (HOURS) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6: CONTRAST = 20; break;
      case 7: CONTRAST = 80; break;
      case 8: CONTRAST = 150; break;
      case 9: CONTRAST = 180; break;
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
      case 16:
      case 17: CONTRAST = 250; break;
      case 18: CONTRAST = 200; break;
      case 19:
      case 20: CONTRAST = 100; break;
      case 21: CONTRAST = 80; break;
      case 22: CONTRAST = 60; break;
      case 23: CONTRAST = 30; break;
    }
    u8g2.setContrast(CONTRAST);
  }
  //fresh weather info
  if (TIME_CLIENT.getEpochTime() - FRESH_TIME > 300) {
    get_weather_prediction();
    get_weather_realtime();
  }

  //auto switch page between 0&1
  if (PAGE == 1) {
    if (MINS % 3 == 0 && SECS == SECS_SHOW_WEATHER && millis() - BTN_COUNTER > 1100) {
      BTN_COUNTER = millis();
      PAGE = !PAGE;
    }
  } else if (PAGE == 0) {
    if (millis() - BTN_COUNTER > 20000) {
      BTN_COUNTER = millis();
      PAGE = !PAGE;
    }
  }

  //Response button to switch state
  if (digitalRead(0) == LOW && millis() - BTN_COUNTER > 300) {
    Serial.println("---system_set--->Page switch");
    BTN_COUNTER = millis();
    PAGE++;
    if (PAGE == 5)
      PAGE = 0;
  }
}

void frame() {
  u8g2.clearBuffer();
  if (PAGE == 0) {
    draw_weather();
  } else if (PAGE == 1) {
    draw_state_scroll();
    draw_State();
    draw_clock();
    draw_foot(true);
    draw_note();
  } else if (PAGE == 2) {
    draw_clock();
    draw_foot(false);
    draw_note();
  } else if (PAGE == 3) {
    draw_big_clock();
    draw_note();
  } else if (PAGE == 4) {
    draw_followers();
    draw_note();
  }
  u8g2.sendBuffer();
}


void setup(void) {
  Serial.begin(115200);
  pinMode(0, INPUT);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setContrast(CONTRAST);

  wifi_setup();
  Serial.println("wifi set is done...");
  Serial.print("wifi.status: ");
  Serial.println(WiFi.status());

  Serial.println("Start Init...");
  Serial.println("ntp Init...");
  if (WiFi.status() == WL_CONNECTED) {
    ntp_setup();
    Serial.println("version Init...");
    vertify_update();
    Serial.println("address Init...");
    get_location();
    Serial.println("slogan Init...");
    get_slogan();
    Serial.println("bilibili follower Init...");
    get_follower_num();
    Serial.print("ntp time : ");
    Serial.println(TIME_CLIENT.getEpochTime());
    Serial.println("realtime weather Init...");
    get_weather_realtime();
    delay(200);
    Serial.println("weather prediction Init...");
    get_weather_prediction();

  }
}

void loop(void) {
  if (millis() - T > TPF) {
    T = millis();
    system_set();
    frame();
  }
}
