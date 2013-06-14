#include "engine.h"

int main(int argc,char* argv[])
{
	wex::GameEngine engine;
        
	if(!engine.init())
	{
		cout << "Engine failed to initialize properly" << endl;
		return -1;
	}		    
	
	engine.run();
	
	SDL_Quit();
	return 0;
}