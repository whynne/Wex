#define NO_SDL_GLEXT

#include "engine.h"
#include <time.h>
#include "luainterface.h"
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
	engine.getLua()["dt"] = dt;    
	frametimer.start();                 
	while(engine.isRunning())
	{
		
		newtime = frametimer.getTimeInSeconds();
		frametime = newtime - currenttime;
		currenttime = newtime;

		if (frametime>0.25f)
			frametime = 0.25f;

		accumulator += frametime;
		eventtime += frametime;

		if(eventtime >= (1.0/30.0))
		{
			eventtime = 0.0;
			engine.handleEvents();
		}

		while (accumulator >= dt)
		{
		   engine.update(t,dt);
		   accumulator -= dt;            
		   t+=dt;                        
		}
		engine.draw(t,dt);
	}
	
	SDL_Quit();
	return 0;
}