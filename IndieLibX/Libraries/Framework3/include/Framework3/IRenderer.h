/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef _IRenderer_H_
#define _IRenderer_H_

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
#include <STX/Matrix.h>
#include <STX/STXinterfaces.h>

typedef enum _tShader
{
	eHLSL_Shader,
	eGLSL_Shader,
	eGLES_Shader_android,
	eGLES_Shader_Ubuntu
} tShader;

struct Framework3ShaderFactory
{
	static const char* GetShader(const char* aShaderName);
};

typedef int TextureID;
typedef int ShaderID;
typedef int VertexBufferID;
typedef int IndexBufferID;
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
	TYPE_TANGENT  = 4,
	TYPE_BINORMAL = 5,
};
enum RENDERERAPI AttributeFormat {
	FORMAT_FLOAT = 0,
	FORMAT_HALF  = 1,
	FORMAT_UBYTE = 2,
};
struct RENDERERAPI FormatDesc {
	/*FormatDesc()
	{
		stream=0;
		type=TYPE_VERTEX;
		format=FORMAT_FLOAT;
		size=3;
	}
	FormatDesc(int astream,
	AttributeType atype,
	AttributeFormat aformat,
	int asize)
	{
		stream=astream;
		type=atype;
		format=aformat;
		size=asize;
	}*/
	int stream;
	AttributeType type;
	AttributeFormat format;
	int size;
};
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


	VertexFormat(){}
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
    //DWORD               Usage;
    //D3DPOOL             Pool;

  //  D3DMULTISAMPLE_TYPE MultiSampleType;
    //DWORD               MultiSampleQuality;
    UINT                Width;
    UINT                Height;
} SURFACE_DESC;
typedef struct RENDERERAPI _VERTEXBUFFER_DESC
{
    //D3DFORMAT           Format;
    //D3DRESOURCETYPE     Type;
    //DWORD               Usage;
    //D3DPOOL             Pool;
    UINT                Size;

    //DWORD               FVF;

} VERTEXBUFFER_DESC;

