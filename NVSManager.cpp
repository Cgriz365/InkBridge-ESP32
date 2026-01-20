#include "NVSManager.h"

const char* NVSManager::NVS_NAMESPACE = "dev_conf";
bool NVSManager::nvs_init = false; 


void NVSManager::init() {
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  nvs_init = true;
}

bool NVSManager::isInit(){
  return nvs_init;
}

void NVSManager::saveString(const char* key, String value) {
  nvs_handle_t my_handle;
  if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK) {
    nvs_set_str(my_handle, key, value.c_str());
    nvs_commit(my_handle);
    nvs_close(my_handle);
  }
}

String NVSManager::loadString(const char* key) {
  nvs_handle_t my_handle;
  String result = "";
  if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle) == ESP_OK) {
    size_t required_size;
    if (nvs_get_str(my_handle, key, NULL, &required_size) == ESP_OK) {
      char* buffer = (char*)malloc(required_size);
      nvs_get_str(my_handle, key, buffer, &required_size);
      result = String(buffer);
      free(buffer);
    }
    nvs_close(my_handle);
  }
  return result;
}

void NVSManager::saveApi(String api) {
  saveString("apikey", api);
}

void NVSManager::saveDeviceId(String deviceId) {
  saveString("deviceId", deviceId);
}

void NVSManager::saveApiUrl(String url) {
  saveString("apiurl", url);
}

void NVSManager::saveFriendlyUser(String user) {
  saveString("friendlyuser", user);
}

void NVSManager::saveUID(String uid) {
  saveString("uid", uid);
}

String NVSManager::loadApi() {
  return loadString("apikey");
}

String NVSManager::loadDeviceId() {
  return loadString("deviceId");
}

String NVSManager::loadUID() {
  return loadString("uid");
}

String NVSManager::loadApiUrl() {
  return loadString("apiurl");
}

String NVSManager::loadFriendlyUser() {
  return loadString("friendlyuser");
}

void NVSManager::factoryReset() {
  nvs_flash_erase();
  nvs_flash_init();
}