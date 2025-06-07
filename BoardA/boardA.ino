#include <Wire.h>

// Variables for Board B data
float boardB_moisture1 = 0, boardB_temp1 = 0;
float boardB_moisture2 = 0, boardB_temp2 = 0;

// I2C sensors
#define BH1750_ADDR 0x23
#define SI7021_ADDR 0x40
TwoWire I2CSi7021 = TwoWire(1);

// I2C pin setup
#define SDA_PIN 8
#define SCL_PIN 9
#define SI7021_SDA_PIN 10
#define SI7021_SCL_PIN 11

// UART2 pins
#define RXD2 16
#define TXD2 17

// Board A SMT50 sensor pins
const int boardAMoisturePins[2] = {4, 14};
const int boardATempPins[2] = {6, 15};

bool bh1750Present = false;
bool si7021Present = false;

// Board A sensor data
float boardA_moisture1 = 0, boardA_temp1 = 0;
float boardA_moisture2 = 0, boardA_temp2 = 0;
float lightLevelLux = 0;
float si7021_temp = 0, si7021_humidity = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(1000);

  Serial.println("Board A: Starting...");

  analogReadResolution(12);
  for (int i = 0; i < 2; i++) {
    analogSetPinAttenuation(boardAMoisturePins[i], ADC_11db);
    analogSetPinAttenuation(boardATempPins[i], ADC_11db);
  }

  Wire.begin(SDA_PIN, SCL_PIN);
  I2CSi7021.begin(SI7021_SDA_PIN, SI7021_SCL_PIN);

  Wire.beginTransmission(BH1750_ADDR);
  if (Wire.endTransmission() == 0) {
    bh1750Present = true;
    Wire.beginTransmission(BH1750_ADDR);
    Wire.write(0x10); // Continuous High Res Mode
    Wire.endTransmission();
    Serial.println("BH1750 configured.");
  }

  I2CSi7021.beginTransmission(SI7021_ADDR);
  if (I2CSi7021.endTransmission() == 0) {
    si7021Present = true;
    I2CSi7021.beginTransmission(SI7021_ADDR);
    I2CSi7021.write(0xFE); // Reset
    I2CSi7021.endTransmission();
    delay(50);
    Serial.println("Si7021 reset.");
  }

  Serial.println("BoardA_Moisture_1\tBoardA_Temp_1\tBoardA_Moisture_2\tBoardA_Temp_2\tLight_Lux\tSi7021_Temp\tSi7021_Humidity\tBoardB_Moisture_1\tBoardB_Temp_1\tBoardB_Moisture_2\tBoardB_Temp_2");

}

void loop() {
  Serial.println("\n==============================");

  // Read Board A SMT50 sensors
  for (int i = 0; i < 2; i++) {
    int rawMoisture = analogRead(boardAMoisturePins[i]);
    int rawTemp = analogRead(boardATempPins[i]);
    float voltMoisture = (rawMoisture / 4095.0) * 3.3;
    float voltTemp = (rawTemp / 4095.0) * 3.3;
    float moisturePercent = (voltMoisture * 0.050 / 3.0) * 100;
    float tempCelsius = (voltTemp - 0.5) * 100;

    if (i == 0) {
      boardA_moisture1 = moisturePercent;
      boardA_temp1 = tempCelsius;
    } else {
      boardA_moisture2 = moisturePercent;
      boardA_temp2 = tempCelsius;
    }
  }

  // Read light sensor
  if (bh1750Present) {
    Wire.requestFrom(BH1750_ADDR, 2);
    if (Wire.available() >= 2) {
      uint16_t rawLevel = Wire.read() << 8 | Wire.read();
      lightLevelLux = rawLevel / 1.2;
    }
  }

  // Read humidity and temp from Si7021
  if (si7021Present) {
    si7021_humidity = readSi7021Humidity();
    si7021_temp = readSi7021Temperature();
  }

  // Read from Board B via Serial
  while (Serial2.available()) {
    String line = Serial2.readStringUntil('\n');
    if (line.startsWith("SENSOR_B:")) {
      int sensorNum = line.charAt(9) - '0';
      int eqIdx = line.indexOf('=');
      int cIdx = line.indexOf('C');
      int slashIdx = line.indexOf('/');

      if (eqIdx > 0 && cIdx > eqIdx && slashIdx > cIdx) {
        float temp = line.substring(eqIdx + 1, cIdx).toFloat();
        float moisture = line.substring(slashIdx + 1, line.length() - 1).toFloat();
        if (sensorNum == 1) {
          boardB_temp1 = temp;
          boardB_moisture1 = moisture;
        } else if (sensorNum == 2) {
          boardB_temp2 = temp;
          boardB_moisture2 = moisture;
        }
      }
    }
  }

  // Print each value for Serial Plotter
  Serial.print("BoardA_Moisture_1:"); Serial.print(boardA_moisture1); Serial.print(" ");
  Serial.print("BoardA_Temp_1:");     Serial.print(boardA_temp1);     Serial.print(" ");
  Serial.print("BoardA_Moisture_2:"); Serial.print(boardA_moisture2); Serial.print(" ");
  Serial.print("BoardA_Temp_2:");     Serial.print(boardA_temp2);     Serial.print(" ");
  Serial.print("Light_Lux:");         Serial.print(lightLevelLux);    Serial.print(" ");
  Serial.print("Si7021_Temp:");       Serial.print(si7021_temp);      Serial.print(" ");
  Serial.print("Si7021_Humidity:");   Serial.print(si7021_humidity);  Serial.print(" ");
  Serial.print("BoardB_Moisture_1:"); Serial.print(boardB_moisture1); Serial.print(" ");
  Serial.print("BoardB_Temp_1:");     Serial.print(boardB_temp1);     Serial.print(" ");
  Serial.print("BoardB_Moisture_2:"); Serial.print(boardB_moisture2); Serial.print(" ");
  Serial.print("BoardB_Temp_2:");     Serial.println(boardB_temp2);

  delay(2000);
}

// Helper functions
float readSi7021Humidity() {
  I2CSi7021.beginTransmission(SI7021_ADDR);
  I2CSi7021.write(0xF5);
  if (I2CSi7021.endTransmission() != 0) return -1;
  delay(30);
  if (I2CSi7021.requestFrom(SI7021_ADDR, 2) != 2) return -1;
  uint16_t raw = I2CSi7021.read() << 8 | I2CSi7021.read();
  return ((125.0 * raw) / 65536.0) - 6.0;
}

float readSi7021Temperature() {
  I2CSi7021.beginTransmission(SI7021_ADDR);
  I2CSi7021.write(0xF3);
  if (I2CSi7021.endTransmission() != 0) return -273.15;
  delay(20);
  if (I2CSi7021.requestFrom(SI7021_ADDR, 2) != 2) return -273.15;
  uint16_t raw = I2CSi7021.read() << 8 | I2CSi7021.read();
  return ((175.72 * raw) / 65536.0) - 46.85;
}
