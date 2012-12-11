#include "graphics.h"

using namespace graphics;

Renderer* Renderer::instance = NULL;

GLubyte uncompressedtgaheader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte compressedtgaheader[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

 map<string,graphics::Texture*>       graphics::textures;
 map<string,graphics::ShaderProgram>  graphics::shaders;




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
	return sequences[name].size();
}

void SpriteSheet::addFrame(string sequence,SpriteFrame frame)
{
	sequences[sequence].push_back(frame);
}


/*=====================
Font Definitions
=====================*/

TextureFont::TextureFont()
{
}

SpriteFrame TextureFont::getFrame(int framenum)
{
	return glyphs[framenum];
}

TextureFont::TextureFont(char* filename)
{
	this->loadUncompressedTGA(filename);
	int i = 0;
	SpriteFrame characterframe;
	characterframe.height = this->height/16;
	characterframe.width = this->width/16;


	for(float row = 0; row < 16; row+=1.0)
	{
		for(float column = 0; column < 16; column+=1.0)
		{
			characterframe.texcoords[0] = TexCoord(column/characterframe.width,row/characterframe.height);
			characterframe.texcoords[1] = TexCoord((column+1.0)/characterframe.width,row/characterframe.height);
			characterframe.texcoords[2] = TexCoord((column+1.0)/characterframe.width,(row+1.0)/characterframe.height);
			characterframe.texcoords[3] = TexCoord(column/characterframe.width,(row+1.0)/characterframe.height);

			glyphs[i]=characterframe;
			i++;
		}
	}
}


/*=====================

Quad definitions

=====================*/
Quad::Quad()
{
	topleft.x = 0;
	topleft.y = 0;
	topright.x = 0;
	topright.y = 0;
	bottomright.x = 0;
	bottomright.y = 0;
	bottomleft.x = 0;
	bottomleft.y = 0;
	this->color = ColorRGBA(1,1,1,1);
}

Quad::Quad(double height,double width,ColorRGBA color)
{
	topleft.x = 0;
	topleft.y = 0;
	topright.x = width;
	topright.y = 0;
	bottomright.x = width;
	bottomright.y = height;
	bottomleft.x = 0;
	bottomleft.y = height;

	midpoint.x = width/2;
	midpoint.y = height/2;
}

ColorRGBA Quad::getColor()
{
	return color;
}

void Quad::setColor(ColorRGBA color)
{
	this->color = color;
}

/*
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

inline Point3f Quad::getMidPoint()
{
	return Point3f(width/2,height/2,0);
}
*/

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
	color = ColorRGBA(1,1,1,1);
	//Default sprite is degenerate quad.
	targetspritesheet = 0;
	frame = 0;
	accumulator = 0.0;
	frametime = 0.0;
}

Sprite::Sprite(float height,float width)
{
	color = ColorRGBA(1,1,1,1);
	targetspritesheet = 0;
	frame = 0;
	accumulator = 0.0;
	frametime = 0.0;

	topleft.x = 0;
	topleft.y = 0;
	topright.x = width;
	topright.y = 0;
	bottomright.x = width;
	bottomright.y = height;
	bottomleft.x = 0;
	bottomleft.y = height;

	midpoint.x = width/2;
	midpoint.y = height/2;
}

void Sprite::changeSequence(string sequence)
{
	this->sequence = sequence;
	this->sequencelength = targetspritesheet->getSequenceLength(sequence);
}

void Sprite::changeSpriteSheet(SpriteSheet* newsprite)
{
		targetspritesheet = newsprite;
		accumulator = 0;
}

