#ifndef INKBRIDGE_H
#define INKBRIDGE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NVSManager.h>

class InkBridge
{
public:
  InkBridge(bool resetDevice = false);
  InkBridge(const char *apiUrl);

  bool begin();
  bool isRegistered();
  void setApiKey(String key);
  bool registerDevice();
  bool loadConfig();

  String getDeviceId();
  String getFriendlyId();
  String getApiKey();

  String getWeather(String location = "");
  String getStock(String symbol = "");
  String getNews(String category = "general");
  String getCalendar(String range = "1d");
  String getTravel(String origin = "", String destination = "", String mode = "driving");
  String getCanvas(String type = "todo"); // type: "todo" or "grades"
  String spotifyRequest(String endpoint, String method = "GET", String body = "");

private:
  String _apiUrl;
  String _uid;
  String _deviceId;
  String _apiKey;
  String _friendlyName;
  bool _resetDevice;

  // Internal helper to perform HTTP GET
  String sendRequest(String endpoint, String method, String payload);
  String getRequest(String endpoint, bool includeApiKey);
};

#endif