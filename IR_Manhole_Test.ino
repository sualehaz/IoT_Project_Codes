#define IR_PIN 35
float DISTANCE_THRESHOLD_CM = 30.0;  // Calibrate based on pole height/mounting angle

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); // onboard LED on most ESP32 dev boards
  delay(500);
  Serial.println("=====================================");
  Serial.println(" Sharp IR Test - Open Manhole Detector");
  Serial.println("=====================================");
}

float readDistanceCM() {
  int raw = analogRead(IR_PIN);
  float voltage = raw * (3.3 / 4095.0);

  if (voltage < 0.1) voltage = 0.1; // avoid divide-by-zero / bad readings

  // Approximation formula for GP2Y0A21YK0F. Swap constants if using
  // a different Sharp model.
  float distance = 27.86 * pow(voltage, -1.15);
  return distance;
}

void loop() {
  unsigned long startTime = micros();   // <-- start timing

  float distance = readDistanceCM();

  unsigned long endTime = micros();     // <-- end timing
  float processingTime_ms = (endTime - startTime) / 1000.0;

  // Build the actual output string so we can measure its byte size
  char buffer[100];
  int len = snprintf(buffer, sizeof(buffer),
    "Distance:%.2f", distance);

  // Print the sensor data
  Serial.print(buffer);
  Serial.print(" cm");

  // if (distance > DISTANCE_THRESHOLD_CM) {
  //   Serial.println("  ->  FLAG: Open manhole detected!");
  //   digitalWrite(2, HIGH); // simulate the pole's indicator LED turning on
  // } else {
  //   Serial.println("  ->  Manhole covered / normal");
  //   digitalWrite(2, LOW);
  // }
  Serial.println();

  // Print diagnostics: processing time and payload size
  Serial.print("ProcTime(ms):");
  Serial.print(processingTime_ms, 3);
  Serial.print(",SerialPayloadBytes:");
  Serial.println(len);

  delay(1000);
}
