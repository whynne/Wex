#include "graphics.h"

using namespace graphics;

Renderer* Renderer::instance = NULL;

GLubyte uncompressedtgaheader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte compressedtgaheader[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

 map<string,graphics::Texture>        textures;
 map<string,graphics::SpriteSheet>    spritesheets;
 map<string,graphics::ShaderProgram>  shaders;

/*=====================
SpriteSheet definitions
=====================*/

SpriteSheet::SpriteSheet()
{
	
}

SpriteFrame SpriteSheet::getFrame(string sequence,unsigned int framenum)
{
	return sequences[sequence][framenum];
}

unsigned int SpriteSheet::getSequenceLength(string name)
{
	return sequences.at(name).size();
}

void SpriteSheet::setFrame(string sequence,unsigned int framenum,SpriteFrame frame)
{

}

/*=====================

Quad definitions

=====================*/
Quad::Quad()
{
	this->height = 10;
	this->width = 10;
	this->color = ColorRGBA(1,1,1,1);
}

Quad::Quad(double height,double width,ColorRGBA color)
{
	this->height = height;
	this->width = width;
	this->color = color;
}

ColorRGBA Quad::getColor()
{
	return color;
}

void Quad::setColor(ColorRGBA color)
{
	this->color = color;
}

double Quad::getHeight()
{
	return height;
}

void  Quad::setHeight(double height)
{
	this->height = height;
}

double Quad::getWidth()
{
	return width;
}

void  Quad::setWidth(double width)
{
	this->width = width;
}

Point3d Quad::getMidPoint()
{
	return Point3d(width/2,height/2,0);
}

/*=====================

Sprite definitions

=====================*/

ColorRGBA::ColorRGBA()
{
	_r = 0.0;
	_g = 0.0;
	_b = 0.0;
	_a = 0.0;
}

ColorRGBA::ColorRGBA(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

Sprite::Sprite()
{
	//Default sprite is degenerate quad.
	height = 0;
	width = 0;
	targetspritesheet = 0;
	frame = 0;
	accumulator = 0.0;
	frametime = 0.0;
}

void Sprite::changeSequence(string sequence)
{
	this->sequence = sequence;
	this->sequencelength = targetspritesheet->getSequenceLength(sequence);
}

void Sprite::changeSpriteSheet(SpriteSheet *newsprite)
{
	if (targetspritesheet != newsprite)
	{
		targetspritesheet = newsprite;
		targetspritesheet = 0;
		accumulator = 0;
	}
}

void Sprite::changeSpriteSheetNoRewind(SpriteSheet *newsprite)
{
	if (targetspritesheet != newsprite)
	{
		targetspritesheet = newsprite;
	}
}

void Sprite::play(double deltatime)
{
	accumulator += deltatime;
	if(accumulator > frametime)
	{
		frame+=1;
		accumulator = 0;
	}
	if(frame > sequencelength)
		targetspritesheet = 0;
}

void Sprite::rewind()
{
	frame = 0;
	accumulator = 0;
}

Point3d Sprite::getOffset()
{
	return offset;
}

SpriteFrame Sprite::getSpriteFrame()
{
	return targetspritesheet->getFrame(sequence,frame);
}

void Sprite::setSpriteFrame(unsigned int frame)
{
	
}

SpriteSheet* Sprite::getSpriteSheet()
{
	return targetspritesheet;
}

/*=====================
Font Definitions
=====================*/

BitmapFont::BitmapFont()
{
}

BitmapFont::BitmapFont(Texture& texture)
{
	buildFont(texture);
}

void BitmapFont::buildFont(Texture& texture)
{
	int currentchar = 0;
	for(int row = 0; row < 16; row++)
	{
		for(int column = 0; column < 16; column++)
		{
			//_charactersprites[currentchar] = graphics::SpriteSheet(texture,1,texture.getHeight()/16,texture.getWidth()/16);
			//_charactersprites[currentchar].setClip(0,column,row);
			currentchar++;
		}
	}
}


/*=====================
Texture Definitions
=====================*/

bool Texture::loadUncompressedTGA(char *filename)
{
	//Headers for the tga texture
	GLubyte header[12];
	GLubyte header2[6];
	
	//These are byte arrays to be compared to the header of our TGA file
	GLubyte uncompressedheader[12];
	GLubyte compressedheader[12];

	GLubyte* imagedata;		        //Pointer for storing image data
	GLuint bpp;                     //Bits per pixel
	GLuint type;                    //Data stored in * imagedata (GL_RGB or GL_RGBA)
	GLuint bytesperpixel;           //BYTES per pixel
	GLuint imagesize;               //Size in bytes of texture

	ifstream texturestream;
	texturestream.open(filename,ios::binary);
	if(texturestream.good())
		cout << "Successfully opened" << endl;
	else
		cout << "FAILED TO OPEN" << endl;
	texturestream.read((char*)header,sizeof(header));

	texturestream.read((char*)header2, sizeof(header2));				//read 6 bytes into the file to get important information
	this->width  = (GLuint)header2[1] * 256 + (GLuint)header2[0];				//read and calculate width and save
	this->height = (GLuint)header2[3] * 256 + (GLuint)header2[2];				//read and calculate height and save
	bpp    = (GLuint)header2[4];										//read bpp and save

	if((this->width <= 0) || (this->height <= 0) || ((bpp != 24) && (bpp !=32)))	//check to make sure the height, width, and bpp are valid
		return false;
	if(bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;
	imagesize = ((bpp/8) * this->width * this->height);										//determine size in bytes of the image
	imagedata = new GLubyte[imagesize];											//allocate memory for our imagedata variable
	texturestream.read((char*)imagedata,imagesize);								//read according the the size of the image and save into imagedata 

	for(GLuint cswap = 0; cswap < (GLuint)imagesize; cswap += (bpp/8))	        //loop through and reverse the tga's BGR format to RGB
	{
		imagedata[cswap] ^= imagedata[cswap+2] ^=				                //1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte
	    imagedata[cswap] ^= imagedata[cswap+2];
	}

	texturestream.close();														//close ifstream because we're done with it

	glGenTextures(1, &this->texid);													// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, this->texid);										

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, type, this->width, this->height, 0, type, GL_UNSIGNED_BYTE, imagedata);
    delete imagedata;
	return true;
}

void Texture::createEmptyTexture(int height,int width)
{
	GLubyte* imagedata;		        //Pointer for storing image data
	imagedata = new GLubyte[height*width*4];
	memset(imagedata,255,height*width*4);
	glGenTextures(1, &this->texid);													// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, this->texid);										

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);
	delete imagedata;
	this->height = height;
	this->width = width;
	cout << "Created " << width << "x" << height << " empty texture with id " << this->texid << endl;
}

GLuint Texture::getHeight()
{
	return height;
}

GLuint Texture::getWidth()
{
	return width;
}

GLuint Texture::getTexId()
{
	return texid;
}

Texture::Texture(){

}


/*=====================
SpriteSheet Batcher definitions
=====================*/

void SpriteBatch::addToBuffer(Quad* quad,Point3d position,double xscale,double yscale,double rotate)
{
	//Get initial points
	vertexbuffer[bufferpos+0].x = 0;
	vertexbuffer[bufferpos+0].y = 0;
	vertexbuffer[bufferpos+0].z = 0;

	vertexbuffer[bufferpos+1].x = quad->getWidth();
	vertexbuffer[bufferpos+1].y = 0;
	vertexbuffer[bufferpos+1].z = 0;

	vertexbuffer[bufferpos+2].x = quad->getWidth();
	vertexbuffer[bufferpos+2].y = quad->getHeight();
	vertexbuffer[bufferpos+2].z = 0;

	vertexbuffer[bufferpos+3].x = 0;
	vertexbuffer[bufferpos+3].y = quad->getHeight();
	vertexbuffer[bufferpos+3].z = 0;

	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] - quad->getMidPoint();
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] - quad->getMidPoint();
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] - quad->getMidPoint();
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] - quad->getMidPoint();
	
    //Scale
	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] * Point3d(xscale,yscale,0);

	//Rotate about center
	vertexbuffer[bufferpos+0].rotate(Point3d(0,0,0),rotate);
	vertexbuffer[bufferpos+1].rotate(Point3d(0,0,0),rotate);
	vertexbuffer[bufferpos+2].rotate(Point3d(0,0,0),rotate);
	vertexbuffer[bufferpos+3].rotate(Point3d(0,0,0),rotate);
	
	
	
	//Translate
	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] + position;
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] + position;
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] + position;
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] + position;
	
	

	//Color
	colorbuffer[bufferpos+0] = quad->getColor();
	colorbuffer[bufferpos+1] = quad->getColor();
	colorbuffer[bufferpos+2] = quad->getColor();
	colorbuffer[bufferpos+3] = quad->getColor();

	bufferpos += 4;
}

