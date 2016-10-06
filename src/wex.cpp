#include "wex.h"

void wex::draw(const wex::graphics::Quad& quad,const Point3f& position)
{
	renderer->drawQuad(quad,position,1,1,0);
}

void wex::draw(const wex::graphics::Sprite& sprite,const Point3f& position)
{
	renderer->drawSprite(sprite,position,1,1,0);
}

void wex::zoom(float factor)
{
}

void wex::setCamera(const Point3f& cameraposition)
{
}

void wex::moveCameraRelative(const Point3f& movementvector)
{
}

void wex::moveCameraTowards(const Point3f& position)
{
}

void wex::changeTexture(const char* name)
{
	renderer->changeTexture(name);
}

void wex::changeShader(const char* name)
{
	renderer->changeShader(name);
}

void wex::drawText(const char* text, const Point3f& position,ColorRGBA color,GLfloat space)
{
	renderer->drawText("default font",text,position,ColorRGBA(1,1,1,1),8,1.0);
}

void wex::drawFormattedText(const char* fontname,const char* text,const Point3f& position,ColorRGBA color, GLfloat space,int linelength)
{
}
/*

void wex::loadSound(const char* filename)
{
	soundmanager->loadSound(filename);
}

void wex::playSound(const char* filename)
{
	soundmanager->playSound(filename);
}
*/

void wex::drawBuffer()
{
	renderer->drawBuffer();
}

void wex::pushState(EngineState& state)
{
	wex::GameEngine::Instance()->pushState(&state);
}

void wex::init()
{
	wex::GameEngine::Instance()->init();
}

void wex::run()
{
	wex::GameEngine::Instance()->run();
}

void wex::loadTexture(const char* filename)
{
	Texture* newtexture = new Texture();
	if(newtexture->loadUncompressedTGA(filename))
		textures[filename] = newtexture;
	else
		delete newtexture;
}
