# Precision-Agriculture
This is an IoT project for an automated irrigation and farm monitoring system.

Farming parameters such as soil moisture, humidity and temperature are sent to a Raspberry pi server which handles the automation of the water pump. Data is also sent to ThingSpeak, an IoT analytics platform service that allows you to aggregate, visualize, and analyze live data streams in the cloud. 

It makes use of the ESP8266 development board,DHT sensor and a soil moisture sensor. We will use MQQT protocol to send data to the raspberry pi. MQTT is a lightweight, publish-subscribe, machine to machine network protocol.

I will try to update the server-side scripts but here are some useful links in the meantime.

1. ThingSpeak platform https://www.instructables.com/Installing-MQTT-BrokerMosquitto-on-Raspberry-Pi/
2. Installation of the MQQT broker on the Raspberry Pi https://pimylifeup.com/raspberry-pi-mosquitto-mqtt-server/
3. Node RED - I used this to develop the server-side scripts, it makes use of provides a browser-based flow editor that makes it easy to wire together flows using the wide range of nodes in the palette (that is how it's difficult to share the code). Flows can be then deployed to the runtime in a single-click. https://nodered.org/docs/getting-started/raspberrypi