void SpriteBatch::addToBuffer(Sprite* sprite,Point3d position,double xscale,double yscale,double rotate)
{

	if (sprite == 0 || sprite->getSpriteSheet() == 0)
	{
		cout << "Error: Attempting to draw sprite from a null pointer!" << endl;
		return;
	}

	const TexCoord topleft     = sprite->getSpriteFrame().texcoords[0];
	const TexCoord topright    = sprite->getSpriteFrame().texcoords[1];
	const TexCoord bottomright = sprite->getSpriteFrame().texcoords[2];
	const TexCoord bottomleft  = sprite->getSpriteFrame().texcoords[3];
	const double   height = sprite->getSpriteSheet()->getHeight();
	const double   width  = sprite->getSpriteSheet()->getWidth();

		//Get initial points
	vertexbuffer[bufferpos+0] = Point3d(0,0,0);
	vertexbuffer[bufferpos+1] = Point3d(sprite->getWidth(),0,0);
	vertexbuffer[bufferpos+2] = Point3d(sprite->getWidth(),sprite->getHeight(),0);
	vertexbuffer[bufferpos+3] = Point3d(0,sprite->getHeight(),0);

	//Rotate about center
	vertexbuffer[bufferpos+0].rotate(sprite->getMidPoint(),rotate);
	vertexbuffer[bufferpos+1].rotate(sprite->getMidPoint(),rotate);
	vertexbuffer[bufferpos+2].rotate(sprite->getMidPoint(),rotate);
	vertexbuffer[bufferpos+3].rotate(sprite->getMidPoint(),rotate);
	
    //Scale
	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] * Point3d(xscale,yscale,0);
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] * Point3d(xscale,yscale,0);

	//Translate
	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] + position;
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] + position;
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] + position;
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] + position;
	
	//Color
	colorbuffer[bufferpos+0] = sprite->getColor();
	colorbuffer[bufferpos+1] = sprite->getColor();
	colorbuffer[bufferpos+2] = sprite->getColor();
	colorbuffer[bufferpos+3] = sprite->getColor();

	bufferpos += 4;
}

