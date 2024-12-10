// Data Logger main
#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

void tostring(char [], int);
char str[10];
char str2[]="° Centigrade";
const int analogOutPin = A0;  // Analog output pin that the LED is attached to

int sensorValue = 0;  
int out = 0; 
// Wifi network station credentials
#define WIFI_SSID "Wifi_Name"
#define WIFI_PASSWORD "PassWord"
// Telegram BOT Token 
#define BOT_TOKEN "Bot Token...."


#define CHAT_ID "ChatID..number"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);
  Serial.println();

 // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "DataLoggerBot started ", "");
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogOutPin);
  out = ((sensorValue*500)/1024);
  
   tostring(str, out);

   strcat(str,str2);
   
   bot.sendMessage(CHAT_ID, str, "");
  
   delay(600);
}

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
