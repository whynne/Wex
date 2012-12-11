#include "engine.h"

using namespace std;

    
GameEngine::GameEngine()
{
}

bool GameEngine::init()
{
   // AllocConsole();
   // freopen( "CON", "w", stdout ); 



    //Game initializations
    srand(time(NULL));
    running = true;	
	TrigLookUp::calculateTrigValues();


    //SDL/OpenGL initializations
	if(!graphics::Init()){
		return false;
	}
	//OpenAL init

	audio::init();
   									    // Enable unicode for translating keypresses into printable characters

    return true;
}

void GameEngine::handleEvents(GameEngine *engine)
{
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN)
        {
            _maincontrol.pressKey(event.key.keysym.sym);

            if(_maincontrol.isTextCaptureMode())
            {
                _maincontrol.insertCharacter(event.key.keysym);
            }
        }
        else if (event.type == SDL_KEYUP)
            _maincontrol.releaseKey(event.key.keysym.sym);	
        else if ( event.type == SDL_MOUSEMOTION ) 
        {
            
        }
    }
    statestack.back()->handleEvents();
    _maincontrol.update();
}

Controller& GameEngine::getUserController()
{
    return _maincontrol;
}


void GameEngine::draw(GameEngine *engine)
{
  //glClearDepth(1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if(!statestack.empty())
    statestack.front()->draw();
  else
    return;
  SDL_GL_SwapBuffers();
}

void GameEngine::update(GameEngine *engine,double t,double dt)
{
    statestack.back()->update(t,dt);
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