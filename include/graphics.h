#pragma once
# define NDEBUG
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <fstream>
#include "SDL/SDL.h"
#include "ft2build.h"
#include FT_FREETYPE_H
//#include "json/libjson.h"
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

    bool Init();

	enum imageformat
    {
    	TYPE_TGA,
    	TYPE_RAW
    };	

	typedef Point2d TexCoord;

	struct SpriteFrame
	{
		TexCoord texcoords[4];
		float height;
		float width;
	};

	struct ColorRGBA
	{
		GLfloat _r;
		GLfloat _g;
		GLfloat _b;
		GLfloat _a;
		ColorRGBA();
		ColorRGBA(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	};

	class Texture
	{
	private:
	    GLuint texid;
		GLuint height;
	    GLuint width;
	public:
		GLuint getHeight();
		GLuint getWidth();
		GLuint getTexId();
	    void createEmptyTexture(int height,int width);
		bool loadUncompressedTGA(char *filename);
		Texture();

	};

    class SpriteSheet : public Texture 
    {
    private:
      map<string,vector<SpriteFrame>> sequences;
    public:
	  unsigned int getSequenceLength(string sequence);
      void setFrame(string sequence,unsigned int framenum,SpriteFrame frame);
	  SpriteFrame getFrame(string sequence,unsigned int framenum);
      SpriteSheet();
    };

	class Quad
	{
	protected:
		double       height;
	    double       width;
	    ColorRGBA    color;
	public:
		ColorRGBA     getColor();
	    void          setColor(ColorRGBA color);
		double        getHeight();
		void          setHeight(double height);
		double        getWidth();
		void          setWidth(double width);
		Point3d       getMidPoint();

		Quad();
		Quad(double height,double width,ColorRGBA color);
	};

	class Sprite : public Quad
	{
	private:
	  Point3d      offset;
	  string       sequence;
	  unsigned int sequencelength;
	  double       frametime;
	  double       accumulator;
	  bool         playing;
	  unsigned int frame;
	  SpriteSheet *targetspritesheet;

	public:
	  Sprite();
	  SpriteSheet*  getSpriteSheet();
	  void          setSpriteSheet(string name);
	  SpriteFrame   getSpriteFrame();
	  void          setSpriteFrame(unsigned int frame);
	  Point3d       getOffset();
	  void          setOffset(Point3d offset);

	  void play(double delta);
	  void playOnce();
	  void stop();
	  void rewind();
	  void changeSequence(string sequence);
	  void changeSpriteSheet(SpriteSheet *newsheet);
	  void changeSpriteSheetNoRewind(SpriteSheet *newsheet);
	};

	class Font : public SpriteSheet
	{
	private:
	public:

	};

	class BitmapFont
	{
	private:       
		GLint  _handle;                        // handle to the font texture in openGL
	public:
		SpriteSheet _fonttexture;
		
		void buildFont(Texture& texture);      // generates font
		BitmapFont();                          // default constructor
		BitmapFont(Texture& texture);          // generates font on construction
	};

	struct VertexData
	{
	    GLfloat x,y,z;
		GLfloat u,v;
		GLfloat r,g,b,a;
	};

	class SpriteBatch
	{
	private:
		Point3d    vertexbuffer[BUFFER_SIZE];
		TexCoord   texcoordbuffer[BUFFER_SIZE];
		ColorRGBA  colorbuffer[BUFFER_SIZE];
		GLint      bufferpos;
	
	public:
	
		void       addToBuffer(Sprite* sprite,Point3d position,double xscale,double yscale,double rotate);
		void       addToBuffer(Quad* quad,Point3d position,double xscale,double yscale,double rotate);

		Point3d*   getVertbuffer();
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
		map<string,SpriteSheet> _sprites;
		map<string,GLint>  _shaders;

	public:
		static Resources _resourcemanager;

		void loadAsTexture(string filename);
		void loadSpriteSheetDefinition(string filename);
		void loadShader(string vertexfilename,string fragmentfilename);
	};

	class Renderer
	{
	private:
		Renderer();
		Renderer&         operator=(Renderer const&){};

		Point3d          view;
		SpriteBatch      spritebatch;
		static Renderer* instance;

		GLuint           vbovertex;
		GLuint           vbotexture;
		GLuint           vbocolor;
		
	
	public:
		static  Renderer* Instance();

		void    setCamera(Point3d cameraposition);
		void    moveCameraRelative(Point3d movementvector);
		void    moveCameraTowards(Point3d position);

	  	void    drawSprite(Sprite* sprite,Point3d position);
		void    drawSprite(Sprite* sprite,Point3d position,double xscale,double yscale,double rotate);
		void    drawFixedSprite(Sprite* sprite,Point3d position);
		void    drawFixedSprite(Sprite* sprite,Point3d position,double xscale,double yscale,double rotate);
	    void    drawQuad(Quad* quad,Point3d position,double xscale,double yscale,double rotate);


		void    drawText(std::string text, Point3d position, GLint space);                   //Draws white text.
		//void   drawText(std::string text, Point3d position, GLint space, ColorRGBA color);  //Draws colored text.
		void    drawBuffer();

		GLint   loadShader(std::string vertexfilename,std::string fragmentfilename);
		void    changeShader();
	};
};