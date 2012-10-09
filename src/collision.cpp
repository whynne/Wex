#include "collision.h"
using namespace collision;

AABB::AABB()
{
	topleft = Point2d(0,1);
	bottomright = Point2d(1,0);
}

AABB::AABB(double x1,double y1,double x2,double y2)
{
	topleft = Point2d(x1,y1);
	bottomright = Point2d(x2,y2);
}