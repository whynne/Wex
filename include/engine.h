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

	class WEXAPI GameEngine
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
	
		bool running;
	    SDL_Event event;
	    Timer fps;
	    
		static Controller controller;
		static audio::Source source;
		static audio::AudioBuffer buffer;

		vector<EngineState*> states;
	
	public:
		GameEngine();
	
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
		GameEngine& engine;
		virtual void init(GameEngine& engine){this->engine = engine;};
		virtual void cleanup() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;
		virtual void handleEvents() = 0;
		virtual void update(double time,double deltatime) = 0;
		virtual void draw() = 0;
	};
}
	
#include "closecode.h"
	
	
	
	
	

