#if defined(D3D11)
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

#ifndef __Framework3D3D11_H__
#define __Framework3D3D11_H__

// Direct3D11 includes
#include <d3dcommon.h>
#include <dxgi.h>
#ifdef WindowsKits
#include <d3d11_2.h>
#else
#include <d3d11.h>
#endif
#include <d3dcompiler.h>
#ifndef WindowsKits
#include <d3dx11.h>
#endif
#include <d3d11shader.h>
#include <d3dx11effect.h>
//include <xnamath.h>
// XInput includes
#include <xinput.h>

#include <Framework3/IRenderer.h>
#include <Framework3/RendererHLSLCg.h>

#define ROLLING_VB_SIZE (64 * 1024)

/*
#define VB_INVALID (-2)
*/

struct TextureD3D11 : public Texture {
	ID3D11Resource *texture;
	ID3D11Resource *pStaging;
	ID3D11ShaderResourceView *srv;
	ID3D11RenderTargetView   *rtv;
	ID3D11DepthStencilView   *dsv;
	ID3D11ShaderResourceView **srvArray;
	ID3D11RenderTargetView   **rtvArray;
	ID3D11DepthStencilView   **dsvArray;
	DXGI_FORMAT texFormat;
	DXGI_FORMAT srvFormat;
	DXGI_FORMAT rtvFormat;
	DXGI_FORMAT dsvFormat;
	int width, height, depth;
	int arraySize;
	uint flags;

	BYTE* Lock(int m, int& pitch);
	void Unlock(int m);
	union
		{
		        ID3D11Device *dev;
		        ID3D11Device *device;
				ID3D11Device *m_pd3dDevice11;
		};
		union
		{
		ID3D11DeviceContext*    m_pImmediateContext;
		ID3D11DeviceContext*    ic;
	};
};

struct ConstantD3D11 : public Constant {
	ConstantD3D11() : Constant()
	{
		name="";
		vsData=0;
		psData=0;
		vsBuffer=0;
		psBuffer=0;
	}
	ConstantD3D11(const char *aname)
	{
		ConstantD3D11();
		name = aname;
	}
	virtual ~ConstantD3D11()
	{
	}
	ConstantD3D11(const ConstantD3D11& rhs)
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

inline int constantCompD3D11(const void *s0, const void *s1){
        return strcmp(((ConstantD3D11 *) s0)->name.c_str(), ((ConstantD3D11 *) s1)->name.c_str());
}

struct SamplerD3D11 : public Sampler {
	SamplerD3D11() : Sampler()
	{
		name="";
		vsIndex=0;
		psIndex=0;
	}
	SamplerD3D11(const char *aname)
	{
		SamplerD3D11();
		name = aname;
	}
	virtual ~SamplerD3D11()
	{
	}
	SamplerD3D11(const SamplerD3D11& rhs)
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

inline int samplerCompD3D11(const void *s0, const void *s1){
        return strcmp(((SamplerD3D11 *) s0)->name.c_str(), ((SamplerD3D11 *) s1)->name.c_str());
}

struct ShaderD3D11 : public Shader {
	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	//ID3D11GeometryShader *geometryShader;
	ID3DBlob *inputSignature;

	ID3D11Buffer **vsConstants;
	//ID3D11Buffer **gsConstants;
	ID3D11Buffer **psConstants;
	uint nVSCBuffers;
	//uint nGSCBuffers;
	uint nPSCBuffers;

	VArray<ConstantD3D11> constants;
	VArray<SamplerD3D11> textures;
	VArray<SamplerD3D11> samplers;

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

struct VertexFormatD3D11 : public VertexFormat {
	ID3D11InputLayout *inputLayout;
	uint vertexSize[MAX_VERTEXSTREAM];
};

struct VertexBufferD3D11 : public VertexBuffer
{
		//friend RendererHLSLD3D11;
	VertexBufferD3D11(ID3D11Device *adev, ID3D11DeviceContext*    aic):VertexBuffer()
	{
		dev=adev;
		ic=aic;
	}
	virtual ~VertexBufferD3D11(){}
	union
	{
	        ID3D11Device *dev;
	        ID3D11Device *device;
			ID3D11Device *m_pd3dDevice11;
	};
	union
	{
	ID3D11DeviceContext*    m_pImmediateContext;
	ID3D11DeviceContext*    ic;
};
	ID3D11Buffer *vertexBuffer;
	long size;
	BYTE* Lock();
	void Unlock();
};
struct IndexBufferD3D11 : public IndexBuffer
{
		//friend RendererHLSLD3D11;
	IndexBufferD3D11(ID3D11Device *adev, ID3D11DeviceContext*    aic):IndexBuffer()
	{
		dev=adev;
		ic=aic;
	}
	virtual ~IndexBufferD3D11(){}
union
{
        ID3D11Device *dev;
        ID3D11Device *device;
		ID3D11Device *m_pd3dDevice11;
};
union
{
ID3D11DeviceContext*    m_pImmediateContext;
ID3D11DeviceContext*    ic;
};
	ID3D11Buffer *indexBuffer;
	uint nIndices;
	uint indexSize;
	BYTE* Lock();
	void Unlock();

};

struct AlphaStateD3D11 : public AlphaState
{
	AlphaStateD3D11():AlphaState()
	{
		blendEnable=false;
	}
	virtual ~AlphaStateD3D11(){}
	int alphaFunc;
	bool blendEnable;
};
struct SamplerStateD3D11 : public SamplerState {
	ID3D11SamplerState *samplerState;
};

struct BlendStateD3D11 : public BlendState {
	ID3D11BlendState *blendState;
};

struct DepthStateD3D11 : public DepthState {
	ID3D11DepthStencilState *dsState;
};

struct RasterizerStateD3D11 : public RasterizerState {
	ID3D11RasterizerState *rsState;
};
class RendererHLSLD3D11 : public RendererHLSLCg
{
	protected:
		bool mOk;
	public:
		virtual unsigned int GetTextureCount(){return textures.size();}
		virtual unsigned int GetShaderCount(){return shaders.size();}

