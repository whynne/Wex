#pragma once
# define NDEBUG
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <fstream>
#include "SDL/SDL.h"
#include "json/libjson.h"
#include "opengl.h"
#include "timer.h" 
#include "vec.h"

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::ios;
using std::ios_base;
using std::ifstream;

const unsigned int BUFFER_SIZE = 4096;

namespace graphics
{
	enum imageformat
    {
    	TYPE_TGA,
    	TYPE_RAW
    };	

	struct TexCoord
	{
	  GLfloat u,v;
	};	

	struct ColorRGBA
	{
		float _r;
		float _g;
		float _b;
		float _a;
		ColorRGBA();
		ColorRGBA(float r, float g, float b, float a);
	};

	struct TexData
	{
	public:
	  int texid;
	  GLfloat height;
	  GLfloat width;
	};

    class Sprite
    {
    private:
    
      TexData*   texture;
      int        maxclips;
      int        clipheight,clipwidth;
      TexCoord** texcoords;
    
    public:
    
      TexCoord* getTexCoords(int frame);
      void      setClip(int frame,GLfloat x,GLfloat y);
      int       getMaxClips(){return maxclips;};
      int       getTexID(){return texture->texid;};
    
      Sprite();
      Sprite(TexData &mytexture,int frames,int height,int width);
    
    };

	class Animation
	{
	private:
	  Point3d _vertices[4];
	  ColorRGBA _color[4];
	  double frametime;
	  double accumulator;
	  bool playing;
	  int currentframe;
	  Sprite *currentsprite;

	public:
	  Animation();
	  Animation(double quadheight,double quadwidth);
	  Animation(double quadheight,double quadwidth,ColorRGBA color);
	  Sprite* getSprite();
	
	  TexCoord* getCurrentTexCoords(){return currentsprite->getTexCoords(currentframe);};
	  Point3d* getVertices(){return _vertices;};
	  ColorRGBA* getColor(){return _color;};
	  int getTexID(){return currentsprite->getTexID();};
	  
	  void setClipDimensions(double height, double width);
	  
	  void play(double delta);
	  void stop();
	  void rewind();
	  void changeSprite(Sprite *newsprite);
	  void changeSpriteNoRewind(Sprite *newsprite);
	};

	class BitmapFont
	{
	private:       
		GLint  _handle;                        // handle to the font texture in openGL
	public:
		Sprite _charactersprites[256];
		BitmapFont();                          // default constructor
		BitmapFont(TexData& texture);          // generates font on construction
		void buildFont(TexData& texture);      // generates font
	};

	class Texture
	{
	protected:
		GLubyte* imagedata;		        //Pointer for storing image data
		GLuint bpp;                     //Bits per pixel
		GLuint width;                   //Width of the entire image
		GLuint height;                  //Height of the entire image
		GLuint texID;                   //Texture ID for use with glBindTexture
		GLuint type;                    //Data stored in * imagedata (GL_RGB or GL_RGBA)
		GLuint bytesperpixel;           //BYTES per pixel
		GLuint imagesize;               //Size in bytes of texture

		TexData texdata;
	public:
		TexData createEmptyTexture(int height,int width);
	};

	class TGA: public Texture
	{
	private:
		//Headers for the tga texture
		GLubyte header[12];
		GLubyte header2[6];
		
	    //These are byte arrays to be compared to the header of our TGA file
		GLubyte uncompressedheader[12];
		GLubyte compressedheader[12];
	
		
	    //Our actual loading functions.  Our public function will determine which of these to use
		bool LoadUncompressedTGA(char *filename,std::ifstream &texturestream);
		bool LoadCompressedTGA(char *filename,std::ifstream &texturestream);
	
	public:
		//Our public loading function
		TGA();
		
	    TexData  loadImage(char *filename);
	    
	    GLubyte* getImageData(){return imagedata;};
	    TexData  getTexData();
	};

	class SpriteBatch
	{
	private:
	
		Point3d*   _tempvertbuf;
		TexCoord*  _temptexbuf;
		ColorRGBA* _tempcolorbuf;
		Point3d    _vertexbuffer[BUFFER_SIZE];
		TexCoord   _texcoordbuffer[BUFFER_SIZE];
		ColorRGBA  _colorbuffer[BUFFER_SIZE];
		GLint      _bufferpos;
	
	public:
	
		void       addToBuffer(Animation* animation,Point3d position);
		Point3d*   get_vertbuffer();
		TexCoord*  getTexCoordBuffer();
		ColorRGBA* getColorBuffer();
		GLint      getBufferLength();
		bool       isFull();
		void       reset();
		
		SpriteBatch();
	};

	class Resources
	{
	private:
		Resources();
		Resources&   operator=(Resources const&){};
		map<string,GLint>  _textures;
		map<string,Sprite> _sprites;
		map<string,GLint>  _shaders;

	public:
		static Resources _resourcemanager;

		void loadAsTexture(string filename);
		void loadSpriteDefinition(string filename);
		void loadShader(string vertexfilename,string fragmentfilename);
	};

	class Renderer
	{
	private:
		Renderer();
		Renderer&         operator=(Renderer const&){};

		Point3d          _camera;
		SpriteBatch      _vertbuffer;
		static Renderer* _singletonRenderer;
		
	
	public:
	
		static  Renderer* Instance();
		void    setCamera(Point3d cameraposition);
		void    moveCameraRelative(Point3d movementvector);
		void    moveCameraTowards(Point3d position);

	  	void    draw(Animation* animation, Point3d position); 
		void    drawFixed(Animation* animation,Point3d position);
			    
		void    drawText(std::string text, Point3d position, GLint space);                   //Draws white text.
		//void    drawText(std::string text, Point3d position, GLint space, ColorRGBA color);  //Draws colored text.
		void    drawBuffer();    // Adds to buffer

		// Shader shit

		GLint   loadShader(std::string vertexfilename,std::string fragmentfilename);
		void    changeShader();
	};
};