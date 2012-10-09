#include "controller.h"

Keydata::Keydata()
{
	state = unmapped;
}

Point2d Controller::getMousePos()
{
	return _mouseposition;
}

void Controller::pressKey(int key)
{
	_keyboard[key].state = pressed;
}

void Controller::releaseKey(int key)
{
	_keyboard[key].state = released;
}

bool Controller::keyHeld(int key)
{
	if(_keyboard[key].state == held)
	{
		return true;
	}
	    return false;
}

bool Controller::keyPressed(int key)
{
	if(_keyboard[key].state == pressed)
	{
		return true;
	}
	else
		return false;
}

bool Controller::keyReleased(int key)
{
	if(_keyboard[key].state == released)
	{
		return true;
	}
	else
		return false;
}

void Controller::setMousePos(Point2d newpos)
{
	_mouseposition = newpos;
}

void Controller::update()
{
	if(_keyboard.empty())
	{
		return;
	}

	std::map<int,Keydata>::iterator it = _keyboard.begin();

	
	while(it != _keyboard.end())
	{
		if(it->second.state == pressed )
		{
			it->second.state = held;
		}
		if(it->second.state == released )
		{
			it->second.state = unmapped;
		}
		++it;
	}
	
}

void Controller::startTextCapture()
{
	_iscapturemode = true;
}

void Controller::stopTextCapture()
{
	_iscapturemode = false;
	_capturedtext.str("");
}

bool Controller::isTextCaptureMode()
{
	return _iscapturemode;
}

void Controller::insertCharacter(SDL_keysym keysym)
{
	long pos = _capturedtext.tellp();
	char character = keysym.unicode & 0x7F;

	if(character >= 32 && character <= 127)
		_capturedtext << character;
	else if(character == 0x08 && pos != 0)
	{
		_capturedtext.seekp(pos-1);
		_capturedtext.write(" ",1);
		_capturedtext.seekp(pos-1);
	}
}

std::string Controller::getCapturedText()
{
	return _capturedtext.str();
}

Controller::Controller()
{
	_mouseposition = Point2d(0.0,0.0);
	_mousemoved = false;
	_mousedown = false;
	_iscapturemode = false;
}