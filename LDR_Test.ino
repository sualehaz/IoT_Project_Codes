#define LDR_PIN 34           
int LIGHT_THRESHOLD = 1500;      

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  delay(500);
  Serial.println("=====================================");
  Serial.println(" LDR Test - Road Lighting Detector");
  Serial.println(" ESP32 ADC range: 0 (dark) - 4095 (bright)");
  Serial.println("=====================================");
}

void loop() {
  unsigned long startTime = micros();   // <-- start timing

  int ldrValue = analogRead(LDR_PIN);

  unsigned long endTime = micros();     // <-- end timing
  float processingTime_ms = (endTime - startTime) / 1000.0;

  // Build the actual output string so we can measure its byte size
  char buffer[100];
  int len = snprintf(buffer, sizeof(buffer),
    "LDR Raw Value:%d", ldrValue);

  Serial.print(buffer);

  if (ldrValue > LIGHT_THRESHOLD) {
    Serial.println("  ->  FLAG: Poor lighting detected!");
  } else {
    Serial.println("  ->  Lighting OK");
  }

  // Print diagnostics: processing time and payload size
  Serial.print("ProcTime(ms):");
  Serial.print(processingTime_ms, 3);
  Serial.print(",SerialPayloadBytes:");
  Serial.println(len);

  delay(1000); // read once per second
}