# Digital Thermometer

Code repository for my homemade digital thermometer. 
The Mcu I'm working with right now is an AVR ATmega328p (Arduino), 
but will be moving this over to an ATTiny84 for the final board.

The board consists of 3 common cathode 7-segment displays, 3 shift registers, 
and a MAX31820 one-wire temperature sensor.

## Board Design

### Bill of Materials

| QTY | PART#           | DESC                        | MANUF                                    |
|:---:| --------------- | --------------------------- | -----------------------------------------|
|  3  | YSD-160AR4B-8   | 7 Seg. Common Anode Display | China Young Sun LED Technology Co., LTD. |
|  3  | SN74HC595N      | 8 bit Shift Register        | Texas Instruments                        |
|  3  |                 | 16 pin DIP socket           |                                          |
| 24  |                 | 330 Ohm resistor            |                                          |
|  1  |                 | SPST normally open switch   |                                          |
|  1  |                 | 1uF capacitor               |                                          |
|  1  | ATtiny84        | 8k Flash, 14 pin DIP        | Atmel                                    |
|  1  |                 | 14 pin DIP socket           |                                          |
|  1  | MAX31820        | One wire temp sensor        | Maxim                                    |
|  1  |                 | 4.7 kOhm resistor           |                                          |

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



// Driven by 3 shift registers.
// 3 bytes of data is shifted out

// First shift register is the 10s digit (last byte shifted in)
// Second shift register is the 1s digit (second byte shifted in)
// Third (last) shift register is the inverted display for °C/°F