#include "HX711.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <GoogleSheets.h>

// Define the pins used by the HX711 and load cell
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;

// Create an instance of the HX711 library with the specified pins
HX711 loadcell(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

// WiFi settings
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Google Sheets settings
const char* host = "script.google.com";
const char* scriptId = "YOUR_SCRIPT_ID";

// Create an instance of the Google Sheets client
GoogleSheets client(host, scriptId);

void setup() {
  // Initialize the serial port
  Serial.begin(9600);

  // Initialize the load cell
  loadcell.set_scale(2280.f); // set the conversion factor
  loadcell.tare();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Read the load cell data
  float load = loadcell.get_units();

  // Print the load cell data to the serial monitor
  Serial.print("Load: ");
  Serial.print(load);
  Serial.println(" kg");

  // Upload the load cell data to Google Sheets
  int statusCode = client.sendData(load);

  // Check if the data was successfully uploaded
  if (statusCode == 200) {
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error sending data: ");
    Serial.println(statusCode);
  }

  // Wait for 1 second before taking the next reading
  delay(1000);
}
