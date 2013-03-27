#pragma once

#include <iostream>
#include "Lua/lua.hpp"
#include <vector>
#include <string>
 
#define getArgString(a,L) lua_tostring(L,(-lua_gettop(L))+(a-1))
#define getArgNumber(a,L) lua_tonumber(L,(-lua_gettop(L))+(a-1))
#define getArgBool(a,L) lua_tobool(L,(-lua_gettop(L))+(a-1))
#define getArgUserData(a,L) lua_touserdata(L,(-lua_gettop(L))+(a-1))
#define getArgNil(a,L) lua_tonil(L,(-lua_gettop(L))+(a-1))

using std::cout;
using std::endl;
using std::string;

enum luatypes
{
	nil,
	newtable
};

int stringtrim(lua_State *L);

class RegistrationUnit
{
private:
	lua_State* L;
	string label;
public:
	void setLabel(string name);
	void operator=(int value);
	void operator=(string value);
	void operator=(luatypes type);
	void operator=(lua_CFunction function);
	void push();
	void call();
	operator double();
	operator string();
	operator bool();
	RegistrationUnit();
	RegistrationUnit(lua_State* L);
};

class Lua
{
private:
	lua_State* L;
	RegistrationUnit regunit;

public:
	Lua();
	Lua(lua_State* L);

	void pushGlobalTable();
	void setField(string name,lua_CFunction function);
	void setField(string name,luatypes type);
	void pushField(string name);
	void pop();

	lua_State* getState(){return L;};
	bool hasError(){return lua_gettop(L) >= 1;};
	std::string getError(){return lua_tostring(L,-1);};
	void loadScript(string name);
	void stackDump();
	RegistrationUnit& operator [](string b);
};