#include "teststate.h"

#define RENDERER graphics::Renderer::Instance()

void TestState::init(Controller &maincontrol)
{
  square = Quad(100,100,ColorRGBA(1,.5,.1,1));
  rot = 0.0;
  scale = 0.0;
  /*
  // Initialize the OpenAL library
  alutInit(0,0);

  cout << alutGetErrorString(alutGetError()) << endl;

  // Create sound buffer and source
  alGenBuffers(1, &bufferID);
  alGenSources(1, &sourceID);
  
  // Set the source and listener to the same location
  alListener3f(AL_POSITION, -10000.0f, -10000.0f, -10000.0f);
  alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

  int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[AUDIO_BUFFER_SIZE];    // Local fixed size array
  FILE *f = 0;

  // Open for binary reading
 
  vorbis_info *pInfo;
  OggVorbis_File oggFile;

  ov_fopen("time2learn.ogg",&oggFile);

   // Get some information about the OGG file
  pInfo = ov_info(&oggFile, -1);
  if(pInfo == 0)
	  return;

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
    bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
    // Append to end of buffer
    bufferData.insert(bufferData.end(), array, array + bytes);
  } while (bytes > 0);


  // Upload sound data to buffer
  alBufferData(bufferID, format, &bufferData[0], static_cast < ALsizei > (bufferData.size()), freq);

  // Attach sound buffer to source
  alSourcei(sourceID, AL_BUFFER, bufferID);

  




  // Finally, play the sound!!!
  //alSourcePlay(sourceID);
  */
  cout << "Loading shaders" << endl;
  regularshader = new ShaderProgram("regular.vert","regular.frag");
  cout << "Setting shader output" << endl;
  regularshader->setOutputSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  cout << "Enabling shader" << endl;
  regularshader->enable(true);
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
	glClearColor(0.0,0.0,0.0,0.0);
    //alSourcef(sourceID, AL_PITCH, sin(t)*.5 + 1.0);
	rot += (sin(t)*.05 + .07);
	//square.setColor(ColorRGBA(abs((sin(t*5)+2)/3),.5,abs(-(sin(t*5)+2)/3),.0001));
	scale = ((sin(t)+2)/2);
}

void TestState::draw()
{
	RENDERER->drawQuad(&square,Point3d(250,250,0),scale,scale,rot);
    square.setColor(ColorRGBA(abs(cos(rot/4.663)),abs(sin(rot/3.545)),abs(sin(-rot/5.623)),1));
	RENDERER->drawBuffer();
}

TestState::TestState()
{

}
