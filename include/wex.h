#ifndef H_WEX
#define H_WEX

#include "graphics.h"
#include "audio.h"
#include "engine.h"
#include "physics.h"
#include "timer.h"
#include "vec.h"
#include "ui.h"
#include "net.h"
#include "matrix.h"
#include "geometry.h"
#include "controller.h"
#include "SDL\SDL_main.h"
#include "SDL\SDL.h"

#include "begincode.h"

#undef main

namespace wex
{
	//rendering
	void WEXAPI draw(const wex::graphics::Quad& quad,const Point3f& position);
	void WEXAPI draw(const wex::graphics::Sprite& sprite,const Point3f& position);
	void WEXAPI zoom(float factor);
	void WEXAPI setCamera(const Point3f& cameraposition);
	void WEXAPI moveCameraRelative(const Point3f& movementvector);
	void WEXAPI moveCameraTowards(const Point3f& position);
	void WEXAPI changeTexture(const char* name);
	void WEXAPI changeShader(const char* name);
	void WEXAPI drawText(const char* text, const Point3f& position,ColorRGBA color,GLfloat space);              //Draws white text.
	void WEXAPI drawFormattedText(const char* fontname,const char* text,const Point3f& position,ColorRGBA color, GLfloat space,int linelength);
	void WEXAPI drawBuffer();
	
	//graphical assets
	void WEXAPI loadTexture(const char* filename);

	//sound
	void WEXAPI loadSound(const char* filename);
	void WEXAPI playSound(const char* filename);

	//engine
	void WEXAPI run();
	void WEXAPI pushState(EngineState& state);
	void WEXAPI init();
}

//sound functions


//window functions

void displayDebugInfo();
void setWindowLabel();

#include "closecode.h"

#endif