#include "graphics.h"


using namespace graphics;

Renderer* Renderer::instance = NULL;

GLubyte uncompressedtgaheader[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
GLubyte compressedtgaheader[12]   = {0,0,10,0,0,0,0,0,0,0,0,0};

map<string,graphics::Texture*>       graphics::textures;
map<string,graphics::ShaderProgram>  graphics::shaders;

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

TextureFont::TextureFont()
{
}

SpriteFrame TextureFont::getFrame(int framenum)
{
	return glyphs[framenum];
}

TextureFont::TextureFont(char* filename,int height,int width)
{
	this->loadUncompressedTGA(filename);
	int i = 0;
	SpriteFrame characterframe;

	characterframe.height = (float)height/16.0;
	characterframe.width = (float)width/16.0;
	charheight = characterframe.height;
	charwidth = characterframe.width;

	for(float row = 0; row < 16; row+=1.0)
	{
		for(float column = 0; column < 16; column+=1.0)
		{
			characterframe.texcoords[0] = TexCoord((column*characterframe.width)/this->width,(row*characterframe.height)/this->height);
			characterframe.texcoords[1] = TexCoord(((column+1)*characterframe.width)/this->width,(row*characterframe.height)/this->height);
			characterframe.texcoords[2] = TexCoord(((column+1)*characterframe.width)/this->width,((row+1)*characterframe.height)/this->height);
			characterframe.texcoords[3] = TexCoord((column*characterframe.width)/this->width,((row+1)*characterframe.height)/this->height);

			glyphs[i]=characterframe;
			i++;
		}
	}
}

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

void Quad::setHeight(float height)
{
	bottomright.y = height;
	bottomleft.y = height;
	midpoint.y = height/2;
}

void Quad::setWidth(float width)
{
	topright.x = width;
	bottomright.x = width;
	midpoint.x = width/2;
}

ColorRGBA::ColorRGBA()
{
	_r = 1.0;
	_g = 1.0;
	_b = 1.0;
	_a = 1.0;
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
	targetspritesheet = 0;
	frame = 0;
	accumulator = 0.0;
	frametime = 0.0;
}

Sprite::~Sprite()
{
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

void Sprite::setWidth(int width)
{
	this->topright.x = width;
	this->bottomright.x = width;
}

void Sprite::setHeight(int height)
{
	this->bottomright.y = height;
	this->bottomleft.y = height;
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

/////////////////////////////////
// Lua bindings for Quad class
/////////////////////////////////

int graphics::l_Quad_constructor(lua_State *L)
{
	cout << "Creating Quad." << endl;
	Quad ** udata = (Quad **)lua_newuserdata(L, sizeof(Quad *));
	*udata = new Quad();
	luaL_getmetatable(L, "luaL_Quad");
	lua_setmetatable(L, -2);
	return 1;
}

int graphics::l_Quad_destructor(lua_State *L)
{
	return 0;
}

int graphics::l_Quad_setColor(lua_State *L)
{
	Quad* quad = l_checkQuad(L,1);
	quad->setColor(ColorRGBA(getArgNumber(2,L),getArgNumber(3,L),getArgNumber(4,L),getArgNumber(5,L)));
	return 0;
}

int graphics::l_Quad_setWidth(lua_State *L)
{
	Quad* quad = l_checkQuad(L,1);
	quad->setWidth(getArgNumber(2,L));
	return 0;
}

int graphics::l_Quad_setHeight(lua_State *L)
{
	Quad* quad = l_checkQuad(L,1);
	quad->setHeight(getArgNumber(2,L));
	return 0;
}

Quad* graphics::l_checkQuad(lua_State *L, int n)
{
	return *(Quad **)luaL_checkudata(L, n, "luaL_Quad");
}

int graphics::l_Quad_getWidth(lua_State *L)
{
	cout << "GetWidth called" << endl;
	Quad* sprite = l_checkQuad(L,1);
	lua_pushnumber(L,sprite->getWidth());
	return 1;
}

int graphics::l_Quad_getHeight(lua_State *L)
{
	cout << "GetHeight called" << endl;
	Quad* quad = l_checkQuad(L,1);
	lua_pushnumber(L,quad->getHeight());
	return 1;
}

int graphics::l_Quad_getColor(lua_State *L)
{
	cout << "GetColor called" << endl;
	Quad* quad = l_checkQuad(L,1);
	lua_pushnumber(L,quad->getColor()._r);
	lua_pushnumber(L,quad->getColor()._g);
	lua_pushnumber(L,quad->getColor()._b);
	lua_pushnumber(L,quad->getColor()._a);
	return 4;
}

void graphics::registerQuad(lua_State *L)
{
	luaL_Reg sQuadRegs[] =
	{
		{ "new",              l_Quad_constructor},
		{ "setWidth",         l_Quad_setWidth},
		{ "setHeight",        l_Quad_setHeight},
		{ "getWidth",         l_Quad_getWidth},
		{ "getHeight",        l_Quad_getHeight},
		{ "setColor",         l_Quad_setColor},
		{ "getColor",         l_Quad_getColor},
		{ "__gc",             l_Quad_destructor},
		{NULL,NULL}
	};
	// Create a luaL metatable. This metatable is not
	// exposed to Lua. The "luaL_Quad" label is used by luaL
	// internally to identity things.

	Lua lua = Lua(L);
	lua.stackDump();

	luaL_newmetatable(L, "luaL_Quad");
	lua.stackDump();
	luaL_setfuncs (L, sQuadRegs, 0);
	lua.stackDump();
	lua_pushvalue(L, -1);
	lua.stackDump();
	lua_setfield(L, -1, "__index");
	lua.stackDump();
	lua_setglobal(L, "Quad");
	lua.stackDump();
}

/////////////////////////////////
// Lua bindings for Sprite class
/////////////////////////////////

int graphics::l_Sprite_constructor(lua_State *L)
{
	cout << "Creating sprite." << endl;
	Sprite ** udata = (Sprite **)lua_newuserdata(L, sizeof(Sprite *));
	*udata = new Sprite();
	luaL_getmetatable(L, "luaL_Sprite");
	lua_setmetatable(L, -2);
	return 1;
}

int graphics::l_Sprite_setWidth(lua_State *L)
{
	Sprite* sprite = l_checkSprite(L,1);
	sprite->setWidth(getArgNumber(2,L));
	return 0;
}

int graphics::l_Sprite_setHeight(lua_State *L)
{
	Sprite* sprite = l_checkSprite(L,1);
	sprite->setHeight(getArgNumber(2,L));
	return 0;
}

int graphics::l_Sprite_getWidth(lua_State *L)
{
	cout << "GetWidth called" << endl;
	Sprite* sprite = l_checkSprite(L,1);
	lua_pushnumber(L,sprite->getWidth());
	return 1;
}

int graphics::l_Sprite_getHeight(lua_State *L)
{
	cout << "GetHeight called" << endl;
	Sprite* sprite = l_checkSprite(L,1);
	lua_pushnumber(L,sprite->getHeight());
	return 1;
}

int graphics::l_Sprite_destructor(lua_State *L)
{
	return 0;
}

int graphics::l_Sprite_changeSequence(lua_State *L)
{
	Sprite* sprite = l_checkSprite(L,1);
	sprite->changeSequence(getArgString(2,L));
	return 0;
}

int graphics::l_Sprite_changeSpriteSheet(lua_State *L)
{
	Sprite* sprite = l_checkSprite(L,1);
	sprite->changeSpriteSheet(l_checkSpriteSheet(L,2));
	return 0;
}

int graphics::l_Sprite_play(lua_State *L)
{
	Sprite* sprite = l_checkSprite(L,1);
	sprite->play(10);
	return 0;
}

int graphics::l_Sprite_stop(lua_State *L)
{
	return 1;
}

int graphics::l_Sprite_rewind(lua_State *L)
{
	return 1;
}

Sprite* graphics::l_checkSprite(lua_State *L, int n)
{
	 return *(Sprite **)luaL_checkudata(L, n, "luaL_Sprite");
}


void graphics::registerSprite(lua_State *L)
{
	
	luaL_Reg sSpriteRegs[] =
	{
		{ "new",              l_Sprite_constructor },
		{ "setWidth",         l_Sprite_setWidth},
		{ "setHeight",        l_Sprite_setHeight},
		{ "getWidth",         l_Sprite_getWidth},
		{ "getHeight",        l_Sprite_getHeight},
		{ "setSpriteSheet",l_Sprite_changeSpriteSheet},
		{ "setSequence",   l_Sprite_changeSequence},
		{ "__gc",             l_Sprite_destructor},
		{NULL,NULL}
	};
	// Create a luaL metatable. This metatable is not
	// exposed to Lua. The "luaL_Sprite" label is used by luaL
	// internally to identity things.

	Lua lua = Lua(L);
	lua.stackDump();

	luaL_newmetatable(L, "luaL_Sprite");
	lua.stackDump();
	luaL_setfuncs (L, sSpriteRegs, 0);
	lua.stackDump();
	lua_pushvalue(L, -1);
	lua.stackDump();
	lua_setfield(L, -1, "__index");
	lua.stackDump();
	lua_setglobal(L, "Sprite");
	lua.stackDump();
}

/////////////////////////////////
// Lua bindings for SpriteSheet class
/////////////////////////////////


int graphics::l_SpriteSheet_constructor(lua_State *L)
{
	cout << "Creating sprite sheet." << endl;
	SpriteSheet ** udata = (SpriteSheet **)lua_newuserdata(L, sizeof(SpriteSheet *));
	*udata = new SpriteSheet();
	luaL_getmetatable(L, "luaL_SpriteSheet");
	lua_setmetatable(L, -2);
	return 1;
}

int graphics::l_SpriteSheet_destructor(lua_State *L)
{
	return 1;
}

int graphics::l_SpriteSheet_loadImage(lua_State *L)
{
	SpriteSheet* sheet = l_checkSpriteSheet(L,1);
	string filename = getArgString(2,L);
	cout << "Loading image to spritesheet " << filename << " at address " << sheet << endl;  
	sheet->loadUncompressedTGA((char*)filename.c_str());
	return 0;
}

//Lua call addFrame(sequence,x,y,height,width)

int graphics::l_SpriteSheet_addFrame(lua_State *L)
{
	SpriteSheet* sheet = l_checkSpriteSheet(L,1);
	string sequence = getArgString(2,L);
	Point2f topleft = Point2f(getArgNumber(3,L),getArgNumber(4,L));
	int height = getArgNumber(5,L);
	int width  = getArgNumber(6,L);

	cout << "Adding frame to sequence " << sequence << " with height of " << height << " and width of " << width << endl;
	sheet->addFrame(sequence,SpriteFrame(height,width,sheet,topleft));
	return 1;
}

void graphics::registerSpriteSheet(lua_State *L)
{
	
	luaL_Reg sSpriteSheetRegs[] =
	{
		{ "new",              l_SpriteSheet_constructor},
		{ "addFrame",         l_SpriteSheet_addFrame},
		{ "loadImage",        l_SpriteSheet_loadImage},
		{ "__gc",             l_Sprite_destructor},
		{NULL,NULL}
	};
	// Create a luaL metatable. This metatable is not
	// exposed to Lua. The "luaL_Sprite" label is used by luaL
	// internally to identity things.

	Lua lua = Lua(L);
	lua.stackDump();

	luaL_newmetatable(L, "luaL_SpriteSheet");
	lua.stackDump();
	luaL_setfuncs (L, sSpriteSheetRegs, 0);
	lua.stackDump();
	lua_pushvalue(L, -1);
	lua.stackDump();
	lua_setfield(L, -1, "__index");
	lua.stackDump();
	lua_setglobal(L, "SpriteSheet");
	lua.stackDump();
}

SpriteSheet* graphics::l_checkSpriteSheet(lua_State *L, int n)
{
	 return *(SpriteSheet **)luaL_checkudata(L, n, "luaL_SpriteSheet");
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
	if(!texturestream.good())
		cout << "Warning: Failed to open texture " << filename << endl;
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
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);
	delete imagedata;
	this->height = height;
	this->width = width;
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

		if(compiled != GL_TRUE)
			cout << "Warning: Shader failed to compile!" << endl;
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
        if (linked != GL_TRUE)
            cout << "Warning: Shader program failed to link" << endl;

	}

ShaderProgram::~ShaderProgram() {
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
	}

void ShaderProgram::bindAttributes() {

		
	}


bool graphics::Init()
{
	 if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )					 // Initialize SDL
        return false;
    while(true)
    {
        if(SDL_WasInit(SDL_INIT_VIDEO) != 0)
            break;
        else
          cout << "SDL: Still not initialized!" << endl;
    }
    SDL_WM_SetCaption( "Dragon Dildos 2: The Dickening", NULL );

    if( SDL_SetVideoMode( SCREEN_WIDTH*1, SCREEN_HEIGHT*1, SCREEN_BPP, SDL_OPENGL) == NULL )	// Set window properties, OpenGL is passed here
    {
		cout << SDL_GetError() << endl;
        return false;
    }
    
    cout << "SDL: Video mode set!" << endl;

    SDL_EnableUNICODE(1);
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );				// Activate double buffer for buffer switching
    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );				// Activate swap control, also for buffer switching
	
	glEnable(GL_TEXTURE_2D);
	
	//glClearColor(0.0,0.0,0.0,1.0 ); 

                                                     // Set clear color.  This is what the buffer gets filled with when we call glClear
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    glEnable (GL_BLEND);										//Enable blending
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);         //Set blending for our alpha-enabled textures
                                                      
    if( glGetError() != GL_NO_ERROR )						    // If we get any errors, return false.
    {
        return false;    
    }

    GLenum err = glewInit();

    if(GLEW_OK != err)
        std::cout << "Error: " << glewGetErrorString(err) << endl;

    if (!glewIsSupported("GL_ARB_fragment_program") || 
		!glewIsSupported("GL_ARB_vertex_program") ||
		!glewIsSupported("GL_ARB_vertex_buffer_object") ||
		!glewIsSupported("GL_ARB_shading_language_100"))
    {  
	   cout << "Wex: Switching to compatibility rendering mode." << endl;
	   renderer->setRenderMode(WEX_VERTEX_ARRAYS);
    }
	else
	{
		cout << "Wex: Requisite extensions present, using normal rendering mode." << endl;
		renderer->setRenderMode(WEX_VERTEX_BUFFER_OBJECTS);

	   int major, minor, revision;
       const GLubyte* sVersion = glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
       if (glGetError() == GL_INVALID_ENUM)
       {
          major = 1; minor = 0; revision=51;
       }
       else
       {
          cout << "OpenGL: Using shading language " << sVersion << endl;
       }
	cout << "Glew: Successfully initialized." << endl;
	graphics::loadAssets();
	}
	return true;
}

