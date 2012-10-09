//This is a sort of scratch-pad state for testing things I'm learning

#pragma once

#include "enginestate.h"
#include "console.h"
#include "vec.h"

class TestState : public EngineState
{
private:
	

public:
	Console testconsole;
	void init(Controller &maincontrol);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	TestState();
};
