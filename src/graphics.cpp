#include "graphics.h"

using namespace graphics;

Renderer* Renderer::_singletonRenderer = NULL;

GLubyte uncompressedtgaheader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte compressedtgaheader[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

/*=====================
Sprite definitions
=====================*/

Sprite::Sprite()
{
	
}

Sprite::Sprite(TexData &mytexture,int frames,int height,int width)
{
  clipheight = height;
  clipwidth = width;
  maxclips = frames;
  texture = &mytexture;
  texcoords = new TexCoord*[frames];
  for(int i = 0;i < frames;i++)
  {
	texcoords[i] = new TexCoord[4];
  }
}

void Sprite::setClip(int frame,GLfloat x,GLfloat y)
{
	if(frame > maxclips)
	{
	   return;
	}
	else
	{
		texcoords[frame][0].u = (x*clipwidth)/texture->width;
		texcoords[frame][0].v = (y*clipheight)/texture->height;
		texcoords[frame][1].u = ((x*clipwidth)+clipwidth)/texture->width;
		texcoords[frame][1].v = (y*clipheight)/texture->height;
		texcoords[frame][2].u = ((x*clipwidth)+clipwidth)/texture->width;
		texcoords[frame][2].v = ((y*clipheight)+clipheight)/texture->height; 
		texcoords[frame][3].u = (x*clipwidth)/texture->width;
		texcoords[frame][3].v = ((y*clipheight)+clipheight)/texture->height;	
	}
}

TexCoord* Sprite::getTexCoords(int frame)
{
	if(frame > maxclips)
	{
	   return 0;
	}

	return texcoords[frame];
}

/*=====================
Animation definitions
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

Animation::Animation()
{
	currentsprite = 0;
	currentframe = 0;
	accumulator = 0.0;
	frametime = 0.0;
}

Sprite* Animation::getSprite()
{
	return currentsprite;
}

Animation::Animation(double quadheight,double quadwidth)
{
	currentframe = 0;
	accumulator = 0.0;
	frametime = 0.5;
	_vertices[0] = Point3d(0,0,0);
	_vertices[1] = Point3d(quadwidth,0,0);
	_vertices[2] = Point3d(quadwidth,quadheight,0); 
	_vertices[3] = Point3d(0,quadheight,0); 
	_color[0] = ColorRGBA();
	_color[1] = ColorRGBA();
    _color[2] = ColorRGBA();
	_color[3] = ColorRGBA();
}

Animation::Animation(double quadheight,double quadwidth,ColorRGBA color)
{
	currentframe = 0;
	accumulator = 0.0;
	frametime = 0.2;
	_vertices[0] = Point3d(0,0,0);
	_vertices[1] = Point3d(quadwidth,0,0);
	_vertices[2] = Point3d(quadwidth,quadheight,0); 
	_vertices[3] = Point3d(0,quadheight,0); 
	_color[0] = color;
	_color[1] = color;
    _color[2] = color;
	_color[3] = color;
}

void Animation::setClipDimensions(double height, double width)
{
	_vertices[0] = Point3d(0,0,0);
	_vertices[1] = Point3d(width,0,0);
	_vertices[2] = Point3d(width,height,0); 
	_vertices[3] = Point3d(0,height,0); 
}

void Animation::changeSprite(Sprite *newsprite)
{
	if (currentsprite != newsprite)
	{
		currentsprite = newsprite;
		currentframe = 0;
		accumulator = 0;
	}
}

void Animation::changeSpriteNoRewind(Sprite *newsprite)
{
	if (currentsprite != newsprite)
	{
		currentsprite = newsprite;
	}
}

void Animation::play(double deltatime)
{
	accumulator += deltatime;
	if(accumulator > frametime)
	{
		currentframe+=1;
		accumulator = 0;
	}
	if(currentframe > currentsprite->getMaxClips()-1)
		currentframe = 0;
}

void Animation::rewind()
{
	currentframe = 0;
	accumulator = 0;
}

/*=====================
Font Definitions
=====================*/

BitmapFont::BitmapFont()
{
}

BitmapFont::BitmapFont(TexData& texture)
{
	buildFont(texture);
}

void BitmapFont::buildFont(TexData& texture)
{
	int currentchar = 0;
	for(int row = 0; row < 16; row++)
	{
		for(int column = 0; column < 16; column++)
		{
			_charactersprites[currentchar] = graphics::Sprite(texture,1,texture.height/16,texture.width/16);
			_charactersprites[currentchar].setClip(0,column,row);
			currentchar++;
		}
	}
}


/*=====================
Texture Definitions
=====================*/

TGA::TGA()
{

}

bool TGA::LoadCompressedTGA(char *filename,std::ifstream &texturestream)
{
	return false;
}

bool TGA::LoadUncompressedTGA(char *filename,std::ifstream &texturestream)
{
	//Headers for the tga texture
	GLubyte header[12];
	GLubyte header2[6];
	
	//These are byte arrays to be compared to the header of our TGA file
	GLubyte uncompressedheader[12];
	GLubyte compressedheader[12];

	GLubyte* imagedata;		        //Pointer for storing image data
	GLuint bpp;                     //Bits per pixel
	GLuint width;                   //Width of the entire image
	GLuint height;                  //Height of the entire image
	GLuint texID;                   //Texture ID for use with glBindTexture
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
	width  = (GLuint)header2[1] * 256 + (GLuint)header2[0];				//read and calculate width and save
	height = (GLuint)header2[3] * 256 + (GLuint)header2[2];				//read and calculate height and save
	bpp    = (GLuint)header2[4];										//read bpp and save

	if((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))	//check to make sure the height, width, and bpp are valid
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
	imagesize = ((bpp/8) * width * height);										//determine size in bytes of the image
	imagedata = new GLubyte[imagesize];											//allocate memory for our imagedata variable
	texturestream.read((char*)imagedata,imagesize);								//read according the the size of the image and save into imagedata 

	for(GLuint cswap = 0; cswap < (GLuint)imagesize; cswap += (bpp/8))	        //loop through and reverse the tga's BGR format to RGB
	{
		imagedata[cswap] ^= imagedata[cswap+2] ^=				                //1st Byte XOR 3rd Byte XOR 1st Byte XOR 3rd Byte
	    imagedata[cswap] ^= imagedata[cswap+2];
	}

	texturestream.close();														//close ifstream because we're done with it

	glGenTextures(1, &texID);													// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, texID);										

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imagedata);
    delete imagedata;
	return true;
}

