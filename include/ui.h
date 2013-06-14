
#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include "graphics.h"
#include "vec.h"


using wex::Point3f;
using wex::Point2f;

using namespace std;
using namespace wex::graphics;

void drawWindow(Point3f pos,float height,float width);

class LogLine;

class Console
{
protected:
	bool newline;
	bool visible;
	TextureFont* font;

	vector<LogLine> linebuffer;
	deque<string> displaybuffer;
	int topline;

	Point3f   position;
	ColorRGBA color;

	float yspacing;
	float xspacing;
	
	float rows;
	float columns;
	
	void addToDisplayBuffer(LogLine line);
public:
	void setVisible(bool visibility);
	void setRows(int lines);
	int  getRows();
	int  getColumns();
	void setColumns(int columns);
	void setSpacing(float x);
	void setLineHeight(float y);
	void setFont(std::string name);
	void setColor(ColorRGBA color);

	int getWindowHeight();
	int getWindowWidth();

	void print(std::string text);
	void reformat();
	void setPosition(Point3f position);
	Point3f getPosition();
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

class LogLine
{
private:
	int formatlength;
	vector<int> breaks;
public:
	string content;

	void   reformat(int length);
	int    getFormattedLineCount();
	string getFormattedLine(int index);
	LogLine(string content,int formatlength);
};
