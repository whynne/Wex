#include "engine.h"

#define SCREEN_HEIGHT 768	
#define SCREEN_WIDTH  768
#define SCREEN_BPP 32

using namespace std;

    
GameEngine::GameEngine()
{
}

bool GameEngine::init()
{
    AllocConsole();
    freopen( "CON", "w", stdout ); 

    //Game initializations
    srand(time(NULL));
    running = true;

    //SDL/OpenGL initializations

    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )					 // Initialize SDL
    {
        return false;
    }
    while(true)
    {
        if(SDL_WasInit(SDL_INIT_VIDEO) != 0)
            break;
        else
          cout << "Still not initialized!" << endl;
    }
    SDL_WM_SetCaption( "Dicktris", NULL );


    glClearColor( 0.0, 0.0, 0.0, 0.0 ); 
    SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,SDL_SWSURFACE);

    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )	// Set window properties, OpenGL is passed here
    {
        return false;
    }
    
    cout << "Video mode set!" << endl;

    SDL_EnableUNICODE(1);									    // Enable unicode for translating keypresses into printable characters

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );				// Activate double buffer for buffer switching
    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );				// Activate swap control, also for buffer switching

                                                                // Set clear color.  This is what the buffer gets filled with when we call glClear
    glClear(GL_COLOR_BUFFER_BIT);
    
    SDL_GL_SwapBuffers();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode( GL_PROJECTION );							    // Set the matrix mode to projection; controls how screen is displayed
    glLoadIdentity();											// Load the matrix identity for the projection matrix.
    glScalef(1.0/256.0, -1.0/256.0, 1.0/256.0);
    glTranslatef(-256.0,-256.0,0.0);
                                                            // to change the coordinate system.  it is currently cartesian. y+ goes up, y- down
    glMatrixMode( GL_MODELVIEW );								// Set matrix mode to modelview; controls models that are shown on screen.

    glEnable (GL_BLEND);										//Enable blending
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);         //Set blending for our alpha-enabled textures
                                                      
    if( glGetError() != GL_NO_ERROR )						    // If we get any errors, return false.
    {
        return false;    
    }

    //Load resources here !!!

    //Load extensions with GLEW

    GLenum err = glewInit();

    if(GLEW_OK != err)
        cerr << "Error: " << glewGetErrorString(err) << endl;

    if (GLEW_VERSION_1_3)
        cout << "OpenGL 1.3 Supported!" << endl;
    if (GLEW_VERSION_1_4)
        cout << "OpenGL 1.4 Supported!" << endl;
    if (glewIsSupported("GL_ARB_fragment_program"))
        cout << "Fragment programs supported" << endl;
    if (glewIsSupported("GL_ARB_vertex_program"))
        cout << "Vertex programs supported" << endl;
    if (glewIsSupported("GL_ARB_shading_language_100")) 
    {  
       int major, minor, revision;
       const GLubyte* sVersion = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
       if (glGetError() == GL_INVALID_ENUM)
       {
          major = 1; minor = 0; revision=51;
       }
       else
       {
          cout << "Using shading language: " << sVersion << endl;
       }
    }
    return true;
}

void GameEngine::handleEvents(GameEngine *engine)
{
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN)
        {
            _maincontrol.pressKey(event.key.keysym.sym);

            if(_maincontrol.isTextCaptureMode())
            {
                _maincontrol.insertCharacter(event.key.keysym);
            }
        }
        else if (event.type == SDL_KEYUP)
            _maincontrol.releaseKey(event.key.keysym.sym);	
        else if ( event.type == SDL_MOUSEMOTION ) 
        {
            
        }
    }
    statestack.back()->handleEvents();
    _maincontrol.update();
}

Controller& GameEngine::getUserController()
{
    return _maincontrol;
}


void GameEngine::draw(GameEngine *engine)
{
  glClearDepth(1.0f);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  if(!statestack.empty())
    statestack.front()->draw();
  else
    return;
  SDL_GL_SwapBuffers();
}

void GameEngine::update(GameEngine *engine,double t,double dt)
{
    statestack.back()->update(t,dt);
}

void GameEngine::changeState(EngineState *state)
{
    if(statestack.back())
    {
        statestack.pop_back();
    }

}

void GameEngine::pushState(EngineState *state)
{
    statestack.push_back(state);
}

void GameEngine::popState(EngineState *state)
{
}

bool GameEngine::isRunning()
{
    return running;
}