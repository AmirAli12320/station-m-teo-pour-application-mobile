//#include <common.h>
//#include <FirebaseFS.h>
//#include <Firebase_ESP_Client.h>
//#include <Utils.h>

#include <WiFi.h>
#include <DHT.h>
#include <Adafruit_BME280.h>

//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 20, 4);



 #if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


#define FIREBASE_HOST "https://stationmeteo-8a5b1-default-rtdb.europe-west1.firebasedatabase.app/"
#define FIREBASE_AUTH "NeX8juDTcx2CFn2iudG46VMX6RwScVOFWCJllZv9"
#define WIFI_SSID "*******" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "******" // Thay đổi mot de passe wifi của bạn
#define DHTPIN 17 // Chân dữ liệu của DHT 11 , với NodeMCU chân D5 GPIO là 14
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
FirebaseData StationMeteo ;

Adafruit_BME280 bme;

float  pressure, altitude;
#define SEALEVELPRESSURE_HPA (1013.25)   // Pression atmosphérique au niveau de la mer.

void setup() {
//  lcd.init();

  Serial.begin(115200);
  delay(1000);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay (500);
  }

   bme.begin(0x76);   // 0x76 = I2C adresse

  dht.begin();
  Serial.println ("");
  Serial.println ("connected:");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
int n=0;

void loop() {
  delay(2000);
  

  float h = dht.readHumidity();
  float t = dht.readTemperature(); // lire la température

  if( isnan(h) || isnan(t)) {
    Serial.println("Échec de la lecture du capteur DHT !");
    return;

  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("*C ");
  Serial.print("Humidity : ");
  Serial.print(h);
  Serial.println("% ");

  getData(); 
  Serial.print(" Pression: ");
  Serial.print(pressure, 2);
  Serial.print(" hPa");
 
  Serial.print(" Approx. Altitude = ");
  Serial.print(altitude, 0);
  Serial.println(" m");
  delay(200);

//  lcd.backlight();
//  lcd.setCursor(0,0);
//  lcd.print("Temperature");
//  lcd.setCursor(12,0);
//  lcd.print(t);
//  lcd.setCursor(0,1);
//  lcd.print("humidity:");
//  lcd.setCursor(10,1);
//  lcd.print(h);
//  
//  delay(1000);
  

  Firebase.setFloat( StationMeteo,"temperature", t);

  Firebase.setFloat ( StationMeteo,"humidity", h);

   Firebase.setFloat ( StationMeteo,"pressure", pressure);
   Firebase.setFloat ( StationMeteo,"altitude", altitude);

  delay(200);

}
void getData() {
  
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
}
