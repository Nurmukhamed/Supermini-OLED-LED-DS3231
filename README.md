# Intro

This is small project aimed to ping (icmp) frigate NVR host on some country side.

# Parts used

* ESP32-C3 Supermini;
* DS3231 board for PI;
* OLED 0.96 Display;
* WS2821 LED Ring;
* MP1584EN DC/DC;
* AMS1117-3.3 DC
* Some board to build it.
* Some pins, wire to build it.

# Platformio required.

Please create file in src - secrets.h 

~~~bash
const char* ssid     = "YOURSSID";
const char* password = "YOURPASSWORD";
const char* hostname = "esp32-node-temperature";
const char* frigate_ip = "192.168.1.1";
~~~