/* Index Buffer Description */
typedef struct RENDERERAPI _INDEXBUFFER_DESC
{
    //D3DFORMAT
	int Format;
    //D3DRESOURCETYPE     Type;
    //DWORD               Usage;
    //D3DPOOL             Pool;
    UINT                Size;
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
struct RENDERERAPI IRenderer
{
public:
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

	inline IndexBufferID CreateIndexBuffer(unsigned int Length,DWORD Usage,unsigned int Format,unsigned int Pool,IndexBufferID* ppIndexBuffer)
	{
		/* D3DFORMAT
    D3DFMT_INDEX16              =101,
    D3DFMT_INDEX32              =102,
		*/
		assert(Format==sizeof(int));
		const uint nIndices=Length/Format;
		const uint indexSize=Format;
		*ppIndexBuffer=addIndexBuffer(nIndices, indexSize, STATIC, 0, -1);
		return *ppIndexBuffer;
	}
	inline int CreateVertexBuffer(unsigned int Length,DWORD Usage,DWORD FVF,unsigned int Pool,VertexBufferID* ppVertexBuffer)
	{
		*ppVertexBuffer=addVertexBuffer(Length, STATIC, 0, -1);
		return *ppVertexBuffer;
	}
	inline D3DFROMWINEVIEWPORT9 GetViewport(D3DFROMWINEVIEWPORT9* pViewport){return GetViewport();}
	inline VertexFormatID SetFVF(DWORD FVF)
	{
		#if 0
		DWORD tcsc=GetTexCoordSetsCount(FVF);
		DWORD offxyz=GetFVFComponentOffset(FVF, D3DFVF_XYZ, 0);
		DWORD offn=GetFVFComponentOffset(FVF, D3DFVF_NORMAL, 0);
		DWORD ss=0;
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
			DWORD s=GetTexCoordSetSize(FVF,i-2);
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
	inline int SetTexture(DWORD Stage,TextureID* pTexture)
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

			primCount);//, const uint flags=RESET_ALL);
	}
	inline int DrawPrimitive(Primitives PrimitiveType,unsigned int StartVertex,unsigned int PrimitiveCount)

	{
		return DrawPrimitive(PrimitiveType,
			StartVertex,
			PrimitiveCount);//, const uint flags=RESET_ALL);

	}
	inline int DrawPrimitiveUP(Primitives PrimitiveType,unsigned int PrimitiveCount,CONST void* pVertexStreamZeroData,unsigned int VertexStreamZeroStride)

	{
		return DrawPrimitiveUP(PrimitiveType,
			PrimitiveCount,
			pVertexStreamZeroData,

			pVertexStreamZeroData,
			VertexStreamZeroStride);//, const uint flags=RESET_ALL);
	}
	inline int DrawIndexedPrimitiveUP(Primitives PrimitiveType,unsigned int MinVertexIndex,unsigned int NumVertices,unsigned int PrimitiveCount,CONST void* pIndexData,unsigned int IndexDataFormat,CONST void* pVertexStreamZeroData,unsigned int VertexStreamZeroStride)
	{
		return DrawIndexedPrimitiveUP(PrimitiveType,
			 MinVertexIndex,
			NumVertices,
			PrimitiveCount,
			pIndexData,
			pIndexData,
		/*D3DFORMAT*/int IndexDataFormat,
			pVertexStreamZeroData,
			pVertexStreamZeroData,
			VertexStreamZeroStride);//, const uint flags=RESET_ALL);
	}
	inline int SetRenderState(D3DFROMWINERENDERSTATETYPE State,DWORD Value){return 0;}
	inline int SetSamplerState(DWORD Sampler,D3DFROMWINESAMPLERSTATETYPE Type,DWORD Value){return 0;}
	inline int SetTextureStageState(DWORD Stage,D3DFROMWINETEXTURESTAGESTATETYPE Type,DWORD Value){return 0;}
	inline int SetTransform(TS State,CONST D3DFROMWINEMATRIX* pMatrix){return 0;}
#endif
#endif
	TextureID addImageLibTexture(const char *fileName, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, uint flags = 0, float3 alpha=float3(0.0f,0.0f,0.0f));
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
	virtual void resetVrtl(const uint flags)=0;

inline void reset(const uint flags=0){resetVrtl(flags);}
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
	virtual void EndVrtl(TextureID id)=0;

inline void End(TextureID id=-1){EndVrtl(id);}
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


	//static int WinVer();
protected:
	static unsigned int width;
	static unsigned int height;
	static unsigned int bpp;
#if 0
	std::string mVersion;
	std::string mVendor;
	std::string mRenderer;
	bool mAntialiasing;
	int mMaxTextureSize;
	DWORD mVertexShaderVersion;
	DWORD mPixelShaderVersion;

public:

	void SetVersion(std::string aVersion){mVersion=aVersion;}
	void SetVendor(std::string aVendor){mVendor=aVendor;}
	void SetRenderer(std::string aRenderer){mRenderer=aRenderer;}
	void SetAntialiasing(bool aAntialiasing){mAntialiasing=aAntialiasing;}
	void SetMaxTextureSize(int aMaxTextureSize){mMaxTextureSize=aMaxTextureSize;}
	void SetVertexShaderVersion(DWORD aVertexShaderVersion){mVertexShaderVersion=aVertexShaderVersion;}
	void SetPixelShaderVersion(DWORD aPixelShaderVersion){mPixelShaderVersion=aPixelShaderVersion;}

	std::string GetVersion(){return mVersion;}
	std::string GetVendor(){return mVendor;}
	std::string GetRenderer(){return mRenderer;}
	bool GetAntialiasing(){return mAntialiasing;}
	int GetMaxTextureSize(){return mMaxTextureSize;}
	DWORD GetVertexShaderVersion(){return mVertexShaderVersion;}
	DWORD GetPixelShaderVersion(){return mPixelShaderVersion;}
#endif
public:
	static unsigned int GetBPP(){return bpp;}
	static unsigned int GetWidth(){return width;}
		static unsigned int GetHeight(){return height;}

	static void OnPresent(
		IInput* Input=0
		);

//
#ifndef SWIG
protected:
static IRenderer* s_pRenderer;
#endif
public:
	virtual const char* getString()=0;
static IRenderer* GetRendererInstance(int awidth=800, int aheight=600, const char* atitle="", tRenderer art=eGLSLGL_1_1,
		bool InitLogFile=false,
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
//					    DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
//					    void* pContext
						)=0;
	virtual void	CreateIndexBuffer_Serial(  IndexBufferID& ppBuffer, UINT iSizeBytes,
//					    DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
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
	virtual D3DXFROMWINEVECTOR4& GetscaleBias2D()=0;
#endif
	virtual int GetTextureWidth(TextureID id)=0;
	virtual int GetTextureHeight(TextureID id)=0;

	virtual VOID UpdateViewer(
                                                   D3DXFROMWINEVECTOR3 &pos, float* quat) = 0;

	virtual void viewport(int x, int y, int viewportWidth, int viewportHeight)=0;



	virtual void DrawText(unsigned int x, unsigned int y, const char *pszFormat, ...) = 0;
	virtual void DrawSmallText(unsigned int x, unsigned int y, const char *pszFormat, ...) = 0;

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



virtual void drawTextureVrtl(TextureID texture,  int x,  int y,  int sx,  int sy,  bool InScene,  float tx,  float ty,  float stx,  float sty) = 0;

inline void drawTexture(TextureID texture, int x=0, int y=0, int sx=600, int sy=600, bool InScene=true, float tx=0.0f, float ty=0.0f, float stx=1.0f, float sty=1.0f){drawTextureVrtl(texture, x, y, sx, sy, InScene, tx, ty, stx, sty);}


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
	virtual void drawFPS() = 0;
	virtual bool debugRuntime() = 0;



	virtual void setTexture(const char *textureName, const TextureID texture
//		, const int vtstream=-1
//		, const VertexBufferID vtbuffer=-1
		) = 0;
	virtual void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState
		, const int vtstream=-1
		, const VertexBufferID vtbuffer=-1
		) = 0;
	virtual void setTextureSlice(const char *textureName, const TextureID texture, const int slice) = 0;


		virtual void Init0()=0;
virtual unsigned int getIndicesCount(const Primitives primitives) = 0;

virtual unsigned int getPrimitiveCount(const Primitives primitives, const int count) = 0;

virtual unsigned int getPrimitiveCountR(const Primitives primitives, const int count) = 0;

virtual unsigned int getVertexCount(const Primitives primitives, const int  NumPrim) = 0;


      virtual int getSamplerUnit(const ShaderID shader, const char *samplerName) const = 0;
	virtual int getShaderParameterIndex(const ShaderID shader, const char *samplerName) = 0;

	virtual void RenderTexture(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 texsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f))=0;


	IRenderer()
	{
/*
		mVersion="";
		mVendor="";
		mRenderer="";
		mAntialiasing=false;
		mMaxTextureSize=4096;
		mVertexShaderVersion=3;
		mPixelShaderVersion=3;
*/
	}
	virtual ~IRenderer();

