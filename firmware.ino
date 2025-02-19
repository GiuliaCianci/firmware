// Pin di direzione per i motori
#define IN1 6  // Motore B - avanti
#define IN2 5  // Motore B - indietro
#define IN3 3  // Motore A - indietro
#define IN4 2  // Motore A - avanti

int speedLevel = 0; // Velocità iniziale (0-255) 255/2=127.5 ->127
char currentCommand = 's'; // Memorizza il comando attuale ('s' significa fermo)
bool firstCommandAfterStop = true; // Flag per il primo comando dopo lo stop

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();  // Legge il comando dalla seriale
    command = tolower(command); //converte in minuscolo
    if (command == '\n' || command=='\r') return;
    //Serial.println(command);       // Debug: stampa il comando ricevuto

    if (command == 'a') {  // Accelera
      speedLevel += 25;
      if (speedLevel >= 255) speedLevel = 255;
      //Serial.print("Velocità aumentata: ");
      //Serial.println(speedLevel);
      applyMovement(); // Applica la velocità al comando attuale
    } 
    else if (command == 'd') {  // Decelera
      speedLevel -= 25;
      if (speedLevel <= 0) speedLevel = 0;
      //Serial.print("Velocità ridotta: ");
      //Serial.println(speedLevel);
      applyMovement(); // Applica la velocità al comando attuale
    }
    else if (command == 'v') {  // Legge la velocità attuale
      delay(50); // Attendi 50 millisecondi
      Serial.println(speedLevel);
    }
    else if (command == 's') {
      currentCommand = 's';
      speedLevel = 0; // Imposta a 0 quando si preme stop
      firstCommandAfterStop = true; // Imposta il flag
      applyMovement();
    }
    else if (command == 'f' || command == 'b' || command == 'l' || command == 'r') {
      currentCommand = command;
      if (firstCommandAfterStop) {
        speedLevel = 127; // Imposta a 127 se è il primo comando dopo lo stop
        firstCommandAfterStop = false; // Resetta il flag
      }
      applyMovement();
    }
    else {  
      // Se è un comando di movimento, aggiorna il comando attuale
      currentCommand = command;
      applyMovement();  // Applica subito il comando
    }
  }
}

// Funzione per applicare il movimento in base al comando attuale e alla velocità
void applyMovement() {
      switch (currentCommand) {
        case 'f':  // Avanti
          moveMotors(speedLevel, 0, 0, speedLevel);
          //Serial.println("Motore avanti");
          break;
    
        case 'b':  // Indietro
          moveMotors(0, speedLevel, speedLevel, 0);
          //Serial.println("Motore indietro");
          break;
    
        case 'l':  // Sinistra
          moveMotors(0, speedLevel, 0, speedLevel);
          //Serial.println("Gira a sinistra");
          break;
    
        case 'r':  // Destra
          moveMotors(speedLevel, 0, speedLevel, 0);
          //Serial.println("Gira a destra");
          break;

        case 's':
          moveMotors(0, 0, 0, 0);
          speedLevel=0;
          break;
    
        default:
          Serial.println("Comando non riconosciuto");
          break;
    
  }
}

// Funzione per controllare i motori con la velocità attuale
void moveMotors(int val1, int val2, int val3, int val4) {
  analogWrite(IN1, val1);
  analogWrite(IN2, val2);
  analogWrite(IN3, val3);
  analogWrite(IN4, val4);
}
