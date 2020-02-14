/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef _IRenderer_H_
#define _IRenderer_H_

#undef __X7__
//const unsigned int iDrawFlag= 0x0000;
const unsigned int iDrawFlag= 0xFFFF;

/*
		   	   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
*/
/*
		  		 x	y     tx    ty
	float v[16] =  {       -1.0f, -1.0f, 0.0f, 1.0f,  
				1.0f, -1.0f, 1.0f, 1.0f,   
				1.0f,  1.0f, 1.0f, 0.0f,  
			       -1.0f,  1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 2, &v[0], &v[0], 4*sizeof(float));
*/
#define TEXTURE_NONE  (-1)
#define SHADER_NONE   (-1)
#define BLENDING_NONE (-1)
#define VF_NONE   (-1)
#define VB_NONE   (-1)
#define IB_NONE   (-1)
#define SS_NONE   (-1)
#define BS_NONE   (-1)
#define DS_NONE   (-1)
#define RS_NONE   (-1)
#define FONT_NONE (-1)
#define FB_COLOR (-2)
#define FB_DEPTH (-2)
#define NO_SLICE (-1)
#define DONTCARE (-2)
// Texture flags
#define CUBEMAP       0x1
#define HALF_FLOAT    0x2
#define SRGB	      0x4
#define SAMPLE_DEPTH  0x8
#define SAMPLE_SLICES 0x10
#define RENDER_SLICES 0x20
#define USE_MIPGEN    0x40
// Shader flags
#define ASSEMBLY 0x1

// Mask constants
#define RED   0x1
#define GREEN 0x2
#define BLUE  0x4

#define PURPLE   0x8
#define LIGHTBLUE   0x10
#define YELLOW   0x20
#define ORANGE   0x40
#define WHITE   0x80

#define ALPHA 0x8
#define ALL (RED | GREEN | BLUE | ALPHA)

#define NONE 0
// reset() flags
#define RESET_ALL    0xFFFF
#define RESET_SHADER 0x1
#define RESET_VF     0x2
#define RESET_VB     0x4
#define RESET_IB     0x8
#define RESET_DS     0x10
#define RESET_BS     0x20
#define RESET_RS     0x40
#define RESET_SS     0x80
#define RESET_TEX    0x100
#define RESET_AS     0x200

#define CLAMP_S    0x1                          /**< Clamp S */
#define CLAMP_T    0x2                          /**< Clamp T */
#define CLAMP_R    0x4                          /**< Clamp R */
// ??? define CLAMP      (CLAMP_S | CLAMP_T | CLAMP_R)/**< Clamp */
//define CUBEMAP    0x8                          /**< Flags that the resource is a cube map texture. */
#define RECTANGLE  0x10                         /**< Treat NPOT texture as a texture rectangle. */
#define NORMALMAP  0x20                         /**< Convert the provided heightmap to a normal map. */

enum ColorDepth {
	eCOLOR,
	eDEPTH
};
enum Blend {
	eNONE,
	eONE,
	eBM_REVERSE_SUBTRACT,
	eBACK,
	eCONSTANT_COLOR,
	eZERO,
	eCULL_CW,
	eCULL_CCW,
	eCULL_NONE,
	eDEPTH_NONE,
	eLESS,
	eLEQUAL,
	eSRC_ALPHA, 
	eONE_MINUS_SRC_ALPHA,
};	
enum ShaderType
{
	ePixelShader,	//	GL_FRAGMENT_SHADER
	eVertexShader,	//	GL_VERTEX_SHADER
    	eGeometryShader,//	GL_GEOMETRY_SHADER
	eHullShader,	//	GL_TESS_CONTROL_SHADER
	eDomainShader,	//	GL_TESS_EVALUATION_SHADER
	eComputeShader	//	GL_COMPUTE_SHADER
};

enum MULTISAMPLE_TYPE
{
	MULTISAMPLE_NONE,
        MULTISAMPLE_2_SAMPLES,
	MULTISAMPLE_16_SAMPLES
};

/**
    Texture filter mode.
*/
enum TextureFilter {
	// Single image filters
	FILTER_POINT,
	FILTER_LINEAR,

	// Mipmapped image filters
	FILTER_BILINEAR,
	FILTER_TRILINEAR,

	// Mipmapped image filters with anisotropic
	FILTER_BILINEAR_ANISO,
	FILTER_TRILINEAR_ANISO,
};

/** Texture filter utility functions. @{ */
inline bool hasLinear(const TextureFilter filter){ return (filter != FILTER_POINT); }
inline bool hasMipMaps(const TextureFilter filter){ return (filter >= FILTER_BILINEAR); }
inline bool hasAnisotropic(const TextureFilter filter){ return (filter >= FILTER_BILINEAR_ANISO); }

#if 0//defined(_MSC_VER) && !defined(IRendererINTERNAL_H)
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\kernel32.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\gdi32.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\user32.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\comctl32.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\advapi32.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\winmm.lib")
#pragma comment(lib, "C:\\Program Files\\Microsoft Platform SDK for Windows Server 2003 R2\\Lib\\shell32.lib")
#if 1//defined(__32_BIT__)
#pragma comment(lib,	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3d9.lib")
#pragma comment(lib,	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx9.lib")
#elif 0//defined(__64_BIT__)
#pragma comment(lib,	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\d3d9.lib")
#pragma comment(lib,	"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\d3dx9.lib")
#endif
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cg.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cgGL.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cgD3D8.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cgD3D9.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cgD3D10.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\Cg\\lib\\cgD3D11.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\OpenGL\\lib\\OPENGL32.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\OpenGL\\lib\\glut32.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\OpenGL\\lib\\GLU32.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\lib\\libGLESv2.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\ARM\\Mali Developer Tools\\OpenGL ES 2.0 Emulator v1.2.0\\lib\\libEGL.lib")
#pragma comment(lib, "C:\\IndieLibX\\Dependencies\\SDL-1.2.14\\lib\\SDL.lib")
//pragma comment(lib, "C:\\IndieLibX\\lib\\segvcatch.lib")
//pragma comment(lib, "C:\\IndieLibX\\lib\\whirlgif_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\png_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\zlib_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\jpeg_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\IL_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\ILU_x.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\STX.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\STXCPP.lib")
//pragma comment(lib, "C:\\IndieLibX\\lib\\hlsl2glsl.lib")
#pragma comment(lib, "C:\\IndieLibX\\lib\\Framework3.lib")
#endif

/*
GLSL:			HLSL:				CG:
matrix?			float4x4			float4x4
mat4			float4x4			float4x4
vec4			float4				float4
vec3			float3				float3
vec2			float2				float2
texture2D		tex2D				tex2D
attribute		IN.				IN.
varying			OUT./IN.			OUT./IN.
#define WSIGN +		#define WSIGN +			#define WSIGN -
#define ROW_MAJOR	#define ROW_MAJOR row_major	#define ROW_MAJOR row_major
#define MVPSEMANTIC	#define MVPSEMANTIC		#define MVPSEMANTIC : state.matrix.mvp
*/

#if 1//ndef SWIG

// This is from Win32's <windef.h>
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
	#define RENDERERAPIENTRY __cdecl
	#define RENDERER_PACKSTRUCT
//#elif defined(linux) || defined(MACOSX) || defined(__CYGWIN__) //fix bug 840364
#elif defined( __GNUC__ )
  // this should work for any of the above commented platforms
  // plus any platform using GCC
	#ifdef __MINGW32__
		#define RENDERERAPIENTRY __cdecl
	#else
		#define RENDERERAPIENTRY
	#endif
	#define RENDERER_PACKSTRUCT __attribute__ ((packed))
#else
	#define RENDERERAPIENTRY
	#define RENDERER_PACKSTRUCT
#endif

// This is from Win32's <wingdi.h> and <winnt.h>
#if defined(__LCC__)
	#define RENDERERAPI __stdcall
#elif defined(_MSC_VER) //changed 20031221 to fix bug 840421
	#ifdef RENDERER_BUILD_STATIC_LIB
		#define RENDERERAPI
	#else
		#ifdef BUILD_SHARED
		#ifdef _RENDERER_BUILD_LIBRARY
			#define RENDERERAPI __declspec(dllexport)
		#else
			#define RENDERERAPI __declspec(dllimport)
		#endif
		#else
			#define RENDERERAPI
		#endif
	#endif
#elif __APPLE__
	#define RENDERERAPI
	//extern
#else
	#define RENDERERAPI
#endif

#ifdef SWIG
#define RENDERERAPIENTRY
#define RENDERERAPI
#endif

#include <STX/STX.h>
#include <Framework3/CrackFVF.h>
#include <STX/d3dx9mathFromWine.h>
#include <Framework3/Colors.h>
#include <STX/Matrix.h>
#include <STX/STXinterfaces.h>
#if 0
typedef enum _tShader
{
	eShaderNone,
	eHLSL_Shader,
	eGLSL_Shader,
	eGLES_Shader_android,
	eGLES_Shader_Ubuntu,
		eSimpleShader,
		eSimpleShader2,
		eAmbientLighting,
		eDiffuseLighting,
		ePhongLighting1,
		ePhongLighting2,
		ePositionTexture,
		ePositionNormal,
		ePositionNormalTexture,
		ePositionNormalNormal,
		eCelShader,
		eDirectionalLighting,
		eSpotLighting,
		eMultiLighting,
		eSnowShader
} tShader;
inline const char* GetShaderName(tShader aShader_)
{
	switch(aShader_)
	{
		case ePositionTexture:
			return "PositionTextureShader";
		case ePositionNormal:
			return "PositionNormalShader";
		case eSimpleShader:
			return "SimpleShader";
		case eSimpleShader2:
			return "SimpleShader2";
		case eAmbientLighting:
			return "AmbientLighting";
		case eDiffuseLighting:
			return "DiffuseLighting";
		case ePhongLighting1:
			return "PhongLighting1";
		case ePhongLighting2:
			return "PhongLighting2";
		case eCelShader:
			return "CelShader";
		case eSnowShader:
			return "SnowShader";
		case eDirectionalLighting:
			return "DirectionalLighting";
		case eSpotLighting:
			return "SpotLighting";
		case eMultiLighting:
			return "MultiLighting";
		default:
			return "SimpleShader";
	}
	return "SimpleShader";
}
#endif
struct Framework3ShaderFactory
{
	static std::string GetShader(const char* aShaderName);
};

typedef int TextureID;
typedef int SurfaceID;
typedef int ShaderID;
typedef int VertexBufferID;
typedef int IndexBufferID;
typedef int exBufferID;
typedef int VertexFormatID;
typedef int SamplerStateID;
typedef int BlendStateID;
typedef int AlphaStateID;
typedef int DepthStateID;
typedef int RasterizerStateID;
typedef int FontID;

struct RENDERERAPI Texture;
struct RENDERERAPI Shader;
struct RENDERERAPI VertexBuffer;
struct RENDERERAPI IndexBuffer;
struct RENDERERAPI VertexFormat;
struct RENDERERAPI SamplerState;
struct RENDERERAPI BlendState;
struct RENDERERAPI AlphaState;
struct RENDERERAPI DepthState;
struct RENDERERAPI RasterizerState;
enum RENDERERAPI STX_INDEXFORMAT {
	CONSTANT_INDEX2=2,
	CONSTANT_INDEX4=4
};
enum RENDERERAPI ConstantType {
	CONSTANT_FLOAT,
	CONSTANT_VEC2,
	CONSTANT_VEC3,
	CONSTANT_VEC4,
	CONSTANT_INT,
	CONSTANT_IVEC2,
	CONSTANT_IVEC3,
	CONSTANT_IVEC4,
	CONSTANT_BOOL,
	CONSTANT_BVEC2,
	CONSTANT_BVEC3,
	CONSTANT_BVEC4,
	CONSTANT_MAT2,
	CONSTANT_MAT3,
	CONSTANT_MAT4,
    CONSTANT_NONE,
	CONSTANT_TYPE_COUNT
};
extern int constantTypeSizes[CONSTANT_TYPE_COUNT];
enum RENDERERAPI Filter {
	NEAREST,
	LINEAR,
	BILINEAR,
	TRILINEAR,
	BILINEAR_ANISO,
	TRILINEAR_ANISO,
};
enum RENDERERAPI AddressMode {
	WRAP,
	CLAMP,
};
inline bool hasMipmaps(const Filter filter){ return (filter >= BILINEAR); }
inline bool hasAniso(const Filter filter){ return (filter >= BILINEAR_ANISO); }
struct RENDERERAPI Character {
	float x0, y0;
	float x1, y1;
	float ratio;
};
struct RENDERERAPI TexFont {
	Character chars[256];
	TextureID texture;
};
struct RENDERERAPI TexVertex {
	TexVertex(){
		position = D3DXFROMWINEVECTOR2(0.0f, 0.0f);
		texCoord = D3DXFROMWINEVECTOR2(0.0f, 0.0f);
	}
	TexVertex(const D3DXFROMWINEVECTOR2 p, const D3DXFROMWINEVECTOR2 t){
		position = p;
		texCoord = t;
	}
	D3DXFROMWINEVECTOR2 position;
	D3DXFROMWINEVECTOR2 texCoord;
};
#define MAKEQUAD(x0, y0, x1, y1, o)\
	D3DXFROMWINEVECTOR2(x0 + o, y0 + o),\
	D3DXFROMWINEVECTOR2(x0 + o, y1 - o),\
	D3DXFROMWINEVECTOR2(x1 - o, y0 + o),\
	D3DXFROMWINEVECTOR2(x1 - o, y1 - o),
#define MAKETEXQUAD(x0, y0, x1, y1, o)\
	TexVertex(D3DXFROMWINEVECTOR2(x0 + o, y0 + o), D3DXFROMWINEVECTOR2(0, 0)),\
	TexVertex(D3DXFROMWINEVECTOR2(x0 + o, y1 - o), D3DXFROMWINEVECTOR2(0, 1)),\
	TexVertex(D3DXFROMWINEVECTOR2(x1 - o, y0 + o), D3DXFROMWINEVECTOR2(1, 0)),\
	TexVertex(D3DXFROMWINEVECTOR2(x1 - o, y1 - o), D3DXFROMWINEVECTOR2(1, 1)),
#define MAKETEXQUAD2(x, y, s, tx, ty, st)\
	TexVertex(D3DXFROMWINEVECTOR2(x    , y    ), D3DXFROMWINEVECTOR2(tx, ty)),\
	TexVertex(D3DXFROMWINEVECTOR2(x    , y + s), D3DXFROMWINEVECTOR2(tx, ty+st)),\
	TexVertex(D3DXFROMWINEVECTOR2(x + s, y    ), D3DXFROMWINEVECTOR2(tx+st, ty)),\
	TexVertex(D3DXFROMWINEVECTOR2(x + s, y + s), D3DXFROMWINEVECTOR2(tx+st, ty+st)),
#define MAKERECT(x0, y0, x1, y1, lw)\
	D3DXFROMWINEVECTOR2(x0, y0),\
	D3DXFROMWINEVECTOR2(x0 + lw, y0 + lw),\
	D3DXFROMWINEVECTOR2(x1, y0),\
	D3DXFROMWINEVECTOR2(x1 - lw, y0 + lw),\
	D3DXFROMWINEVECTOR2(x1, y1),\
	D3DXFROMWINEVECTOR2(x1 - lw, y1 - lw),\
	D3DXFROMWINEVECTOR2(x0, y1),\
	D3DXFROMWINEVECTOR2(x0 + lw, y1 - lw),\
	D3DXFROMWINEVECTOR2(x0, y0),\
	D3DXFROMWINEVECTOR2(x0 + lw, y0 + lw),

