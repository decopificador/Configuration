
#include "Configuration.h"

void Configuration::setWifiApEnabled(bool enabled){
  enabled ? strcpy(wifi_ap_enabled,"true") : strcpy(wifi_ap_enabled,"false");
}

void Configuration::setWifiApSsid(char* ssid){
  wifi_ap_ssid = ssid;
}

void Configuration::setWifiApPassword(char* password){
  wifi_ap_password = password;
}

void Configuration::setWifiStationEnabled(bool enabled){
  enabled ? strcpy(wifi_station_enabled,"true") : strcpy(wifi_station_enabled,"false");
}

void Configuration::setWifiStationSsid(char* ssid){
  wifi_station_ssid = ssid;
}

void Configuration::setWifiStationPassword(char* password){
  wifi_station_password = password;
}

void Configuration::setMqttEnabled(bool enabled){
  enabled ? strcpy(mqtt_enabled,"true") : strcpy(mqtt_enabled,"false");
}

void Configuration::setMqttServer(char* server){
  mqtt_host = server;
}

void Configuration::setMqttPort(char* port){
  mqtt_port = port;
}

void Configuration::setMqttPassword(char* password){
  mqtt_password = password;
}

void Configuration::setMqttUser(char* user){
  mqtt_user = user;
}

void Configuration::setMqttDeviceName(char* deviceName){
  mqtt_device_name = deviceName;
}

char* Configuration::getWifiApSsid(){
  return wifi_ap_ssid;
}

char* Configuration::getWifiApPassword(){
  return wifi_ap_password;
}

char* Configuration::getWifiStationSsid(){
  return wifi_station_ssid;
}

char* Configuration::getWifiStationPassword(){
  return wifi_station_password;
}

char* Configuration::getMqttServer(){
  return mqtt_host;
}

char* Configuration::getMqttPort(){
  return mqtt_port;
}

char* Configuration::getMqttUser() {
  return mqtt_user;
}

char* Configuration::getMqttDeviceName() {
  return mqtt_device_name;
}

char* Configuration::getMqttPassword() {
  return mqtt_password;
}

String Configuration::getRawConfiguration() {
  if (SPIFFS.begin()) {
    if (_rawConfiguration == NULL) {
      if (SPIFFS.exists("/configuration.json")) {
        //     //file exists, reading and loading
        Serial.println("reading config file");
        File configFile = SPIFFS.open("/configuration.json", "r");
        if (configFile) {
          _rawConfiguration = configFile.readString();
          configFile.close();
          Serial.printf("content: ");
          Serial.println(_rawConfiguration);
          return _rawConfiguration;
        } else {
          Serial.println("error opening config file");
        }
      } else {
        Serial.println("file does not exist");
        return "{}";
      }
    } else {
      return _rawConfiguration;
    }
  } else {
    Serial.println("failed to mount FS");
    return "{}";
  }
  //end read
}

