#include "engine.h"

#define vpos(h) h*SCREEN_HEIGHT
#define hpos(v) v*SCREEN_WIDTH
#define blink(a,t) (sin(t*a) > 0 && controller.isTextCaptureMode() ? "_" : " ")


 Controller         GameEngine::controller;
 LSConsole*          GameEngine::console;
 audio::Source      GameEngine::source;
 audio::AudioBuffer GameEngine::buffer;

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

	audio::init();
	
	source = audio::Source::Source();
    console = new LSConsole();

	console->setColor(graphics::ColorRGBA(1,1,1,1));
  
	
	console->setFont("uifont");
    controller.startTextCapture();
	
    //Bind all our lua functions
	
	lua.pushGlobalTable();

	graphics::registerSprite(lua.getState());

	setKeyConstants();
    lua["wex"] = newtable;
    lua["wex"].push();
		lua["console"] = newtable;
		lua["audio"]   = newtable;
		lua["physics"] = newtable;
		lua["graphics"] = newtable;
		lua["input"] = newtable;

		lua["console"].push();
		    lua["print"]       = l_console_print;
		    lua["setPosition"] = l_console_setPosition;
		    lua["show"]        = l_console_show;
		    lua["hide"]        = l_console_hide;
		    lua["setRows"]     = l_console_setRows;
		    lua["setColumns"]  = l_console_setColumns;
		lua.pop();
		
		lua["audio"].push();
		    lua["load"] = l_audio_loadOgg;
		    lua["play"] = l_audio_playOgg;
		lua.pop();

		lua["graphics"].push();
		    lua["drawSprite"] = l_graphics_drawSprite;
		lua.pop();

		lua["input"].push();
		    lua["keyPressed"]  = l_input_keyPressed;
		    lua["keyHeld"]     = l_input_keyHeld;
			lua["keyReleased"] = l_input_keyReleased;
		lua.pop();
    lua.pop();

    lua.loadScript("main.lua");
    lua["main.lua"].call();

	lua["wex"].push();
	lua["init"].call();
	lua.pop();
  
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
        else if ( event.type == SDL_MOUSEMOTION ) 
        {
            
        }
    }
	
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
	lua["wex"].push();
    lua["handleEvents"].call();
    lua.pop();

	controller.update();
}


void GameEngine::draw(double t,double dt)
{
	
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	
	if(controller.isTextCaptureMode())
	{
		drawWindow(console->getPosition()+Point3f(0,console->getWindowHeight()+8,0),20, controller.getCapturedText().length() < console->getColumns() ? console->getWindowWidth() : (8*(controller.getCapturedText().length()+2)));
		renderer->changeTexture("uifont");
		renderer->drawText("uifont",controller.getCapturedText() + blink(10,t),console->getPosition()+Point3f(16,console->getWindowHeight()+20,0),ColorRGBA(1,1,1,1),8.0);
		renderer->drawText("uifont","wex v0.1a",Point3f(10,10,0),ColorRGBA(1,1,1,1),8.0);
		renderer->drawBuffer();
		console->draw();
	}
  
  lua["wex"].push();
  lua["draw"].call();
  lua.pop();
  SDL_GL_SwapBuffers();
  
}

void GameEngine::update(double t,double dt)
{
    lua["wex"].push();
	lua["update"].call();
	lua.pop();
}

bool GameEngine::isRunning()
{
    return running;
}

int GameEngine::l_console_print(lua_State* L)
{
	int args = lua_gettop(L);
	for(int i=1;i<=args;i++)
	{
		if(lua_isstring(L,(-lua_gettop(L))+(i-1)))
			console->print(getArgString(i,L));
		else
			cout << "Warning: Arg " << i << " is not a string" << endl; 
	}
	return 0;
}

int GameEngine::l_console_setPosition(lua_State* L)
{
	int x = (int)getArgNumber(1,L);
	int y = (int)getArgNumber(2,L);
	cout << "(" << x << ","<< y << ")" << endl;
	console->setPosition(Point3f(x,y,0));
	return 0;
}

int GameEngine::l_console_show(lua_State* L)
{
	console->setVisible(true);
	return 0;
}
int GameEngine::l_console_hide(lua_State* L)
{
	console->setVisible(false);
	return 0;
}
int GameEngine::l_console_setRows(lua_State* L)
{
	console->setRows(getArgNumber(1,L));
	return 0;
}
int GameEngine::l_console_setColumns(lua_State* L)
{
	console->setColumns(getArgNumber(1,L));
	return 0;
}
int GameEngine::l_console_getCommand(lua_State* L)
{
	lua_pushstring(L,controller.getCapturedText().c_str());
	return 1;
}

int GameEngine::l_audio_loadOgg(lua_State* L)
{
	int args = lua_gettop(L);
	for(int i=1;i<=args;i++)
	{
		if(lua_isstring(L,(-lua_gettop(L))+(i-1)))
		{
			audio::AudioBuffer buf;
			buf.loadVorbisFile(getArgString(i,L));
			if(!buf.getId() > 0)
			{
				cout << "Warning: Failed to load file " << getArgString(i,L) << "... Skipping." << endl;
				continue;
			}
			audio::audiobuffers[getArgString(i,L)] = buf;
			cout << "Successfully loaded file" << getArgString(i,L) << " with id " << buf.getId() << endl;
		}
		else
			cout << "Warning: Arg " << i << " is not a string" << endl; 
	}

	return 0;
}
int GameEngine::l_audio_playOgg(lua_State* L)
{
	int args = lua_gettop(L);
	if(args != 1)
	{
		cout << "Warning: Wrong number of args in function playOgg." << endl;
		return 0;
	}
	if(lua_isstring(L,1) && audio::audiobuffers.find(getArgString(1,L)) != audio::audiobuffers.end())
	{
		cout << "playing..." << endl;
		source.setPosition(Point3f(0,0,0));
		source.attachBuffer(audio::audiobuffers[getArgString(1,L)]);
		source.play();
	}

	return 0;
}

int GameEngine::l_input_keyPressed(lua_State* L)
{
	int key = getArgNumber(1,L);
	bool result = controller.keyPressed(key);
	lua_pushboolean(L,result);
	return 1;
}
int GameEngine::l_input_keyReleased(lua_State* L)
{
	int key = getArgNumber(1,L);
	bool result = controller.keyReleased(key);
	lua_pushboolean(L,result);
	return 1;
}
int GameEngine::l_input_keyHeld(lua_State* L)
{
	int key = getArgNumber(1,L);
	bool result = controller.keyHeld(key);
	lua_pushboolean(L,result);
	return 1;
}

int GameEngine::l_graphics_drawSprite(lua_State* L)
{
	Sprite* sprite = (Sprite*)getArgUserData(1,L);
	cout << sprite->getHeight() << endl;
	cout << sprite->getWidth() << endl;
	return 0;
}

void GameEngine::setKeyConstants()
{
	lua["key"] = newtable;
	lua["key"].push();
	
	lua["up"]    = SDLK_UP;
	lua["down"]  = SDLK_DOWN;
	lua["left"]  = SDLK_LEFT;
	lua["right"] = SDLK_RIGHT;
	lua["enter"] = SDLK_RETURN;
	lua["esc"]   = SDLK_ESCAPE;
	lua["tab"]   = SDLK_TAB;
	lua["space"] = SDLK_SPACE;

	lua.pop();
}