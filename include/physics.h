#pragma once

#include "begincode.h"

#include "vec.h"

using wex::Point3d;

namespace physics
{

	struct  WEXAPI State
	{
		Point3d p; // position
		Point3d v; // velocity
		Point3d f; // force
		double m;
	};
	
	struct WEXAPI Derivative
	{ 
		Point3d dx; // derivative of position: velocity
		Point3d dv; // derivative of velocity: acceleration
	};
	
	State interpolate(const State &previous, const State &current, float alpha);
	
	class WEXAPI PhysState
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
}

#include "closecode.h"