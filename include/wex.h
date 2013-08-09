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

#undef main

//render functions
void draw(const wex::graphics::Quad& quad,const Point3f& position);
void draw(const wex::graphics::Sprite& sprite,const Point3f& position);
void zoom(float factor);
void setCamera(const Point3f& cameraposition);
void moveCameraRelative(const Point3f& movementvector);
void moveCameraTowards(const Point3f& position);
void changeTexture(const char* name);
void changeShader(const char* name);
void drawText(const char* fontname,const char* text, Point3f position,ColorRGBA color,GLfloat space);                   //Draws white text.
void drawFormattedText(const char* fontname,const char* text,Point3f position,ColorRGBA color, GLfloat space,int linelength);
void drawBuffer();

//sound functions


//window functions

void displayDebugInfo();
void setWindowLabel();

#endif