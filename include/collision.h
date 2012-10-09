#pragma once

// Collision.h

//Contains classes and code for spatial indexing and collision

#include "vec.h"
#include <list>

namespace collision
{

	struct Circle
	{
		Point2d center;
		double radius;
	};
	
	struct Rectangle
	{
		Point2d topleft;
		Point2d topright;
		Point2d bottomleft;
		Point2d bottomright;
		Rectangle(double x1,double y1,double x2,double y2);
	};
	
	//Axis Aligned Bounding Box, requires less space 
	struct AABB
	{
	    Point2d topleft;
		Point2d bottomright;
	
		Point2d getMidPoint();
		AABB();
		AABB(double size);
	    AABB(double x1,double y1,double x2,double y2);
	};

	/*

	template <class T>
	class PositionItem
	{
		T*   parent;
		AABB boundingbox;
	};
	
	template <class T>
	class SpatialGrid
	{
	public:
		void insert(PositionItem<T> item);
		list<T> queryItems(Point2i location);
	};
	
	template <class T>
	class QuadTree
	{
		QuadTreeNode<T> root;
	};
	
	template <class T>
	class QuadTreeNode
	{
	private:
		bool ispartitioned;
		char maxitems;
		AABB boundingbox;
		list<PositionItem<T>> items;
		QuadTreeNode<T>* topleft;
		QuadTreeNode<T>* topright;
		QuadTreeNode<T>* bottomleft;
		QuadTreeNode<T>* bottomright;
	public:
		void insert(QuadTreeNode);
		partition();
	
	
		QuadTreeNode();
	};
	*/

}