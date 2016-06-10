#if defined(D3D10)
/* * * * * * * * * * * * * Author's note * * * * * * * * * * * *\
*   _       _   _       _   _       _   _       _     _ _ _ _   *
*  |_|     |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|  *
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _     *
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_   *
*  |_|     |_| |_|_ _ _|_| |_|     |_| |_|_ _ _|_|  _ _ _ _|_|  *
*  |_|     |_|   |_|_|_|   |_|     |_|   |_|_|_|   |_|_|_|_|    *
*                                                               *
*                     http://www.humus.name                     *
*                                                                *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.             *
*                                                                *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#ifndef __Framework3D3D10_H__
#define __Framework3D3D10_H__

#define D3D_DEBUG_INFO
#include <d3d10_1.h>
#include <d3d10_1shader.h>
#include <d3d10effect.h>

#include <dxgiformat.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#include <Framework3/IRenderer.h>
#include <Framework3/RendererHLSLCg.h>

#define ROLLING_VB_SIZE (64 * 1024)

/*
#define VB_INVALID (-2)
*/

struct TextureD3D10 : public Texture {
	ID3D10Resource *texture;
	ID3D10Resource *pStaging;
	ID3D10ShaderResourceView *srv;
	ID3D10RenderTargetView   *rtv;
	ID3D10DepthStencilView   *dsv;
	ID3D10ShaderResourceView **srvArray;
	ID3D10RenderTargetView   **rtvArray;
	ID3D10DepthStencilView   **dsvArray;
	DXGI_FORMAT texFormat;
	DXGI_FORMAT srvFormat;
	DXGI_FORMAT rtvFormat;
	DXGI_FORMAT dsvFormat;
	int width, height, depth;
	int arraySize;
	uint flags;
		BYTE* Lock(int m, int& pitch)
		{
			BYTE* m_pLockedData=0;
			switch(dim)
			{
	/*		case 1:
			D3D10_MAPPED_TEXTURE1D MappedTex;
			((ID3D10Texture1D*)texture)->Map(m,D3D10_MAP_WRITE,0,&MappedTex);
	        m_pLockedData=MappedTex.pData;
			pitch=MappedTex.RowPitch;
			break;*/
			case 2:
			D3D10_MAPPED_TEXTURE2D MappedTex2D;
			((ID3D10Texture2D*)texture)->Map(m,D3D10_MAP_WRITE_DISCARD,0,&MappedTex2D);
	        m_pLockedData=(BYTE *)MappedTex2D.pData;
			pitch=MappedTex2D.RowPitch;
			break;
			case 3:
			D3D10_MAPPED_TEXTURE3D MappedTex3D;
			((ID3D10Texture3D*)texture)->Map(m,D3D10_MAP_WRITE_DISCARD,0,&MappedTex3D);
	        m_pLockedData=(BYTE *)MappedTex3D.pData;
			pitch=MappedTex3D.RowPitch;
			break;
			};
			return m_pLockedData;
		}
		void Unlock(int m)
		{
			switch(dim)
			{/*
			case 1:
			((ID3D10Texture1D*)texture)->Unmap(m);
	        break;*/
			case 2:
			((ID3D10Texture2D*)texture)->Unmap(m);
	        break;
			case 3:
			((ID3D10Texture3D*)texture)->Unmap(m);
	       break;
			};
	}
};

struct ConstantD3D10 : public Constant {
	ConstantD3D10() : Constant()
	{
		name="";
		vsData=0;
		psData=0;
		vsBuffer=0;
		psBuffer=0;
	}
	ConstantD3D10(const char *aname)
	{
		ConstantD3D10();
		name = aname;
	}
	virtual ~ConstantD3D10()
	{
	}
	ConstantD3D10(const ConstantD3D10& rhs)
	{
		name = rhs.name;
		vsData=rhs.vsData;
		psData=rhs.psData;
		vsBuffer=rhs.vsBuffer;
		psBuffer=rhs.psBuffer;
	}
	std::string name;
	ubyte *vsData;
	//ubyte *gsData;
	ubyte *psData;
	int vsBuffer;
	//int gsBuffer;
	int psBuffer;
};

inline int constantCompD3D10(const void *s0, const void *s1){
        return strcmp(((ConstantD3D10 *) s0)->name.c_str(), ((ConstantD3D10 *) s1)->name.c_str());
}

