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

#include <Framework3/RendererHLSLD3D11.h>
#ifdef __Framework3D3D11_H__



BYTE* TextureD3D11::Lock(int m, int& pitch)
	{
		D3D11_MAPPED_SUBRESOURCE MappedTex;
		BYTE* m_pLockedData=0;
		switch(dim)
		{
		case 1:
		ic->Map(((ID3D11Texture1D*)texture),m,D3D11_MAP_WRITE,0,&MappedTex);
        m_pLockedData=(BYTE *)MappedTex.pData;
		pitch=MappedTex.RowPitch;
		break;
		case 2:
		ic->Map(((ID3D11Texture2D*)texture),m,D3D11_MAP_WRITE,0,&MappedTex);
        m_pLockedData=(BYTE *)MappedTex.pData;
		pitch=MappedTex.RowPitch;
		break;
		case 3:
		ic->Map(((ID3D11Texture3D*)texture),m,D3D11_MAP_WRITE,0,&MappedTex);
        m_pLockedData=(BYTE *)MappedTex.pData;
		pitch=MappedTex.RowPitch;
		break;
		};
		return m_pLockedData;
	}
	void TextureD3D11::Unlock(int m)
	{
		switch(dim)
		{
		case 1:
		ic->Unmap(((ID3D11Texture1D*)texture),m);
		break;
		case 2:
		ic->Unmap(((ID3D11Texture2D*)texture),m);
		break;
		case 3:
		ic->Unmap(((ID3D11Texture3D*)texture),m);
		break;
		};
	}

	BYTE* VertexBufferD3D11::Lock()
	{
		D3D11_MAPPED_SUBRESOURCE m_pLockedData;
/*
enum D3D11_MAP
    {	D3D11_MAP_READ	= 1,
	D3D11_MAP_WRITE	= 2,
	D3D11_MAP_READ_WRITE	= 3,
	D3D11_MAP_WRITE_DISCARD	= 4,
	D3D11_MAP_WRITE_NO_OVERWRITE	= 5
    } 	D3D11_MAP;
*/
        ic->Map(vertexBuffer,0,
			D3D11_MAP_WRITE,
			0,//UINT MapFlags,
			&m_pLockedData);
		return (BYTE *)m_pLockedData.pData;
	}
	void VertexBufferD3D11::Unlock()
	{
		ic->Unmap(vertexBuffer,0);
	}


	BYTE* IndexBufferD3D11::Lock()
	{
		D3D11_MAPPED_SUBRESOURCE m_pLockedData;
/*
enum D3D11_MAP
    {	D3D11_MAP_READ	= 1,
	D3D11_MAP_WRITE	= 2,
	D3D11_MAP_READ_WRITE	= 3,
	D3D11_MAP_WRITE_DISCARD	= 4,
	D3D11_MAP_WRITE_NO_OVERWRITE	= 5
    } 	D3D11_MAP;
*/
        ic->Map(indexBuffer,0,
			D3D11_MAP_WRITE,
			0,//UINT MapFlags,
			&m_pLockedData);
		return (BYTE *)m_pLockedData.pData;
	}
	void IndexBufferD3D11::Unlock()
	{
		ic->Unmap(indexBuffer,0);
	}


static DXGI_FORMAT formatsd3d11[] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R8_SNORM,
	DXGI_FORMAT_R8G8_SNORM,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R16_SNORM,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
		DXGI_FORMAT_UNKNOWN,//DXGI_FORMAT_R11G11B11_FLOAT,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,//DXGI_FORMAT_R11G11B11A2_UNORM,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC5_UNORM,
}
;

/*
'SimpleTexture.exe': Loaded 'C:\Windows\SysWOW64\nvapi.dll'
The thread 'Win32 Thread' (0xaf4) has exited with code 0 (0x0).
D3D11CreateDevice: Flags (0x2) were specified which require the D3D11 SDK Layers for Windows 10, but they are not present on the system.
These flags must be removed, or the Windows 10 SDK must be installed.
Flags include: D3D11_CREATE_DEVICE_DEBUG
First-chance exception at 0x76fd3e28 in SimpleTexture.exe: Microsoft C++ exception: _com_error at memory location 0x0019f32c..
'SimpleTexture.exe': Unloaded 'C:\Windows\SysWOW64\nvwgf2um.dll'
*/

RendererHLSLD3D11::RendererHLSLD3D11(int awidth, int aheight,
		bool InitLogFile,
			bool fullScreen,
int argc, char *argv[]
) : mOk(false), RendererHLSLCg(awidth, aheight,InitLogFile,argc, argv),m_pd3dDevice11(0)
{
		HRESULT hr = E_FAIL;
#if 0
		ID3D11Device *d3ddev=0;
		UINT createDeviceFlags = 0;
		#ifdef _DEBUG
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
		}
		;
		UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		sd.OutputWindow = STX_Service::GetWindowInstance()->GetHWND();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		UINT FeatureLevels=6;
		CONST D3D_FEATURE_LEVEL pFeatureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		}
		;
		D3D_FEATURE_LEVEL fl112=fl11;
		UINT driverTypeIndex = 0;
		for( driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(
			0,
			m_driverType,
			0,
			createDeviceFlags,
			pFeatureLevels,
			FeatureLevels,
			D3D11_SDK_VERSION,
			&sd,
			&m_pSwapChain,
			&d3ddev,
			&fl112,
			&ic
			);
			if( SUCCEEDED( hr ) )
			break;
		}
#else
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = STX_Service::GetWindowInstance()->GetHWND();// the window to be used
    scd.SampleDesc.Count = 1;                               // how many multisamples
    scd.Windowed = TRUE;
	UINT createDeviceFlags = 0;
		#ifdef _DEBUG
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
		}
		;
		UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );
		UINT FeatureLevels=6;
		CONST D3D_FEATURE_LEVEL pFeatureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		}
		;
		D3D_FEATURE_LEVEL fl112=fl11;
		//UINT driverTypeIndex = 0;
		//for( driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			//m_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  createDeviceFlags,
			pFeatureLevels,
			FeatureLevels,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &m_pSwapChain,
                                  &device,
                                  &fl112,
                                  &ic);
			//if( SUCCEEDED( hr ) )
			//break;
		}
#endif

		if( FAILED( hr ) )
		return;

		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&m_pBackBuffer );
		if( FAILED( hr ) )
		{
			LOG_PRINT("g_pSwapChain->GetBuffer failed\n");
			return;
		}
		hr = dev->CreateRenderTargetView( m_pBackBuffer, 0, &RTV );
		if( FAILED( hr ) )
		{
			LOG_PRINT("Creation of rendertarget failed\n");
			return;
		}
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = dev->CreateTexture2D( &descDepth, 0, &m_pDepthBuffer );
		if( FAILED( hr ) )
		{
			LOG_PRINT("Creation of Depthstencil failed\n");
			return;
		}
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		descDSV.Flags=0;
		hr = dev->CreateDepthStencilView( m_pDepthBuffer, &descDSV, &DSV );
		if( FAILED( hr ) )
		{
			LOG_PRINT("Creation of DepthstencilView failed\n");
			return;
		}
		ic->OMSetRenderTargets( 1, &RTV, DSV );
		D3D11_VIEWPORT vp;
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		ic->RSSetViewports( 1, &vp );
		eventQuery = 0;
		nImageUnits = 16;
		maxAnisotropic = 16;
		nMRTs = 8;
		if (nMRTs > MAX_MRTS) nMRTs = MAX_MRTS;
		plainShader = SHADER_NONE;
		plainVF = VF_NONE;
		rollingVB = VB_NONE;
		rollingVBOffset = 0;
		ZERO		    = D3D11_BLEND_ZERO;
		ONE		    = D3D11_BLEND_ONE;
		SRC_COLOR	    = D3D11_BLEND_SRC_COLOR;
		ONE_MINUS_SRC_COLOR = D3D11_BLEND_INV_SRC_COLOR;
		DST_COLOR	    = D3D11_BLEND_DEST_COLOR;
		ONE_MINUS_DST_COLOR = D3D11_BLEND_INV_DEST_COLOR;
		SRC_ALPHA	    = D3D11_BLEND_SRC_ALPHA;
		ONE_MINUS_SRC_ALPHA = D3D11_BLEND_INV_SRC_ALPHA;
		DST_ALPHA	    = D3D11_BLEND_DEST_ALPHA;
		ONE_MINUS_DST_ALPHA = D3D11_BLEND_INV_DEST_ALPHA;
		SRC_ALPHA_SATURATE  = D3D11_BLEND_SRC_ALPHA_SAT;
		BM_ADD		    = D3D11_BLEND_OP_ADD;
		BM_SUBTRACT	    = D3D11_BLEND_OP_SUBTRACT;
		BM_REVERSE_SUBTRACT = D3D11_BLEND_OP_REV_SUBTRACT;
		BM_MIN		    = D3D11_BLEND_OP_MIN;
		BM_MAX		    = D3D11_BLEND_OP_MAX;
		NEVER	 = D3D11_COMPARISON_NEVER;
		LESS	 = D3D11_COMPARISON_LESS;
		EQUAL	 = D3D11_COMPARISON_EQUAL;
		LEQUAL	 = D3D11_COMPARISON_LESS_EQUAL;
		GREATER  = D3D11_COMPARISON_GREATER;
		NOTEQUAL = D3D11_COMPARISON_NOT_EQUAL;
		GEQUAL	 = D3D11_COMPARISON_GREATER_EQUAL;
		ALWAYS	 = D3D11_COMPARISON_ALWAYS;
		KEEP	 = D3D11_STENCIL_OP_KEEP;
		SET_ZERO = D3D11_STENCIL_OP_ZERO;
		REPLACE  = D3D11_STENCIL_OP_REPLACE;
		INVERT	 = D3D11_STENCIL_OP_INVERT;
		INCR	 = D3D11_STENCIL_OP_INCR;
		DECR	 = D3D11_STENCIL_OP_DECR;
		INCR_SAT = D3D11_STENCIL_OP_INCR_SAT;
		DECR_SAT = D3D11_STENCIL_OP_DECR_SAT;
		CULL_NONE  = D3D11_CULL_NONE;
		CULL_BACK  = D3D11_CULL_BACK;
		CULL_FRONT = D3D11_CULL_FRONT;
		SOLID = D3D11_FILL_SOLID;
		WIREFRAME = D3D11_FILL_WIREFRAME;

		setD3Ddefaults();
		resetToDefaults();

		for (uint i = 0; i < MAX_TEXTUREUNIT; i++){
			selectedTexturesVS[i] = TEXTURE_NONE;
			selectedTexturesGS[i] = TEXTURE_NONE;
			selectedTexturesPS[i] = TEXTURE_NONE;
			selectedTextureSlicesVS[i] = NO_SLICE;
			selectedTextureSlicesGS[i] = NO_SLICE;
			selectedTextureSlicesPS[i] = NO_SLICE;
		}

		for (uint i = 0; i < MAX_SAMPLERSTATE; i++){
			selectedSamplerStatesVS[i] = SS_NONE;
			selectedSamplerStatesGS[i] = SS_NONE;
			selectedSamplerStatesPS[i] = SS_NONE;
		}

				//defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);
		/*
		SetVersion("");
		SetVendor("");
		SetRenderer("");
		SetAntialiasing(false);
		SetMaxTextureSize(4096);
		SetVertexShaderVersion(3);
		SetPixelShaderVersion(3);
*/
		LOG_PRINT("RendererHLSLD3D11\n");
		if(!FAILED(hr)) mOk=true;
	}

RendererHLSLD3D11::~RendererHLSLD3D11(){
	ic->ClearState();

	if (eventQuery) eventQuery->Release();


/*
	releaseFrameBufferSurfaces();
*/

	// Delete shaders
	for (uint i = 0; i < shaders.getCount(); i++){
		if (shaders[i].vertexShader  ) shaders[i].vertexShader->Release();
		//if (shaders[i].geometryShader) shaders[i].geometryShader->Release();
		if (shaders[i].pixelShader   ) shaders[i].pixelShader->Release();
		if (shaders[i].inputSignature) shaders[i].inputSignature->Release();

		for (uint k = 0; k < shaders[i].nVSCBuffers; k++){
			shaders[i].vsConstants[k]->Release();
			delete shaders[i].vsConstMem[k];
		}
		//for (uint k = 0; k < shaders[i].nGSCBuffers; k++){
		//	shaders[i].gsConstants[k]->Release();
		//	delete shaders[i].gsConstMem[k];
		//}
		for (uint k = 0; k < shaders[i].nPSCBuffers; k++){
			shaders[i].psConstants[k]->Release();
			delete shaders[i].psConstMem[k];
		}
		delete shaders[i].vsConstants;
		//delete shaders[i].gsConstants;
		delete shaders[i].psConstants;
		delete shaders[i].vsConstMem;
		//delete shaders[i].gsConstMem;
		delete shaders[i].psConstMem;

		/*
		for (uint k = 0; k < shaders[i].nConstants; k++){
			delete shaders[i].constants[k].name;
		}
		*/
		//delete shaders[i].constants;

		/*
		for (uint k = 0; k < shaders[i].nTextures; k++){
			delete shaders[i].textures[k].name;
		}
		*/
		//delete shaders[i].textures;

		/*
		for (uint k = 0; k < shaders[i].nSamplers; k++){
			delete shaders[i].samplers[k].name;
		}
		*/
		//delete shaders[i].samplers;

		delete shaders[i].vsDirty;
		//delete shaders[i].gsDirty;
		delete shaders[i].psDirty;
	}

    // Delete vertex formats
	for (uint i = 0; i < vertexFormats.getCount(); i++){
		if (vertexFormats[i].inputLayout) vertexFormats[i].inputLayout->Release();
	}

    // Delete vertex buffers
	for (uint i = 0; i < vertexBuffers.getCount(); i++){
		if (vertexBuffers[i].vertexBuffer) vertexBuffers[i].vertexBuffer->Release();
	}

	// Delete index buffers
	for (uint i = 0; i < indexBuffers.getCount(); i++){
		if (indexBuffers[i].indexBuffer) indexBuffers[i].indexBuffer->Release();
	}

	// Delete samplerstates
	for (uint i = 0; i < samplerStates.getCount(); i++){
		if (samplerStates[i].samplerState) samplerStates[i].samplerState->Release();
	}

	// Delete blendstates
	for (uint i = 0; i < blendStates.getCount(); i++){
		if (blendStates[i].blendState) blendStates[i].blendState->Release();
	}

	// Delete depthstates
	for (uint i = 0; i < depthStates.getCount(); i++){
		if (depthStates[i].dsState) depthStates[i].dsState->Release();
	}

	// Delete rasterizerstates
	for (uint i = 0; i < rasterizerStates.getCount(); i++){
		if (rasterizerStates[i].rsState) rasterizerStates[i].rsState->Release();
	}

	// Delete textures
	for (uint i = 0; i < textures.getCount(); i++){
		removeTexture(i);
	}

//	if (rollingVB) rollingVB->Release();
}