Point3d* SpriteBatch::getVertbuffer()
{
	return vertexbuffer;
}

TexCoord* SpriteBatch::getTexCoordBuffer()
{
	return texcoordbuffer;
}

ColorRGBA* SpriteBatch::getColorBuffer()
{
	return colorbuffer;
}

GLint SpriteBatch::getBufferLength()
{
	return bufferpos;
}

void SpriteBatch::reset()
{
	bufferpos = 0;  
}

bool SpriteBatch::isFull()
{
	if(BUFFER_SIZE-bufferpos < 4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

SpriteBatch::SpriteBatch()
{
	bufferpos = 0;
}

/*=====================
Shader definitions
=====================*/


	const char* graphics::SAMPLER_NAME = "fs_Sampler0";
	const char* graphics::TFS_TEXCOORD = "fs_TexCoord";
	const char* graphics::TFS_COLOR    = "fs_Color";

	const char* graphics::TVS_PROJECTION_MAT = "vs_Projection";
	const char* graphics::TVS_MODEL_VIEW_MAT = "vs_ModelView";

	const int   graphics::VERTEX_ATTRIBUTE_ID = 2;
	const char* graphics::VERTEX_ATTRIBUTE_NAME = "vs_Vertex";

	const int   graphics::TEXCOORD_ATTRIBUTE_ID = 1;
	const char* graphics::TEXCOORD_ATTRIBUTE_NAME = "vs_TexCoord";

	const int   graphics::COLOR_ATTRIBUTE_ID   = 0;
	const char* graphics::COLOR_ATTRIBUTE_NAME = "vs_Color";

	Shader::Shader(string filename,GLenum type) 
	{
		this->loadFromFile(filename,type);
	}

	Shader::Shader() 
	{

	}

	void Shader::loadFromFile(std::string filename,GLenum type)
	{
		std::ifstream file;
		unsigned int sourcelength;
		
		//Status variables
		GLint compiled;
	
		//Open vertex file, read into a string
	
		file.open(filename,ios::binary|ios_base::ate);
		sourcelength = file.tellg();
		source = new GLchar[sourcelength+1];
		file.seekg(ios::beg);
		unsigned int i = 0;
		while(file.good())
		{
			source[i] = file.get();
			if(!file.eof())
				i++;
		}
		source[sourcelength] = '\0';
		
		//Create a handle for our shader, then compile.
	
		handle = glCreateShader(type);

		glShaderSource(handle, 1, (const GLchar**)&source, NULL);
		glCompileShader(handle);
		
		glGetObjectParameterivARB(handle, GL_COMPILE_STATUS, &compiled);

		if(compiled == GL_TRUE)
			cout << "Shader " << filename << " compiled successfully!" << endl;
		GLint blen = 0;	
		GLsizei slen = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH , &blen);    
		
		//If any errors, display log
		if(blen > 1)
		{
		    GLchar* compiler_log = new GLchar[blen];	
		    glGetInfoLogARB(handle, blen, &slen, compiler_log);
		    cout << "compiler_log:" << endl;
		    cout << compiler_log;
		    delete[] compiler_log;
		}
	}

	Shader::~Shader() 
	{
		cout << "Tell me this isn't happening" << endl;
		if (handle) glDeleteShader(handle);
	}
	ShaderProgram::ShaderProgram()
	{
	}

	ShaderProgram::ShaderProgram(std::string vsfilename,std::string fsfilename) {
		GLint linked = 100;
		vs = new Shader(vsfilename,GL_VERTEX_SHADER);
		fs = new Shader(fsfilename,GL_FRAGMENT_SHADER);
		handle = glCreateProgram();
		glAttachShader(handle, vs->GetHandle());
		glAttachShader(handle, fs->GetHandle());
		glLinkProgram(handle);
		this->bindAttributes();
		this->getUniformHandles();

		glGetProgramiv(handle, GL_LINK_STATUS, &linked);
		cout << "Program handle " << handle << endl;
        if (linked == GL_TRUE)
            cout << "Shader program successfully linked! " << endl;

	}

	ShaderProgram::~ShaderProgram() {
		cout << "What the fuck" << endl;
		if (vs) delete vs;
		if (fs) delete fs;
		if (handle) glDeleteProgram(handle);
	}

	void ShaderProgram::enable(bool state) {
		if (state) {
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex_handle);
			glUseProgram(handle);

			Mat4 identity;
			Mat4::Identity(identity);

			Mat4 ortho;
			Mat4::Ortho(ortho, 0, 0, (float)width, (float)height, 0, 1);

			Mat4 proj_matrix;
			identity.Multiply(proj_matrix, ortho);
			
			// The input texture should be on sampler 0.
			glUniform1i(shader_tex_handle, GL_TEXTURE0);
			glUniformMatrix4fv(shader_mat_proj_handle, 1, false, (float*)&proj_matrix);
			glUniformMatrix4fv(shader_mat_mv_handle, 1, false, (float*)&identity);
		} else {
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	}

    void ShaderProgram::getUniformHandles() {
		glUseProgram(handle);
		shader_mat_proj_handle = glGetUniformLocation(handle, TVS_PROJECTION_MAT);
		shader_tex_handle      = glGetUniformLocation(handle, SAMPLER_NAME);
		shader_mat_mv_handle   = glGetUniformLocation(handle, TVS_MODEL_VIEW_MAT);
	}

    void ShaderProgram::bindAttributes() {
		cout << "Location of vertex attribute: " << glGetAttribLocation(handle,VERTEX_ATTRIBUTE_NAME) << endl;
		cout << "Location of texcoord attribute: " << glGetAttribLocation(handle,TEXCOORD_ATTRIBUTE_NAME) << endl;
		cout << "Location of color attribute: " << glGetAttribLocation(handle,COLOR_ATTRIBUTE_NAME) << endl;
		
	}
	/*
	void VertexShader::Enable(bool state) 
	{

	}
	*/




/*=====================
Renderer definitions
=====================*/

bool graphics::Init()
{
	 if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )					 // Initialize SDL
        return false;
    while(true)
    {
        if(SDL_WasInit(SDL_INIT_VIDEO) != 0)
            break;
        else
          cout << "Still not initialized!" << endl;
    }
    SDL_WM_SetCaption( "Wex", NULL );
    SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,SDL_SWSURFACE);

    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )	// Set window properties, OpenGL is passed here
    {
        return false;
    }
    
    cout << "Video mode set!" << endl;

    SDL_EnableUNICODE(1);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );				// Activate double buffer for buffer switching
    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );				// Activate swap control, also for buffer switching
	
	glClearColor( 0.0,1.0,0.0,1.0 ); 

                                                     // Set clear color.  This is what the buffer gets filled with when we call glClear
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glViewport(0,0,SCREEN_HEIGHT,SCREEN_WIDTH);
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
        std::cerr << "Error: " << glewGetErrorString(err) << endl;

    if (GLEW_VERSION_1_3)
        cout << "OpenGL 1.3 Supported!" << endl;
    if (GLEW_VERSION_1_4)
        cout << "OpenGL 1.4 Supported!" << endl;
	if (GLEW_VERSION_1_5)
        cout << "OpenGL 1.5 Supported!" << endl;
	if (GLEW_VERSION_2_0)
        cout << "OpenGL 2.0 Supported!" << endl;
    if (GLEW_VERSION_2_1)
        cout << "OpenGL 2.1 Supported!" << endl;
	if (GLEW_VERSION_3_0)
        cout << "OpenGL 3.0 Supported!" << endl;
	if (GLEW_VERSION_3_1)
        cout << "OpenGL 3.1 Supported!" << endl;
    if (GLEW_VERSION_3_2)
        cout << "OpenGL 3.2 Supported!" << endl;
	if (GLEW_VERSION_3_3)
        cout << "OpenGL 3.3 Supported!" << endl;
    if (GLEW_VERSION_4_0)
        cout << "OpenGL 4.0 Supported!" << endl;
    if (GLEW_VERSION_4_1)
        cout << "OpenGL 4.1 Supported!" << endl;

    if (glewIsSupported("GL_ARB_fragment_program"))
        cout << "Fragment programs supported" << endl;
    if (glewIsSupported("GL_ARB_vertex_program"))
        cout << "Vertex programs supported" << endl;
	if (glewIsSupported("GL_ARB_vertex_buffer_object"))
		cout << "VBOs supported" << endl;
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
}

