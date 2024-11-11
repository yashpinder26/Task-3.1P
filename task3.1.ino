#include <WiFiNINA.h>        // Include WiFi library for Arduino
#define LIGHT_SENSOR 2       // Define the light sensor pin

// WiFi credentials
char ssid[] = "A16";
char pass[] = "22222222";

// Initialize WiFi client object
WiFiClient client;

// IFTTT webhook details
char HOST_NAME[] = "maker.ifttt.com";

// Variables to store light state
String lstate = "Off";       // Current state of the light
int lvalue = 0;              // Raw light sensor value
String llight = "Off";       // Previous light state for comparison

void setup() {
  Serial.begin(115200);           // Initialize serial communication at 115200 bps
  connectToWiFi(ssid, pass);      // Connect to WiFi
  while (!Serial);                // Wait for Serial to initialize (for debugging)
  pinMode(LIGHT_SENSOR, INPUT);    // Set light sensor pin as input
}

void loop() {
  int curr_light = readLightSensor();  // Read the current light sensor value

  // Check if light intensity is above a threshold
  if (curr_light > 20) {
    lstate = "On";    // Light is detected, set state to "On"
  } else {
    lstate = "Off";   // No light detected, set state to "Off"
  }

  // If light state has changed, send a webhook notification
  if (lstate != llight) {
    sendWebhook(lstate);  // Trigger IFTTT webhook
    llight = lstate;      // Update the previous light state
  }
  delay(1000);            // Wait for 1 second before checking again
}

// Function to send a webhook notification
void sendWebhook(String state) {
  // Try to connect to the IFTTT server
  if (!client.connect(HOST_NAME, 80)) {
    Serial.println("Connection failed!"); // Print failure message if connection unsuccessful
    return;
  }

  // Send HTTP GET request to the IFTTT webhook URL
  Serial.println("Connected to server");
  client.println("GET /trigger/sunlight_enter/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ HTTP/1.1");
  client.println("Host: maker.ifttt.com");
  client.println("Connection: close");
  client.println();       // End of request

  client.stop();          // Close the connection
  Serial.println("Sent notification to IFTTT");
}

// Function to read light sensor value
int readLightSensor() {
  lvalue = digitalRead(LIGHT_SENSOR);  // Read the value from the light sensor
  return lvalue;                       // Return the sensor value
}

// Function to connect to WiFi network
bool connectToWiFi(char ssid[], char pass[]) {
  if (WiFi.status() != WL_CONNECTED) {     // Check if not connected
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);              // Attempt to connect to WiFi
      Serial.print(".");                   // Print dots to show progress
      delay(5000);                         // Wait 5 seconds before retrying
      if (i > 6) {                         // After 6 attempts, give up
        Serial.print("Wifi Connection Failed!");
        return false;
        break;
      }
      i++;
    }
    Serial.println("\nConnected.");       // Print success message
    return true;                          // Return true if connected
  }
  return true;                            // If already connected, return true
}
