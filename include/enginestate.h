#pragma once
#include "graphics.h"
#include "timer.h"
#include "controller.h"
#include <math.h>
#include <string>
#include <sstream>

/*
Basic state class for the input/update/draw loop
*/

class EngineState
{
protected:
	int frames;
	Controller* _maincontrol;
public:
	virtual void init(Controller &maincontrol);
	virtual void cleanup() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void handleEvents() = 0;
	virtual void update(double time,double deltatime) = 0;
	virtual void draw() = 0;
};