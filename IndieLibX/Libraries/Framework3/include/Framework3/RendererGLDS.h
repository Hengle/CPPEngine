#ifndef __RendererGLDS_H__
#define __RendererGLDS_H__

/* * * * * * * * * * * * * Author's note * * * * * * * * * * * *\
*
*  |_|	   |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|	*
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _	*
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_	*
*  |_|	   |_| |_|_ _ _|_| |_|	   |_| |_|_ _ _|_|  _ _ _ _|_|	*
*  |_|	   |_|	 |_|_|_|   |_|	   |_|	 |_|_|_|   |_|_|_|_|	*
*								*
*		      http://www.humus.name			*
*								 *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.		   *
*								 *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <STX/STX.h>
#include <Framework3/IRenderer.h>
#include <Framework3/RendererHLSLCg.h>
#ifdef HAS_CGGL
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) ((char *) 0 + (i))
#endif
/*
extern int GL_ARB_shading_language_100_supported;
extern int GL_ARB_fragment_shader_supported;
extern int GL_ARB_fragment_program_supported;
extern int GL_ARB_draw_buffers_supported;
extern int GL_EXT_texture_filter_anisotropic_supported;
*/
#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI 0x8837

#if !defined(D3D11) && !defined(D3D10) && !defined(D3D9)
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

typedef int GLsizei;

#endif
#ifndef RENDERERAPI
#define RENDERERAPI
#endif
struct RENDERERAPI AttribShaderGLSLGL3 : public Constant {
	AttribShaderGLSLGL3() : Constant(){name="";base=0;loc=0;}
	virtual ~AttribShaderGLSLGL3(){}
        std::string name;
        //std::vector<ubyte>
	float* base;
        //uint index;
        uint loc;
        //uint size;
	//uint offset;
        //ConstantType type;
		//CGparameterclass type;
        //int nElements;
        //bool dirty;
};
struct RENDERERAPI ConstantShaderGLSLGL3 : public Constant {
	ConstantShaderGLSLGL3() : Constant()
	{
		name="";
		data.resize(0);
		index=0;
		//type;
		nElements=0;
        	dirty=false;
	}
	virtual ~ConstantShaderGLSLGL3(){}
        std::string name;
        std::vector<ubyte> data;
        uint index;
        ConstantType type;
		//CGparameterclass type;
        int nElements;
        bool dirty;
};

struct RENDERERAPI SamplerShaderGLSLGL3 : public Sampler {
	SamplerShaderGLSLGL3() : Sampler(){name="";index=0;}
	virtual ~SamplerShaderGLSLGL3(){}
        std::string name;
        uint index;
};
#ifdef __APPLE__
#ifdef QT_BUILD
typedef void* GLhandleARB;
#else
//typedef void* GLhandleARB;
typedef void* GLhandleARB;
#endif
#else
typedef unsigned int GLhandleARB;
#endif

#if defined(__APPLE__) && !(defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
/** Convert a GLhandleARB to GLuint */
static inline GLuint handle_to_uint(GLhandleARB handle)
{
#ifdef __APPLE__
   /* As of glext.h version 20130624, on Mac OS X, GLhandleARB is defined
    * as a pointer instead of an unsigned int.  We use a union here to do
    * the conversion and assume that the bits we care about are in the least
    * significant bits of the handle, and we're on a little-endian system.
    */
   union handle_uint {
      GLhandleARB handle;
      GLuint ui;
   } temp;
   temp.handle = handle;
   return temp.ui;
#else
   return handle;
#endif
}
#else
   #define handle_to_uint
#endif

struct ShaderGLSLGL3 : public Shader
{
	ShaderGLSLGL3() : Shader()
	{
		nAttribs=0;
        	nUniforms=0;
        	nSamplers=0;
		HLSLFS="";
		GLSLFS="";
		HLSLVS="";
		GLSLVS="";

	}
	virtual ~ShaderGLSLGL3(){}

#if defined(__APPLE__) && !(defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
	GLhandleARB program;
        GLhandleARB vertexShader;
        GLhandleARB fragmentShader;
#else
	GLuint program;
        GLuint vertexShader;
        GLuint fragmentShader;
#endif