void Sprite::changeSpriteSheet(std::string name)
{
	targetspritesheet = (SpriteSheet*)graphics::textures[name];
	accumulator = 0;
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

Point3f Sprite::getOffset()
{
	return offset;
}

SpriteFrame Sprite::getSpriteFrame()
{
	return targetspritesheet->getFrame(sequence,frame);
}


void Sprite::setSpriteFrame(unsigned int frame)
{
	this->frame = frame;
}

SpriteSheet* Sprite::getSpriteSheet()
{
	return targetspritesheet;
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
		cout << "Successfully opened texture" << endl;
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

	cout << "Loaded texture " << filename << " with id " << texid << endl;
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
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	*/
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
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

Glyph::Glyph(float height,float width)
{
	character = '0';

	topleft.x = 0;
	topleft.y = 0;
	topright.x = width;
	topright.y = 0;
	bottomright.x = width;
	bottomright.y = height;
	bottomleft.x = 0;
	bottomleft.y = height;

	midpoint.x = width/2;
	midpoint.y = height/2;
}

void Glyph::setFont(TextureFont* font)
{
	this->font = font;
}

void Glyph::setGlyph(char character)
{
	this->character = character;
}

SpriteFrame Glyph::getGlyph()
{
	return font->TextureFont::getFrame(character);
}

/*=====================
SpriteSheet Batcher definitions
=====================*/
inline void SpriteBatch::addToBuffer(Glyph glyph,Point3f position)
{
	
	//Get initial points
	vertexbuffer[bufferpos+0].point.x = 0;
	vertexbuffer[bufferpos+0].point.y = 0;
	vertexbuffer[bufferpos+0].point.z = 0;

	vertexbuffer[bufferpos+1].point.x = glyph.topright.x;
	vertexbuffer[bufferpos+1].point.y = 0;
	vertexbuffer[bufferpos+1].point.z = 0;

	vertexbuffer[bufferpos+2].point.x = glyph.bottomright.x;
	vertexbuffer[bufferpos+2].point.y = glyph.bottomright.y;
	vertexbuffer[bufferpos+2].point.z = 0;

	vertexbuffer[bufferpos+3].point.x = 0;
	vertexbuffer[bufferpos+3].point.y = glyph.bottomleft.y;
	vertexbuffer[bufferpos+3].point.z = 0;

	
	
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point - glyph.midpoint;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point - glyph.midpoint;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point - glyph.midpoint;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point - glyph.midpoint;
	
	//Translate
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point + position;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point + position;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point + position;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point + position;
	
	
	//Texcoords
	vertexbuffer[bufferpos+0].texcoord = glyph.getGlyph().texcoords[0];
	vertexbuffer[bufferpos+1].texcoord = glyph.getGlyph().texcoords[1];
	vertexbuffer[bufferpos+2].texcoord = glyph.getGlyph().texcoords[2];
	vertexbuffer[bufferpos+3].texcoord = glyph.getGlyph().texcoords[3];

	//Color
	vertexbuffer[bufferpos+0].color = glyph.getColor();
	vertexbuffer[bufferpos+1].color = glyph.getColor();
	vertexbuffer[bufferpos+2].color = glyph.getColor();
	vertexbuffer[bufferpos+3].color = glyph.getColor();
	
	
	bufferpos += 4;
}

inline void SpriteBatch::addToBuffer(Sprite sprite,Point3f position)
{
	
	//Get initial points
	vertexbuffer[bufferpos+0].point.x = 0;
	vertexbuffer[bufferpos+0].point.y = 0;
	vertexbuffer[bufferpos+0].point.z = 0;

	vertexbuffer[bufferpos+1].point.x = sprite.topright.x;
	vertexbuffer[bufferpos+1].point.y = 0;
	vertexbuffer[bufferpos+1].point.z = 0;

	vertexbuffer[bufferpos+2].point.x = sprite.bottomright.x;
	vertexbuffer[bufferpos+2].point.y = sprite.bottomright.y;
	vertexbuffer[bufferpos+2].point.z = 0;

	vertexbuffer[bufferpos+3].point.x = 0;
	vertexbuffer[bufferpos+3].point.y = sprite.bottomleft.y;
	vertexbuffer[bufferpos+3].point.z = 0;

	
	
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point - sprite.midpoint;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point - sprite.midpoint;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point - sprite.midpoint;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point - sprite.midpoint;
	
	//Translate
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point + position;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point + position;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point + position;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point + position;
	
	
	//Texcoords
	vertexbuffer[bufferpos+0].texcoord = sprite.getSpriteFrame().texcoords[0];
	vertexbuffer[bufferpos+1].texcoord = sprite.getSpriteFrame().texcoords[1];
	vertexbuffer[bufferpos+2].texcoord = sprite.getSpriteFrame().texcoords[2];
	vertexbuffer[bufferpos+3].texcoord = sprite.getSpriteFrame().texcoords[3];

	//Color
	vertexbuffer[bufferpos+0].color = sprite.getColor();
	vertexbuffer[bufferpos+1].color = sprite.getColor();
	vertexbuffer[bufferpos+2].color = sprite.getColor();
	vertexbuffer[bufferpos+3].color = sprite.getColor();
	
	
	bufferpos += 4;
}

inline void SpriteBatch::addToBuffer(Quad quad,Point3f position,double xscale,double yscale,double rotate)
{
	
	//Get initial points
	vertexbuffer[bufferpos+0].point.x = 0;
	vertexbuffer[bufferpos+0].point.y = 0;
	vertexbuffer[bufferpos+0].point.z = 0;

	vertexbuffer[bufferpos+1].point.x = quad.topright.x;
	vertexbuffer[bufferpos+1].point.y = 0;
	vertexbuffer[bufferpos+1].point.z = 0;

	vertexbuffer[bufferpos+2].point.x = quad.bottomright.x;
	vertexbuffer[bufferpos+2].point.y = quad.bottomright.y;
	vertexbuffer[bufferpos+2].point.z = 0;

	vertexbuffer[bufferpos+3].point.x = 0;
	vertexbuffer[bufferpos+3].point.y = quad.bottomleft.y;
	vertexbuffer[bufferpos+3].point.z = 0;

	
	
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point - quad.midpoint;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point - quad.midpoint;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point - quad.midpoint;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point - quad.midpoint;
	
	
    //Scale
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point * Point3f(xscale,yscale,0);
	
	
	//Rotate about center
	vertexbuffer[bufferpos+0].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+1].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+2].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+3].point.rotate(Point3f(0,0,0),rotate);
	
	
	
	//Translate
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point + position;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point + position;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point + position;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point + position;
	
	
	//Texcoords
	vertexbuffer[bufferpos+0].texcoord.x = 0.0;
	vertexbuffer[bufferpos+0].texcoord.y = 0.0;
	vertexbuffer[bufferpos+1].texcoord.x = 1.0;
	vertexbuffer[bufferpos+1].texcoord.y = 0.0;
	vertexbuffer[bufferpos+2].texcoord.x = 1.0;
	vertexbuffer[bufferpos+2].texcoord.y = 1.0;
	vertexbuffer[bufferpos+3].texcoord.x = 0.0;
	vertexbuffer[bufferpos+3].texcoord.y = 1.0;

	//Color
	vertexbuffer[bufferpos+0].color = quad.getColor();
	vertexbuffer[bufferpos+1].color = quad.getColor();
	vertexbuffer[bufferpos+2].color = quad.getColor();
	vertexbuffer[bufferpos+3].color = quad.getColor();
	
	
	bufferpos += 4;
}

