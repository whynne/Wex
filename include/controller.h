#pragma once

#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include "SDL/SDL.h"
#include "vec.h"



enum Keystates
{
	unmapped,
	pressed,
	released,
	held,
};

struct Keydata
{
	int state;
	Keydata();
};


class Controller
{
private:
	Point2d _prevmousepos;
	Point2d _mouseposition;
	bool _mousemoved;
	bool _mousedown;  
	bool _iscapturemode;
	std::map<int,Keydata> _keyboard;
	std::stringstream _capturedtext;

public:
	//Set
	void pressKey(int key);
	void releaseKey(int key);

	//Query
	Point2d getMousePos();
	bool  keyPressed(int key);
	bool  keyReleased(int key);
	bool  keyHeld(int key);

	bool mouseMoved();

	void setMousePos(Point2d newpos);
	void update();
	bool isTextCaptureMode();
	void pasteClipboard();
	void insertCharacter(SDL_keysym keysym);
	void stopTextCapture();
	void startTextCapture();
	std::string getCapturedText();
	Controller();
};