        std::vector<AttribShaderGLSLGL3  > attribs;
	std::vector<ConstantShaderGLSLGL3 > uniforms;
        std::vector<SamplerShaderGLSLGL3  > samplers;

	uint nAttribs;
        uint nUniforms;
        uint nSamplers;
		std::string HLSLFS;
		std::string GLSLFS;
		std::string HLSLVS;
		std::string GLSLVS;
};
#if !defined(_DEBUG)
#define glerror
#define checkGlError
#elif 0//defined(ANDROID)
#define glerror
#define checkGlError
#else
#define checkGlError { \
    GLint err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        LOG_PRINT("GL error:\nfile=%s:\nfn=%s:\nline=%d:\n 0x%08x\n", __FILE__, __FUNCTION__, __LINE__, err); \
    } \
}

#define glerror { \
    GLint err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        LOG_PRINT("GL error:\nfile=%s:\nfn=%s:\nline=%d:\n 0x%08x\n", __FILE__, __FUNCTION__, __LINE__, err); \
    } \
}
#endif
#ifndef _MSC_VER
extern int GL_ARB_shading_language_100_supported;
extern int GL_ARB_fragment_shader_supported;
extern int GL_ARB_fragment_program_supported;
extern int GL_ARB_draw_buffers_supported;
extern int GL_EXT_texture_filter_anisotropic_supported;
#endif

//using namespace GL2;
#if 0
struct CTextureProcessorGL: public CTextureProcessor
{
	CTextureProcessorGL(int& id):CTextureProcessor(id){}
	virtual ~CTextureProcessorGL(){}
};
struct CVertexBufferProcessorGL: public CVertexBufferProcessor
{
	CVertexBufferProcessorGL(int& id):CVertexBufferProcessor(id){}
	virtual ~CVertexBufferProcessorGL(){}
};
struct CIndexBufferProcessorGL: public CIndexBufferProcessor
{
	CIndexBufferProcessorGL(int& id):CIndexBufferProcessor(id){}
	virtual ~CIndexBufferProcessorGL(){}
};
#else
//include "ContentLoadersGL2.h"
#endif
//namespace CG
//{
//typedef struct RENDERERAPI _CGparameter *CGparameter;
//};
struct TextureGL: public Texture
{
	TextureGL():Texture()
	{
		glTexID=0;
		glDepthID=0;
		glTarget=0;
		glTargetW=0;
		components=0;
		wrap=0;
		glFormat=0;
		flags=0;
		lod=0.0f;
		mipMapped=false;
		SamplerStateID samplerState=-1;
		TU=0;
	}
	virtual ~TextureGL(){}

	//???union {
	GLuint glTexID;
	GLuint glDepthID;
	//???};
		//union {
	GLuint glTarget;
	GLuint glTargetW;
		//};
	GLuint components;
	GLuint wrap;
	GLuint glFormat;
	uint flags;
	float lod;
	bool mipMapped;
	SamplerStateID samplerState;
#ifdef HAS_CGGL
	CGparameter param;
#endif
	int TU;
	BYTE* Lock(int m, int& pitch)//int& slicePitch;
;
	void Unlock(int m)
;

};
struct SamplerStateGL: public SamplerState
{
	SamplerStateGL():SamplerState()
	{
		minFilter=0;
		magFilter=0;
		wrapS=0;
		wrapT=0;
		wrapR=0;
		aniso=0;
		lod=0.0f;
	}
	virtual ~SamplerStateGL(){}
	GLint minFilter;
	GLint magFilter;
	GLint wrapS;
	GLint wrapT;
	GLint wrapR;
	GLint aniso;
	float lod;
#ifdef HAS_CGGL
	CGparameter param;
#endif
};

