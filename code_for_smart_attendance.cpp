#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

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

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'E' || c == 'e') {
      enrollFinger();
    }
  }

  int id = getFingerprintID();
  if (id > 0) {
    Serial.print("Attendance Marked: ");
    Serial.println(id);

    Serial.print(id);
    Serial.print(",");
    Serial.println("TIMESTAMP");   
    delay(1500); 
}

void enrollFinger() {
  int id;
  Serial.println("Enter ID number (1-127):");
  while (!Serial.available());
  id = Serial.parseInt();
  Serial.print("Enrolling ID #");
  Serial.println(id);

  if (id == 0) return;

  Serial.println("Place finger...");
  while (finger.getImage() != FINGERPRINT_OK);
  finger.image2Tz(1);

  Serial.println("Remove finger...");
  delay(2000);

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

int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;

  return finger.fingerID;  
}
