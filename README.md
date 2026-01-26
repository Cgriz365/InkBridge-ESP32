# InkBridge Library Documentation
## Built for [InkBridge](https://inkbase01.web.app), [Github Repo](https://github.com/Cgriz365/InkBridge) ##

A comprehensive Arduino library for ESP32 devices that provides seamless integration with the InkBase cloud API, enabling access to weather, stocks, crypto, news, calendar, travel, Spotify, and custom canvas data.

## Features

- **Auto-registration**: Automatic device registration using MAC address
- **Persistent Storage**: NVS-based configuration storage for API keys and device credentials
- **HTTPS Support**: Secure communication with cloud APIs
- **Multiple Data Sources**: Weather, stocks, crypto, news, calendar, travel, and Spotify integration
- **Retry Logic**: Built-in HTTP request retry mechanism
- **Memory Efficient**: Modular feature flags to disable unused integrations
- **Factory Reset**: Option to reset device configuration

## Installation

1. Download the library and place it in your Arduino `libraries` folder
2. Include the required dependencies:
   - `WiFi.h` or `WiFiClientSecure.h`
   - `HTTPClient.h`
   - `ArduinoJson.h`
   - `NVSManager.h` (included with library)

### Feature Flags
To save memory, you can disable unused integrations by modifying `Inkbridge.h`:
```cpp
#define INK_ENABLE_WEATHER 1
#define INK_ENABLE_STOCKS 1
#define INK_ENABLE_CRYPTO 1
#define INK_ENABLE_NEWS 1
#define INK_ENABLE_CALENDAR 1
#define INK_ENABLE_TRAVEL 1
#define INK_ENABLE_CANVAS 1
#define INK_ENABLE_SPOTIFY 1
```

## Quick Start
```cpp
#include "Inkbridge.h"

// Create instance (default API endpoint)
InkBridge ink;

// Or with custom API endpoint
// InkBridge ink("https://your-api-endpoint.com");

// Or with factory reset enabled
// InkBridge ink(true);

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi first
    WiFi.begin("your-ssid", "your-password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    
    // Initialize InkBridge
    if (ink.begin()) {
        Serial.println("InkBridge initialized successfully!");
    }
}
```

## API Reference

### Response Object
Most methods return a `Response` struct containing the status and parsed JSON data:
```cpp
struct Response {
  String status;      // e.g., "OK", "HTTP_ERROR_404", "WIFI_DISCONNECTED"
  JsonDocument data;  // Parsed ArduinoJson document
};
```

### Initialization

#### `InkBridge(bool resetDevice = false)`
Constructor with optional factory reset.
- **resetDevice**: If `true`, clears all stored configuration on initialization

#### `InkBridge(const char *apiUrl)`
Constructor with custom API endpoint.
- **apiUrl**: Custom API endpoint URL

#### `bool begin()`
Initializes the library, loads stored configuration, and auto-registers if needed.
- **Returns**: `true` if device is registered, `false` otherwise

### Registration

#### `bool isRegistered()`
Checks if device has valid API credentials.
- **Returns**: `true` if API key, device ID, and UID are set

#### `bool registerDevice()`
Manually registers the device with the InkBase API.
- **Returns**: `true` if registration successful

#### `void setApiKey(String key)`
Manually set API key (bypasses auto-registration).
- **key**: API key string

#### `String getApiKey()`
Retrieves the current API key.
- **Returns**: API key string

#### `String getDeviceId()`
Retrieves the device ID (MAC address).
- **Returns**: Device ID string

#### `String getFriendlyId()`
Retrieves the friendly user ID.
- **Returns**: Friendly name string

### Weather

#### `Response getWeather(String location = "")`
Fetches current weather data.
- **location**: Optional location override
- **Returns**: Response object

**Helpers:**
- `double getWeatherTemperature(String location = "")`
- `String getWeatherCondition(String location = "")`
- `String getWeatherDescription(String location = "")`
- `String getWeatherLocation(String location = "")`

#### `Response getWeatherForecast(String location = "", int days = 3)`
Fetches weather forecast.

**Helpers:**
- `int getWeatherForcastDayCount(String location = "", int days = 3)`
- `String getWeatherForcastLocation(String location = "", int days = 3)`
- `String getWeatherForecastDate(int index, String location = "", int days = 3)`
- `String getWeatherForecastMinTemp(int index, String location = "", int days = 3)` or `(String date, String location = "", int days = 3)`
- `String getWeatherForecastMaxTemp(int index, String location = "", int days = 3)` or `(String date, String location = "", int days = 3)`
- `String getWeatherForecastCondition(int index, String location = "", int days = 3)` or `(String date, String location = "", int days = 3)`
- `String getWeatherForcastTrend(String location = "", int days = 3)`

