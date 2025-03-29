# Arduino Code for Automated Plant Care System

This Arduino code implements a basic automated plant care system that monitors soil moisture, temperature, humidity, and rainfall. It controls a water pump and a shade mechanism based on these sensor readings.

## Hardware Connections

The code defines the following pin assignments:

**Shade Control:**

* `shadeOn`: Digital Pin 5 (Shade ON control) - Connected to a mechanism that extends the shade (e.g., through a relay or motor driver).
* `shadeOff`: Digital Pin 6 (Shade OFF control) - Connected to a mechanism that retracts the shade.
* `shadeSpeed`: Digital Pin 3 (Shade Motor Speed Control - PWM) - Connected to the speed control input of the shade motor driver.
* `shadeValue`: Defined as 240, representing the PWM value for the shade motor speed (0-255).

**Water Control:**

* `water`: Digital Pin 4 (Water Pump Control) - Connected to the control pin of a water pump (likely through a relay or transistor).

**Sensors:**

* `soilSensor`: Analog Pin A3 (Soil Moisture Sensor) - Connected to the analog output of the soil moisture sensor.
* `rainSensor`: Analog Pin A1 (Rain Sensor) - Connected to the analog output of the rain sensor.
* `DHTPIN`: Digital Pin 7 (DHT Sensor Data Pin) - Connected to the data pin of the DHT11 temperature and humidity sensor.
* `DHTTYPE`: Defined as `DHT11`, specifying the type of DHT sensor being used.

## Code Description

### Includes

* `#include <DHT.h>`: Includes the library necessary for interacting with DHT series temperature and humidity sensors.

### Definitions

* Pin assignments for shade control, water control, and sensors are defined using `#define`.
* `shadeValue`: Sets the PWM value for the shade motor speed.
* `shadeStatus`: An integer variable to track the current state of the shade (0 for off/retracted, 1 for on/extended). Initialized to 0.

### DHT Sensor Initialization

* `DHT dht(DHTPIN, DHTTYPE);`: Creates an object `dht` of the `DHT` class, specifying the data pin and sensor type.

### `setup()` Function

* Initializes the serial communication at a baud rate of 115200 for sending sensor data.
* Sets the pin modes for the control and sensor pins:
    * `shadeOn`, `shadeOff`, `shadeSpeed`, and `water` are set as `OUTPUT`.
    * `soilSensor` and `rainSensor` are set as `INPUT`.
* Initializes the shade to the OFF (retracted) state by setting `digitalWrite(shadeOn, LOW)`.
* Sets the speed of the shade motor using `analogWrite(shadeSpeed, shadeValue)`.
* Initializes the DHT sensor using `dht.begin()`.

### Global Variables

* `soil`: A double variable to store the soil moisture level (mapped to 0-100).
* `temp`: A double variable to store the temperature reading from the DHT sensor.
* `hum`: A double variable to store the humidity reading from the DHT sensor.
* `rain`: An integer variable to indicate the presence of rain (1 for rain detected, 0 for no rain).

### `readSensorValues()` Function

* Reads the analog value from the `soilSensor`.
* Maps the raw soil sensor reading (assumed to be in the range of approximately 250-450, where lower values indicate more moisture) to a percentage range of 0-100. **Note:** These raw values might need adjustment based on your specific sensor.
* Reads the analog value from the `rainSensor`.
* Determines if it's raining based on the `rainSensor` reading. If the reading is below 600, `rain` is set to 1 (rain detected); otherwise, it's set to 0. **Note:** This threshold might need adjustment based on your specific rain sensor.
* Reads the temperature and humidity from the DHT sensor using `dht.readTemperature()` and `dht.readHumidity()`.
* Calls the `constructString()` function to prepare the sensor data for serial output.

### `getMoisture()` Function

* Reads the analog value from the `soilSensor` and maps it to the 0-100 percentage range. This function is specifically used to get an updated soil moisture reading after watering.

