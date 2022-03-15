/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

//This example shows how to backup and restore database data

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "API_KEY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  //Or use legacy authenticate method
  //config.database_url = DATABASE_URL;
  //config.signer.tokens.legacy_token = "<database secret>";

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

#if defined(ESP8266)
  //required for large file data, increase Rx size as needed.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
}

void loop()
{
  //Flash string (PROGMEM and  (FPSTR), String, C/C++ string, const char, char array, string literal are supported
  //in all Firebase and FirebaseJson functions, unless F() macro is not supported.

  if (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;

    //Download and save data to SD card.
    //<target node> is the full path of database to backup and restore.
    //<file name> is file name in 8.3 DOS format (max. 8 bytes file name and 3 bytes file extension)

    Serial.printf("Backup... %s\n", Firebase.backup(fbdo, StorageType::SD, "/<target node>" /* node path to backup*/, "/<file name>" /* file name included path to save */) ? "ok" : fbdo.fileTransferError().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
    {
      Serial.printf("backup file, %s\n", fbdo.getBackupFilename().c_str());
      Serial.printf("file size, %d\n", fbdo.getBackupFileSize());
    }

    //Restore data to defined database path using backup file on Flash memory.
    //<target node> is the full path of database to restore
    //<file name> is file name included path of backed up file.
    //The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.

    Serial.printf("Restore... %s\n", Firebase.restore(fbdo, StorageType::SD, "/<target node>" /* node path to restore */, "/<file name>" /* backup file to restore */) ? "ok" : fbdo.fileTransferError().c_str());

    if (fbdo.httpCode() == FIREBASE_ERROR_HTTP_CODE_OK)
      Serial.printf("backup file, %s\n", fbdo.getBackupFilename().c_str());
  }
}