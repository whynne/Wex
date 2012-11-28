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
#define NDEBUG
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include <fstream>
#include "SDL/SDL.h"
#include "ft2build.h"
#include FT_FREETYPE_H
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

const unsigned int SCREEN_HEIGHT = 500;
const unsigned int SCREEN_WIDTH = 500;
const unsigned int SCREEN_BPP = 32;
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

    class Shader {
        GLchar* source;
        unsigned int handle;
    public:
        void loadFromFile(std::string filename,GLenum type);
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
        ShaderProgram(std::string vsfilename,std::string fsfilename);
        ShaderProgram();
        ~ShaderProgram();
        void enable(bool state);
        inline unsigned int getHandle() const;
        inline void setTexture(unsigned int tx_handle);
        inline void setOutputSize(int width, int height);
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

	extern map<string,graphics::Texture>        textures;
    extern map<string,graphics::SpriteSheet>    spritesheets;
    extern map<string,graphics::ShaderProgram>  shaders;

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
		ShaderProgram    *defaultshader;
        
    
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
		void    changeTexture(int texhandle);
		void    changeTexture(std::string name);
		void    changeShader(std::string name);

        void    drawText(std::string text, Point3d position, GLint space);                   //Draws white text.
        void    drawBuffer();
    };
};