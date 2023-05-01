// IMPORTANT: Set these differently for each bulb
#define HOSTNAME "lifx-switch1" // lifx-switch1.local
#define TARGET_BULB_IP "192.168.0.109"

#define SWITCH_PIN D4

#include <Arduino.h>
#include <Bounce2.h>
#include <WiFiSetup.h>

#include <ArduinoLifx.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <WiFiUdp.h>

void toggleLight();

Bounce2::Button button = Bounce2::Button();

WiFiUDP Udp;
ArduinoLifx lifx(Udp);

char targetIP[] = TARGET_BULB_IP;

void setup() {
  Serial.begin(9600);

  initWiFiManager();
  initOTA();

  button.attach(SWITCH_PIN, INPUT_PULLUP);
  button.interval(25);
  button.setPressedState(LOW);

  // Don't need this for standard network
  char targetMac[] = "000000000000";

  IPAddress myIp = WiFi.localIP();
  lifx.begin(myIp, targetIP, targetMac);
  lifx.setFlags(1, 1, 1);
}

void loop() {
  ArduinoOTA.handle();

  button.update();
  if (button.pressed()) {
    toggleLight();
  }

  // For testing button via serial
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 't') {
      toggleLight();
    }
  }
}

void toggleLight() {
  // TODO: Get current state from bulb
  static bool lightOn = false;
  lightOn = !lightOn;
  lifx.setPower(lightOn, 700);
}
