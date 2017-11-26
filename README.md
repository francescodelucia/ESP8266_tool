# ESP8266_tool
Framework for simple develop Wifi ESP8266 application with Arduino IDE.

This FramewWork is composed by varius Classes (C++) and can help programmer  
to develope complex application in tiny time.

The Framework contains structural classes for:

- Manage Network comunication
- Manage Timing event (like a tiny sheduler)
- Manage Sensor on I2C  protocol for read Values ( At moment only LM75, but in future can simply expanded)
- Manage a simplify a creation of Telegram Bot
- Manage and Creation of Web Pages for personal Applications.
- NTP Client for the timing sincronization.

Base Wifi Settings
when u start first time the device.You find Ap with name like this ESP8266<mac_device> the password for device is
<mac_device>:
  es. AP-SSID -> ESP82665ccf7f10b141
      AP-PASSWORD -> 5ccf7f10b141

Naturally you can change AP-SSID and Password on manager page.

Base Home page OutOfBox give information of Device
http://x.x.x.x

Setting page can find at
http://x.x.x.x/manager

In the manager page you can:

- Change Ap to connect like client
- Enable NTP ad decide the server
- Change Local Time
- Update Firmware OTA
- Reboot device






