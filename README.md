#  Smart Irrigation System using ESP8266

A smart and IoT-enabled irrigation system that automates watering based on soil moisture, temperature, and humidity readings. The system uses an ESP8266 NodeMCU, sensors, and a mobile app (Blynk) for real-time monitoring and control.

---

#  Features

- 💧 Automatic irrigation based on soil moisture
- 🌡️ Real-time temperature and humidity sensing using DHT11
- 📱 Remote monitoring and pump control using Blynk App
- 🖥️ Live status display using 16x2 I2C LCD
- 🔘 Manual pump control using a push button
- 🔌 Low-power operation with 18650 battery

---

#  Components Used

| Component                             | Quantity |
|---------------------------------------|----------|
| ESP8266 NodeMCU                       |     1    |
| Soil Moisture Sensor (FC-28)          |     1    |
| DHT11 Temperature & Humidity Sensor   |     1    |
| 5V Relay Module                       |     1    |
| Water Pump                            |     1    |
| 16x2 I2C LCD Display                  |     1    |
| Push Button                           |     1    |
| 18650 Battery + Holder                |     1    |
| Jumper Wires + Breadboard             | as needed |

---

#  How It Works

- The DHT11 sensor measures ambient temperature and humidity.
- The Soil Moisture Sensor checks the soil's wetness level.
- Based on the moisture percentage:
  - If too dry, the pump turns ON automatically.
  - If moist enough, the pump remains OFF.
- You can manually override the pump using:
  - A push button, or
  - The Blynk mobile app (Virtual Pin `V12`).
- All data (temp, humidity, moisture level, pump status) is displayed on:
  - The LCD display
  - The Blynk app interface

---

#  Setup Instructions

1. Set Up Hardware:
  Connect all components as per the wiring diagram.
  Make sure sensors are properly connected to their respective GPIO pins.
2. Install Arduino IDE & ESP8266 Board:
  Add ESP8266 board via Board Manager.
  Select NodeMCU 1.0 as the board.
3. Install Required Libraries:
  Use Library Manager or download libraries manually.
4. Open Code:
  Open sketch_feb29a.ino in Arduino IDE.
5. Update Wi-Fi & Blynk Credentials:
  Replace ssid, pass, and BLYNK_AUTH_TOKEN with your actual Wi-Fi and Blynk details.
6. Compile and Upload:
  Connect ESP8266 to your PC.
  Select correct Port and Board, then upload the code.
7. Use the System:
  Open the Blynk App and start the project.
  Monitor live data and control the pump remotely.
  Use the LCD to observe real-time values locally.

