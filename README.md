# InkBridge Library Documentation
## Built for [InkBridge](https://inkbase01.web.app), [Github Repo](https://github.com/Cgriz365/InkBridge) ##

A comprehensive Arduino library for ESP32 devices that provides seamless integration with the InkBase cloud API, enabling access to weather, stocks, crypto, news, calendar, travel, Spotify, and custom canvas data.

## Features

- **Auto-registration**: Automatic device registration using MAC address
- **Persistent Storage**: NVS-based configuration storage for API keys and device credentials
- **HTTPS Support**: Secure communication with cloud APIs
- **Multiple Data Sources**: Weather, stocks, crypto, news, calendar, travel, and Spotify integration
- **Retry Logic**: Built-in HTTP request retry mechanism
- **Factory Reset**: Option to reset device configuration

## Installation

1. Download the library and place it in your Arduino `libraries` folder
2. Include the required dependencies:
   - `WiFi.h` or `WiFiClientSecure.h`
   - `HTTPClient.h`
   - `ArduinoJson.h`
   - `NVSManager.h` (included with library)

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

#### `String getWeather(String location = "")`
Fetches weather data.
- **location**: Optional location override
- **Returns**: JSON response with weather data

### Stocks & Crypto

#### `String getStock(String symbol = "")`
Fetches current stock data.
- **symbol**: Optional stock symbol (e.g., "AAPL")
- **Returns**: JSON response with stock data

#### `String getCrypto(String symbol = "")`
Fetches current cryptocurrency data.
- **symbol**: Optional crypto symbol (e.g., "BTC")
- **Returns**: JSON response with crypto data

#### `String getStockArray(String symbol = "", int days = 7)`
Fetches historical stock data.
- **symbol**: Optional stock symbol
- **days**: Number of days of historical data
- **Returns**: JSON response with stock price array

#### `String getCryptoArray(String symbol = "", int days = 7)`
Fetches historical cryptocurrency data.
- **symbol**: Optional crypto symbol
- **days**: Number of days of historical data
- **Returns**: JSON response with crypto price array

### News & Calendar

#### `String getNews(String category)`
Fetches news articles.
- **category**: News category (e.g., "technology", "business")
- **Returns**: JSON response with news articles

#### `String getCalendar(String range)`
Fetches calendar events.
- **range**: Time range (e.g., "today", "week", "month")
- **Returns**: JSON response with calendar events

### Travel

#### `String getTravel(String origin = "", String destination = "", String mode = "driving")`
Fetches travel/route information.
- **origin**: Starting location
- **destination**: Destination location
- **mode**: Travel mode ("driving", "walking", "transit")
- **Returns**: JSON response with travel data

### Canvas

#### `String getCanvas(String type)`
Fetches custom canvas data.
- **type**: Canvas type identifier
- **Returns**: JSON response with canvas data

### Spotify Integration

#### `String spotifyRequest(String endpoint, String method = "GET", String body = "")`
Makes a raw Spotify API request.
- **endpoint**: Spotify API endpoint
- **method**: HTTP method ("GET", "POST", "PUT")
- **body**: Optional JSON body for POST/PUT requests
- **Returns**: JSON response from Spotify API

#### `String getSpotifyAlbums(int limit = 20, int offset = 0)`
Fetches user's saved albums.
- **limit**: Number of albums to fetch
- **offset**: Pagination offset
- **Returns**: JSON response with albums

#### `String getSpotifyPlaylists(int limit = 20, int offset = 0)`
Fetches user's playlists.
- **limit**: Number of playlists to fetch
- **offset**: Pagination offset
- **Returns**: JSON response with playlists

#### `String getSpotifyLikedSongs(int limit = 20, int offset = 0)`
Fetches user's liked songs.
- **limit**: Number of songs to fetch
- **offset**: Pagination offset
- **Returns**: JSON response with liked songs

#### `String getSpotifyFollowedArtists(int limit = 20, String after = "")`
Fetches user's followed artists.
- **limit**: Number of artists to fetch
- **after**: Cursor for pagination
- **Returns**: JSON response with followed artists

#### `String getSpotifyDevices()`
Fetches available Spotify playback devices.
- **Returns**: JSON response with device list

#### `String spotifyPlayback(String action, String uri = "", int volume = -1, int position = -1, String state = "", String targetDeviceId = "")`
Controls Spotify playback.
- **action**: Playback action ("play", "pause", "next", "previous", "volume", "seek", "transfer")
- **uri**: Optional Spotify URI to play
- **volume**: Volume level (0-100), -1 to ignore
- **position**: Seek position in ms, -1 to ignore
- **state**: Playback state ("playing", "paused")
- **targetDeviceId**: Device ID to transfer playback to
- **Returns**: JSON response with playback status

## Usage Examples

### Weather Data
```cpp
String weather = ink.getWeather("New York");
JsonDocument doc;
deserializeJson(doc, weather);
float temp = doc["temperature"];
```

### Stock Prices
```cpp
String stock = ink.getStock("AAPL");
JsonDocument doc;
deserializeJson(doc, stock);
float price = doc["price"];
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
String events = ink.getCalendar("today");
JsonDocument doc;
deserializeJson(doc, events);
String firstEvent = doc["events"][0]["title"];
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
