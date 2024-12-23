#include <Arduino.h>
#define BLYNK_TEMPLATE_ID "TMPL6UVuUVijB"
#define BLYNK_TEMPLATE_NAME "Led Controller"
#define BLYNK_AUTH_TOKEN "Y8_VOgkci5kNhAWmxULwJelTsFWFRgsZ"

#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Hardware setup
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 8

#define CLK_PIN 18  // GPIO18: Clock
#define DATA_PIN 23 // GPIO23: Data In
#define CS_PIN 5    // GPIO5: Chip Select

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Scrolling parameters
uint8_t frameDelay = 25;               // default frame delay value
textPosition_t scrollEffect = PA_LEFT; // default scroll effect is left
uint8_t scrollSpeed = 5;               // default scroll speed value (change as needed)
uint8_t brightnessLevel = 0;           // default brightness level (range: 1-15)

#define BUF_SIZE 512
char curMessage[BUF_SIZE] = "Ayoh! Horob1 is coming!";
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  P.begin();
  P.displayClear();
  P.displaySuspend(false);

  delay(1000);
  Blynk.begin(auth, ssid, pass);
  delay(2000);
  P.displayScroll(curMessage, scrollEffect, PA_SCROLL_LEFT, frameDelay);
}

void loop()
{
  // Run Blynk
  Blynk.run();

  // Check if a new message is available and update the display
  if (newMessageAvailable)
  {
    strcpy(curMessage, newMessage);
    newMessageAvailable = false;
    P.displayReset();
  }

  // Animate the display
  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
}

// Blynk virtual pin handler for receiving messages
BLYNK_WRITE(V0)
{
  strncpy(newMessage, param.asString(), BUF_SIZE);
  newMessageAvailable = true;
}