#if 1//ndef SWIG
enum RENDERERAPI BufferAccess {
	STATIC,
	DEFAULT,
	DYNAMIC,
	STAGING
};
#else
enum RENDERERAPI BufferAccess {
	STATIC_FOR_SWIG,
	DEFAULT,
	DYNAMIC,
	STAGING
};
#endif
enum RENDERERAPI AttributeType {
	TYPE_GENERIC  = 0,
	TYPE_VERTEX   = 1,
	TYPE_TEXCOORD = 2,
	TYPE_NORMAL   = 3,
	//TYPE_COLOR   = 3,
	TYPE_TANGENT  = 4,
	TYPE_BINORMAL = 5,
};
enum RENDERERAPI AttributeFormat {
	FORMAT_FLOAT = 0,
	FORMAT_HALF  = 1,
	FORMAT_UBYTE = 2,
	FORMAT_USHORT = 3,
};
struct RENDERERAPI FormatDesc {
	int stream;
	AttributeType type;
	AttributeFormat format;
	int size;
};

#define VertexAttribute int // ???
#define TRACK_WINDOW_SIZE 0x80

#define MAX_MRTS 8
#define MAX_VERTEXSTREAM 8
#define MAX_TEXTUREUNIT  16
#define MAX_SAMPLERSTATE 16

enum RENDERERAPI Primitives {
	PRIM_TRIANGLES	    = 0,
	PRIM_TRIANGLE_FAN   = 1,
	PRIM_TRIANGLE_STRIP = 2,
	PRIM_QUADS	    = 3,
	PRIM_LINES	    = 4,
	PRIM_LINE_STRIP     = 5,
	PRIM_LINE_LOOP	    = 6,
	PRIM_POINTS	    = 7,
};
struct IReferenceCount
{
	IReferenceCount(){}
	virtual ~IReferenceCount(){}
	//virtual QueryInterface)(THIS_ REFIID,PVOID*) =0;
	virtual ULONG AddRef() =0;
	virtual ULONG Release() =0;
};
struct RENDERERAPI Texture
{


public:

        SamplerStateID ss;
	int width, height,depth,mipMapLevels;
	int dim;
	FORMAT format;
	Texture(){ss=-1;}
	virtual ~Texture(){}
	virtual BYTE* Lock(int m, int& pitch)=0;
	virtual void Unlock(int m)=0;
};
struct RENDERERAPI VertexFormat
{
public:
	int size;
	VertexFormat()
	{
		size=0;
	}
	virtual ~VertexFormat(){}
};
struct RENDERERAPI VertexBuffer
{


public:


	VertexBuffer(){}
	virtual ~VertexBuffer(){}
	virtual BYTE* Lock()=0;
	virtual void Unlock()=0;
};
struct RENDERERAPI IndexBuffer
{


public:


	IndexBuffer(){}
	virtual ~IndexBuffer(){}
	virtual BYTE* Lock()=0;
	virtual void Unlock()=0;
};
typedef std::string _StringBase;
typedef struct RENDERERAPI _SURFACE_DESC
{
    //FORMAT           Format;
    //D3DRESOURCETYPE     Type;
    //__DWORD__               Usage;
    //D3DPOOL             Pool;

  //  D3DMULTISAMPLE_TYPE MultiSampleType;
    //__DWORD__               MultiSampleQuality;
    UINT                Width;
    UINT                Height;
} SURFACE_DESC;
typedef struct RENDERERAPI _VERTEXBUFFER_DESC
{
    //D3DFORMAT           Format;
    //D3DRESOURCETYPE     Type;
    //__DWORD__               Usage;
    //D3DPOOL             Pool;
    UINT                Size;

    //__DWORD__               FVF;

} VERTEXBUFFER_DESC;

/* Index Buffer Description */
typedef struct RENDERERAPI _INDEXBUFFER_DESC
{
    //D3DFORMAT
	int Format;
    //D3DRESOURCETYPE     Type;
    //__DWORD__               Usage;
    //D3DPOOL             Pool;
    UINT                indexSize;
    UINT                nIndices;
} INDEXBUFFER_DESC;
class RENDERERAPI CResourceReuseCache;
typedef enum RENDERERAPI _TS
{
	    TS_PROJECTION,
		TS_VIEW,
		TS_WORLD,
} TS;

typedef int LoaderID;
typedef int ProcessorID;
class RENDERERAPI CPackedFile;
class RENDERERAPI CAsyncLoader;
class RENDERERAPI CResourceReuseCache;
struct RENDERERAPI Shader
{
	Shader(){}
	virtual ~Shader();
	std::string fileName;
#if 0
	std::string vsTxt;
	std::string fsTxt;
	std::string gsTxt;
	std::string hsTxt;
	std::string dsTxt;
	std::string csTxt;
	std::string vsMain;
	std::string fsMain;
	std::string gsMain;
	std::string hsMain;
	std::string dsMain;
	std::string csMain;
#endif
	ShaderID id;
};
struct RENDERERAPI ShaderParameter
{
	//Shader* shader;
	ShaderParameter(){}//shader=0;}
	//ShaderParameter(){}//Shader* ashader){shader=ashader;}
	virtual ~ShaderParameter(){}
};
//include <Framework3/RendererHLSLCg.h>

#ifndef SWIG

#endif
#if 0
#define __BOOST__ 1

#undef __BOOST__
using namespace boost;
#endif
#endif
struct stx_Effect;
struct RENDERERAPI IRenderer
{
#if 0
protected:
	unsigned int m_bFlag;
public:
	void setResetFlag(const unsigned int flags=RESET_ALL){m_bFlag=flags;}
	unsigned int getResetFlag(){return m_bFlag;}
	void setTextureStateFlag(const unsigned int flags=RESET_TEX)
	{
		if (flags & RESET_TEX)
		{
			m_bFlag|=RESET_TEX;
		}
	}
	void setShaderFlag(const unsigned int flags=RESET_SHADER)
	{
		if (flags & RESET_SHADER)
		{
			m_bFlag|=RESET_SHADER;
		}
	}
	void setVertexFormatFlag(const unsigned int flags=RESET_VF)
	{
		if (flags & RESET_VF)
		{
			m_bFlag|=RESET_VF;
		}
	}
	void setVertexBufferFlag(const unsigned int flags=RESET_VB)
	{
		if (flags & RESET_VB)
		{
			m_bFlag|=RESET_VB;
		}
	}
	void setIndexBufferFlag(const unsigned int flags=RESET_IB)
	{
		if (flags & RESET_IB)
		{
			m_bFlag|=RESET_IB;
		}
	}
	void setBlendStateFlag(const unsigned int flags=RESET_BS)
	{
		if (flags & RESET_BS)
		{
			m_bFlag|=RESET_BS;
		}
	}
	void setAlphaStateFlag(const unsigned int flags=RESET_AS)
	{
		if (flags & RESET_AS)
		{
			m_bFlag|=RESET_AS;
		}
	}
	void setDepthStateFlag(const unsigned int flags=RESET_DS)
	{
		if (flags & RESET_DS)
		{
			m_bFlag|=RESET_DS;
		}
	}
	void setRasterizerStateFlag(const unsigned int flags=RESET_RS)
	{
		if (flags & RESET_RS)
		{
			m_bFlag|=RESET_RS;
		}
	}
#endif
#if 1
	virtual ShaderID GetCurrentShader()=0;
	virtual ShaderID GetSelectedShader()=0;

	virtual VertexFormatID GetCurrentVertexFormat()=0;
	virtual VertexFormatID GetSelectedVertexFormat()=0;
	virtual IndexBufferID GetCurrentIndexBuffer()=0;
	virtual IndexBufferID GetSelectedIndexBuffer()=0;

	virtual DepthStateID GetCurrentDepthState()=0;
	virtual DepthStateID GetSelectedDepthState()=0;
	virtual BlendStateID GetCurrentBlendState()=0;
	virtual BlendStateID GetSelectedBlendState()=0;
	virtual AlphaStateID GetCurrentAlphaState()=0;
	virtual AlphaStateID GetSelectedAlphaState()=0;
	virtual RasterizerStateID GetCurrentRasterizerState()=0;
	virtual RasterizerStateID GetSelectedRasterizerState()=0;

	inline bool setFPSLimit(float fpsLimit)
	{
		static float timeAtLastUpdate=1.0f*timeGetTime();
    		static float timeDelay = 1000.0f / fpsLimit;
    		if( timeGetTime() - timeAtLastUpdate > timeDelay)
		{  
			timeAtLastUpdate=1.0f*timeGetTime();
        		return true;
		}
    		else
        		return false;
	}

	inline IndexBufferID CreateIndexBuffer(unsigned int Length,__DWORD__ Usage,unsigned int Format,unsigned int Pool,IndexBufferID* ppIndexBuffer)
	{
		/* D3DFORMAT
    D3DFMT_INDEX16              =101,
    D3DFMT_INDEX32              =102,
		*/
		assert((Format==sizeof(__WORD__))||(Format==sizeof(__DWORD__)));
		const unsigned int nIndices=Length/Format;
		const unsigned int indexSize=Format;
		*ppIndexBuffer=addIndexBuffer(nIndices, indexSize, STATIC, 0, -1);
		return *ppIndexBuffer;
	}
	inline int CreateVertexBuffer(unsigned int Length,__DWORD__ Usage,__DWORD__ FVF,unsigned int Pool,VertexBufferID* ppVertexBuffer)
	{
		*ppVertexBuffer=addVertexBuffer(Length, STATIC, 0, -1);
		return *ppVertexBuffer;
	}
	inline D3DFROMWINEVIEWPORT9 GetViewport(D3DFROMWINEVIEWPORT9* pViewport){return GetViewport();}
	inline VertexFormatID SetFVF(__DWORD__ FVF)
	{
		#if 0
		__DWORD__ tcsc=GetTexCoordSetsCount(FVF);
		__DWORD__ offxyz=GetFVFComponentOffset(FVF, D3DFVF_XYZ, 0);
		__DWORD__ offn=GetFVFComponentOffset(FVF, D3DFVF_NORMAL, 0);
		__DWORD__ ss=0;
		if(offxyz!=0xFFFFFFFF) ss++;
		if(offn!=0xFFFFFFFF) ss++;
		FormatDesc decl[ss+tcsc];
		if(offxyz!=0xFFFFFFFF)
		{
			decl[0].stream=0;
			decl[1].type=TYPE_VERTEX;
			decl[2].format=FORMAT_FLOAT;
			decl[3].size=3;
		}
		unsigned int i=1;
		if(offn!=0xFFFFFFFF)
		{
			decl[4*i+0].stream=0;
			decl[4*i+1].type=TYPE_NORMAL;
			decl[4*i+2].format=FORMAT_FLOAT;
			decl[4*i+3].size=3;
		}
		if(offn!=0xFFFFFFFF)
		if(offn!=0xFFFFFFFF)
		for(i=2;i<ss+tcsc;i++)
		{
			__DWORD__ s=GetTexCoordSetSize(FVF,i-2);
			decl[4*i+0].stream=0;
			decl[4*i+1].type=TYPE_TEXCOORD;
			decl[4*i+2].format=FORMAT_FLOAT;
			decl[4*i+3].size=s;
		}
		return IRenderer::GetRendererInstance()->addVertexFormat(decl, elementsOf(decl), GetCurrentShader());		
		#endif
		return -1;
	}
	inline int SetIndices(IndexBufferID* pIndexData)
	{
		setIndexBuffer(*pIndexData);
		return 0;
	}
	inline int SetMaterial(CONST D3DFROMWINEMATERIAL9* pMaterial){return 0;}//???
	inline int SetStreamSource(unsigned int StreamNumber,VertexBufferID* pStreamData,unsigned int OffsetInBytes,unsigned int Stride)
	{
		setVertexBuffer(StreamNumber, *pStreamData, OffsetInBytes);
		return 0;
	}
	inline int SetTexture(__DWORD__ Stage,TextureID* pTexture)
	{
		//setTexture(GetSamplerName(pTexture)/* ??? */, *pTexture);
		return 0;
	}
#if 0

	inline int DrawIndexedPrimitive(Primitives PrimitiveType,int BaseVertexIndex,unsigned int MinVertexIndex,unsigned int NumVertices,unsigned int startIndex,unsigned int primCount)
	{
		return DrawIndexedPrimitive(PrimitiveType,

			BaseVertexIndex,
			MinVertexIndex,
			NumVertices,
			startIndex,

			primCount);//, const unsigned int flags=RESET_ALL);
	}
	inline int DrawPrimitive(Primitives PrimitiveType,unsigned int StartVertex,unsigned int PrimitiveCount)

	{
		return DrawPrimitive(PrimitiveType,
			StartVertex,
			PrimitiveCount);//, const unsigned int flags=RESET_ALL);

	}
	inline int DrawPrimitiveUP(Primitives PrimitiveType,unsigned int PrimitiveCount,CONST void* pVertexStreamZeroData,unsigned int VertexStreamZeroStride)

	{
		return DrawPrimitiveUP(PrimitiveType,
			PrimitiveCount,
			pVertexStreamZeroData,

			pVertexStreamZeroData,
			VertexStreamZeroStride);//, const unsigned int flags=RESET_ALL);
	}
	inline int DrawIndexedPrimitiveUP(Primitives PrimitiveType,unsigned int MinVertexIndex,unsigned int NumVertices,unsigned int PrimitiveCount,CONST void* pIndexData,STX_INDEXFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,unsigned int VertexStreamZeroStride)
	{
		return DrawIndexedPrimitiveUP(PrimitiveType,
			 MinVertexIndex,
			NumVertices,
			PrimitiveCount,
			pIndexData,
			pIndexData,
		STX_INDEXFORMAT IndexDataFormat,
			pVertexStreamZeroData,
			pVertexStreamZeroData,
			VertexStreamZeroStride);//, const unsigned int flags=RESET_ALL);
	}
	inline int SetRenderState(D3DFROMWINERENDERSTATETYPE State,__DWORD__ Value){return 0;}
	inline int SetSamplerState(__DWORD__ Sampler,D3DFROMWINESAMPLERSTATETYPE Type,__DWORD__ Value){return 0;}
	inline int SetTextureStageState(__DWORD__ Stage,D3DFROMWINETEXTURESTAGESTATETYPE Type,__DWORD__ Value){return 0;}
	inline int SetTransform(TS State,CONST D3DFROMWINEMATRIX* pMatrix){return 0;}
#endif
#endif
	TextureID addImageLibTexture(const char *fileName, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0, ubyte R=0, ubyte G=0, ubyte B=0, ubyte A=255);
	virtual bool IsOK()=0;
	virtual BlendStateID getBlendState(const TextureID texture)=0;
#if 0
	void Set3dMeshSequence(int, FW3_3dMesh*, int&){}
	void SetTransform3d(int, int, int, int, int, int, int, int, int, FW3_Matrix*){}
	void SetRainbow3d(int, int, FW3, int, int, int, int, int, int, int, int, FW3, FW3){}
	void Blit3dMesh(int, FW3_3dMesh*){}

