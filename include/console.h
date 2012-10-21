
#pragma once

#include <string>
#include <list>
#include "graphics.h"
#include "vec.h"

using namespace std;
using namespace graphics;

class Console
{
private:
	list<string> _linebuffer;
	list<string>::iterator _topline;

	Point3d _position;
	ColorRGBA _color;

	double yspacing;
	double xspacing;

	double _rows;
	double _columns;
public:

	void print(std::string text);
	void setPosition(Point3d position);
	void draw();
	void clear();
	Console();
};
