#include "Inkbridge.h"
#include "NVSManager.h"       // Ensure this is included
#include <WiFiClientSecure.h> // Required for HTTPS connections

// Constructor
InkBridge::InkBridge(bool resetDevice)
{
    _apiUrl = "https://us-central1-inkbase01.cloudfunctions.net/api";
    _friendlyName = "Unknown";
    _apiKey = "";
    _deviceId = "";
    _uid = "";
    _resetDevice = resetDevice;
}

InkBridge::InkBridge(const char *apiUrl)
{
    _apiUrl = String(apiUrl);
    _friendlyName = "Unknown";
    _apiKey = "";
    _deviceId = "";
    _uid = "";
}

// Initialization
bool InkBridge::begin()
{
    // 1. Initialize NVS
    if (!NVSManager::isInit())
    {
        NVSManager::init();
        Serial.println("[NVS] Initializing NVS for Configuration Storage...");
    }

    if (_resetDevice)
    {
        Serial.println("[Ink] Resetting device configuration as requested...");
        NVSManager::factoryReset();
    }

    // 2. Load existing config from NVS
    String storedDeviceId = NVSManager::loadDeviceId();
    String storedUID = NVSManager::loadUID();
    String storedApi = NVSManager::loadApi();
    String storedUrl = NVSManager::loadApiUrl();
    String storedFriendly = NVSManager::loadFriendlyUser();

    // 3. Handle Device ID (MAC Address)
    if (storedDeviceId == "" || storedDeviceId == "null")
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            String mac = WiFi.macAddress();
            mac.replace(":", "");
            _deviceId = mac;
            Serial.println("[Ink] New Device ID detected: " + _deviceId);
            NVSManager::saveDeviceId(_deviceId);
            Serial.println("[Ink] Device ID Saved");
        }
        else
        {
            if ((WiFi.localIP() != INADDR_NONE || WiFi.localIP()[0] != 0))
            {
                String mac = WiFi.macAddress();
                mac.replace(":", "");
                _deviceId = mac;
                Serial.println("[Ink] New Device ID detected: " + _deviceId);
                NVSManager::saveDeviceId(_deviceId);
                Serial.println("[Ink] Device ID Saved");
            }
            else
            {
                Serial.println("[Error] WiFi needed to generate Device ID");
                return false;
            }
        }
    }
    else
    {
        _deviceId = storedDeviceId;
        Serial.println("[Ink] Loaded Device ID from NVS(Non-Volotile Storage): " + _deviceId);
    }

    // 4. Handle API Key
    if (storedApi != "" && storedApi != "null")
    {
        _apiKey = storedApi;
        Serial.println("[Ink] API Key loaded from storage.");
    }

    // 5. Handle Friendly Name
    if (storedFriendly != "" && storedFriendly != "null")
    {
        _friendlyName = storedFriendly;
    }

    if (storedUID != "" && storedUID != "null")
    {
        _uid = storedUID;
    }

    // 6. Handle API URL override (optional)
    if (storedUrl != "" && storedUrl != "null")
    {
        _apiUrl = storedUrl;
    }

    // 7. Auto-Register if we have ID but no Key
    if (_deviceId != "" && _apiKey == "")
    {
        Serial.println("[Ink] Device not registered. Attempting auto-registration...");
        return registerDevice();
    }

    return isRegistered();
}

// Check if registered
bool InkBridge::isRegistered()
{
    return (_apiKey.length() > 0 && _deviceId.length() > 0 && _uid.length() > 0);
}

// Set Key Manually
void InkBridge::setApiKey(String key)
{
    if (key != "")
    {
        _apiKey = key;
        NVSManager::saveApi(_apiKey);
        Serial.println("[Ink] API Key Set Manually");
    }
}

// Get Key
String InkBridge::getApiKey()
{
    return _apiKey;
}