struct Attrib {
	int stream;
	int size;
	AttributeFormat format;
	int offset;
};
#define MAX_GENERIC 8
#define MAX_TEXCOORD 8
struct VertexFormatGL: public VertexFormat
{
	VertexFormatGL():VertexFormat()
	{
		maxGeneric=0;
        	maxTexCoord=0;
		pBase=0;
	}
	virtual ~VertexFormatGL(){}
        Attrib generic[MAX_GENERIC];
        Attrib texCoord[MAX_TEXCOORD];
        Attrib vertex;
        Attrib normal;
        int vertexSize[MAX_VERTEXSTREAM];
        int maxGeneric;
        int maxTexCoord;
	ShaderID shader;
	BYTE* pBase;
};
struct VertexBufferGL: public VertexBuffer
{
	VertexBufferGL():VertexBuffer()
	{
		vboVB=0;
		size=0;
		data=0;
        	//data.resize(0);
	}
	virtual ~VertexBufferGL(){}
	GLuint vboVB;
	GLsizei size;
	BYTE* Lock();
	void Unlock();
	BYTE* data;
};
struct IndexBufferGL: public IndexBuffer
{
	IndexBufferGL():IndexBuffer()
	{
       		vboIB=0;
        	nIndices=0;
        	indexSize=0;
		data=0;
        	//data.resize(0);
	}
	virtual ~IndexBufferGL(){}
        GLuint vboIB;
        uint nIndices;
        uint indexSize;
	BYTE* Lock();
	void Unlock();
	//std::vector<BYTE> data;
	BYTE* data;
};
struct AlphaStateGL: public AlphaState
{
	AlphaStateGL():AlphaState()
{
		blendEnable=false;
		alphaFunc=0;
	}
	virtual ~AlphaStateGL(){}
	bool blendEnable;
	int alphaFunc;
};
struct BlendStateGL: public BlendState
{
	BlendStateGL():BlendState()
	{
		mask=0;
        	blendEnable=false;
       		alphaEnable=false;
		alphaToCoverageEnable=false;
	}
	virtual ~BlendStateGL(){}
        GLenum srcFactorRGB;
        GLenum dstFactorRGB;
        GLenum srcFactorAlpha;
        GLenum dstFactorAlpha;
        GLenum blendModeRGB;
        GLenum blendModeAlpha;
        int mask;
        bool blendEnable;
        bool alphaEnable;
	bool alphaToCoverageEnable;
};
struct DepthStateGL: public DepthState
{
	DepthStateGL():DepthState()
	{
        	depthFunc=0;
        	depthTest=false;
        	depthWrite=false;
	}
	virtual ~DepthStateGL(){}
        int depthFunc;
        bool depthTest;
        bool depthWrite;
};
struct RasterizerStateGL: public RasterizerState
{
	RasterizerStateGL():RasterizerState()
	{
		cullMode=0;
        	fillMode=0;
       		multiSample=false;
        	scissor=false;
	}
	virtual ~RasterizerStateGL(){}
        int cullMode;
        int fillMode;
        bool multiSample;
        bool scissor;
};

struct TextureGLES: public Texture
{
	TextureGLES():Texture()
	{
		glTexID=0;
		glDepthID=0;
		glTarget=0;
		glTargetW=0;
		components=0;
		wrap=0;
		glFormat=0;
		flags=0;
		lod=0.0f;
		mipMapped=false;
		SamplerStateID samplerState=-1;
		TU=0;
	}
	virtual ~TextureGLES(){}
	BYTE* Lock(int m, int& pitch);
	void Unlock(int m);


	//???union {
	GLuint glTexID;
	GLuint glDepthID;
	//???};
		//union {
	GLuint glTarget;
	GLuint glTargetW;
		//};
	GLuint components;
	GLuint wrap;
	GLuint glFormat;
	uint flags;
	float lod;
	bool mipMapped;
	SamplerStateID samplerState;
#ifdef HAS_CGGL
	CGparameter param;
#endif
	int TU;
};
struct VertexBufferGLES: public VertexBuffer
{
	VertexBufferGLES():VertexBuffer(){}
	virtual ~VertexBufferGLES()
	{
		vboVB=0;
		size=0;
        	//data.resize(0);
		pVertices=0;
	}
	BYTE* Lock();
	void Unlock();
	GLuint vboVB;
	GLsizei size;
	//std::vector<BYTE> data;
	BYTE* pVertices;
};
struct IndexBufferGLES: public IndexBuffer
{
	IndexBufferGLES():IndexBuffer()
	{
       		vboIB=0;
        	nIndices=0;
        	indexSize=0;
        	//data.resize(0);
		pIndices=0;
	}
	virtual ~IndexBufferGLES(){}
	BYTE* Lock();
	void Unlock();
        GLuint vboIB;
        uint nIndices;
        uint indexSize;
	//std::vector<BYTE> data;
	BYTE* pIndices;
};

#endif

