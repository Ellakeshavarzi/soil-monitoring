import serial
import csv
from datetime import datetime

# Port and baud
port = 'COM5'
baud = 115200

# Output CSV file
filename = f"sensor_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"

# Expected sensor labels (column headers)
headers = [
    "Timestamp",
    "BoardA_Moisture_1", "BoardA_Temp_1",
    "BoardA_Moisture_2", "BoardA_Temp_2",
    "Light_Lux", "Si7021_Temp", "Si7021_Humidity",
    "BoardB_Moisture_1", "BoardB_Temp_1",
    "BoardB_Moisture_2", "BoardB_Temp_2"
]

print(f"Listening on {port}...")
try:
    with serial.Serial(port, baud, timeout=1) as ser, open(filename, "w", newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=headers)
        writer.writeheader()
        print(f"Logging to {filename}")

        while True:
            line = ser.readline().decode("utf-8").strip()

            # Only parse lines that contain valid sensor data
            if line.startswith("BoardA_Moisture_1"):
                # Initialize data row
                data = {key: "" for key in headers}
                data["Timestamp"] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                # Split line into key:value pairs
                parts = line.split()
                for part in parts:
                    if ":" in part:
                        key, value = part.split(":")
                        key = key.strip()
                        value = value.strip()
                        if key in data:
                            data[key] = value

                writer.writerow(data)
                csvfile.flush()
                print(">>", data)

except serial.SerialException as e:
    print(f"Failed to open {port}: {e}")
