// LDR-Sensoren
const int ldrPinX = A0;  // LDR-Sensor für X-Achse
const int ldrPinY = A1;  // LDR-Sensor für Y-Achse

// Schwellenwerte Sonnenposition
const int xThreshold = 512;  // X-Achse Schwellenwert
const int yThreshold = 512;  // Y-Achse Schwellenwert

//Relais
const int relayPin1 = 2;  // Relais 1 Steuerpin (X-Achse vorwärts)
const int relayPin2 = 3;  // Relais 2 Steuerpin (X-Achse rückwärts)
const int relayPin3 = 4;  // Relais 3 Steuerpin (Y-Achse vorwärts)
const int relayPin4 = 5;  // Relais 4 Steuerpin (Y-Achse rückwärts)

// Ausgangsposition
const int initialPositionX = 90;  // Ausgangsposition X-Achse
const int initialPositionY = 90;  // Ausgangsposition Y-Achse

// Aktuelle Position der Linearmotoren
int currentPositionX = initialPositionX;
int currentPositionY = initialPositionY;

// Ansteuern der Linearmotoren
void moveLinearMotor(int relayPinForward, int relayPinBackward) {
  digitalWrite(relayPinForward, HIGH);
  digitalWrite(relayPinBackward, LOW);
  delay(2000);  // Motor für 1 Sekunde einschalten
  digitalWrite(relayPinForward, LOW);
  digitalWrite(relayPinBackward, LOW);
}

void setup() {
  pinMode(ldrPinX, INPUT);
  pinMode(ldrPinY, INPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  digitalWrite(relayPin3, LOW);
  digitalWrite(relayPin4, LOW);
}

void loop() {
  // Werte LDR-Sensoren
  int ldrValueX = analogRead(ldrPinX);
  int ldrValueY = analogRead(ldrPinY);

  // Sonnenposition in der X-Achse
  if (ldrValueX > xThreshold) {
    if (currentPositionX < 180) {
      moveLinearMotor(relayPin1, relayPin2); // X-Achse vorwärts
      currentPositionX++;
    }
  } 
  else if (ldrValueX < xThreshold) {
    if (currentPositionX > 0) {
      moveLinearMotor(relayPin2, relayPin1); // X-Achse rückwärts
      currentPositionX--;
    }
  }

  // Sonnenposition in der Y-Achse
  if (ldrValueY > yThreshold) {
    if (currentPositionY < 180) {
      moveLinearMotor(relayPin3, relayPin4); // Y-Achse vorwärts
      currentPositionY++;
    }
  } 
  else if (ldrValueY < yThreshold) {
    if (currentPositionY > 0) {
      moveLinearMotor(relayPin4, relayPin3); // Y-Achse rückwärts
      currentPositionY--;
    }
  }

  // Begrenze die Positionswerte
  currentPositionX = constrain(currentPositionX, 0, 180);
  currentPositionY = constrain(currentPositionY, 0, 180);

  // Überprüfe ob die Sonne nicht scheint und fahr dann in die Ausgangsposition
  if (ldrValueX < xThreshold && ldrValueY < yThreshold) {
    // Fahre Linearmotoren in die Ausgangsposition
    while (currentPositionX > initialPositionX) {
      moveLinearMotor(relayPin2, relayPin1); // X-Achse rückwärts
      currentPositionX--;
    }

    while (currentPositionX < initialPositionX) {
      moveLinearMotor(relayPin1, relayPin2); // X-Achse vorwärts
      currentPositionX++;
    }

    while (currentPositionY > initialPositionY) {
      moveLinearMotor(relayPin4, relayPin3); // Y-Achse rückwärts
      currentPositionY--;
    }

    while (currentPositionY < initialPositionY) {
      moveLinearMotor(relayPin3, relayPin4); // Y-Achse vorwärts
      currentPositionY++;
    }
  }

  delay(600000); // Warte 10 Minuten und beginnt dann wieder die Ausrichtung
}