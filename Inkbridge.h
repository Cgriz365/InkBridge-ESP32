#ifndef INKBRIDGE_H
#define INKBRIDGE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NVSManager.h>

#define INK_ENABLE_WEATHER 1
#define INK_ENABLE_STOCKS 1
#define INK_ENABLE_CRYPTO 1
#define INK_ENABLE_NEWS 1
#define INK_ENABLE_CALENDAR 1
#define INK_ENABLE_TRAVEL 1
#define INK_ENABLE_CANVAS 1
#define INK_ENABLE_SPOTIFY 1

struct Response {
  String status;
  JsonDocument data;
};

class InkBridge
{
public:
  InkBridge();
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
  String getApiUrl();
  String getUID();

#if INK_ENABLE_WEATHER
  Response getWeather(String location = "");
  double getWeatherTemperature(String location = "");
  String getWeatherCondition(String location = "");
  String getWeatherDescription(String location = "");
  String getWeatherLocation(String location = "");

  Response getWeatherForecast(String location = "", int days = 3);
  int getWeatherForcastDayCount(String location = "", int days = 3);
  String getWeatherForcastLocation(String location = "", int days = 3);
  String getWeatherForecastDate(int index, String location = "", int days = 3);
  String getWeatherForecastMinTemp(String date, String location = "", int days = 3);
  String getWeatherForecastMinTemp(int index, String location = "", int days = 3);
  String getWeatherForecastMaxTemp(String date, String location = "", int days = 3);
  String getWeatherForecastMaxTemp(int index, String location = "", int days = 3);
  String getWeatherForecastCondition(String date, String location = "", int days = 3);
  String getWeatherForecastCondition(int index, String location = "", int days = 3);
  String getWeatherForcastTrend(String location = "", int days = 3);

  Response getWeatherHistory(String location = "", String date = ""); // date format: YYYY-MM-DD
  int getWeatherHistoryCount(String location = "", String date = "");
  String getWeatherHistoryLocation(String location = "", String date = "");
  String getWeatherHistoryDate(int index, String location = "", String date = "");
  String getWeatherHistoryAvgTemp(String date, String location = "");
  String getWeatherHistoryAvgTemp(int index, String location = "", String date = "");
  String getWeatherHistoryCondition(String date, String location = "");
  String getWeatherHistoryCondition(int index, String location = "", String date = "");
  String getWeatherHistoryTrend(String location = "", String date = "");

  Response getAstronomy(String location = ""); 
  String getAstronomySunrise(String location = "");
  String getAstronomySunset(String location = "");
  String getAstronomyMoonrise(String location = "");
  String getAstronomyMoonset(String location = "");
  String getAstronomyLocation(String location = "");
  String getAstronomyMoonPhase(String location = "");
  int getAstronomyMoonIllumination(String location = "");
  bool getAstronomyIsDaytime(String location = "");

  Response weatherHistory;
  Response weatherForecast;
  Response weather;
  Response astronomy;
#endif

#if INK_ENABLE_STOCKS
  Response getStock(String symbol = "");
  double getStockPrice(String symbol = "");
  double getStockPercent(String symbol = "");
  String getStockSymbol(String symbol = "");
  double getStockHigh(String symbol = "");
  double getStockLow(String symbol = "");

  Response getStockArray(String symbol = "", int days = 7);
  Response stocks;
#endif

#if INK_ENABLE_CRYPTO
  Response getCrypto(String symbol);
  double getCryptoPrice(String symbol = "");
  double getCryptoPercent(String symbol = "");
  String getCryptoSymbol(String symbol = "");
  String getCryptoName(String symbol = "");

  Response getCryptoArray(String symbol, int days);
  Response crypto;
#endif

#if INK_ENABLE_NEWS
  Response getNews(String category);
  int getNewsArticleCount(String category = "general");
  String getNewsArticleTitle(int index, String category = "general");
  String getNewsArticleSource(int index, String category = "general");

  Response news;
#endif

#if INK_ENABLE_CALENDAR
  Response getCalendar(String range);
  int getCalendarEventCount(String range = "1d");
  String getCalendarEventTime(int index, String range = "1d");
  String getCalendarEventTitle(int index, String range = "1d");
  String getCalendarEventLocation(int index, String range = "1d");
  Response calendar;
#endif

#if INK_ENABLE_TRAVEL
  Response getTravel(String origin, String destination, String mode);
  String getTravelDuration(String origin = "", String destination = "", String mode = "driving");
  String getTravelDistance(String origin = "", String destination = "", String mode = "driving");
  String getTravelOrigin(String origin = "", String destination = "", String mode = "driving");
  String getTravelDestination(String origin = "", String destination = "", String mode = "driving");
  String getTravelMode(String origin = "", String destination = "", String mode = "driving");
  Response travel;
#endif

#if INK_ENABLE_CANVAS
  Response getCanvas(String type, String domain, String canvasApiKey); // type: "todo" or "grades"
  Response getCanvasAssignment(String id, String domain = "", String canvasApiKey = "");
  Response getCanvasAssignment(int index, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentName(String id, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentName(int index, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentDueDate(String id, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentDueDate(int index, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentType(String id, String domain = "", String canvasApiKey = "");
  String getCanvasAssignmentType(int index, String domain = "", String canvasApiKey = "");
  Response getCanvasGradeSet(String course, String domain = "", String canvasApiKey = "");
  Response getCanvasGradeSet(int index, String domain = "", String canvasApiKey = "");
  String getCanvasLetterGrade(String course, String domain = "", String canvasApiKey = "");
  String getCanvasLetterGrade(int index, String domain = "", String canvasApiKey = "");
  int getCanvasNumericGrade(String course, String domain = "", String canvasApiKey = "");
  int getCanvasNumericGrade(int index, String domain = "", String canvasApiKey = "");
  double getGPAEstimate(bool weighted = false, String domain = "", String canvasApiKey = "", double Aplus = 4.0, double A = 4.0, double Aminus = 3.7,
                        double Bplus = 3.3, double B = 3.0, double Bminus = 2.7,
                        double Cplus = 2.3, double C = 2.0, double Cminus = 1.7,
                        double Dplus = 1.3, double D = 1.0, double Dminus = 0.7,
                        double F = 0.0);

  Response canvasGrades;
  Response canvasTodos;
#endif

#if INK_ENABLE_SPOTIFY
  Response spotifyRequest(String endpoint, String method = "GET", String body = "");
  Response getSpotifyAlbums(int limit = 5, int offset = 0);
  Response getSpotifyPlaylists(int limit = 5, int offset = 0);
  Response getSpotifyLikedSongs(int limit = 5, int offset = 0);
  Response getSpotifyFollowedArtists(int limit = 5, String after = "");
  Response getSpotifyDevices();
  Response spotifyPlayback(String action, String uri = "", int volume = -1, int position = -1, String state = "", String targetDeviceId = "");
#endif

private:
  String _apiUrl;
  String _uid;
  String _deviceId;
  String _apiKey;
  String _friendlyName;
  bool _resetDevice;

  // Internal helper to perform HTTP GET
  Response sendRequest(String endpoint, String method, String payload);
  Response getRequest(String endpoint, bool includeApiKey);
};

#endif