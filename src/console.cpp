
#include "console.h"

typedef std::list<string>::iterator StringListIt;

void Console::print(std::string text)
{
	_linebuffer.push_back(text);
	if(_linebuffer.size() > _rows)
	{
		_topline++;
		return;
	}
	else
	{
		_topline = _linebuffer.begin();
		return;
	}
}

void Console::clear()
{
	StringListIt pos;
	for(pos = _linebuffer.begin();pos != _linebuffer.end();pos++)
	{
		_linebuffer.erase(pos);
	}
}

void Console::draw()
{
	StringListIt pos;
	double yoffset = 0;

	for(pos = _topline;pos != _linebuffer.end();pos++,yoffset-=yspacing)
	{
		glBindTexture(GL_TEXTURE_2D,2);
		cout << "Trying to draw the console but it's not fully implemented, ya dummy!" << endl;
		//Renderer::Instance()->drawText(*pos,_position-Point3d(0,yoffset,0),xspacing,_color);
	}
}

void Console::setPosition(Point3d position)
{
	_position = position;
}

Console::Console()
{
	xspacing = 10;
	yspacing = 15;
	_rows = 5;
	_topline = _linebuffer.begin();
	_color = ColorRGBA(1.0,1.0,0,1.0);
	_position = Point3d(0,0,0);
}
