#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// GNSS module serial connections (adjust pins as needed)
SoftwareSerial gpsSerial(4, 3); // RX, TX
TinyGPSPlus gps;

// State machine enums
enum FlightState {
  IDLE,
  ASCENT,
  APOGEE,
  DESCENT,
  PAYLOAD_DEPLOYED,
  LANDED
};

FlightState currentState = IDLE;
double prevAltitude = 0, maxAltitude = 0;
bool payloadDeployed = false;

void sendGNSSConfig() {
  // Disable all default sentences
  gpsSerial.println("$PQTXT,W,DISABLE*3F");
  delay(100);

  // Enable only GNRMC and GNGGA
  gpsSerial.println("$PQTXT,E,GNRMC*3B");
  delay(100);
  gpsSerial.println("$PQTXT,E,GNGGA*34");
  delay(100);
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  Serial.println(F("Quectel L89HA GNSS Setup for CanSat..."));
  delay(2000);

  // Configure GNSS module output
  sendGNSSConfig();

  Serial.println(F("GNSS configured: Only GNRMC & GNGGA enabled"));
  Serial.println(F("Waiting for GPS fix..."));
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid() && gps.altitude.isValid() && gps.time.isValid()) {
    double altitude = gps.altitude.meters();

    // ---- Clean Data Print ----
    Serial.print("[UTC ");
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print("]  ");

    Serial.print("Lat: "); Serial.print(gps.location.lat(), 6); Serial.print("  ");
    Serial.print("Lon: "); Serial.print(gps.location.lng(), 6); Serial.print("  ");
    Serial.print("Alt: "); Serial.print(altitude, 2); Serial.print(" m  ");

    // ---- State Machine ----
    switch (currentState) {
      case IDLE:
        if (altitude > 2.0) {
          currentState = ASCENT;
          maxAltitude = altitude;
          Serial.print("State: ASCENT");
        } else {
          Serial.print("State: IDLE");
        }
        break;

      case ASCENT:
        if (altitude > maxAltitude) {
          maxAltitude = altitude;
        }
        if (altitude < maxAltitude - 1.0) {
          currentState = APOGEE;
          Serial.print("State: APOGEE");
        } else {
          Serial.print("State: ASCENT");
        }
        break;

      case APOGEE:
        if (altitude < maxAltitude - 1.0) {
          currentState = DESCENT;
          Serial.print("State: DESCENT");
        } else {
          Serial.print("State: APOGEE");
        }
        break;

      case DESCENT:
        if (!payloadDeployed && altitude < (0.75 * maxAltitude)) {
          currentState = PAYLOAD_DEPLOYED;
          payloadDeployed = true;
          Serial.print("State: PAYLOAD DEPLOYED");
        } else if (altitude <= 2.0) {
          currentState = LANDED;
          Serial.print("State: LANDED");
        } else {
          Serial.print("State: DESCENT");
        }
        break;

      case PAYLOAD_DEPLOYED:
        if (altitude <= 2.0) {
          currentState = LANDED;
          Serial.print("State: LANDED");
        } else {
          Serial.print("State: PAYLOAD DEPLOYED");
        }
        break;

      case LANDED:
        Serial.print("State: LANDED");
        break;
    }

    Serial.println();
    prevAltitude = altitude;
  } else {
    Serial.println("Waiting for valid GPS fix...");
  }

  delay(400); // Update rate
}
