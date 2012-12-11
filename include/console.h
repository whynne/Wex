
#pragma once

#include <string>
#include <vector>
#include "graphics.h"
#include "vec.h"

using namespace std;
using namespace graphics;

class Console
{
protected:
	TextureFont* font;

	vector<string>           linebuffer;
	int topline;

	Point3f   position;
	ColorRGBA color;

	float yspacing;
	float xspacing;
	
	float rows;
	float columns;
public:
	void setRows(int lines);
	void setColumns(int columns);
	void setSpacing(float x);
	void setLineHeight(float y);
	void setFont(std::string name);
	void setColor(ColorRGBA color);
	void print(std::string text);
	void setPosition(Point3f position);
	void draw();
	void clear();
	Console();
};

class LSConsole : public Console
{
private:
	Sprite corner;
	Sprite horizontalborder;
	Sprite verticalborder;
public:
	static void init();
	void draw();
	LSConsole();

};