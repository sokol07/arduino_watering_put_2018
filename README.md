# Arduino Watering Plant for 3 plants  
### Project created for Internet Applications subject on semester 6 of Automation and Robotics studies at Poznan University of Technology in 2018.  

  
##### Main features:
  * Three plants watered  
  * Automatic moisture checking  
  * Automatic work with control using website  
  * Water in container with pump dry running prevention  
  * 12V solenoid valves  
  * 12v submersible pump  
  * NodeMCU controller with Arduino IDE sofware  
  * Thingspeak communication, visualization and statistics
  * simple website control: manual watering, checking moisture, changing triggering moisture level, changing water amoung per watering, periodic watering, visualization  
 
###### Electric scheme in file  ["podlewanie_bb.pdf"](https://github.com/sokol07/arduino_watering_put_2018/blob/master/podlewanie_bb.pdf).
###### Video presenting the device on  [YouTube](https://youtu.be/-tGSdsGSWy0).

##### Abstract:  
Main idea is to have an automatically watered station for three plants. It should be possible to leave you appartment for a dozen days without being worried about you green friends. Our device will check the moisture in pots and, if required, turn on watering. The important part is possibility to change most important parameters of the watering using a dedicated webpage.  
The whole device is based on one of NodeMCU devices due to their usability for IoT projects. In the prototype build a Wemos D1 Mini Lite was used. Other hardawre used was bought from Aliexpress and these were: 12V pump, 12V solenoid valves, moisture sensors, relays. NodeMCU has got only 1 analog input, thus, a 74HC4051 multiplexer was used.  

##### Description:
This paragraph describes usage of the code.
The first part of the program is the local one: checking available water level, checking moisture, watering. The more complicated part is the one concerning uploading and reading data from Thinspeak.com.
Uploading is pretty straightforward and it is done in the way which is described in Thingspeak documentation. However, reading data from Thingspeak channel is a bit more complicated: whole channel info is requested from Thingspeak in json format and than, using ArduinoJson library by Benoit Blanchon, it is decoded and converted from strings to int values. Whole program in commented, I think it is pretty easy to understand what is happening in it. However, for making it easier I'll explain which line is doing what:
* lines 4-9: initializing variables
* lines 12-16: variables necessary for reading data from Thingspeak
* lines 19-21: variables necessary for uploading data to Thingspeak
* lines 24-79: requesting json data from Thingspeak, converting to char, to string and finally to int.
* lines 84-101: setting up ESP pins, initial pullup of pins 0,2,4,5,16 (relays are controlled with LOW value)
* lines 104-118: connecting to WiFi
* line 121: beginning of loop function, 
* lines 124-129: checking moisture levels
* lines 132-164: Reading data from Thingspeak
* lines 167-196: Uploading data to Thingspeak
* line 200: checking water availability
* line 201-211: watering plants
* lines 212-229: Printing watering parameters in Serial
* lines 232-282: functions for: checking moisture levels, watering and checking water availability

###### A few words about Thingspeak configuration
The program is using two Thingspeak channels. First one of them is used for storing measured moisture levels: there are three fields used, one for every moisture level. Graphs generated automatically by Thingspeak were used for a webpage controlling the whole device remotely. The second channel is used for watering parameters: three trigger moisture levels ("water the plant if the moisture level is lower than..."), three water amounts and one enable value (if enable is false than the pump won't be turned on but the moisture will be read and uploaded). Therefore, ESP8266 was reading data from second channel (read api) and sending data to first channel (write api).  
  
  ###### Webpage
  The second side of the device was a webpage. Using it, the user was able to set watering parameters and read moisture levels (with graphs). The webpage was reading data from first channel and sending data to the second channel. It was done as described in Thingspeak documentation.  
  
  ###### Hardware
All hardware used in the project was described in the Abstract, nothing else was added. However, Id like to add a few notes here:
   * the project is using three different voltages which CAN'T be mistaken. Moisture sensors and water level sensor are using 3.3V as this is main logic voltage for Wemos D1. Relays are operated using 5V, which is also provided from Wemos board. Solenoid valves and pump are using 12V from pc power supply and this voltage must be well separated from Wemos as it is deadly for the board.
   * it is likely that there is an error is multiplexing (moisture sensors are checked in 1-3-2 order). However, it is also possible that I connected something in wrong order. Sorry for that.
   * the moisture sensors must be connected using the relay, cutting off the 3.3V supply to them. Otherwise they'd be always turned on and therefore they'd be damaged by the oxidation very soon.  
    