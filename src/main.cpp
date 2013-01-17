#define NO_SDL_GLEXT
#include "enginestate.h"
#include "engine.h"

#include "mainstate.h"
#include <time.h>

#include "AL/al.h"
#include "AL/alut.h"
#include "graphics.h"

int main(int argc,char* argv[])
{
	
	Timer frametimer;

	double t = 0.0;
	const double dt = .01;

	double accumulator = 0.0; 
	double frametime = 0.0;   
	double newtime = 0.0;    
	double eventtime = 0.0;
	double currenttime = 0.0;
	
	GameEngine engine;       
    srand(time(NULL));       
	if(!engine.init())
	{
		cout << "Engine failed to initialize properly" << endl;
		return -1;
	}		 
		
	cout << "Creating state" << endl;
	EngineState* EngineState = new MainGameState(engine);
	cout << "Pushing state" << endl;
	engine.pushState(EngineState);                                                    
	frametimer.start();                           

	cout << "Initializing state" << endl;
	engine.initState();

	cout << "Running engine" << endl;
	while(engine.isRunning())
	{
		
		newtime = frametimer.getTimeInSeconds();
		frametime = newtime - currenttime;
		currenttime = newtime;

		if (frametime>0.25f)
			frametime = 0.25f;

		accumulator += frametime;
		
		if(eventtime >= (1/30))
		{
			engine.handleEvents();
		}

		

		while (accumulator >= dt)
		{
		   engine.updateState(t,dt);
		   accumulator -= dt;            
		   t+=dt;                        
		}
		
		
		engine.drawState(t,dt);
		
	}

	SDL_Quit();

	cout << "GAME OVER" << endl;

	

	

	return 0;
}