#### `Response getWeatherHistory(String location = "", String date = "")`
Fetches historical weather data.
- **location**: Optional location override
- **date**: Date in YYYY-MM-DD format

**Helpers:**
- `int getWeatherHistoryCount(String location = "", String date = "")`
- `String getWeatherHistoryLocation(String location = "", String date = "")`
- `String getWeatherHistoryDate(int index, String location = "", String date = "")`
- `String getWeatherHistoryAvgTemp(int index, String location = "", String date = "")` or `(String date, String location = "")`
- `String getWeatherHistoryCondition(int index, String location = "", String date = "")` or `(String date, String location = "")`
- `String getWeatherHistoryTrend(String location = "", String date = "")`

#### `Response getAstronomy(String location = "")`
Fetches astronomy data.

**Helpers:**
- `String getAstronomySunrise(String location = "")`
- `String getAstronomySunset(String location = "")`
- `String getAstronomyMoonrise(String location = "")`
- `String getAstronomyMoonset(String location = "")`
- `String getAstronomyLocation(String location = "")`
- `String getAstronomyMoonPhase(String location = "")`
- `int getAstronomyMoonIllumination(String location = "")`
- `bool getAstronomyIsDaytime(String location = "")`

### Stocks & Crypto

#### `Response getStock(String symbol = "")`
Fetches current stock data.
- **symbol**: Optional stock symbol (e.g., "AAPL")
- **Returns**: Response object

**Helpers:**
- `double getStockPrice(String symbol = "")`
- `double getStockPercent(String symbol = "")`
- `String getStockSymbol(String symbol = "")`
- `double getStockHigh(String symbol = "")`
- `double getStockLow(String symbol = "")`

#### `Response getCrypto(String symbol = "")`
Fetches current cryptocurrency data.
- **symbol**: Optional crypto symbol (e.g., "BTC")
- **Returns**: Response object

**Helpers:**
- `double getCryptoPrice(String symbol = "")`
- `double getCryptoPercent(String symbol = "")`
- `String getCryptoSymbol(String symbol = "")`
- `String getCryptoName(String symbol = "")`

#### `Response getStockArray(String symbol = "", int days = 7)`
Fetches historical stock data.

#### `Response getCryptoArray(String symbol = "", int days = 7)`
Fetches historical cryptocurrency data.

### News & Calendar

#### `Response getNews(String category)`
Fetches news articles.
- **category**: News category (e.g., "technology", "business")
- **Returns**: Response object

**Helpers:**
- `int getNewsArticleCount(String category = "general")`
- `String getNewsArticleTitle(int index, String category = "general")`
- `String getNewsArticleSource(int index, String category = "general")`

#### `Response getCalendar(String range)`
Fetches calendar events.
- **range**: Time range (e.g., "today", "week", "month")
- **Returns**: Response object

**Helpers:**
- `int getCalendarEventCount(String range = "1d")`
- `String getCalendarEventTitle(int index, String range = "1d")`
- `String getCalendarEventTime(int index, String range = "1d")`
- `String getCalendarEventLocation(int index, String range = "1d")`

### Travel

#### `Response getTravel(String origin = "", String destination = "", String mode = "driving")`
Fetches travel/route information.
- **origin**: Starting location
- **destination**: Destination location
- **mode**: Travel mode ("driving", "walking", "transit")
- **Returns**: Response object

**Helpers:**
- `String getTravelDuration(String origin = "", String destination = "", String mode = "driving")`
- `String getTravelDistance(String origin = "", String destination = "", String mode = "driving")`
- `String getTravelOrigin(String origin = "", String destination = "", String mode = "driving")`
- `String getTravelDestination(String origin = "", String destination = "", String mode = "driving")`
- `String getTravelMode(String origin = "", String destination = "", String mode = "driving")`

### Canvas

#### `Response getCanvas(String type, String domain, String canvasApiKey)`
Fetches custom canvas data.
- **type**: "todo" or "grades"
- **domain**: Canvas domain (optional if cached)
- **canvasApiKey**: Canvas API key (optional if cached)
- **Returns**: Response object