inline void SpriteBatch::addToBuffer(Sprite sprite, Point3f position,double xscale,double yscale,double rotate)
{
	//Get initial points
	vertexbuffer[bufferpos+0].point.x = 0;
	vertexbuffer[bufferpos+0].point.y = 0;
	vertexbuffer[bufferpos+0].point.z = 0;

	vertexbuffer[bufferpos+1].point.x = sprite.topright.x;
	vertexbuffer[bufferpos+1].point.y = 0;
	vertexbuffer[bufferpos+1].point.z = 0;

	vertexbuffer[bufferpos+2].point.x = sprite.bottomright.x;
	vertexbuffer[bufferpos+2].point.y = sprite.bottomright.y;
	vertexbuffer[bufferpos+2].point.z = 0;

	vertexbuffer[bufferpos+3].point.x = 0;
	vertexbuffer[bufferpos+3].point.y = sprite.bottomleft.y;
	vertexbuffer[bufferpos+3].point.z = 0;

	
	
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point - sprite.midpoint;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point - sprite.midpoint;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point - sprite.midpoint;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point - sprite.midpoint;
	
	
    //Scale
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point * Point3f(xscale,yscale,0);
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point * Point3f(xscale,yscale,0);
	
	
	//Rotate about center
	vertexbuffer[bufferpos+0].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+1].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+2].point.rotate(Point3f(0,0,0),rotate);
	vertexbuffer[bufferpos+3].point.rotate(Point3f(0,0,0),rotate);
	
	
	
	//Translate
	vertexbuffer[bufferpos+0].point = vertexbuffer[bufferpos+0].point + position;
	vertexbuffer[bufferpos+1].point = vertexbuffer[bufferpos+1].point + position;
	vertexbuffer[bufferpos+2].point = vertexbuffer[bufferpos+2].point + position;
	vertexbuffer[bufferpos+3].point = vertexbuffer[bufferpos+3].point + position;
	
	
	//Texcoords
	vertexbuffer[bufferpos+0].texcoord = sprite.getSpriteFrame().texcoords[0];
	vertexbuffer[bufferpos+1].texcoord = sprite.getSpriteFrame().texcoords[1];
	vertexbuffer[bufferpos+2].texcoord = sprite.getSpriteFrame().texcoords[2];
	vertexbuffer[bufferpos+3].texcoord = sprite.getSpriteFrame().texcoords[3];

	//Color
	vertexbuffer[bufferpos+0].color = sprite.getColor();
	vertexbuffer[bufferpos+1].color = sprite.getColor();
	vertexbuffer[bufferpos+2].color = sprite.getColor();
	vertexbuffer[bufferpos+3].color = sprite.getColor();
	
	
	bufferpos += 4;
	
}