struct SamplerD3D10 : public Sampler {
	SamplerD3D10() : Sampler()
	{
		name="";
		vsIndex=0;
		psIndex=0;
	}
	SamplerD3D10(const char *aname)
	{
		SamplerD3D10();
		name = aname;
	}
	virtual ~SamplerD3D10()
	{
	}
	SamplerD3D10(const SamplerD3D10& rhs)
	{
		name = rhs.name;
		vsIndex=rhs.vsIndex;
		psIndex=rhs.psIndex;
	}
	std::string name;
	int vsIndex;
	//int gsIndex;
	int psIndex;
};

inline int samplerCompD3D10(const void *s0, const void *s1){
        return strcmp(((SamplerD3D10 *) s0)->name.c_str(), ((SamplerD3D10 *) s1)->name.c_str());
}

struct ShaderD3D10 : public Shader {
	ID3D10VertexShader *vertexShader;
	ID3D10PixelShader *pixelShader;
	//ID3D10GeometryShader *geometryShader;
	ID3D10Blob *inputSignature;

	ID3D10Buffer **vsConstants;
	//ID3D10Buffer **gsConstants;
	ID3D10Buffer **psConstants;
	uint nVSCBuffers;
	//uint nGSCBuffers;
	uint nPSCBuffers;

	VArray<ConstantD3D10> constants;
	VArray<SamplerD3D10> textures;
	VArray<SamplerD3D10> samplers;

	uint nConstants;
	uint nTextures;
	uint nSamplers;

	ubyte **vsConstMem;
	//ubyte **gsConstMem;
	ubyte **psConstMem;

	bool *vsDirty;
	//bool *gsDirty;
	bool *psDirty;
};
struct VertexFormatD3D10 : public VertexFormat {
	ID3D10InputLayout *inputLayout;
	uint vertexSize[MAX_VERTEXSTREAM];
};

struct VertexBufferD3D10 : public VertexBuffer {
	ID3D10Buffer *vertexBuffer;
	long size;
	BYTE* Lock()
	{
		BYTE* m_pLockedData=0;
/*
enum D3D10_MAP
    {	D3D10_MAP_READ	= 1,
	D3D10_MAP_WRITE	= 2,
	D3D10_MAP_READ_WRITE	= 3,
	D3D10_MAP_WRITE_DISCARD	= 4,
	D3D10_MAP_WRITE_NO_OVERWRITE	= 5
    } 	D3D10_MAP;
*/
        vertexBuffer->Map(
			D3D10_MAP_WRITE_DISCARD,
			0,//UINT MapFlags,
			(void **)&m_pLockedData);
		return m_pLockedData;
	}
	void Unlock()
	{
		vertexBuffer->Unmap();
	}
};

struct IndexBufferD3D10 : public IndexBuffer {
	ID3D10Buffer *indexBuffer;
	uint nIndices;
	uint indexSize;
	BYTE* Lock()
	{
		BYTE* m_pLockedData=0;
/*
enum D3D10_MAP
    {	D3D10_MAP_READ	= 1,
	D3D10_MAP_WRITE	= 2,
	D3D10_MAP_READ_WRITE	= 3,
	D3D10_MAP_WRITE_DISCARD	= 4,
	D3D10_MAP_WRITE_NO_OVERWRITE	= 5
    } 	D3D10_MAP;
*/
        indexBuffer->Map(
			D3D10_MAP_WRITE_DISCARD,
			0,//UINT MapFlags,
			(void **)&m_pLockedData);
		return m_pLockedData;
	}
	void Unlock()
	{
		indexBuffer->Unmap();
	}
};

struct AlphaStateD3D10 : public AlphaState
{
	AlphaStateD3D10():AlphaState()
	{
		blendEnable=false;
	}
	virtual ~AlphaStateD3D10(){}
	int alphaFunc;
	bool blendEnable;
};
struct SamplerStateD3D10 : public SamplerState {
	ID3D10SamplerState *samplerState;
};

struct BlendStateD3D10 : public BlendState {
	ID3D10BlendState *blendState;
};

struct DepthStateD3D10 : public DepthState {
	ID3D10DepthStencilState *dsState;
};

struct RasterizerStateD3D10 : public RasterizerState {
	ID3D10RasterizerState *rsState;
};
class RendererHLSLD3D10 : public RendererHLSLCg
{
	protected:
		bool mOk;
	public:
		virtual const char* getString(){return "D3D10";}
		virtual bool IsOK(){return mOk;}