### `putShadeOn()` Function

* Activates the shade extension mechanism by setting `digitalWrite(shadeOn, HIGH)` and `digitalWrite(shadeOff, LOW)`.
* Introduces a delay of 5500 milliseconds (5.5 seconds), which is assumed to be the time it takes for the shade to fully extend. **Note:** Adjust this delay based on your hardware.
* Deactivates the shade extension by setting `digitalWrite(shadeOn, LOW)`.
* Updates the `shadeStatus` to 1 (shade is on).

### `takeShadeOff()` Function

* Activates the shade retraction mechanism by setting `digitalWrite(shadeOff, HIGH)` and `digitalWrite(shadeOn, LOW)`.
* Introduces a delay of 4500 milliseconds (4.5 seconds), assumed to be the retraction time. **Note:** Adjust this delay based on your hardware.
* Deactivates the shade retraction by setting `digitalWrite(shadeOff, LOW)`.
* Updates the `shadeStatus` to 0 (shade is off).

### `constructString()` Function

* Creates a double array `valuesList` to store the sensor readings (soil moisture, temperature, humidity, rain).
* Populates the array with the current sensor values.
* Constructs a comma-separated string `message` containing the sensor values.
* Sends the `message` over the serial port using `Serial.write(message.c_str())`.

### `loop()` Function

* Continuously executes the following steps:
    * Calls `readSensorValues()` to update the sensor readings and send them over serial.
    * **Watering Logic:**
        * Checks if the `soil` moisture level is below 30%.
        * If it is, enters a `while` loop that continues as long as the soil moisture remains below 30%.
        * Inside the loop:
            * Turns the water pump ON by setting `digitalWrite(water, HIGH)` for 2000 milliseconds (2 seconds).
            * Turns the water pump OFF by setting `digitalWrite(water, LOW)` for 5000 milliseconds (5 seconds).
            * Calls `getMoisture()` to get an updated soil moisture reading.
    * **Shade Control Logic:**
        * Checks if it is raining (`rain == 1`) and the shade is currently off (`shadeStatus == 0`). If both are true, it calls `putShadeOn()` to extend the shade.
        * Else if it is not raining (`rain == 0`) and the shade is currently on (`shadeStatus == 1`), it calls `takeShadeOff()` to retract the shade.
    * Introduces a delay of 1000 milliseconds (1 second) before the next iteration of the loop.

## Notes

* **Sensor Calibration:** The raw readings and mapping ranges for the soil and rain sensors might need calibration based on the specific sensors you are using and the environmental conditions in Kandy, Central Province, Sri Lanka. Use the Serial Monitor to observe the raw sensor values in different states (e.g., dry soil, wet soil, no rain, raining) and adjust the `map()` function parameters and the rain detection threshold accordingly.
* **Shade and Water Timing:** The delays used for extending and retracting the shade (`delay(5500)`, `delay(4500)`) and the watering duration (`delay(2000)`) should be adjusted based on the characteristics of your specific shade mechanism and water pump to ensure proper operation.
* **DHT Sensor Readings:** The DHT11 sensor has limitations in terms of accuracy and reading frequency. If more precise and frequent readings are required, consider using a different DHT sensor (e.g., DHT22) or another type of temperature and humidity sensor.
* **Error Handling:** The code includes basic sensor reading, but more robust error handling could be added, especially for the DHT sensor (e.g., checking for `isnan()` results).
* **Power Considerations:** Ensure that your power supply is adequate for all the connected components (Arduino, sensors, motors, pump).
* **Wiring:** Double-check all the wiring connections between the Arduino and the sensors, motors, and pump before running the code. Incorrect wiring can damage your components.
* **Location Specifics:** The rain detection threshold might need fine-tuning based on the typical rainfall patterns in Kandy.

This code provides a foundation for an automated plant care system. You can expand upon it by adding more sensors, implementing more sophisticated control logic, and integrating it with other systems (e.g., for remote monitoring and control).
