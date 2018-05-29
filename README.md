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

###### Electric scheme in file ["podlewanie_bb.pdf"](https://github.com/sokol07/arduino_watering_put_2018/blob/master/podlewanie_bb.pdf).
###### Component .ino files (basic functions in separate files) are in ["Component" branch](https://github.com/sokol07/arduino_watering_put_2018/tree/components).
  
##### Abstract:  
Main idea is to have an automatically watered station for three plants. It should be possible to leave you appartment for a dozen days without being worried about you green friends. Our device will check the moisture in pots and, if required, turn on watering. The important part is possibility to change most important parameters of the watering using a dedicated webpage.  
The whole device is based on one of NodeMCU devices due to their usability for IoT projects. In the prototype build a Wemos D1 Lite was used. Other hardawre used was bought from Aliexpress and these were: 12V pump, 12V solenoid valves, moisture sensors, relays. NodeMCU has got only 1 analog input, thus, a 74HC4051 multiplexer was used.  
#### More info will be added along with the code, project is still in development, master branch consists of the most up-to-date and complex files.