Vertex* SpriteBatch::getVertbuffer()
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
	//memset(vertexbuffer,0.0,sizeof(Vertex)*BUFFER_SIZE);
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
		file.open(filename.c_str(),ios::binary|ios_base::ate);
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
			//Sets the active texture unit, commented out since we don't need to mess with texture units
			//glActiveTexture(GL_TEXTURE0);
			//
			glBindTexture(GL_TEXTURE_2D, tex_handle);
			glUseProgram(handle);

			Mat4 identity;
			Mat4::Identity(identity);

			Mat4 ortho;
			Mat4::Ortho(ortho, 0, 0, (float)width, (float)height, 0, 1);

			Mat4 proj_matrix;
			identity.Multiply(proj_matrix, ortho);


			glUniformMatrix4fv(shader_mat_proj_handle, 1, false, (float*)&proj_matrix);
			glUniformMatrix4fv(shader_mat_mv_handle, 1, false, (float*)&identity);
		} else {
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	}



	void ShaderProgram::zoom(float factor)
	{
		
			
		Mat4 identity;
		Mat4::Identity(identity);
		Mat4 ortho;
		Mat4::Ortho(ortho, (-(float)SCREEN_WIDTH/2)/factor, (-(float)SCREEN_WIDTH/2)/factor,((float)SCREEN_WIDTH/2)/factor, ((float)SCREEN_HEIGHT/2)/factor, 0, 1);

		Mat4 proj_matrix = ortho*identity;

		glUniformMatrix4fv(shader_mat_proj_handle, 1, false, (float*)&proj_matrix);
	}
	

    void ShaderProgram::getUniformHandles() {
		glUseProgram(handle);
		shader_mat_proj_handle = glGetUniformLocation(handle, TVS_PROJECTION_MAT);
		shader_tex_handle      = glGetUniformLocation(handle, SAMPLER_NAME);
		shader_mat_mv_handle   = glGetUniformLocation(handle, TVS_MODEL_VIEW_MAT);

		cout << "Projection matrix at: " << shader_mat_proj_handle << endl;
		cout << "Sampler at:           " <<  shader_tex_handle << endl;
		cout << "Modelview matrix at:  " <<  shader_mat_mv_handle << endl;
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

    if( SDL_SetVideoMode( SCREEN_WIDTH*1, SCREEN_HEIGHT*1, SCREEN_BPP, SDL_OPENGL) == NULL )	// Set window properties, OpenGL is passed here
    {
        return false;
    }
    
    cout << "Video mode set!" << endl;

    SDL_EnableUNICODE(1);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );				// Activate double buffer for buffer switching
    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );				// Activate swap control, also for buffer switching
	
	glClearColor( 1.0,1.0,1.0,1.0 ); 

                                                     // Set clear color.  This is what the buffer gets filled with when we call glClear
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
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
        std::cout << "OH SHIT Error: " << glewGetErrorString(err) << endl;
	else
		cout << "GLEW: NO ERRORS HERE" << endl;
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

	graphics::loadAssets();
}

void Renderer::setDefaultRendering(bool state)
{
	defaultshader->enable(state);
}

void Renderer::zoom(float factor)
{
	defaultshader->zoom(factor);
}

