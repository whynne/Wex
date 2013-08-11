#pragma once

#include "begincode.h"

#define NO_SDL_GLEXT

#include <ctime>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include "SDL\SDL.h"
#include "graphics.h"
#include "physics.h"
#include "audio.h"
#include "timer.h"
#include "ui.h"
#include "controller.h"
#include "opengl.h"
#include "triglookup.h"

namespace wex
{
	class GameEngine;
	class EngineState;

	class GameEngine
	{
	public:
		int ups;
		int dps;
	
		Timer frametimer;
		Timer counter;
	
		double t;
		double dt;
	
		double accumulator;
		double frametime;   
		double newtime;   
		double eventtime;
		double currenttime;
	private:
	    static GameEngine* instance;
		bool running;
	    SDL_Event event;
	    Timer fps;
		static Controller controller;
		vector<EngineState*> states;
	    GameEngine();
        GameEngine&  operator=(GameEngine const&){return *this;};

	public:
		static GameEngine* Instance();
		void run();
		bool isRunning();
		bool init();
	    void pushState(EngineState* state);
		void popState();
		void handleEvents();
	    void update(double time,double delta);
	    void draw(double t,double dt);
		void resetCounters(){ups=0;dps=0;};
	};
	class EngineState
	{
	protected:
		int frames;
	public:
		GameEngine* engine;
		void setEngine(GameEngine* engine);
		virtual void begin() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual void end() = 0;
		virtual void handleEvents() = 0;
		virtual void update(double time,double deltatime) = 0;
		virtual void draw() = 0;
	};
}
	
#include "closecode.h"
	
	
	
	
	