	virtual Texture* GetTexture(TextureID id)
	{
		if(id==-1)
			return 0;
		if(id>=textures.size())
			return 0;
		return &textures[id];
	}
		virtual const char* getString(){return "D3D11";}
		virtual bool IsOK(){return mOk;}
	virtual void resetToDefaults();
	void setD3Ddefaults();

//	bool resetDevice();

	TextureID addTexture(ID3D11Resource *resource, uint flags = 0);
	TextureID addTexture(Image3 &img, const SamplerStateID samplerState = SS_NONE, uint flags = 0);

	bool resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize);
	bool generateMipMaps(const TextureID renderTarget);

	int removeTexture(const TextureID texture);

	virtual ShaderID addHLSLShaderVrtl(const char *vsText, const char *gsText, const char *fsText, const char *vsName, const char *psName, const int vsLine, const int gsLine, const int fsLine,
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
	//void drawPlain(const Primitives primitives, D3DXFROMWINEVECTOR2 *vertices, const uint nVertices, const BlendStateID blendState, const DepthStateID depthState, const D3DXFROMWINEVECTOR4 *color = 0);
	//void drawTextured(const Primitives primitives, TexVertex *vertices, const uint nVertices, const TextureID texture, const SamplerStateID samplerState, const BlendStateID blendState, const DepthStateID depthState, const D3DXFROMWINEVECTOR4 *color = 0);

	void setFrameBuffer(ID3D11RenderTargetView *colorRTV, ID3D11DepthStencilView *depthDSV){
		backBufferRTV  = colorRTV;
		depthBufferDSV = depthDSV;
	}

	ID3D11Resource *getResource(const TextureID texture) const;

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

			RendererHLSLD3D11(int awidth=800, int aheight=600, bool InitLogFile=false, bool fullScreen=false, int argc=0, char *argv[]=0);
			virtual ~RendererHLSLD3D11();

			bool captureScreenshot(Image3* img);
			virtual void captureScreenshot(Image3& img);
			void SaveScreenShot3(const char* fn);

			float	GetMaxPointSpriteSize(){IMPLEMENT;return -1.0;}
			void	BeginPointSpriteRendering(){IMPLEMENT;}
			void	EndPointSpriteRendering(){IMPLEMENT;}

			AlphaStateID addAlphaState(const int func, const float ref);
			virtual void changeAlphaState(const AlphaStateID alphaState);

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

protected:
	ID3D11ShaderResourceView *createSRV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D11RenderTargetView   *createRTV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);
	ID3D11DepthStencilView   *createDSV(ID3D11Resource *resource, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, const int firstSlice = -1, const int sliceCount = -1);

public:
union
{
        ID3D11Device *dev;
        ID3D11Device *device;
		ID3D11Device *m_pd3dDevice11;
};
union
{
ID3D11DeviceContext*    m_pImmediateContext;
ID3D11DeviceContext*    ic;
};
union
{
        ID3D11RenderTargetView *RTV;
        ID3D11RenderTargetView *backBufferRTV;
};
union
{
        ID3D11DepthStencilView *DSV;
        ID3D11DepthStencilView *depthBufferDSV;
};

IDXGISwapChain*                   m_pSwapChain;// = 0;
D3D_DRIVER_TYPE                   m_driverType;// = D3D11_DRIVER_TYPE_0;

DXGI_FORMAT backBufferFormat;
DXGI_FORMAT depthBufferFormat;
int msaaSamples;
ID3D11Texture2D *m_pBackBuffer;
ID3D11Texture2D* m_pDepthBuffer;

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

	ID3D11Query *eventQuery;public:
	VArray <TextureD3D11> textures;
	VArray <ShaderD3D11> shaders;
	VArray <VertexBufferD3D11> vertexBuffers;
	VArray <IndexBufferD3D11> indexBuffers;
	VArray <VertexFormatD3D11> vertexFormats;
	VArray <SamplerStateD3D11> samplerStates;
	VArray <BlendStateD3D11> blendStates;
	VArray <AlphaStateD3D11> alphaStates;
	VArray <DepthStateD3D11> depthStates;
	VArray <RasterizerStateD3D11> rasterizerStates;
};

#endif // _DIRECT3D11RENDERER_H_
#endif
