#include <Stepper.h>			// bibliotek for trinnmotoren og driverkort
const int trinnPerRotasjon = 64;	// antall ”trinn” per motor-rotasjon, brukes med trinnmotoren og driverkort
Stepper kamMotor(trinnPerRotasjon, 8, 10, 9, 11);  // pin-nr for motoren, brukes for driverkort

// pin-nr for pedal-knappen
int pedalKnapp = 3;

// ettSteg er hvor mange motor-rotasjoner som trengs for å flytte kammen fra ”tann til tann”
// halvtSteg er halvparten antall rotasjoner
int ettSteg = 22;
int halvtSteg = 11;

int periode = 1000;	// Variabelen periode er reaksjonstiden til pedalen. Dermed bestemmer den også hvor raskt et dobbelttrykk må være. Brukes med millis()
unsigned long tidTeller = 0;	// Blir brukt med millis()

int holdeInneTeller = 0; 	// Denne blir å enten være 0 eller langt over 0
int antallTrykkTeller = 0; 	// Denne teller hvor mange ganger holdeInneTeller går over 0

// tall er enten ettSteg eller halvtSteg
void counterclockwise(int tall) {
	kamMotor.step(trinnPerRotasjon*tall);
}

void clockwise(int tall) {
	kamMotor.step(-trinnPerRotasjon*tall);
}

// Kjører ett hakk på kammen. retning bestemmer rotasjonsretning
void helSteg(String retning) {
	if (retning == ”+”) {
		clockwise(ettSteg);
	}
	else if (retning == ”-”) {
		counterclockwise(ettSteg);
	}
}

// Kjører et halvt hakk på kammen. retning bestemmer rotasjonsretning
void halvSteg (String retning) {
	if (retning == ”+”) {
		clockwise(halvtSteg);
	}
	else if (retning == ”-”) {
		counterclockwise(halvtSteg);
	}
}



// Blar et ark. retning bestemmer rotasjonsretning
void blaArk(String retning) {
	if (retning == ”+”) {
		helSteg(”+”);
		halvSteg(”+”);
		halvSteg(”-”);;
	}
	else if (retning == ”-”) {
		helSteg(”-”);
		halvSteg(”-”);
		halvSteg(”+”);
	}
}

void setup() {
	pinMode(pedalKnapp, INPUT_PULLUP);
	kamMotor.setSpeed(400); // hastighet på motoren
	Serial.begin(9600);
}

void loop() {
// holdeInneTeller sin verdi stiger så lenge knappen(pedalen) holdes inne. Programmet er skrevet slik at et langt trykk oppfattes som kun ett trykk
	if (digitalRead(pedalKnapp) == LOW) {
		holdeInneTeller += 1;
	}

// når knappen blir sluppet teller antallTrykkTeller antall trykk. Funksjonen setter også tidTeller for millis()-betingelsen i neste if-setning
	else if (holdeInneTeller > 0) {
		antallTrykkTeller += 1;
		holdeInneTeller = 0;
		tidTeller = millis();
	}
	
	if (antallTrykkTeller > 0 && (millis() - tidTeller) > periode) {

		// Blar en side frem hvis antallTrykkTeller = 1
		if (antallTrykkTeller == 1) {	
			blaArk(”+”);
		}

		// Blar bakover hvis antallTrykkTeller = 2
		else if (antallTrykkTeller == 2) { 
			blaArk(”-”);
		}

		antallTrykkTeller = 0; // nullstiller
	}
}
