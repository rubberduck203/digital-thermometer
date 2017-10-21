# Digital Thermometer

Code repository for my homemade digital thermometer. 
The Mcu I'm working with right now is an AVR ATmega328p (Arduino), 
but will be moving this over to an ATTiny84 for the final board.

The board consists of 3 common cathode 7-segment displays, 3 shift registers, 
and a MAX31820 one-wire temperature sensor.

## Board Design

### Bill of Materials
TODO:

### Layout
TODO: 

## Code Design

- Main initializes the application then delegates to the Controller.
- Controller coordinates interactions between the components.
- Display abstracts our 3 7-seg displays as a single display.
- 7-Segment Driver defines character mappings and uses a Shift Register Driver to display them.
- Shift Register Driver knows how to shift bits into the registers.
- CFSwitch (°C/°F Switch) monitors switch state so we know if the user wants °C or °F displayed.
- TempSensor reads data from the Max31820.
- TempConverter converts °C to °F

```
                                   +------------+
                                   |            |
                                   |    Main    |
                                   |            |
                                   |            |
                                   +------+-----+
                                          |
                                          |
+----------+       +----------+    +------v-----+     +-------------+      +--------------+
|          |       |          |    |            |     |             |      |              |
| 7-Seg    <-------+ Display  <----+ Controller +----->  Temp       +------> One Wire     |
| Driver   |       |          |    |            |     |  Sensor     |      | Driver       |
|          |       |          |    |            |     |             |      |              |
+----+-----+       +----------+    +---+-----+--+     +-------------+      +--------------+
     |                                 |     |
     |                            +----+     +----+
+----v-----+                      |               |
|          |               +------v-----+     +---v---------+
| Shift    |               |            |     |             |
| Register |               |  C/F       |     | C/F Switch  |
|          |               |  Converter |     |             |
+----------+               |            |     |             |
                           +------------+     +-------------+

```