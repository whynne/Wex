#pragma once

#include "SDL\SDL.h"
#include "EngineState.h"
#include "graphics.h"
#include "timer.h"
#include "controller.h"
#include <ctime>
#include <vector>
#include "opengl.h"


using namespace std;

class GameEngine
{
private:
	bool running;
	SDL_Event event;
    Timer fps;
	vector<EngineState*> statestack;
	Controller _maincontrol;

public:
	GameEngine();
	bool isRunning();
	bool init();

	void changeState(EngineState *state);
	void pushState(EngineState *state);
	void popState(EngineState *state);
	void handleEvents(GameEngine *engine);
	void update(GameEngine *engine,double time,double delta);
	void draw(GameEngine *engine);

	Controller& getUserController();
};
