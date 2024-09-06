#define ONE_WIRE_BUS 4
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3Yc846VBe"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring System"


#define BLYNK_AUTH_TOKEN "7NsccDsimzgyf0I3MtyOmVTVjxf3OlIm"
#include <OneWire.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DallasTemperature.h>


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
char auth[] = "7NsccDsimzgyf0I3MtyOmVTVjxf3OlIm";
char ssid[] = "realme 9 Pro+";
char pass[] = "nividreal9";

#define pump_pin D5
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Use BlynkTimer instead of SimpleTimer
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V5, h); // V5 is for Humidity
  Blynk.virtualWrite(V4, t); // V6 is for Temperature
}

int sensor = 0;
int output = 0;
void sendTemps()
{
  sensor = analogRead(A0);
  output = (145 - map(sensor, 0, 1023, 0, 100)); // Adjust 145 based on the sensor calibration
  sensors.requestTemperatures();
  if(output < 50){
  Blynk.logEvent("soilmoisture","Soil Moisture is low!!!!! Turn on the Water Pump...");
  
  }

  float temp = sensors.getTempCByIndex(0);
  if(temp > 25 ){
  Blynk.logEvent("temperature","Temperature is High!!!");
}

  
  Serial.println(temp);
  Serial.print("moisture = ");
  Serial.print(output);
  Serial.println("%");

  Blynk.virtualWrite(V4, temp);    // Send temperature to Blynk
  Blynk.virtualWrite(V0, output);  // Send moisture to Blynk
}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  sensors.begin();
  
  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // Setup timers
  timer.setInterval(1000L, sendSensor); // Every 1 second
  timer.setInterval(2000L, sendTemps);  // Every 2 seconds
}
void loop()
{
  Blynk.run();
  timer.run(); // Run the timers
}
