#pragma once
#include "vec.h"
#include "luainterface.h"

namespace physics
{

	struct State
	{
		Point3d p; // position
		Point3d v; // velocity
		Point3d f; // force
		double m;
	};
	
	struct Derivative
	{ 
		Point3d dx; // derivative of position: velocity
		Point3d dv; // derivative of velocity: acceleration
	};
	
	State interpolate(const State &previous, const State &current, float alpha);
	
	class PhysState
	{
	public:
		State state,prevstate;
	
		Derivative evaluate(double t);
		Derivative evaluate(double t, double dt, const Derivative &d);
		
		void integrate(double t, double dt);
		void ApplyForce(Point3d force);	
	
		Point3d acceleration(const State &state, double t);
	
		PhysState();
		PhysState(double p, double v);
	};
	
	//Quad lua bindings
	
	int l_PhysState_constructor(lua_State *L);
	int l_PhysState_destructor(lua_State *L);
	int l_PhysState_applyForce(lua_State *L);
	int l_PhysState_integrate(lua_State *L);
	PhysState* l_checkPhysState(lua_State *L, int n);
	void registerPhysState(lua_State *L);
}