# 🌱 Soil Monitoring System with ESP32

This project is a real-time **soil, temperature, humidity, and light monitoring system** using two ESP32 boards (Board A and Board B), multiple sensors, and serial communication.

---

## 📦 Components

- 2 × ESP32-S3 boards (Board A & Board B)
- 4 × SMT50 Soil Moisture & Temperature sensors
- 1 × BH1750 Light sensor (I2C)
- 1 × Si7021 Humidity & Temperature sensor (I2C)
- Jumper wires
- Breadboards
- Optional: Raspberry Pi (for logging)

---

## 🔌 Wiring Instructions

### 🔗 Board A ↔ Board B Connections

| Purpose               | Board A Pin       | Board B Pin       | Description                                |
|-----------------------|-------------------|--------------------|--------------------------------------------|
| UART Receive (RXD2)   | `GPIO 16`         | `TX (GPIO 21)`     | For receiving serial data from Board B     |
| Ground                | `GND`             | `GND`              | Shared ground (must be connected)          |

> ⚠️ **TX from Board A is not used** in this setup.

---

### 🧪 Sensor Pin Assignments

#### 📍 Board A (Main Logger)

| Sensor                     | Signal     | ESP32 Pin |
|----------------------------|------------|-----------|
| SMT50 #1 (Soil + Temp)     | Moisture   | GPIO 4    |
|                            | Temp       | GPIO 6    |
| SMT50 #2 (Soil + Temp)     | Moisture   | GPIO 14   |
|                            | Temp       | GPIO 15   |
| BH1750 (Light)             | SDA        | GPIO 8    |
|                            | SCL        | GPIO 9    |
| Si7021 (Humidity + Temp)   | SDA        | GPIO 10   |
|                            | SCL        | GPIO 11   |
| Serial Input               | RX         | GPIO 16   |

#### 📍 Board B (Sensor Transmitter)

| Sensor                     | Signal     | ESP32 Pin |
|----------------------------|------------|-----------|
| SMT50 #3 (Soil + Temp)     | Moisture   | GPIO 14   |
|                            | Temp       | GPIO 15   |
| SMT50 #4 (Soil + Temp)     | Moisture   | GPIO 16   |
|                            | Temp       | GPIO 17   |
| Serial Output              | TX         | GPIO 21   |

---

## 🚀 Setup Instructions

### 1. Flash Arduino Code

- Flash `boardA.ino` to Board A
- Flash `boardB.ino` to Board B
- Make sure Board B is powered and transmitting before Board A starts reading

### 2. Start Serial Logging (Python)

Use the provided Python script (`data_import.py`) to log sensor readings to CSV:

```bash
python data_import.py
```

This script:
- Reads data from COM port (e.g. COM5)
- Writes to a timestamped `.csv` file
- Prints logs with timestamps

### 3. Visualize Data

Use the `data_visualize.py` script to create line plots of selected features:

```bash
python data_visualize.py
```
---

## 📝 File Structure

```
soil-monitoring/
│
├── boardA.ino           # Main board code
├── boardB.ino           # Secondary board sensor broadcaster
├── data_import.py        # Python script to save data to CSV
├── plot_data.py          # Visualizes sensor data
└── README.md             # This documentation
```

---

## 🛠️ Notes

- Ensure that **only one program accesses the COM port at a time**.
- Start Board B first so that Board A can receive sensor data.
- You can optionally attach a Raspberry Pi to run the Python logging and store data long-term.

---

## 📈 Sample Output

CSV data columns:
```
timestamp, BoardA_Moisture_1, BoardA_Temp_1, BoardA_Moisture_2, BoardA_Temp_2, Light_Lux, Si7021_Temp, Si7021_Humidity, BoardB_Moisture_1, BoardB_Temp_1, BoardB_Moisture_2, BoardB_Temp_2
```

Example:
```
2025-06-07 15:01:22, 1.25, 23.6, 0.94, 24.1, 0.00, 22.5, 60.1, 1.10, 23.8, 1.03, 24.3
```

---

## 📧 License

MIT License  