Renderer::Renderer()
{
		glEnable(GL_TEXTURE_2D);

		glGenBuffers( 1, &vbovertex);
		glGenBuffers( 1, &vbotexture);
		glGenBuffers( 1, &vbocolor);

		defaultshader = new ShaderProgram("regular.vert","regular.frag");
		cout << "Setting output size" << endl;
		defaultshader->setOutputSize(SCREEN_WIDTH,SCREEN_HEIGHT);
		cout << "Enabling shader" << endl;
		defaultshader->enable(true);
	    cout << "Getting attribute locations" << endl;
		int vertexattriboffset = glGetAttribLocation(defaultshader->getHandle(),VERTEX_ATTRIBUTE_NAME);
		int texcoordattriboffset = glGetAttribLocation(defaultshader->getHandle(),TEXCOORD_ATTRIBUTE_NAME);;
		int colorattriboffset = glGetAttribLocation(defaultshader->getHandle(),COLOR_ATTRIBUTE_NAME);;

		cout << "Enabling attribute Arrays" << endl;

		glEnableVertexAttribArray(vertexattriboffset);
		glBindBuffer(GL_ARRAY_BUFFER,vbovertex);
		glVertexAttribPointer(vertexattriboffset,3, GL_FLOAT,0,sizeof(Vertex),(void*)0);

		glEnableVertexAttribArray(texcoordattriboffset);
		glVertexAttribPointer(texcoordattriboffset,2, GL_FLOAT, 0,sizeof(Vertex),(void*)12);

		glEnableVertexAttribArray(colorattriboffset);
		glVertexAttribPointer(colorattriboffset,4,GL_FLOAT,GL_TRUE,sizeof(Vertex),(void*)20);

		cout << "Renderer construction complete" << endl;

		//Request block of memory from the device

}

void Renderer::drawSprite(Sprite animation,Point3f position,double xscale,double yscale,double rotate)
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

void Renderer::drawSprite(Sprite animation,Point3f position)
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

void Renderer::drawFixedSprite(Sprite animation,Point3f position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(animation,position);
	}
	else
		spritebatch.addToBuffer(animation,position);
}

void Renderer::drawFixedGlyph(Glyph glyph,Point3f position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
			spritebatch.addToBuffer(glyph,position);
	}
	else
		spritebatch.addToBuffer(glyph,position);
}

void Renderer::drawFixedSprite(Sprite animation,Point3f position,double xscale,double yscale,double rotate)
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

void Renderer::drawQuad(Quad quad,Point3f position,double xscale,double yscale,double rotate)
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
		glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*spritebatch.getBufferLength(),spritebatch.getVertbuffer(),GL_STREAM_DRAW);
		glDrawArrays(GL_QUADS,0,spritebatch.getBufferLength());
        spritebatch.reset();
	}
	
}

void Renderer::drawText(TextureFont* font,std::string text,Point3f position,ColorRGBA color, GLfloat space)
{
	
	Glyph character(16.0,16.0);
	character.setColor(color);
	character.setFont(font);
	
	for(float i = 0; i < text.size(); i+=1.0)
	{
		character.setGlyph(text[i]);
        drawFixedGlyph(character,Point3f(position.x+((float)i*space),position.y,0));
	}
	
}
void Renderer::moveCameraTowards(Point3f position)
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


void Renderer::changeTexture(std::string name)
{
	glBindTexture(GL_TEXTURE_2D,textures[name]->getTexId());
}


Renderer* Renderer::Instance()
{
	if(!instance)
		instance = new Renderer();
	return instance;
}

SpriteFrame::SpriteFrame(float height,float width,Texture* texture,Point2f topleft)
{
	texcoords[0] = TexCoord(topleft.x/texture->getWidth(),topleft.y/texture->getHeight());
	texcoords[1] = TexCoord((topleft.x+width)/texture->getWidth(),topleft.y/texture->getHeight());
	texcoords[2] = TexCoord((topleft.x+width)/texture->getWidth(),(topleft.y+height)/texture->getHeight());
	texcoords[3] = TexCoord(topleft.x/texture->getWidth(),(topleft.y+height)/texture->getHeight());
}

SpriteFrame::SpriteFrame()
{

}

void graphics::loadAssets()
{

  SpriteSheet uispritesheet;

  SpriteFrame tempframe;
  uispritesheet.loadUncompressedTGA("uisprites.tga");

  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(16,0));
  uispritesheet.addFrame("horizontal border",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(0,16));
  uispritesheet.addFrame("vertical border",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(32,0));
  uispritesheet.addFrame("constitution icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(48,0));
  uispritesheet.addFrame("speed icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(48,16));
  uispritesheet.addFrame("intelligence icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(32,16));
  uispritesheet.addFrame("strength icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(0,0));
  uispritesheet.addFrame("border corner",tempframe);

  graphics::textures["ui"] = new SpriteSheet(uispritesheet);
  graphics::textures["uifont"] = new TextureFont("uifont.tga");

  cout << "HONK " << endl;

}