virtual D3DXFROMWINEMATRIX GetMVP()=0;
virtual void setViewportVrtl(int x,  int y,  int viewportWidth,  int viewportHeight,  float minZ,  float maxZ) = 0;

inline void setViewport(int x, int y, int viewportWidth, int viewportHeight, float minZ=0.0f, float maxZ=1.0f){setViewportVrtl(x, y, viewportWidth, viewportHeight, minZ, maxZ);}


	virtual void apply() = 0;

	virtual ShaderID addHLSLShaderVrtl(
		const char *vsText,
		const char *gsText,
		const char *fsText,
		const char *vsName,
		const char *psName,
		const int vsLine,
		const int gsLine,
		const int fsLine,
		const char *header,
		const char *extra,
		const char *fileName,
		const char **attributeNames,
		const int nAttributes,
		const uint flags)=0;

	inline ShaderID addHLSLShader(
		const char *vsText,
		const char *gsText,
		const char *fsText,
		const char *vsName,
		const char *psName,
		const int vsLine,
		const int gsLine,
		const int fsLine,
		const char *header = 0,
		const char *extra = 0,
		const char *fileName = 0,
		const char **attributeNames = 0,
		const int nAttributes = 0,
		const uint flags = 0){return addHLSLShaderVrtl(
		vsText,
		gsText,
		fsText,
		vsName,
		psName,
		vsLine,
		gsLine,
		fsLine,
		header,
		extra,
		fileName,
		attributeNames,
		nAttributes,
		flags);}

	virtual ShaderID addHLSLShaderVrtl(const char *shaderTextp, const char *vsName, const char *psName,  const char **attributeNames, const int nAttributes, const char *extra, const uint flags);
	inline ShaderID addHLSLShader(const char *shaderTextp, const char *vsName, const char *psName,  const char **attributeNames = 0, const int nAttributes = 0, const char *extra = 0, const uint flags = 0)
	{
		return addHLSLShaderVrtl(shaderTextp, vsName, psName,  attributeNames, nAttributes, extra, flags);
	}

	virtual ShaderID addShaderFromFileVrtl(const char *fileName, const char *vsName, const char *psName,  const char **attributeNames, const int nAttributes, const char *extra, const uint flags);

	inline ShaderID addShaderFromFile(const char *fileName, const char *vsName, const char *psName,  const char **attributeNames=0, const int nAttributes=0, const char *extra = 0, const uint flags = 0)
	{
		return addShaderFromFileVrtl(fileName, vsName, psName,  attributeNames, nAttributes, extra, flags);
	}

	virtual int getFormatSize(const AttributeFormat format) const=0;
	virtual VertexFormatID addVertexFormatVrtl(const FormatDesc *formatDesc,  const uint nAttribs,  const ShaderID shader ) = 0;

