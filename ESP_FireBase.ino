
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
char ssid[] = "Light";
char pass[] = "impossible000";

/* 2. Define the RTDB URL */
#define DATABASE_URL "https://mq2sensor-91d4c-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

#define MQ2_A 35 // define MQ2 analog pin
#define MQ2_D 14 // define MQ2 digital pin
#define LED 25

int A_value, D_value;

unsigned long dataMillis = 0;

void setup()
{

    Serial.begin(115200);

    pinMode(MQ2_A, INPUT);
    pinMode(MQ2_D, INPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);

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

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL(required) */
    config.database_url = DATABASE_URL;

    config.signer.test_mode = true;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}

void loop()
{
  A_value = analogRead(MQ2_A);
  D_value = digitalRead(MQ2_D);
  
  if (millis() - dataMillis > 5000)
  {
    Serial.printf("Analog Value: %d\n", A_value);
    Serial.printf("Digital Value: %d\n", D_value);
    Serial.printf("Update ... %s\n", Firebase.setInt(fbdo, "/data/value", A_value) ? "Ok" : fbdo.errorReason().c_str());
    dataMillis = millis();
  }

  if(!D_value){
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }
}





