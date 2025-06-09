// Board B: Sends 2 SMT50 sensor readings over Serial2 TX 

const int moisturePins[2] = {14, 16};     
const int temperaturePins[2] = {15, 17};   // Adjust as needed

#define TX_PIN 21  

void setup() {
  Serial.begin(115200);  // USB serial
  Serial2.begin(9600, SERIAL_8N1, -1, TX_PIN);  // TX only

  delay(1000);
  Serial.println("Board B: Started");

  analogReadResolution(12);
  for (int i = 0; i < 2; i++) {
    analogSetPinAttenuation(moisturePins[i], ADC_11db);
    analogSetPinAttenuation(temperaturePins[i], ADC_11db);
  }
}

void loop() {
  for (int i = 0; i < 2; i++) {
    int rawMoisture = analogRead(moisturePins[i]);
    int rawTemp = analogRead(temperaturePins[i]);

    float vMoisture = (rawMoisture / 4095.0) * 3.3;
    float vTemp = (rawTemp / 4095.0) * 3.3;

    float moisturePercent = (vMoisture * 0.050 / 3.0) * 100;
    float tempC = (vTemp - 0.5) * 100;

    Serial2.print("SENSOR_B:");
    Serial2.print(i + 1);
    Serial2.print("=");
    Serial2.print(tempC, 1);
    Serial2.print("C/");
    Serial2.print(moisturePercent, 1);
    Serial2.println("%");
  }

  delay(10000);  // Send every 10 seconds
}
