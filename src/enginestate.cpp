#include "EngineState.h"

#define Renderer Renderer::Instance()

void EngineState::init(Controller &maincontrol)
{
	_maincontrol = &maincontrol;
}