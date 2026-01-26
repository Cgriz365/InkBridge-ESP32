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
  double getWeatherTemperature();
  String getWeatherCondition();
  String getWeatherDescription();
  String getWeatherLocation();

  Response getWeatherForecast(String location = "", int days = 3);
  int getWeatherForcastDayCount();
  String getWeatherForcastLocation();
  String getWeatherForecastDate(int index);
  String getWeatherForecastMinTemp(String date);
  String getWeatherForecastMinTemp(int index);
  String getWeatherForecastMaxTemp(String date);
  String getWeatherForecastMaxTemp(int index);
  String getWeatherForecastCondition(String date);
  String getWeatherForecastCondition(int index);
  String getWeatherForcastTrend();

  Response getWeatherHistory(String location = "", String date = ""); // date format: YYYY-MM-DD
  int getWeatherHistoryCount();
  String getWeatherHistoryLocation();
  String getWeatherHistoryDate(int index);
  String getWeatherHistoryAvgTemp(String date);
  String getWeatherHistoryAvgTemp(int index);
  String getWeatherHistoryCondition(String date);
  String getWeatherHistoryCondition(int index);
  String getWeatherHistoryTrend();

  Response getAstronomy(String location = ""); 
  String getAstronomySunrise();
  String getAstronomySunset();
  String getAstronomyMoonrise();
  String getAstronomyMoonset();
  String getAstronomyLocation();
  String getAstronomyMoonPhase();
  int getAstronomyMoonIllumination();
  bool getAstronomyIsDaytime();

  Response weatherHistory;
  Response weatherForecast;
  Response weather;
  Response astronomy;
#endif

#if INK_ENABLE_STOCKS
  Response getStock(String symbol = "");
  double getStockPrice();
  double getStockPercent();
  String getStockSymbol();
  double getStockHigh();
  double getStockLow();

  Response getStockArray(String symbol = "", int days = 7);
  Response stocks;
#endif

#if INK_ENABLE_CRYPTO
  Response getCrypto(String symbol);
  double getCryptoPrice();
  double getCryptoPercent();
  String getCryptoSymbol();
  String getCryptoName();

  Response getCryptoArray(String symbol, int days);
  Response crypto;
#endif

#if INK_ENABLE_NEWS
  Response getNews(String category);
  int getNewsArticleCount();
  String getNewsArticleTitle(int index);
  String getNewsArticleSource(int index);

  Response news;
#endif

#if INK_ENABLE_CALENDAR
  Response getCalendar(String range);
  int getCalendarEventCount();
  String getCalendarEventTime(int index);
  String getCalendarEventTitle(int index);
  String getCalendarEventLocation(int index);
  Response calendar;
#endif

#if INK_ENABLE_TRAVEL
  Response getTravel(String origin, String destination, String mode);
  String getTravelDuration();
  String getTravelDistance();
  String getTravelOrigin();
  String getTravelDestination();
  String getTravelMode();
  Response travel;
#endif

#if INK_ENABLE_CANVAS
  Response getCanvas(String type, String domain, String canvasApiKey); // type: "todo" or "grades"
  Response getCanvasAssignment(String id);
  Response getCanvasAssignment(int index);
  String getCanvasAssignmentName(String id);
  String getCanvasAssignmentName(int index);
  String getCanvasAssignmentDueDate(String id);
  String getCanvasAssignmentDueDate(int index);
  String getCanvasAssignmentType(String id);
  String getCanvasAssignmentType(int index);
  Response getCanvasGradeSet(String course);
  Response getCanvasGradeSet(int index);
  String getCanvasLetterGrade(String course);
  String getCanvasLetterGrade(int index);
  int getCanvasNumericGrade(String course);
  int getCanvasNumericGrade(int index);
  double getGPAEstimate(bool weighted = false, double Aplus = 4.0, double A = 4.0, double Aminus = 3.7,
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