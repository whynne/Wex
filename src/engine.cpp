#include "engine.h"


Controller GameEngine::controller;
    
GameEngine::GameEngine()
{
}

bool GameEngine::init()
{
   AllocConsole();
   freopen( "CON", "w", stdout ); 



    //Game initializations
    srand(time(NULL));
    running = true;	
	TrigLookUp::calculateTrigValues();


    //SDL/OpenGL initializations
	if(!graphics::Init()){
		return false;
	}
	cout << "Graphics successfully initialized!" << endl;
	//OpenAL init

	//audio::init();
   									    // Enable unicode for translating keypresses into printable characters

    return true;
}

void GameEngine::handleEvents()
{
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN)
        {
            controller.pressKey(event.key.keysym.sym);

            if(controller.isTextCaptureMode())
            {
                controller.insertCharacter(event.key.keysym);
            }
        }
        else if (event.type == SDL_KEYUP)
            controller.releaseKey(event.key.keysym.sym);	
        else if ( event.type == SDL_MOUSEMOTION ) 
        {
            
        }
    }
    statestack.back()->handleEvents();
    controller.update();
}



void GameEngine::initState()
{
	statestack.front()->initState();
}

void GameEngine::drawState(double t,double dt)
{
  glClearDepth(1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if(!statestack.empty())
    statestack.front()->drawState(t,dt);
  else
    return;
  SDL_GL_SwapBuffers();
}

void GameEngine::updateState(double t,double dt)
{
    statestack.back()->updateState(t,dt);
}

void GameEngine::changeState(EngineState *state)
{
    if(statestack.back())
    {
        statestack.pop_back();
    }

}

void GameEngine::pushState(EngineState *state)
{
    statestack.push_back(state);
}

void GameEngine::popState(EngineState *state)
{
}

bool GameEngine::isRunning()
{
    return running;
}

Controller& GameEngine::getController()
{
	return controller;
}