String InkBridge::sendRequest(String endpoint, String method, String payload)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        if (WiFi.localIP() == INADDR_NONE && WiFi.localIP()[0] == 0)
        {
            Serial.println("[Error] WiFi not connected");
            return "";
        }
    }

    WiFiClientSecure client;
    client.setInsecure(); // Skip cert validation
    client.setHandshakeTimeout(10);

    HTTPClient http;
    http.setReuse(false);
    http.setTimeout(15000);

    String url = _apiUrl + endpoint;

    // For GET requests, we append query params manually.
    // For POST, we rely on the JSON body.
    if (method == "GET")
    {
        url += (url.indexOf("?") == -1 ? "?" : "&");
        url += "device_id=" + _deviceId;
        if (_apiKey.length() > 0)
        {
            url += "&api_key=" + _apiKey;
        }
    }

    Serial.print("[HTTP] " + method + ": " + url);

    if (!http.begin(client, url))
    {
        Serial.println(" [Error] Connect Failed");
        return "";
    }

    // Standard Headers
    http.addHeader("x-device-id", _deviceId);
    if (_apiKey.length() > 0)
        http.addHeader("x-api-key", _apiKey);

    // JSON Header for POST requests
    if (method == "POST")
    {
        http.addHeader("Content-Type", "application/json");
    }

    int httpCode = -1;
    for (int i = 0; i < 3; i++)
    {
        if (method == "POST")
        {
            httpCode = http.POST(payload);
        }
        else
        {
            httpCode = http.GET();
        }

        if (httpCode > 0)
            break;
        Serial.print("."); // Retry indicator
        delay(1000);
    }

    String response = "";
    if (httpCode > 0)
    {
        response = http.getString();
        if (httpCode >= 400)
        {
            Serial.printf(" [Error %d] %s\n", httpCode, response.c_str());
        }
        else
        {
            Serial.println(" [Success]");
        }
    }
    else
    {
        Serial.printf(" [Fatal] %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return response;
}

String InkBridge::getRequest(String endpoint, bool includeApiKey)
{
    return sendRequest(endpoint, "GET", "");
}

// Register Device (/setup)
bool InkBridge::registerDevice()
{
    // Call /setup
    // Note: sendRequest handles x-device-id header automatically
    String payload = sendRequest("/setup", "GET", "");
    if (payload == "")
    {
        Serial.println("[Ink] Payload empty check API connection");
        return false;
    }
    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error || doc["status"] != "success")
    {
        Serial.println("[Ink] Registration Failed: " + doc["message"].as<String>());
        return false;
    }
    // Save credentials
    _apiKey = doc["api_key"].as<String>();
    _friendlyName = doc["friendly_user_id"].as<String>();
    _uid = doc["uid"].as<String>();

    NVSManager::saveApi(_apiKey);
    NVSManager::saveFriendlyUser(_friendlyName);
    NVSManager::saveUID(_uid);
    
    Serial.println("[Ink] Registration Successful! Linked to: " + _friendlyName);
    return true;
}

String InkBridge::getWeather(String location) {
    JsonDocument doc;
    Serial.println("UID: " + _uid);
    doc["uid"] = _uid; // Pass MAC address as UID
    doc["device_id"] = _deviceId;
    if(location != "") doc["location"] = location;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/weather", "POST", json);
}

String InkBridge::getStock(String symbol) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    if(symbol != "") doc["symbol"] = symbol;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/stock", "POST", json);
}

String InkBridge::getCrypto(String symbol) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    if(symbol != "") doc["symbol"] = symbol;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/crypto", "POST", json);
}

String InkBridge::getStockArray(String symbol, int days) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    if(symbol != "") doc["symbol"] = symbol;
    doc["days"] = days;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/stock/array", "POST", json);
}

String InkBridge::getCryptoArray(String symbol, int days) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    if(symbol != "") doc["symbol"] = symbol;
    doc["days"] = days;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/crypto/array", "POST", json);
}

String InkBridge::getNews(String category) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["category"] = category;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/news", "POST", json);
}

String InkBridge::getCalendar(String range) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["range"] = range;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/calendar", "POST", json);
}

String InkBridge::getTravel(String origin, String destination, String mode) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    if(origin != "") doc["origin"] = origin;
    if(destination != "") doc["destination"] = destination;
    doc["mode"] = mode;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/travel", "POST", json);
}

String InkBridge::getCanvas(String type) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["type"] = type;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/canvas", "POST", json);
}

String InkBridge::spotifyRequest(String endpoint, String method, String body) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["endpoint"] = endpoint;
    doc["method"] = method;
    if (body != "") doc["body"] = body;

    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/request", "POST", json);
}

String InkBridge::getSpotifyAlbums(int limit, int offset) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["limit"] = limit;
    doc["offset"] = offset;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/user_albums", "POST", json);
}

String InkBridge::getSpotifyPlaylists(int limit, int offset) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["limit"] = limit;
    doc["offset"] = offset;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/user_playlists", "POST", json);
}

String InkBridge::getSpotifyLikedSongs(int limit, int offset) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["limit"] = limit;
    doc["offset"] = offset;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/liked_songs", "POST", json);
}

String InkBridge::getSpotifyFollowedArtists(int limit, String after) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["limit"] = limit;
    if (after != "") doc["after"] = after;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/followed_artists", "POST", json);
}

String InkBridge::getSpotifyDevices() {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    
    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/devices", "POST", json);
}

String InkBridge::spotifyPlayback(String action, String uri, int volume, int position, String state, String targetDeviceId) {
    JsonDocument doc;
    doc["uid"] = _uid;
    doc["device_id"] = _deviceId;
    doc["action"] = action;
    if (uri != "") doc["uri"] = uri;
    if (volume != -1) doc["volume_percent"] = volume;
    if (position != -1) doc["position_ms"] = position;
    if (state != "") doc["state"] = state;
    if (targetDeviceId != "") doc["target_device_id"] = targetDeviceId;

    String json;
    serializeJson(doc, json);
    return sendRequest("/spotify/playback", "POST", json);
}

String InkBridge::getDeviceId()
{
    return _deviceId;
}

String InkBridge::getFriendlyId()
{
    return _friendlyName;
}
