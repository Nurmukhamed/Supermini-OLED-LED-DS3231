# Intro

This is small project aimed to ping (icmp) frigate NVR host on some country side.

# Parts used

* [ESP32-C3 Supermini][1];
* [DS3231 board for PI][2];
* [OLED 0.96 Display][3];
* [WS2821 LED Ring][4];
* [MP1584EN DC/DC][5];
* [AMS1117-3.3 DC/DC][6];
* [Some board to place][7].
* Some pins, wire to build it.

# Code were borrowed from

* [RTC and OLED][8];
* [RTC and OLED][9];
* [WS2821 Led Ring][10];

# Platformio required.

Please create file in src - secrets.h 

~~~bash
const char* ssid     = "YOURSSID";
const char* password = "YOURPASSWORD";
const char* hostname = "esp32-node-temperature";
const char* frigate_ip = "192.168.1.1";
~~~

# Links 
* [1]: https://aliexpress.ru/item/1005006241069300.html?spm=a2g2w.orderdetail.0.0.48764aa6QgWFs6&sku_id=12000037691944835
* [2]: https://aliexpress.ru/item/32315883368.html?spm=a2g2w.orderdetail.0.0.1c994aa6tZMocu&sku_id=58978523933
* [3]: https://aliexpress.ru/item/1005007696040254.html?spm=a2g2w.orderdetail.0.0.7ed24aa6AxVX9W&sku_id=12000048371935539
* [4]: https://aliexpress.ru/item/32835427711.html?spm=a2g2w.orderdetail.0.0.76f94aa6w0WDiL&sku_id=66934085589
* [5]: https://aliexpress.ru/item/1005005870392716.html?spm=a2g2w.orderdetail.0.0.33764aa6ZH7Dfb&sku_id=12000034645748480
* [6]: https://aliexpress.ru/item/1005002065656202.html?sku_id=12000043386862310&spm=a2g2w.productlist.search_results.7.65d36bb6q9hygL
* [7]: https://aliexpress.ru/item/1005001526781123.html?sku_id=12000016471650073&spm=a2g2w.productlist.search_results.2.5acd1111dhEQSv
* [8]: https://www.hackster.io/mdraber/tutorial-on-how-to-use-ds3231-rtc-module-e12b38
* [9]: https://www.youtube.com/watch?v=qHRPHefREAY
* [10]: https://github.com/voidlooprobotech/Aruino-basic-to-advance-examples/tree/main/59%20WS2812%20RBG%20with%20Arduino/strandtest

