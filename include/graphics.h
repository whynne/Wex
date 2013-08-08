// ============================================================================
// Copyright (c) 2012 Carlos Ramirez
//
// Shader & ShaderProgram classes modified from MIT-licensed code (c) 2011-2012 
// by J.C. Moyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ============================================================================

#pragma once

#include "begincode.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>C
#include <list>
#include <assert.h>
#include <fstream>
#include "SDL/SDL.h"
#include "opengl.h"
#include "timer.h" 
#include "vec.h"
#include "matrix.h"

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::ios;
using std::ios_base;
using std::ifstream;
using wex::Point3f;
using wex::Point3d;
using wex::Point2f;

const unsigned int SCREEN_HEIGHT = 720;
const unsigned int SCREEN_WIDTH = 960;
const unsigned int SCREEN_BPP = 32;
const unsigned int BUFFER_SIZE = 14400;

#define renderer wex::graphics::Renderer::Instance()

namespace wex
{
	namespace graphics
	{
		class Renderer;
		class Texture;
		class Glyph;
		class SpriteSheet;
		class TextureFont;
		class ShaderProgram;
		class SpriteBatch;
		class Sprite;
		class Polygon;
		class Quad;
	
	    bool Init();
	
		extern map<string,Texture*>       textures;
	    extern map<string,ShaderProgram*>  shaders;
	
	
	    enum imageformat
	    {
	        TYPE_TGA,
	        TYPE_RAW
	    };    
	
	    typedef Point2f TexCoord;
	
	    struct WEXAPI SpriteFrame
	    {
	        TexCoord texcoords[4];
	        float height;
	        float width;
			SpriteFrame(float height,float width,Texture* texture,Point2f topleft);
			SpriteFrame();
	    };
	
	    struct WEXAPI ColorRGBA
	    {
	        GLfloat _r;
	        GLfloat _g;
	        GLfloat _b;
	        GLfloat _a;
	        ColorRGBA();
	        ColorRGBA(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	    };
	
	    class WEXAPI Texture
	    {
		protected:
	        GLuint texid;
	        float height;
	        float width;
	    public:
	        GLuint getHeight();
	        GLuint getWidth();
	        GLuint getTexId();
	        void createEmptyTexture(int height,int width);
	        bool loadUncompressedTGA(char *filename);
	        Texture();
	
	    };
	
	    class WEXAPI SpriteSheet : public Texture 
	    {
	    private:
	      map<string,vector<SpriteFrame>> sequences;
	    public:
	      unsigned int getSequenceLength(string sequence);
	      void addFrame(string sequence,SpriteFrame frame);
	      SpriteFrame getFrame(string sequence,unsigned int framenum);
	      SpriteSheet();
	    };
	
		struct y_compare {
			bool operator() (const Point2f& lhs, const Point2f rhs) const{
				return lhs.y < rhs.y;
			}
		};
	
		//Note: verts must be ordered for drawing.  Will implement
		//triangulator later
	
		class WEXAPI Polygon
		{
		public:
	        std::list<Point2f> verts;
	        ColorRGBA          color;
		public:
			void addVertex(Point2f vert);
	        Polygon();
		};
	
	    class WEXAPI Quad
	    {
		public:
	        Point2f        topleft;
			Point2f        topright;
			Point2f        bottomright;
			Point2f        bottomleft;
			Point3f        midpoint;
	        ColorRGBA      color;
	    public:
	        ColorRGBA     getColor();
	        void          setColor(ColorRGBA color);
			float         getHeight(){return bottomright.y;};
			void          setHeight(float height);
			float         getWidth(){return topright.x;};
			void          setWidth(float width);
	        inline Point3f getMidPoint();
	
	        Quad();
	        Quad(double height,double width,ColorRGBA color);
	    };
	
