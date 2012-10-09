#pragma once

#include "graphics.h"
#include "timer.h"
#include "physics.h"
#include "collision.h"
#include <list>
#include "SDL.h"

using namespace graphics;
using namespace std;

// Basic collideable object


class CollideableActor
{
protected:
	collision::AABB  _boundingbox;
	
public:
    collision::AABB  getBoundingBox();
	bool _aabb
	void draw();                         
};