void RendererHLSLD3D11::resetVrtl(const uint flags){
	reset0(flags);
	if (flags & RESET_TEX){
		for (uint i = 0; i < MAX_TEXTUREUNIT; i++){
			selectedTexturesVS[i] = TEXTURE_NONE;
			selectedTexturesGS[i] = TEXTURE_NONE;
			selectedTexturesPS[i] = TEXTURE_NONE;
			selectedTextureSlicesVS[i] = NO_SLICE;
			selectedTextureSlicesGS[i] = NO_SLICE;
			selectedTextureSlicesPS[i] = NO_SLICE;
		}
	}

	if (flags & RESET_SS){
		for (uint i = 0; i < MAX_SAMPLERSTATE; i++){
			selectedSamplerStatesVS[i] = SS_NONE;
			selectedSamplerStatesGS[i] = SS_NONE;
			selectedSamplerStatesPS[i] = SS_NONE;
		}
	}
}

void RendererHLSLD3D11::resetToDefaults(){
	reset0ToDefaults();
	for (uint i = 0; i < MAX_TEXTUREUNIT; i++){
		currentTexturesVS[i] = TEXTURE_NONE;
		currentTexturesGS[i] = TEXTURE_NONE;
		currentTexturesPS[i] = TEXTURE_NONE;
		currentTextureSlicesVS[i] = NO_SLICE;
		currentTextureSlicesGS[i] = NO_SLICE;
		currentTextureSlicesPS[i] = NO_SLICE;
	}

	for (uint i = 0; i < MAX_SAMPLERSTATE; i++){
		currentSamplerStatesVS[i] = SS_NONE;
		currentSamplerStatesGS[i] = SS_NONE;
		currentSamplerStatesPS[i] = SS_NONE;
	}

	// TODO: Fix ...
	currentRasterizerState = -2;


/*
	currentDepthRT = FB_DEPTH;
*/
}

void RendererHLSLD3D11::setD3Ddefaults(){
/*
	// Set some of my preferred defaults
	dev->SetRenderState(D3DRS_LIGHTING, FALSE);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	if (maxAnisotropic > 1){
		for (uint i = 0; i < nImageUnits; i++){
			dev->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, maxAnisotropic);
		}
	}
*/
}

/*bool createRenderTarget(LPDIRECT3DDEVICE9 dev, Texture &tex){
	if (isDepthFormat(tex.format)){
		if (tex.surfaces == 0) tex.surfaces = new LPDIRECT3DSURFACE9;
		if (dev->CreateDepthStencilSurface(tex.width, tex.height, formats[tex.format], D3DMULTISAMPLE_NONE, 0, FALSE, tex.surfaces, 0) != D3D_OK){
			delete tex.surfaces;

			LOG_PRINT("Couldn't create depth surface");
			return false;
		}
	} else {
		bool mipMapped = (tex.filter.mipFilter != D3DTEXF_NONE);

		if (tex.flags & CUBEMAP){
			if (dev->CreateCubeTexture(tex.width, mipMapped? 0 : 1, tex.usage, formats[tex.format], D3DPOOL_DEFAULT, (LPDIRECT3DCUBETEXTURE9 *) &tex.texture, 0) != D3D_OK){
				LOG_PRINT("Couldn't create render target");
				return false;
			}

			if (tex.surfaces == 0) tex.surfaces = new LPDIRECT3DSURFACE9[6];
			for (uint i = 0; i < 6; i++){
				((LPDIRECT3DCUBETEXTURE9) tex.texture)->GetCubeMapSurface((D3DCUBEMAP_FACES) i, 0, &tex.surfaces[i]);
			}
		} else {
			if (dev->CreateTexture(tex.width, tex.height, mipMapped? 0 : 1, tex.usage, formats[tex.format], D3DPOOL_DEFAULT, (LPDIRECT3DTEXTURE9 *) &tex.texture, 0) != D3D_OK){
				LOG_PRINT("Couldn't create render target");
				return false;
			}
			if (tex.surfaces == 0) tex.surfaces = new LPDIRECT3DSURFACE9;
			((LPDIRECT3DTEXTURE9) tex.texture)->GetSurfaceLevel(0, tex.surfaces);
		}
	}

	return true;
}
*/

/*bool RendererHLSLD3D11::resetDevice(){
	for (uint i = 0; i < textures.getCount(); i++){
		if (textures[i].surfaces){
			int n = (textures[i].flags & CUBEMAP)? 6 : 1;

			if (textures[i].texture) textures[i].texture->Release();
			for (int k = 0; k < n; k++){
				textures[i].surfaces[k]->Release();
			}
		}
	}

	if (!releaseFrameBufferSurfaces()) return false;

	if (dev->Reset(&d3dpp) != D3D_OK){
		LOG_PRINT("Device reset failed");
		return false;
	}

	if (!createFrameBufferSurfaces()) return false;

	resetToDefaults();
	setD3Ddefaults();

	for (uint i = 0; i < textures.getCount(); i++){
		if (textures[i].surfaces){
			createRenderTarget(dev, textures[i]);
		}
	}

	return true;
}*/

TextureID RendererHLSLD3D11::addTexture(ID3D11Resource *resource, uint flags){
	TextureD3D11 tex;
	memset(&tex, 0, sizeof(tex));

	tex.texture = resource;
	tex.srv = createSRV(resource);
	tex.flags = flags;

	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);

	switch (type){
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			D3D11_TEXTURE1D_DESC desc1d;
			((ID3D11Texture1D *) resource)->GetDesc(&desc1d);

			tex.width  = desc1d.Width;
			tex.height = 1;
			tex.depth  = 1;
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			D3D11_TEXTURE2D_DESC desc2d;
			((ID3D11Texture2D *) resource)->GetDesc(&desc2d);

			tex.width  = desc2d.Width;
			tex.height = desc2d.Height;
			tex.depth  = 1;
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			D3D11_TEXTURE3D_DESC desc3d;
			((ID3D11Texture3D *) resource)->GetDesc(&desc3d);

			tex.width  = desc3d.Width;
			tex.height = desc3d.Height;
			tex.depth  = desc3d.Depth;
			break;
	}

	return textures.add(tex);
}


TextureID RendererHLSLD3D11::addTexture(Image3 &img, const SamplerStateID samplerState, uint flags){
	TextureD3D11 tex;
	memset(&tex, 0, sizeof(tex));

	switch (img.getFormat()){
		case FORMAT_RGB8:
			img.convert(FORMAT_RGBA8);
//			img.convert(FORMAT_RGB11A2);
			break;
		case FORMAT_RGB16:
			img.convert(FORMAT_RGBA16);
			break;
		case FORMAT_RGB16F:
			img.convert(FORMAT_RGBA16F);
			break;
		case FORMAT_RGB32F:
			img.convert(FORMAT_RGBA32F);
			break;
	}

	FORMAT format = img.getFormat();
	uint nMipMaps = img.getMipMapCount();
	uint nSlices = img.isCube()? 6 : 1;
	uint arraySize = img.getArraySize();

	static D3D11_SUBRESOURCE_DATA texData[1124];
	D3D11_SUBRESOURCE_DATA *dest = texData;
	//for (uint n = 0; n < arraySize; n++){
		for (uint k = 0; k < nSlices; k++){
			for (uint i = 0; i < nMipMaps; i++){
				uint pitch, slicePitch;
				if (isCompressedFormat(format)){
					pitch = ((img.getWidth(i) + 3) >> 2) * getBytesPerBlock(format);
					slicePitch = pitch * ((img.getHeight(i) + 3) >> 2);
				} else {
					pitch = img.getWidth(i) * getBytesPerPixel(format);
					slicePitch = pitch * img.getHeight(i);
				}

				dest->pSysMem = img.getPixels(i/*, n*/) + k * slicePitch;
				dest->SysMemPitch = pitch;
				dest->SysMemSlicePitch = slicePitch;
				dest++;
			}
		}
	//}

	tex.texFormat = formatsd3d11[format];
	if (flags & SRGB){
		// Change to the matching sRGB format
		switch (tex.texFormat){
			case DXGI_FORMAT_R8G8B8A8_UNORM: tex.texFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; break;
			case DXGI_FORMAT_BC1_UNORM: tex.texFormat = DXGI_FORMAT_BC1_UNORM_SRGB; break;
			case DXGI_FORMAT_BC2_UNORM: tex.texFormat = DXGI_FORMAT_BC2_UNORM_SRGB; break;
			case DXGI_FORMAT_BC3_UNORM: tex.texFormat = DXGI_FORMAT_BC3_UNORM_SRGB; break;
		}
	}

	HRESULT hr;
	if (img.is1D()){
		D3D11_TEXTURE1D_DESC desc;
		desc.Width  = img.getWidth();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.MiscFlags = 0;

		hr = device->CreateTexture1D(&desc, texData, (ID3D11Texture1D **) &tex.texture);
	} else if (img.is2D() || img.isCube()){
		D3D11_TEXTURE2D_DESC desc;
		desc.Width  = img.getWidth();
		desc.Height = img.getHeight();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		if (img.isCube()){
			desc.ArraySize = 6 * arraySize;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		} else {
			desc.ArraySize = 1;
			desc.MiscFlags = 0;
		}

		hr = device->CreateTexture2D(&desc, texData, (ID3D11Texture2D **) &tex.texture);
	} else if (img.is3D()){
		D3D11_TEXTURE3D_DESC desc;
		desc.Width  = img.getWidth();
		desc.Height = img.getHeight();
		desc.Depth  = img.getDepth();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = device->CreateTexture3D(&desc, texData, (ID3D11Texture3D **) &tex.texture);
	}

	if (FAILED(hr)){
		LOG_PRINT("Couldn't create texture");
		return TEXTURE_NONE;
	}

	tex.srvFormat = tex.texFormat;
	tex.srv = createSRV(tex.texture, tex.srvFormat);

	return textures.add(tex);
}

