#include <Stepper.h>              //brukes med trinnmotoren og driverkort
const int stepsPerRevolution = 64;  // number of steps per revolution, brukes med trinnmotoren og driverkort
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); //brukes for driverkort

// Hva det er og hvilken pin den bruker, bruker dette for å teste
int LED4 = 4;
int knapp3 = 3;
int knapp2 = 2;

// ettSteg er hvor mange steps*64 som trengs for å flytte kammen fra tann til tann
// halvtSteg er da 11*64 steps for å flytte en halv tann på kammen
// disse variablene brukes bare i deres funksjoner
int ettSteg = 22;
int halvtSteg = 11;

int period = 1000;          //tidsinterval for hvor lenge den skal vente på å registrere knappe trykk. brukes med millis()
unsigned long time_now = 0; //blir brukt med millis()

int holdeInneTeller = 0; //Denne blir å enten være 0 eller langt over 0
int knappTeller = 0; // Denne teller hvor mange ganger holdeInneTeller går over 0

// tall = 16 er er halv rotasjon hos motorer
// tall = 8 er en kvart
void counterclockwise(int tall) {
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution*tall);
}

// tall = 16 er er halv rotasjon hos motoren
// tall = 8 er en kvart
void clockwise(int tall) {
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution*tall);
}

// Kjører ett hakk på kammen. retning velger rotasjons retning
void etSteg (String retning) {
  Serial.println("Kjører ett steg på kammen");
  if (retning == "+") {
    clockwise(ettSteg);
  }
  else if (retning == "-") {
    counterclockwise(ettSteg);
  }
}

// Kjører halvt hakk på kammen. retning velger rotasjons retning
void halvSteg (String retning) {
  Serial.println("Kjører ett steg på kammen");
  if (retning == "+") {
    clockwise(halvtSteg);
  }
  else if (retning == "-") {
    counterclockwise(halvtSteg);
  }
}


void setup() {
  pinMode(LED4, OUTPUT);
  pinMode(knapp3,INPUT_PULLUP);
  pinMode(knapp2,INPUT_PULLUP);


  myStepper.setSpeed(400); // hastighet på motoren
  Serial.begin(9600);
}



void loop() {

  // når knappen blir trykket inn så vil den "+=" for mange ganger i sekundet
  if (digitalRead(knapp3) == LOW) {
    holdeInneTeller += 1;
  }

  // når knappen blir sluppet, vil denne telle antall trykk
  // den vil også sette variablene slik at millis() if-setningen under vil fungere
  else if (holdeInneTeller > 0) {
    knappTeller += 1;
    holdeInneTeller = 0;
    time_now = millis();
    
    //digitalWrite(LED4,LOW);
  }

  /* Lar dette ligge slik at jeg kan teste
  else if (knappTeller == 1) {
    holdeInneTeller = 0;
    //Serial.println(knappTeller);
  }
  
  else if (knappTeller == 2) {
    //etSteg("-");
    holdeInneTeller = 0;
    //Serial.println(knappTeller);
  }
  else if (knappTeller == 3){    
  }
  */
    
  //
  Serial.println(millis() - time_now);
  if (knappTeller > 0 && millis() - time_now > period) {
    
    //digitalWrite(LED4,HIGH);
    
    // Blar en side frem hvis knappteller = 1
    if (knappTeller == 1) {
      etSteg("+");
      //halvSteg("+");
      //halvSteg("-");
    }

    // Blar en side tilbake hvis knappteller = 2
    else if (knappTeller == 2) {
      etSteg("-");
      //halvSteg("-");
      //halvSteg("+");
    }
    
    knappTeller = 0;
      
  }
  

}
