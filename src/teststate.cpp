#include "teststate.h"

#define RENDERER graphics::Renderer::Instance()

void TestState::init(Controller &maincontrol)
{
  audio::Source source;
  audio::AudioBuffer buffer;
  buffer.loadVorbisFile("whenwillitend.ogg");
  source.attachBuffer(buffer);
  source.play();
  blanktexture.createEmptyTexture(100,100);
  RENDERER->changeTexture(blanktexture.getTexId());
  square = Quad(20,20,ColorRGBA(1,.5,.1,1));
  rot = 0.0;
  scale = 0.0;
  

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
	glClearColor(0.0,0.0,0.0,1.0);
    //alSourcef(sourceID, AL_PITCH, sin(t)*.5 + 1.0);
	rot -= (sin(t*6.7)*.05 + .07);
	scale = ((sin(t*6.7)+2)/10);
}

void TestState::draw()
{
	for(int i = 0;i<200;i++)
	{
		square.setColor(ColorRGBA(0,1,0,.01));
		RENDERER->drawQuad(&square,Point3d(250,250,0),scale+(i/10.0),scale+(i/10.0),rot+(i/(200.02*scale)));
	}

	RENDERER->drawBuffer();
}

TestState::TestState()
{

}
