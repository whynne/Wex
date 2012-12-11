#define DEVBUILD
#define NO_SDL_GLEXT
#include "enginestate.h"
#include "MainState.h"
#include <time.h>
#include "engine.h"
#include "AL/al.h"
#include "AL/alut.h"
#include "graphics.h"

int main( int argc, char* args[] )
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
	engine.init();           
			 

	EngineState* EngineState = new MainState;
	EngineState->init(engine.getUserController());  
	engine.pushState(EngineState);                                                    
	frametimer.start();                           

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
			engine.handleEvents(&engine);
		}



		while (accumulator >= dt)
		{
		   engine.update(&engine,t,dt);
		   accumulator -= dt;            
		   t+=dt;                        
		}

		engine.draw(&engine);
	}

	SDL_Quit();

	cout << "GAME OVER" << endl;

	return 0;
}