inline VertexFormatID addVertexFormatV(CONST VOID *formatDesc, int nAttribs, CONST ShaderID shader){return addVertexFormatVrtl((const FormatDesc *)formatDesc, nAttribs, shader);}

inline VertexFormatID addVertexFormat(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader = SHADER_NONE){return addVertexFormatVrtl(formatDesc, nAttribs, shader);}

	virtual VertexBufferID addVertexBufferVrtl(const long size,  const BufferAccess bufferAccess,  const void *data ,  VertexBufferID old)=0;

inline VertexBufferID addVertexBuffer(const long size, const BufferAccess bufferAccess, const void *data = 0, VertexBufferID old=-1){return addVertexBufferVrtl(size, bufferAccess, data, old);}//, const IDataProcessor* p=0) = 0;
	virtual IndexBufferID addIndexBufferVrtl(const uint nIndices,  const uint indexSize,  const BufferAccess bufferAccess,  const void *data ,  IndexBufferID old)=0;

inline IndexBufferID addIndexBuffer(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data = 0, IndexBufferID old=-1){return addIndexBufferVrtl(nIndices, indexSize, bufferAccess, data, old);}//, const IDataProcessor* p=0)  = 0;
		virtual TextureID addTextureVrtl(int Width,  int Height,  bool MipLevels,  FORMAT format,  const SamplerStateID samplerState ,  float3 alpha, const uint flags )=0;

inline TextureID addTexture(int Width, int Height, bool MipLevels, FORMAT format, const SamplerStateID samplerState = SS_NONE, float3 alpha=float3(0.0f,0.0f,0.0f),const uint flags = 0){return addTextureVrtl(Width, Height, MipLevels, format, samplerState, alpha, flags);}



	virtual SamplerStateID addSamplerStateVrtl(const Filter filter,  const AddressMode s,  const AddressMode t,  const AddressMode r,  const float lod ,  const uint maxAniso ,  const int compareFunc ,  const float *border_color ) = 0;

