#include "engine.h"

#define vpos(h) h*SCREEN_HEIGHT
#define hpos(v) v*SCREEN_WIDTH
#define blink(a,t) (sin(t*a) > 0 && controller.isTextCaptureMode() ? "_" : " ")

using namespace wex;

GameEngine* GameEngine::instance;

Controller wex::GameEngine::controller;

GameEngine::GameEngine()
{
	t = 0.0;
	dt = 1.0/60.0;
	
	accumulator = 0.0; 
	frametime = 0.0;   
	newtime = 0.0;    
	eventtime = 0.0;
	currenttime = 0.0;
}

bool GameEngine::init()
{  
	//Game initializations
	srand(time(NULL));
	running = true;	
	TrigLookUp::calculateTrigValues();

	
	//SDL/OpenGL initializations
	if(!graphics::Init()){
		return false;
	}
	cout << "Graphics successfully initialized!" << endl;
	glClearColor(0,0,0,1);

	//OpenAL init
	//audio::init();
	counter.start();
	return true;
}

void GameEngine::handleEvents()
{
	//Pump and catch basic events
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
		else if ( event.type == SDL_MOUSEBUTTONDOWN ) 
		{
		}
	}
	/*
	
	if(controller.keyPressed(SDLK_RETURN) && controller.isTextCaptureMode())
	{
		if(controller.getCapturedText() == "")
		{
			return;
		}
		console->print(controller.getCapturedText() + "\n");
		controller.flushText();
	}
	else if(controller.keyPressed(SDLK_BACKQUOTE) && !controller.isTextCaptureMode())
	{
		console->setVisible(true);
		controller.startTextCapture();
	}
	else if(controller.keyPressed(SDLK_BACKQUOTE) && controller.isTextCaptureMode())
	{
		console->setVisible(false);
		controller.stopTextCapture();
		controller.backspace();
	}
	controller.update();
	controller.updateMouse();
	*/
}

void GameEngine::draw(double t,double dt)
{
	
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	/*
	if(controller.isTextCaptureMode())
	{
		drawWindow(console->getPosition()+Point3f(0,console->getWindowHeight()+8,0),20, controller.getCapturedText().length() < console->getColumns() ? console->getWindowWidth() : (8*(controller.getCapturedText().length()+2)));
		renderer->drawText("uifont",controller.getCapturedText() + blink(10,t),console->getPosition()+Point3f(16,console->getWindowHeight()+20,0),ColorRGBA(1,1,1,1),8.0);
		console->draw();
	}
	*/
	states.back()->draw();
	SDL_GL_SwapWindow(window);
}

void GameEngine::run()
{
	frametimer.start();
	while(this->isRunning())
	{
		if(states.back()==0)
		{
			cout << "[WEX]: No state to run.  Shutting down..." << endl;
			return; // We have no state to run.  Return prematurely.
		}

		if(counter.getTimeInSeconds() > 1.0)
		{
			counter.start();
			cout << "[WEX]: " << dps << " draws this second - "  << ups << " updates this second" << endl;
			resetCounters();
		}

		newtime = frametimer.getTimeInSeconds();
		frametime = newtime - currenttime;
		currenttime = newtime;

		if (frametime>0.25f)
			frametime = 0.25f;

		accumulator += frametime;
		eventtime += frametime;

		handleEvents();

		while (accumulator >= dt)
		{
			update(t,dt);
			ups+=1;
			accumulator -= dt;            
			t+=dt;                        
		}

		draw(t,dt);

		dps+=1;
	}
}

void GameEngine::update(double t,double dt)
{
	states.back()->update(t,dt);
}

bool GameEngine::isRunning()
{
	return running;
}

void GameEngine::pushState(EngineState* state)
{
	state->setEngine(this);
	state->begin();
	states.push_back(state);
}

void GameEngine::popState()
{
	states.pop_back();
}

GameEngine* GameEngine::Instance()
{
	if(!GameEngine::instance)
		instance = new GameEngine();
	return instance;
}


void EngineState::setEngine(GameEngine* engine)
{
	this->engine = engine;
}