#include "luainterface.h"

RegistrationUnit& Lua::operator[](string b)
{
	regunit.setLabel(b);
	return regunit;
}

void Lua::pushGlobalTable()
{
	lua_pushglobaltable(L);
}

void RegistrationUnit::setLabel(string name)
{
	label = name;
}

void RegistrationUnit::operator=(int value)
{
	lua_pushnumber(L,value);
	lua_setfield(L,-2,label.c_str());
}

void RegistrationUnit::operator=(float value)
{
	lua_pushnumber(L,value);
	lua_setfield(L,-2,label.c_str());
}

void RegistrationUnit::operator=(double value)
{
	lua_pushnumber(L,value);
	lua_setfield(L,-2,label.c_str());
}

void RegistrationUnit::operator=(string value)
{
	lua_pushstring(L,value.c_str());
	lua_setfield(L,-2,label.c_str());
}

void RegistrationUnit::push()
{
	lua_getfield(L,-1,label.c_str());
}

void Lua::setField(string field,lua_CFunction function)
{
	lua_pushcfunction(L,function);
	lua_setfield(L,-2,field.c_str());
}

void Lua::setField(string field,luatypes type)
{
	if(type == newtable)
	{
		lua_newtable(L);
		lua_setfield(L,-2,field.c_str());
	}
	
}

void Lua::pop()
{
	lua_pop(L,1);
}


void RegistrationUnit::operator=(luatypes type)
{
	if(type == nil)
		lua_pushnil(L);
	else if(type == newtable)
		lua_newtable(L);
	lua_setfield(L,-2,label.c_str());
}

void RegistrationUnit::operator=(lua_CFunction function)
{
	lua_pushcfunction(L,function);
	lua_setfield(L,-2,label.c_str());
}


RegistrationUnit::operator bool()
{
	lua_getglobal(L,label.c_str());
	return lua_toboolean(L,-1);
}

RegistrationUnit::operator double()
{
	lua_getglobal(L,label.c_str());
	return lua_tonumber(L,-1);
}

RegistrationUnit::operator string()
{
	lua_getglobal(L,label.c_str());
	return lua_tostring(L,-1);
}

Lua::Lua()
{
	L = luaL_newstate();
    luaL_openlibs(L);

	regunit = RegistrationUnit(L);
}

Lua::Lua(lua_State* L)
{
	this->L = L;
	regunit = RegistrationUnit(L);
}

//Print the lua stack 

void Lua::stackDump()
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {
      int t = lua_type(L, i);
	  cout << "Stack[" << i << "]: ";
      switch (t) {
        case LUA_TSTRING:
          cout << "\"" << lua_tostring(L, i) << "\"";
          break;
        case LUA_TBOOLEAN:
          cout << lua_toboolean(L, i) ? "true" : "false";
          break;
        case LUA_TNUMBER:
          cout << lua_tonumber(L, i);
          break;
        default:
          cout << lua_typename(L, t) << " " << cout;
		  cout << " " << i << " "; 
          break;
      }
      cout<<"  ";
    }
    cout<<"\n";  
}

void Lua::loadScript(string name)
{
	luaL_loadfile(L, name.c_str());
	lua_setglobal(L,name.c_str());
}

RegistrationUnit::RegistrationUnit()
{
	L = 0;
}

RegistrationUnit::RegistrationUnit(lua_State* L)
{
	this->L = L;
}

void RegistrationUnit::call()
{
	lua_getfield(L,-1,label.c_str());
	lua_pcall(L, 0, LUA_MULTRET, 0);
}


int stringtrim(lua_State *L)
{
 const char *front;
 const char *end;
 size_t      size;

 front = luaL_checklstring(L,1,&size);
 end   = &front[size - 1];

 for ( ; size && isspace(*front) ; size-- , front++)
   ;
 for ( ; size && isspace(*end) ; size-- , end--)
   ;

 lua_pushlstring(L,front,(size_t)(end - front) + 1);
 return 1;
}