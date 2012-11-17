#include "teststate.h"

#define RENDERER graphics::Renderer::Instance()

void TestState::init(Controller &maincontrol)
{
  square = Quad(100,100,ColorRGBA(1,.5,.1,1));
  rot = 0.0;
  scale = 0.0;
  

  // Initialize the OpenAL library
  alutInit(0,0);

  cout << alutGetErrorString(alutGetError()) << endl;

  // Create sound buffer and source
  alGenBuffers(1, &bufferID);
  alGenSources(1, &sourceID);
  
  // Set the source and listener to the same location
  alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
  alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

  int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[AUDIO_BUFFER_SIZE];    // Local fixed size array
  FILE *f = 0;

  // Open for binary reading
 
  vorbis_info *pInfo;
  OggVorbis_File oggFile;

  ov_fopen("whenwillitend.ogg",&oggFile);

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
  alSourcePlay(sourceID);


  cout << "Loading shaders" << endl;
  regularshader = new ShaderProgram("regular.vert","regular.frag");
  cout << "Setting shader output" << endl;
  regularshader->setOutputSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  cout << "Enabling shader" << endl;

  int n;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
  cout << n << " is the number of shits you can have on your vertices" << endl;
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
	glClearColor(0.0,1.0,0.0,1.0);
    //alSourcef(sourceID, AL_PITCH, sin(t)*.5 + 1.0);
	rot += (sin(t)*.05 + .07);
	scale = ((sin(t)+2)/2);
}

void TestState::draw()
{
	for(int i = 0;i<100;i++)
	{
		square.setColor(ColorRGBA(.3,.2,.9,.005));
		RENDERER->drawQuad(&square,Point3d(250,250,0),scale+(i/100),scale+(i/100),rot+(i/10));
	}

	RENDERER->drawBuffer();
}

TestState::TestState()
{

}
