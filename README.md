# source-code-examples
Examples of code for the ESP8266

## IOT_LED
- A simple LED connected to a UDP socket.
- The following commands can be used over netcat on port 2500
  - 'LED' toggles the LED on GPIO2
  - 'ADC' sends the ADC value back over the network
  - 'RSSI' sends the RSSI of the connected access point back over the network (good for range calulcations)
