#pragma once

#include "vec.h"
#include "AL/al.h"
#include "AL/alut.h"
#include "vorbis/vorbisfile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define AUDIO_BUFFER_SIZE   32768     // 32 KB buffers

namespace audio
{
	void init();
	class AudioBuffer;
	class Source;

	class Source
	{
		ALuint id;
	public:
		Source();
		void   setPosition(Point3f position);
		void   attachBuffer(AudioBuffer buffer);
		ALuint getId();
		void   play();
	};
	
	class AudioBuffer
	{
	    ALuint id;
		ALenum format;
		ALsizei freq;

	public:
		AudioBuffer();
		ALuint getId();
		void   loadVorbisFile(std::string name);
	};

	extern std::map<std::string,AudioBuffer> audiobuffers;

}