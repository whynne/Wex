#include "graphics.h"

using namespace graphics;

Renderer* Renderer::_singletonRenderer = NULL;

GLubyte uncompressedtgaheader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte compressedtgaheader[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

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
	_r = 1;
	_g = 1;
	_b = 1;
	_a = 1;
}

ColorRGBA::ColorRGBA(float r, float g, float b, float a)
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
	{
		return false;
	}
	if(bpp == 24)									
	{
		type = GL_RGB;
	}
	else
	{
		type = GL_RGBA;
	}
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

	
	//Rotate about center
	vertexbuffer[bufferpos+0].rotate(quad->getMidPoint(),rotate);
	vertexbuffer[bufferpos+1].rotate(quad->getMidPoint(),rotate);
	vertexbuffer[bufferpos+2].rotate(quad->getMidPoint(),rotate);
	vertexbuffer[bufferpos+3].rotate(quad->getMidPoint(),rotate);
	
	vertexbuffer[bufferpos+0] = vertexbuffer[bufferpos+0] - quad->getMidPoint();
	vertexbuffer[bufferpos+1] = vertexbuffer[bufferpos+1] - quad->getMidPoint();
	vertexbuffer[bufferpos+2] = vertexbuffer[bufferpos+2] - quad->getMidPoint();
	vertexbuffer[bufferpos+3] = vertexbuffer[bufferpos+3] - quad->getMidPoint();
	
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
Renderer definitions
=====================*/


Renderer::Renderer()
{
		glEnable(GL_TEXTURE_2D);
	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
	
		glVertexPointer(3,GL_DOUBLE, 0,_vertbuffer.getVertbuffer());
		glTexCoordPointer(2,GL_FLOAT, 0,_vertbuffer.getTexCoordBuffer());
		glColorPointer(4,GL_FLOAT,0,_vertbuffer.getColorBuffer());
		
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::drawSprite(Sprite* animation,Point3d position,double xscale,double yscale,double rotate)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position-_camera,xscale,yscale,rotate);
	}
	else
		_vertbuffer.addToBuffer(animation,position-_camera,xscale,yscale,rotate);
}

void Renderer::drawSprite(Sprite* animation,Point3d position)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position-_camera,1.0,1.0,0);
	}
	else
		_vertbuffer.addToBuffer(animation,position-_camera,1.0,1.0,0);
}

void Renderer::drawFixedSprite(Sprite* animation,Point3d position)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position,1.0,1.0,0);
	}
	else
		_vertbuffer.addToBuffer(animation,position,1.0,1.0,0);
}

void Renderer::drawFixedSprite(Sprite* animation,Point3d position,double xscale,double yscale,double rotate)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position,xscale,yscale,rotate);
	}
	else
		_vertbuffer.addToBuffer(animation,position,xscale,yscale,rotate);
}

void Renderer::drawQuad(Quad* quad,Point3d position,double xscale,double yscale,double rotate)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(quad,position,xscale,yscale,rotate);
	}
	else
		_vertbuffer.addToBuffer(quad,position,xscale,yscale,rotate);
}


void Renderer::drawBuffer()
{

	if(_vertbuffer.getBufferLength() > 0)
	{	
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, _vertbuffer.getBufferLength());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		_vertbuffer.reset();
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
	if (_camera.x < position.x) // camera is deeper than current position
		_camera.x += (position.x-_camera.x)/8;
	if (_camera.x > position.x) //camera is higher than current position
		_camera.x -= (_camera.x-position.x)/32;

	if (_camera.y < position.y) // camera is deeper than current position
		_camera.y += (position.y-_camera.y)/8;
	if (_camera.y > position.y) //camera is higher than current position
		_camera.y -= (_camera.y-position.y)/32;
}

Renderer* Renderer::Instance()
{
	if(!_singletonRenderer)
		_singletonRenderer = new Renderer;
	return _singletonRenderer;
}

GLint Renderer::loadShader(std::string vertexfilename,std::string fragmentfilename)
{
	//File streams for shader files
	std::ifstream vertexfile;
	std::ifstream fragmentfile;

	//Source buffers
	GLchar* vertexsource;
	GLchar* fragmentsource;
	unsigned int vsourcelength;
	unsigned int fsourcelength;
	
	//Status variables

	GLint vertexcompiled;
	GLint fragmentcompiled;
	GLint linked;

	//Open vertex file, read into a string

	vertexfile.open(vertexfilename,ios::binary|ios_base::ate);
	vsourcelength = vertexfile.tellg();
	vertexsource = new GLchar[vsourcelength+1];
	vertexfile.seekg(ios::beg);
	unsigned int i = 0;
	while(vertexfile.good())
	{
		vertexsource[i] = vertexfile.get();
		if(!vertexfile.eof())
			i++;
	}
	vertexsource[vsourcelength] = '\0';
	
	//Create a handle for our shader, then compile.

	GLuint vertexhandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexhandle, 1, (const GLchar**)&vertexsource, NULL);
	glCompileShader(vertexhandle);
	
	glGetObjectParameterivARB(vertexhandle, GL_COMPILE_STATUS, &vertexcompiled);
	if(vertexcompiled)
		cout << "Vertex shader " << vertexfilename << " compiled successfully!" << endl;
	GLint blen = 0;	
	GLsizei slen = 0;
	
	glGetShaderiv(vertexhandle, GL_INFO_LOG_LENGTH , &blen);       
	
	//If any errors, display log

	if (blen > 1)
	{
	    GLchar* compiler_log = new GLchar[blen];	
	    glGetInfoLogARB(vertexhandle, blen, &slen, compiler_log);
	    cout << "compiler_log:" << endl;
	    cout << compiler_log;
	    delete[] compiler_log;
	}

	//Repeat the process for our fragment shader

	fragmentfile.open(fragmentfilename,ios::binary|ios_base::ate);
	vsourcelength = fragmentfile.tellg();
	fragmentsource = new GLchar[vsourcelength+1];
	fragmentfile.seekg(ios::beg);
	i = 0;
	while(fragmentfile.good())
	{
		fragmentsource[i] = fragmentfile.get();
		if(!fragmentfile.eof())
			i++;
	}
	fragmentsource[vsourcelength] = '\0';
	
	//Create a handle for our shader, then compile.

	GLuint fragmenthandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmenthandle, 1, (const GLchar**)&fragmentsource, NULL);
	glCompileShader(fragmenthandle);
	
	glGetObjectParameterivARB(fragmenthandle, GL_COMPILE_STATUS, &fragmentcompiled);
	if(fragmentcompiled)
		cout << "fragment shader " << fragmentfilename << " compiled successfully!" << endl;
	blen = 0;	
	slen = 0;
	
	glGetShaderiv(fragmenthandle, GL_INFO_LOG_LENGTH , &blen);       
	
	//If any errors, display log

	if (blen > 1)
	{
	    GLchar* compiler_log = new GLchar[blen];	
	    glGetInfoLogARB(fragmenthandle, blen, &slen, compiler_log);
	    cout << "compiler_log:" << endl;
	    cout << compiler_log;
	    delete[] compiler_log;
	}

	
	//Attatch shaders to program object, link and return the handle
	
	GLint programhandle = glCreateProgram();
	glAttachShader(programhandle, vertexhandle);
	glAttachShader(programhandle, fragmenthandle);
	glLinkProgram(programhandle);

	
    glGetProgramiv(programhandle, GL_LINK_STATUS, &linked);
    if (linked)
        cout << "Shader program successfully linked!" << endl;
	return programhandle;
}