TextureID RendererHLSLD3D11::addRenderTargetVrtl(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags){
	TextureD3D11 tex;
	memset(&tex, 0, sizeof(tex));

	tex.width  = width;
	tex.height = height;
	tex.depth  = depth;
	tex.arraySize = arraySize;
	tex.flags  = flags;
	tex.texFormat = formatsd3d11[format];

	if (depth == 1){
		D3D11_TEXTURE2D_DESC desc;
		desc.Width  = width;
		desc.Height = height;
		desc.Format = tex.texFormat;
		desc.MipLevels = mipMapCount;
		desc.SampleDesc.Count = msaaSamples;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		if (flags & CUBEMAP){
			desc.ArraySize = 6;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		} else {
			desc.ArraySize = arraySize;
			desc.MiscFlags = 0;
		}
		if (flags & USE_MIPGEN){
			desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		if (FAILED(device->CreateTexture2D(&desc, 0, (ID3D11Texture2D **) &tex.texture))){
			LOG_PRINT("Couldn't create render target");
			return TEXTURE_NONE;
		}
	} else {
		D3D11_TEXTURE3D_DESC desc;
		desc.Width  = width;
		desc.Height = height;
		desc.Depth  = depth;
		desc.Format = tex.texFormat;
		desc.MipLevels = mipMapCount;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		if (flags & USE_MIPGEN){
			desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		if (FAILED(device->CreateTexture3D(&desc, 0, (ID3D11Texture3D **) &tex.texture))){
			LOG_PRINT("Couldn't create render target");
			return TEXTURE_NONE;
		}
	}


	tex.srvFormat = tex.texFormat;
	tex.rtvFormat = tex.texFormat;
	tex.srv = createSRV(tex.texture, tex.srvFormat);
	tex.rtv = createRTV(tex.texture, tex.rtvFormat);

	int sliceCount = (depth == 1)? arraySize : depth;

	if (flags & SAMPLE_SLICES){
		tex.srvArray = new ID3D11ShaderResourceView *[sliceCount];
		for (int i = 0; i < sliceCount; i++){
			tex.srvArray[i] = createSRV(tex.texture, tex.srvFormat, i);
		}
	}

	if (flags & RENDER_SLICES){
		tex.rtvArray = new ID3D11RenderTargetView *[sliceCount];

		for (int i = 0; i < sliceCount; i++){
			tex.rtvArray[i] = createRTV(tex.texture, tex.rtvFormat, i);
		}
	}

	return textures.add(tex);
}

TextureID RendererHLSLD3D11::addRenderDepthVrtl(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags){
	TextureD3D11 tex;
	memset(&tex, 0, sizeof(tex));

	tex.width  = width;
	tex.height = height;
	tex.depth  = 1;
	tex.arraySize = arraySize;
	tex.flags  = flags;
	tex.texFormat = tex.dsvFormat = formatsd3d11[format];

	D3D11_TEXTURE2D_DESC desc;
	desc.Width  = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = msaaSamples;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	if (flags & CUBEMAP){
		desc.ArraySize = 6;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	} else {
		desc.ArraySize = arraySize;
		desc.MiscFlags = 0;
	}

	if (flags & SAMPLE_DEPTH){
		switch (tex.dsvFormat){
			case DXGI_FORMAT_D16_UNORM:
				tex.texFormat = DXGI_FORMAT_R16_TYPELESS;
				tex.srvFormat = DXGI_FORMAT_R16_UNORM;
				break;
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
				tex.texFormat = DXGI_FORMAT_R24G8_TYPELESS;
				tex.srvFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				break;
			case DXGI_FORMAT_D32_FLOAT:
				tex.texFormat = DXGI_FORMAT_R32_TYPELESS;
				tex.srvFormat = DXGI_FORMAT_R32_FLOAT;
				break;
		}
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	desc.Format = tex.texFormat;

	if (FAILED(device->CreateTexture2D(&desc, 0, (ID3D11Texture2D **) &tex.texture))){
		LOG_PRINT("Couldn't create depth target");
		return TEXTURE_NONE;
	}

	tex.dsv = createDSV(tex.texture, tex.dsvFormat);
	if (flags & RENDER_SLICES){
		tex.dsvArray = new ID3D11DepthStencilView *[arraySize];
		for (int i = 0; i < arraySize; i++){
			tex.dsvArray[i] = createDSV(tex.texture, tex.dsvFormat, i);
		}
	}

	if (flags & SAMPLE_DEPTH){
		tex.srv = createSRV(tex.texture, tex.srvFormat);

		if (flags & SAMPLE_SLICES){
			tex.srvArray = new ID3D11ShaderResourceView *[arraySize];
			for (int i = 0; i < arraySize; i++){
				tex.srvArray[i] = createSRV(tex.texture, tex.srvFormat, i);
			}
		}
	}

	return textures.add(tex);
}

bool RendererHLSLD3D11::resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize){
	D3D11_RESOURCE_DIMENSION type;
	textures[renderTarget].texture->GetType(&type);

	switch (type){
/*		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			D3D11_TEXTURE1D_DESC desc1d;
			((ID3D11Texture1D *) textures[renderTarget].texture)->GetDesc(&desc1d);

			desc1d.Width     = width;
			desc1d.ArraySize = arraySize;
			break;*/
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			D3D11_TEXTURE2D_DESC desc2d;
			((ID3D11Texture2D *) textures[renderTarget].texture)->GetDesc(&desc2d);

			desc2d.Width     = width;
			desc2d.Height    = height;
			desc2d.ArraySize = arraySize;
			desc2d.MipLevels = mipMapCount;

			textures[renderTarget].texture->Release();
			if (FAILED(device->CreateTexture2D(&desc2d, 0, (ID3D11Texture2D **) &textures[renderTarget].texture))){
				LOG_PRINT("Couldn't create render target");
				return false;
			}
			break;
/*		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			D3D11_TEXTURE3D_DESC desc3d;
			((ID3D11Texture3D *) textures[renderTarget].texture)->GetDesc(&desc3d);

			desc3d.Width  = width;
			desc3d.Height = height;
			desc3d.Depth  = depth;
			break;*/
		default:
			return false;
	}

	if (textures[renderTarget].rtv){
		textures[renderTarget].rtv->Release();
		textures[renderTarget].rtv = createRTV(textures[renderTarget].texture, textures[renderTarget].rtvFormat);
	}
	if (textures[renderTarget].dsv){
		textures[renderTarget].dsv->Release();
		textures[renderTarget].dsv = createDSV(textures[renderTarget].texture, textures[renderTarget].dsvFormat);
	}
	if (textures[renderTarget].srv){
		textures[renderTarget].srv->Release();
		textures[renderTarget].srv = createSRV(textures[renderTarget].texture, textures[renderTarget].srvFormat);
	}
	if (textures[renderTarget].rtvArray){
		for (int i = 0; i < textures[renderTarget].arraySize; i++){
			textures[renderTarget].rtvArray[i]->Release();
		}
		if (arraySize != textures[renderTarget].arraySize){
			delete [] textures[renderTarget].rtvArray;
			textures[renderTarget].rtvArray = new ID3D11RenderTargetView *[arraySize];
		}
		for (int i = 0; i < arraySize; i++){
			textures[renderTarget].rtvArray[i] = createRTV(textures[renderTarget].texture, textures[renderTarget].rtvFormat, i);
		}
	}
	if (textures[renderTarget].dsvArray){
		for (int i = 0; i < textures[renderTarget].arraySize; i++){
			textures[renderTarget].dsvArray[i]->Release();
		}
		if (arraySize != textures[renderTarget].arraySize){
			delete [] textures[renderTarget].dsvArray;
			textures[renderTarget].dsvArray = new ID3D11DepthStencilView *[arraySize];
		}
		for (int i = 0; i < arraySize; i++){
			textures[renderTarget].dsvArray[i] = createDSV(textures[renderTarget].texture, textures[renderTarget].dsvFormat, i);
		}
	}
	if (textures[renderTarget].srvArray){
		for (int i = 0; i < textures[renderTarget].arraySize; i++){
			textures[renderTarget].srvArray[i]->Release();
		}
		if (arraySize != textures[renderTarget].arraySize){
			delete [] textures[renderTarget].srvArray;
			textures[renderTarget].srvArray = new ID3D11ShaderResourceView *[arraySize];
		}
		for (int i = 0; i < arraySize; i++){
			textures[renderTarget].srvArray[i] = createSRV(textures[renderTarget].texture, textures[renderTarget].srvFormat, i);
		}
	}

	textures[renderTarget].width  = width;
	textures[renderTarget].height = height;
	textures[renderTarget].depth  = depth;
	textures[renderTarget].arraySize = arraySize;

	return true;
}

bool RendererHLSLD3D11::generateMipMaps(const TextureID renderTarget){
	ic->GenerateMips(textures[renderTarget].srv);

	return true;
}

int RendererHLSLD3D11::removeTexture(const TextureID texture){
	SAFE_RELEASE(textures[texture].texture);
	SAFE_RELEASE(textures[texture].srv);
	SAFE_RELEASE(textures[texture].rtv);
	SAFE_RELEASE(textures[texture].dsv);

	int sliceCount = (textures[texture].depth == 1)? textures[texture].arraySize : textures[texture].depth;

	if (textures[texture].srvArray){
		for (int i = 0; i < sliceCount; i++){
			textures[texture].srvArray[i]->Release();
		}
		delete [] textures[texture].srvArray;
		textures[texture].srvArray = 0;
	}
	if (textures[texture].rtvArray){
		for (int i = 0; i < sliceCount; i++){
			textures[texture].rtvArray[i]->Release();
		}
		delete [] textures[texture].rtvArray;
		textures[texture].rtvArray = 0;
	}
	if (textures[texture].dsvArray){
		for (int i = 0; i < sliceCount; i++){
			textures[texture].dsvArray[i]->Release();
		}
		delete [] textures[texture].dsvArray;
		textures[texture].dsvArray = 0;
	}
	return 0;
}
#if 1
	bool samplerCompD3D11std(const SamplerD3D11 & s0, const SamplerD3D11 &s1)
	{
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
	}
	bool constantCompD3D11std(const ConstantD3D11&	s0, const ConstantD3D11 &s1)
	{
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
	}
#endif
#if 1//ndef WindowsKits
	HRESULT CompileShader(uint32 id, const char* shaderstr, const char* entry,  ID3D11VertexShader** vss,
	ID3DBlob** ppBlob = 0
	)
	{
		HRESULT hr;
		char buf[65536];
        stx_snprintf(buf, 256, "%s\n", shaderstr);
		OutputDebugStringA(buf);
		#if 0
		DWORD dwShaderFlags =D3D11_SHADER_OPTIMIZATION_LEVEL0 | D3D11_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
		#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3D11_SHADER_DEBUG;
		#endif
		#else
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
		#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		#endif
		#endif
		ID3DBlob* error=0;
		#if 0
		hr = D3DX11CompileFromResource(ModuleHandle, 0, 0, 0, 0, entry,
		shaderstr, 0, 0, 0, &shader, &error, 0);
		#else
		hr=D3DCompile(	shaderstr, stx_strlen(shaderstr), "vsfilename", 0, 0,
						entry, vs11, dwShaderFlags, 0, ppBlob, &error);
		#endif
		if(error)
		{
			ErrorMsg((const char *) error->GetBufferPointer());
			DBG_HALT;
		}
		if(FAILED(hr))
		{
			DBG_HALT;
			return hr;
		}
		hr = ((RendererHLSLD3D11*)IRenderer::GetRendererInstance())->dev->CreateVertexShader((void*)(*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), 0, vss);

		if(FAILED(hr))
		{
			DBG_HALT;
			return hr;
		}
		return hr;
	}
	HRESULT CompileShader(uint32 id, const char* shaderstr, const char* entry, ID3D11PixelShader** pss,
	ID3DBlob** ppBlob = 0
	)
	{
		HRESULT hr;
		char buf[65536];
        stx_snprintf(buf, 256, "%s\n", shaderstr);
		OutputDebugStringA(buf);
		#if 0
		DWORD dwShaderFlags =D3D11_SHADER_OPTIMIZATION_LEVEL0 | D3D11_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
		#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3D11_SHADER_DEBUG;
		#endif
		#else
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
		#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		#endif
		#endif
		ID3DBlob* error=0;
		#if 0
		hr = D3DX11CompileFromResource(ModuleHandle, MAKEINTRESOURCE(id), 0, 0, 0, entry,
		shaderstr, 0, 0, 0, &shader, &error, 0);
		#else
        hr=D3DCompile(shaderstr, stx_strlen(shaderstr), "psfilename", 0, 0,
		entry, ps11, dwShaderFlags, 0,
		ppBlob, &error);
		#endif
		if(error)
		{
			ErrorMsg((const char *) error->GetBufferPointer());
			DBG_HALT;
		}
		if(FAILED(hr))
		{
			DBG_HALT;
			return hr;
		}
		hr = ((RendererHLSLD3D11*)IRenderer::GetRendererInstance())->dev->CreatePixelShader((void*)(*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(),0,  pss);
		if(FAILED(hr))
		{
			DBG_HALT;
			return hr;
		}
		return hr;
	}
#endif
ShaderID RendererHLSLD3D11::addHLSLShaderVrtl(const char *vsText, const char *gsText, const char *fsText
	, const char *vsName, const char *psName,
	const int vsLine, const int gsLine, const int fsLine,
	const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags)
	{
		if (vsText == 0 && gsText == 0 && fsText == 0) return SHADER_NONE;
		const char * mainvs=vsName;
		const char * mainps=psName;

		ShaderD3D11  shader;
		memset(&shader, 0, sizeof(ShaderD3D11));

		HRESULT hr=S_OK;
		ID3DBlob *shaderBuf = 0;
		ID3DBlob *errorsBuf = 0;
		ID3D11ShaderReflection *vsRefl = 0;
		ID3D11ShaderReflection *psRefl = 0;

		std::string shaderStringvs = "#define ROW_MAJOR row_major\n#define MVPSEMANTIC\n #define WSIGN +\n";
			if (extra != 0) shaderStringvs.append(extra);
			if (header != 0) shaderStringvs.append(header);

			std::string shaderStringfs=shaderStringvs;
			shaderStringvs.append(vsText);
			shaderStringfs.append(fsText);

			ID3DBlob* pVSBlob = 0;
			ID3DBlob* pPSBlob = 0;
			LOG_PRINT("\n%s\n",shaderStringvs.c_str());
			hr = CompileShader(	0,
						shaderStringvs.c_str(),
						mainvs,
						&(shader.vertexShader),
						&pVSBlob);
			if( FAILED( hr ) )
			DBG_HALT;
			if ((shader).vertexShader == 0) return SHADER_NONE;
			shader.inputSignature=pVSBlob;
			LOG_PRINT("\n%s\n",shaderStringfs.c_str());
			hr = CompileShader(1, shaderStringfs.c_str(), mainps, &(shader.pixelShader),&pPSBlob);
			if( FAILED( hr ) )
			DBG_HALT;

			if ((shader).pixelShader == 0) return SHADER_NONE;

			hr = D3DReflect(	pVSBlob->GetBufferPointer(),
						pVSBlob->GetBufferSize(),
						IID_ID3D11ShaderReflection,
						(void **)&vsRefl);
			if( FAILED( hr ) )
			DBG_HALT;

			hr = D3DReflect(	pPSBlob->GetBufferPointer(),
						pPSBlob->GetBufferSize(),
						IID_ID3D11ShaderReflection,
						(void **)&psRefl);
			if( FAILED( hr ) )
			DBG_HALT;

			D3D11_SHADER_DESC vsDesc;
			D3D11_SHADER_DESC gsDesc, psDesc;
			if (vsRefl)
			{
				vsRefl->GetDesc(&vsDesc);
				if (vsDesc.ConstantBuffers)
				{
					shader.nVSCBuffers = vsDesc.ConstantBuffers;
					shader.vsConstants = new ID3D11Buffer *[shader.nVSCBuffers];
					shader.vsConstMem = new ubyte *[shader.nVSCBuffers];
					shader.vsDirty = new bool[shader.nVSCBuffers];
				}
			}

			if (psRefl)
			{
				psRefl->GetDesc(&psDesc);
				if (psDesc.ConstantBuffers){
					shader.nPSCBuffers = psDesc.ConstantBuffers;
					shader.psConstants = new ID3D11Buffer *[shader.nPSCBuffers];
					shader.psConstMem = new ubyte *[shader.nPSCBuffers];
					shader.psDirty = new bool[shader.nPSCBuffers];
				}
			}

			D3D11_SHADER_BUFFER_DESC sbDesc;
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.Usage = D3D11_USAGE_DEFAULT;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = 0;
			cbDesc.MiscFlags = 0;
			VArray <ConstantD3D11> constants;

			char buf[1024];
			for (uint i = 0; i < (shader).nVSCBuffers; i++)
			{
				vsRefl->GetConstantBufferByIndex(i)->GetDesc(&sbDesc);
				cbDesc.ByteWidth = sbDesc.Size;
				device->CreateBuffer(&cbDesc, 0, &shader.vsConstants[i]);
				int limit=stx_min(sbDesc.Variables,20);
				shader.vsConstMem[i] = new ubyte[sbDesc.Size];
				for (uint k = 0; k < limit; k++)
				{

					if(k>20) DBG_HALT;
					D3D11_SHADER_VARIABLE_DESC vDesc;

					vsRefl->GetConstantBufferByIndex(i)->GetVariableByIndex(k)->GetDesc(&vDesc);

                    stx_snprintf(buf, 256, "n:%s",vDesc.Name);
					OutputDebugStringA(buf);
                    stx_snprintf(buf, 256, "s:%d",vDesc.Size);
					OutputDebugStringA(buf);
                    stx_snprintf(buf, 256, "o:%d",vDesc.StartOffset);
					OutputDebugStringA(buf);

					ConstantD3D11 constant;
                    			constant.name = (char*)vDesc.Name;
					constant.vsData = shader.vsConstMem[i] + vDesc.StartOffset;
					constant.psData = 0;
					constant.vsBuffer = i;
					constant.psBuffer = -1;
					constants.push_back(constant);
				}
				(shader).vsDirty[i] = false;
			}
			uint maxConst = constants.getCount();

			maxConst = constants.getCount();

			for (uint i = 0; i < (shader).nPSCBuffers; i++)
			{
				psRefl->GetConstantBufferByIndex(i)->GetDesc(&sbDesc);
				cbDesc.ByteWidth = sbDesc.Size;
				device->CreateBuffer(&cbDesc, 0, &shader.psConstants[i]);
				int limit=stx_min(sbDesc.Variables,20);
				shader.psConstMem[i] = new ubyte[sbDesc.Size];
				for (uint k = 0; k < limit; k++)
				{

					if(k>20) DBG_HALT;
					D3D11_SHADER_VARIABLE_DESC vDesc;

					psRefl->GetConstantBufferByIndex(i)->GetVariableByIndex(k)->GetDesc(&vDesc);

                    stx_snprintf(buf, 256, "n:%s",vDesc.Name);
					OutputDebugStringA(buf);
                    stx_snprintf(buf, 256, "s:%d",vDesc.Size);
					OutputDebugStringA(buf);
                    stx_snprintf(buf, 256, "o:%d",vDesc.StartOffset);
					OutputDebugStringA(buf);

					int merge = -1;
					for (uint i = 0; i < maxConst; i++)
					{
						if (strcmp(constants[i].name.c_str(), vDesc.Name) == 0)
						{
							merge = i;
							break;
						}
					}
					if (merge < 0)
					{
						ConstantD3D11 constant((char*)vDesc.Name);
                                		//size_t length = stx_strlen(vDesc.Name);
						//constant.name=(char*)vDesc.Name;
						constant.vsData = 0;
						constant.psData = shader.psConstMem[i] + vDesc.StartOffset;
						constant.vsBuffer = -1;
						constant.psBuffer = i;
						constants.push_back(constant);
					}
					else
					{
						constants[merge].psData = shader.psConstMem[i] + vDesc.StartOffset;
						constants[merge].psBuffer = i;
					}
				}
				(shader).psDirty[i] = false;
			}

			shader.nConstants = constants.getCount();
			//shader.constants = new ConstantD3D11[shader.nConstants];
			//memcpy(shader.constants, constants.getArray(), shader.nConstants * sizeof(ConstantD3D11));
			shader.constants=constants;
#if 1
			if(shader.constants.getCount()>1) qsort((void*)shader.constants.getArray(), shader.nConstants, sizeof(ConstantD3D11), constantCompD3D11);
#else
			std::sort((shader).constants.begin(), (shader).constants.end(), constantCompD3D11std);
#endif

			uint nMaxVSRes = vsRefl? vsDesc.BoundResources : 0;
			uint nMaxPSRes = psRefl? psDesc.BoundResources : 0;
			int maxResources = nMaxVSRes + nMaxPSRes;
			if (maxResources)
			{
				//shader.textures = (SamplerD3D11 *) malloc(maxResources * sizeof(Sampler));
				//shader.samplers = (SamplerD3D11 *) malloc(maxResources * sizeof(Sampler));
				shader.nTextures = 0;
				shader.nSamplers = 0;

				D3D11_SHADER_INPUT_BIND_DESC siDesc;
				for (uint i = 0; i < nMaxVSRes; i++)
				{
					vsRefl->GetResourceBindingDesc(i, &siDesc);
					if (siDesc.Type == D3D_SIT_TEXTURE)
					{
						SamplerD3D11 sampler;
						shader.textures.push_back(sampler);
						shader.textures[shader.nTextures].name = siDesc.Name;
						(shader).textures[(shader).nTextures].vsIndex = siDesc.BindPoint;
						(shader).textures[(shader).nTextures].psIndex = -1;
						(shader).nTextures++;
					}
					else if (siDesc.Type == D3D_SIT_SAMPLER)
					{
						SamplerD3D11 sampler;
						shader.samplers.push_back(sampler);
						shader.samplers[shader.nSamplers].name = siDesc.Name;


						(shader).samplers[(shader).nSamplers].vsIndex = siDesc.BindPoint;
						(shader).samplers[(shader).nSamplers].psIndex = -1;
						(shader).nSamplers++;
					}
				}
				uint maxTexture = (shader).nTextures;
				uint maxSampler = (shader).nSamplers;

			maxTexture = (shader).nTextures;
			maxSampler = (shader).nSamplers;
			for (uint i = 0; i < nMaxPSRes; i++)
			{
				psRefl->GetResourceBindingDesc(i, &siDesc);
				if (siDesc.Type == D3D_SIT_TEXTURE)
				{
					int merge = -1;
																for (uint i = 0; i < maxTexture; i++){
																	if (strcmp(shader.textures[i].name.c_str(), siDesc.Name) == 0){
																		merge = i;
																		break;
																	}
											}
					if (merge < 0)
					{
						SamplerD3D11 sampler;
						sampler.name = siDesc.Name;
						sampler.vsIndex = -1;
						sampler.psIndex = siDesc.BindPoint;
						shader.textures.push_back(sampler);
						(shader).nTextures++;
					}
					else
					{
						(shader).textures[merge].psIndex = siDesc.BindPoint;
					}
				}
				else if (siDesc.Type == D3D_SIT_SAMPLER)
				{
					int merge = -1;
																for (uint i = 0; i < maxSampler; i++){
																	if (strcmp(shader.samplers[i].name.c_str(), siDesc.Name) == 0){
																		merge = i;
																		break;												}
				}

									if (merge < 0){
						SamplerD3D11 sampler;
						sampler.name = siDesc.Name;
						sampler.vsIndex = -1;
						sampler.psIndex = siDesc.BindPoint;
						shader.samplers.push_back(sampler);
						shader.nSamplers++;
									} else {
										shader.samplers[merge].psIndex = siDesc.BindPoint;
				}
				}
			}

	//shader.textures = (SamplerD3D11 *) realloc(shader.textures, shader.nTextures * sizeof(SamplerD3D11));
	//shader.samplers = (SamplerD3D11 *) realloc(shader.samplers, shader.nSamplers * sizeof(SamplerD3D11));
#if 1
	if(shader.textures.getCount()>1) qsort((void*)shader.textures.getArray(), shader.nTextures, sizeof(SamplerD3D11), samplerCompD3D11);
	if(shader.samplers.getCount()>1) qsort((void*)shader.samplers.getArray(), shader.nSamplers, sizeof(SamplerD3D11), samplerCompD3D11);
#else
	std::sort((shader).textures.begin(), (shader).textures.end(), samplerCompD3D11std);
	std::sort((shader).samplers.begin(), (shader).samplers.end(), samplerCompD3D11std);
#endif
		}

		if (vsRefl) vsRefl->Release();
		if (psRefl) psRefl->Release();

		if(pPSBlob) pPSBlob->Release();
		shaders.push_back(shader);



		LOG_PRINT("Textures:\n");
		for(int i=0;i<shader.nTextures;i++)
		{
			LOG_PRINT("vsIndex=%x\n",(shader).textures[(shader).nTextures].vsIndex);
			LOG_PRINT("psIndex=%x\n",(shader).textures[(shader).nTextures].psIndex);
		}
		LOG_PRINT("Samplers:\n");
		for(int i=0;i<shader.nSamplers;i++)
		{
			LOG_PRINT("vsIndex=%x\n",(shader).samplers[(shader).nSamplers].vsIndex);
			LOG_PRINT("psIndex=%x\n",(shader).samplers[(shader).nSamplers].psIndex);
		}

		return shaders.size()-1;
	}

VertexFormatID RendererHLSLD3D11::addVertexFormatVrtl(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader){
	static const DXGI_FORMAT formats[][4] = {
		DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32G32_FLOAT, DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R16_FLOAT, DXGI_FORMAT_R16G16_FLOAT, DXGI_FORMAT_UNKNOWN,         DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R8_UNORM,  DXGI_FORMAT_R8G8_UNORM,   DXGI_FORMAT_UNKNOWN,         DXGI_FORMAT_R8G8B8A8_UNORM,
	};

	static const char *semantics[] = {
		0,
		"Position",
		"Texcoord",
		"Normal",
		"Tangent",
		"Binormal",
	};


	int index[6];
	memset(index, 0, sizeof(index));

	VertexFormatD3D11 vf;
	memset(vf.vertexSize, 0, sizeof(vf.vertexSize));

	D3D11_INPUT_ELEMENT_DESC *desc = new D3D11_INPUT_ELEMENT_DESC[nAttribs];

	// Fill the vertex element array
	for (uint i = 0; i < nAttribs; i++){
		int stream = formatDesc[i].stream;
		int size = formatDesc[i].size;
		desc[i].InputSlot = stream;
		desc[i].AlignedByteOffset = vf.vertexSize[stream];
		desc[i].SemanticName = semantics[formatDesc[i].type];
		desc[i].SemanticIndex = index[formatDesc[i].type]++;
		desc[i].Format = formats[formatDesc[i].format][size - 1];
		desc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc[i].InstanceDataStepRate = 0;

		vf.vertexSize[stream] += size * getFormatSize(formatDesc[i].format);
	}

	HRESULT hr = device->CreateInputLayout(desc, nAttribs, shaders[shader].inputSignature->GetBufferPointer(), shaders[shader].inputSignature->GetBufferSize(), &vf.inputLayout);
	delete desc;

	if (FAILED(hr)){
		LOG_PRINT("Couldn't create vertex declaration");
		return VF_NONE;
	}

	return vertexFormats.add(vf);
}

D3D11_USAGE usage[] = {
	D3D11_USAGE_IMMUTABLE,
	D3D11_USAGE_DEFAULT,
	D3D11_USAGE_DYNAMIC,
};

VertexBufferID RendererHLSLD3D11::addVertexBufferVrtl(const long size, const BufferAccess bufferAccess, const void *data, VertexBufferID old){
	VertexBufferD3D11 vb(dev, ic);
	vb.size = size;

	D3D11_BUFFER_DESC desc;
	desc.Usage = usage[bufferAccess];
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = (bufferAccess == DYNAMIC)? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData;
	vbData.pSysMem = data;
	vbData.SysMemPitch = 0;
	vbData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&desc, data? &vbData : 0, &vb.vertexBuffer))){
        LOG_PRINT("Couldn't create vertex buffer");
		return VB_NONE;
	}

	return vertexBuffers.add(vb);
}

