#include "ui.h"


typedef std::deque<string>::iterator StringDequeIt;
typedef std::vector<LogLine>::iterator LogLineIt;

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

void Console::setVisible(bool visibility)
{
	visible = visibility;
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
	//Add a blank line if there's no lines in the buffer
	if(linebuffer.size()==0)
		linebuffer.push_back(LogLine("",columns));

	//Take care of single character newline cases
	if(text == "\n" && newline)
	{
		linebuffer.push_back(LogLine("",columns));
		displaybuffer.push_back("");
		return;
	}
	else if(text == "\n" && !newline)
	{
		newline = true;
		return;
	}
	
	//Tokenize input text by newline and process them

	stringstream stream(text);
	string line;

	while(getline(stream,line,'\n'))
	{
		//If the line is a blank token, then we encountered leading/trailing newline
		if(newline == true)
		{
			linebuffer.push_back(LogLine("",columns));
			displaybuffer.push_back("");
			newline = false;
		}
		if(line == "")
		{
			newline = true;
			continue;
		}

		//Get the original number of formatted lines
		int originalcount = linebuffer.back().getFormattedLineCount();

		//Add the text to the log.
		linebuffer.back().content += line;
		linebuffer.back().reformat(columns);

		//Get the difference in formated lines after printing.
		int delta = linebuffer.back().getFormattedLineCount() - originalcount;
		//If there was a change, we need to add some lines to the display buffer
		if(delta > 0)
		{
			//pop the last line, which has changed
			displaybuffer.pop_back();
			//push in all the new lines
			for(int i = -1;i < delta;i++)
				displaybuffer.push_back(linebuffer.back().getFormattedLine((originalcount+i)));
		}
		//Otherwise just add the text directly.
		else
		{
			displaybuffer.back()+=line;
		}
	}

	//Oh god. 

	if(text.length()>0)
	{
		if(text[text.length()-1]=='\n')
		{
			if(newline)
			{
				linebuffer.push_back(LogLine("",columns));
				displaybuffer.push_back("");
			}
			newline = true;
		}
	}
}


void Console::clear()
{
	LogLineIt pos;
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
}


void Console::setPosition(Point3f position)
{
	this->position = position;
}

Point3f Console::getPosition()
{
	return position;
}

int Console::getRows()
{
	return rows;
}

int Console::getColumns()
{
	return columns;
}

Console::Console()
{
	xspacing = 10;
	yspacing = 15;
	rows = 5;
	columns = 20;
	topline = 0;
	color = ColorRGBA(0,0,1,1);
	position = Point3f(0,0,0);
	displaybuffer.push_back("");
	linebuffer.push_back(LogLine("",columns));
}

LSConsole::LSConsole()
{
	xspacing = 8;
	yspacing = 15;
	rows = 5;
	columns = 20;
	topline = 0;
	color = ColorRGBA(1,1,0,1);
	position = Point3f(0,0,0);
	this->setFont("uifont");
}

int Console::getWindowHeight()
{
	return yspacing*(rows);
}

int Console::getWindowWidth()
{
	return xspacing*(columns+2);
}

void LSConsole::draw()
{	
	if(!visible)
		return;
	LogLineIt pos;
	double yoffset = 0;
	
	drawWindow(this->position,getWindowHeight(),getWindowWidth());

	renderer->changeTexture("uifont");

	if(font == 0)
	{
		cout << "Warning: No font set in console!" << endl;
		return;
	}
	renderer->changeTexture(font->getTexId());
	int i = 0;
	for(StringDequeIt it = displaybuffer.end()-1;i<=rows-1;yoffset+=yspacing,i++)
	{
		renderer->drawText("uifont",*it,position+Point3f(0,yspacing*rows,0)+Point3f(16,-5,0)-Point3f(0,yoffset,0),ColorRGBA(1,0,1,1),xspacing);
		if(it==displaybuffer.begin())
			break;
		it--;
	}
	renderer->drawBuffer();

	
}

void drawWindow(Point3f pos,float height,float width)
{	

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

void LogLine::reformat(int length)
{
	if(content.empty())
		return;
	int position;
	//Continue formatting after the last line break if you enter the same length for reformatting
	if(length == this->formatlength)
	{
		breaks.pop_back();
		position = breaks.size() > 0 ? breaks.back() : 0;
	}
	//Otherwise clear all the linebreak positions and start the process anew
	else
	{
		breaks.clear();
		position = 0;
	}
	while(true)
	{
		int breakpos = 0;
		int line = breaks.size();
		string currentline = content.substr(position,length+1);


		if(line > 0)
		{
			for(int i = 0;i < currentline.length();i++)
			{
				if(currentline[i] == ' ')
					breaks[line-1] += 1;
				else
					break;
			}
		}

		if(currentline.length()-1 < length)
		{
			breaks.push_back(position + currentline.length());
			return;
		}

		breakpos += currentline.find_last_of(' ');
		
		if(breakpos == currentline.length())
		{
			breaks.push_back(position + length);
			position+=length;
		}
		else if(breakpos!=string::npos)
		{
			breaks.push_back(position + breakpos);
			position+=breakpos;
		}
		else
		{
			breaks.push_back(position + length);
			position+=length;
		}
	}
}

int LogLine::getFormattedLineCount()
{
	return breaks.size();
}

string LogLine::getFormattedLine(int index)
{
	if(index == 0)
		return content.substr(0,breaks[0]);
	else
		return content.substr(breaks[index-1],breaks[index]-breaks[index-1]);
}

LogLine::LogLine(string content,int formatlength)
{
	this->content = content;
	this->formatlength = formatlength;
	breaks.push_back(0);
	reformat(this->formatlength);
}
