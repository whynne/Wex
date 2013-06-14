#pragma once

#include "begincode.h"

#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include "SDL/SDL.h"
#include "vec.h"

using wex::Point2i;

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


class WEXAPI Controller
{
private:
	Point2i _prevmousepos;
	Point2i _mouseposition;
	bool _mousemoved;
	bool _mousedown;  
	bool _iscapturemode;
	std::map<int,Keydata> _keyboard;
	std::string _capturedtext;

public:
	//Set
	void pressKey(int key);
	void releaseKey(int key);

	//Query
	Point2i getMousePos();
	bool  keyPressed(int key);
	bool  keyReleased(int key);
	bool  keyHeld(int key);

	bool mouseMoved();

	void backspace();
	void flushText();
	void updateMouse();
	void update();
	bool isTextCaptureMode();
	void pasteClipboard();
	void insertCharacter(SDL_keysym keysym);
	void stopTextCapture();
	void startTextCapture();
	void setKeyRepeat(bool v);
	std::string getCapturedText();
	Controller();
};

#include "closecode.h"