/*
 Name:		stepper.ino
 Created:	02.12.2018 14:15:48
 Author:	Mateusz
*/
#include "motorShieldL298P.h"

Engine engine;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);

}

// the loop function runs over and over again until power down or reset
void loop() {

	engine.Run();

}
