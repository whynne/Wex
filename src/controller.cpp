#include "controller.h"

Keydata::Keydata()
{
	state = unmapped;
}

Point2i Controller::getMousePos()
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

void Controller::updateMouse()
{
	SDL_GetMouseState(&_mouseposition.x,&_mouseposition.y);
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
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	_iscapturemode = true;
}

void Controller::backspace()
{
	std::string old = _capturedtext;
	_capturedtext = old.substr(0,old.length()-1);
}

void Controller::stopTextCapture()
{
	SDL_EnableKeyRepeat(0,0);
	_iscapturemode = false;
}

bool Controller::isTextCaptureMode()
{
	return _iscapturemode;
}

void Controller::insertCharacter(SDL_keysym keysym)
{
	char character = keysym.unicode & 0x7F;

	if(character >= 32 && character <= 127)
		_capturedtext += character;
	else if(character == 0x08 && _capturedtext.length() != 0)
	{
		std::string old = _capturedtext;
		_capturedtext = old.substr(0,old.length()-1);
	}
}

std::string Controller::getCapturedText()
{
	std::string result = _capturedtext;
	return result;
}

void Controller::flushText()
{
	_capturedtext = "";
}

void Controller::setKeyRepeat(bool v)
{
}

Controller::Controller()
{
	_mouseposition = Point2i(0,0);
	_mousemoved = false;
	_mousedown = false;
	_iscapturemode = false;
}