void graphics::loadAssets()
{

  SpriteSheet uispritesheet;

  SpriteFrame tempframe;
  uispritesheet.loadUncompressedTGA("uisprites.tga");

  tempframe = SpriteFrame(4,4,&uispritesheet,Point2f(0,10));
  uispritesheet.addFrame("horizontal border",tempframe);
  tempframe = SpriteFrame(4,4,&uispritesheet,Point2f(5,10));
  uispritesheet.addFrame("vertical border",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(32,0));
  uispritesheet.addFrame("constitution icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(48,0));
  uispritesheet.addFrame("speed icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(48,16));
  uispritesheet.addFrame("intelligence icon",tempframe);
  tempframe = SpriteFrame(16,16,&uispritesheet,Point2f(32,16));
  uispritesheet.addFrame("strength icon",tempframe);
  tempframe = SpriteFrame(4,4,&uispritesheet,Point2f(0,0));
  uispritesheet.addFrame("top left corner",tempframe);
  tempframe = SpriteFrame(4.0,4.0,&uispritesheet,Point2f(5,0));
  uispritesheet.addFrame("top right corner",tempframe);
  tempframe = SpriteFrame(4.0,4.0,&uispritesheet,Point2f(5,5));
  uispritesheet.addFrame("bottom right corner",tempframe);
  tempframe = SpriteFrame(4.0,4.0,&uispritesheet,Point2f(0,5));
  uispritesheet.addFrame("bottom left corner",tempframe);

  Texture* blanktex = new Texture();
  blanktex->createEmptyTexture(2,2);

  graphics::textures["blank"] = blanktex;
  graphics::textures["ui"] = new SpriteSheet(uispritesheet);
  graphics::textures["uifont"] = new TextureFont("uifont.tga",192,128);

  cout << "Assets successfully loaded" << endl;

}


Renderer::Renderer()
{
}

void Renderer::setRenderMode(RenderMode mode)
{
	if(mode == WEX_VERTEX_BUFFER_OBJECTS)
	{
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
		
		glBindBuffer(GL_ARRAY_BUFFER,vbovertex);

		glEnableVertexAttribArray(vertexattriboffset);
		glVertexAttribPointer(vertexattriboffset,3, GL_FLOAT,0,sizeof(Vertex),(void*)0);

		glEnableVertexAttribArray(texcoordattriboffset);
		glVertexAttribPointer(texcoordattriboffset,2, GL_FLOAT, 0,sizeof(Vertex),(void*)12);

		glEnableVertexAttribArray(colorattriboffset);
		glVertexAttribPointer(colorattriboffset,4,GL_FLOAT,GL_TRUE,sizeof(Vertex),(void*)20);
	}
	if(mode == WEX_VERTEX_ARRAYS)
	{
		glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT ); 
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 ); 
	    glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glClearColor(1,1,1,1);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Vertex),spritebatch.getVertbuffer());
		glDisableClientState(GL_VERTEX_ARRAY);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex),&spritebatch.getVertbuffer()->texcoord);

		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT,sizeof(Vertex),&spritebatch.getVertbuffer()->color);
		glDisableClientState(GL_COLOR_ARRAY);
		
	}

	this->rendermode = mode;
}

