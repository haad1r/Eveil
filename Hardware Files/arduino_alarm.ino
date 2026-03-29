const int FSR_PINS[4] = {A0, A1, A2, A3};
const int BUZZER_PIN  = 8;
int THRESHOLD   = 650;

bool alarmActive = false;
bool forceAlarmActive = false; // NEW: Keeps track if we are forcing the alarm

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  Serial.println("Arduino ready. Waiting for ESP32 command...");
}

void loop() {
  // Check for commands from ESP32
  if (Serial1.available()) {
    String msg = Serial1.readStringUntil('\n');
    msg.trim();
    
    if (msg == "ALARM_START") {
      alarmActive = true;
      forceAlarmActive = false; // Reset force flag
      Serial.println(">>> ALARM_START received. Sensors are LIVE!");
    } 
    else if (msg == "ALARM_STOP") {
      alarmActive = false;
      forceAlarmActive = false; // Reset force flag
      noTone(BUZZER_PIN);
      Serial.println(">>> ALARM_STOP received. System deactivated.");
    }
    else if (msg.startsWith("THRESHOLD:")) {
      String val = msg.substring(10); // Strip "THRESHOLD:"
      THRESHOLD = val.toInt();
      Serial.println("Threshold updated to: " + String(THRESHOLD));
    }
    else if (msg == "TEST_ALARM") {
      Serial.println(">>> TEST_ALARM received. Testing buzzer...");
      tone(BUZZER_PIN, 1000);  // Turn buzzer ON
      delay(2000);             // Hold for 2 seconds
      
      // Only turn off if we aren't currently forcing the alarm
      if (!forceAlarmActive) {
        noTone(BUZZER_PIN);      
      }
      Serial.println(">>> Test complete.");
    }
    else if (msg == "MANUAL_NOISE") {
      Serial.println(">>> FORCE ALARM received. Ignoring FSRs, buzzing indefinitely!");
      alarmActive = false;       // Disable normal sensor checking
      forceAlarmActive = true;   // Lock into forced state
      tone(BUZZER_PIN, 1000);    // Turn buzzer ON permanently
    }
    else if (msg == "MANUAL_STOP") {
      Serial.println(">>> ALARM KILL received. Stopping everything.");
      alarmActive = false;       // Disable normal mode
      forceAlarmActive = false;  // Disable forced mode
      noTone(BUZZER_PIN);        // Silence the buzzer
    }
  }

  // Read Sensors (Only if Armed AND NOT Forced)
  // By requiring !forceAlarmActive, we prevent the "tug-of-war" bug!
  if (alarmActive && !forceAlarmActive) {
    int readings[4];
    bool anySensorActive = false;

    for (int i = 0; i < 4; i++) {
      readings[i] = analogRead(FSR_PINS[i]);
      if (readings[i] >= THRESHOLD) {
        anySensorActive = true;
      }
    }

    //  Send all 4 FSR readings to ESP32 
    String dataMsg = "FSR:" + String(readings[0]) + "," +
                              String(readings[1]) + "," +
                              String(readings[2]) + "," +
                              String(readings[3]);
    Serial1.println(dataMsg);
    Serial.println("Sent: " + dataMsg);

    // Trigger Buzzer 
    if (anySensorActive) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
    }
  }

  delay(500);
}
