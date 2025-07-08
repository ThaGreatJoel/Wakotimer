#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_HACKATIME_API_KEY";
const char* host = "hackatime.hackclub.com";
const char* user = "YOUR_HACKATIME_USERNAME";

LiquidCrystal lcd(D1, D2, D3, D4, D5, D6);

const int buzzer = D7;
const int buttonPin = D0;

int modeIndex = 0;
unsigned long lastButtonPress = 0;
String lastProject = "";
int lastSeconds = -1;

void setup() {
  lcd.begin(16, 2);
  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(buzzer, LOW);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    lcd.print(".");
    tries++;
  }
  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.print("WiFi Connected");
    delay(1000);
  } else {
    lcd.print("WiFi Failed");
    while (true);
  }
}

void loop() {
  if (digitalRead(buttonPin) == LOW && millis() - lastButtonPress > 400) {
    modeIndex = (modeIndex + 1) % 3;
    lastButtonPress = millis();
    lcd.clear();
  }

  if (modeIndex == 0) showLiveProject();
  else if (modeIndex == 1) showMostUsedProject();
  else if (modeIndex == 2) showTotalTime();

  delay(10000);
}

void buzz(bool error) {
  if (error) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
  } else {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
  }
}

String getApiResponse() {
  WiFiClientSecure client;
  client.setInsecure();
  if (!client.connect(host, 443)) {
    lcd.clear();
    lcd.print("API Error");
    buzz(true);
    delay(3000);
    return "";
  }

  client.println(String("GET /api/v1/users/") + user + "/stats HTTP/1.1");
  client.println("Host: hackatime.hackclub.com");
  client.println(String("Authorization: Bearer ") + apiKey);
  client.println("Connection: close");
  client.println();

  String response = "";
  while (client.available() == 0) delay(100);
  while (client.available()) response += client.readStringUntil('\n');
  return response;
}

void showLiveProject() {
  String res = getApiResponse();
  if (res == "") return;

  int projStart = res.indexOf("\"name\":\"");
  int projEnd = res.indexOf("\"", projStart + 8);
  int secStart = res.indexOf("\"seconds\":", projEnd);
  int secEnd = res.indexOf("}", secStart);

  if (projStart != -1 && secStart != -1) {
    String project = res.substring(projStart + 8, projEnd);
    int seconds = res.substring(secStart + 10, secEnd).toInt();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(project.length() > 16 ? project.substring(0, 16) : project);
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(seconds));

    if (project != lastProject || seconds > lastSeconds) {
      buzz(false);
      lastProject = project;
      lastSeconds = seconds;
    }

  } else {
    lcd.clear();
    lcd.print("Parse Error");
    buzz(true);
  }
}

void showMostUsedProject() {
  String res = getApiResponse();
  if (res == "") return;

  int start = res.indexOf("\"projects\":[{");
  int nameStart = res.indexOf("\"name\":\"", start);
  int nameEnd = res.indexOf("\"", nameStart + 8);
  int secStart = res.indexOf("\"seconds\":", nameEnd);
  int secEnd = res.indexOf("}", secStart);

  if (nameStart != -1 && secStart != -1) {
    String project = res.substring(nameStart + 8, nameEnd);
    int seconds = res.substring(secStart + 10, secEnd).toInt();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Top: " + project.substring(0, 10));
    lcd.setCursor(0, 1);
    lcd.print("Time: " + String(seconds) + "s");
  } else {
    lcd.clear();
    lcd.print("Parse Error");
    buzz(true);
  }
}

void showTotalTime() {
  String res = getApiResponse();
  if (res == "") return;

  int totalStart = res.indexOf("\"total_seconds\":");
  if (totalStart != -1) {
    int secEnd = res.indexOf(",", totalStart);
    int seconds = res.substring(totalStart + 17, secEnd).toInt();
    int hours = seconds / 3600;
    int mins = (seconds % 3600) / 60;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Total Coded");
    lcd.setCursor(0, 1);
    lcd.print(String(hours) + "h " + String(mins) + "m");
  } else {
    lcd.clear();
    lcd.print("Parse Error");
    buzz(true);
  }
}
