#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char ssid[] = "matekaja kau";
const char pass[] = "anjenggg";

const int pinLvl1 = 21;
const int pinLvl2 = 22;
const int pinLvl3 = 23;

String response = "";

DynamicJsonDocument doc(2048);

unsigned long currentTime= millis();
unsigned long previousTime=0;
const long timeoutTime=1*60*1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pinLvl1, OUTPUT);
  pinMode(pinLvl2, OUTPUT);
  pinMode(pinLvl3, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");
  
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("WIFI connected with IP: ");
  Serial.println(WiFi.localIP());
  
}

void loop(void) {
  // put your main code here, to run repeatedly:
  
  HTTPClient http;
  currentTime= millis();

  if(currentTime - previousTime > timeoutTime){
    Serial.println("send request");
    previousTime = currentTime;

    String request ="https://api.tomorrow.io/v4/timelines?location=-7.290071, 112.674659&fields=temperature&timesteps=current&timezone=Asia/Jakarta&units=metric&apikey=TTsIvKDerpbVMdSiBRiTiGyL4f2TKbV6";

    http.begin(request);

    http.GET();

    response = http.getString();
    
    DeserializationError error = deserializeJson(doc, response);
    if(error){
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    JsonObject data_timelines_0 = doc["data"]["timelines"][0];
    const char* data_timelines_0_intervals_0_startTime = data_timelines_0["intervals"][0]["startTime"];

    float data_timelines_0_intervals_0_values_temperature = data_timelines_0["intervals"][0]["values"]["temperature"];
    
    Serial.print("Waktu: ");
    Serial.println(data_timelines_0_intervals_0_startTime);
    Serial.print("Temperature: ");
    Serial.println(data_timelines_0_intervals_0_values_temperature);
    if(data_timelines_0_intervals_0_values_temperature <= 20){
      digitalWrite(pinLvl1, LOW);
      digitalWrite(pinLvl2, HIGH);
      digitalWrite(pinLvl3, HIGH);
      Serial.println("Level: 1");
    }
    if(data_timelines_0_intervals_0_values_temperature > 20 && data_timelines_0_intervals_0_values_temperature <= 30 ){
      digitalWrite(pinLvl1, HIGH);
      digitalWrite(pinLvl2, LOW);
      digitalWrite(pinLvl3, HIGH);
      Serial.println("Level: 2");
    }
    if(data_timelines_0_intervals_0_values_temperature > 30){
      digitalWrite(pinLvl1, HIGH);
      digitalWrite(pinLvl2, HIGH);
      digitalWrite(pinLvl3, LOW);
      Serial.println("Level: 3");
    }
    http.end();
  }
}