	void SetViewPort3d(int,int,int,int){}
	void SetCamera3d(FW3_Camera3d*){}
	void SetCamera2d(FW3_Camera2d*){}
#endif

	void ShowFpsInWindowTitle(){}
    float GetFrameTime(){return 0.0f;}
	void ToggleFullScreen(){}
	void ClearViewPort(int,int,int){}
	void SetViewPort2d(int,int,int,int){}
public:
#if 0
	FW3_Entity2dManager* Entity2dManager;
	FW3_Entity3dManager* Entity3dManager;
	FW3_AnimationManager* AnimationManager;
	FW3_SurfaceManager* SurfaceManager;
	FW3_FontManager* FontManager;
	FW3_LightManager* LightManager;
	FW3_MeshManager* MeshManager;
#endif
	virtual int addAnimation(char* afile)=0;
	virtual int addMesh(char* afile)=0;
	virtual int addSound(char* afile, char* aname)=0;

	virtual void deleteAnimation(char* afile)=0;
	virtual void deleteMesh(char* afile)=0;
	virtual void deleteSound(char* afile, char* aname)=0;

	virtual void renderAnimation(int id, int x, int y, int sx, int sy)=0;
	virtual void renderMesh(int id, D3DXFROMWINEMATRIX mvp)=0;
	virtual void playSound(int id)=0;
	virtual void stopSound(int id)=0;
protected:
	static float m_fieldOfView;
    	static float m_aspectRatio;
    	static float m_zNear;
    	static float m_zFar;

    	static std::map<std::string, TextureID> m_Texture_cache;
public:
	static TextureID find_from_Texture_cache(const char* fileName);
	static void insert_to_Texture_cache(const char* fileName, TextureID id);

	virtual void moveTo(int x, int y)=0;
	virtual void lineTo(int x, int y)=0;
	virtual void resetVrtl(const unsigned int flags)=0;

inline void reset(const unsigned int flags=0){resetVrtl(flags);}
	virtual void resetToDefaults()=0;

	virtual void DumpRenderer()=0;
	virtual void SetInput(IInput* a_pInput)=0;
	D3DXFROMWINEVECTOR4 translation;
	D3DXFROMWINEVECTOR4 GetTranslation(){return translation;}
	void SetTranslation(float x,float y,float z)
	{
		translation.x=0;//x;
		translation.y=0;//y;
		translation.z=0;//z;
		translation.w=1;
	}
	virtual void PushMatrix()=0;
	virtual void PopMatrix()=0;
	virtual void Translatef(float ax, float ay, float az)=0;
protected:
	static bool SDL;
	static int fps;
public:
	static bool GetSDL(){return SDL;}
	virtual int getFPS()=0;
	virtual FontID GetdefaultFont()=0;
   virtual void VertexPointer(const int w, /*GL_INT,*/ const int off, const void* p)=0;
   virtual void NormalPointer(const int off, const void* p)=0;
   virtual void TexCoordPointer(const int w, /*GL_FLOAT,*/ const int off, const void* p) =0;

	virtual void Begin(Primitives prim)=0;
	virtual void EndVrtl(const TextureID id, const D3DXFROMWINEMATRIX* pMVP, const char* aXFile=0, const char* aTextureFile=0)=0;

inline void End(const TextureID id=TEXTURE_NONE, const D3DXFROMWINEMATRIX* pMVP=0, const char* aXFile=0, const char* aTextureFile=0){EndVrtl(id, pMVP, aXFile, aTextureFile);}
  virtual void Vertex2i(int x,int y)=0;
  virtual void Vertex3i(int x,int y, int z)=0;
  virtual void Vertex4i(int x,int y, int z ,int w)=0;
  virtual void Vertex2f(float x,float  y)=0;
  virtual void Vertex3f(float x,float  y, float  z)=0;
  virtual void Vertex4f(float x,float  y, float  z ,float  w)=0;
  virtual void Vertex2fv(const float* v)=0;
  virtual void Vertex3fv(const float* v)=0;
  virtual void Vertex4fv(const float* v)=0;
  virtual void Color3f(float x,float  y, float  z)=0;
  virtual void Color4f(float x,float  y, float  z ,float  w)=0;
  virtual void Color3fv(const float* v)=0;
  virtual void Color4fv(const float* v)=0;
  virtual void TexCoord2f(float x,float  y)=0;
	unsigned int GetSHADING_LANGUAGE_VERSION_MAJOR(){return SHADING_LANGUAGE_VERSION_MAJOR;}
	unsigned int GetSHADING_LANGUAGE_VERSION_MINOR(){return SHADING_LANGUAGE_VERSION_MINOR;}
	//static int WinVer();
protected:
	static std::string Str;
	static std::string m_title;

	static bool m_bLuaScript;
	static bool m_bDebug;

	static unsigned int width;
	static unsigned int height;
	static unsigned int bpp;
	unsigned int SHADING_LANGUAGE_VERSION_MAJOR;
	unsigned int SHADING_LANGUAGE_VERSION_MINOR;
#if 0
	std::string mVersion;
	std::string mVendor;
	std::string mRenderer;
	bool mAntialiasing;
	__DWORD__ mSHADING_LANGUAGE_VERSION_MAJOR;
	__DWORD__ mSHADING_LANGUAGE_VERSION_MINOR;

public:

	void SetVersion(std::string aVersion){mVersion=aVersion;}
	void SetVendor(std::string aVendor){mVendor=aVendor;}
	void SetRenderer(std::string aRenderer){mRenderer=aRenderer;}
	void SetAntialiasing(bool aAntialiasing){mAntialiasing=aAntialiasing;}
	void SetMaxTextureSize(int aMaxTextureSize){mMaxTextureSize=aMaxTextureSize;}
	void SetSHADING_LANGUAGE_VERSION_MAJOR(__DWORD__ aSHADING_LANGUAGE_VERSION_MAJOR){mSHADING_LANGUAGE_VERSION_MAJOR=aSHADING_LANGUAGE_VERSION_MAJOR;}
	void SetSHADING_LANGUAGE_VERSION_MINOR(__DWORD__ aSHADING_LANGUAGE_VERSION_MINOR){mSHADING_LANGUAGE_VERSION_MINOR=aSHADING_LANGUAGE_VERSION_MINOR;}

	std::string GetVersion(){return mVersion;}
	std::string GetVendor(){return mVendor;}
	std::string GetRenderer(){return mRenderer;}
	bool GetAntialiasing(){return mAntialiasing;}
	__DWORD__ GetSHADING_LANGUAGE_VERSION_MAJOR(){return mSHADING_LANGUAGE_VERSION_MAJOR;}
	__DWORD__ GetSHADING_LANGUAGE_VERSION_MINOR(){return mSHADING_LANGUAGE_VERSION_MINOR;}
#endif
	const int mMaxTextureSize=4096;
public:
	int GetMaxTextureSize(){return mMaxTextureSize;}
	static unsigned int GetBPP(){return bpp;}
	static unsigned int GetWidth(){return width;}
	static unsigned int GetHeight(){return height;}

	static void OnPresent(
		IInput* Input=0
		);
	//virtual void OnBeginScene(){}

//
#ifndef SWIG
protected:
static IRenderer* s_pRenderer;
#endif
public:
	virtual const char* getString()=0;
static IRenderer* GetRendererInstance(const char* atitle="", tRenderer art=eGLGLSL_1_1,
		bool fullScreen=false
		,int argc=0, char *argv[]=0);
static void CloseRenderer(){
	if(s_pRenderer) delete s_pRenderer;s_pRenderer=0;
}

	void OnEndScene();
	virtual Texture* GetTexture(TextureID id)=0;
	virtual unsigned int GetTextureCount()=0;
	virtual unsigned int GetShaderCount()=0;
	virtual unsigned int GetLevelCount(TextureID id)=0;
	virtual void GetLevelDesc(TextureID id, unsigned int level,SURFACE_DESC* sDesc)=0;
	virtual void GetDesc(VertexBufferID id,VERTEXBUFFER_DESC* sDesc)=0;
	virtual void GetDesc(IndexBufferID id, INDEXBUFFER_DESC* sDesc)=0;

	virtual void SetInt(const char* n, int d)=0;
	virtual void SetFloat(const char* n, float d)=0;
	virtual void SetVector(const char* n, const D3DXFROMWINEVECTOR4 * d)=0;
	virtual void SetVectorArray(const char* n, const D3DXFROMWINEVECTOR4 * d,unsigned int nn)=0;
	virtual void SetMatrix(const char* n, const D3DXFROMWINEMATRIX * d)=0;
	virtual void SetMatrixTransposeArray(const char* n, const D3DXFROMWINEMATRIX * d,unsigned int nn)=0;
	virtual void SetTexture(const char* n, TextureID id)=0;
protected:
	static int ZERO;
	static int ONE;
	static int SRC_COLOR;
	static int ONE_MINUS_SRC_COLOR;
	static int DST_COLOR;
	static int ONE_MINUS_DST_COLOR;
	static int SRC_ALPHA;
	static int ONE_MINUS_SRC_ALPHA;
	static int DST_ALPHA;
	static int ONE_MINUS_DST_ALPHA;
	static int SRC_ALPHA_SATURATE;
	static int BM_ADD;
	static int BM_SUBTRACT;
	static int BM_REVERSE_SUBTRACT;
	static int BM_MIN;
	static int BM_MAX;
	static int NEVER;
	static int LESS;
	static int EQUAL;
	static int LEQUAL;
	static int GREATER;
	static int NOTEQUAL;
	static int GEQUAL;
	static int ALWAYS;
	static int KEEP;
	static int SET_ZERO;
	static int REPLACE;
	static int INVERT;
	static int INCR;
	static int DECR;
	static int INCR_SAT;
	static int DECR_SAT;
	static int CULL_NONE;
	static int CULL_BACK;
	static int CULL_FRONT;
	static int SOLID;
	static int WIREFRAME;
public:
    virtual int Getlinear()=0;
    virtual int GetlinearClamp()=0;
    virtual int GetblendSrcAlpha()=0;
    virtual int GetnoDepthTest()=0;
    virtual int GetlinearAniso()=0;
    virtual int GetFILTER_LINEAR_ANISO()=0;
    virtual int Getbilinear()=0;
    virtual int GetbilinearAniso()=0;
    virtual int GetFILTER_BILINEAR_ANISO()=0;
    virtual int Gettrilinear()=0;
    virtual int GettrilinearAniso()=0;
    virtual int GetFILTER_TRILINEAR_ANISO()=0;
    virtual int Getnearest()=0;
    virtual int GetnearestClamp()=0;
    virtual int GetbilinearClamp()=0;
    virtual int GettrilinearClamp()=0;
    virtual int GetenableAlpha()=0;
    virtual int GetdisableAlpha()=0;
    virtual int GetblendOneOne()=0;
    virtual int GetblendOneOneReverseSubstract()=0;
    virtual int GetnoDepthWrite()=0;
    virtual int GetcullNone()=0;
    virtual int GetcullBack()=0;
    virtual int GetcullFront()=0;
    virtual int GetfillSolid()=0;
    virtual int GetfillWireFrame()=0;


	//virtual CResourceReuseCache* Getg_pResourceReuseCache()=0;

    virtual BYTE* LockTexture(TextureID id, int m, int& pitch)=0;
    virtual void UnlockTexture(TextureID id, int m)=0;
    virtual BYTE* LockVertexBuffer(VertexBufferID id)=0;
    virtual void UnLockVertexBuffer(VertexBufferID id)=0;
    virtual BYTE* LockIndexBuffer(IndexBufferID id)=0;
    virtual void UnLockIndexBuffer(IndexBufferID id)=0;

	virtual float	GetMaxPointSpriteSize()=0;
	virtual void	BeginPointSpriteRendering()=0;
	virtual void	EndPointSpriteRendering()=0;
#if 0
	virtual void	CreateTextureFromFile_Serial(  CHAR* szFileName,
						TextureID* ppTexture,
						//, void* pContext
						CPackedFile*		    g_PackFile,
						UINT g_SkipMips=0
						)=0;
	virtual void	CreateVertexBuffer_Serial(  VertexBufferID& ppBuffer, UINT iSizeBytes,
//					    __DWORD__ Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
//					    void* pContext
						)=0;
	virtual void	CreateIndexBuffer_Serial(  IndexBufferID& ppBuffer, UINT iSizeBytes,
//					    __DWORD__ Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
//					    void* pContext
						)=0;
#endif



	virtual BYTE *fillVertexBuffer(const VertexBufferID id,  BYTE*  m_pData,  int l)=0;
	virtual BYTE *fillIndexBuffer(const IndexBufferID id,  BYTE*  m_pData,  int l)=0;
/*???

		*/
#if 1
	virtual ShaderID& GetplainShader()=0;
	virtual VertexFormatID& GetplainVF()=0;
	virtual ShaderID& GetplainShader3()=0;
	virtual VertexFormatID& GetplainVF3()=0;
	virtual ShaderID& GettexShader()=0;
	virtual VertexFormatID& GettexVF()=0;
	virtual ShaderID& GettexShader3()=0;
	virtual VertexFormatID& GettexVF3()=0;
	virtual D3DXFROMWINEVECTOR4& GetscaleBias2D()=0;
#endif
	virtual int GetTextureWidth(TextureID id)=0;
	virtual int GetTextureHeight(TextureID id)=0;
	virtual FORMAT GetTextureFormat(const TextureID id)=0;
	inline int GetMaxTextureWidth(){return 4096;} // ???
	inline int GetMaxTextureHeight(){return 4096;} // ???

	virtual VOID UpdateViewer(
                                                   D3DXFROMWINEVECTOR3 &pos, float* quat) = 0;

	virtual void viewport(int x, int y, int viewportWidth, int viewportHeight)=0;



	virtual unsigned int DrawText(unsigned int x, unsigned int y, const char *pszFormat, ...) = 0;
	virtual unsigned int DrawSmallText(unsigned int x, unsigned int y, const char *pszFormat, ...) = 0;

	virtual D3DXFROMWINEVECTOR3 GetEyePt() = 0;

	virtual D3DXFROMWINEVECTOR3 GetLookAtPt() = 0;

	virtual D3DXFROMWINEVECTOR3 GetUpPt() = 0;


	virtual float GetYaw() = 0;

	virtual float GetPitch() = 0;

	virtual float GetRoll() = 0;


	virtual D3DXFROMWINEMATRIX Getp() = 0;

	virtual void Setp(const D3DXFROMWINEMATRIX& m) = 0;

	virtual D3DXFROMWINEMATRIX Getmv() = 0;

	virtual D3DXFROMWINEMATRIX Getpmv() = 0;

	virtual D3DXFROMWINEMATRIX Getmvp() = 0;


	virtual D3DXFROMWINEMATRIX Getpmvt() = 0;

	virtual D3DXFROMWINEMATRIX Getmvpt() = 0;


	virtual D3DXFROMWINEMATRIX Getpp() = 0;
	virtual D3DXFROMWINEMATRIX Getvv() = 0;
	virtual D3DXFROMWINEMATRIX Getww() = 0;

	virtual void Setmv(const D3DXFROMWINEMATRIX& m) = 0;



	virtual void GetTransform( TS t, D3DXFROMWINEMATRIX& m) = 0;


