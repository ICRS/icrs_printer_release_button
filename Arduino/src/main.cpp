#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "config.h"

// Define the pins for the red and green LED
#define RED_LED_PIN 4
#define GREEN_LED_PIN 3
#define BUTTON_PIN 7
#define POWER_PIN 6

#define OFF 0
#define RED 1
#define GREEN 2
#define YELLOW 3

// Server details
char serverAddress[] = "192.168.1.113";  // Replace with your server address
int port = 30003;  // Replace with your server port

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);

// Global variable to control LED mode
volatile int mode = 0;  // 0: off, 1: red, 2: green, 3: yellow

// Timer variables
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 10000; // Interval to update mode in milliseconds

void updateMode() {
  String url = "/availability?printer_name=" + (String) printerName;

  client.get(url);

  // Read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("MODE UPDATE -> Status code: ");
  Serial.println(statusCode);
  Serial.print("MODE UPDATE -> Response: ");
  Serial.println(response);

  if (statusCode == 200) {
    // Check the response content and update mode accordingly
    // Assuming response contains some status info
    if (response.indexOf("true") >= 0) {
      mode = GREEN; // Set mode to green if printer is available
    } else if (response.indexOf("false") >= 0) {
      mode = YELLOW; // Set mode to yellow if printer is busy
    } else {
      mode = RED; // Set mode to red if status is unknown
    }
  } else {
    mode = RED; // Set mode to red if the POST request failed
  }
}

void flashLED(int mode, int times, int delayTime) {
  for (int i = 0; i < times; i++) {
    switch (mode) {
      case OFF: // LEDs off
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
        break;
      case RED: // Blink red
        if (delayTime != 0) {
          digitalWrite(RED_LED_PIN, !digitalRead(RED_LED_PIN));
          digitalWrite(GREEN_LED_PIN, LOW);
        } else {
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(GREEN_LED_PIN, LOW);
        }
        break;
      case GREEN: // Blink green
        if (delayTime != 0) {
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_LED_PIN));
        } else {
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(GREEN_LED_PIN, HIGH);
        }
        break;
      case YELLOW: // Blink yellow (both LEDs)
        if (delayTime != 0) {
          digitalWrite(RED_LED_PIN, !digitalRead(RED_LED_PIN));
          digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_LED_PIN));
        } else {
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(GREEN_LED_PIN, HIGH);
        }
        break;
    }
    
    if(delayTime != 0) {
      delay(delayTime);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      delay(delayTime);
    }
    
  }
}

void setup() {
  // Initialize the LED pins and other pins as outputs or inputs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button input with internal pull-up
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW); // Set power pin low

  // Initialize Serial Monitor
  Serial.begin(115200);

  client.setTimeout(5000); // Set timeout for HTTP request
  client.setHttpResponseTimeout(5000); // Set timeout for HTTP response

  // Start WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network:
    WiFi.begin(ssid, password);

    // Flash yellow light quickly while connecting
    for (int i = 0; i < 4 && WiFi.status() != WL_CONNECTED; i++) {
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, HIGH);
      delay(200); // Flash on
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, LOW);
      delay(200); // Flash off
    }
  }

  // WiFi connected
  Serial.println("WiFi connected");

  // Print the SSID and IP address
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Flash green to indicate successful connection
  flashLED(GREEN, 10, 100);
}

void loop() {
  // Check if it's time to update the mode
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    updateMode(); // Call the updateMode function asynchronously
  }

  // Flash the LED based on the current mode
  flashLED(mode, 1, 0);

  // Check if the button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    flashLED(YELLOW, 5, 100);
    Serial.println("Making POST request");

    String contentType = "application/json";
    String postData = "{\"\"}";

    String url = "/availability?printer_name=" + (String) printerName + "&available=true";

    client.post(url, contentType, postData);

    // Read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("CHANGE AVAILABILITY -> Status code: ");
    Serial.println(statusCode);
    Serial.print("CHANGE AVAILABILITY -> Response: ");
    Serial.println(response);

    // Updated the mode based on the response
    if (statusCode == 200) { // POST request was successful
      flashLED(GREEN, 4, 250); // Flash green 4 times slowly
      mode = GREEN; // Set mode to green
    } else { // POST request failed
      flashLED(RED, 4, 250); // Flash red 4 times slowly
      mode = RED; // Set mode to red
    }

    // Debounce delay
    delay(100);
  }

  // Flash the LED based on the current mode
  flashLED(mode, 1, 0);
}