	virtual Texture* GetTexture(TextureID id)
	{
		if(id==-1)
			return 0;
		if(id>=textures.size())
			return 0;
		return &textures[id];
	}
	virtual void resetToDefaults();
	void setD3Ddefaults();

//	bool resetDevice();

	TextureID addTexture(ID3D10Resource *resource, uint flags = 0);
	TextureID addTexture(Image3 &img, const SamplerStateID samplerState = SS_NONE, uint flags = 0);

	bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize);
	bool generateMipMaps(const TextureID renderTarget);

	int removeTexture(const TextureID texture);

	virtual  ShaderID addHLSLShaderVrtl(const char *vsText, const char *gsText, const char *fsText, const char *vsName, const char *psName, const int vsLine, const int gsLine, const int fsLine,
                const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags);

	BlendStateID addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask = ALL, const bool alphaToCoverage = false);
	DepthStateID addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc = LEQUAL, const bool stencilTest = false,
		const uint8 stencilMask = 0xFF, const int stencilFunc = ALWAYS, const int stencilFail = KEEP, const int depthFail = KEEP, const int stencilPass = KEEP);

	void setTextureSlice(const char *textureName, const TextureID texture, const int slice);
	void applyTextures();

	void setSamplerState(const char *samplerName, const SamplerStateID samplerState);
	void applySamplerStates();

	void setShaderConstantRaw(const char *name, const void *data, const int size);
	void applyConstants();

//	void changeTexture(const uint imageUnit, const TextureID textureID);
	virtual void changeToMainFramebuffer();
	void changeShader(const ShaderID shaderID);
	void changeVertexFormat(const VertexFormatID vertexFormatID);
	void changeIndexBuffer(const IndexBufferID indexBufferID);
	void changeCullFace(const int cullFace);

