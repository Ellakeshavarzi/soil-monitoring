# Soil Monitoring System

**An experimental platform for collecting and visualizing real-time soil moisture, temperature, humidity, and light data using ESP32 microcontrollers.**

---

## 📡 Overview

This project uses two ESP32-S3 boards to collect environmental data from soil sensors and I2C modules, and transmits the data via UART for unified logging and visualization.

- **Board A:** Reads from 2 SMT50 sensors, a BH1750 light sensor, and an Si7021 humidity/temperature sensor. It also receives data from Board B.
- **Board B:** Reads from 2 additional SMT50 sensors and sends the data over Serial to Board A.
- Data is sent over Serial to a Raspberry Pi or laptop for logging and plotting.

---

## 🛠 Hardware Setup

### Sensors:
- 3x **SMT50** Soil Moisture + Temperature sensors
- 1x **BH1750** Light sensor (I2C)
- 1x **Si7021** Humidity + Temperature sensor (I2C)

### Boards:
- 2x **ESP32-S3-WROOM-1**
- 1x **Raspberry Pi (or any computer)** for logging

---

## 🧰 Software Components

- **Arduino IDE** for ESP32 code
- **Python script** (via `pyserial`) for data logging on Raspberry Pi or laptop
- **Arduino Serial Plotter** (optional, for real-time visualization)

---

## 🔌 Wiring

- **Board A**
  - I2C1: BH1750 — `SDA=8`, `SCL=9`
  - I2C2: Si7021 — `SDA=10`, `SCL=11`
  - UART: RXD=16 (receives from Board B's TX)
  - SMT50 sensors on analog pins: 4, 14 (moisture); 6, 15 (temperature)

- **Board B**
  - SMT50 sensors on analog pins: 14, 16 (moisture); 15, 17 (temperature)
  - UART TX connected to Board A RXD

---

## 🧪 Data Format

Each 10 seconds, Board A sends the following tab-separated values over Serial:

```
BoardA_Moisture_1    BoardA_Temp_1    BoardA_Moisture_2    BoardA_Temp_2    Light_Lux    Si7021_Temp    Si7021_Humidity    BoardB_Moisture_1    BoardB_Temp_1    BoardB_Moisture_2    BoardB_Temp_2
```

Each variable appears on its own line for real-time plotting.

---

## 🖥 Python Serial Logger

Example Python script to run on the Raspberry Pi:

```python
import serial
import time

with serial.Serial('/dev/ttyUSB0', 115200, timeout=1) as ser:
    with open("sensor_log.tsv", "a") as log_file:
        while True:
            line = ser.readline().decode('utf-8').strip()
            print(line)
            log_file.write(line + "\n")
            log_file.flush()
```

> 🔧 Adjust `/dev/ttyUSB0` if needed (e.g., `COM3` on Windows).

---

## 📊 Visualization

- Use `Arduino Serial Plotter` for quick feedback.
- Data can be imported into tools like **Excel**, **Google Sheets**, or **Python (pandas/matplotlib)** for deeper analysis.

---

## 🔄 Future Work

- Add support for wireless transmission (e.g., WiFi + MQTT or Bluetooth)
- Automate irrigation based on threshold values
- Design a web-based live dashboard

---

## 🧑‍💻 Authors

Developed by Jasper Antonius Vollmert, Elaheh Keshavarzikhoozani, and Adel Alatassi. Contributions welcome!

---

## 📄 License

MIT License — feel free to use and modify with attribution.
