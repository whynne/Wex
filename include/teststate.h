//This is a sort of scratch-pad state for testing things I'm learning

#pragma once

#define AUDIO_BUFFER_SIZE   32768     // 32 KB buffers


#include "freetype/freetype.h"
#include "AL/al.h"
#include "AL/alut.h"
#include "vorbis/vorbisfile.h"
#include "enginestate.h"
#include "console.h"
#include "vec.h"

class TestState : public EngineState
{
private:
	ShaderProgram regularshader;
	double rot;
	double xscale;
	double yscale;
	Quad  square;
	ALint state;                // The state of the sound source
    ALuint bufferID;            // The OpenAL sound buffer ID
    ALuint sourceID;            // The OpenAL sound source
    ALenum format;              // The sound data format
    ALsizei freq;               // The frequency of the sound data
    vector < char > bufferData; // The sound buffer data from file


public:
	Console testconsole;
	void init(Controller &maincontrol);
	void cleanup();
	void pause();
	void resume();
	void handleEvents();
	void update(double t,double dt);
	void draw();

	TestState();
};