//	void changeSamplerState(const uint samplerUnit, const SamplerStateID samplerState);
	void changeRasterizerState(const RasterizerStateID rasterizerState);


	void clear(const bool clearColor, const bool clearDepth, const bool clearStencil, const float *color, const float depth, const uint stencil);
	void clearRenderTarget(const TextureID renderTarget, const D3DXFROMWINEVECTOR4 &color, const int slice = NO_SLICE);
	void clearDepthTarget(const TextureID depthTarget, const float depth = 1.0f, const int slice = NO_SLICE);

	//void drawArrays(const Primitives primitives, const int firstVertex, const int nVertices);
	//void drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices);

	//void setup2DMode(const float left, const float right, const float top, const float bottom);
	void drawTextured(const Primitives primitives, TexVertex *vertices, const uint nVertices, const TextureID texture, const SamplerStateID samplerState, const BlendStateID blendState, const DepthStateID depthState, const D3DXFROMWINEVECTOR4 *color = 0);

	void setFrameBuffer(ID3D10RenderTargetView *colorRTV, ID3D10DepthStencilView *depthDSV){
		backBufferRTV  = colorRTV;
		depthBufferDSV = depthDSV;
	}

	ID3D10Resource *getResource(const TextureID texture) const;

	void flush();
	void finish();

			virtual void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState
		, const int vtstream=-1
		, const VertexBufferID vtbuffer=-1
		);
			virtual void SaveScreenShotVrtl(const char* fn);
			virtual void BeginSceneVrtl(bool aB){};
			virtual void EndSceneVrtl(bool aB){OnEndScene();}
			virtual void PresentVrtl(bool aB);

			virtual IndexBufferID addIndexBufferVrtl(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID old);//, const IDataProcessor* p);
			virtual RasterizerStateID addRasterizerStateVrtl(const int cullMode, const int fillMode, const bool multiSample, const bool scissor);
			virtual TextureID addRenderTargetVrtl(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags);
			virtual TextureID addRenderDepthVrtl(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags);
			virtual SamplerStateID addSamplerStateVrtl(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod, const uint maxAniso, const int compareFunc, const float *border_color);
			virtual VertexBufferID addVertexBufferVrtl(const long size, const BufferAccess bufferAccess, const void *data, VertexBufferID old);//, const IDataProcessor* p);
			virtual VertexFormatID addVertexFormatVrtl(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader);
			virtual void changeBlendStateVrtl(const BlendStateID blendState, const uint sampleMask);
	        virtual void changeDepthStateVrtl(const DepthStateID depthState, const uint stencilRef);
	        virtual void changeRenderTargetsVrtl(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int depthSlice, const int *slices);
	        virtual void changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBuffer, const intptr offset);
	        virtual BYTE *fillVertexBufferVrtl(const VertexBufferID id, BYTE*  m_pData, int l);
			virtual BYTE *fillIndexBufferVrtl(const IndexBufferID id, BYTE*  m_pData, int l);

	D3DXFROMWINEVECTOR4 scaleBias2D;
	D3DXFROMWINEVECTOR4& GetscaleBias2D(){return scaleBias2D;}

			virtual BlendStateID getBlendStateVrtl(const TextureID texture)
			{
				if(textures[texture].format==FORMAT_RGB8)
					return IRenderer::GetRendererInstance()->GetblendOneOne();
				else
					return IRenderer::GetRendererInstance()->GetblendSrcAlpha();
			}
	        virtual void resetVrtl(const uint flags);
	        virtual int texturePitchVrtl(const TextureID id,int level);
			virtual BYTE *unlockTextureVrtl(const TextureID id,int level);

			virtual void setup2DMode(const float left, const float right, const float top, const float bottom);
			virtual void drawArrays(const Primitives primitives, const int firstVertex, const int nVertices);
			virtual void drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices);

			virtual void Init0(void){}

			RendererHLSLD3D10(int awidth=800, int aheight=600, bool InitLogFile=false, bool fullScreen=false, int argc=0, char *argv[]=0);
			virtual ~RendererHLSLD3D10();

			bool captureScreenshot(Image3* img);
			virtual void captureScreenshot(Image3& img);
			void SaveScreenShot3(const char* fn);

			AlphaStateID addAlphaState(const int func, const float ref);
			virtual void changeAlphaState(const AlphaStateID alphaState);

			float	GetMaxPointSpriteSize(){IMPLEMENT;return -1.0;}
			void	BeginPointSpriteRendering(){IMPLEMENT;}
			void	EndPointSpriteRendering(){IMPLEMENT;}

			virtual void DrawPrimitive(Primitives PrimitiveType, UINT StartVertex, UINT PrimitiveCount, const uint flags);
		    virtual void DrawIndexedPrimitive(Primitives PrimitiveType,
		                INT BaseVertexIndex,
		                UINT MinVertexIndex,
		                UINT NumVertices,
		                UINT startIndex,
		                UINT primCount, const uint flags);

		    virtual void Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors);
		    virtual float getTextWidth(const FontID font, const char *str, int length) const;

			virtual unsigned int GetLevelCount(TextureID id);
			virtual void GetLevelDesc(TextureID id, unsigned int level,SURFACE_DESC* sDesc);
			virtual void GetDesc(VertexBufferID id,VERTEXBUFFER_DESC* sDesc);
			virtual void GetDesc(IndexBufferID id, INDEXBUFFER_DESC* sDesc);

	    	virtual BYTE* LockTexture(TextureID id, int m, int& pitch);
	    	virtual void UnlockTexture(TextureID id, int m);
	    	virtual BYTE* LockVertexBuffer(VertexBufferID id);
	    	virtual void UnLockVertexBuffer(VertexBufferID id);
	    	virtual BYTE* LockIndexBuffer(IndexBufferID id);
	    	virtual void UnLockIndexBuffer(IndexBufferID id);

			virtual int GetTextureWidth(TextureID id);
			virtual int GetTextureHeight(TextureID id);
			virtual UINT32 GetAvailableTextureMem();
			virtual int deleteVertexBuffer(const VertexBufferID vertexBuffer);
			virtual int deleteIndexBuffer(const IndexBufferID indexBuffer);

			virtual BYTE* lockTexture(const TextureID texture, int level, int& pitch);
			virtual BYTE* lockVertexBuffer(const VertexBufferID vertexBuffer);
			virtual BYTE* lockIndexBuffer(const IndexBufferID indexBuffer);

			virtual BYTE* unlockVertexBuffer(const VertexBufferID vertexBuffer);
			virtual BYTE* unlockIndexBuffer(const IndexBufferID indexBuffer);