	    class WEXAPI Sprite : public Quad
	    {
	    private:
	      Point3f      offset;
	      string       sequence;
	      unsigned int sequencelength;
	      double       frametime;
	      double       accumulator;
	      bool         playing;
	      unsigned int frame;
	      SpriteSheet *targetspritesheet;
	
	    public:
	      Sprite();
		  ~Sprite();
		  Sprite(float height,float width);
	
		  int           getTexHandle(){return targetspritesheet->getTexId();};
	      SpriteSheet*  getSpriteSheet();
	      void          setSpriteSheet(string name);
		  void          setSpriteSheet(SpriteSheet* spritesheet);
	      SpriteFrame   getSpriteFrame();
	      void          setSpriteFrame(unsigned int frame);
	      Point3f       getOffset();
	      void          setOffset(Point3f offset);
	
	      void play(double delta);
	      void playOnce();
	      void stop();
	      void rewind();
		  void setWidth(int width);
		  void setHeight(int height);
	      void changeSequence(string sequence);
	      void changeSpriteSheet(SpriteSheet* newsheet);
		  void changeSpriteSheet(char* name);
	      void changeSpriteSheetNoRewind(SpriteSheet *newsheet);
	    };
	
	    class WEXAPI TextureFont : public SpriteSheet
	    {
	    private:
			int charheight;
			int charwidth;
	        SpriteFrame glyphs[256];
	    public:
			SpriteFrame getFrame(int framenum);
	        void buildFont(Texture& texture);      // generates font
	        TextureFont();                          // default constructor
	        TextureFont(char *filename,int height,int width);          // generates font on construction
			int getCharHeight(){return charheight;};
			int getCharWidth(){return charwidth;};
	    };
	
		class WEXAPI Glyph : public Quad
		{
		private:
			char character;
			TextureFont* font;
		public:
			int getTexHandle(){return font->getTexId();};
			void setFont(TextureFont* font);
			SpriteFrame getGlyph();
			void setGlyph(char character);
			Glyph(float height,float width);
		};
	
	    struct WEXAPI VertexData
	    {
	        GLfloat x,y,z;
	        GLfloat u,v;
	        GLfloat r,g,b,a;
	    };
	
		struct WEXAPI Vertex{
			Point3f  point;
			TexCoord texcoord;
			ColorRGBA color;
		};
	
		class WEXAPI PolygonBatch
		{
		private:
			vector<Polygon> polygons;
			Vertex         vertexbuffer[BUFFER_SIZE];	
			GLint          bufferpos;
		public:   
	        inline void addToBuffer(Polygon polygon, Point3d position,double xscale,double yscale,double rotate);
			inline void addToBuffer(Polygon polygon, Point3d position);
	        Vertex*     getVertbuffer();
	        GLint       getBufferLength();
	        int         remainingSpace();
	        void        reset();
			PolygonBatch();
		};
	
	    class WEXAPI SpriteBatch
	    {
	    private:
			vector<Sprite> sprites;
			Vertex     vertexbuffer[BUFFER_SIZE];		
	        GLint      bufferpos;
	    
	    public:
	    
	        inline void addToBuffer(Sprite sprite,Point3f position,double xscale,double yscale,double rotate);
	        inline void addToBuffer(Quad quad,Point3f position,double xscale,double yscale,double rotate);
			inline void addToBuffer(Glyph glyph,Point3f position);
			inline void addToBuffer(Sprite sprite,Point3f position);
	        Vertex*    getVertbuffer();
	        GLint      getBufferLength();
	        bool       isFull();
	        void       reset();
	        
	        SpriteBatch();
	    };
	
	
	
	    class WEXAPI Shader {
	        GLchar* source;
	        unsigned int handle;
	    public:
	        void loadFromFile(char* filename,GLenum type);
	        Shader();
	        Shader(char* filename,GLenum type);
	        ~Shader();
	        inline unsigned int GetHandle() const;
	    };
	
	    inline unsigned int Shader::GetHandle() const {
	        return handle;
	    }
	
