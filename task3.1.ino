#include <WiFiNINA.h>
#define LIGHT_SENSOR 2
char ssid[] = "A16";
char pass[] = "22222222";

WiFiClient client;
//https://maker.ifttt.com/trigger/sunlight_enter/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ

char   HOST_NAME[] = "maker.ifttt.com";

String lstate = "Off";
int lvalue = 0;
String llight = "Off";

void setup() {

  Serial.begin(115200);
  connectToWiFi(ssid,pass);
  while (!Serial);


  pinMode(LIGHT_SENSOR,INPUT);
}

void loop() {

  int curr_light = readLightSensor();

  if (curr_light > 0) {
    lstate = "On";
  } else {
    lstate = "Off";
  }

  if (lstate != llight) {
    sendWebhook(lstate);
    llight = lstate;
  }
  delay(1000); 

}

void sendWebhook(String state) {
  if (!client.connect(HOST_NAME, 80)) {
    Serial.println("Connection failed!");
    return;
  }


    Serial.println("Connected to server");
    client.println("GET trigger/sunlight_enter/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ HTTP/1.1");
    client.println("Host: maker.ifttt.com"  );
    client.println("Connection: close");
    client.println(); 


  client.stop();
  Serial.println("Sent notification to IFTTT");
}

int readLightSensor() {
  lvalue = digitalRead(LIGHT_SENSOR);
  return lvalue;
}

bool connectToWiFi(char ssid[], char pass[]) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
      if (i > 6) {
        Serial.print("Wifi Connection Failed!");
        return false;
        break;
      }
      i++;
    }
    Serial.println("\nConnected.");
    return true;
  }
  return true; 
}