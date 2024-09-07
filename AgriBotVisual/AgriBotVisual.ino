#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Wifi network station credentials
#define WIFI_SSID ""  //"Lab. Telematica"
#define WIFI_PASSWORD ""  //"l4bt3l3m4tic@"
// Telegram BOT Token (Get from Botfather)
#define BOTtoken ""//

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
#define CHAT_ID ""//

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 23;
const int SPRINKLER_PIN = 2;
bool ledState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control the sprinklers and check the sensor data:\n\n";
      welcome += "/sprinkler_on to turn the sprinklers ON\n";
      welcome += "/sprinkler_off to turn the sprinklers OFF\n";
      welcome += "/status to get the current status of temperature, humidity, and sprinklers\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/sprinkler_on") {
      bot.sendMessage(chat_id, "Sprinkler state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/sprinkler_off") {
      bot.sendMessage(chat_id, "Sprinkler state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/status") {
      String sprinklerState = "";
      if (digitalRead(ledPin)){
        sprinklerState = "ON";
      }
      else{
         sprinklerState = "OFF";
      }
      String message = "Temperature: 30°C\n";
      message += "Humidity: 70%\n";
      message += "Sprinklers: " + sprinklerState;
//SPRINKLER_PIN
      bot.sendMessage(chat_id, message, ""); 
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
