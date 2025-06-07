import pandas as pd
import matplotlib.pyplot as plt
from tkinter import Tk, filedialog

Tk().withdraw()

# Select the CSV file
file_path = filedialog.askopenfilename(
    title="Select CSV file",
    filetypes=[("CSV files", "*.csv")]
)
if not file_path:
    print("No file selected.")
    exit()

# Load CSV
df = pd.read_csv(file_path)

# Convert timestamp column to datetime if present
if 'Timestamp' in df.columns:
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])

# Plot each sensor as a line
plt.figure(figsize=(14, 7))
for column in df.columns:
    if column != 'Timestamp':
        plt.plot(df['Timestamp'], df[column], label=column)

plt.xlabel("Time")
plt.ylabel("Sensor Values")
plt.title("Sensor Data Over Time")
plt.legend()
plt.xticks(rotation=45)
plt.tight_layout()
plt.grid(True)
plt.show()
