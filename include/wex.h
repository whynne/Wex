#ifndef H_WEX
#define H_WEX

#include "graphics.h"
//#include "audio.h"
#include "engine.h"
#include "physics.h"
#include "timer.h"
#include "vec.h"
#include "ui.h"
#include "net.h"
#include "matrix.h"
#include "geometry.h"
#include "controller.h"
#include "SDL2/SDL_main.h"
#include "SDL2/SDL.h"

#undef main

namespace wex
{
	//rendering
	void draw(const wex::graphics::Quad& quad,const Point3f& position);
	void draw(const wex::graphics::Sprite& sprite,const Point3f& position);
	void zoom(float factor);
	void setCamera(const Point3f& cameraposition);
	void moveCameraRelative(const Point3f& movementvector);
	void moveCameraTowards(const Point3f& position);
	void changeTexture(const char* name);
	void changeShader(const char* name);
	void drawText(const char* text, const Point3f& position,ColorRGBA color,GLfloat space);              //Draws white text.
	void drawFormattedText(const char* fontname,const char* text,const Point3f& position,ColorRGBA color, GLfloat space,int linelength);
	void drawBuffer();
	
	//graphical assets
	void loadTexture(const char* filename);

	//sound
	/*
	void loadSound(const char* filename);
	void playSound(const char* filename);
	*/

	//engine
	void run();
	void pushState(EngineState& state);
	void init();
}

void displayDebugInfo();
void setWindowLabel();

#endif