virtual void buildPerspectiveMatrix(float fieldOfView,
                                   float aspectRatio,
                                   float zNear, float zFar);//,D3DXFROMWINEMATRIX& m)

        virtual void buildLookAtMatrix(float eyex, float eyey, float eyez,
                              float centerx, float centery, float centerz,
                              float upx, float upy, float upz);//,D3DXFROMWINEMATRIX& m)


			virtual int getSamplerUnit(const ShaderID shader, const char *samplerName) const;

			virtual void setTexture(const char *textureName, const TextureID texture);
			//virtual void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState, const int vtstream=-1, const VertexBufferID vtbuffer=-1);
			virtual void setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState);

			virtual TextureID addTexture(Image3* img, const bool useMipMaps=false, const SamplerStateID samplerState = SS_NONE, uint flags = 0, float3 alpha=float3(0.0f,0.0f,0.0f));
			virtual void viewport(int x, int y, int viewportWidth, int viewportHeight);
			virtual unsigned int GetTextureCount(){return textures.size();}
			virtual unsigned int GetShaderCount(){return shaders.size();}

protected:
	ID3D10ShaderResourceView *createSRV(ID3D10Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D10RenderTargetView   *createRTV(ID3D10Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D10DepthStencilView   *createDSV(ID3D10Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);

public:
#if 0//def USE_D3D10_1
union
{
        ID3D10Device1 *dev;
        ID3D10Device1 *device;
        ID3D10Device1 *m_pd3dDevice10;
};
#else
union
{
        ID3D10Device *dev;
        ID3D10Device *device;
	ID3D10Device *m_pd3dDevice10;
};
#endif
union
{
        ID3D10RenderTargetView *RTV;
        ID3D10RenderTargetView *backBufferRTV;
};
union
{
        ID3D10DepthStencilView *DSV;
        ID3D10DepthStencilView *depthBufferDSV;
};
		IDXGISwapChain*                   m_pSwapChain;
		D3D10_DRIVER_TYPE                   m_driverType;

		DXGI_FORMAT backBufferFormat;
		DXGI_FORMAT depthBufferFormat;
		int msaaSamples;
		ID3D10Texture2D *m_pBackBuffer;
		ID3D10Texture2D* m_pDepthBuffer;

protected:
	TextureID currentTexturesVS[MAX_TEXTUREUNIT], selectedTexturesVS[MAX_TEXTUREUNIT];
	TextureID currentTexturesGS[MAX_TEXTUREUNIT], selectedTexturesGS[MAX_TEXTUREUNIT];
	TextureID currentTexturesPS[MAX_TEXTUREUNIT], selectedTexturesPS[MAX_TEXTUREUNIT];
	int currentTextureSlicesVS[MAX_TEXTUREUNIT], selectedTextureSlicesVS[MAX_TEXTUREUNIT];
	int currentTextureSlicesGS[MAX_TEXTUREUNIT], selectedTextureSlicesGS[MAX_TEXTUREUNIT];
	int currentTextureSlicesPS[MAX_TEXTUREUNIT], selectedTextureSlicesPS[MAX_TEXTUREUNIT];

	SamplerStateID currentSamplerStatesVS[MAX_SAMPLERSTATE], selectedSamplerStatesVS[MAX_SAMPLERSTATE];
	SamplerStateID currentSamplerStatesGS[MAX_SAMPLERSTATE], selectedSamplerStatesGS[MAX_SAMPLERSTATE];
	SamplerStateID currentSamplerStatesPS[MAX_SAMPLERSTATE], selectedSamplerStatesPS[MAX_SAMPLERSTATE];

private:
	ubyte *mapRollingVB(const uint size);
	void unmapRollingVB(const uint size);
	uint copyToRollingVB(const void *src, const uint size);

	VertexBufferID rollingVB;
	int rollingVBOffset;

	ShaderID plainShader, texShader;
	VertexFormatID plainVF, texVF;

	D3DXFROMWINEVECTOR4 scaleBias2D;

	ID3D10Query *eventQuery;
public:
	VArray <TextureD3D10> textures;
	VArray <ShaderD3D10> shaders;
	VArray <VertexBufferD3D10> vertexBuffers;
	VArray <IndexBufferD3D10> indexBuffers;
	VArray <VertexFormatD3D10> vertexFormats;
	VArray <SamplerStateD3D10> samplerStates;
	VArray <BlendStateD3D10> blendStates;
	VArray <AlphaStateD3D10> alphaStates;
	VArray <DepthStateD3D10> depthStates;
	VArray <RasterizerStateD3D10> rasterizerStates;
};

#endif // _DIRECT3D10RENDERER_H_
#endif

