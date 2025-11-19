#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Fingerprint sensor connected to pins 2 (RX) and 3 (TX)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);

  mySerial.begin(57600);
  finger.begin(57600);

  Serial.println("Simple Fingerprint Attendance System");
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor connected!");
  } else {
    Serial.println("Sensor NOT found :(");
    while (1);
  }

  Serial.println("Commands:");
  Serial.println("E = Enroll Finger");
}

void loop() {

  // Check if user typed E in Serial Monitor
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'E' || c == 'e') {
      enrollFinger();
    }
  }

  // Try reading a finger
  int id = getFingerprintID();
  if (id > 0) {
    Serial.print("Attendance Marked: ");
    Serial.println(id);

    // Simple CSV output
    Serial.print(id);
    Serial.print(",");
    Serial.println("TIMESTAMP");   // You can replace this later with RTC timestamp

    delay(1500);  // debounce delay
  }
}

// -----------------------------
// ENROLLMENT FUNCTION
// -----------------------------
void enrollFinger() {
  int id;
  Serial.println("Enter ID number (1-127):");
  while (!Serial.available());
  id = Serial.parseInt();
  Serial.print("Enrolling ID #");
  Serial.println(id);

  if (id == 0) return;

  // Step 1
  Serial.println("Place finger...");
  while (finger.getImage() != FINGERPRINT_OK);
  finger.image2Tz(1);

  Serial.println("Remove finger...");
  delay(2000);

  // Step 2
  Serial.println("Place same finger again...");
  while (finger.getImage() != FINGERPRINT_OK);
  finger.image2Tz(2);

  if (finger.createModel() == FINGERPRINT_OK) {
    if (finger.storeModel(id) == FINGERPRINT_OK) {
      Serial.println("Enrolled successfully!");
    } else {
      Serial.println("Store failed");
    }
  } else {
    Serial.println("Enroll failed");
  }
}

// -----------------------------
// MATCH FINGERPRINT
// -----------------------------
int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;

  return finger.fingerID;   // return matched ID
}