	virtual void SetTransform( TS t, const D3DXFROMWINEMATRIX& m) = 0;


/*
	virtual void GetTransform( TS t, D3DXFROMWINEMATRIX* m) = 0;


	virtual void SetTransform( TS t, const D3DXFROMWINEMATRIX* m) = 0;
*/


	virtual D3DFROMWINEVIEWPORT9 GetViewport() = 0;

	virtual int GetViewportWidth() const {return IRenderer::width;}
	virtual int GetViewportHeight() const {return IRenderer::height;}

	virtual int GetViewportX() = 0;

	virtual int GetViewportY() = 0;

#if 1

	//virtual HRESULT  AddWorkItem( LoaderID pDataLoader,CAsyncLoader * l, ProcessorID pDataProcessor,
//		HRESULT* pHResult, void** ppDeviceObject ) = 0;

#if 0
   virtual  void SetProcessorResourceError(ProcessorID pProcessor) = 0;
    virtual void DestroyProcessor(ProcessorID pProcessor) = 0;
    virtual void DestroyLoader(ProcessorID pLoader) = 0;

	virtual BYTE* GetVertexBufferProcessorData(ProcessorID ID)=0;
	virtual BYTE* GetIndexBufferProcessorData(ProcessorID ID)=0;
	virtual BYTE* GetTextureProcessorData(ProcessorID ID, UINT i)=0;
	virtual UINT GetTextureProcessorPitch(ProcessorID ID, UINT i)=0;
#endif
#if 0
	virtual LoaderID addTextureLoader( CHAR* szFileName, CPackedFile* g_PackFile ) = 0;
    virtual ProcessorID addTextureProcessor( TextureID& ppTexture, int g_SkipMips )=0;
    virtual HRESULT LoadTexture(LoaderID pLoader) = 0;
    virtual HRESULT DecompressTexture(LoaderID	pLoader, void** pLocalData, SIZE_T* Bytes ) = 0;
    virtual HRESULT ProcessTexture( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )=0;
    virtual HRESULT LockTextureDeviceObject(ProcessorID pProcessor)=0;
    virtual HRESULT CopyToResourceTexture(ProcessorID pProcessor)=0;
    virtual HRESULT UnLockTextureDeviceObject(ProcessorID pProcessor)=0;

    virtual LoaderID addVertexBufferLoader() = 0;
    virtual ProcessorID addVertexBufferProcessor( VertexBufferID& ppBuffer, int iSizeBytes,   void* pData)=0;
    virtual HRESULT LoadVertexBuffer(LoaderID pLoader) = 0;
    virtual HRESULT DecompressVertexBuffer(LoaderID pLoader, void**pLocalData, SIZE_T* Bytes ) = 0;
    virtual HRESULT ProcessVertexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )=0;
    virtual HRESULT LockVertexBufferDeviceObject(ProcessorID pProcessor)=0;
    virtual HRESULT CopyToResourceVertexBuffer(ProcessorID pProcessor)=0;
    virtual HRESULT UnLockVertexBufferDeviceObject(ProcessorID pProcessor)=0;

    virtual LoaderID addIndexBufferLoader() = 0;
    virtual ProcessorID addIndexBufferProcessor( IndexBufferID& ppBuffer, int iSizeBytes, void* pData)=0;
    virtual HRESULT LoadIndexBuffer(LoaderID pLoader) = 0;
    virtual HRESULT DecompressIndexBuffer(LoaderID pLoader, void**pLocalData, SIZE_T*Bytes )=0;
    virtual HRESULT ProcessIndexBuffer(ProcessorID pProcessor, void*pLocalData, SIZE_T Bytes )=0;
    virtual HRESULT LockIndexBufferDeviceObject(ProcessorID pProcessor)=0;
    virtual HRESULT CopyToResourceIndexBuffer(ProcessorID pProcessor)=0;
    virtual HRESULT UnLockIndexBufferDeviceObject(ProcessorID pProcessor)=0;
#endif
#endif
	virtual UINT32 GetAvailableTextureMem()=0;
	 virtual D3DXFROMWINEMATRIX identity() = 0;


	virtual int removeTexture(const TextureID texture) = 0;
		virtual int deleteTexture(const TextureID texture) = 0;
		virtual int deleteVertexBuffer(const VertexBufferID vertexBuffer)=0;
		virtual int deleteIndexBuffer(const IndexBufferID indexBuffer)=0;
			virtual int texturePitchVrtl(const TextureID texture,  int level)
			=0;

inline int texturePitch(const TextureID texture, int level=0){return texturePitchVrtl(texture, level);}

			virtual BYTE* lockTexture(const TextureID texture, int level, int& pitch)=0;
		virtual BYTE* lockVertexBuffer(const VertexBufferID vertexBuffer)=0;
		virtual BYTE* lockIndexBuffer(const IndexBufferID indexBuffer)=0;
			virtual BYTE* unlockTextureVrtl(const TextureID texture,  int level)=0;

inline BYTE* unlockTexture(const TextureID texture, int level=0){return unlockTextureVrtl(texture, level);}
		virtual BYTE* unlockVertexBuffer(const VertexBufferID vertexBuffer)=0;
		virtual BYTE* unlockIndexBuffer(const IndexBufferID indexBuffer)=0;



virtual unsigned int drawTextureVrtl(TextureID texture,  int x,  int y,  int sx,  int sy,  bool InScene,  float tx,  float ty,  float stx,  float sty) = 0;

inline unsigned int drawTexture(TextureID texture, int x=0, int y=0, int sx=600, int sy=600, bool InScene=true, float tx=0.0f, float ty=0.0f, float stx=1.0f, float sty=1.0f){return drawTextureVrtl(texture, x, y, sx, sy, InScene, tx, ty, stx, sty);}


	virtual D3DXFROMWINEMATRIX GetProjMatrix() = 0;

		virtual D3DXFROMWINEMATRIX GetViewMatrix() = 0;


	virtual void buildLookAtMatrix(D3DXFROMWINEVECTOR3 at
		,IInput* Input=0
		)
	 = 0;

	virtual void buildLookAtMatrix(D3DXFROMWINEVECTOR3 eye,
			      D3DXFROMWINEVECTOR3 center,
			      D3DXFROMWINEVECTOR3 up//,D3DXFROMWINEMATRIX& m
			      ) = 0;



	virtual void buildLookAtMatrix(D3DXFROMWINEVECTOR4 eye,
			      D3DXFROMWINEVECTOR4 center,
			      D3DXFROMWINEVECTOR4 up//,D3DXFROMWINEMATRIX& m
			      ) = 0;





	virtual void buildLookAtMatrix(float eyex, float eyey, float eyez,
			      float centerx, float centery, float centerz,
			      float upx, float upy, float upz//,D3DXFROMWINEMATRIX& m
			      )=0;
				  virtual void buildPerspectiveMatrix(float fieldOfView,
				   float aspectRatio,
				   float zNear, float zFar//,D3DXFROMWINEMATRIX& m
				   )=0;

	virtual void updateClipPlanes(float zNear, float zFar) = 0;




#if 0
	virtual void PushpMatrix() = 0;


	virtual void PoppMatrix() = 0;


	//???static void MultmvMatrix(D3DXFROMWINEMATRIX w) = 0;


	virtual void MultpMatrix(D3DXFROMWINEMATRIX w) = 0;
#endif

	virtual bool supportsR2VB() = 0;
	virtual bool supportsHalf() = 0;
	virtual unsigned int drawFPS() = 0;
	virtual bool debugRuntime() = 0;

	virtual void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState=-1, const int vtstream=-1, const VertexBufferID vtbuffer=-1)=0;
	virtual void SetTexture(const int textureIndex, const TextureID texture, const SamplerStateID samplerState=-1, const int vtstream=-1, const VertexBufferID vtbuffer=-1){} // ???
	virtual void setTextureSlice(const char *textureName, const TextureID texture, const int slice) = 0;

		virtual void Init0()=0;
virtual unsigned int getIndicesCount(const Primitives primitives) = 0;

virtual unsigned int getPrimitiveCount(const Primitives primitives, const int count) = 0;

virtual unsigned int getPrimitiveCountR(const Primitives primitives, const int count) = 0;

virtual unsigned int getVertexCount(const Primitives primitives, const int  NumPrim) = 0;

	virtual unsigned int RenderTexture(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 texsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f))=0;


	IRenderer()
	{
		SHADING_LANGUAGE_VERSION_MAJOR=0;
		SHADING_LANGUAGE_VERSION_MINOR=0;
/*
		mVersion="";
		mVendor="";
		mRenderer="";
		mAntialiasing=false;
		mMaxTextureSize=4096;
		mSHADING_LANGUAGE_VERSION_MAJOR=3;
		mSHADING_LANGUAGE_VERSION_MINOR=3;
*/
	}
	virtual ~IRenderer();
protected:
	std::vector<std::string> m_vMain;
	std::vector<std::string> m_vText;
	std::vector<ShaderType> m_vType;
public:
#if 0
CG_API CGeffect CGENTRY cgCreateEffect(CGcontext context, const char *code, const char **args);
CG_API CGeffect CGENTRY cgCreateEffectFromFile(CGcontext context, const char *filename, const char **args);
CG_API CGeffect CGENTRY cgCopyEffect(CGeffect effect);
CG_API void CGENTRY cgDestroyEffect(CGeffect effect);
CG_API CGcontext CGENTRY cgGetEffectContext(CGeffect effect);
CG_API CGbool CGENTRY cgIsEffect(CGeffect effect);
CG_API CGeffect CGENTRY cgGetFirstEffect(CGcontext context);
CG_API CGeffect CGENTRY cgGetNextEffect(CGeffect effect);
CG_API CGprogram CGENTRY cgCreateProgramFromEffect(CGeffect effect, CGprofile profile, const char *entry, const char **args);
CG_API CGtechnique CGENTRY cgGetFirstTechnique(CGeffect effect);
CG_API CGtechnique CGENTRY cgGetNextTechnique(CGtechnique tech);
CG_API CGtechnique CGENTRY cgGetNamedTechnique(CGeffect effect, const char *name);
CG_API const char * CGENTRY cgGetTechniqueName(CGtechnique tech);
CG_API CGbool CGENTRY cgIsTechnique(CGtechnique tech);
CG_API CGbool CGENTRY cgValidateTechnique(CGtechnique tech);
CG_API CGbool CGENTRY cgIsTechniqueValidated(CGtechnique tech);
CG_API CGeffect CGENTRY cgGetTechniqueEffect(CGtechnique tech);
CG_API CGpass CGENTRY cgGetFirstPass(CGtechnique tech);
CG_API CGpass CGENTRY cgGetNamedPass(CGtechnique tech, const char *name);
CG_API CGpass CGENTRY cgGetNextPass(CGpass pass);
CG_API CGbool CGENTRY cgIsPass(CGpass pass);
CG_API const char * CGENTRY cgGetPassName(CGpass pass);
CG_API CGtechnique CGENTRY cgGetPassTechnique(CGpass pass);
CG_API CGprogram CGENTRY cgGetPassProgram(CGpass pass, CGdomain domain);
#elif 0
        virtual void      HSSetShaderResources( 
 
            UINT StartSlot,
 
            UINT NumViews,
 
            ShaderResourceView *const *ppShaderResourceViews) = 0;
        
        virtual void      HSSetShader( 
 
                                          HullShader *pHullShader,
 
            ClassInstance *const *ppClassInstances,
            UINT NumClassInstances) = 0;
        
        virtual void      HSSetSamplers( 
 
            UINT StartSlot,
 
            UINT NumSamplers,
 
            SamplerState *const *ppSamplers) = 0;
        
        virtual void      HSSetConstantBuffers( 
 
            UINT StartSlot,
 
            UINT NumBuffers,
 
            Buffer *const *ppConstantBuffers) = 0;
        
        virtual void      DSSetShaderResources( 
 
            UINT StartSlot,
 
            UINT NumViews,
 
            ShaderResourceView *const *ppShaderResourceViews) = 0;
        
        virtual void      DSSetShader( 
 
                                          DomainShader *pDomainShader,
 
            ClassInstance *const *ppClassInstances,
            UINT NumClassInstances) = 0;
        
        virtual void      DSSetSamplers( 
 
            UINT StartSlot,
 
            UINT NumSamplers,
 
            SamplerState *const *ppSamplers) = 0;
        
        virtual void      DSSetConstantBuffers( 
 
            UINT StartSlot,
 
            UINT NumBuffers,
 
            Buffer *const *ppConstantBuffers) = 0;
        
        virtual void      CSSetShaderResources( 
 
            UINT StartSlot,
 
            UINT NumViews,
 
            ShaderResourceView *const *ppShaderResourceViews) = 0;
        
        virtual void      CSSetUnorderedAccessViews( 
 
            UINT StartSlot,
 
            UINT NumUAVs,
 
            UnorderedAccessView *const *ppUnorderedAccessViews,
 
            const UINT *pUAVInitialCounts) = 0;
        
        virtual void      CSSetShader( 
 
                                          ComputeShader *pComputeShader,
 
            ClassInstance *const *ppClassInstances,
            UINT NumClassInstances) = 0;
        
        virtual void      CSSetSamplers( 
 
            UINT StartSlot,
 
            UINT NumSamplers,
 
            SamplerState *const *ppSamplers) = 0;
        
        virtual void      CSSetConstantBuffers( 
 
            UINT StartSlot,
 
            UINT NumBuffers,
 
            Buffer *const *ppConstantBuffers) = 0;
#endif

    float GetAspectRatio()
    {
        return 1.0f*STX_Service::GetWindowInstance()->GetWidth()/STX_Service::GetWindowInstance()->GetHeight();
    }
	void setCullFace(int a)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
	void setDepthFunc(int a)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
	void setBlending(int ONE1=eONE, int ONE2=eONE, int a1=eBM_REVERSE_SUBTRACT)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
	void setMask(int COLOR_DEPTH)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }

    void changeBlendConstantColor(D3DXFROMWINEVECTOR4 a)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }

    void SetClipPlane(__DWORD__ Index, CONST float* pPlane)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
    void CreateRenderTarget(const int aWidth, const int aHeight, FORMAT aF)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
        #if 0
    void CreateDepthStencilSurface(const int aWidth, const int aHeight, FORMAT aF)
    {
        #if 0
        LOG_FNLN;
        LOG_PRINT("IMPLEMENT\n");
        #endif
    }
        #endif
bool loadSlicedImage(Image3& img, const char **fileNames, const int nImages, const int nArraySlices = 1, uint flags = 0){
	int maxImage = nImages? nImages : 6;
		//LOG_FNLN;
		//LOG_PRINT("maxImage=%d\n", maxImage);
		//LOG_PRINT("nArraySlices=%d\n", nArraySlices);

	Image3 *images = new Image3[maxImage * nArraySlices];

	for (int i = 0; i < maxImage * nArraySlices; i++){

		std::string fn=stx_convertpath(fileNames[i]);

		//LOG_FNLN;
		//LOG_PRINT("fileNames[%d]=%s\n", i, fileNames[i]);

		//if (!
		images[i].loadImage(fn.c_str(), flags);
		if(0){
		//LOG_FNLN;
			delete [] images;
			return false;
		}
		//LOG_FNLN;
	}
		//LOG_FNLN;

	uint nMipMaps = images[0].getMipMapCount();
	ubyte *dest = img.create(images[0].getFormat(), images[0].getWidth(), images[0].getHeight(), nImages, nMipMaps, nArraySlices);

	for (int arraySlice = 0; arraySlice < nArraySlices; arraySlice++){
		int base = arraySlice * maxImage;

		for (uint level = 0; level < nMipMaps; level++){
			int size = images[0].getMipMappedSize(level, 1);
			for (int i = 0; i < maxImage; i++){
				stx_memcpy(dest, images[base + i].getPixels(level), size);
				dest += size;
			}
		}
	}

	delete [] images;
		//LOG_FNLN;

	return true;
}

