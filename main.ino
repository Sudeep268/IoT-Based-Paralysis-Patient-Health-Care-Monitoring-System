#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h> 
#include <ArduinoJson.h> 
#include <LiquidCrystal_I2C.h> 
#include <HTTPClient.h>   //Including HTTPClient.h library to use all api 
#include <WiFiClient.h> 
#include "DHT.h" 
const char* ssid = "YOUR_SSID"; 
const char* password ="YOUR_PASSWORD"; 
// Initialize Telegram BOT 
#define BOTtoken "YOUR_BOT_TOKEN"  // your Bot 
Token (Get from Botfather) 
// Use @myidbot to find out the chat ID of an individual or a group 
// Also note that you need to click "start" on a bot before it can 
// message you 
#define CHAT_ID "YOUR_CHAT_ID" 
WiFiClientSecure client; 
UniversalTelegramBot bot(BOTtoken, client); 
#define DHTPIN 16      
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE); 
const int xpin = A0; // x-axis of the accelerometer 
const int ypin = A3; // y-axis 
const int zpin = A4; // z-axis 
const int buzzerPin = 25; // GPIO pin 34 connected to the buzzer 
String message; 
const int pulsePin = 35; 
float pulseValue; 
float bpm; 
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD with the corresponding pins 
void setup() { 
Serial.begin(115200); 
Serial.println(F("DHTxx test!")); 
dht.begin(); 
pinMode(pulsePin, INPUT); 
// Attempt to connect to Wifi network: 
Serial.print("Connecting Wifi: "); 
  Serial.println(ssid); 
 
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password); 
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for 
api.telegram.org 
   
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
 
  Serial.println(""); 
  Serial.println("WiFi connected"); 
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP()); 
  lcd.clear(); 
  // initialize LCD 
  lcd.init(); 
  // turn on LCD backlight 
  lcd.backlight(); 
  pinMode(buzzerPin, OUTPUT); 
} 
 
void loop()  
{ 
  int x = analogRead(xpin); // Read from xpin 
  delay(1); 
  int y = analogRead(ypin); // Read from ypin 
  delay(1); 
  int z = analogRead(zpin); // Read from zpin 
 
  float zero_G = 512.0; // ADC is 0~1023, the zero g output equals to Vs/2 
  float scale = 102.3; // ADXL335 Sensitivity is 330mv/g 
 
  // Calculate the acceleration values 
  float accelX = (((float)x - 331.5) / 65 * 9.8); 
  float accelY = (((float)y - 329.5) / 68.5 * 9.8); 
  float accelZ = (((float)z - 340) / 68 * 9.8); 
 
  // Display the acceleration values on the LCD 
  lcd.clear(); 
  if (accelX < 200) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("I need water"); 
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer 
    bot.sendMessage(CHAT_ID, "I need water", ""); 
  }  
  else if (accelX > 260) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("I need food"); 
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer 
    bot.sendMessage(CHAT_ID, "I need food", ""); 
  }  
  else if (accelY > 230) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Help"); 
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer 
    bot.sendMessage(CHAT_ID, "Help", ""); 
  }  
  else if (accelY < 180) { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Washroom"); 
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer 
    bot.sendMessage(CHAT_ID, "Washroom", ""); 
  }  
  else { 
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("No Need"); 
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer 
  } 
 
  // Wait for 1 second 
   // Wait a few seconds between measurements. 
 
  // Reading temperature or humidity takes about 250 milliseconds! 
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor) 
  float h = dht.readHumidity(); 
        if (h > 87) { 
    bot.sendMessage(CHAT_ID, "The humidity is high", ""); 
    } 
      // Read temperature as Celsius (the default) 
  float t = dht.readTemperature(); 
        if (t > 34) { 
    bot.sendMessage(CHAT_ID, "The temperature is above 34 degree", ""); 
    } 
    { 
  Serial.println(F("Humidity: ")); 
  Serial.println(h); 
  Serial.println(F("%  Temperature: ")); 
  Serial.println(t); 
  Serial.println(F("°C ")); 
 
    pulseValue = analogRead(pulsePin); 
    if (pulseValue>600){ 
      bpm=60000/pulseValue; 
              if (bpm > 70) { 
    bot.sendMessage(CHAT_ID, " The bpm is high", ""); 
    } 
            else if (bpm < 15) { 
    bot.sendMessage(CHAT_ID, " The bpm is low", ""); 
    } 
      Serial.print("Heart rate: "); 
      Serial.print(bpm); 
      Serial.println(" BPM"); 
      delay(700); 
      String message="Heart rate: " + String(bpm) + " BPM"; 
      Serial.println(message); 
    }  
    }    
}
