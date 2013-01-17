#include "mainstate.h"

#define vpos(h) h*SCREEN_HEIGHT
#define hpos(v) v*SCREEN_WIDTH

#define controller GameEngine::getController()
#define blink(a,t) (sin(t*a) > 0 ? "_" : " ")


///////////////////////////////////////////////
//  MAIN GAME STATE - Pushed onto stack when  
//                    game is started/loaded
///////////////////////////////////////////////

LSConsole MainGameState::console;
//audio::AudioBuffer MainGameState::buffer;
//audio::Source MainGameState::source;


int MainGameState::print(lua_State* L)
{
	int args = lua_gettop(L);
	cout << "print called with " << args << " arguments" << endl; 
	for(int i=1;i<=args;i++)
	{
		if(lua_isstring(L,(-lua_gettop(L))+(i-1)))
			MainGameState::console.print(getArgString(i,L));
		else
			cout << "Warning: Arg " << i << " is not a string" << endl; 
	}

	return 0;
}
int MainGameState::setConsolePosition(lua_State* L)
{
	console.setPosition(Point3f(getArgNumber(1,L),getArgNumber(2,L),0));
	return 0;
}

/*
int MainGameState::loadOgg(lua_State* L)
{
	int args = lua_gettop(L);
	cout << "loadOgg called with " << args << " arguments" << endl; 
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
*/

/*
int MainGameState::playOgg(lua_State* L)
{
	int args = lua_gettop(L);
	cout << "playOgg called with " << args << " arguments" << endl; 
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
*/
int MainGameState::showConsole(lua_State* L)
{
	console.setVisible(true);
	return 0;
}
int MainGameState::hideConsole(lua_State* L)
{
	console.setVisible(false);
	return 0;
}
int MainGameState::setConsoleRows(lua_State* L)
{
	console.setRows(getArgNumber(1,L));
	return 0;
}
int MainGameState::setConsoleColumns(lua_State* L)
{
	console.setColumns(getArgNumber(1,L));
	return 0;
}

int MainGameState::getCommand(lua_State* L)
{
	lua_pushstring(L,GameEngine::getController().getCapturedText().c_str());
	return 1;
}


void MainGameState::initState()
{
  cout << "Setting color" << endl;
  console.setColor(ColorRGBA(1,1,1,1));
  cout << "Setting font" << endl;
  console.setFont("uifont");
  cout  << "Starting text capture" << endl;
  GameEngine::getController().startTextCapture();
  //Bind all our lua functions
  //source.attachBuffer(buffer);
  
  cout  << "Binding lua functions" << endl;
  
  
  lua["print"] = print;
  lua["setConsolePosition"] = setConsolePosition;
  lua["showConsole"] = showConsole;
  lua["hideConsole"] = hideConsole;
  lua["setConsoleRows"] = setConsoleRows; 
  lua["setConsoleColumns"] = setConsoleColumns;
  lua["getCommand"] = getCommand;
  //lua["loadOgg"] = loadOgg;
  //lua["playOgg"] = playOgg;
  lua["trim"] = stringtrim;
  cout << "Loading lua scripts" << endl;
  lua.loadScript("init.lua");
  lua.loadScript("command.lua");
  lua.loadScript("step.lua");

  //Run the init script
  lua["init.lua"].call();
  
}

void MainGameState::cleanup()
{

}

void MainGameState::pause()
{

}

void MainGameState::resume()
{

}

void MainGameState::handleEvents()
{
	if(controller.keyPressed(SDLK_RETURN))
	{
		if(controller.getCapturedText() == "")
			return;
		console.print(controller.getCapturedText() + "\n");
		cout << "Command given is: " + controller.getCapturedText() << endl;
		lua["command"] = controller.getCapturedText();
		lua["command.lua"].call();
		if(lua.hasError())
			cout << lua.getError() << endl;
		controller.flushText();
	}
}

void MainGameState::updateState(double t,double dt)
{
}

void MainGameState::drawState(double t,double dt)
{
	drawWindow(console.getPosition()+Point3f(0,console.getWindowHeight()+8,0),20, controller.getCapturedText().length() < console.getColumns() ? console.getWindowWidth() : (8*(controller.getCapturedText().length()+2)));
	renderer->changeTexture("uifont");
	renderer->drawText("uifont",controller.getCapturedText() + blink(10,t),console.getPosition()+Point3f(16,console.getWindowHeight()+20,0),ColorRGBA(1,1,1,1),8.0);
	renderer->drawText("uifont","wex v0.1a",Point3f(10,10,0),ColorRGBA(1,1,1,1),8.0);

	renderer->drawBuffer();
	MainGameState::console.draw();
}

MainGameState::MainGameState(GameEngine& engine)
{
	this->engine = &engine;
}

MainGameState::MainGameState()
{

}