TextureID addCubemap_(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, const int nArraySlices = 1, uint flags = 0)
{
		//LOG_FNLN;
	Image3 img;
		//LOG_FNLN;
	if (loadSlicedImage(img, fileNames, 0, nArraySlices))
	{
		//LOG_FNLN;
		//if (img.getFormat() == FORMAT_RGBE8) img.unpackImage();
		if (useMipMaps && img.getMipMapCount() <= 1) img.createMipMaps();
		//LOG_FNLN;
		TextureID id=IRenderer::GetRendererInstance()->addTexture(&img,true, samplerState, flags);
		//LOG_FNLN;
		return id;
	}
	else
	{
		//LOG_FNLN;
		char str[1024];
        int n = stx_snprintf(str,1024, "Couldn't open cubemap:\n");
		//LOG_FNLN;
		for (int i = 0; i < 6 * nArraySlices; i++)
		{
            n += stx_snprintf(str + n,1024, "%s\n", fileNames[i]);
		}
		//LOG_FNLN;
		//LOG_PRINT(str);
		return TEXTURE_NONE;
	}
		//LOG_FNLN;
}

TextureID addCubemap(const char* filename="Olympus", const char* ext="jpg", bool mipmaps=false)
{
	std::string res="/";
	char *fileNames[6];
	fileNames[0]=new char[128];
	fileNames[1]=new char[128];
	fileNames[2]=new char[128];
	fileNames[3]=new char[128];
	fileNames[4]=new char[128];
	fileNames[5]=new char[128];	

	stx_snprintf(fileNames[0], 128, "%sSkyBox/%s/posx.%s", res.c_str(), filename, ext);
	stx_snprintf(fileNames[1], 128, "%sSkyBox/%s/negx.%s", res.c_str(), filename, ext);
	stx_snprintf(fileNames[2], 128, "%sSkyBox/%s/posy.%s", res.c_str(), filename, ext);
	stx_snprintf(fileNames[3], 128, "%sSkyBox/%s/negy.%s", res.c_str(), filename, ext);
	stx_snprintf(fileNames[4], 128, "%sSkyBox/%s/posz.%s", res.c_str(), filename, ext);
	stx_snprintf(fileNames[5], 128, "%sSkyBox/%s/negz.%s", res.c_str(), filename, ext);
	//LOG_FNLN;
	return IRenderer::GetRendererInstance()->addCubemap_((const char**)fileNames, mipmaps, IRenderer::GetRendererInstance()->GetbilinearClamp());
}


virtual D3DXFROMWINEMATRIX GetMVP()=0;
virtual void setViewportVrtl(int x,  int y,  int viewportWidth,  int viewportHeight,  float minZ,  float maxZ) = 0;

inline void setViewport(int x, int y, int viewportWidth, int viewportHeight, float minZ=0.0f, float maxZ=1.0f){setViewportVrtl(x, y, viewportWidth, viewportHeight, minZ, maxZ);}

inline void setViewport(D3DFROMWINEVIEWPORT9* pVP)
{
	if(!pVP)
	setViewportVrtl(pVP->X, pVP->Y, pVP->Width, pVP->Height, pVP->MinZ, pVP->MaxZ);
}

	virtual void apply() = 0;

	virtual stx_Effect* addEffectFromFile(const std::string& aFileName, const char* aDefines=0)=0;

    virtual ShaderID addHLSLShaderVrtl(
const char *vsText, const char *gsText, const char *fsText, const char *csText, const char *hsText, const char *dsText,
const char *vsMain, const char *gsMain, const char *fsMain, const char *csMain, const char *hsMain, const char *dsMain, 
                                            	const unsigned int flags)=0;

ShaderID addShaderFromFile(  const char* fileName, 
                             const char* vsMain="main", 
                             const char* fsMain="main",
                             const char *defines = 0,
                             const unsigned int flags = 0);

ShaderID addShader( const char* shaderText, 
                    const char* vsMain="main", 
                    const char* fsMain="main",
                    const char *defines = 0,
                    const unsigned int flags = 0);
inline ShaderID addHLSLShader( const char* shaderText, 
                    const char* vsMain="main", 
                    const char* fsMain="main",
                    const char *defines = 0,
                    const unsigned int flags = 0)
{
	return addShader(shaderText, vsMain, fsMain, defines, flags);
}

unsigned int DrawAtlas(std::vector<TextureID>& av)
{
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	D3DXFROMWINEMATRIX I;
	unsigned int N=0;
	static ShaderID shd=-1;
	static VertexFormatID vf=-1;
	if(shd==-1)
	{
		shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.shd", "main", "main");
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
		};
		vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	}
	
	D3DXFROMWINEMatrixIdentity(&I);
	N=sqrtf(av.size());
	if(N*N<av.size())
		N++;
	float dx=2.0f/N;
	float dy=2.0f/N;
	for(unsigned int y=0;y<N;y++)
	for(unsigned int x=0;x<N;x++)
	{
		if((y*N+x+1)>av.size())
			break;
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setTexture("Base", av[y*N+x]);
	IRenderer::GetRendererInstance()->setShaderConstant4f("color", color);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
  	IRenderer::GetRendererInstance()->setBlendState(IRenderer::GetRendererInstance()->GetblendSrcAlpha());
  	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	float px=dx*x;
	float py=dy*y;
	//		   x	 y     tx    ty
	{float v[16] =  {-1.0f+px+dx, 1.0f-py-dy, 1.0f, 1.0f,
			 -1.0f+px+dx, 1.0f-py,    1.0f, 0.0f,
			 -1.0f+px,    1.0f-py-dy, 0.0f, 1.0f,
			 -1.0f+px,    1.0f-py,    0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));}
}
return 0;
}

	virtual int getFormatSize(const AttributeFormat format) const=0;
	virtual VertexFormatID addVertexFormatVrtl(const FormatDesc *formatDesc,  const unsigned int nAttribs,  const ShaderID shader ) = 0;

inline VertexFormatID addVertexFormatV(CONST VOID *formatDesc, int nAttribs, CONST ShaderID shader){return addVertexFormatVrtl((const FormatDesc *)formatDesc, nAttribs, shader);}

inline VertexFormatID addVertexFormat(const FormatDesc *formatDesc, const unsigned int nAttribs, const ShaderID shader = SHADER_NONE){return addVertexFormatVrtl(formatDesc, nAttribs, shader);}

	virtual VertexBufferID addVertexBufferVrtl(const int size,  const BufferAccess bufferAccess,  const void *data ,  VertexBufferID old)=0;

inline VertexBufferID addVertexBuffer(const int size, const BufferAccess bufferAccess=STATIC, const void *data = 0, VertexBufferID old=-1){return addVertexBufferVrtl(size, bufferAccess, data, old);}//, const IDataProcessor* p=0) = 0;
	virtual IndexBufferID addIndexBufferVrtl(const unsigned int nIndices,  const unsigned int indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID old)=0;

inline IndexBufferID addIndexBuffer(const unsigned int nIndices, const unsigned int indexSize=2, const BufferAccess bufferAccess=STATIC, const void *data = 0, IndexBufferID old=-1){return addIndexBufferVrtl(nIndices, indexSize, bufferAccess, data, old);}//, const IDataProcessor* p=0)  = 0;
		virtual TextureID addTextureVrtl(int Width,  int Height,  bool MipLevels,  FORMAT format,  const SamplerStateID samplerState, const unsigned int flags )=0;

inline TextureID addTexture(int Width, int Height, bool MipLevels, FORMAT format, const SamplerStateID samplerState = SS_NONE, const unsigned int flags = 0){return addTextureVrtl(Width, Height, MipLevels, format, samplerState, flags);}



	virtual SamplerStateID addSamplerStateVrtl(const Filter filter,  const AddressMode s,  const AddressMode t,  const AddressMode r,  const float lod ,  const unsigned int maxAniso ,  const int compareFunc ,  const float *border_color ) = 0;

inline SamplerStateID addSamplerState(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod = 0, const unsigned int maxAniso = 16, const int compareFunc = 0, const float *border_color = 0){return addSamplerStateVrtl(filter, s, t, r, lod, maxAniso, compareFunc, border_color);}

	//void SetBlending( bool onoff, PSYSGFX::BlendFactor src, PSYSGFX::BlendFactor dst );
	//void SetAlphaTest( bool onoff, PSYSGFX::TestFunc tstfunc, float refparam );
	virtual AlphaStateID addAlphaState(
		const int func=GEQUAL,
		const float ref=0.5f
		)=0;

	virtual BlendStateID addBlendState(
		const int srcFactor,
		const int destFactor,
		const int blendMode = BM_ADD,
		const int mask = ALL,
		const bool alphaToCoverage = false)=0;

	virtual BlendStateID addBlendState(
		const int srcFactorRGB,
		const int destFactorRGB,
		const int srcFactorAlpha,
		const int destFactorAlpha,
		const int blendModeRGB,
		const int blendModeAlpha,
		const int mask = ALL,
		const bool alphaToCoverage = false) = 0;
	virtual DepthStateID addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc = LEQUAL, const bool stencilTest = false,
		const uint8 stencilMask = 0xFF, const int stencilFunc = ALWAYS, const int stencilFail = KEEP, const int depthFail = KEEP, const int stencilPass = KEEP) = 0;
	virtual RasterizerStateID addRasterizerStateVrtl(const int cullMode,  const int fillMode ,  const bool multiSample ,  const bool scissor ) = 0;

inline RasterizerStateID addRasterizerState(const int cullMode, const int fillMode = SOLID, const bool multiSample = true, const bool scissor = false){return addRasterizerStateVrtl(cullMode, fillMode, multiSample, scissor);}

	//???static FontID addFont(const char *textureFile, const char *fontFile, const SamplerStateID samplerState);
inline unsigned int Draw(Primitives PrimitiveType,
		UINT StartVertex,
		UINT PrimitiveCount, const unsigned int flags=RESET_ALL)
{
	return DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount, flags);
}
	virtual unsigned int DrawPrimitive(Primitives PrimitiveType,
		UINT StartVertex,
		UINT PrimitiveCount, const unsigned int flags=RESET_ALL)=0;
	virtual unsigned int DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation){return 0;} // ???

inline unsigned int DrawIndexed(Primitives PrimType, UINT IndexCount, UINT startIndex, UINT VertexStart, const unsigned int flags=RESET_ALL)
{
	UINT PrimCount=IndexCount/getIndicesCount(PrimType);
	return DrawIndexedPrimitive(PrimType,0,0,
		getVertexCount(PrimType, PrimCount),
		startIndex,
		PrimCount, flags);
}

    virtual unsigned int DrawIndexedPrimitive(Primitives PrimitiveType,
		INT BaseVertexIndex,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT startIndex,
		UINT primCount, const unsigned int flags=RESET_ALL)=0;
    virtual unsigned int DrawPrimitiveUP(Primitives PrimitiveType,
		UINT PrimitiveCount,
		CONST void* pVertexStreamZeroData0,
		CONST void* pVertexStreamZeroData,
		UINT VertexStreamZeroStride, const unsigned int flags=RESET_ALL)=0;


    inline unsigned int DrawPrimitiveUP(Primitives PrimitiveType,
		UINT PrimitiveCount,
		CONST void* pVertexStreamZeroData0,
		UINT VertexStreamZeroStride, const unsigned int flags=RESET_ALL)
    {
        return DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData0, VertexStreamZeroStride, flags);
    }

/*
    virtual unsigned int DrawPrimitiveUPSWIG(Primitives PrimitiveType,
		UINT PrimitiveCount,
		CONST void* pVertexStreamZeroData0,
		CONST void* pVertexStreamZeroData,
		UINT VertexStreamZeroStride, unsigned int flags)
	{	
    	DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData0, pVertexStreamZeroData, VertexStreamZeroStride, flags);
	}
*/
    virtual unsigned int DrawIndexedPrimitiveUPVrtl(Primitives PrimitiveType,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT PrimitiveCount,
		CONST void* pIndexData0,
		CONST void* pIndexData=0,
		STX_INDEXFORMAT IndexDataFormat=CONSTANT_INDEX2,
		CONST void* pVertexStreamZeroData0=0,
		CONST void* pVertexStreamZeroData=0,
		UINT VertexStreamZeroStride=0, const unsigned int flags=RESET_ALL)=0;
inline unsigned int DrawIndexedPrimitiveUP(Primitives PrimitiveType,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT PrimitiveCount,
		CONST void* pIndexData0,
		CONST void* pIndexData,
		STX_INDEXFORMAT IndexDataFormat=CONSTANT_INDEX2,
		CONST void* pVertexStreamZeroData0=0,
		CONST void* pVertexStreamZeroData=0,
		UINT VertexStreamZeroStride=0, const unsigned int flags=RESET_ALL)
{
    return DrawIndexedPrimitiveUPVrtl(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData0, pIndexData, IndexDataFormat, pVertexStreamZeroData0, pVertexStreamZeroData, VertexStreamZeroStride, flags);
}

    virtual void BeginSceneVrtl(bool aB)=0;

inline void BeginScene(bool aB=false){BeginSceneVrtl(aB);}
    virtual void EndSceneVrtl(bool aB)=0;

inline void EndScene(bool aB=false){EndSceneVrtl(aB);}
	virtual void Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors=D3DXFROMWINEVECTOR4(0.5f, 0.5f, 0.5f, 0.5f))=0;
	inline void Clear(bool color, bool depth, __DWORD__ acolor)
	{
		D3DXFROMWINEVECTOR4 colors=stx_GetRGBA(acolor);
		Clear(color, depth, colors);
	}
    virtual void PresentVrtl(bool aB)=0;

inline void Present(bool aB=false){PresentVrtl(aB);}




    virtual void RenderTexVrtl(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition, D3DXFROMWINEVECTOR2 texsize)=0;
    inline void RenderTex(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 texsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f))
	{
		RenderTexVrtl(id, aposition, asize, texposition, texsize);
	}

	inline void drawQuad(TextureID id)
	{
	ShaderID shd=-1;
	VertexFormatID vf=-1;
	if(shd==-1)
	{
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.shd", "mainVS", "mainPS");
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	}
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	IRenderer::GetRendererInstance()->setTexture("Base", id);
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	IRenderer::GetRendererInstance()->setShaderConstant4f("color", color);
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	//		   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
	}

    virtual unsigned int drawTexturedVrtl(const Primitives primitives,  TexVertex *vertices,  const unsigned int nVertices,  const TextureID texture,  const BlendStateID blendState,  const DepthStateID depthState,  const D3DXFROMWINEVECTOR4 *color )=0;

inline unsigned int drawTextured(const Primitives primitives, TexVertex *vertices, const unsigned int nVertices, const TextureID texture, const BlendStateID blendState = -1, const DepthStateID depthState = -1, const D3DXFROMWINEVECTOR4 *color = 0){return drawTexturedVrtl(primitives, vertices, nVertices, texture, blendState, depthState, color);}

