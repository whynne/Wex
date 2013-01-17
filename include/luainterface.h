#pragma once

#include <iostream>
#include <Lua/lua.hpp>
#include <vector>
#include <string>
 
#define getArgString(a,L) lua_tostring(L,(-lua_gettop(L))+(a-1))
#define getArgNumber(a,L) lua_tonumber(L,(-lua_gettop(L))+(a-1))
#define getArgBool(a,L) lua_tobool(L,(-lua_gettop(L))+(a-1))
#define getArgNil(a,L) lua_tonil(L,(-lua_gettop(L))+(a-1))

using std::cout;
using std::endl;
using std::string;

enum Nil
{
	nil,
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
	void operator=(Nil nil);
	void operator=(lua_CFunction function);
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
	lua_State* getState(){return L;};
	bool hasError(){return lua_gettop(L) >= 1;};
	std::string getError(){return lua_tostring(L,-1);};
	void loadScript(string name);
	void stackDump();
	RegistrationUnit& operator [](string b);
};