#include "motorShieldL298P.h"


Engine::Engine()
{

	pinMode(12, OUTPUT);  //CH A -- HIGH = forwards and LOW = backwards
	pinMode(13, OUTPUT); //CH B -- HIGH = forwards and LOW = backwards

	//establish motor brake pins
	pinMode(9, OUTPUT);  //brake (disable) CH A
	pinMode(8, OUTPUT); //brake (disable) CH B

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
	stepperMove();

}


void Engine::stepperMove() {

	if (this->start == 1) {

		if (this->mode == 1) {

			if (this->steps > 0) {
				moveSolver(this->dir);
				this->steps--;
			}
		}
		else moveSolver(this->dir);

	}
}

void Engine::getDataFromPC() {

	while (Serial.available() > 0) {

		char x = Serial.read();

		// the order of these IF clauses is significant

		if (x == endMarker) {
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
	}
}

// This function split data into its parts
void Engine::parseData() {

	char * strtokIndx; // this is used by strtok() as an index

	strtokIndx = strtok(inputBuffer, ",");
	this->steps = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->dir = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->start = atoi(strtokIndx);

	strtokIndx = strtok(NULL, ",");
	this->mode = atoi(strtokIndx);
}