inline unsigned int drawTexturedV6(CONST Primitives primitives, CONST VOID *vertices, CONST int nVertices, CONST TextureID texture, CONST BlendStateID blendState, CONST DepthStateID depthState){return drawTextured(primitives, (TexVertex *)vertices, nVertices, texture, blendState, depthState);}

inline unsigned int drawTexturedV7(CONST Primitives primitives, CONST VOID *vertices, CONST int nVertices, CONST TextureID texture, CONST BlendStateID blendState, CONST DepthStateID depthState, CONST D3DXFROMWINEVECTOR4 *color){return drawTextured(primitives, (TexVertex *)vertices, nVertices, texture, blendState, depthState, color);}


	virtual void applyTextures() = 0;


	virtual void setSamplerState(const char *samplerName, const SamplerStateID samplerState) = 0;
	virtual void applySamplerStates() = 0;

	virtual void applyConstants() = 0;

	virtual void setShader(const ShaderID shader)=0;
	virtual void setPixelShader(const ShaderID shader)=0;
	virtual void setVertexShader(const ShaderID shader)=0;
	virtual void setGeometryShader(const ShaderID shader)=0;
	virtual void setHullShader(const ShaderID shader)=0;
	virtual void setDomainShader(const ShaderID shader)=0;
	virtual void setComputeShader(const ShaderID shader)=0;

	virtual void setVertexFormat(const VertexFormatID vertexFormat)=0;
	inline void SetVertexDeclaration(const VertexFormatID vertexFormat){setVertexFormat(vertexFormat);}

	virtual void setVertexBufferVrtl(const int stream,  const VertexBufferID vertexBuffer,  const intptr offset )=0;

inline void setVertexBuffer(const int stream, const VertexBufferID vertexBuffer, const intptr offset = 0){setVertexBufferVrtl(stream, vertexBuffer, offset);}

		virtual void setVertexBuffer(const int stream, const void *base)=0;

	virtual void setIndexBuffer(const IndexBufferID indexBuffer)=0;




	virtual void setBlendStateVrtl(const BlendStateID blendState,  const unsigned int sampleMask )=0;

inline void setBlendState(const BlendStateID blendState, const unsigned int sampleMask = ~0){setBlendStateVrtl(blendState, sampleMask);}

	virtual void setAlphaState(const AlphaStateID alphaState)=0;

	virtual void setDepthStateVrtl(const DepthStateID depthState,  const unsigned int stencilRef )=0;

inline void setDepthState(const DepthStateID depthState, const unsigned int stencilRef = 0){setDepthStateVrtl(depthState, stencilRef);}

	virtual void setRasterizerState(const RasterizerStateID rasterizerState)=0;

#if 0
    g_CgFxEffect->GetParameterByName
    g_CgFxEffect->SetMatrix
    g_CgFxEffect->SetTexture
    g_CgFxEffect->Begin
    g_CgFxEffect->Pass
    g_CgFxEffect->End();
#endif

	virtual void setShaderConstant1i(const char *name, const int constant)=0;
	virtual void setShaderConstant1f(const char *name, const float constant)=0;


	virtual void setShaderConstant2f(const char *name, const D3DXFROMWINEVECTOR2 &constant)=0;
	virtual void setShaderConstant3f(const char *name, const D3DXFROMWINEVECTOR3 &constant)=0;
	virtual void setShaderConstant4f(const char *name, const D3DXFROMWINEVECTOR4 &constant)=0;
	virtual void setShaderConstant4x4f(const char *name, const D3DXFROMWINEMATRIX &constant)=0;

	virtual void setShaderConstantArray1f(const char *name, const float *constant, const unsigned int count)=0;

	virtual void setShaderConstantArray2f(const char *name, const D3DXFROMWINEVECTOR2  *constant, const unsigned int count)=0;
	virtual void setShaderConstantArray3f(const char *name, const D3DXFROMWINEVECTOR3  *constant, const unsigned int count)=0;
	virtual void setShaderConstantArray4f(const char *name, const D3DXFROMWINEVECTOR4  *constant, const unsigned int count)=0;
	virtual void setShaderConstantArray4x4f(const char *name, const D3DXFROMWINEMATRIX *constant, const unsigned int count)=0;

	virtual void setShaderConstantRaw(const char *name, const void *data, const int size) = 0;

	virtual void changeRenderTargetVrtl(const TextureID colorRT,  const TextureID depthRT ,  const int slice )=0;

inline void changeRenderTarget(const TextureID colorRT, const TextureID depthRT = TEXTURE_NONE, const int slice = NO_SLICE){changeRenderTargetVrtl(colorRT, depthRT, slice);}


	virtual void changeRenderTargets(const TextureID *colorRTs, const unsigned int nRenderTargets, const TextureID depthRT, const int *faces)=0;


	virtual void changeRenderTargetsVrtl(const TextureID *colorRTs,  const unsigned int nRenderTargets,  const TextureID depthRT ,  const int depthSlice ,  const int *slices ) = 0;

inline void changeRenderTargets(const TextureID *colorRTs, const unsigned int nRenderTargets, const TextureID depthRT = TEXTURE_NONE, const int depthSlice = NO_SLICE, const int *slices = 0){changeRenderTargetsVrtl(colorRTs, nRenderTargets, depthRT, depthSlice, slices);}
	virtual void changeToMainFramebuffer() = 0;

    inline void ClearDepthStencilView(const TextureID& DSV, int flags, float af, int i )
    {
        changeRenderTargets(0, 0, DSV);
        float f=128.0f/256.0f;
        Clear(false, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
        changeToMainFramebuffer();
    }

	virtual TextureID addTexture(Image3* img, const bool useMipMaps=true, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0
	
	) = 0;

	//static TextureID addTexture(const char *fileName, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0);
	//virtual TextureID addCubemapVrtl(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState, const int nArraySlices, unsigned int flags)=0;
	//inline TextureID addCubemap(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, const int nArraySlices = 1, unsigned int flags = 0){addCubemapVrtl(fileNames, useMipMaps, samplerState, nArraySlices, flags);}
	//static TextureID addNormalMap(const char *fileName, const FORMAT destFormat, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, float sZ = 1.0f, float mipMapScaleZ = 2.0f, unsigned int flags = 0);

	virtual TextureID addRenderTargetVrtl(const int width,  const int height,  const FORMAT format,  const SamplerStateID samplerState ,  unsigned int flags )=0;

inline TextureID addRenderTarget(const int width, const int height, const FORMAT format, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0){return addRenderTargetVrtl(width, height, format, samplerState, flags);}

inline void SetDepthStencilSurface(TextureID id){}

inline TextureID addDepthStencilSurface(const int width,  const int height,  const FORMAT format, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0){return addRenderDepthVrtl(width, height, 0, format, 1, samplerState, flags);}
inline TextureID CreateDepthStencilSurface(const int width,  const int height,  const FORMAT format, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0){return addRenderDepthVrtl(width, height, 0, format, 1, samplerState, flags);}

inline TextureID addRenderDepthStencil(const int width,  const int height,  const int a=0,  const int b=0,  const int c=0){return -1;} // ???

	virtual TextureID addRenderTargetVrtl(const int width,  const int height,  const int depth,  const int mipMapCount,  const int arraySize,  const FORMAT format,  const int msaaSamples ,  const SamplerStateID samplerState ,  unsigned int flags ) = 0;

inline TextureID addRenderTarget(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0){return addRenderTargetVrtl(width, height, depth, mipMapCount, arraySize, format, msaaSamples, samplerState, flags);}
	virtual TextureID addRenderDepth(const int width, const int height, const int depthBits)=0;


	virtual TextureID addRenderDepthVrtl(const int width,  const int height,  const int arraySize,  const FORMAT format,  const int msaaSamples ,  const SamplerStateID samplerState ,  unsigned int flags ) = 0;

inline TextureID addRenderDepth(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, unsigned int flags = 0){return addRenderDepthVrtl(width, height, arraySize, format, msaaSamples, samplerState, flags);}

	virtual bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize) = 0;
	virtual bool generateMipMaps(const TextureID renderTarget) = 0;



	virtual void changeShader(const ShaderID shader) = 0;
	virtual void changeVertexFormat(const VertexFormatID vertexFormat) = 0;
	virtual void changeVertexBufferVrtl(const int stream,  const VertexBufferID vertexBuffer,  const intptr offset ) = 0;

inline void changeVertexBuffer(const int stream, const VertexBufferID vertexBuffer, const intptr offset = 0){changeVertexBufferVrtl(stream, vertexBuffer, offset);}
	virtual void changeVertexBuffer(const void *base)=0;

	virtual void changeIndexBuffer(const IndexBufferID indexBuffer) = 0;

//	virtual void changeSamplerState(const unsigned int samplerUnit, const SamplerStateID samplerState) = 0;

	virtual int getSamplerIndex(ShaderID shd, const char* aName)=0;

	virtual void changeBlendStateVrtl(const BlendStateID blendState,  const unsigned int sampleMask ) = 0;

inline void changeBlendState(const BlendStateID blendState, const unsigned int sampleMask = ~0){changeBlendStateVrtl(blendState, sampleMask);}
	virtual void changeAlphaState(const AlphaStateID alphaState)=0;
	virtual void changeDepthStateVrtl(const DepthStateID depthState,  const unsigned int stencilRef ) = 0;

inline void changeDepthState(const DepthStateID depthState, const unsigned int stencilRef = 0){changeDepthStateVrtl(depthState, stencilRef);}
	virtual void changeRasterizerState(const RasterizerStateID rasterizerState) = 0;

    virtual unsigned int DrawFPSVrtl(int x,  int y)=0;

inline unsigned int DrawFPS(int x=10, int y=10){DrawFPSVrtl(x, y);}

	virtual void setup2DMode(const float left, const float right, const float top, const float bottom)=0;

    virtual void SaveScreenShotVrtl(const char* fn)=0;

inline void SaveScreenShot(const char* fn=0){SaveScreenShotVrtl(fn);}
	virtual void captureScreenshot(Image3& img)=0;

	virtual float getStringWidth(FontID font,const char* str, const float charWidth)=0;
	virtual void fillTextBuffer(TexVertex *dest, const char *str, float x, float y, const float charWidth, const float charHeight, const FontID font)=0;
	virtual unsigned int drawText(const char *str, float x, float y, const float charWidth, const float charHeight, const FontID& font, const SamplerStateID samplerState = SS_NONE, const BlendStateID blendState = -1, const DepthStateID depthState = -1, const D3DXFROMWINEVECTOR4 col=D3DXFROMWINEVECTOR4(1.0f, 1.0f, 1.0f, 1.0f))=0;
	virtual unsigned int getTextQuads(const char *str) const=0;
	virtual void elevate(float add)=0;
	virtual float GetAspect()=0;
	virtual unsigned int drawPlain(const Primitives primitives, D3DXFROMWINEVECTOR2 *vertices, const unsigned int nVertices, const BlendStateID blendState = -1, const DepthStateID depthState = -1, const D3DXFROMWINEVECTOR4 *color=0)=0;
	virtual float getTextWidth(const FontID font, const char *str, int length = -1) const=0;

	virtual unsigned int drawLineVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline unsigned int drawLine(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 1){return drawLineVrtl(x0, y0, x1, y1, color, lineWidth);}
	virtual unsigned int drawRectVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline unsigned int drawRect(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return drawRectVrtl(x0, y0, x1, y1, color, lineWidth);}
	virtual unsigned int drawRoundRectVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const float r,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline unsigned int drawRoundRect(const float x0, const float y0, const float x1, const float y1, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return drawRoundRectVrtl(x0, y0, x1, y1, r, color, lineWidth);}