inline SamplerStateID addSamplerState(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod = 0, const uint maxAniso = 16, const int compareFunc = 0, const float *border_color = 0){return addSamplerStateVrtl(filter, s, t, r, lod, maxAniso, compareFunc, border_color);}

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

	virtual void DrawPrimitive(Primitives PrimitiveType,
		UINT StartVertex,
		UINT PrimitiveCount, const uint flags=RESET_ALL)=0;
    virtual void DrawIndexedPrimitive(Primitives PrimitiveType,
		INT BaseVertexIndex,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT startIndex,
		UINT primCount, const uint flags=RESET_ALL)=0;
    virtual void DrawPrimitiveUP(Primitives PrimitiveType,
		UINT PrimitiveCount,
		CONST void* pVertexStreamZeroData0,
		CONST void* pVertexStreamZeroData,
		UINT VertexStreamZeroStride, const uint flags=RESET_ALL)=0;

    virtual void DrawIndexedPrimitiveUP(Primitives PrimitiveType,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT PrimitiveCount,
		CONST void* pIndexData0,
		CONST void* pIndexData,
		/*D3DFORMAT*/int IndexDataFormat,
		CONST void* pVertexStreamZeroData0,
		CONST void* pVertexStreamZeroData,
		UINT VertexStreamZeroStride, const uint flags=RESET_ALL)=0;

    virtual void BeginSceneVrtl(bool aB)=0;

inline void BeginScene(bool aB=false){BeginSceneVrtl(aB);}
    virtual void EndSceneVrtl(bool aB)=0;

inline void EndScene(bool aB=false){EndSceneVrtl(aB);}
	virtual void Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors)=0;
    virtual void PresentVrtl(bool aB)=0;

inline void Present(bool aB=false){PresentVrtl(aB);}






    virtual void drawTexturedVrtl(const Primitives primitives,  TexVertex *vertices,  const uint nVertices,  const TextureID texture,  const BlendStateID blendState,  const DepthStateID depthState,  const D3DXFROMWINEVECTOR4 *color )=0;

inline void drawTextured(const Primitives primitives, TexVertex *vertices, const uint nVertices, const TextureID texture, const BlendStateID blendState = -1, const DepthStateID depthState = -1, const D3DXFROMWINEVECTOR4 *color = 0){drawTexturedVrtl(primitives, vertices, nVertices, texture, blendState, depthState, color);}

inline void drawTexturedV6(CONST Primitives primitives, CONST VOID *vertices, CONST int nVertices, CONST TextureID texture, CONST BlendStateID blendState, CONST DepthStateID depthState){drawTextured(primitives, (TexVertex *)vertices, nVertices, texture, blendState, depthState);}

inline void drawTexturedV7(CONST Primitives primitives, CONST VOID *vertices, CONST int nVertices, CONST TextureID texture, CONST BlendStateID blendState, CONST DepthStateID depthState, CONST D3DXFROMWINEVECTOR4 *color){drawTextured(primitives, (TexVertex *)vertices, nVertices, texture, blendState, depthState, color);}


	virtual void applyTextures() = 0;


	virtual void setSamplerState(const char *samplerName, const SamplerStateID samplerState) = 0;
	virtual void applySamplerStates() = 0;

	virtual void applyConstants() = 0;

	virtual void setShader(const ShaderID shader)=0;

	virtual void setVertexFormat(const VertexFormatID vertexFormat)=0;

	virtual void setVertexBufferVrtl(const int stream,  const VertexBufferID vertexBuffer,  const intptr offset )=0;

inline void setVertexBuffer(const int stream, const VertexBufferID vertexBuffer, const intptr offset = 0){setVertexBufferVrtl(stream, vertexBuffer, offset);}

		virtual void setVertexBuffer(const int stream, const void *base)=0;

	virtual void setIndexBuffer(const IndexBufferID indexBuffer)=0;




	virtual void setBlendStateVrtl(const BlendStateID blendState,  const uint sampleMask )=0;

