#include "audio.h"

using namespace wex;
using namespace audio;

using std::cout;
using std::endl;
using std::string;


SoundManager* SoundManager::instance;

void SoundManager::loadSound(string filename)
{
	AudioBuffer newsound;
	newsound.loadVorbisFile(filename);
	audiobuffers[filename] = newsound;  
	cout << "Successfully loaded" << filename << endl;
}

SoundManager::SoundManager()
{
	for(int i = 0;i < NUM_SOURCES;i++)
	{
		Source temp;
		sources.push_back(temp);
	}
}

void SoundManager::playSound(string name)
{
	//find free sound source
	std::vector<Source>::iterator it;
	for(it = sources.begin();it!=sources.end();it++)
	{
		if(it->isPlaying() == false)
		{
			it->attachBuffer(audiobuffers[name]);
			it->play();
			cout << "Playing " << name << "..." << endl;
			return;
		}

	}
	cout << "Could not find free sources to play" << endl;
}

SoundManager* SoundManager::Instance()
{
	if(!instance)
		instance = new SoundManager();
	return instance;
}


void audio::init()
{
  // Initialize the OpenAL library
  alutInit(0,0);
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  std::cout << alutGetErrorString(alutGetError()) << std::endl;
}

AudioBuffer::AudioBuffer()
{
	int error;
	alGenBuffers(1,&id);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alGenBuffers failed: %d", error);
	}
}

ALuint AudioBuffer::getId()
{
	return id;
}

void AudioBuffer::loadVorbisFile(std::string name)
{
  std::vector<char> bufferData;
  int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[AUDIO_BUFFER_SIZE];    // Local fixed size array
  FILE *f = 0;

  // Open for binary reading
 
  vorbis_info *pInfo;
  OggVorbis_File oggFile;

  ov_fopen(name.c_str(),&oggFile);

   // Get some information about the OGG file
  pInfo = ov_info(&oggFile, -1);
  if(pInfo == 0)
  {
	  cout << "Warning: Could not retrieve information from file " << name << endl;
	  return;
  }

  // Check the number of channels... always use 16-bit samples
  if (pInfo->channels == 1)
    format = AL_FORMAT_MONO16;
  else
    format = AL_FORMAT_STEREO16;
  // end if

  // The frequency of the sampling rate
  freq = pInfo->rate;

    do {
    // Read up to a buffer's worth of decoded sound data
    bytes = ov_read(&oggFile, array, AUDIO_BUFFER_SIZE, endian, 2, 1, &bitStream);
    // Append to end of buffer
    bufferData.insert(bufferData.end(), array, array + bytes);
  } while (bytes > 0);


  // Upload sound data to buffer
  alBufferData(id, format, &bufferData[0], static_cast < ALsizei > (bufferData.size()), freq);
}

void Source::attachBuffer(AudioBuffer buffer)
{
	alSourcei(id, AL_BUFFER, buffer.getId());
}

ALuint Source::getId()
{
	return id;
}

Source::Source()
{
	alGenSources(1, &id);
	int error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("alGenSources failed: %d", error);
	}
	alSourceStop(id);
    alSource3f(id, AL_POSITION, 0.0f, 0.0f, 0.0f);
}

bool Source::isPlaying()
{
	int state;
    alGetSourcei(id,AL_SOURCE_STATE,&state);
	cout << state << endl;
	return (state == AL_PLAYING);
}

bool Source::isPaused()
{
	int state;
    alGetSourcei(id,AL_SOURCE_STATE,&state);
	return (state == AL_PAUSED);
}

void Source::play()
{
	alSourcePlay(id);
}

void Source::pause()
{
	alSourcePause(id);
}


void audio::Source::setPosition(Point3f position)
{
	alSource3f(id,AL_POSITION,position.x,position.y,position.z);
}