void Renderer::setDefaultRendering(bool state)
{
	defaultshader->enable(state);
}

void Renderer::zoom(float factor)
{
	defaultshader->zoom(factor);
}

bool Renderer::changeTexHandle(int handle)
{
	if(this->currenttex != handle)
	{
		drawBuffer();
		spritebatch.reset();
		currenttex = handle;
		glBindTexture(GL_TEXTURE_2D,handle);
		return true;
	}
	else
		return false;
}

void Renderer::drawSprite(Sprite sprite,Point3f position,double xscale,double yscale,double rotate)
{

	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(sprite.getTexHandle());
	spritebatch.addToBuffer(sprite,position-view,xscale,yscale,rotate);
}

void Renderer::drawSprite(Sprite sprite,Point3f position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(sprite.getTexHandle());
	spritebatch.addToBuffer(sprite,position-view,1.0,1.0,0);
}

void Renderer::drawFixedSprite(Sprite sprite,Point3f position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(sprite.getTexHandle());
	spritebatch.addToBuffer(sprite,position);
}

void Renderer::drawFixedGlyph(Glyph glyph,Point3f position)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(glyph.getTexHandle());
	spritebatch.addToBuffer(glyph,position);
}

void Renderer::drawFixedSprite(Sprite sprite,Point3f position,double xscale,double yscale,double rotate)
{
	if(spritebatch.isFull())
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(sprite.getTexHandle());
	spritebatch.addToBuffer(sprite,position,xscale,yscale,rotate);
}