inline void setBlendState(const BlendStateID blendState, const uint sampleMask = ~0){setBlendStateVrtl(blendState, sampleMask);}

	virtual void setAlphaState(const AlphaStateID alphaState)=0;

	virtual void setDepthStateVrtl(const DepthStateID depthState,  const uint stencilRef )=0;

inline void setDepthState(const DepthStateID depthState, const uint stencilRef = 0){setDepthStateVrtl(depthState, stencilRef);}

	virtual void setRasterizerState(const RasterizerStateID rasterizerState)=0;


	virtual void setShaderConstant1i(const char *name, const int constant)=0;
	virtual void setShaderConstant1f(const char *name, const float constant)=0;


	virtual void setShaderConstant2f(const char *name, const D3DXFROMWINEVECTOR2 &constant)=0;
	virtual void setShaderConstant3f(const char *name, const D3DXFROMWINEVECTOR3 &constant)=0;
	virtual void setShaderConstant4f(const char *name, const D3DXFROMWINEVECTOR4 &constant)=0;
	virtual void setShaderConstant4x4f(const char *name, const D3DXFROMWINEMATRIX &constant)=0;

	virtual void setShaderConstantArray1f(const char *name, const float *constant, const uint count)=0;

	virtual void setShaderConstantArray2f(const char *name, const D3DXFROMWINEVECTOR2  *constant, const uint count)=0;
	virtual void setShaderConstantArray3f(const char *name, const D3DXFROMWINEVECTOR3  *constant, const uint count)=0;
	virtual void setShaderConstantArray4f(const char *name, const D3DXFROMWINEVECTOR4  *constant, const uint count)=0;
	virtual void setShaderConstantArray4x4f(const char *name, const D3DXFROMWINEMATRIX *constant, const uint count)=0;

	virtual void setShaderConstantRaw(const char *name, const void *data, const int size) = 0;

	virtual void changeRenderTargetVrtl(const TextureID colorRT,  const TextureID depthRT ,  const int slice )=0;

inline void changeRenderTarget(const TextureID colorRT, const TextureID depthRT = TEXTURE_NONE, const int slice = NO_SLICE){changeRenderTargetVrtl(colorRT, depthRT, slice);}


	virtual void changeRenderTargets(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int *faces)=0;


	virtual void changeRenderTargetsVrtl(const TextureID *colorRTs,  const uint nRenderTargets,  const TextureID depthRT ,  const int depthSlice ,  const int *slices ) = 0;

inline void changeRenderTargets(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT = TEXTURE_NONE, const int depthSlice = NO_SLICE, const int *slices = 0){changeRenderTargetsVrtl(colorRTs, nRenderTargets, depthRT, depthSlice, slices);}
	virtual void changeToMainFramebuffer() = 0;

	virtual TextureID addTexture(Image3* img, const bool useMipMaps=true, const SamplerStateID samplerState = SS_NONE, uint flags = 0
	, float3 alpha=float3(0.0f,0.0f,0.0f)
	) = 0;

	//static TextureID addTexture(const char *fileName, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, uint flags = 0, float3 alpha=float3(0.0f,0.0f,0.0f));
	//virtual TextureID addCubemapVrtl(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState, const int nArraySlices, uint flags)=0;
	//inline TextureID addCubemap(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, const int nArraySlices = 1, uint flags = 0){addCubemapVrtl(fileNames, useMipMaps, samplerState, nArraySlices, flags);}
	//static TextureID addNormalMap(const char *fileName, const FORMAT destFormat, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, float sZ = 1.0f, float mipMapScaleZ = 2.0f, uint flags = 0);

	virtual TextureID addRenderTargetVrtl(const int width,  const int height,  const FORMAT format,  const SamplerStateID samplerState ,  uint flags )=0;

inline TextureID addRenderTarget(const int width, const int height, const FORMAT format, const SamplerStateID samplerState = SS_NONE, uint flags = 0){return addRenderTargetVrtl(width, height, format, samplerState, flags);}


	virtual TextureID addRenderTargetVrtl(const int width,  const int height,  const int depth,  const int mipMapCount,  const int arraySize,  const FORMAT format,  const int msaaSamples ,  const SamplerStateID samplerState ,  uint flags ) = 0;

