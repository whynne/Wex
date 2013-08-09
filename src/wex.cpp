#include "wex.h"

void draw(const wex::graphics::Quad& quad,const Point3f& position)
{
	renderer->drawQuad(quad,position,1,1,0);
}

void draw(const wex::graphics::Sprite& sprite,const Point3f& position)
{
	renderer->drawSprite(sprite,position,1,1,0);
}

void zoom(float factor)
{
}

void setCamera(const Point3f& cameraposition)
{
}

void moveCameraRelative(const Point3f& movementvector)
{
}

void moveCameraTowards(const Point3f& position)
{
}

void changeTexture(const char* name)
{
	renderer->changeTexture(name);
}

void changeShader(const char* name)
{
	renderer->changeShader(name);
}

void drawText(const char* fontname,const char* text, Point3f position,ColorRGBA color,GLfloat space)
{

}

void drawFormattedText(const char* fontname,const char* text,Point3f position,ColorRGBA color, GLfloat space,int linelength)
{
}

void drawBuffer()
{
	renderer->drawBuffer();
}