IndexBufferID RendererHLSLD3D11::addIndexBufferVrtl(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID old){
	IndexBufferD3D11 ib(dev, ic);
	ib.indexSize = indexSize;
	ib.nIndices = nIndices;

	D3D11_BUFFER_DESC desc;
	desc.Usage = usage[bufferAccess];
	desc.ByteWidth = nIndices * indexSize;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = (bufferAccess == DYNAMIC)? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData;
	ibData.pSysMem = data;
	ibData.SysMemPitch = 0;
	ibData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&desc, data? &ibData : 0, &ib.indexBuffer))){
        LOG_PRINT("Couldn't create vertex buffer");
		return IB_NONE;
	}

	return indexBuffers.add(ib);
}

D3D11_FILTER filters[] = {
	D3D11_FILTER_MIN_MAG_MIP_POINT,
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
	D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	D3D11_FILTER_ANISOTROPIC,
	D3D11_FILTER_ANISOTROPIC,
};

D3D11_TEXTURE_ADDRESS_MODE address_modes[] = {
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,
};

SamplerStateID RendererHLSLD3D11::addSamplerStateVrtl(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod, const uint maxAniso, const int compareFunc, const float *border_color){
	SamplerStateD3D11 samplerState;

	D3D11_SAMPLER_DESC desc;
	desc.Filter = filters[filter];
	if (compareFunc){
		desc.Filter = (D3D11_FILTER) (desc.Filter | 0x80);
	}
	desc.ComparisonFunc = (D3D11_COMPARISON_FUNC) compareFunc;
	desc.AddressU = address_modes[s];
	desc.AddressV = address_modes[t];
	desc.AddressW = address_modes[r];
	desc.MipLODBias = lod;
	desc.MaxAnisotropy = hasAniso(filter)? maxAniso : 1;
	if (border_color)
	{
		desc.BorderColor[0] = border_color[0];
		desc.BorderColor[1] = border_color[1];
		desc.BorderColor[2] = border_color[2];
		desc.BorderColor[3] = border_color[3];
	}
	else
	{
		desc.BorderColor[0] = 0;
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 0;
	}
	desc.MinLOD = 0;
	desc.MaxLOD = hasMipmaps(filter)? D3D11_FLOAT32_MAX : 0;

	if (FAILED(device->CreateSamplerState(&desc, &samplerState.samplerState))){
		LOG_PRINT("Couldn't create samplerstate");
		return SS_NONE;
	}

	return samplerStates.add(samplerState);
}

BlendStateID RendererHLSLD3D11::addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask, const bool alphaToCoverage){
	#if 0
	BlendStateD3D11 blendState;

	BOOL blendEnable =
		srcFactorRGB != D3D11_BLEND_ONE || destFactorRGB != D3D11_BLEND_ZERO ||
		srcFactorAlpha != D3D11_BLEND_ONE || destFactorAlpha != D3D11_BLEND_ZERO;

	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = (BOOL) alphaToCoverage;
	desc.BlendOp = (D3D11_BLEND_OP) blendModeAlpha;
	desc.SrcBlend = (D3D11_BLEND) srcFactorRGB;
	desc.DestBlend = (D3D11_BLEND) destFactorRGB;
	desc.BlendOpAlpha = (D3D11_BLEND_OP) blendModeAlpha;
	desc.SrcBlendAlpha = (D3D11_BLEND) srcFactorAlpha;
	desc.DestBlendAlpha = (D3D11_BLEND) destFactorAlpha;

	memset(&desc.BlendEnable, 0, sizeof(desc.BlendEnable));
	memset(&desc.RenderTargetWriteMask, 0, sizeof(desc.RenderTargetWriteMask));
	desc.BlendEnable[0] = blendEnable;
	desc.RenderTargetWriteMask[0] = mask;

	if (FAILED(device->CreateBlendState(&desc, &blendState.blendState))){
		LOG_PRINT("Couldn't create blendstate");
		return BS_NONE;
	}

	return blendStates.add(blendState);
	#else
	return -1;
	#endif
}

DepthStateID RendererHLSLD3D11::addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc,
											   const bool stencilTest, const uint8 stencilMask, const int stencilFunc,
											   const int stencilFail, const int depthFail, const int stencilPass){
	DepthStateD3D11 depthState;

	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = (BOOL) depthTest;
	desc.DepthWriteMask = depthWrite? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = (D3D11_COMPARISON_FUNC) depthFunc;
	desc.StencilEnable = (BOOL) stencilTest;
	desc.StencilReadMask  = stencilMask;
	desc.StencilWriteMask = stencilMask;
	desc.BackFace. StencilFunc = (D3D11_COMPARISON_FUNC) stencilFunc;
	desc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC) stencilFunc;
	desc.BackFace. StencilDepthFailOp = (D3D11_STENCIL_OP) depthFail;
	desc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP) depthFail;
	desc.BackFace. StencilFailOp = (D3D11_STENCIL_OP) stencilFail;
	desc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP) stencilFail;
	desc.BackFace. StencilPassOp = (D3D11_STENCIL_OP) stencilPass;
	desc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP) stencilPass;

	if (FAILED(device->CreateDepthStencilState(&desc, &depthState.dsState))){
		LOG_PRINT("Couldn't create depthstate");
		return DS_NONE;
	}

	return depthStates.add(depthState);
}

RasterizerStateID RendererHLSLD3D11::addRasterizerStateVrtl(const int cullMode, const int fillMode, const bool multiSample, const bool scissor){
	RasterizerStateD3D11 rasterizerState;

	D3D11_RASTERIZER_DESC desc;
	desc.CullMode = (D3D11_CULL_MODE) cullMode;
	desc.FillMode = (D3D11_FILL_MODE) fillMode;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.AntialiasedLineEnable = FALSE;
	desc.DepthClipEnable = TRUE;
	desc.MultisampleEnable = (BOOL) multiSample;
	desc.ScissorEnable = (BOOL) scissor;

	if (FAILED(device->CreateRasterizerState(&desc, &rasterizerState.rsState))){
		LOG_PRINT("Couldn't create rasterizerstate");
		return RS_NONE;
	}

	return rasterizerStates.add(rasterizerState);
}

const SamplerD3D11 *getSampler(const SamplerD3D11 *samplers, const int count, const char *name){
	int minSampler = 0;
	int maxSampler = count - 1;

	// Do a quick lookup in the sorted table with a binary search
	while (minSampler <= maxSampler){
		int currSampler = (minSampler + maxSampler) >> 1;
        int res = strcmp(name, samplers[currSampler].name.c_str());
		if (res == 0){
			return samplers + currSampler;
		} else if (res > 0){
            minSampler = currSampler + 1;
		} else {
            maxSampler = currSampler - 1;
		}
	}

	return 0;
}

void RendererHLSLD3D11::setTexture(const char *textureName, const TextureID texture){
	ASSERT(selectedShader != SHADER_NONE);

	const SamplerD3D11 *s = getSampler(shaders[selectedShader].textures.getArray(), shaders[selectedShader].nTextures, textureName);
	if (s){
		if (s->vsIndex >= 0){
			selectedTexturesVS[s->vsIndex] = texture;
			selectedTextureSlicesVS[s->vsIndex] = NO_SLICE;
		}/*
		if (s->gsIndex >= 0){
			selectedTexturesGS[s->gsIndex] = texture;
			selectedTextureSlicesGS[s->gsIndex] = NO_SLICE;
		}*/
		if (s->psIndex >= 0){
			selectedTexturesPS[s->psIndex] = texture;
			selectedTextureSlicesPS[s->psIndex] = NO_SLICE;
		}
	}
#ifdef _DEBUG
	else {
		char str[256];
		sprintf(str, "Invalid texture \"%s\"", textureName);
		LOG_PRINT("%s\n", str);
	}
#endif
	//setSamplerState(textureName, Getlinear());
}

