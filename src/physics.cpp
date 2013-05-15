#include "physics.h"

using namespace physics;

State interpolate(const State &previous, const State &current, float alpha)
{
	State state;
	state.p = current.p*alpha + previous.p*(1-alpha);
	state.v = current.v*alpha + previous.v*(1-alpha);
	return state;
}

Derivative PhysState::evaluate(double t)
{
	Derivative derivativesample;

	derivativesample.dx = state.v;

	derivativesample.dv = acceleration(state,t);

	return derivativesample;
}

Derivative PhysState::evaluate(double t, double dt, const Derivative &d)
{
     State newstate;                   


     newstate.p = state.p + d.dx * (double)dt;  // new position = initial position + (previously derived velocity) * time 
     newstate.v = state.v + d.dv * (double)dt;  // new velocity = initial velocity + (previously derived acceleration) * time

     Derivative derivativesample;

     derivativesample.dx = newstate.v;						 
     derivativesample.dv = acceleration(newstate, t+dt);     //Calculate the acceleration based on the new state
     return derivativesample;                                
}

Point3d PhysState::acceleration(const State &newstate, double t)
{
	return state.f;
}

void PhysState::integrate(double t, double dt)
{
	 prevstate = state;
     Derivative a = evaluate(t);                       // First sample (no delta time)
     Derivative b = evaluate(t, dt*0.5, a);            // Second sample based on first
     Derivative c = evaluate(t, dt*0.5, b);            // Third sample based on second
     Derivative d = evaluate(t, dt, c);                // Fourth sample based on third

     const Point3d dxdt = (1.0/6.0)*(a.dx + (b.dx + c.dx)*2.0 + d.dx);   //Weighted average of the samples
     const Point3d dvdt = (1.0/6.0)*(a.dv + (b.dv + c.dv)*2.0 + d.dv);   //

     state.p = state.p + dxdt * dt;  // Position = Position + Velocity * time
     state.v = state.v + dvdt * dt;  // Velocity = Velocity + Acceleration * time

	 // I MIGHT WANT TO REMOVE THIS FOLLOWING LINE LATER BEACUSE IM SHIT AT PHYSICS AND IM JUST SORTA GOING BY INTUITION HERE

	 //Reset the forces before next integration.
	 state.f = Point3d(0,0,0);
}


PhysState::PhysState()
{
	state.p = Point3d(0,0,0);
	state.v = Point3d(0,0,0);
	state.f = Point3d(0,0,0);

	state.m = 1;

	prevstate.p = state.p;
	prevstate.v = state.v;
}

void PhysState::ApplyForce(Point3d force)
{
	state.f = state.f + force;
}

int physics::l_PhysState_constructor(lua_State *L)
{
	cout << "Creating new physics object" << endl;
	PhysState ** udata = (PhysState **)lua_newuserdata(L, sizeof(PhysState *));
	*udata = new PhysState();
	luaL_getmetatable(L, "luaL_PhysState");
	lua_setmetatable(L, -2);
	return 1;
}

int physics::l_PhysState_destructor(lua_State *L)
{
	return 0;
}

int physics::l_PhysState_applyForce(lua_State *L)
{
	PhysState* state = l_checkPhysState(L,1);
	Vector3d vec = Vector3d(getArgNumber(2,L),getArgNumber(3,L),getArgNumber(4,L));
	state->ApplyForce(vec);
	return 0;
}

int physics::l_PhysState_integrate(lua_State *L)
{
	PhysState* state = l_checkPhysState(L,1);
	state->integrate(getArgNumber(2,L),getArgNumber(3,L));
	return 0;
}

PhysState* physics::l_checkPhysState(lua_State *L, int n)
{
	return *(PhysState **)luaL_checkudata(L, n, "luaL_PhysState");
}

void physics::registerPhysState(lua_State *L)
{
	
	luaL_Reg sPhysStateRegs[] =
	{
		{ "new",              l_PhysState_constructor},
		{ "applyForce",       l_PhysState_applyForce},
		{ "integrate" ,       l_PhysState_integrate},
		{ "__gc",             l_PhysState_destructor},
		{NULL,NULL}
	};
	// Create a luaL metatable. This metatable is not
	// exposed to Lua. The "luaL_Sprite" label is used by luaL
	// internally to identity things.

	Lua lua = Lua(L);
	lua.stackDump();

	luaL_newmetatable(L, "luaL_PhysState");
	lua.stackDump();
	luaL_setfuncs (L, sPhysStateRegs, 0);
	lua.stackDump();
	lua_pushvalue(L, -1);
	lua.stackDump();
	lua_setfield(L, -1, "__index");
	lua.stackDump();
	lua_setglobal(L, "PhysState");
	lua.stackDump();
}
