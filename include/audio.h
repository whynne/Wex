

#pragma once

#include "begincode.h"

#include "vec.h"
#include "AL/al.h"
#include "AL/alut.h"
#include "vorbis/vorbisfile.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>


#define NUM_SOURCES 31
#define AUDIO_BUFFER_SIZE   32768     // 32 KB buffers
#define soundmanager wex::audio::SoundManager::Instance()

using std::string;

namespace wex
{
	namespace audio
	{
		void init();
		class SoundManager;
		class AudioBuffer;
		class Source;

		
		class WEXAPI SoundManager
		{
		private:
			static SoundManager* instance;
			std::vector<Source> sources;
			std::map<std::string,AudioBuffer> audiobuffers;
		    SoundManager();
	        SoundManager& operator=(SoundManager const&){return *this;};
		public:
			void playSound(string name);  //For now, if you call playSound and there are no available sources, then it will simply be dropped.
			void loadSound(string filename);
			void update();
			static SoundManager* Instance();
		};

		class WEXAPI Source
		{
			ALuint id;
		public:
			Source();
			void   setPosition(Point3f position);
			void   attachBuffer(AudioBuffer buffer);
			ALuint getId();
			void   play();
			void   pause();
			bool   isPlaying();
			bool   isPaused();
		};
		
		class WEXAPI AudioBuffer
		{
		    ALuint id;
			ALenum format;
			ALsizei freq;
	
		public:
			AudioBuffer();
			ALuint getId();
			void   loadVorbisFile(std::string name);
		};
	}
}

#include "closecode.h"