void RendererHLSLD3D11::setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState){
	ASSERT(selectedShader != SHADER_NONE);

	const SamplerD3D11 *s = getSampler(shaders[selectedShader].textures.getArray(), shaders[selectedShader].nTextures, textureName);
	if (s){
		if (s->vsIndex >= 0){
			selectedTexturesVS[s->vsIndex] = texture;
			selectedTextureSlicesVS[s->vsIndex] = NO_SLICE;
			selectedSamplerStatesVS[s->vsIndex] = samplerState;
		}/*
		if (s->gsIndex >= 0){
			selectedTexturesGS[s->gsIndex] = texture;
			selectedTextureSlicesGS[s->gsIndex] = NO_SLICE;
			selectedSamplerStatesGS[s->gsIndex] = samplerState;
		}*/
		if (s->psIndex >= 0){
			selectedTexturesPS[s->psIndex] = texture;
			selectedTextureSlicesPS[s->psIndex] = NO_SLICE;
			selectedSamplerStatesPS[s->psIndex] = samplerState;
		}
	}
#ifdef _DEBUG
	else {
		char str[256];
		sprintf(str, "Invalid texture \"%s\"", textureName);
		LOG_PRINT("%s\n", str);
	}
#endif
	//setSamplerState(textureName, samplerState);
}

void RendererHLSLD3D11::setTextureSlice(const char *textureName, const TextureID texture, const int slice){
	ASSERT(selectedShader != SHADER_NONE);

	const SamplerD3D11 *s = getSampler(shaders[selectedShader].textures.getArray(), shaders[selectedShader].nTextures, textureName);
	if (s){
		if (s->vsIndex >= 0){
			selectedTexturesVS[s->vsIndex] = texture;
			selectedTextureSlicesVS[s->vsIndex] = slice;
		}/*
		if (s->gsIndex >= 0){
			selectedTexturesGS[s->gsIndex] = texture;
			selectedTextureSlicesGS[s->gsIndex] = slice;
		}*/
		if (s->psIndex >= 0){
			selectedTexturesPS[s->psIndex] = texture;
			selectedTextureSlicesPS[s->psIndex] = slice;
		}
	}
#ifdef _DEBUG
	else {
		char str[256];
		sprintf(str, "Invalid texture \"%s\"", textureName);
		LOG_PRINT("%s\n", str);
	}
#endif
}


bool fillSRV(ID3D11ShaderResourceView **dest, int &min, int &max, const TextureID selectedTextures[], TextureID currentTextures[], const TextureID selectedTextureSlices[], TextureID currentTextureSlices[], const TextureD3D11 *textures){
	min = 0;
	do {
		if (selectedTextures[min] != currentTextures[min] || selectedTextureSlices[min] != currentTextureSlices[min]){
			max = MAX_TEXTUREUNIT;
			do {
				max--;
			} while (selectedTextures[max] == currentTextures[max] && selectedTextureSlices[max] != currentTextureSlices[max]);

			for (int i = min; i <= max; i++){
				if (selectedTextures[i] != TEXTURE_NONE){
					if (selectedTextureSlices[i] == NO_SLICE){
						*dest++ = textures[selectedTextures[i]].srv;
					} else {
						*dest++ = textures[selectedTextures[i]].srvArray[selectedTextureSlices[i]];
					}
				} else {
					*dest++ = 0;
				}
				currentTextures[i] = selectedTextures[i];
				currentTextureSlices[i] = selectedTextureSlices[i];
			}
			return true;
		}
		min++;
	} while (min < MAX_TEXTUREUNIT);

	return false;
}

void RendererHLSLD3D11::applyTextures(){
	ID3D11ShaderResourceView *srViews[MAX_TEXTUREUNIT];

	int min, max;
	if (fillSRV(srViews, min, max, selectedTexturesVS, currentTexturesVS, selectedTextureSlicesVS, currentTextureSlicesVS, textures.getArray())){
		ic->VSSetShaderResources(min, max - min + 1, srViews);
	}
	//if (fillSRV(srViews, min, max, selectedTexturesGS, currentTexturesGS, selectedTextureSlicesGS, currentTextureSlicesGS, textures.getArray())){
	//	device->GSSetShaderResources(min, max - min + 1, srViews);
	//}
	if (fillSRV(srViews, min, max, selectedTexturesPS, currentTexturesPS, selectedTextureSlicesPS, currentTextureSlicesPS, textures.getArray())){
		ic->PSSetShaderResources(min, max - min + 1, srViews);
	}
}

void RendererHLSLD3D11::setSamplerState(const char *samplerName, const SamplerStateID samplerState){
	ASSERT(selectedShader != SHADER_NONE);

	const SamplerD3D11 *s = getSampler(shaders[selectedShader].samplers.getArray(), shaders[selectedShader].nSamplers, samplerName);
	if (s){
		if (s->vsIndex >= 0) selectedSamplerStatesVS[s->vsIndex] = samplerState;
		//if (s->gsIndex >= 0) selectedSamplerStatesGS[s->gsIndex] = samplerState;
		if (s->psIndex >= 0) selectedSamplerStatesPS[s->psIndex] = samplerState;
	}
#ifdef _DEBUG
	else {
		char str[256];
		sprintf(str, "Invalid samplerstate \"%s\"", samplerName);
		LOG_PRINT("%s\n", str);
	}
#endif
}

bool fillSS(ID3D11SamplerState **dest, int &min, int &max, const SamplerStateID aselectedSamplerStates[], SamplerStateID acurrentSamplerStates[], const SamplerStateD3D11 *asamplerStates){
	min = 0;
	do {
		if (aselectedSamplerStates[min] != acurrentSamplerStates[min]){
			max = MAX_SAMPLERSTATE;
			do {
				max--;
			} while (aselectedSamplerStates[max] == acurrentSamplerStates[max]);

			for (int i = min; i <= max; i++){
				if (aselectedSamplerStates[i] != SS_NONE){
					*dest++ = asamplerStates[aselectedSamplerStates[i]].samplerState;
				} else {
					*dest++ = 0;
				}
				acurrentSamplerStates[i] = aselectedSamplerStates[i];
			}
			return true;
		}
		min++;
	} while (min < MAX_SAMPLERSTATE);

	return false;
}

void RendererHLSLD3D11::applySamplerStates(){
	ID3D11SamplerState *samplers[MAX_SAMPLERSTATE];

	int min, max;
	int size=samplerStates.getCount();
	if(size)
	if (fillSS(samplers, min, max, selectedSamplerStatesVS, currentSamplerStatesVS, samplerStates.getArray())){
		ic->VSSetSamplers(min, max - min + 1, samplers);
	}
	//if(size) if (fillSS(samplers, min, max, selectedSamplerStatesGS, currentSamplerStatesGS, samplerStates.getArray())){
	//	device->GSSetSamplers(min, max - min + 1, samplers);
	//}
	if(size)
	if (fillSS(samplers, min, max, selectedSamplerStatesPS, currentSamplerStatesPS, samplerStates.getArray())){
		ic->PSSetSamplers(min, max - min + 1, samplers);
	}
}

void RendererHLSLD3D11::setShaderConstantRaw(const char *name, const void *data, const int size){
	int minConstant = 0;
	if(selectedShader==-1) return;
	int maxConstant = shaders[selectedShader].nConstants - 1;
	ConstantD3D11 *constants = shaders[selectedShader].constants.getArray();

	// Do a quick lookup in the sorted table with a binary search
	while (minConstant <= maxConstant){
		int currConstant = (minConstant + maxConstant) >> 1;
		if (currConstant == -1) return;
		int res = strcmp(name, constants[currConstant].name.c_str());
		if (res == 0){
			ConstantD3D11 *c = constants + currConstant;

			if (c->vsData){
				if (memcmp(c->vsData, data, size)){
					memcpy(c->vsData, data, size);
					shaders[selectedShader].vsDirty[c->vsBuffer] = true;
				}
			}/*
			if (c->gsData){
				if (memcmp(c->gsData, data, size)){
					memcpy(c->gsData, data, size);
					shaders[selectedShader].gsDirty[c->gsBuffer] = true;
				}
			}*/
			if (c->psData){
				if (memcmp(c->psData, data, size)){
					memcpy(c->psData, data, size);
					shaders[selectedShader].psDirty[c->psBuffer] = true;
				}
			}
			return;

		} else if (res > 0){
			minConstant = currConstant + 1;
		} else {
			maxConstant = currConstant - 1;
		}
	}

#ifdef _DEBUG
	char str[256];
	sprintf(str, "Invalid constant \"%s\"", name);
	LOG_PRINT("%s\n", str);
#endif
}

void RendererHLSLD3D11::applyConstants(){
	if (currentShader != SHADER_NONE){
		ShaderD3D11 *shader = &shaders[currentShader];

		for (uint i = 0; i < shader->nVSCBuffers; i++){
			if (shader->vsDirty[i]){
				ic->UpdateSubresource(shader->vsConstants[i], 0, 0, shader->vsConstMem[i], 0, 0);
				shader->vsDirty[i] = false;
			}
		}
		/*
		for (uint i = 0; i < shader->nGSCBuffers; i++){
			if (shader->gsDirty[i]){
				ic->UpdateSubresource(shader->gsConstants[i], 0, 0, shader->gsConstMem[i], 0, 0);
				shader->gsDirty[i] = false;
			}
		}
		*/
		for (uint i = 0; i < shader->nPSCBuffers; i++){
			if (shader->psDirty[i]){
				ic->UpdateSubresource(shader->psConstants[i], 0, 0, shader->psConstMem[i], 0, 0);
				shader->psDirty[i] = false;
			}
		}
	}
}

void RendererHLSLD3D11::changeRenderTargetsVrtl(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int depthSlice, const int *slices){
	// Reset bound textures
	for (int i = 0; i < MAX_TEXTUREUNIT; i++){
		selectedTexturesVS[i] = TEXTURE_NONE;
		selectedTexturesGS[i] = TEXTURE_NONE;
		selectedTexturesPS[i] = TEXTURE_NONE;
	}
	applyTextures();

	ID3D11RenderTargetView *rtv[16];
	ID3D11DepthStencilView *dsv;

	if (depthRT == FB_DEPTH){
		dsv = depthBufferDSV;
	} else if (depthRT == TEXTURE_NONE){
		dsv = 0;
	} else if (depthSlice == NO_SLICE){
		dsv = textures[depthRT].dsv;
	} else {
		dsv = textures[depthRT].dsvArray[depthSlice];
	}
	currentDepthRT = depthRT;
	currentDepthSlice = depthSlice;

	for (uint i = 0; i < nRenderTargets; i++){
		TextureID rt = colorRTs[i];
		int slice = NO_SLICE;
		if (slices == 0 || slices[i] == NO_SLICE){
			if (rt == FB_COLOR){
				rtv[i] = backBufferRTV;
			} else {
				rtv[i] = textures[rt].rtv;
			}
		} else {
			slice = slices[i];
			rtv[i] = textures[rt].rtvArray[slice];
		}

		currentColorRT[i] = rt;
		currentColorRTSlice[i] = slice;
	}

	for (uint i = nRenderTargets; i < MAX_MRTS; i++){
		currentColorRT[i] = TEXTURE_NONE;
		currentColorRTSlice[i] = NO_SLICE;
	}

	ic->OMSetRenderTargets(nRenderTargets, rtv, dsv);

	D3D11_VIEWPORT vp0;
	UINT nvp0;
	ic->RSGetViewports(&nvp0, &vp0);;

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	if (nRenderTargets > 0){
		TextureID rt = colorRTs[0];
		if (rt == FB_COLOR){
			vp.Width  = vp0.Width;
			vp.Height = vp0.Height;
		} else {
			vp.Width  = textures[rt].width;
			vp.Height = textures[rt].height;
		}
	} else {
		vp.Width  = textures[depthRT].width;
		vp.Height = textures[depthRT].height;
	}
	ic->RSSetViewports(1, &vp);
}

void RendererHLSLD3D11::changeToMainFramebuffer(){
	// Reset bound textures
	for (int i = 0; i < MAX_TEXTUREUNIT; i++){
		selectedTexturesVS[i] = TEXTURE_NONE;
		selectedTexturesGS[i] = TEXTURE_NONE;
		selectedTexturesPS[i] = TEXTURE_NONE;
	}
	applyTextures();

	ic->OMSetRenderTargets(1, &backBufferRTV, depthBufferDSV);

	D3D11_VIEWPORT vp = { 0, 0, IRenderer::GetViewportWidth(), IRenderer::GetViewportHeight(), 0.0f, 1.0f };
	ic->RSSetViewports(1, &vp);

	currentColorRT[0] = FB_COLOR;
	currentColorRTSlice[0] = NO_SLICE;

	for (uint i = 1; i < MAX_MRTS; i++){
		currentColorRT[i] = TEXTURE_NONE;
		currentColorRTSlice[i] = NO_SLICE;
	}
	currentDepthRT = FB_DEPTH;
	currentDepthSlice = NO_SLICE;
}

void RendererHLSLD3D11::changeShader(const ShaderID shaderID){
	if (shaderID != currentShader){

			if (shaderID == SHADER_NONE)
			{
				ic->VSSetShader(0, 0, 0 );
				ic->GSSetShader(0, 0, 0 );
				ic->PSSetShader(0, 0, 0 );
			}
			else {
			ic->VSSetShader(shaders[shaderID].vertexShader, 0, 0 );
			ic->GSSetShader(0, 0, 0 );
			ic->PSSetShader(shaders[shaderID].pixelShader, 0, 0 );

			if (shaders[shaderID].nVSCBuffers) ic->VSSetConstantBuffers(0, shaders[shaderID].nVSCBuffers, shaders[shaderID].vsConstants);
			//if (shaders[shaderID].nGSCBuffers) device->GSSetConstantBuffers(0, shaders[shaderID].nGSCBuffers, shaders[shaderID].gsConstants);
			if (shaders[shaderID].nPSCBuffers) ic->PSSetConstantBuffers(0, shaders[shaderID].nPSCBuffers, shaders[shaderID].psConstants);
		}

		currentShader = shaderID;
	}
}

void RendererHLSLD3D11::changeVertexFormat(const VertexFormatID vertexFormatID){
	if (vertexFormatID != currentVertexFormat){
		if (vertexFormatID == VF_NONE){
			ic->IASetInputLayout(0);
		} else {
			ic->IASetInputLayout(vertexFormats[vertexFormatID].inputLayout);

			/*if (currentVertexFormat != VF_NONE){
				for (int i = 0; i < MAX_VERTEXSTREAM; i++){
					if (vertexFormats[vertexFormatID].vertexSize[i] != vertexFormats[currentVertexFormat].vertexSize[i]){
						currentVertexBuffers[i] = VB_INVALID;
					}
				}
			}*/
		}

		currentVertexFormat = vertexFormatID;
	}
}

void RendererHLSLD3D11::changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBufferID, const intptr offset){
	if (vertexBufferID != currentVertexBuffers[stream] || offset != currentOffsets[stream]){
		UINT strides[1];
		UINT offsets[1];
		if (vertexBufferID == VB_NONE){
			strides[0] = 0;
			offsets[0] = 0;
			ID3D11Buffer *null[] = { 0 };
			ic->IASetVertexBuffers(stream, 1, null, strides, offsets);
		} else {
			strides[0] = vertexFormats[currentVertexFormat].vertexSize[stream];
			offsets[0] = (UINT) offset;
			ic->IASetVertexBuffers(stream, 1, &vertexBuffers[vertexBufferID].vertexBuffer, strides, offsets);
		}

		currentVertexBuffers[stream] = vertexBufferID;
		currentOffsets[stream] = offset;
	}
}