void Configuration::write(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json[PARAM_WIFI_AP_SSID]      = wifi_ap_ssid;
  json[PARAM_WIFI_AP_PASSWORD]  = wifi_ap_password;
  json[PARAM_WIFI_AP_ENABLED]   = wifi_ap_enabled;

  json[PARAM_WIFI_STATION_SSID]     = wifi_station_ssid;
  json[PARAM_WIFI_STATION_PASSWORD] = wifi_station_password;
  json[PARAM_WIFI_STATION_ENABLED]  = wifi_station_enabled;

  json[PARAM_MQTT_ENABLED]      = mqtt_enabled;
  json[PARAM_MQTT_HOST]         = mqtt_host;
  json[PARAM_MQTT_PORT]         = mqtt_port;
  json[PARAM_MQTT_USER]         = mqtt_user;
  json[PARAM_MQTT_PASSWORD]     = mqtt_password;
  json[PARAM_MQTT_DEVICE_NAME]  = mqtt_device_name;
  //
  File configFile = SPIFFS.open("/configuration.json", "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }
  //
  json.printTo(Serial);
  json.printTo(configFile);
  configFile.close();
};

bool Configuration::isWifiApConfigurationValid(){
  if ( wifi_ap_ssid == NULL) return false;
  if ( wifi_ap_ssid != NULL && strlen(wifi_ap_ssid) < 3 ) return false;
  if ( wifi_ap_password != NULL && strlen(wifi_ap_password) < 8 ) return false;
  return true;
}

bool Configuration::isWifiStationConfigurationValid(){
  if ( wifi_station_ssid == NULL) return false;
  if ( wifi_station_ssid != NULL && strlen(wifi_ap_ssid) < 3 ) return false;
  if ( wifi_station_password != NULL && strlen(wifi_ap_password) < 8 ) return false;
  return true;
}

bool Configuration::isWifiApEnabled(){
  return strcmp(wifi_ap_enabled,"true") ? false : true;
}

bool Configuration::isWifiStationEnabled(){
  return strcmp(wifi_station_enabled,"true") ? false : true;
}

bool Configuration::isMqttEnabled() {
  return strcmp(mqtt_enabled,"true") ? false : true;
}

bool Configuration::isMqttConfigurationValid(){
  if ( mqtt_host == NULL) return false;
  if ( mqtt_device_name == NULL) return false;
  if (atoi(mqtt_port) < 1000) return false;
  if (atoi(mqtt_port) > 30000) return false;
  return true;
}

void Configuration::writeConfiguration(const char* &configuration){
  if (SPIFFS.begin()) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(configuration);
    File configFile = SPIFFS.open("/configuration.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
      return;
    }
    //
    Serial.println("Writing configuration");
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    Serial.println("Done");
    _rawConfiguration = configuration;
  }  else {
    Serial.println("failed to open spiffs for writing");
  }
}

void Configuration::writeConfiguration(String &configuration){
  if (SPIFFS.begin()) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(configuration);
    File configFile = SPIFFS.open("/configuration.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
      return;
    }
    //
    Serial.println("Writing configuration");
    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    Serial.println("Done");
    _rawConfiguration = configuration;
  }  else {
    Serial.println("failed to open spiffs for writing");
  }
}

void Configuration::read(){
  getRawConfiguration();
  if (_rawConfiguration) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(_rawConfiguration);
      if (json.success()) {
        Serial.println("\nparsed json");
        // reading wifi ap configuration
        readParameter(PARAM_WIFI_AP_SSID, wifi_ap_ssid, json);
        readParameter(PARAM_WIFI_AP_PASSWORD, wifi_ap_password, json);
        readParameter(PARAM_WIFI_AP_ENABLED, wifi_ap_enabled, json);
        // reading wifi station configuration
        readParameter(PARAM_WIFI_STATION_SSID, wifi_station_ssid, json);
        readParameter(PARAM_WIFI_STATION_PASSWORD, wifi_station_password, json);
        readParameter(PARAM_WIFI_STATION_ENABLED, wifi_station_enabled, json);
        // reading mqtt configuration
        readParameter(PARAM_MQTT_ENABLED, mqtt_enabled, json);
        readParameter(PARAM_MQTT_HOST, mqtt_host, json);
        readParameter(PARAM_MQTT_PORT, mqtt_port, json);
        readParameter(PARAM_MQTT_USER, mqtt_user, json);
        readParameter(PARAM_MQTT_PASSWORD, mqtt_password, json);
        readParameter(PARAM_MQTT_DEVICE_NAME, mqtt_device_name, json);
      } else {
        Serial.println("failed to load json config");
      }
  } else {
    Serial.println("failed to read configuration");
    return;
  }
  //end read
}

void Configuration::readParameter(String parameterName, char* variable, JsonObject& json){
  if (json.containsKey(parameterName)) {
    const char* value = json[parameterName];
    if (value != NULL) {
      strcpy(variable, value);
    }
  }
}
