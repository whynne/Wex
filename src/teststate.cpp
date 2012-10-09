#include "teststate.h"

#define RENDERER graphics::Renderer::Instance()

void TestState::init(Controller &maincontrol)
{
	testconsole.print("Hello, world!");
}

void TestState::cleanup()
{

}

void TestState::pause()
{

}

void TestState::resume()
{

}

void TestState::handleEvents()
{

}

void TestState::update(double t,double dt)
{
	
}

void TestState::draw()
{
	testconsole.draw();
}

TestState::TestState()
{

}
