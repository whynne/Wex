#pragma once

#include <ctime>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include "enginestate.h"
#include "SDL\SDL.h"
#include "graphics.h"
#include "audio.h"
#include "timer.h"
#include "controller.h"
#include "opengl.h"
#include "triglookup.h"

class EngineState;
class GameEngine;

class GameEngine
{
private:

	bool running;
    SDL_Event event;
    Timer fps;
	vector<EngineState*> statestack;
    static Controller controller;

public:
	
	static Controller& getController();
	GameEngine();
	bool isRunning();
	bool init();
	
    void changeState(EngineState *state);
    void pushState(EngineState *state);
    void popState(EngineState *state);
    
	void handleEvents();
	void initState();
    void updateState(double time,double delta);
    void drawState(double t,double dt);

};

class EngineState
{
protected:
	int frames;
	GameEngine* engine;
public:
	
	virtual void initState() = 0;
	virtual void cleanup() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void handleEvents() = 0;
	virtual void updateState(double time,double deltatime) = 0;
	virtual void drawState(double t,double dt) = 0;
	
};