	    extern const char* SAMPLER_NAME;
	    extern const char* TFS_TEXCOORD;
	    extern const char* TFS_COLOR;
	
	    extern const char* TVS_PROJECTION_MAT;
	    extern const char* TVS_MODEL_VIEW_MAT;
	    extern const int   VERTEX_ATTRIBUTE_ID;
	    extern const char* VERTEX_ATTRIBUTE_NAME;
	    extern const int   TEXCOORD_ATTRIBUTE_ID;
	    extern const char* TEXCOORD_ATTRIBUTE_NAME;
	    extern const int   COLOR_ATTRIBUTE_ID;
	    extern const char* COLOR_ATTRIBUTE_NAME;
	
	
	    class WEXAPI ShaderProgram {
	        unsigned int handle;
	        Shader* vs;
	        Shader* fs;
	        int width, height;
	        unsigned int tex_handle;
	        unsigned int shader_tex_handle;
	        unsigned int shader_mat_proj_handle;
	        unsigned int shader_mat_mv_handle;
	    public:
	        ShaderProgram(char* vsfilename,char* fsfilename);
	        ShaderProgram();
	        ~ShaderProgram();
	        void enable(bool state);
	        inline unsigned int getHandle() const;
	        inline void setTexture(unsigned int tx_handle);
	        inline void setOutputSize(int width, int height);
			void zoom(float factor);
	    private:
	        void getUniformHandles();
	        void bindAttributes();
	    };
	
	    // Inline functions
	
	    inline unsigned int ShaderProgram::getHandle() const {
	        return handle;
	    }
	
	    inline void ShaderProgram::setTexture(unsigned int handle) {
	        tex_handle = handle;
	    }
	
	    inline void ShaderProgram::setOutputSize(int width, int height) {
	        this->width  = width;
	        this->height = height;
	    }
	
		enum WEXAPI RenderMode
		{
			WEX_VERTEX_BUFFER_OBJECTS,
			WEX_VERTEX_ARRAYS
		};
	
	    class WEXAPI Renderer
	    {
	    private:
			int currenttex;
			bool changeTexHandle(int handle);
	
	        Renderer();
	        Renderer&         operator=(Renderer const&){return *this;};
	
	        Point3f          view;
			PolygonBatch     polybatch;
	        SpriteBatch      spritebatch;
	        static Renderer* instance;
			RenderMode       rendermode;
	
	        GLuint           vbovertex;
	        GLuint           vbotexture;
	        GLuint           vbocolor;
	
	    public:
	        static  Renderer* Instance();
			void    setRenderMode(RenderMode mode);
			void    zoom(float factor);
	        void    setCamera(Point3f cameraposition);
	        void    moveCameraRelative(Point3f movementvector);
	        void    moveCameraTowards(Point3f position);
			void    setDefaultRendering(bool state);
	        void    drawSprite(Sprite sprite,Point3f position);
	        void    drawSprite(Sprite sprite,Point3f position,double xscale,double yscale,double rotate);
	        void    drawFixedSprite(Sprite sprite,Point3f position);
	        void    drawFixedSprite(Sprite sprite,Point3f position,double xscale,double yscale,double rotate);
	        void    drawFixedGlyph(Glyph glyph,Point3f position);
			void    drawQuad(Quad quad,Point3f position,double xscale,double yscale,double rotate);
			void    changeTexture(int texhandle);
			void    changeTexture(char* name);
			void    changeShader(char* name);
	
	        void    drawText(char* fontname,const char* text, Point3f position,ColorRGBA color,GLfloat space);                   //Draws white text.
	        void    drawFormattedText(char* fontname,const char* text,Point3f position,ColorRGBA color, GLfloat space,int linelength);
			void    drawBuffer();
	    };
	
		void WEXAPI printShaderList();
		void WEXAPI printTextureList();

		void WEXAPI loadAssets();
	}
}

#include "closecode.h"