void RendererHLSLD3D11::changeIndexBuffer(const IndexBufferID indexBufferID){
	if (indexBufferID != currentIndexBuffer){
		if (indexBufferID == IB_NONE){
			ic->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		} else {
			DXGI_FORMAT format = indexBuffers[indexBufferID].indexSize < 4? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
			ic->IASetIndexBuffer(indexBuffers[indexBufferID].indexBuffer, format, 0);
		}

		currentIndexBuffer = indexBufferID;
	}
}

void RendererHLSLD3D11::changeBlendStateVrtl(const BlendStateID blendState, const uint sampleMask){
#if 0
	if (blendState != currentBlendState || sampleMask != currentSampleMask){
		if (blendState == BS_NONE){
			ic->OMSetBlendState(0, D3DXFROMWINEVECTOR4(0, 0, 0, 0), sampleMask);
		} else {
			ic->OMSetBlendState(blendStates[blendState].blendState, D3DXFROMWINEVECTOR4(0, 0, 0, 0), sampleMask);
		}

		currentBlendState = blendState;
		currentSampleMask = sampleMask;
	}
#endif
}

void RendererHLSLD3D11::changeDepthStateVrtl(const DepthStateID depthState, const uint stencilRef){
	if (depthState != currentDepthState || stencilRef != currentStencilRef){
		if (depthState == DS_NONE){
			ic->OMSetDepthStencilState(0, stencilRef);
		} else {
			ic->OMSetDepthStencilState(depthStates[depthState].dsState, stencilRef);
		}

		currentDepthState = depthState;
		currentStencilRef = stencilRef;
	}
}

void RendererHLSLD3D11::changeRasterizerState(const RasterizerStateID rasterizerState){
	if (rasterizerState != currentRasterizerState){
		if (rasterizerState == RS_NONE){
			//ic->RSSetState(0);
			ic->RSSetState(rasterizerStates[0].rsState);
		} else {
			ic->RSSetState(rasterizerStates[rasterizerState].rsState);
		}

		currentRasterizerState = rasterizerState;
	}
}

void RendererHLSLD3D11::clear(const bool clearColor, const bool clearDepth, const bool clearStencil, const float *color, const float depth, const uint stencil){
	if (clearColor){
		if (currentColorRT[0] == FB_COLOR){
			ic->ClearRenderTargetView(backBufferRTV, color);
		}

		for (int i = 0; i < MAX_MRTS; i++){
			if (currentColorRT[i] >= 0){
				if (currentColorRTSlice[i] == NO_SLICE){
					ic->ClearRenderTargetView(textures[currentColorRT[i]].rtv, color);
				} else {
					ic->ClearRenderTargetView(textures[currentColorRT[i]].rtvArray[currentColorRTSlice[i]], color);
				}
			}
		}
	}
	if (clearDepth || clearStencil){
		UINT clearFlags = 0;
		if (clearDepth)   clearFlags |= D3D11_CLEAR_DEPTH;
		if (clearStencil) clearFlags |= D3D11_CLEAR_STENCIL;

		if (currentDepthRT == FB_DEPTH){
			ic->ClearDepthStencilView(depthBufferDSV, clearFlags, depth, stencil);
		} else if (currentDepthRT >= 0){
			if (currentDepthSlice == NO_SLICE){
				ic->ClearDepthStencilView(textures[currentDepthRT].dsv, clearFlags, depth, stencil);
			} else {
				ic->ClearDepthStencilView(textures[currentDepthRT].dsvArray[currentDepthSlice], clearFlags, depth, stencil);
			}
		}
	}
}

void RendererHLSLD3D11::clearRenderTarget(const TextureID renderTarget, const D3DXFROMWINEVECTOR4 &color, const int slice){
	if (slice == NO_SLICE){
		ic->ClearRenderTargetView(textures[renderTarget].rtv, color);
	} else {
		ic->ClearRenderTargetView(textures[renderTarget].rtvArray[slice], color);
	}
}

void RendererHLSLD3D11::clearDepthTarget(const TextureID depthTarget, const float depth, const int slice){
	if (slice == NO_SLICE){
		ic->ClearDepthStencilView(textures[depthTarget].dsv, D3D11_CLEAR_DEPTH, depth, 0);
	} else {
		ic->ClearDepthStencilView(textures[depthTarget].dsvArray[slice], D3D11_CLEAR_DEPTH, depth, 0);
	}
}


const D3D11_PRIMITIVE_TOPOLOGY d3dPrim[] = {
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, // Triangle fans not supported
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, // Quads not supported
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, // Line loops not supported
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
};

void RendererHLSLD3D11::drawArrays(const Primitives primitives, const int firstVertex, const int nVertices){
	if(D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED==d3dPrim[primitives]) return;
	ic->IASetPrimitiveTopology(d3dPrim[primitives]);
	ic->Draw(nVertices, firstVertex);
}

void RendererHLSLD3D11::drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices){
	if(D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED==d3dPrim[primitives]) return;
	ic->IASetPrimitiveTopology(d3dPrim[primitives]);
	ic->DrawIndexed(nIndices, firstIndex, 0);
}

void RendererHLSLD3D11::setup2DMode(const float left, const float right, const float top, const float bottom){
	scaleBias2D.x = 2.0f / (right - left);
	scaleBias2D.y = 2.0f / (top - bottom);
	scaleBias2D.z = -1.0f;
	scaleBias2D.w =  1.0f;
}

