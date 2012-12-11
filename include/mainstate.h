//This is a sort of scratch-pad state for testing things I'm learning

#pragma once
#include <ft2build.h> 
#include FT_FREETYPE_H
#include "enginestate.h"
#include "console.h"
#include "audio.h"
#include <string>
#include "vec.h"

class MainState : public EngineState
{
private:
	LSConsole               mainconsole; 
	graphics::ShaderProgram *regularshader;
public:
	Console testconsole;
	void init(Controller &maincontrol);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	MainState();
};
