#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Wifi network station credentials
#define WIFI_SSID "Lab. Telematica"
#define WIFI_PASSWORD "l4bt3l3m4tic@"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "" // 

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
#define CHAT_ID "" //  

// DHT Sensor
#define DHTPIN 0           
#define DHTTYPE DHT22      
DHT dht(DHTPIN, DHTTYPE);

// Sprinkler control
#define SPRINKLER_PIN 5    // Pin connected to the relay or control module

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Initialize DHT sensor
  dht.begin();

  // Initialize Sprinkler Pin
  pinMode(SPRINKLER_PIN, OUTPUT);
  digitalWrite(SPRINKLER_PIN, LOW); // Assume LOW means off

  // Attempt to connect to Wifi network
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // Get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  // Send a message to Telegram
  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    String text = bot.messages[i].text;
    Serial.println(text);

    if (text == "/start") {
      String welcome = "Welcome! Use the following commands to control the sprinklers and check the sensor data:\n\n";
      welcome += "/sprinkler_on to turn the sprinklers ON\n";
      welcome += "/sprinkler_off to turn the sprinklers OFF\n";
      welcome += "/status to get the current status of temperature, humidity, and sprinklers\n";
      bot.sendMessage(chat_id, welcome, "");
    } else if (text == "/sprinkler_on") {
      bot.sendMessage(chat_id, "Sprinkler state set to ON", "");
      digitalWrite(SPRINKLER_PIN, HIGH);
    } else if (text == "/sprinkler_off") {
      bot.sendMessage(chat_id, "Sprinkler state set to OFF", "");
      digitalWrite(SPRINKLER_PIN, LOW);
    } else if (text == "/status") {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      if (isnan(temperature) || isnan(humidity)) {
        bot.sendMessage(chat_id, "Failed to read from DHT sensor!", "");
        continue;
      }

      String sprinklerState = (digitalRead(SPRINKLER_PIN) == HIGH) ? "ON" : "OFF";
      String message = "Temperature: " + String(temperature) + " °C\n";
      message += "Humidity: " + String(humidity) + " %\n";
      message += "Sprinklers: " + sprinklerState;

      bot.sendMessage(chat_id, message, "");
    } else {
      bot.sendMessage(chat_id, "Unknown command", "");
    }
  }
}





void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Get the state of the sprinkler
  String sprinklerState = (digitalRead(SPRINKLER_PIN) == HIGH) ? "ON" : "OFF";

  // Prepare message
  String message = "Temperature: " + String(temperature) + " °C\n";
  message += "Humidity: " + String(humidity) + " %\n";
  message += "Sprinklers: " + sprinklerState;

  // Send the message to Telegram
  //bot.sendMessage(CHAT_ID, message, "");

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // Wait for a while before sending the next update
  delay(60000); // Update every 60 seconds
}