virtual unsigned int drawEllipseVrtl(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawEllipse(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return  drawEllipseVrtl(originx, originy, width, height, color, lineWidth);}



virtual unsigned int drawParaboleVrtl(const float xpos, const float x0, const float k, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawParabole(const float xpos, const float x0, const float k, const D3DXFROMWINEVECTOR4 &color, const float lineWidth=0){return drawParaboleVrtl(xpos, x0, k, color, lineWidth);}



virtual unsigned int drawParaboleVrtl(const float p, const float h, const unsigned int k, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawParabole(const float p, const float h, const unsigned int k, const D3DXFROMWINEVECTOR4 &color, const float lineWidth=0){return drawParaboleVrtl(p, h, k, color, lineWidth);}


virtual unsigned int drawParaboleVrtl(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawParabole(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return  drawParaboleVrtl(originx, originy, width, height, color, lineWidth);}

virtual unsigned int drawParaboleVrtl(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawParabole(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return  drawParaboleVrtl(x1, y1, x2, y2, x3, y3, color, lineWidth);}

virtual unsigned int drawHyperboleVrtl(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth)=0;
inline  unsigned int drawHyperbole(const float originx, const float originy, const float width, const float height, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return  drawHyperboleVrtl(originx, originy, width, height, color, lineWidth);}

virtual unsigned int drawBezierVrtl(D3DXFROMWINEVECTOR2 anchor1, D3DXFROMWINEVECTOR2 anchor2, D3DXFROMWINEVECTOR2 control1, D3DXFROMWINEVECTOR2 control2, const D3DXFROMWINEVECTOR4 &color, const float lineWidth=0)=0;
unsigned int drawBezier(D3DXFROMWINEVECTOR2 anchor1, D3DXFROMWINEVECTOR2 anchor2, D3DXFROMWINEVECTOR2 control1, D3DXFROMWINEVECTOR2 control2, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){drawBezierVrtl(anchor1, anchor2, control1, control2, color, lineWidth);}

	virtual unsigned int drawCircleVrtl(const float x,  const float y,  const float r,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline unsigned int drawCircle(const float x, const float y, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return drawCircleVrtl(x, y, r, color, lineWidth);}
	virtual unsigned int drawTriangleVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const float x2,  const float y2,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline unsigned int drawTriangle(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){return drawTriangleVrtl(x0, y0, x1, y1, x2, y2, color, lineWidth);}
	virtual unsigned int drawQuadVrtl(const float x0, const float y0, const float x1, const float y1, const float s0, const float t0, const float s1, const float t1, const D3DXFROMWINEVECTOR4 &color)=0;
inline unsigned int drawQuad(const float x0, const float y0, const float x1, const float y1, const float s0, const float t0, const float s1, const float t1, const D3DXFROMWINEVECTOR4 &color){return drawQuadVrtl(x0, y0, x1, y1, s0, t0, s1, t1, color);}
	virtual unsigned int drawArrays(const Primitives primitives, const int firstVertex, const int nVertices)=0;
	virtual unsigned int drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices)=0;


	virtual unsigned int drawPrimitivesVrtl(	const Primitives primitives,
						const unsigned int firstv, const unsigned int countv,
						const unsigned int firsti, const unsigned int counti,
				const ubyte* vbdata, const unsigned int vbsize,
				const ubyte* ibdata, const unsigned int ibsize)=0;
	inline unsigned int drawPrimitives(	const Primitives primitives,
						const unsigned int firstv=0, const unsigned int countv=0,
						const unsigned int firsti=0, const unsigned int counti=0,
				const ubyte* vbdata=0, const unsigned int vbsize=0,
				const ubyte* ibdata=0, const unsigned int ibsize=2)
	{drawPrimitivesVrtl(primitives, firstv, countv, firsti, counti, vbdata, vbsize, ibdata, ibsize);}
	virtual void getExtentsVrtl(FontID font,char *string, float x, float y, const float charWidth, const float charHeight, const float skewX = 0.0f, const float skewY = 0.0f,
		//const FONT_HADJUST hAdjust = HA_LEFT,
		//const FONT_VADJUST vAdjust = VA_TOP,
		float *left = 0, float *right = 0, float *top = 0, float *bottom = 0){}

	virtual void InitTex(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitTex3(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlain(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlain3(ShaderID& s, VertexFormatID& vf)=0;

	virtual void InitTexN(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitTex2N(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitTex3N(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlainN(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlain3N(ShaderID& s, VertexFormatID& vf)=0;
};

#ifndef SWIG
class RENDERERAPI IShaderTranslator
{
public:
	IShaderTranslator(){}
	virtual ~IShaderTranslator(){}
public:
    typedef enum
{
	eDefault,
	eHLSL2GLSLHLSL2GLSL,
	eHLSL2GLSLBoost,
/*
	eGLSL2HLSL,
	eHLSL2GLSLES,
	eGLSLES2HLSL,
*/
} tShaderTranslator;
public:
	virtual bool TranslateShader(ShaderType aType, const std::string& hlsl, const std::string& main, std::string& glsl, const int version)=0;
};
struct RENDERERAPI Constant {
	Constant(){}
	virtual ~Constant(){}
};
struct RENDERERAPI Sampler {
	Sampler(){}
	virtual ~Sampler(){}
};
struct RENDERERAPI SamplerState
{


public:


	SamplerState(){}
	virtual ~SamplerState(){}
};
struct RENDERERAPI BlendState
{


public:


	BlendState(){}
	virtual ~BlendState(){}
};
struct RENDERERAPI AlphaState
{


public:


	AlphaState(){}
	virtual ~AlphaState(){}
};
struct RENDERERAPI DepthState
{


public:


	DepthState(){}
	virtual ~DepthState(){}
};
struct RENDERERAPI RasterizerState
{


public:


	RasterizerState(){}
	virtual ~RasterizerState(){}
};
class RENDERERAPI HLSL2GLSLTranslator : public IShaderTranslator
{
public:
	HLSL2GLSLTranslator();
	virtual ~HLSL2GLSLTranslator();
	virtual bool TranslateShader(ShaderType aType, const std::string& hlsl, const std::string& main, std::string& glsl, const int version);
};

class HlslCrossCompiler;
typedef HlslCrossCompiler* ShHandle;
class RENDERERAPI HLSL2GLSLHLSL2GLSLTranslator : public HLSL2GLSLTranslator
{
public:
	HLSL2GLSLHLSL2GLSLTranslator();
	virtual ~HLSL2GLSLHLSL2GLSLTranslator();
	virtual bool TranslateShader(ShaderType aType, const std::string& hlsl, const std::string& main, std::string& glsl, const int version);
};

class RENDERERAPI HLSL2GLSLBoostTranslator : public HLSL2GLSLTranslator
{
public:
	HLSL2GLSLBoostTranslator();
	virtual ~HLSL2GLSLBoostTranslator();
	virtual bool TranslateShader(ShaderType aType, const std::string& hlsl, const std::string& main, std::string& glsl, const int version);
};
#ifndef IMPLEMENT
#define IMPLEMENT
//LOG_PRINT("implement %s:%d\n",__FUNCTION__,__LINE__)
#endif
class RENDERERAPI CCamera {

public:
#if 0
	D3DXFROMWINEMATRIX GetProjMatrix(){IMPLEMENT;D3DXFROMWINEMATRIX& m;return m;}
    D3DXFROMWINEMATRIX GetViewMatrix(){IMPLEMENT;D3DXFROMWINEMATRIX& m;return m;}
#else
	D3DXFROMWINEMATRIX GetProjMatrix(){IMPLEMENT;D3DXFROMWINEMATRIX m;return m;}
    D3DXFROMWINEMATRIX GetViewMatrix(){IMPLEMENT;D3DXFROMWINEMATRIX m;return m;}
	void SetViewParams(D3DXFROMWINEVECTOR3 apos,
		D3DXFROMWINEVECTOR3 alook){IMPLEMENT;}
	void SetProjParams(float angle, float aspect , float nearClip, float farClip){IMPLEMENT;}
	void reset(){IMPLEMENT;}
	void controls3(){IMPLEMENT;}
	void elevate(float e){IMPLEMENT;}
#endif
	// Our camera constructor
	CCamera();
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// These are are data access functions for our camera's private data
	D3DXFROMWINEVECTOR3 Position() {	return m_vPosition;		}
	D3DXFROMWINEVECTOR3 View()		{	return m_vView; 		}
	D3DXFROMWINEVECTOR3 UpVector() {	return m_vUpVector;		}
	D3DXFROMWINEVECTOR3 Strafe() {	return m_vStrafe;		}
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
					    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);
	// This rotates the camera's view around the position depending on the values passed in.
	void RotateView(float angle, float X, float Y, float Z);
	// This moves the camera's view by the mouse movements (First person view)
	void SetViewByMouse();
	// This rotates the camera around a point (I.E. your character).
	void RotateAroundPoint(D3DXFROMWINEVECTOR3 vCenter, float X, float Y, float Z);
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// This strafes the camera left or right depending on the speed (+/-)
	void StrafeCamera(float speed);
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);
	// This checks for keyboard movement
	void CheckForMovement(
		IInput* Input=0
		);
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// This updates the camera's view and other data (Should be called each frame)
	void Update();
	// This uses gluLookAt() to tell OpenGL where to look
	void Look();
public:
	D3DXFROMWINEMATRIX matProj;
	D3DXFROMWINEMATRIX matView;
	D3DXFROMWINEMATRIX matWorld;
	// The camera's position
	D3DXFROMWINEVECTOR3 m_vPosition;
	D3DXFROMWINEVECTOR3 _pos;
	// The camera's view
	D3DXFROMWINEVECTOR3 m_vView;
	D3DXFROMWINEMATRIX mv;
	// The camera's up vector
	D3DXFROMWINEVECTOR3 m_vUpVector;
	// The camera's strafe vector
	D3DXFROMWINEVECTOR3 m_vStrafe;
/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
};
//typedef CCamera Camera;
#ifndef _WIN32
//-------------------------------------------------------------------

// Flexible vertex format bits
//
#define D3DFVF_RESERVED0        0x001
#define D3DFVF_POSITION_MASK    0x400E
#define D3DFVF_XYZ              0x002
#define D3DFVF_XYZRHW           0x004
#define D3DFVF_XYZB1            0x006
#define D3DFVF_XYZB2            0x008
#define D3DFVF_XYZB3            0x00a
#define D3DFVF_XYZB4            0x00c
#define D3DFVF_XYZB5            0x00e
#define D3DFVF_XYZW             0x4002

#define D3DFVF_NORMAL           0x010
#define D3DFVF_PSIZE            0x020
#define D3DFVF_DIFFUSE          0x040
#define D3DFVF_SPECULAR         0x080

#define D3DFVF_TEXCOUNT_MASK    0xf00
#define D3DFVF_TEXCOUNT_SHIFT   8
#define D3DFVF_TEX0             0x000
#define D3DFVF_TEX1             0x100
#define D3DFVF_TEX2             0x200
#define D3DFVF_TEX3             0x300
#define D3DFVF_TEX4             0x400
#define D3DFVF_TEX5             0x500
#define D3DFVF_TEX6             0x600
#define D3DFVF_TEX7             0x700
#define D3DFVF_TEX8             0x800

#define D3DFVF_LASTBETA_UBYTE4   0x1000
#define D3DFVF_LASTBETA_D3DCOLOR 0x8000

#define D3DFVF_RESERVED2         0x6000  // 2 reserved bits

inline void mkdir(const char* f){}
#endif
struct RENDERERAPI Cube
{
	public:
		VertexFormatID cubeVF;
		VertexBufferID cubeVB,cubeVBI;
		IndexBufferID cubeIB;
	Cube(){}
	virtual ~Cube(){}
	void init( ShaderID shader=-1 );
	void render(void );
};
struct RENDERERAPI Quad
{
	public:
		VertexFormatID quadVF;
		VertexBufferID triVB;
	Quad(){}
	virtual ~Quad(){}
	void init( ShaderID shader=-1 );
	void render(void );
};
struct RENDERERAPI FastBuffers
{
	virtual ~FastBuffers(){}
	int ARRAY_SIZE;
	int INDEX_COUNT;
	FastBuffers(int aARRAY_SIZE=512);
	VertexBufferID arrayVB;
	IndexBufferID  arrayIB;
	VertexFormatID arrayVF;
	//bool supportsHalf;
	void fillTerrainIB(unsigned int *iDest);
	void init( ShaderID shader=-1 );
	void render(void );
};

FORMAT FORMAT3FromD3D (UINT Format);

#endif

#if 0
#ifndef D3DFROMWINECOLORVALUE_DEFINED
typedef struct _D3DFROMWINECOLORVALUE {
    float r;
    float g;
    float b;
    float a;
} D3DFROMWINECOLORVALUE;
#define D3DFROMWINECOLORVALUE_DEFINED
#endif
typedef struct _D3DFROMWINEMATERIAL9 {
    D3DFROMWINECOLORVALUE   Diffuse;        /* Diffuse color RGBA */
    D3DFROMWINECOLORVALUE   Ambient;        /* Ambient color RGB */
    D3DFROMWINECOLORVALUE   Specular;       /* Specular 'shininess' */
    D3DFROMWINECOLORVALUE   Emissive;       /* Emissive color RGB */
    float           Power;          /* Sharpness if specular highlight */
} D3DFROMWINEMATERIAL9;
#endif
class CTimer
{
	__DWORD__ mStartTime;
public:
	CTimer();
	virtual ~CTimer();
	CTimer(const CTimer& rhs);
	float GetTime();
	void Begin();
	void Stop();
	bool IsRunning();
	static void UpdateAll(float aElapsedTime);
};
class CAnimSequence
{
public:
	std::vector<TextureID> m_Sequence;
	std::vector<float> m_Duration;
	std::vector<float> m_Then;
	CAnimSequence();
	virtual ~CAnimSequence();
	CAnimSequence(const CAnimSequence& rhs);
	void AddFrame(const char* aImageFileName, float aDuration);
	unsigned int Size();
};
class CSprite
{
	CAnimSequence m_Sequence;
	float m_Size;
	unsigned int m_index;
	__DWORD__ m_dt;
public:
	CSprite();
	virtual ~CSprite();
	CSprite(const CSprite& rhs);
	void Render(D3DFROMWINEMATRIX& aViewMatrix);
	void SetAnim(CAnimSequence& aAnim);
  	void SetSize(float aSize);
};
#ifdef HAS_GLCG
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif
struct XSampler : public Sampler 
{
	XSampler() : Sampler()
	{	name="";
		location=0;
		unit=0;
	}
	virtual ~XSampler(){}
	XSampler(const XSampler& rhs)
	{	name=rhs.name;
		location=rhs.location;
		unit=rhs.unit;
	}
	std::string name;
	int location, unit;
#ifdef HAS_GLCG
	CGparameter param;
	std::string paramname;
#endif
};

inline int compXSampler(const void *s0, const void *s1){
        return strcmp(((XSampler *) s0)->name.c_str(), ((XSampler *) s1)->name.c_str());
}
inline int getSamplerUnit(const ShaderID shader, const char *samplerName, 
				std::vector<XSampler>& samplers)
{
	int minSampler = 0;
	int maxSampler = samplers.size() - 1;
	while (minSampler <= maxSampler)
	{
		int currSampler = (minSampler + maxSampler) >> 1;
		int res = strcmp(samplerName, (samplers[currSampler]).name.c_str());
		if (res == 0)
			return (samplers[currSampler]).location;
		else if (res > 0)
			minSampler = currSampler + 1;
		else
			maxSampler = currSampler - 1;
	}
	return -1;
}

inline int getShaderParameterIndex(const ShaderID shader, const char *samplerName, 
				std::vector<XSampler>& samplers)
{
	return getSamplerUnit(shader, samplerName, samplers);
}

struct STX_Rect
{
	STX_Rect(int ax=0, int ay=0, int aw=0, int ah=0)
	{	x=ax;
		y=ay;
		w=aw;
		h=ah;
	}
	virtual ~STX_Rect(){}
	int x;
	int y;
	int w;
	int h;
};

inline void STX_RenderFillRect(STX_Rect& rect)
{
	D3DXFROMWINEVECTOR4 color(1.0f, 1.0f, 1.0f, 1.0f);
	float lineWidth = 0;
	IRenderer::GetRendererInstance()->drawRect(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h, color, lineWidth);
}
inline void STX_DrawTexture(TextureID texture)
{
	static ShaderID shd=-1;
	static VertexFormatID vf=-1;
	if(shd==-1)
	{
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.shd", "main", "main"); 
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	}
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setTexture("Base", texture);
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	IRenderer::GetRendererInstance()->setShaderConstant4f("color", color);
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	//		   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
}
#ifndef SWIG
#include <STX/STX.h>
#include <STX/STXCPP.h>
#include <STX/STXinterfaces.h>
#include <STX/CInput.h>
#include <STX/Noise.h>
#if 0
#include <STX/FilteredNoise.h>
#include <STX/noise_ex.h>
#include <STX/KdTree.h>
#include <STX/PerlinNoise.h>
#endif
#include <Framework3/RendererHLSLCg.h>
//include <Framework3/camera3.h>
#include <STX/LuaUtil.h>
using namespace LuaUtil;
#include <tinyxml/tinyxml.h>
#endif
//include <Framework3/SDKmesh.h>
struct MeshRenderer2;
struct stx_SkyBox
{
    stx_SkyBox()
    {
    	env=-1;
    	skyBox=-1;
    	skyboxVF=-1;
	m_pEffect=0;
	m_pMesh=0;
    	D3DXFROMWINEMatrixIdentity(&mvp);
    }
    virtual ~stx_SkyBox(){if(m_pMesh) delete m_pMesh;}
    D3DXFROMWINEVECTOR4 fogColor;
    TextureID env;
    const bool mipmaps=false;
    ShaderID skyBox;
    stx_Effect* m_pEffect;
	MeshRenderer2* m_pMesh;
    VertexFormatID skyboxVF;
    D3DXFROMWINEVECTOR3 camPos;
    D3DXFROMWINEMATRIX mvp;
void OnCreateDevice(const unsigned int aN, const TextureID& aEnv, const char* aEffectFileName );
int init(const char* aName="Olympus", const char* aExt=0)
{
	skyBox = IRenderer::GetRendererInstance()->addShaderFromFile("/SkyBox/skybox.shd", "main", "main");
	
	FormatDesc skyboxFmt[] = { 0, TYPE_VERTEX, FORMAT_FLOAT, 3 };
	
	skyboxVF = IRenderer::GetRendererInstance()->addVertexFormat(skyboxFmt, elementsOf(skyboxFmt), skyBox);

	camPos=D3DXFROMWINEVECTOR3(0.0f, 0.8f, 0.0f);
	fogColor = D3DXFROMWINEVECTOR4(1.0f, 0.5f, 0.1f, 1.0f);
	D3DXFROMWINEMatrixPerspectiveFovLH(&mvp, D3DXFROMWINE_PI / 2.0f, float(IRenderer::GetRendererInstance()->GetViewportWidth())/float(IRenderer::GetRendererInstance()->GetViewportHeight()), 0.001f, 1200000.0f);

    if(aExt)
	    env = IRenderer::GetRendererInstance()->addCubemap(aName, aExt);
    else
	    env = IRenderer::GetRendererInstance()->addImageLibTexture(aName, false, IRenderer::GetRendererInstance()->Getlinear());

	return 0;
}
void render(const D3DXFROMWINEMATRIX* aMvp=0);
void render_()
{
	float vertices[] = { 0, 2, 0, 0, -2, -1, -1, -2, 1, 1, -2, 1 };
	ushort indices[] = { 0, 1, 2, 3, 0, 1 };
	IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLE_STRIP, 0, 4, 4, indices, indices, CONSTANT_INDEX2, vertices, vertices, sizeof(D3DXFROMWINEVECTOR3));
}
};

typedef int EffectID;
typedef int EffectTechniqueID;
typedef int DepthStencilViewID;
typedef int ShaderResourceViewID;
typedef int EffectShaderResourceVariableID;
typedef int EffectScalarVariableID;
typedef int EffectVectorVariableID;
typedef int EffectMatrixVariableID;
typedef int RenderTargetViewID;
//typedef D3DXFROMWINEVECTOR4 D3DXFROMWINECOLOR;
//typedef D3DXFROMWINEMATRIX CModelViewerCamera;
//stx_Effect addEffectFromFile(const char* aFileName);

struct TECHNIQUE_DESC
{
	TECHNIQUE_DESC(){Passes=0;}
	virtual ~TECHNIQUE_DESC(){}
	TECHNIQUE_DESC(const TECHNIQUE_DESC& rhs){Passes=rhs.Passes;}
	unsigned int Passes;
};

struct stx_Effect
{
//protected:
	struct stx_Pass
	{
	    struct stx_Texture_
	    {
		    stx_Texture_()
		    {
                //<Texture name="Base" file="/test.bmp" dimension="2" mipmap="false" SamplerState="linear" />
		        texID=-1;
                m_iDimension=2;
                m_bUseMipmap=false;
                ss=-1;
            }
		    virtual ~stx_Texture_(){}
		    stx_Texture_(const stx_Texture_& rhs)
		    {
			    m_sName=rhs.m_sName;
                m_sFile=rhs.m_sFile;
		        texID=rhs.texID;
                m_iDimension=rhs.m_iDimension;
                m_bUseMipmap=rhs.m_bUseMipmap;
                ss=rhs.ss;
            }
		        std::string m_sName;
		        std::string m_sFile;
		        TextureID texID;
                unsigned int m_iDimension;
                bool m_bUseMipmap;
                SamplerStateID ss;
        };
		stx_Pass()
		{
			Shader=-1;
        		VertexFormat=-1;
			SamplerState=-1;
			BlendState=-1;
			AlphaState=-1;
        		DepthStencilState=-1;
        		RasterizerState=-1;
		}
		stx_Pass(const char * aFileName,
const char *vsMain, const char *gsMain, const char *fsMain, const char *csMain, const char *hsMain, const char *dsMain)
		{
			Shader=-1;
        		VertexFormat=-1;
			SamplerState=-1;
			BlendState=-1;
			AlphaState=-1;
        		DepthStencilState=-1;
        		RasterizerState=-1;
			#if 1
			Shader=IRenderer::GetRendererInstance()->addShaderFromFile(aFileName, vsMain, fsMain);
			#else
			Shader=IRenderer::GetRendererInstance()->addShaderFromFile(aFileName, vsMain, gsMain, fsMain, csMain, hsMain, dsMain);
			#endif
		}
		virtual ~stx_Pass(){}
		void Apply(unsigned int aI){} // ???
		stx_Pass(const stx_Pass& rhs)
		{
			//m_vTextures=rhs.m_vTextures;
			m_sName=rhs.m_sName;
			Shader=rhs.Shader;

			PixelShaderprofile=rhs.PixelShaderprofile;
			PixelShadermain=rhs.PixelShadermain;
			VertexShaderprofile=rhs.VertexShaderprofile;
			VertexShadermain=rhs.VertexShadermain;
			HullShaderprofile=rhs.HullShaderprofile;
			HullShadermain=rhs.HullShadermain;
			DomainShaderprofile=rhs.DomainShaderprofile;
			DomainShadermain=rhs.DomainShadermain;
			GeometryShaderprofile=rhs.GeometryShaderprofile;
			GeometryShadermain=rhs.GeometryShadermain;
			ComputeShaderprofile=rhs.ComputeShaderprofile;
			ComputeShadermain=rhs.ComputeShadermain;

        		VertexFormat=rhs.VertexFormat;
        		format_=rhs.format_;
			SamplerState=rhs.SamplerState;
			SamplerName=rhs.SamplerName;
			BlendState=rhs.BlendState;
			AlphaState=rhs.AlphaState;
        		DepthStencilState=rhs.DepthStencilState;
        		RasterizerState=rhs.RasterizerState;
		}
		std::string m_sName;
		ShaderID Shader;
		std::string PixelShaderprofile;
		std::string PixelShadermain;
		std::string VertexShaderprofile;
		std::string VertexShadermain;
		std::string HullShaderprofile;
		std::string HullShadermain;
		std::string DomainShaderprofile;
		std::string DomainShadermain;
		std::string GeometryShaderprofile;
		std::string GeometryShadermain;
		std::string ComputeShaderprofile;
		std::string ComputeShadermain;
		std::vector<int> format_;
        	VertexFormatID VertexFormat;
		SamplerStateID SamplerState;
		std::string SamplerName;
		BlendStateID BlendState;
		AlphaStateID AlphaState;
        	DepthStateID DepthStencilState;
        	RasterizerStateID RasterizerState;
		//std::vector<stx_Texture_> m_vTextures;
	};
	struct stx_Variable
	{
        	std::string m_sName;
		stx_Variable(){}
		virtual ~stx_Variable(){}
		stx_Variable(const stx_Variable& rhs)
		{
            m_sName=rhs.m_sName;
		}

	void SetResourceArray( ShaderResourceViewID* id, unsigned int startindex, unsigned int aN )
	{
		for(unsigned int i=startindex;i<aN;i++)
		{
			//int getSamplerIndex(ShaderID shd, const char* aName);
			//SetTexture(id[i]); // sampler name ???
		}
	}
	void SetResource( ShaderResourceViewID id ){SetTexture(id);} // sampler name ???
        void SetFloat( float aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant1f(m_sName.c_str(), aValue);
        }
        void SetFloatVector( float* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantArray4f(m_sName.c_str(), (D3DXFROMWINEVECTOR4*)aValue, elementsOf(aValue));
        }
        void SetFloatVectorArray( float* aValue, unsigned int aS, unsigned int aN )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantArray4f(m_sName.c_str(), (D3DXFROMWINEVECTOR4*)aValue, aN);
        }
        void SetIntVector( int* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantRaw(m_sName.c_str(), (int*)aValue, elementsOf(aValue));
        }
	void SetFloatArray( float* aValue, unsigned int aS, unsigned int aN )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantArray1f(m_sName.c_str(), aValue, aN);
        }
        void SetMatrix( float* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant4x4f(m_sName.c_str(), *((D3DXFROMWINEMATRIX*)aValue));
        }
        void SetMatrix( D3DXFROMWINEMATRIX* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant4x4f(m_sName.c_str(), *aValue);
        }
        void SetVector( D3DXFROMWINEVECTOR4* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant4f(m_sName.c_str(), *aValue);
        }
		void SetInt(int aValue)
		{
			IRenderer::GetRendererInstance()->setShaderConstant1i(m_sName.c_str(), aValue);
		}
        void SetInt( int* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant1i(m_sName.c_str(), *aValue);
        }
        void SetValue( void* aPtr, unsigned int aSize )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantRaw(m_sName.c_str(), &aPtr, aSize);
        }
        void SetTexture( TextureID aValue )
        {
	        IRenderer::GetRendererInstance()->setTexture(m_sName.c_str(), aValue);
        }
        void SetBool( bool aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstantRaw(m_sName.c_str(), &aValue, sizeof(bool));
        }
        void SetMatrixArray( void* aPtr, unsigned int aSize, unsigned int aN)
	{
	        IRenderer::GetRendererInstance()->setShaderConstantRaw(m_sName.c_str(), &aPtr, sizeof(D3DXFROMWINEMATRIX)*aN);
	}
	};
	struct stx_Technique
	{
		stx_Technique(stx_Effect* pEffect=0){m_pEffect=pEffect;}
		virtual ~stx_Technique(){}
		stx_Technique(const stx_Technique& rhs)
		{
			m_pEffect=rhs.m_pEffect;
			m_sName=rhs.m_sName;
			m_vPasses=rhs.m_vPasses;
		}
		const char* GetName(){return m_sName.c_str();}
		void GetDesc( TECHNIQUE_DESC* techDesc ){if(techDesc)techDesc->Passes=m_vPasses.size();}
		stx_Pass* GetPassByIndex(unsigned int aI)
		{
			if((aI+1)>m_vPasses.size())
				return 0;
			return &m_vPasses[aI];
		} // ???
		stx_Pass* GetPassByName(const std::string& aName)
		{
			int idx=-1;
			for(unsigned int i=0;i<m_vPasses.size();i++)
			{
				if(m_vPasses[i].m_sName==aName)
				{
					idx=i;
					break;
				}	
			}
			if(idx>-1)
				return &m_vPasses[idx];
			return 0;
		} // ???
		stx_Effect* m_pEffect;
		std::string m_sName;
		std::vector<stx_Pass> m_vPasses;
		std::vector<stx_Variable> m_vVariables;
	};
	TiXmlDocument doc;
	TiXmlHandle* docHandle;
	TiXmlElement *m_pFxRootElement;
    	std::string m_sFX;
	std::string m_sXML;
	std::vector<stx_Effect::stx_Pass::stx_Texture_> m_vTextures;
	std::vector<stx_Technique> m_vTechniques;
	std::string m_sTechnique;
	int  m_iTechnique;
	std::string m_sFileName;
public:
	stx_Effect(){m_iTechnique=-1;m_pFxRootElement=0;}
	stx_Effect(const std::string& aFileName, const char* aDefines=0);
	std::string ParseEffect(const std::string& aFileName, const char* aDefines=0);
	virtual ~stx_Effect(){}
	stx_Effect(const stx_Effect& rhs)
	{
		docHandle=rhs.docHandle;
		m_sFileName=rhs.m_sFileName;
		m_sTechnique=rhs.m_sTechnique;
		m_iTechnique=rhs.m_iTechnique;
		m_vTechniques=rhs.m_vTechniques;
		m_vTextures=rhs.m_vTextures;
	}
        void SetMatrix( const char* aName, D3DXFROMWINEMATRIX* aValue );
        void SetVector( const char* aName, D3DXFROMWINEVECTOR4* aValue );
        void SetFloat( const char* aName, float* aValue )
        {
	        IRenderer::GetRendererInstance()->setShaderConstant1f(aName, *aValue);
        }

        void SetValue( const char* aName, void* aPtr, unsigned int aSize );
	void SetTexture( const char* aName, TextureID aValue );

	void SetBool( const char* aName, bool aValue );
	void SetTechnique( const char* aName );

		stx_Variable* GetVariableByName( const char* aName )
		{
			if(!aName)
				return 0;
			if(m_iTechnique<0)
				return 0;
			if(m_iTechnique>=m_vTechniques.size())
				return 0;
			int idx=-1;
			for(unsigned int i=0;i<m_vTechniques[m_iTechnique].m_vVariables.size();i++)
			{
				if(m_vTechniques[m_iTechnique].m_vVariables[i].m_sName==std::string(aName))
				{
					idx=i;
					break;
				}	
			}
			if(idx>-1)
				return &m_vTechniques[m_iTechnique].m_vVariables[idx];
			return 0;
		} // ???
		
		stx_Technique* GetTechniqueByName( const char* aName )
		{
			if(!aName)
				return 0;
			int idx=-1;
			for(unsigned int i=0;i<m_vTechniques.size();i++)
			{
				if(m_vTechniques[i].m_sName==std::string(aName))
				{
					idx=i;
					break;
				}	
			}
			if(idx>-1)
				return &m_vTechniques[idx];
			return 0;
		} // ???


	void Begin( unsigned int *cPasses, unsigned int aValue );
	void BeginPass( unsigned int iPass );
        void EndPass();
        void End();
};
#include "GUI.h"
typedef enum _tShader
{
	eShaderNone,
	eHLSL_Shader,
	eGLSL_Shader,
	eGLES_Shader_android,
	eGLES_Shader_Ubuntu,
		eSimpleShader,
		eSimpleShader2,
		eColorShader,
		eAmbientLighting,
		eDiffuseLighting,
		ePhongLighting1,
		ePhongLighting2,
		ePhysicalBasedLighting,
		ePositionTexture,
		ePositionNormal,
		ePositionNormalTexture,
		ePositionNormalNormal,
		eCelShader,
		eDirectionalLighting,
		eSpotLighting,
		eMultiLighting,
		eSnowShader,
		eCelShader1,
		eCelShader2,
		eCelShader3,
		eCelShader4,
		eSimpleLighting,
        e_Lighting
} tShader;
inline const char* GetShaderName(tShader aShader_)
{
	switch(aShader_)
	{
		case ePositionTexture:
			return "PositionTextureShader";
		case ePositionNormal:
			return "PositionNormalShader";
		case eSimpleShader:
			return "SimpleShader";
		case eSimpleShader2:
			return "SimpleShader2";
		case eColorShader:
			return "ColorShader";
		case eAmbientLighting:
			return "AmbientLighting";
		case eDiffuseLighting:
			return "DiffuseLighting";
		case ePhongLighting1:
			return "PhongLighting1";
		case ePhongLighting2:
			return "PhongLighting2";
		case eCelShader:
			return "CelShader";
		case eSnowShader:
			return "SnowShader";
		case eDirectionalLighting:
			return "DirectionalLighting";
		case eSpotLighting:
			return "SpotLighting";
		case eMultiLighting:
			return "MultiLighting";
		case eCelShader1:
			return "CelShader1";
		case eCelShader2:
			return "CelShader2";
		case eCelShader3:
			return "CelShader3";
		case eCelShader4:
			return "CelShader4";
		default:
			return "SimpleShader";
	}
	return "SimpleShader";
}

	enum tShape
	{
		eShapeNone,
		eTexturedBox,
		eBox,
		eTexturedQuad,
		eSphere,
		eHalfSphere,
		eCylinder,
		//eGrid,
		eCone,
		eOcta,
		eTetra,
		eEllipsoid,
#if 1
		eHemis,
		eTorus,

#endif
/*
		Hexahedron (cube)
		Tetrahedron
		Octahedron
		Icosahedron
		Dodecahedron
*/
		eMesh
	};

#include <MeshRenderer2.h>
#include <FW3.h>
//include <MeshRenderer3.h>
#ifndef NATURESCENEBUILD
#include <GUI/GUIUtils.h>
#endif
#endif

