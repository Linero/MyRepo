#pragma once
#include <Arduino.h>

class Engine
{
private:
	int delaylegnth;
	int steps;
	int dir;
	int start;
	int mode;
	const byte buffSize = 45;
	char inputBuffer[45];
	const char startMarker = '<';
	const char endMarker = '>';
	byte bytesRecvd = 0;
	boolean readInProgress = false;

public:
	Engine();
	~Engine();
	void Run();



private:
	void moveSolver(int);
	void stepperMove();
	void getDataFromPC();
	void parseData();
};

