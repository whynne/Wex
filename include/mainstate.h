//This is a sort of scratch-pad state for testing things I'm learning

#pragma once
#include <ft2build.h> 
#include FT_FREETYPE_H
#include "enginestate.h"
#include "ui.h"
#include "audio.h"
#include <string>
#include "vec.h"


class MainMenuState : public EngineState
{
private:
	TextureFont font;
public:
	Console testconsole;
	void init(Controller &maincontrol);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	MainMenuState();
};



class MainGameState : public EngineState
{
private:
	LSConsole console;

public:
	Console testconsole;
	void init(Controller &maincontrol);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	MainGameState();
};


class BattleState : public EngineState
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

	BattleState();
};
