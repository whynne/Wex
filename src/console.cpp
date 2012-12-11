
#include "console.h"

#define SCALE 8.0f
#define FACTOR (9.0/(SCALE*2))

typedef std::vector<string>::iterator StringListIt;

void Console::setSpacing(float x)
{
	xspacing = x;
}

void Console::setLineHeight(float y)
{
	yspacing = y;
}

void Console::setRows(int rows)
{
	this->rows = rows;
}

void Console::setColumns(int columns)
{
	this->columns = columns;
}

void Console::setFont(std::string name)
{
	this->font = (TextureFont*)graphics::textures[name];
}

void Console::print(std::string text)
{
	if(text.length() < columns)
	{
		linebuffer.push_back(text);
	}
	else
	{
		bool done = false;
		int beg = 0;
		int pos = columns;
		while(!done)
		{
			if(text[pos] == ' ')
			{
				linebuffer.push_back(text.substr(beg,pos-beg));
				if(linebuffer.size() > rows)
				    topline++;
					

				text.erase(pos,1);
				beg = pos;
				pos += columns;
				if(beg+columns >= text.length())
				{
					linebuffer.push_back(text.substr(beg,text.length()-beg));
					if(linebuffer.size() > rows)
						topline++;
					break;
				}
			}
			else
			{
				pos--;
			}
		}
	}
}

void Console::clear()
{
	StringListIt pos;
	for(pos = linebuffer.begin();pos != linebuffer.end();pos++)
	{
		linebuffer.erase(pos);
	}
}

void Console::setColor(ColorRGBA color)
{
	this->color = color;
}

void Console::draw()
{
	StringListIt pos;
	double yoffset = 0;
	if(font == 0)
	{
		cout << "attempting to draw without a font" << endl;
		return;
	}
	for(int pos = topline;pos != linebuffer.size();pos++,yoffset-=yspacing)
	{
		renderer->drawText(font,linebuffer[pos],position-Point3f(0,yoffset,0),color,xspacing);
	}
}

void Console::setPosition(Point3f position)
{
	this->position = position;
}

Console::Console()
{
	xspacing = 10;
	yspacing = 15;
	rows = 5;
	columns = 20;
	topline = 0;
	color = ColorRGBA(0,0,0,0);
	position = Point3f(0,0,0);
}

LSConsole::LSConsole()
{

	color = ColorRGBA(0,0,0,1);
	corner = Sprite(SCALE*2,SCALE*2);
	verticalborder = Sprite(SCALE*2,SCALE*2);
	horizontalborder = Sprite(SCALE*2,SCALE*2);

	verticalborder.changeSpriteSheet("ui");
	verticalborder.changeSequence("vertical border");
	horizontalborder.changeSpriteSheet("ui");
	horizontalborder.changeSequence("horizontal border");
	corner.changeSpriteSheet("ui");
	corner.changeSequence("border corner");
}

void LSConsole::draw()
{	
	
	StringListIt pos;
	double yoffset = 0;

	
	renderer->changeTexture("ui");
	renderer->drawSprite(horizontalborder,position+Point3f(FACTOR*SCALE*(columns+SCALE/2),0.0,0),FACTOR*(columns+3),1,0);
	renderer->drawSprite(horizontalborder,position+Point3f(FACTOR*SCALE*(columns+SCALE/2),2*SCALE*(rows+2),0),FACTOR*(columns+3),1,0);
	renderer->drawSprite(corner,position+Point3f(FACTOR*2*SCALE*(columns+SCALE/2),0,0));
	renderer->drawSprite(corner,position+Point3f(FACTOR*2*SCALE*(columns+SCALE/2),2*SCALE*(rows+2),0));

	renderer->drawSprite(corner,position);
	
	renderer->drawSprite(corner,position+Point3f(0,2*SCALE*(rows+2),0));
	renderer->drawSprite(verticalborder,position+Point3f(0,SCALE*(rows+2),0),1,rows+1,0);
	renderer->drawSprite(verticalborder,position+Point3f(FACTOR*2*SCALE*(columns+SCALE/2),SCALE*(rows+2),0),1,rows+1,0);

	renderer->drawBuffer();
	
	
	if(font == 0)
		return;
	renderer->changeTexture(font->getTexId());
	for(int pos = topline;pos != linebuffer.size();pos++,yoffset-=yspacing)
		renderer->drawText(font,linebuffer[pos],position+Point3f(SCALE*3,SCALE*3,0)-Point3f(0,yoffset,0),color,xspacing);
	renderer->drawBuffer();

	
}