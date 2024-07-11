![Die Anzeige](./image/esp8266_max72xx_.png?raw=true "Title")

Eine Compact Anzeige von ein Paar werten der Photovoltaik Anlage.
Die Informationen werden über MQTT abgefragt und diverse Informationen auch per MQTT zurückgegeben.
Eine 8 LED Ring (WB2812) zeigt in bestimmten Abständen den Ladezustand (SOC) der PV-Batterie an.
Das ganze wird von einem IOBroker Script per MQTT bereitgestellt
Zu verbessern wäre hier:
1. Abfrage MQTT
2. Exaterer Abgleich des 8 LED Rings
3. Verbesserte Darstellung in der DotMatrix