**Helpers:**
- `Response getCanvasAssignment(String id, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `String getCanvasAssignmentName(String id, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `String getCanvasAssignmentDueDate(String id, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `String getCanvasAssignmentType(String id, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `Response getCanvasGradeSet(String course, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `String getCanvasLetterGrade(String course, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `int getCanvasNumericGrade(String course, String domain = "", String canvasApiKey = "")` or `(int index, String domain = "", String canvasApiKey = "")`
- `double getGPAEstimate(bool weighted, String domain = "", String canvasApiKey = "", ...)`

### Spotify Integration

#### `Response spotifyRequest(String endpoint, String method = "GET", String body = "")`
Makes a raw Spotify API request.
- **endpoint**: Spotify API endpoint
- **method**: HTTP method ("GET", "POST", "PUT")
- **body**: Optional JSON body for POST/PUT requests
- **Returns**: Response object

#### `Response getSpotifyAlbums(int limit = 20, int offset = 0)`
Fetches user's saved albums.
- **limit**: Number of albums to fetch
- **offset**: Pagination offset
- **Returns**: Response object

#### `Response getSpotifyPlaylists(int limit = 20, int offset = 0)`
Fetches user's playlists.
- **limit**: Number of playlists to fetch
- **offset**: Pagination offset
- **Returns**: Response object

#### `Response getSpotifyLikedSongs(int limit = 20, int offset = 0)`
Fetches user's liked songs.
- **limit**: Number of songs to fetch
- **offset**: Pagination offset
- **Returns**: Response object

#### `Response getSpotifyFollowedArtists(int limit = 20, String after = "")`
Fetches user's followed artists.
- **limit**: Number of artists to fetch
- **after**: Cursor for pagination
- **Returns**: Response object

#### `Response getSpotifyDevices()`
Fetches available Spotify playback devices.
- **Returns**: Response object

#### `Response spotifyPlayback(String action, String uri = "", int volume = -1, int position = -1, String state = "", String targetDeviceId = "")`
Controls Spotify playback.
- **action**: Playback action ("play", "pause", "next", "previous", "volume", "seek", "transfer")
- **uri**: Optional Spotify URI to play
- **volume**: Volume level (0-100), -1 to ignore
- **position**: Seek position in ms, -1 to ignore
- **state**: Playback state ("playing", "paused")
- **targetDeviceId**: Device ID to transfer playback to
- **Returns**: Response object

## Usage Examples

### Weather Data
```cpp
// Using helper (automatically fetches if needed)
double temp = ink.getWeatherTemperature();
String cond = ink.getWeatherCondition();

// Or manually checking response
Response r = ink.getWeather("New York");
if (r.status == "OK") {
    double temp = r.data["temperature"];
}
```

### Stock Prices
```cpp
// Using helper
double price = ink.getStockPrice();

// Or manual fetch
Response r = ink.getStock("AAPL");
double price = r.data["price"];
```

### Spotify Playback
```cpp
// Play a track
ink.spotifyPlayback("play", "spotify:track:TRACK_ID");

// Pause playback
ink.spotifyPlayback("pause");

// Set volume to 50%
ink.spotifyPlayback("volume", "", 50);

// Next track
ink.spotifyPlayback("next");
```

### Calendar Events
```cpp
// Using helper
int count = ink.getCalendarEventCount();
if (count > 0) {
    String title = ink.getCalendarEventTitle(0);
}
```

### Canvas GPA Estimator
```cpp
// Calculate unweighted GPA (using default 4.0 scale)
double gpa = ink.getGPAEstimate(false);

// Calculate weighted GPA
double weightedGpa = ink.getGPAEstimate(true);
```

## Configuration Storage

The library uses NVS (Non-Volatile Storage) to persist:
- Device ID (MAC address)
- API Key
- User ID
- Friendly Name
- API URL (if custom)

Configuration survives device reboots and can be reset using:
```cpp
InkBridge ink(true); // Factory reset on initialization
```

## Error Handling

All API methods return empty strings on failure. Check responses before parsing:
```cpp
String response = ink.getWeather();
if (response != "") {
    JsonDocument doc;
    if (deserializeJson(doc, response) == DeserializationError::Ok) {
        // Process data
    }
}
```

## Requirements

- **Hardware**: ESP32 (or compatible board with WiFi)
- **WiFi**: Active internet connection required
- **Libraries**: 
  - ArduinoJson (v6+)
  - ESP32 WiFi libraries
  - HTTPClient

## Notes

- WiFi must be connected before calling `begin()`
- Device auto-registers on first run using MAC address
- HTTPS connections use insecure mode (certificate validation disabled)
- All requests include retry logic (3 attempts)
- Timeout set to 15 seconds per request

## License

See LICENSE file for details.

## Support

For issues, feature requests, or questions, please open an issue on GitHub.
