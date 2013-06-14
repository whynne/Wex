#pragma once

#include "begincode.h"

#include <set>
#include "vec.h"
namespace wex
{
	namespace geometry
	{
	
		template<typename T>
		std::set<Vec2<T>> WEXAPI convexHull2D(std::set<Vec2<T>> points)
		{
			std::set<Vec2<T>> resultset;
			std::set<Vec2<T>>::iterator it = points.begin();
			Vec2<T> initpoint = *it;
			Vec2<T> currentpoint;
			float smallestangle = 0;
			float lastangle = 0;
			//using the jarvis march and bottom-most point to start.
			it++;
			//Find lowest point
			for(;it!=points.end();it++)
			{
				if(it->y < initpoint.y)
					initpoint = *it;
			}
			currentpoint = initpoint;
			//Add to set
			resultset.insert(initpoint);
			//Start back at the beginning and get the first angle.  This is our smallest angle so far
	
			//Test all other points for smallest angle larger than previous angle
	
			while(true)
			{
				Vec2<T> hullpoint;
				it=points.begin();
				if(*it == currentpoint)
					it++;
				smallestangle = 2*pi;
				for(;it!=points.end();it++)
				{
					if(*it == currentpoint)
						continue;
					float a = clockwiseangle(currentpoint,*it);
					if(a >= lastangle && a < smallestangle)
					{
						smallestangle = a;
						hullpoint = *it;
					}
				}
				if(hullpoint != initpoint)
				{
					lastangle = clockwiseangle(currentpoint,hullpoint);
					currentpoint = hullpoint;
					resultset.insert(hullpoint);
				}
				else
					break;
			}
			return resultset;
		}
	}
}

#include "closecode.h"