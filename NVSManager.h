#ifndef NVSMANAGER_H
#define NVSMANAGER_H

#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>

class NVSManager {
public:
  static void init();  // Call nvs_flash_init here
  static bool isInit();

  static void saveApi(String api);
  static void saveDeviceId(String deviceId);
  static void saveApiUrl(String url);
  static void saveFriendlyUser(String user);
  static void saveUID(String uid);
  static void factoryReset();
  static String loadApi();
  static String loadApiUrl();
  static String loadDeviceId();
  static String loadUID();
  static String loadFriendlyUser();

private:
  static void saveString(const char* key, String value);
  static String loadString(const char* key);
  static const char* NVS_NAMESPACE;
  static bool nvs_init;
};

#endif