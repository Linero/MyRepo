#include "motorShieldL298P.h"


Engine::Engine()
{

	pinMode(12, OUTPUT); //Initiates Motor Channel A pin
	pinMode(9, OUTPUT); //Initiates Brake Channel A pin
	start = 0;
	demo = 0;
	pwm = 0;
	dir = 0;

}


Engine::~Engine()
{
}


void Engine::moveSolver(int dir) {

	digitalWrite(9, LOW);   //ENABLE CH A
	digitalWrite(8, HIGH); //DISABLE CH B

	digitalWrite(12, HIGH); //Sets direction of CH A
	analogWrite(3, 255);   //Moves CH A

	delay(delaylegnth);

	digitalWrite(9, HIGH); //DISABLE CH A
	digitalWrite(8, LOW); //ENABLE CH B

	if (dir == 1) {
		digitalWrite(13, LOW);   //Sets direction of CH B
		analogWrite(11, 255);   //Moves CH B
	}
	else {
		digitalWrite(13, HIGH);  //Sets direction of CH B
		analogWrite(11, 255);   //Moves CH B
	}

	delay(delaylegnth);
}

void Engine::Run() {

	getDataFromPC();
	motorMove();

}


void Engine::motorMove() {

	
	if (start == 0) {
		digitalWrite(9, HIGH);
	}
	else{
		digitalWrite(9, LOW);
		analogWrite(3, pwm);
	}
	if (dir == 0) {
		digitalWrite(12, HIGH);
	}
	else {
		digitalWrite(12, LOW);
	}
	if (demo == 1) {
		Demo();
	}



}

void Engine::getDataFromPC() {

	while (Serial.available() > 0) {

		char x = Serial.read();

		// the order of these IF clauses is significant

		if (x == this->endMarker) {
			this->readInProgress = false;
			this->inputBuffer[this->bytesRecvd] = 0;
			parseData();
		}

		if (this->readInProgress) {
			this->inputBuffer[this->bytesRecvd] = x;
			this->bytesRecvd++;
			if (this->bytesRecvd == this->buffSize) {
				this->bytesRecvd = this->buffSize - 1;
			}
		}

		if (x == this->startMarker) {
			this->bytesRecvd = 0;
			this->readInProgress = true;
		}

		/*if (x == cmdMarker) {
			DataSync();
		}*/
	}
}

// This function split data into its parts
void Engine::parseData() {

	char * strtokIndx; // this is used by strtok() as an index

	strtokIndx = strtok(inputBuffer, ",");
	this->start = atoi(strtokIndx);
	
	strtokIndx = strtok(NULL, ",");
	this->pwm = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->dir = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->demo = atoi(strtokIndx);
}

void Engine::Demo() {

		//forward @ full speed
		digitalWrite(12, HIGH); //Establishes forward direction of Channel A
		digitalWrite(9, LOW);   //Disengage the Brake for Channel A
		analogWrite(3, 255);   //Spins the motor on Channel A at full speed

		delay(3000);

		digitalWrite(9, HIGH); //Eengage the Brake for Channel A

		delay(1000);

		//backward @ half speed
		digitalWrite(12, LOW); //Establishes backward direction of Channel A
		digitalWrite(9, LOW);   //Disengage the Brake for Channel A
		analogWrite(3, 123);   //Spins the motor on Channel A at half speed

		delay(3000);

		digitalWrite(9, HIGH); //Eengage the Brake for Channel A

		delay(1000);

	
}

/*void Engine::DataSync() {

	while (Serial.available()) {
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:

		if (inChar == '\n') {
			this->commandString = inputString.substring(1, inputString.length() - 2);
			stringComplete = true;
		}

		if (stringComplete == true) {
			if (commandString.equals("DEMO")) {

				this->demo = 1;
			}


			stringComplete = false;
		}




	}
}*/