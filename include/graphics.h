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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
#include <fstream>
#include "SDL2/SDL.h"
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
	
		extern SDL_GLContext		glcontext;
		extern SDL_Window*			window;
		
		extern map<string,Texture*>       textures;
		extern map<string,ShaderProgram*>  shaders;
	
	
		enum imageformat
		{
			TYPE_TGA,
			TYPE_RAW
		};    
	
		typedef Point2f TexCoord;
	
		struct SpriteFrame
		{
			TexCoord texcoords[4];
			float height;
			float width;
			SpriteFrame(float height,float width,Texture* texture,Point2f topleft);
			SpriteFrame();
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
		protected:
			GLuint texid;
			float height;
			float width;
		public:
			GLuint getHeight();
			GLuint getWidth();
			GLuint getTexId();
			void createEmptyTexture(int height,int width);
			bool loadUncompressedTGA(const char *filename);
			Texture();
	
		};
	
		class SpriteSheet : public Texture 
		{
		private:
		  map<string,vector<SpriteFrame> > sequences;
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
	
		class Polygon
		{
		public:
			std::list<Point2f> verts;
			ColorRGBA          color;
		public:
			void addVertex(Point2f vert);
			Polygon();
		};
	
		class Quad
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
	
		class Sprite : public Quad
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
		  void changeSpriteSheet(const char* name);
		  void changeSpriteSheetNoRewind(SpriteSheet *newsheet);
		};
	
		class TextureFont : public SpriteSheet
		{
		private:
			int charheight;
			int charwidth;
			SpriteFrame glyphs[256];
		public:
			SpriteFrame getFrame(int framenum);
			void buildFont(Texture& texture);      // generates font
			TextureFont();                          // default constructor
			TextureFont(const char *filename,int height,int width);          // generates font on construction
			int getCharHeight(){return charheight;};
			int getCharWidth(){return charwidth;};
		};
	
		class Glyph : public Quad
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
	
		struct VertexData
		{
			GLfloat x,y,z;
			GLfloat u,v;
			GLfloat r,g,b,a;
		};
	
		struct Vertex{
			Point3f  point;
			TexCoord texcoord;
			ColorRGBA color;
		};
	
		class PolygonBatch
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
	
		class SpriteBatch
		{
		private:
			vector<Sprite> sprites;
			Vertex     vertexbuffer[BUFFER_SIZE];		
			GLint      bufferpos;
		
		public:
		
			inline void addToBuffer(Sprite sprite,Point3f position,double xscale,double yscale,double rotate);
			inline void addToBuffer(Quad quad,Point3f position,double xscale,double yscale,double rotate);
			inline void addToBuffer(Glyph glyph,Point3f position);
			inline void addToBuffer(Glyph glyph,Point3f position, double xscale, double yscale, double rotate);
			inline void addToBuffer(Sprite sprite,Point3f position);
			Vertex*    getVertbuffer();
			GLint      getBufferLength();
			bool       isFull();
			void       reset();
			
			SpriteBatch();
		};
	
	
	
		class Shader {
			GLchar* source;
			unsigned int handle;
		public:
			void loadFromFile(string filename,GLenum type);
			Shader();
			Shader(string filename,GLenum type);
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
	
	
		class ShaderProgram {
			unsigned int handle;
			Shader* vs;
			Shader* fs;
			int width, height;
			unsigned int tex_handle;
			unsigned int shader_tex_handle;
			unsigned int shader_mat_proj_handle;
			unsigned int shader_mat_mv_handle;
		public:
			ShaderProgram(string vsfilename,string fsfilename);
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
	
		enum RenderMode
		{
			WEX_VERTEX_BUFFER_OBJECTS,
			WEX_VERTEX_ARRAYS
		};
	
		class Renderer
		{
		private:
			int currenttex;
			bool changeTexHandle(int handle);
	
			Renderer();
			Renderer&         operator=(Renderer const&){return *this;};
	
			Point3f				view;
			PolygonBatch		polybatch;
			SpriteBatch			spritebatch;
			static Renderer*	instance;
			RenderMode			rendermode;
	
			GLuint				vbovertex;
			GLuint				vbotexture;
			GLuint				vbocolor;
	
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
			void    drawFixedGlyph(Glyph glyph, Point3f position, GLfloat xscale, GLfloat yscale, GLfloat rotate);
			void    drawQuad(Quad quad,Point3f position,double xscale,double yscale,double rotate);
			void    changeTexture(int texhandle);
			void    changeTexture(const char* name);
			void    changeShader(const char* name);
	
			void    drawText(const char* fontname,const char* text,Point3f position,ColorRGBA color, GLfloat space, GLfloat scale);               //Draws white text.
			void    drawFormattedText(const char* fontname,const char* text,Point3f position,ColorRGBA color, GLfloat space, GLfloat scale, int linelength);
			void    drawBuffer();
		};
	
		void printShaderList();
		void printTextureList();

		void loadAssets();
	}
}

#endif