TexData Texture::createEmptyTexture(int height,int width)
{
	imagedata = new GLubyte[height*width*4];
	glGenTextures(1, &texID);													// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, texID);										

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
	texdata.height = height;
	texdata.width = width;
	texdata.texid = texID;

	cout << "Created " << width << "x" << height << " empty texture with id " << texID << endl;
	return texdata;
}


TexData TGA::loadImage(char *filename)
{
	texdata.texid = texID;
	texdata.width = width;
	texdata.height = height;
	cout << texID << "Texture " << filename <<  " successfully loaded with id " << texdata.texid << endl;
	return texdata;
}


/*=====================
Sprite Batcher definitions
=====================*/


void SpriteBatch::addToBuffer(Animation* animation,Point3d position)
{

	if (animation == 0 || animation->getSprite() == 0)
	{
		cout << "Error: Attempting to draw animation from a null pointer!" << endl;
		return;
	}

	_tempvertbuf = animation->getVertices();
	_temptexbuf = animation->getCurrentTexCoords();
	_tempcolorbuf = animation->getColor();


	_vertexbuffer[_bufferpos+0] = _tempvertbuf[0]+position;
	_vertexbuffer[_bufferpos+1] = _tempvertbuf[1]+position;
	_vertexbuffer[_bufferpos+2] = _tempvertbuf[2]+position;
	_vertexbuffer[_bufferpos+3] = _tempvertbuf[3]+position;
	

	_texcoordbuffer[_bufferpos+0] = _temptexbuf[0];
	_texcoordbuffer[_bufferpos+1] = _temptexbuf[1];
	_texcoordbuffer[_bufferpos+2] = _temptexbuf[2];
	_texcoordbuffer[_bufferpos+3] = _temptexbuf[3];

	
	_colorbuffer[_bufferpos+0] = _tempcolorbuf[0];
	_colorbuffer[_bufferpos+1] = _tempcolorbuf[1];
	_colorbuffer[_bufferpos+2] = _tempcolorbuf[2];
	_colorbuffer[_bufferpos+3] = _tempcolorbuf[3];

	_bufferpos += 4;
}

Point3d* SpriteBatch::get_vertbuffer()
{
	return _vertexbuffer;
}

TexCoord* SpriteBatch::getTexCoordBuffer()
{
	return _texcoordbuffer;
}

ColorRGBA* SpriteBatch::getColorBuffer()
{
	return _colorbuffer;
}

GLint SpriteBatch::getBufferLength()
{
	return _bufferpos;
}

void SpriteBatch::reset()
{
	_bufferpos = 0;  
}

bool SpriteBatch::isFull()
{
	if(BUFFER_SIZE-_bufferpos < 4)
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
	_bufferpos = 0;
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
	
		glVertexPointer(3,GL_DOUBLE, 0,_vertbuffer.get_vertbuffer());
		glTexCoordPointer(2,GL_FLOAT, 0,_vertbuffer.getTexCoordBuffer());
		glColorPointer(4,GL_FLOAT,0,_vertbuffer.getColorBuffer());
		
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::draw(Animation* animation,Point3d position)
{
	assert(animation != 0);

	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position-_camera);
	}
	else
		_vertbuffer.addToBuffer(animation,position-_camera);
}

void Renderer::drawFixed(Animation* animation,Point3d position)
{
	if(_vertbuffer.isFull())
	{
			drawBuffer();
			_vertbuffer.reset();
			_vertbuffer.addToBuffer(animation,position);
	}
	else
		_vertbuffer.addToBuffer(animation,position);
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
	Animation character(16,16);
	for(int i = 0; i < text.size(); i++)
	{
		character.changeSprite(&fonts[0]._charactersprites[(int)text[i]]);

		drawFixed(&character,Point3d(position._x+(i*space),position._y,0));
	}
	*/
}
void Renderer::moveCameraTowards(Point3d position)
{
	if (_camera._x < position._x) // camera is deeper than current position
		_camera._x += (position._x-_camera._x)/8;
	if (_camera._x > position._x) //camera is higher than current position
		_camera._x -= (_camera._x-position._x)/32;

	if (_camera._y < position._y) // camera is deeper than current position
		_camera._y += (position._y-_camera._y)/8;
	if (_camera._y > position._y) //camera is higher than current position
		_camera._y -= (_camera._y-position._y)/32;
}

Renderer* Renderer::Instance()
{
	if(!_singletonRenderer)
		_singletonRenderer = new Renderer;
	return _singletonRenderer;
}

/*

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

*/