ID3D11ShaderResourceView *RendererHLSLD3D11::createSRV(ID3D11Resource *resource, DXGI_FORMAT format, const int firstSlice, const int sliceCount){
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ID3D11ShaderResourceView *srv;

	switch (type){
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			D3D11_TEXTURE1D_DESC desc1d;
			((ID3D11Texture1D *) resource)->GetDesc(&desc1d);

			srvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc1d.Format;
			if (desc1d.ArraySize > 1){

				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture1DArray.FirstArraySlice = 0;
				srvDesc.Texture1DArray.ArraySize = desc1d.ArraySize;
				srvDesc.Texture1DArray.MostDetailedMip = 0;
				srvDesc.Texture1DArray.MipLevels = desc1d.MipLevels;
			} else {
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				srvDesc.Texture1D.MostDetailedMip = 0;
				srvDesc.Texture1D.MipLevels = desc1d.MipLevels;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			D3D11_TEXTURE2D_DESC desc2d;
			((ID3D11Texture2D *) resource)->GetDesc(&desc2d);

			srvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc2d.Format;
			if (desc2d.ArraySize > 1){
				if (desc2d.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE){
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					srvDesc.TextureCube.MostDetailedMip = 0;
					srvDesc.TextureCube.MipLevels = desc2d.MipLevels;
				} else {
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					if (firstSlice < 0){
						srvDesc.Texture2DArray.FirstArraySlice = 0;
						srvDesc.Texture2DArray.ArraySize = desc2d.ArraySize;
					} else {
						srvDesc.Texture2DArray.FirstArraySlice = firstSlice;
						if (sliceCount < 0){
							srvDesc.Texture2DArray.ArraySize = 1;
						} else {
							srvDesc.Texture2DArray.ArraySize = sliceCount;
						}
					}
					srvDesc.Texture2DArray.MostDetailedMip = 0;
					srvDesc.Texture2DArray.MipLevels = desc2d.MipLevels;
				}
			} else {
				srvDesc.ViewDimension = (desc2d.SampleDesc.Count > 1)? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc2d.MipLevels;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			D3D11_TEXTURE3D_DESC desc3d;
			((ID3D11Texture3D *) resource)->GetDesc(&desc3d);

			srvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc3d.Format;

			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;

			srvDesc.Texture3D.MostDetailedMip = 0;
			srvDesc.Texture3D.MipLevels = desc3d.MipLevels;

			break;
		default:
			LOG_PRINT("Unsupported type");
			return 0;
	}

	if (FAILED(device->CreateShaderResourceView(resource, &srvDesc, &srv))){

		LOG_PRINT("CreateShaderResourceView failed");
		return 0;
	}

	return srv;
}

ID3D11RenderTargetView *RendererHLSLD3D11::createRTV(ID3D11Resource *resource, DXGI_FORMAT format, const int firstSlice, const int sliceCount){
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ID3D11RenderTargetView *rtv;

	switch (type){
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			D3D11_TEXTURE2D_DESC desc2d;
			((ID3D11Texture2D *) resource)->GetDesc(&desc2d);

			rtvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc2d.Format;
			if (desc2d.ArraySize > 1){
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				if (firstSlice < 0){
					rtvDesc.Texture2DArray.FirstArraySlice = 0;
					rtvDesc.Texture2DArray.ArraySize = desc2d.ArraySize;
				} else {
					rtvDesc.Texture2DArray.FirstArraySlice = firstSlice;
					if (sliceCount < 0){
						rtvDesc.Texture2DArray.ArraySize = 1;
					} else {
						rtvDesc.Texture2DArray.ArraySize = sliceCount;
					}
				}
				rtvDesc.Texture2DArray.MipSlice = 0;
			} else {
				rtvDesc.ViewDimension = (desc2d.SampleDesc.Count > 1)? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
				rtvDesc.Texture2D.MipSlice = 0;
			}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			D3D11_TEXTURE3D_DESC desc3d;
			((ID3D11Texture3D *) resource)->GetDesc(&desc3d);

			rtvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc3d.Format;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			if (firstSlice < 0){
				rtvDesc.Texture3D.FirstWSlice = 0;
				rtvDesc.Texture3D.WSize = desc3d.Depth;
			} else {
				rtvDesc.Texture3D.FirstWSlice = firstSlice;
				if (sliceCount < 0){
					rtvDesc.Texture3D.WSize = 1;
				} else {
					rtvDesc.Texture3D.WSize = sliceCount;
				}
			}
			rtvDesc.Texture3D.MipSlice = 0;
			break;
		default:
			LOG_PRINT("Unsupported type");
			return 0;
	}

	if (FAILED(device->CreateRenderTargetView(resource, &rtvDesc, &rtv))){
		LOG_PRINT("CreateRenderTargetView failed");
		return 0;
	}

	return rtv;
}

ID3D11DepthStencilView *RendererHLSLD3D11::createDSV(ID3D11Resource *resource, DXGI_FORMAT format, const int firstSlice, const int sliceCount){
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType(&type);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ID3D11DepthStencilView *dsv;

	switch (type){
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			D3D11_TEXTURE2D_DESC desc2d;
			((ID3D11Texture2D *) resource)->GetDesc(&desc2d);

			dsvDesc.Format = (format != DXGI_FORMAT_UNKNOWN)? format : desc2d.Format;
			if (desc2d.ArraySize > 1){
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				if (firstSlice < 0){
					dsvDesc.Texture2DArray.FirstArraySlice = 0;
					dsvDesc.Texture2DArray.ArraySize = desc2d.ArraySize;
					dsvDesc.Texture2DArray.MipSlice = 0;
				} else {
					dsvDesc.Texture2DArray.FirstArraySlice = firstSlice;
					if (sliceCount < 0){
						dsvDesc.Texture2DArray.ArraySize = 1;
					} else {
						dsvDesc.Texture2DArray.ArraySize = sliceCount;
					}
					dsvDesc.Texture2DArray.MipSlice = 0;
				}
			} else {
				dsvDesc.ViewDimension = (desc2d.SampleDesc.Count > 1)? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice = 0;
			}
			break;
		default:
			LOG_PRINT("Unsupported type");
			return 0;
	}

	if (FAILED(device->CreateDepthStencilView(resource, &dsvDesc, &dsv))){
		LOG_PRINT("CreateDepthStencilView failed");
		return 0;
	}

	return dsv;
}
#if 0
ubyte *RendererHLSLD3D11::mapRollingVB(const uint size){
	ASSERT(size <= ROLLING_VB_SIZE);

	if (rollingVB == VB_NONE) rollingVB = addVertexBuffer(ROLLING_VB_SIZE, DEFAULT);

	ubyte *data = 0;
	D3D11_MAP flag = D3D11_MAP_WRITE_NO_OVERWRITE;
	if (rollingVBOffset + size > ROLLING_VB_SIZE){
		flag = D3D11_MAP_WRITE_DISCARD;
		rollingVBOffset = 0;
	}

	vertexBuffers[rollingVB].vertexBuffer->Map(flag, 0, (void **) &data);

	return data + rollingVBOffset;
}

void RendererHLSLD3D11::unmapRollingVB(const uint size){
	vertexBuffers[rollingVB].vertexBuffer->Unmap();

	rollingVBOffset += size;
}

uint RendererHLSLD3D11::copyToRollingVB(const void *src, const uint size){
	ASSERT(size <= ROLLING_VB_SIZE);

	if (rollingVB == VB_NONE) rollingVB = addVertexBuffer(ROLLING_VB_SIZE, DYNAMIC);

	ubyte *data = 0;
	D3D11_MAP flag = D3D11_MAP_WRITE_NO_OVERWRITE;
	if (rollingVBOffset + size > ROLLING_VB_SIZE){
		flag = D3D11_MAP_WRITE_DISCARD;
		rollingVBOffset = 0;
	}

	uint offset = rollingVBOffset;
	vertexBuffers[rollingVB].vertexBuffer->Map(flag, 0, (void **) &data);
		memcpy(data + offset, src, size);
	vertexBuffers[rollingVB].vertexBuffer->Unmap();
/*
	D3D11_BOX box;
	box.left   = offset;
	box.right  = offset + size;
	box.top    = 0;
	box.bottom = 1;
	box.front  = 0;
	box.back   = 1;

	ic->UpdateSubresource(vertexBuffers[rollingVB].vertexBuffer, 0, &box, src, 0, 0);
*/
	rollingVBOffset += size;

	return offset;
}
#endif
ID3D11Resource *RendererHLSLD3D11::getResource(const TextureID texture) const {
	return textures[texture].texture;
}

void RendererHLSLD3D11::flush(){
	ic->Flush();
}

void RendererHLSLD3D11::finish(){
	#if 0
	if (eventQuery == 0){
		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_EVENT;
		desc.MiscFlags = 0;
		device->CreateQuery(&desc, &eventQuery);
	}

	eventQuery->End();

	ic->Flush();

	BOOL result = FALSE;
	do {
		eventQuery->GetData(&result, sizeof(BOOL), 0);
	} while (!result);
#endif
}











UINT32 RendererHLSLD3D11::GetAvailableTextureMem( )
{
	UINT32 AvailableVideoMem=0;
    // Get availble texture memory
    IDXGIDevice* pDXGIDevice = 0;
    IDXGIAdapter* pDXGIAdapter = 0;
    dev->QueryInterface( IID_IDXGIDevice, ( void** )&pDXGIDevice ) ;
    pDXGIDevice->GetAdapter( &pDXGIAdapter ) ;
    DXGI_ADAPTER_DESC AdapterDesc;
    pDXGIAdapter->GetDesc( &AdapterDesc );
    if( AdapterDesc.DedicatedVideoMemory )
	AvailableVideoMem = AdapterDesc.DedicatedVideoMemory;
    else
	AvailableVideoMem = AdapterDesc.SharedSystemMemory;
    SAFE_RELEASE( pDXGIDevice );
    SAFE_RELEASE( pDXGIAdapter );

	return AvailableVideoMem;
}

			int RendererHLSLD3D11::deleteVertexBuffer(const VertexBufferID vertexBuffer)
			{
				if(vertexBuffer>=vertexBuffers.size()) return -1;
				if ((vertexBuffers[vertexBuffer]).vertexBuffer)
				{
					(vertexBuffers[vertexBuffer]).vertexBuffer->Release();
					(vertexBuffers[vertexBuffer]).vertexBuffer = 0;
				}
	int r=0;
	for(unsigned int i=0;i<vertexBuffers.size();i++)
	{
		if(vertexBuffers[i].vertexBuffer) r++;
	}
	return r;

			}
			int RendererHLSLD3D11::deleteIndexBuffer(const IndexBufferID indexBuffer)
			{
					if(indexBuffer>=indexBuffers.size()) return -1;
				if ((indexBuffers[indexBuffer]).indexBuffer)
				{
					(indexBuffers[indexBuffer]).indexBuffer->Release();
					(indexBuffers[indexBuffer]).indexBuffer = 0;
				}
					int r=0;
	for(unsigned int i=0;i<indexBuffers.size();i++)
	{
		if(indexBuffers[i].indexBuffer) r++;
	}
	return r;

			}
int RendererHLSLD3D11::texturePitchVrtl(const TextureID id,int level)
{
	D3D10_MAPPED_TEXTURE2D m_pLockedRects11;
	((ID3D10Texture2D*)textures[id].texture)->Map( level,
		//D3D11_MAP_WRITE,
		D3D10_MAP_WRITE_DISCARD,
		0, &m_pLockedRects11 );
    return m_pLockedRects11.RowPitch;
}
BYTE *RendererHLSLD3D11::lockTexture(const TextureID id,int level, int& pitch)
{
	D3D10_MAPPED_TEXTURE2D m_pLockedRects11;
	/*
//???D3D11: ERROR: ID3D11Texture2D::Map: Map cannot be called with
	MAP_WRITE access, because the Resource was not created with the
		D3D11_CPU_ACCESS_WRITE flag.
		[ RESOURCE_MANIPULATION ERROR #307: TEXTURE2D_MAP_INVALIDMAPTYPE ]
	*/
	((ID3D10Texture2D*)textures[id].texture)->Map( level,
		//D3D11_MAP_WRITE,
		D3D10_MAP_WRITE_DISCARD,
		0, &m_pLockedRects11 );
	pitch=m_pLockedRects11.RowPitch;
	BYTE *b= (BYTE *)m_pLockedRects11.pData;
	return b;
}
BYTE *RendererHLSLD3D11::unlockTextureVrtl(const TextureID id,int level)
{
	((ID3D10Texture2D*)textures[id].texture)->Unmap( level );
	return 0;
}
BYTE *RendererHLSLD3D11::unlockVertexBuffer(const VertexBufferID id)
{
	return 0;
}
BYTE *RendererHLSLD3D11::unlockIndexBuffer(const IndexBufferID id)
{
	return 0;
}
BYTE *RendererHLSLD3D11::fillVertexBufferVrtl(const VertexBufferID id, BYTE*  m_pData, int l)
{
        ic->UpdateSubresource( vertexBuffers[id].vertexBuffer, 0, 0, m_pData, 0, 0 );
	return 0;
}
BYTE *RendererHLSLD3D11::fillIndexBufferVrtl(const IndexBufferID id, BYTE*  m_pData, int l)
{
        ic->UpdateSubresource( indexBuffers[id].indexBuffer, 0, 0, m_pData, 0, 0 );
	return 0;
}
BYTE *RendererHLSLD3D11::lockVertexBuffer(const VertexBufferID id){return 0;}
BYTE *RendererHLSLD3D11::lockIndexBuffer(const IndexBufferID id){return 0;}

void RendererHLSLD3D11::viewport(int x, int y, int viewportWidth, int viewportHeight){
        D3D11_VIEWPORT vp = { x,y, viewportWidth, viewportHeight, 0.0f, 1.0f };
        ic->RSSetViewports(1, &vp);
  m_viewport.X=x;
  m_viewport.Y=y;
  m_viewport.Width=viewportWidth;
  m_viewport.Height=viewportHeight;

}
AlphaStateID RendererHLSLD3D11::addAlphaState(
		const int func,
		const float ref
	)
{
	AlphaStateD3D11 blendState;
    blendState.blendEnable = true;
	blendState.alphaFunc=GEQUAL;
	alphaStates.push_back(blendState);
	return alphaStates.size()-1;
}



    BYTE* RendererHLSLD3D11::LockTexture(TextureID id, int m, int& pitch)
    {
		return textures[id].Lock(m,pitch);
	}
    void RendererHLSLD3D11::UnlockTexture(TextureID id, int m)
    {
		textures[id].Unlock(m);
	}
    BYTE* RendererHLSLD3D11::LockVertexBuffer(VertexBufferID id)
    {
		return vertexBuffers[id].Lock();
	}
    void RendererHLSLD3D11::UnLockVertexBuffer(VertexBufferID id)
    {
		vertexBuffers[id].Unlock();
	}
    BYTE* RendererHLSLD3D11::LockIndexBuffer(IndexBufferID id)
    {
		return indexBuffers[id].Lock();
	}
    void RendererHLSLD3D11::UnLockIndexBuffer(IndexBufferID id)
    {
		indexBuffers[id].Unlock();
	}

	void RendererHLSLD3D11::GetLevelDesc(TextureID id, unsigned int level,SURFACE_DESC* sDesc)
	{
		sDesc->Width=textures[id].width;
		sDesc->Height=textures[id].height;
		int i=0;
		while(i<level)
		{
		sDesc->Width = sDesc->Width >> 1;
        sDesc->Height = sDesc->Height >> 1;
		i++;
		}
	}
	void RendererHLSLD3D11::GetDesc(VertexBufferID id,VERTEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=vertexBuffers[id].size;
	}
	void RendererHLSLD3D11::GetDesc(IndexBufferID id,INDEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=indexBuffers[id].nIndices*indexBuffers[id].indexSize;
		sDesc->Format=indexBuffers[id].indexSize;
	}
	unsigned int RendererHLSLD3D11::GetLevelCount(TextureID id)
	{
				return textures[id].mipMapLevels;
	}
float RendererHLSLD3D11::getTextWidth(const FontID font, const char *str, int length) const {
	if (font < 0) return 0;
    if (length < 0) length = (int) stx_strlen(str);

	Character *chars = fonts[font].chars;

	float len = 0;
	for (int i = 0; i < length; i++){
		len += chars[(unsigned char) str[i]].ratio;
	}

	return len;
}

	void RendererHLSLD3D11::captureScreenshot(Image3& img)
	{
		#if 0
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = IRenderer::GetRendererInstance()->GetViewportWidth();
		desc.Height = IRenderer::GetRendererInstance()->GetViewportHeight();
		desc.Format = backBufferFormat;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = 0;
		desc.MipLevels = 1;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bool result = false;
		ID3D11Texture2D *texture;
		if (SUCCEEDED(m_pd3dDevice11->CreateTexture2D(&desc, 0, &texture)))
		{
			if (msaaSamples > 1)
			{
				ID3D11Texture2D *resolved = 0;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.CPUAccessFlags = 0;
				if (SUCCEEDED(m_pd3dDevice11->CreateTexture2D(&desc, 0, &resolved)))
				{
					ic->ResolveSubresource(resolved, 0, m_pBackBuffer, 0, desc.Format);
					ic->CopyResource(texture, resolved);
					resolved->Release();
				}
			}
			else
			{
				ic->CopyResource(texture, m_pBackBuffer);
			}
			D3D10_MAPPED_TEXTURE2D map;
			if (SUCCEEDED(texture->Map(0, D3D10_MAP_READ, 0, &map)))
			{
				/*
				if (backBufferFormat == DXGI_FORMAT_R11G11B11A2_UNORM)
				{
					uint32 *dst = (uint32 *) img.create(FORMAT_RGB11A2, IRenderer::GetRendererInstance()->GetViewportWidth(), IRenderer::GetRendererInstance()->GetViewportHeight(), 1, 1);
					ubyte *src = (ubyte *) map.pData;
					for (int y = 0; y < IRenderer::GetRendererInstance()->GetViewportHeight(); y++)
					{
						for (int x = 0; x < IRenderer::GetRendererInstance()->GetViewportWidth(); x++)
						{
							dst[x] = ((uint32 *) src)[x] | 0xC0000000;
						}
						dst += IRenderer::GetRendererInstance()->GetViewportWidth();
						src += map.RowPitch;
					}
				}
				else*/
				{
					ubyte *dst = img.create(FORMAT_RGB8, IRenderer::GetRendererInstance()->GetViewportWidth(), IRenderer::GetRendererInstance()->GetViewportHeight(), 1, 1);
					ubyte *src = (ubyte *) map.pData;
					for (int y = 0; y < IRenderer::GetRendererInstance()->GetViewportHeight(); y++)
					{
						for (int x = 0; x < IRenderer::GetRendererInstance()->GetViewportWidth(); x++)
						{
							dst[3 * x + 0] = src[4 * x + 0];
							dst[3 * x + 1] = src[4 * x + 1];
							dst[3 * x + 2] = src[4 * x + 2];
						}
						dst += IRenderer::GetRendererInstance()->GetViewportWidth() * 3;
						src += map.RowPitch;
					}
				}
				result = true;
				texture->Unmap(0);
			}
			texture->Release();
		}
		#endif
	}
	void RendererHLSLD3D11::SaveScreenShot3(const char* fn)
	{
		Image3 img;
		captureScreenshot(img);
/*
		ID3D11Resource* id=0;
		D3DX11SaveTextureToFileA(
		id,
		D3DX11_IFF_JPG,
		fn
		);
*/
		int l=0;
        l=int(stx_strrchr(fn,'.')-fn);

		std::string fns(fn);
		if(l)fns.resize(l);
					char bufpng[256];
                    stx_snprintf(bufpng,256,"%s.png",fns.c_str());
					img.saveImageLibImage(bufpng,false);

	}
	void RendererHLSLD3D11::SaveScreenShotVrtl(const char* fn)
	{
		if(fn==0)return;

		{
			Image3 img;
			char buf[256];
			char gifbuf[256];
			if(fn!=0)
			{
                stx_snprintf(buf,256,"../%s.jpg",fn);
                stx_snprintf(gifbuf,256,"../%s.gif",fn);

			}
			else
			{
                stx_snprintf(buf,256,"../%s.jpg","ScreenShot.jpg");
                stx_snprintf(gifbuf,256,"../%s.jpg","ScreenShot.gif");
			}

			captureScreenshot(img);
			//img.flipX();
			img.saveImageLibImage(buf,false);
			RendererHLSLCg::OpenILToGIF(img,gifbuf);
		}
	}

void RendererHLSLD3D11::buildPerspectiveMatrix(float fieldOfView,
                                   float aspectRatio,
                                   float zNear, float zFar)//,D3DXFROMWINEMATRIX& m)
{
buildPerspectiveMatrixD3D(fieldOfView,
                                   aspectRatio,
                                   zNear, zFar,(*(D3DXFROMWINEMATRIX*)pp));
	m_fieldOfView=fieldOfView;
    m_aspectRatio=aspectRatio;
    m_zNear=zNear;
    m_zFar=zFar;
//	p=m;

}
        void RendererHLSLD3D11::buildLookAtMatrix(float eyex, float eyey, float eyez,
                              float centerx, float centery, float centerz,
                              float upx, float upy, float upz)//,D3DXFROMWINEMATRIX& m)
        {
#if 1
buildLookAtMatrixD3D(eyex, eyey, eyez,
                              centerx, centery, centerz,
                              upx, upy, upz,(*(D3DXFROMWINEMATRIX*)vv));
#else
	D3DXFROMWINEVECTOR3 c1=D3DXFROMWINEVECTOR3(centerx-eyex,centery-eyey,centerz-eyez);

		mv=identity4();

if(_isinf(c1.x)) return;
if(_isinf(c1.y)) return;
if(_isinf(c1.z)) return;
	float radius=0.0f; float phi=0.0f; float theta=0.0f;
	float f=c1.x*c1.x+c1.y*c1.y+c1.z*c1.z;
	if(f<=0.0000001f) return;
	radius=sqrt(f);
	phi = asinf(c1.z / (radius));
	if(0.0<=c1.y)
		theta = acosf(c1.x / ((radius)* cosf((phi))));
	else if(c1.y<0.0f)
		theta = -acosf(c1.x /((radius)*cosf((phi))));
	mv=
	rotateXY(phi,theta)
	*translate(-D3DXFROMWINEVECTOR3(eyex, eyey, eyez));
#endif
	m_eye=D3DXFROMWINEVECTOR3(eyex, eyey, eyez);
	m_center=D3DXFROMWINEVECTOR3(centerx, centery, centerz);
    m_up=D3DXFROMWINEVECTOR3(upx, upy, upz);
//mv=m;
        }

int RendererHLSLD3D11::GetTextureWidth(TextureID id)
{
	assert(id<textures.size());
	return textures[id].width;
}

int RendererHLSLD3D11::GetTextureHeight(TextureID id)
{
	assert(id<textures.size());
	return textures[id].height;
}

void RendererHLSLD3D11::PresentVrtl(bool aB)
{
	bool b=true;//((aB&&(m_pInput!=0)) || ((!aB)&&(m_pInput==0)));
	if(b)
	{

	IRenderer::OnPresent();

	#if !defined(WIN_PHONE_APP)
	m_pSwapChain->Present( 0, 0 );
	#endif
}
}

	void RendererHLSLD3D11::DrawPrimitive(Primitives PrimitiveType,UINT StartVertex,UINT PrimitiveCount, const uint flags)
	{
		apply();
		drawArrays(		PrimitiveType,
						StartVertex,
						getVertexCount(PrimitiveType,PrimitiveCount));
	resetVrtl(RESET_ALL);
	nDrawCalls++;
	}
	void RendererHLSLD3D11::DrawIndexedPrimitive(Primitives PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount, const uint flags)
	{
		apply();
		drawElements(	PrimitiveType,
						startIndex,
						getIndicesCount(PrimitiveType)*primCount,
						0,
						NumVertices);
	resetVrtl(RESET_ALL);
	nDrawCalls++;
	}

int RendererHLSLD3D11::getSamplerUnit(const ShaderID shader, const char *samplerName) const
{
	if(!shaders[selectedShader].nSamplers) return -1;
	const SamplerD3D11 *samplers=&(shaders[selectedShader].samplers[0]);
	const int count=(shaders[selectedShader]).nSamplers;
	const char *name=samplerName;
	int minSampler = 0;
	int maxSampler = count - 1;
	int currSampler = 0;
	while (minSampler <= maxSampler)
	{
		currSampler = (minSampler + maxSampler) >> 1;
		int res = strcmp(name, samplers[currSampler].name.c_str());
		if (res == 0)
		{
			break;
		}
		else if (res > 0)
		{
			minSampler = currSampler + 1;
		}
		else
		{
			maxSampler = currSampler - 1;
		}
	}
	if (minSampler > maxSampler) currSampler =0;
	return currSampler;
}

TextureID RendererHLSLD3D11::addTexture(Image3* img, const bool useMips, const SamplerStateID samplerState, uint flags, float3 alpha)
{
	TextureD3D11 tex;
	memset(&tex, 0, sizeof(TextureD3D11));
	img->BindImage();
	tex.ss=samplerState;
	tex.width=img->getWidth();
	tex.height=img->getHeight();
	tex.depth=img->getDepth();
	if(useMips&&img->getMipMapCount()<2)
		img->createMipMaps();
	tex.mipMapLevels=img->getMipMapCount();
#if 1


	switch (img->getFormat()){
		case FORMAT_RGB8:
			img->convert(FORMAT_RGBA8);
//			img->convert(FORMAT_RGB10A2);
			break;
		case FORMAT_RGB16:
			img->convert(FORMAT_RGBA16);
			break;
		case FORMAT_RGB16F:
			img->convert(FORMAT_RGBA16F);
			break;
		case FORMAT_RGB32F:
			img->convert(FORMAT_RGBA32F);
			break;
	}

	FORMAT format = img->getFormat();
	uint nMipMaps = img->getMipMapCount();
	uint nSlices = img->isCube()? 6 : 1;
	uint arraySize = img->getArraySize();

	static D3D11_SUBRESOURCE_DATA texData[1024];
	D3D11_SUBRESOURCE_DATA *dest = texData;
	//for (uint n = 0; n < arraySize; n++){
		for (uint k = 0; k < nSlices; k++){
			for (uint i = 0; i < nMipMaps; i++){
				uint pitch, slicePitch;
				if (isCompressedFormat(format)){
					pitch = ((img->getWidth(i) + 3) >> 2) * getBytesPerBlock(format);
					slicePitch = pitch * ((img->getHeight(i) + 3) >> 2);
				} else {
					pitch = img->getWidth(i) * getBytesPerPixel(format);
					slicePitch = pitch * img->getHeight(i);
				}

				dest->pSysMem = img->getPixels(i/*, n*/) + k * slicePitch;
				dest->SysMemPitch = pitch;
				dest->SysMemSlicePitch = slicePitch;
				dest++;
			}
		}
	//}

	tex.texFormat = formatsd3d11[format];
	if (flags & SRGB){
		// Change to the matching sRGB format
		switch (tex.texFormat){
			case DXGI_FORMAT_R8G8B8A8_UNORM: 
				tex.texFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; 
				break;
			case DXGI_FORMAT_BC1_UNORM: 
				tex.texFormat = DXGI_FORMAT_BC1_UNORM_SRGB; 
				break;
			case DXGI_FORMAT_BC2_UNORM: 
				tex.texFormat = DXGI_FORMAT_BC2_UNORM_SRGB; 
				break;
			case DXGI_FORMAT_BC3_UNORM: 
				tex.texFormat = DXGI_FORMAT_BC3_UNORM_SRGB; 
				break;
		}
	}

	HRESULT hr;
	if (img->is1D()){
		D3D11_TEXTURE1D_DESC desc;
		desc.Width  = img->getWidth();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.ArraySize = 1;
		desc.MiscFlags = 0;

		hr = device->CreateTexture1D(&desc, texData, (ID3D11Texture1D **) &tex.texture);
	} else if (img->is2D() || img->isCube()){
		D3D11_TEXTURE2D_DESC desc;
		desc.Width  = img->getWidth();
		desc.Height = img->getHeight();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		if (img->isCube()){
			desc.ArraySize = 6 * arraySize;
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		} else {
			desc.ArraySize = 1;
			desc.MiscFlags = 0;
		}

		hr = device->CreateTexture2D(&desc, texData, (ID3D11Texture2D **) &tex.texture);
	} else if (img->is3D()){
		D3D11_TEXTURE3D_DESC desc;
		desc.Width  = img->getWidth();
		desc.Height = img->getHeight();
		desc.Depth  = img->getDepth();
		desc.Format = tex.texFormat;
		desc.MipLevels = nMipMaps;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = device->CreateTexture3D(&desc, texData, (ID3D11Texture3D **) &tex.texture);
	}

	if (FAILED(hr)){
		LOG_PRINT("Couldn't create texture");
		return TEXTURE_NONE;
	}
#else
				switch (img->getFormat())
				{
					case FORMAT_RGB8:
					img->convert(FORMAT_RGBA8);
					break;
					case FORMAT_RGB16:
					img->convert(FORMAT_RGBA16);
					break;
					case FORMAT_RGB16f:
					img->convert(FORMAT_RGBA16f);
					break;
					case FORMAT_RGB32f:
					img->convert(FORMAT_RGBA32f);
					break;
				}
				const FORMAT format = img->getFormat();
							if(alpha>0.0f)
					{
#if 1
						if(getChannelCount(format)==4){img->removeChannels(true,true,true,false);}
						img->addChannel(alpha);
#else
						img->format(FORMAT_RGBA8);
						img->setAlpha(alpha);
						format = img->getFormat();
#endif

					}
				////img->swapChannels2(0, 2);
				//img->swapChannels(0, 2);
	//if (format == FORMAT_RGB8 || format == FORMAT_RGBA8) img->swap(0, 2);

				uint nMipMaps = img->getMipMapCount();
				uint nSlices = img->isCube()? 6 : 1;
				uint arraySize = img->getArraySize();
				static D3D11_SUBRESOURCE_DATA texData[1024];
				D3D11_SUBRESOURCE_DATA *dest = texData;
				for (uint n = 0; n < arraySize; n++)
				{
					for (uint k = 0; k < nSlices; k++)
					{
						for (uint i = 0; i < nMipMaps; i++)
						{
							uint pitch, slicePitch;
							if (isCompressedFormat(format))
							{
								pitch = ((img->getWidth(i) + 3) >> 2) * getBytesPerBlock(format);
								slicePitch = pitch * ((img->getHeight(i) + 3) >> 2);
							}
							else
							{
								pitch = img->getWidth(i) * getBytesPerPixel(format);
								slicePitch = pitch * img->getHeight(i);
							}
							dest->pSysMem = img->getPixels(i) + k * slicePitch;
							dest->SysMemPitch = pitch;
							dest->SysMemSlicePitch = slicePitch;
							dest++;
						}
					}
				}
				tex.texFormat = formatsd3d11[format];
				if (flags & SRGB)
				{
					switch (tex.texFormat)
					{
						case DXGI_FORMAT_R8G8B8A8_UNORM: tex.texFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; break;
						case DXGI_FORMAT_BC1_UNORM: tex.texFormat = DXGI_FORMAT_BC1_UNORM_SRGB; break;
						case DXGI_FORMAT_BC2_UNORM: tex.texFormat = DXGI_FORMAT_BC2_UNORM_SRGB; break;
						case DXGI_FORMAT_BC3_UNORM: tex.texFormat = DXGI_FORMAT_BC3_UNORM_SRGB; break;
					}
				}
				HRESULT hr;
				if (img->is1D())
				{
					tex.dim=1;
					D3D11_TEXTURE1D_DESC desc;
					desc.Width  = img->getWidth();
					desc.Format = tex.texFormat;
					desc.MipLevels = (flags&DYNAMIC)?1:nMipMaps;
					desc.Usage = (flags&DYNAMIC)?

    //D3D10_USAGE_DEFAULT
	//D3D10_USAGE_IMMUTABLE
	D3D11_USAGE_DYNAMIC
	//D3D10_USAGE_STAGING

:D3D11_USAGE_DEFAULT;//D3D10_USAGE_IMMUTABLE;
					desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//					desc.CPUAccessFlags = 0;
desc.CPUAccessFlags = (flags&DYNAMIC)? D3D11_CPU_ACCESS_WRITE : 0;


					desc.ArraySize = 1;
					desc.MiscFlags = 0;
if (flags==STAGING)
{
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            hr = dev->CreateTexture1D( &desc, 0, (ID3D11Texture1D **)&tex.pStaging );
}
					hr = device->CreateTexture1D(&desc, texData, (ID3D11Texture1D **) &tex.texture);
				}
				else if (img->is2D() || img->isCube())
				{
					tex.dim=2;
					D3D11_TEXTURE2D_DESC desc;
					desc.Width  = img->getWidth();
					desc.Height = img->getHeight();
					desc.Format = tex.texFormat;
					desc.MipLevels = (flags&DYNAMIC)?1:nMipMaps;
					desc.SampleDesc.Count = 1;
					desc.SampleDesc.Quality = 0;
					desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					desc.Usage = (flags&DYNAMIC)?

    //D3D10_USAGE_DEFAULT
	//D3D10_USAGE_IMMUTABLE
	D3D11_USAGE_DYNAMIC
	//D3D10_USAGE_STAGING

:D3D11_USAGE_DEFAULT;//D3D10_USAGE_IMMUTABLE;
//					desc.CPUAccessFlags = 0;
desc.CPUAccessFlags = (flags&DYNAMIC)? D3D11_CPU_ACCESS_WRITE : 0;

					if (img->isCube())
					{
						desc.ArraySize = 6 * arraySize;
						desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
					}
					else
					{
						desc.ArraySize = 1;
						desc.MiscFlags = 0;
					}


if (flags==STAGING)
{
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            hr = dev->CreateTexture2D( &desc, 0, (ID3D11Texture2D **)&tex.pStaging );
}

					hr = device->CreateTexture2D(&desc, texData, (ID3D11Texture2D **) &tex.texture);
				}
				else if (img->is3D())
				{
					tex.dim=3;
					D3D11_TEXTURE3D_DESC desc;
					desc.Width  = img->getWidth();
					desc.Height = img->getHeight();
					desc.Depth  = img->getDepth();
					desc.Format = tex.texFormat;
					desc.MipLevels = (flags&DYNAMIC)?1:nMipMaps;
					desc.Usage = (flags&DYNAMIC)?

    //D3D10_USAGE_DEFAULT
	//D3D10_USAGE_IMMUTABLE
	D3D11_USAGE_DYNAMIC
	//D3D10_USAGE_STAGING

:D3D11_USAGE_DEFAULT;//D3D10_USAGE_IMMUTABLE;
					desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//					desc.CPUAccessFlags = 0;
desc.CPUAccessFlags = (flags&DYNAMIC)? D3D11_CPU_ACCESS_WRITE : 0;
					desc.MiscFlags = 0;
if (flags==STAGING)
{
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;//0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            hr = dev->CreateTexture3D( &desc, 0, (ID3D11Texture3D **)&tex.pStaging );
}
					hr = device->CreateTexture3D(&desc, texData, (ID3D11Texture3D **) &tex.texture);
				}
				if (FAILED(hr))
				{
/*
(1508): RESOURCE WARNING: Device needs to create new Texture
D3D10: ERROR: ID3D10Device::CreateTexture2D: A D3D10_USAGE_IMMUTABLE Resource cannot have any CPUAccessFlags set. The following CPUAccessFlags bits cannot be set in this case: D3D10_CPU_ACCESS_READ (0), D3D10_CPU_ACCESS_WRITE (1). [ STATE_CREATION ERROR #98: CREATETEXTURE2D_INVALIDCPUACCESSFLAGS ]
First-chance exception at 0x76f142eb in ContentStreaming9-debug.exe: Microsoft C++ exception: _com_error at memory location 0x0012e9b4..
D3D10: ERROR: ID3D10Device::CreateTexture2D: Returning E_INVALIDARG, meaning invalid parameters were passed. [ STATE_CREATION ERROR #104: CREATETEXTURE2D_INVALIDARG_RETURN ]
(1508): Couldn't create textureFirst-chance exception at 0x0086326f in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.
Unhandled exception at 0x0086326f in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.

D3D10: ERROR: ID3D10Device::CreateTexture2D: A D3D10_USAGE_DYNAMIC Resource must have MipLevels equal to 1. [ STATE_CREATION ERROR #102: CREATETEXTURE2D_INVALIDMIPLEVELS ]
First-chance exception at 0x76f142eb in ContentStreaming9-debug.exe: Microsoft C++ exception: _com_error at memory location 0x0012e9b4..
D3D10: ERROR: ID3D10Device::CreateTexture2D: Returning E_INVALIDARG, meaning invalid parameters were passed. [ STATE_CREATION ERROR #104: CREATETEXTURE2D_INVALIDARG_RETURN ]
(932): Couldn't create textureFirst-chance exception at 0x0086329b in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.
Unhandled exception at 0x0086329b in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.

*/
					/*
D3D10: ERROR: ID3D10Device::CreateShaderResourceView1: A ShaderResourceView cannot be created of a Resource that did not specify the D3D10_BIND_SHADER_RESOURCE BindFlag. [ STATE_CREATION ERROR #129: CREATESHADERRESOURCEVIEW_INVALIDRESOURCE ]
First-chance exception at 0x77c742eb in ContentStreaming9-debug.exe: Microsoft C++ exception: _com_error at memory location 0x0012e92c..
D3D10: ERROR: ID3D10Device::CreateShaderResourceView1: Returning E_INVALIDARG, meaning invalid parameters were passed. [ STATE_CREATION ERROR #131: CREATESHADERRESOURCEVIEW_INVALIDARG_RETURN ]
(3332): CreateShaderResourceView failedFirst-chance exception at 0x00882b83 in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.
Unhandled exception at 0x00882b83 in ContentStreaming9-debug.exe: 0xC0000005: Access violation reading location 0xffffffff.

					*/
					LOG_PRINT("Couldn't create texture");
					return TEXTURE_NONE;
				}
#endif
				tex.srvFormat = tex.texFormat;
				tex.srv = createSRV(tex.texture, tex.srvFormat);
				return textures.add(tex);
}

	void RendererHLSLD3D11::Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors)
	{
		if(color)
		{
			float ClearColor[4] =
			{
				colors.x,colors.y,colors.z,colors.w
			}
			;
			ic->ClearRenderTargetView( RTV, ClearColor );
		}
		if(depth) ic->ClearDepthStencilView( DSV, D3D11_CLEAR_DEPTH, 1.0, 0 );
	}

    void RendererHLSLD3D11::changeAlphaState(const AlphaStateID blendState)
    {
    }
void RendererHLSLD3D11::setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState, const int vtstream, const VertexBufferID vtbuffer)
{
	if(texture==-1) DBG_HALT;
	if(selectedShader == SHADER_NONE) return;
	if(!shaders[selectedShader].nTextures) return;
	const SamplerD3D11* s = (const SamplerD3D11 *)getSampler(shaders[selectedShader].textures.getArray(), (shaders[selectedShader]).nTextures, textureName);
	if (s)
	{
		if (s->vsIndex >= 0)
		{
			selectedTexturesVS[s->vsIndex] = texture;
			selectedTextureSlicesVS[s->vsIndex] = NO_SLICE;
			selectedSamplerStatesVS[s->vsIndex] = samplerState;
		}
		/*
		if (s->gsIndex >= 0)
		{
			selectedTexturesGS[s->gsIndex] = texture;
			selectedTextureSlicesGS[s->gsIndex] = NO_SLICE;
			selectedSamplerStatesGS[s->gsIndex] = samplerState;
		}
		*/
		if (s->psIndex >= 0)
		{
			selectedTexturesPS[s->psIndex] = texture;
			selectedTextureSlicesPS[s->psIndex] = NO_SLICE;
			selectedSamplerStatesPS[s->psIndex] = samplerState;
		}

	}
	#ifdef _DEBUG
	else
	{
		char str[256];
        stx_snprintf(str,256, "Invalid texture \"%s\"", textureName);
		LOG_PRINT("%s", str);
	}
	#endif
	//setSamplerState(textureName, samplerState);
}
#endif


