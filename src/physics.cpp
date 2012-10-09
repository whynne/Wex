#include "physics.h"

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
	 return state.f/state.m;
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

	state.m = 10;

	prevstate.p = state.p;
	prevstate.v = state.v;
}

void PhysState::ApplyForce(Point3d force)
{
	state.f = state.f + force;
}