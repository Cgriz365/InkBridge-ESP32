#include "Inkbridge.h"

InkBridge ink(false);

#define SSID "your_ssid"
#define PASSWORD "your_password"

void connect_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("[WiFi] Connecting...");

  // Wait for valid IP with timeout
  int wifi_timeout = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_timeout < 200)// 20 second timeout
  {
    delay(100);
    wifi_timeout++;
    if (wifi_timeout % 10 == 0)
      Serial.print(".");
  }

  if (WiFi.localIP()[0] != 0)
  {
    Serial.println();
    Serial.print("[WiFi] Connected! IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println();
    Serial.println("[WiFi] Connection timeout!");
    return;
  }

  // NTP sync (non-blocking alternative)
  Serial.println("[NTP] Starting time sync...");
  configTime(-25200, 3600, "pool.ntp.org");

  // Check a few times but don't block forever
  for (int i = 0; i < 50; i++)
  {
    if (time(nullptr) > 100000)
    {
      Serial.println("[NTP] Synchronized!");
      return;
    }
    delay(100);
  }

  Serial.println("[NTP] Sync taking longer than expected, continuing...");
}

void setup()
{
  Serial.begin(115200);

  connect_wifi();
  if (ink.begin())
  {
    Serial.println("[Ink] Inkbase Initialized Successful! Account linked to: " + ink.getFriendlyId());
  }
}

void loop()
{
  Serial.println(ink.getWeather("Denver Colorado"));
  Serial.println(ink.getStock("AAPL"));
  Serial.println(ink.getNews("general"));
  Serial.println(ink.getCalendar("1d"));
  Serial.println(ink.getTravel("Boulder,CO", "Denver,CO", "driving"));
  Serial.println(ink.getCanvas("todo"));
  Serial.println(ink.spotifyRequest("/me/player/currently-playing", "GET", ""));

  delay(30000); // Wait 30 seconds before next loop
}

