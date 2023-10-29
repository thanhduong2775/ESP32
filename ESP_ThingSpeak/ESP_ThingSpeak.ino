#include <WiFi.h>
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros


#define WIFI_SSID "LAU 2 2.4G"		// replace MySSID with your WiFi network name
#define WIFI_PASSWORD "phong882"	// replace MyPassword with your WiFi password

#define SECRET_CH_ID 1943428			// replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "UTV5X4A6OCBF55P5"   // replace XYZ with your channel write API Key

#define MQ2_A 35 // define MQ2 analog pin
#define MQ2_D 14 // define MQ2 digital pin
#define LED 25

WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int D_value, A_value;
unsigned long dataMillis = 0;

void setup() {
  Serial.begin(115200);  //Initialize serial

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

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  A_value = analogRead(MQ2_D);
  D_value = digitalRead(MQ2_D);

  Serial.printf("Analog value: %d\n", A_value);
  Serial.printf("Digital value: %d\n", D_value);

  ThingSpeak.setField(1, A_value);
  ThingSpeak.setField(2, !D_value);

  if (millis() - dataMillis > 20000)
  {
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }  
  }

  if(!D_value){
    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    delay(200);
  }
}
