#pragma once

#include <ctime>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include "enginestate.h"
#include "SDL\SDL.h"
#include "graphics.h"
#include "audio.h"
#include "timer.h"
#include "ui.h"
#include "luainterface.h"
#include "controller.h"
#include "opengl.h"
#include "triglookup.h"

class GameEngine
{
private:
	bool running;
    SDL_Event event;
    Timer fps;
	Lua lua;
    
	static Controller controller;
	static LSConsole* console;
	static audio::Source source;
	static audio::AudioBuffer buffer;

public:
	GameEngine();

	bool isRunning();
	bool init();
    
	void handleEvents();
    void update(double time,double delta);
    void draw(double t,double dt);
	
	static int l_console_print(lua_State* L);
	static int l_console_setPosition(lua_State* L);
	static int l_console_show(lua_State* L);
	static int l_console_hide(lua_State* L);
	static int l_console_setRows(lua_State* L);
	static int l_console_setColumns(lua_State* L);
	static int l_console_getCommand(lua_State* L);
	
	static int l_audio_loadOgg(lua_State* L);
	static int l_audio_playOgg(lua_State* L);
	
	static int l_main_handleEvents(lua_State* L);
	static int l_main_update(lua_State* L);
	static int l_main_draw(lua_State* L);

	static int l_input_keyPressed(lua_State* L);
	static int l_input_keyReleased(lua_State* L);
	static int l_input_keyHeld(lua_State* L);

	static int l_graphics_drawSprite(lua_State* L);

	void setKeyConstants();
	
};
	
	
	
	
	
	
	
	
	
	
	

