# Wearable-AQI-detection-Band-
ESP32 Wearable Air Quality Monitor Project Overview The Wearable AQI Detection Band is an ESP32-based IoT device designed to monitor environmental conditions in real-time. It tracks gas concentrations and ambient temperature/humidity, displaying the data on a compact OLED screen.  

#Components List
Microcontroller: ESP32 (DevKit V1)
Gas Sensor: MQ-2 (Combustible gas and smoke)
Environment Sensor: DHT11 (Temperature and Humidity)
Display: 0.96" OLED Display (SSD1306)
Power: Jumper wires and breadboard (for prototyping)

Hardware Connections-
Power Distribution
Breadboard +ve Rail: Connected to ESP32 3.3V
Breadboard -ve Rail: Connected to ESP32 GND
OLED VCC: Connected to +ve rail
OLED GND: Connected to -ve rail
DHT11 VCC: Connected to +ve rail
DHT11 GND: Connected to -ve rail
MQ-2 Gas Sensor VCC: Connected to ESP32 5V (Note: MQ sensors typically require 5V for the internal heater to function correctly).
MQ-2 Gas Sensor GND: Connected to -ve rail

Data Pins
MQ-2 Analog Out (A0): Connected to ESP32 GPIO 34
DHT11 Data Pin: Connected to ESP32 GPIO 4
OLED SCL: Connected to ESP32 GPIO 22
OLED SDA: Connected to ESP32 GPIO 21

Software Requirements-
This project is developed using Arduino IDE 2.3.6. The following libraries and board packages must be installed:
Library Manager:  U8g2 by Oliver: Used for monochrome LCD/OLED displays (supports SSD1306, SSD1309, etc.).
DHT sensor library by Adafruit.

Board Manager:
esp32 by Espressif Systems (Supports boards like LOLIN D32, ESP32 Dev Module, etc.).


MORE INFO ABOUT THE PROJECT FOR THE COMPONENTS USED:--
1.The MQ-2 is excellent for detecting smoke and combustible gases like LPG and Butane. However, for a wearable, its main limitation is power consumption. The internal heating element requires significant current (approx. 150mA), which can drain a small wearable battery quickly. In a production version, I would consider a MEMS-based gas sensor for lower power consumption.

2.The MQ-2 sensor requires a 5V supply to heat the internal sensing element to the required operating temperature. However, the analog output signal must be checked to ensure it doesn't exceed 3.3V to protect the ESP32 GPIO. If the output exceeds 3.3V, a voltage divider circuit should be used.

3.I2C was chosen because it only requires two pins (SDA and SCL), which is ideal for a wearable project where minimizing wire count and footprint is essential. While SPI is faster, I2C speed is more than sufficient for updating a small 128x64 OLED display.

4.The DHT11 uses a proprietary "Single-Bus" serial protocol. It is not standard I2C or UART. It uses a specific timing-based handshake where the ESP32 pulls the line low to start communication, and the sensor responds with a 40-bit data packet containing humidity and temperature values.

5.If this device were to run on a battery----
I would implement "Deep Sleep" mode. The ESP32 would wake up every 5-10 minutes, take a sensor reading, update the display, and immediately return to deep sleep. Additionally, I would use a MOSFET to turn off power to the sensors (especially the MQ-2 heater) during sleep cycles.

6.The U8g2 library is highly memory-efficient and supports a massive range of fonts and display controllers. It also offers "page buffer" mode, which uses very little RAM, making it ideal if the project expands to include more complex graphics or animations.

handle "noise" in the analog readings from the MQ-2 sensor-----
Analog sensors often have fluctuations. I handle this in the firmware by implementing software filtering, such as "Moving Average Filtering," where I take 10 readings in quick succession and calculate the average to ensure a stable AQI value on the display.