inline TextureID addRenderTarget(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, uint flags = 0){return addRenderTargetVrtl(width, height, depth, mipMapCount, arraySize, format, msaaSamples, samplerState, flags);}
	virtual TextureID addRenderDepth(const int width, const int height, const int depthBits)=0;


	virtual TextureID addRenderDepthVrtl(const int width,  const int height,  const int arraySize,  const FORMAT format,  const int msaaSamples ,  const SamplerStateID samplerState ,  uint flags ) = 0;

inline TextureID addRenderDepth(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples = 1, const SamplerStateID samplerState = SS_NONE, uint flags = 0){return addRenderDepthVrtl(width, height, arraySize, format, msaaSamples, samplerState, flags);}

	virtual bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize) = 0;
	virtual bool generateMipMaps(const TextureID renderTarget) = 0;



	virtual void changeShader(const ShaderID shader) = 0;
	virtual void changeVertexFormat(const VertexFormatID vertexFormat) = 0;
	virtual void changeVertexBufferVrtl(const int stream,  const VertexBufferID vertexBuffer,  const intptr offset ) = 0;

inline void changeVertexBuffer(const int stream, const VertexBufferID vertexBuffer, const intptr offset = 0){changeVertexBufferVrtl(stream, vertexBuffer, offset);}
	virtual void changeVertexBuffer(const void *base)=0;

	virtual void changeIndexBuffer(const IndexBufferID indexBuffer) = 0;

//	virtual void changeSamplerState(const uint samplerUnit, const SamplerStateID samplerState) = 0;

	virtual void changeBlendStateVrtl(const BlendStateID blendState,  const uint sampleMask ) = 0;

inline void changeBlendState(const BlendStateID blendState, const uint sampleMask = ~0){changeBlendStateVrtl(blendState, sampleMask);}
	virtual void changeAlphaState(const AlphaStateID alphaState)=0;
	virtual void changeDepthStateVrtl(const DepthStateID depthState,  const uint stencilRef ) = 0;

inline void changeDepthState(const DepthStateID depthState, const uint stencilRef = 0){changeDepthStateVrtl(depthState, stencilRef);}
	virtual void changeRasterizerState(const RasterizerStateID rasterizerState) = 0;

    virtual void DrawFPSVrtl(int x,  int y)=0;

inline void DrawFPS(int x=10, int y=10){DrawFPSVrtl(x, y);}

	virtual void setup2DMode(const float left, const float right, const float top, const float bottom)=0;

    virtual void SaveScreenShotVrtl(const char* fn)=0;

