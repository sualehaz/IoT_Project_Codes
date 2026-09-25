#include <Wire.h>
const int MPU_ADDR = 0x68; // default I2C address

void setup() {
  Serial.begin(115200);
  Wire.begin(); // init I2C (SDA=21, SCL=22)

  // Wake MPU6050: write 0x00 to PWR_MGMT_1 (0x6B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);   // PWR_MGMT_1 register
  Wire.write(0x00);   // clear sleep bit
  Wire.endTransmission(true);

  Serial.println("MPU6050 Initialized");
}

void loop() {
  unsigned long startTime = micros();   // <-- ADDED: start timing here

  // Point internal register pointer to ACCEL_XOUT_H (0x3B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false); // keep bus active for read

  // Burst-read 14 bytes: AccX(2) AccY(2) AccZ(2) Temp(2) GyrX(2) GyrY(2) GyrZ(2)
  uint8_t payloadBytes = Wire.requestFrom(MPU_ADDR, 14);  // <-- ADDED: capture bytes received

  int16_t AccX_raw = Wire.read() << 8 | Wire.read();
  int16_t AccY_raw = Wire.read() << 8 | Wire.read();
  int16_t AccZ_raw = Wire.read() << 8 | Wire.read();
  Wire.read(); Wire.read(); // skip temperature (not used here)
  int16_t GyrX_raw = Wire.read() << 8 | Wire.read();
  int16_t GyrY_raw = Wire.read() << 8 | Wire.read();
  int16_t GyrZ_raw = Wire.read() << 8 | Wire.read();

  // Convert to physical units (default full-scale: +/-2g, +/-250 deg/s)
  float AccX = AccX_raw / 16384.0;
  float AccY = AccY_raw / 16384.0;
  float AccZ = AccZ_raw / 16384.0;
  float GyrX = GyrX_raw / 131.0;
  float GyrY = GyrY_raw / 131.0;
  float GyrZ = GyrZ_raw / 131.0;

  unsigned long endTime = micros();          // end timing
  float processingTime_ms = (endTime - startTime) / 1000.0;

  // Build the actual output string so we can measure its byte size
  char buffer[160];
  int len = snprintf(buffer, sizeof(buffer),
    "AccX:%.3f,AccY:%.3f,AccZ:%.3f,GyrX:%.3f,GyrY:%.3f,GyrZ:%.3f",
    AccX, AccY, AccZ, GyrX, GyrY, GyrZ);

  // Print the sensor data
  Serial.println(buffer);

  // Print diagnostics: processing time and payload sizes
  Serial.print("ProcTime(ms):");
  Serial.print(processingTime_ms, 3);
  Serial.print(",I2C_PayloadBytes:");
  Serial.print(payloadBytes);
  Serial.print(",SerialPayloadBytes:");
  Serial.println(len);

  delay(100);
}