void Renderer::drawQuad(Quad quad,Point3f position,double xscale,double yscale,double rotate)
{
	if(spritebatch.isFull() || changeTexHandle(::textures["blank"]->getTexId()))
	{
			drawBuffer();
			spritebatch.reset();
	}
	changeTexHandle(::textures["blank"]->getTexId());
	spritebatch.addToBuffer(quad,position,xscale,yscale,rotate);
}

void Renderer::drawBuffer()
{
	
	if(spritebatch.getBufferLength() > 0)
	{	
		if(rendermode ==  WEX_VERTEX_BUFFER_OBJECTS)
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbovertex);
			glBufferData( GL_ARRAY_BUFFER,sizeof(Vertex)*spritebatch.getBufferLength(),spritebatch.getVertbuffer(),GL_STREAM_DRAW);
			glDrawArrays(GL_QUADS,0,spritebatch.getBufferLength());
		}
		else if(rendermode == WEX_VERTEX_ARRAYS)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			
			glDrawArrays(GL_QUADS, 0, spritebatch.getBufferLength());
			
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			cout << "Drawing Arrays" << endl;
		}

		spritebatch.reset();
	}
	
}

void Renderer::drawText(std::string fontname,std::string text,Point3f position,ColorRGBA color, GLfloat space)
{
	TextureFont* font = (TextureFont*)graphics::textures[fontname];
	
	Glyph character(font->getCharHeight(),font->getCharWidth());
	character.setColor(color);
	character.setFont(font);

	for(float i = 0; i < text.size(); i+=1.0)
	{
		character.setGlyph(text[i]);
        drawFixedGlyph(character,Point3f(position.x+((float)i*space),position.y,0));
	}
}

void Renderer::drawFormattedText(std::string fontname,std::string text,Point3f position,ColorRGBA color, GLfloat space,int linelength)
{
	//Determine how much of the text we can render properly on one line.
	TextureFont* font = (TextureFont*)graphics::textures[fontname];
	
	Glyph character(font->getCharHeight(),font->getCharWidth());
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



