//This is a sort of scratch-pad state for testing things I'm learning

#pragma once
#include "engine.h"
#include "ui.h"
#include "audio.h"
#include <string>
#include "vec.h"
#include "luainterface.h"

class MainGameState : public EngineState
{
private:

	Lua lua;

public:
	static LSConsole console;
	//static audio::Source source;
	//static audio::AudioBuffer buffer;
	
	static int print(lua_State* L);
	static int setConsolePosition(lua_State* L);
	static int showConsole(lua_State* L);
	static int hideConsole(lua_State* L);
	static int setConsoleRows(lua_State* L);
	static int setConsoleColumns(lua_State* L);
	static int getCommand(lua_State* L);
	static int loadOgg(lua_State* L);
	static int playOgg(lua_State* L);


	void initState();
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void updateState(double t,double dt);
	void drawState(double t,double dt);

	MainGameState(GameEngine& engine);
	MainGameState();
};