inline void SaveScreenShot(const char* fn=0){SaveScreenShotVrtl(fn);}
	virtual void captureScreenshot(Image3& img)=0;

	virtual float getStringWidth(FontID font,const char* str, const float charWidth)=0;
	virtual void fillTextBuffer(TexVertex *dest, const char *str, float x, float y, const float charWidth, const float charHeight, const FontID font)=0;
	virtual bool drawText(const char *str, float x, float y, const float charWidth, const float charHeight, const FontID& font, const SamplerStateID samplerState = SS_NONE, const BlendStateID blendState = -1, const DepthStateID depthState = -1)=0;
	virtual uint getTextQuads(const char *str) const=0;
	virtual void elevate(float add)=0;
	virtual float GetAspect()=0;
	virtual void drawPlain(const Primitives primitives, D3DXFROMWINEVECTOR2 *vertices, const uint nVertices, const BlendStateID blendState = -1, const DepthStateID depthState = -1, const D3DXFROMWINEVECTOR4 *color=0)=0;
	virtual float getTextWidth(const FontID font, const char *str, int length = -1) const=0;

	virtual void drawLineVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline void drawLine(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 1){drawLineVrtl(x0, y0, x1, y1, color, lineWidth);}
	virtual void drawRectVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline void drawRect(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){drawRectVrtl(x0, y0, x1, y1, color, lineWidth);}
	virtual void drawRoundRectVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const float r,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline void drawRoundRect(const float x0, const float y0, const float x1, const float y1, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){drawRoundRectVrtl(x0, y0, x1, y1, r, color, lineWidth);}
	virtual void drawCircleVrtl(const float x,  const float y,  const float r,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline void drawCircle(const float x, const float y, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){drawCircleVrtl(x, y, r, color, lineWidth);}
	virtual void drawTriangleVrtl(const float x0,  const float y0,  const float x1,  const float y1,  const float x2,  const float y2,  const D3DXFROMWINEVECTOR4 &color,  const float lineWidth )=0;

inline void drawTriangle(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2, const D3DXFROMWINEVECTOR4 &color, const float lineWidth = 0){drawTriangleVrtl(x0, y0, x1, y1, x2, y2, color, lineWidth);}
	virtual void drawQuadVrtl(const float x0, const float y0, const float x1, const float y1, const float s0, const float t0, const float s1, const float t1, const D3DXFROMWINEVECTOR4 &color)=0;
inline void drawQuad(const float x0, const float y0, const float x1, const float y1, const float s0, const float t0, const float s1, const float t1, const D3DXFROMWINEVECTOR4 &color){drawQuadVrtl(x0, y0, x1, y1, s0, t0, s1, t1, color);}
	virtual void drawArrays(const Primitives primitives, const int firstVertex, const int nVertices)=0;
	virtual void drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices)=0;
	virtual void getExtentsVrtl(FontID font,char *string, float x, float y, const float charWidth, const float charHeight, const float skewX = 0.0f, const float skewY = 0.0f,
		//const FONT_HADJUST hAdjust = HA_LEFT,
		//const FONT_VADJUST vAdjust = VA_TOP,
		float *left = 0, float *right = 0, float *top = 0, float *bottom = 0){}

	virtual void InitTex(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlain(ShaderID& s, VertexFormatID& vf)=0;
	virtual void InitPlain3(ShaderID& s, VertexFormatID& vf)=0;

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
    static IShaderTranslator  *Get(tShaderTranslator aShaderTranslator = eHLSL2GLSLHLSL2GLSL);
		virtual Shader* addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header = 0, const char *extra = 0, const char *fileName = 0,
const char **attributeNames = 0, const int nAttributes = 0, const uint flags = 0)=0;

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
	/*
		int samplerCompareGL2(const void *sampler0, const void *sampler1)
		;
		ConstantType getConstantType(GLenum type);
		*/
	virtual Shader* addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header = 0, const char *extra = 0, const char *fileName = 0,
const char **attributeNames = 0, const int nAttributes = 0, const uint flags = 0)=0;
};
class RENDERERAPI HLSL2GLSLHLSL2GLSLTranslator : public HLSL2GLSLTranslator
{
public:
	HLSL2GLSLHLSL2GLSLTranslator();
	virtual ~HLSL2GLSLHLSL2GLSLTranslator();
	virtual Shader* addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header = 0, const char *extra = 0, const char *fileName = 0,
const char **attributeNames = 0, const int nAttributes = 0, const uint flags = 0);
	virtual bool TranslateVertexShader(const std::string& hlsl, const std::string& main, std::string& glsl);
	virtual bool TranslateFragmentShader(const std::string& hlsl, const std::string& main, std::string& glsl);
};

class RENDERERAPI HLSL2GLSLBoostTranslator : public HLSL2GLSLTranslator
{
public:
	HLSL2GLSLBoostTranslator();
	virtual ~HLSL2GLSLBoostTranslator();
	virtual Shader* addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header = 0, const char *extra = 0, const char *fileName = 0,
const char **attributeNames = 0, const int nAttributes = 0, const uint flags = 0);
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
typedef CCamera Camera;
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
	void fillTerrainIB(uint *iDest);
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

#endif