Renderer::Renderer()
{
		glEnable(GL_TEXTURE_2D);

		glGenBuffers( 1, &vbovertex);
		glGenBuffers( 1, &vbotexture);
		glGenBuffers( 1, &vbocolor);

		defaultshader = new ShaderProgram("regular.vert","regular.frag");
		defaultshader->setOutputSize(SCREEN_WIDTH,SCREEN_HEIGHT);
		defaultshader->enable(true);
	    int vertexattriboffset = glGetAttribLocation(defaultshader->getHandle(),VERTEX_ATTRIBUTE_NAME);
		int texcoordattriboffset = glGetAttribLocation(defaultshader->getHandle(),TEXCOORD_ATTRIBUTE_NAME);;
		int colorattriboffset = glGetAttribLocation(defaultshader->getHandle(),COLOR_ATTRIBUTE_NAME);;



		glEnableVertexAttribArray(vertexattriboffset);
		glBindBuffer(GL_ARRAY_BUFFER,vbovertex);
		glVertexAttribPointer(vertexattriboffset,3, GL_DOUBLE,0,0,0);

		glEnableVertexAttribArray(texcoordattriboffset);
		glBindBuffer(GL_ARRAY_BUFFER,vbotexture);
		glVertexAttribPointer(texcoordattriboffset,2, GL_DOUBLE, 0,0,0);

		glEnableVertexAttribArray(colorattriboffset);
		glBindBuffer(GL_ARRAY_BUFFER,vbocolor);
		glVertexAttribPointer(colorattriboffset,4,GL_FLOAT,GL_TRUE,0,0);
}

