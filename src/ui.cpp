#include "ui.h"


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
		renderer->drawText("uifont",linebuffer[pos],position-Point3f(0,yoffset,0),color,xspacing);
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
	xspacing = 8;
	yspacing = 15;
	rows = 5;
	columns = 20;
	topline = 0;
	color = ColorRGBA(1,1,1,1);
	position = Point3f(0,0,0);
	this->setFont("uifont");
}

void LSConsole::draw()
{	
	StringListIt pos;
	double yoffset = 0;
	
	drawWindow(this->position,yspacing*(rows+1),xspacing*(columns+2));

	renderer->changeTexture("uifont");

	if(font == 0)
		return;
	renderer->changeTexture(font->getTexId());
	for(int pos = topline;pos != linebuffer.size();pos++,yoffset-=yspacing)
		renderer->drawText("uifont",linebuffer[pos],position+Point3f(16,16,0)-Point3f(0,yoffset,0),color,xspacing);
	renderer->drawBuffer();

	
}

void drawWindow(Point3f pos,float height,float width)
{	
	ColorRGBA color = ColorRGBA(0,0,0,1);
	Sprite corner = Sprite(4.0000000,4.0000000);
	Sprite verticalborder = Sprite(4,4);
	Sprite horizontalborder = Sprite(4,4);

	verticalborder.changeSpriteSheet("ui");
	verticalborder.changeSequence("vertical border");
	horizontalborder.changeSpriteSheet("ui");
	horizontalborder.changeSequence("horizontal border");
	corner.changeSpriteSheet("ui");
    renderer->changeTexture("ui");

	renderer->drawSprite(horizontalborder,pos+Point3f((corner.getWidth()+width)/2.0,0,0),width/corner.getWidth(),1,0);
	renderer->drawSprite(horizontalborder,pos+Point3f((corner.getWidth()+width)/2.0,corner.getHeight()+height,0),width/corner.getWidth(),1,0);
	renderer->drawSprite(verticalborder,pos+Point3f(0,(corner.getHeight()+height)/2.0,0),1,height/corner.getHeight(),0);
	renderer->drawSprite(verticalborder,pos+Point3f(corner.getWidth()+width,(corner.getHeight()+height)/2.0,0),1,height/corner.getHeight(),0);


	corner.changeSequence("top left corner");
	renderer->drawSprite(corner,pos);
	corner.changeSequence("top right corner");
	renderer->drawSprite(corner,pos+Point3f(width+corner.getWidth(),0,0));
	corner.changeSequence("bottom right corner");
	renderer->drawSprite(corner,pos+Point3f(width+corner.getWidth(),height+corner.getHeight(),0));
	corner.changeSequence("bottom left corner");
	renderer->drawSprite(corner,pos+Point3f(0,height+corner.getHeight(),0));

	renderer->drawBuffer();
	
}