void Renderer::drawSprite(Sprite* animation,Point3d position,double xscale,double yscale,double rotate)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(animation,position-view,xscale,yscale,rotate);
	}
	else
		spritebatch.addToBuffer(animation,position-view,xscale,yscale,rotate);
}

void Renderer::drawSprite(Sprite* animation,Point3d position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(animation,position-view,1.0,1.0,0);
	}
	else
		spritebatch.addToBuffer(animation,position-view,1.0,1.0,0);
}

void Renderer::drawFixedSprite(Sprite* animation,Point3d position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(animation,position,1.0,1.0,0);
	}
	else
		spritebatch.addToBuffer(animation,position,1.0,1.0,0);
}

void Renderer::drawFixedSprite(Sprite* animation,Point3d position,double xscale,double yscale,double rotate)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(animation,position,xscale,yscale,rotate);
	}
	else
		spritebatch.addToBuffer(animation,position,xscale,yscale,rotate);
}

void Renderer::drawQuad(Quad* quad,Point3d position,double xscale,double yscale,double rotate)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(quad,position,xscale,yscale,rotate);
	}
	else
		spritebatch.addToBuffer(quad,position,xscale,yscale,rotate);
}


void Renderer::drawBuffer()
{

	if(spritebatch.getBufferLength() > 0)
	{	
		
		glBindBuffer( GL_ARRAY_BUFFER, vbovertex);
		glBufferData( GL_ARRAY_BUFFER,sizeof(Point3d)*spritebatch.getBufferLength(),spritebatch.getVertbuffer(),GL_STREAM_DRAW);
		
		glBindBuffer( GL_ARRAY_BUFFER, vbotexture);
		glBufferData( GL_ARRAY_BUFFER,sizeof(TexCoord)*spritebatch.getBufferLength(),spritebatch.getTexCoordBuffer(),GL_STREAM_DRAW);
		
		glBindBuffer( GL_ARRAY_BUFFER, vbocolor);
		glBufferData( GL_ARRAY_BUFFER,sizeof(ColorRGBA)*spritebatch.getBufferLength(),spritebatch.getColorBuffer(),GL_STREAM_DRAW);
		
		glDrawArrays(GL_QUADS,0,spritebatch.getBufferLength());
		glBindBuffer(GL_ARRAY_BUFFER,0);
		spritebatch.reset();
	}
}

void Renderer::drawText(std::string text,Point3d position, GLint space)
{
	/*
	Sprite character(16,16);
	for(int i = 0; i < text.size(); i++)
	{
		character.changeSpriteSheet(&fonts[0]._charactersprites[(int)text[i]]);

		drawFixed(&character,Point3d(position.x+(i*space),position.y,0));
	}
	*/
}
void Renderer::moveCameraTowards(Point3d position)
{
	if (view.x < position.x) // camera is deeper than current position
		view.x += (position.x-view.x)/8;
	if (view.x > position.x) //camera is higher than current position
		view.x -= (view.x-position.x)/32;

	if (view.y < position.y) // camera is deeper than current position
		view.y += (position.y-view.y)/8;
	if (view.y > position.y) //camera is higher than current position
		view.y -= (view.y-position.y)/32;
}

void Renderer::changeTexture(int texhandle)
{
	glBindTexture(GL_TEXTURE_2D,texhandle);
}

Renderer* Renderer::Instance()
{
	if(!instance)
		instance = new Renderer();
	return instance;
}

