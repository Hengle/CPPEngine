//----------------------------------------------------------------------------------
// File:   VolumeRenderer.cpp
// Author: Ignacio Llamas and Chris (Wei-Tae) Kim and Sarah Tariq 
// Email:  sdkfeedback@nvidia.com
// 
// Copyright (c) 2007 NVIDIA_Corporation. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------

#include "nvutPCH.h"
#include "Smoke.h"
#include "SDKmisc.h"
#include "VolumeRenderer.h"
#pragma warning(disable:4995)
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;


struct VsInput
{
    D3DXVECTOR3 pos;  
};

// constructor
VolumeRenderer::VolumeRenderer(  ID3D10Device* pd3dDevice ) : 
    m_pD3DDevice(NULL), m_maxDim(0), m_pEffect(NULL), m_pTechnique(NULL),
    m_evTexture_rayData(NULL), m_evTexture_rayDataSmall(NULL), m_evTexture_volume(NULL),
    m_evTexture_rayCast(NULL), m_evTexture_edge(NULL), m_evTexture_glow(NULL), m_evTexture_sceneDepthTex(NULL),
    m_evRTWidth(NULL), m_evRTHeight(NULL), m_evUseGlow(NULL),
    m_evWorldViewProjectionMatrix(NULL), m_evInvWorldViewProjectionMatrix(NULL), m_evGrid2WorldMatrix(NULL),
    m_evZNear(NULL), m_evZFar(NULL), m_evGridScaleFactor(NULL), m_evEyeOnGrid(NULL),
    m_evGlowContribution(NULL), m_evFinalIntensityScale(NULL), m_evFinalAlphaScale(NULL), 
    m_evSmokeColorMultiplier(NULL), m_evsmokeAlphaMultiplier(NULL), m_evRednessFactor(NULL),
    m_evWorldViewMatrix(NULL), m_evTan_FovYhalf(NULL), m_evTan_FovXhalf(NULL),
    m_evRaycastBisection(NULL), m_evRaycastFilterTricubic(NULL), m_evRaycastShaderAsWater(NULL),
    m_epQuadRaycastLevelSet(NULL), m_epQuadRaycastFire(NULL), m_epQuadRaycastSmoke(NULL), 
    m_pGridBoxLayout(NULL), m_pGridBoxVertexBuffer(NULL), m_pGridBoxIndexBuffer(NULL), 
    m_pQuadLayout(NULL), m_pQuadVertexBuffer(NULL), 
    m_pRayDataSmallTex2D(NULL), m_pRayDataSmallRTV(NULL),m_pRayDataSmallSRV(NULL), m_pGlowTex2D(NULL), m_pGlowTempTex2D(NULL),m_pGlowSRV(NULL),m_pGlowTempSRV(NULL),
    m_pRayCastTex2D(NULL), m_pRayCastRTV(NULL), m_pRayCastSRV(NULL),
    m_pEdgeTex2D(NULL), m_pEdgeSRV(NULL), m_pEdgeRTV(NULL), m_pGlowRTV(NULL), m_pGlowTempRTV(NULL),
    m_bRaycastBisection(true), m_bRaycastFilterTricubic(true), m_bRaycastShadeAsWater(true), m_eRenderMode(RM_SMOKE),
    m_renderTextureWidth(0), m_renderTextureHeight(0), m_useFP32Blending(true)
{
    SAFE_ACQUIRE(m_pD3DDevice, pd3dDevice);
    
    stx_memset(m_gridDim,0, sizeof(m_gridDim));
    stx_memset(pRayDataTex2D, 0, sizeof(pRayDataTex2D));
    stx_memset(m_pRayDataRTV, 0, sizeof(m_pRayDataRTV));
    stx_memset(m_pRayDataSRV, 0, sizeof(m_pRayDataSRV));

    D3DXMatrixIdentity(&m_gridMatrix);
}

// destructor
VolumeRenderer::~VolumeRenderer( void )
{ 
    SAFE_RELEASE(m_pD3DDevice);

    Cleanup();
}

void VolumeRenderer::Cleanup( void )
{
    
    SAFE_RELEASE(m_pEffect);
    
    SAFE_RELEASE(m_pGridBoxLayout);
    SAFE_RELEASE(m_pGridBoxVertexBuffer);
    SAFE_RELEASE(m_pGridBoxIndexBuffer);
    
    SAFE_RELEASE(m_pQuadLayout);
    SAFE_RELEASE(m_pQuadVertexBuffer);
    
    SAFE_RELEASE(pRayDataTex2D[0]);
    SAFE_RELEASE(pRayDataTex2D[1]);
    SAFE_RELEASE(m_pRayDataSRV[0]);
    SAFE_RELEASE(m_pRayDataSRV[1]);
    SAFE_RELEASE(m_pRayDataRTV[0]);
    SAFE_RELEASE(m_pRayDataRTV[1]);
    SAFE_RELEASE(m_pRayDataSmallTex2D);
    SAFE_RELEASE(m_pRayDataSmallRTV);
    SAFE_RELEASE(m_pRayDataSmallSRV);
    SAFE_RELEASE(m_pRayCastTex2D);
    SAFE_RELEASE(m_pRayCastSRV);
    SAFE_RELEASE(m_pRayCastRTV);
    SAFE_RELEASE(m_pGlowTex2D);
    SAFE_RELEASE(m_pGlowSRV);
    SAFE_RELEASE(m_pGlowRTV);
    SAFE_RELEASE(m_pGlowTempTex2D);
    SAFE_RELEASE(m_pGlowTempSRV);
    SAFE_RELEASE(m_pGlowTempRTV);
    SAFE_RELEASE(m_pEdgeTex2D); 
    SAFE_RELEASE(m_pEdgeSRV);
    SAFE_RELEASE(m_pEdgeRTV);
    
}


HRESULT VolumeRenderer::Initialize(int gridWidth, int gridHeight, int gridDepth, RENDER_MODE renderMode)
{
    HRESULT hr(S_OK);
    
    Cleanup();

    m_eRenderMode = renderMode;

    m_gridDim[0] = float(gridWidth);
    m_gridDim[1] = float(gridHeight);
    m_gridDim[2] = float(gridDepth);

    m_maxDim = max( max( m_gridDim[0], m_gridDim[1] ), m_gridDim[2] );

    // Initialize the grid offset matrix
    {
        // Make a scale matrix to scale the unit-sided box to be unit-length on the 
        //  side/s with maximum dimension 
        D3DXMATRIX scaleM;
        D3DXMatrixIdentity(&scaleM);
        D3DXMatrixScaling(&scaleM, m_gridDim[0] / m_maxDim, m_gridDim[1] / m_maxDim, m_gridDim[2] / m_maxDim);
        // offset grid to be centered at origin
        D3DXMATRIX translationM;
        D3DXMatrixTranslation(&translationM, -0.5, -0.5, -0.5);

        m_gridMatrix = translationM * scaleM;
    }

    // Check if the device supports FP32 blending to choose the right codepath depending on this
    UINT rgba32fFormatSupport;
    m_pD3DDevice->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT, &rgba32fFormatSupport);
    m_useFP32Blending = (rgba32fFormatSupport & D3D10_FORMAT_SUPPORT_BLENDABLE) ? true : false;
    m_useFP32Blending = false;

    V_RETURN(initShaders());
    V_RETURN(createGridBox());
    V_RETURN(createScreenQuad());

    V_RETURN(createJitterTexture());
    V_RETURN(loadTextureFromFile(m_pD3DDevice, L"..\\..\\Media\\FireTransferFunction.dds", m_pEffect, m_effectPath, "fireTransferFunction"));


    return hr;
}


void VolumeRenderer::Draw(ID3D10ShaderResourceView * pSourceTexSRV)
{
    m_evTexture_volume->SetResource(pSourceTexSRV);
    // Set some variables required by the shaders:
    //=========================================================================

    m_evGlowContribution->SetFloat(g_glowContribution);
    m_evFinalIntensityScale->SetFloat(g_finalIntensityScale);
    m_evFinalAlphaScale->SetFloat(g_finalAlphaScale);
    m_evSmokeColorMultiplier->SetFloat(g_smokeColorMultiplier);
    m_evsmokeAlphaMultiplier->SetFloat(g_smokeAlphaMultiplier);
    m_evRednessFactor->SetInt(g_RednessFactor);

    // The near and far planes are used to unproject the scene's z-buffer values
    m_evZNear->SetFloat(g_zNear);
    m_evZFar->SetFloat(g_zFar);

    D3DMATRIX grid2World = m_gridMatrix * g_gridWorld;
    m_evGrid2WorldMatrix->SetMatrix( (float*) &grid2World );

    D3DXMATRIX worldView = g_gridWorld * g_View;

    // The length of one of the axis of the worldView matrix is the length of longest side of the box
    //  in view space. This is used to convert the length of a ray from view space to grid space.
    D3DXVECTOR3 worldXaxis = D3DXVECTOR3(worldView._11, worldView._12, worldView._13);
    float worldScale = D3DXVec3Length(&worldXaxis);
    m_evGridScaleFactor->SetFloat( worldScale );

    // We prepend the current world matrix with this other matrix which adds an offset (-0.5, -0.5, -0.5)
    //  and scale factors to account for unequal number of voxels on different sides of the volume box. 
    // This is because we want to preserve the aspect ratio of the original simulation grid when 
    //  raytracing through it, and this matrix allows us to do the raytracing in grid (texture) space: 
    //  i.e. each side of the box spans the 0 to 1 range
    worldView = m_gridMatrix * worldView;

    m_evWorldViewMatrix->SetMatrix( (float*)&worldView );
    m_evTan_FovYhalf->SetFloat( tan(g_Fovy/2.0f) );
    m_evTan_FovXhalf->SetFloat( tan(g_Fovy/2.0f)*m_renderTextureWidth/m_renderTextureHeight );

    // options for the LevelSet raytracer
    m_evRaycastBisection->SetBool(m_bRaycastBisection);
    m_evRaycastFilterTricubic->SetBool(m_bRaycastFilterTricubic);
    m_evRaycastShaderAsWater->SetBool(m_bRaycastShadeAsWater);

	// worldViewProjection is used to transform the volume box to screen space
    D3DXMATRIX worldViewProjection;
    worldViewProjection = worldView * g_Projection;
    m_evWorldViewProjectionMatrix->SetMatrix( (float*)&worldViewProjection );

    // invWorldViewProjection is used to transform positions in the "near" plane into grid space
    D3DXMATRIX invWorldViewProjection;
    D3DXMatrixInverse(&invWorldViewProjection, NULL, &worldViewProjection);
    m_evInvWorldViewProjectionMatrix->SetMatrix((float*)&invWorldViewProjection);

    // Compute the inverse of the worldView matrix 
    D3DXMATRIX worldViewInv;
    D3DXMatrixInverse(&worldViewInv, NULL, &worldView);
    // Compute the eye's position in "grid space" (the 0-1 texture coordinate cube)
    D3DXVECTOR4 eyeInGridSpace;
    D3DXVECTOR3 origin(0,0,0);
    D3DXVec3Transform(&eyeInGridSpace, &origin, &worldViewInv);
    m_evEyeOnGrid->SetFloatVector((float*)&eyeInGridSpace);

    float color[4] = {0, 0, 0, 0 };


    // Ray cast and render to a temporary buffer
    //=========================================================================

    // Partial init of viewport struct used below
    D3D10_VIEWPORT rtViewport;
    rtViewport.TopLeftX = 0;
    rtViewport.TopLeftY = 0;
    rtViewport.MinDepth = 0;
    rtViewport.MaxDepth = 1;


    // Compute the ray data required by the raycasting pass below.
    //  This function will render to a buffer of float4 vectors, where
    //  xyz is starting position of the ray in grid space
    //  w is the length of the ray in view space
    computeRayData();


    // Do edge detection on this image to find any 
    //  problematic areas where we need to raycast at higher resolution
    computeEdgeTexture();
    

    // Raycast into the temporary render target: 
    //  raycasting is done at the smaller resolution, using a fullscreen quad
    m_pD3DDevice->ClearRenderTargetView( m_pRayCastRTV, color );
    m_pD3DDevice->OMSetRenderTargets( 1, &m_pRayCastRTV , NULL ); 

    rtViewport.Width = m_renderTextureWidth;
    rtViewport.Height = m_renderTextureHeight;
    m_pD3DDevice->RSSetViewports(1,&rtViewport);

    m_evRTWidth->SetFloat((float)m_renderTextureWidth);
    m_evRTHeight->SetFloat((float)m_renderTextureHeight);

    m_evTexture_rayDataSmall->SetResource(m_pRayDataSmallSRV);
   
    if( m_eRenderMode == RM_LEVELSET )
        m_epQuadRaycastLevelSet->Apply(0);
    else if(m_eRenderMode == RM_FIRE)
        m_epQuadRaycastFire->Apply(0);
    else
        m_epQuadRaycastSmoke->Apply(0);

    drawScreenQuad();

   
    m_pD3DDevice->ClearRenderTargetView( m_pGlowRTV, color );

    //blur the raycast image to get a blur texture
    m_evUseGlow->SetBool(g_bRenderGlow);
    if((m_eRenderMode == RM_FIRE) && g_bRenderGlow)
    {
        m_pD3DDevice->ClearRenderTargetView( m_pGlowTempRTV, color );
        m_pD3DDevice->OMSetRenderTargets( 1, &m_pGlowTempRTV , NULL ); 
        m_evTexture_glow->SetResource(m_pRayCastSRV);
        m_epGlowHorizontal->Apply(0);
        drawScreenQuad();
        
        m_pD3DDevice->OMSetRenderTargets( 1, &m_pGlowRTV , NULL ); 
        m_evTexture_glow->SetResource(m_pGlowTempSRV);
        m_epGlowVertical->Apply(0);
        drawScreenQuad();
        
    }


    // Render to the back buffer sampling from the raycast texture that we just created
    // If and edge was detected at the current pixel we will raycast again to avoid
    // smoke aliasing artifacts at scene edges
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    m_pD3DDevice->OMSetRenderTargets( 1, &pRTV , pDSV ); 

    rtViewport.Width = g_Width;
    rtViewport.Height = g_Height;
    m_pD3DDevice->RSSetViewports(1,&rtViewport);

    m_evRTWidth->SetFloat((float)g_Width);
    m_evRTHeight->SetFloat((float)g_Height);

    m_evTexture_rayCast->SetResource(m_pRayCastSRV);
    m_evTexture_edge->SetResource(m_pEdgeSRV);
    m_evTexture_glow->SetResource(m_pGlowSRV);
    
    if( m_eRenderMode == RM_LEVELSET )
        m_epQuadRaycastUpsampleLevelSet->Apply(0);
    else if(m_eRenderMode == RM_FIRE)
        m_epQuadRaycastUpsampleFire->Apply(0);
    else
        m_epQuadRaycastUpsampleSmoke->Apply(0);

    drawScreenQuad();

    m_evTexture_rayCast->SetResource(NULL);
    m_evTexture_edge->SetResource(NULL);
    m_evTexture_glow->SetResource(NULL);
    m_epQuadRaycastUpsampleFire->Apply(0);

}

void VolumeRenderer::computeRayData( void )
{
    // Clear the color buffer to 0
    float blackColor[4] = {0, 0, 0, 0 };
    m_pD3DDevice->ClearRenderTargetView(m_pRayDataRTV[0], blackColor);
    if( !m_useFP32Blending )
        m_pD3DDevice->ClearRenderTargetView(m_pRayDataRTV[1], blackColor);

    m_pD3DDevice->OMSetRenderTargets(1, &m_pRayDataRTV[0], NULL);

    m_evTexture_sceneDepthTex->SetResource(g_pSceneDepthSRV);
    
    // Setup viewport to match the window's backbuffer
    D3D10_VIEWPORT rtViewport;
    rtViewport.TopLeftX = 0;
    rtViewport.TopLeftY = 0;
    rtViewport.MinDepth = 0;
    rtViewport.MaxDepth = 1;
    rtViewport.Width = g_Width;
    rtViewport.Height = g_Height;
    m_pD3DDevice->RSSetViewports(1,&rtViewport);
    m_evRTWidth->SetFloat((float)g_Width);
    m_evRTHeight->SetFloat((float)g_Height);

    // Render volume back faces
    // We output xyz=(0,-1,0) and w=min(sceneDepth, boxDepth)
    m_epCompRayData_Back->Apply(0);
    drawBox();

    if( !m_useFP32Blending )
    {
        // repeat the back face pass in m_pRayDataRTV[1] - we could also do CopySubResource
        m_pD3DDevice->OMSetRenderTargets(1, &m_pRayDataRTV[1], NULL);
        drawBox();
    }

    // Render volume front faces using subtractive blending or doing texture lookups
    //  depending on hw support for FP32 blending. Note that an FP16 RGBA buffer is
    //  does not have enough precision to represent the different XYZ postions 
    //  for each ray entry point in most common circumstances.
    // We output xyz="position in grid space" and w=boxDepth,
    //  unless the pixel is occluded by the scene, in which case we output xyzw=(1,0,0,0)
    if( m_useFP32Blending )
    {
        m_epCompRayData_Front->Apply(0);
    }
    else
    {
        m_evTexture_rayData->SetResource(m_pRayDataSRV[0]);
        m_epCompRayData_FrontNOBLEND->Apply(0);
    }
    drawBox();

}

void VolumeRenderer::computeEdgeTexture(void)
{
    // First setup viewport to match the size of the destination low-res texture
    D3D10_VIEWPORT rtViewport;
    rtViewport.TopLeftX = 0;
    rtViewport.TopLeftY = 0;
    rtViewport.MinDepth = 0;
    rtViewport.MaxDepth = 1;
    rtViewport.Width = m_renderTextureWidth;
    rtViewport.Height = m_renderTextureHeight;
    m_pD3DDevice->RSSetViewports(1,&rtViewport);
    m_evRTWidth->SetFloat((float)m_renderTextureWidth);
    m_evRTHeight->SetFloat((float)m_renderTextureHeight);

    // Downsample the rayDataTexture to a new small texture, simply using point sample (no filtering)
    m_pD3DDevice->OMSetRenderTargets( 1, &m_pRayDataSmallRTV , NULL ); 
    if( m_useFP32Blending )
        m_evTexture_rayData->SetResource(m_pRayDataSRV[0]);
    else
        m_evTexture_rayData->SetResource(m_pRayDataSRV[1]);
    m_epQuadDownSampleRayDataTexture->Apply(0);
    drawScreenQuad();

    // Create an edge texture, performing edge detection on 'rayDataTexSmall'
    m_pD3DDevice->OMSetRenderTargets( 1, &m_pEdgeRTV , NULL ); 
    m_evTexture_rayDataSmall->SetResource(m_pRayDataSmallSRV);
    m_epQuadEdgeDetect->Apply(0);
    drawScreenQuad();
}

void VolumeRenderer::drawBox(void)
{
    UINT stride = sizeof( VsInput );
    UINT offset = 0;
    m_pD3DDevice->IASetVertexBuffers( 0, 1, &m_pGridBoxVertexBuffer, &stride, &offset );
    m_pD3DDevice->IASetIndexBuffer( m_pGridBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
    m_pD3DDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    m_pD3DDevice->IASetInputLayout(m_pGridBoxLayout);
    m_pD3DDevice->DrawIndexed(36, 0, 0);
}


void VolumeRenderer::drawScreenQuad(void)
{
    UINT strides = sizeof(VsInput);
    UINT offsets = 0;
    m_pD3DDevice->IASetInputLayout( m_pQuadLayout );
    m_pD3DDevice->IASetVertexBuffers( 0, 1, &m_pQuadVertexBuffer, &strides, &offsets );
    m_pD3DDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
    m_pD3DDevice->Draw( 4, 0 );
}

HRESULT VolumeRenderer::SetScreenSize( int width, int height )
{
    HRESULT hr(S_OK);

    V_RETURN(createRayDataResources(width, height));
    return S_OK;
}

// Resource initialization functions below

HRESULT VolumeRenderer::initShaders(  )
{
    HRESULT hr(S_OK);
    __DWORD__ dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
    
    V_RETURN(NVUTFindDXSDKMediaFileCchT( m_effectPath, MAX_PATH, L"VolumeRenderer.fx" ) );
    V_RETURN(D3DX10CreateEffectFromFile(m_effectPath, g_pShadersMacros, NULL, "fx_4_0", dwShaderFlags, 0, m_pD3DDevice, NULL, NULL, &m_pEffect, NULL, &hr ));

    V_GET_TECHNIQUE_RET(m_pEffect, m_effectPath, m_pTechnique, "VolumeRenderer");

    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_rayData, "rayDataTex", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_rayDataSmall, "rayDataTexSmall", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_volume, "volumeTex", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_rayCast, "rayCastTex", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_edge, "edgeTex", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_glow, "glowTex", AsShaderResource);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTexture_sceneDepthTex, "sceneDepthTex", AsShaderResource );

    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRTWidth, "RTWidth", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRTHeight, "RTHeight", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evUseGlow, "useGlow", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evWorldViewProjectionMatrix, "WorldViewProjection", AsMatrix);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evInvWorldViewProjectionMatrix, "InvWorldViewProjection", AsMatrix);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evGrid2WorldMatrix, "Grid2World", AsMatrix);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evZNear, "ZNear", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evZFar, "ZFar", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evGridScaleFactor,  "gridScaleFactor", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evEyeOnGrid, "eyeOnGrid", AsVector);

    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evGlowContribution, "glowContribution", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evFinalIntensityScale, "finalIntensityScale", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evFinalAlphaScale, "finalAlphaScale", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evSmokeColorMultiplier, "smokeColorMultiplier", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evsmokeAlphaMultiplier, "smokeAlphaMultiplier", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRednessFactor, "rednessFactor", AsScalar);

    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evWorldViewMatrix, "WorldView", AsMatrix);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTan_FovYhalf, "tan_FovYhalf", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evTan_FovXhalf, "tan_FovXhalf", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRaycastBisection, "g_bRaycastBisection", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRaycastFilterTricubic, "g_bRaycastFilterTricubic", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, m_evRaycastShaderAsWater, "g_bRaycastShadeAsWater", AsScalar);

    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastLevelSet, "QuadRaycastLevelSet");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastFire, "QuadRaycastFire");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastSmoke, "QuadRaycastSmoke");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epGlowHorizontal, "GlowHorizontal");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epGlowVertical, "GlowVertical");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastUpsampleLevelSet, "QuadRaycastUpsampleLevelSet");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastUpsampleFire, "QuadRaycastUpsampleFire");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadRaycastUpsampleSmoke, "QuadRaycastUpsampleSmoke");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epCompRayData_Back, "CompRayData_Back");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epCompRayData_Front, "CompRayData_Front");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epCompRayData_FrontNOBLEND, "CompRayData_FrontNOBLEND");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadDownSampleRayDataTexture, "QuadDownSampleRayDataTexture");
    V_GET_PASS_RET(m_pTechnique, m_effectPath, m_epQuadEdgeDetect, "QuadEdgeDetect");


    D3DXVECTOR3 recGridDim(1.0f/m_gridDim[0], 1.0f/m_gridDim[1], 1.0f/m_gridDim[2]);
    
    ID3D10EffectVectorVariable *evGridDim;
    ID3D10EffectVectorVariable *evRecGridDim;
    ID3D10EffectScalarVariable *evMaxGridDim;
    ID3D10EffectShaderResourceVariable  *evTexture_envMap;

    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, evGridDim, "gridDim", AsVector);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, evRecGridDim, "recGridDim", AsVector);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, evMaxGridDim, "maxGridDim", AsScalar);
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, evTexture_envMap, "envMapTex", AsShaderResource);
    
    evGridDim->SetFloatVector(m_gridDim);
    evRecGridDim->SetFloatVector(recGridDim);
    evMaxGridDim->SetFloat(m_maxDim);
    evTexture_envMap->SetResource(g_pEnvMapSRV);

    return S_OK;
}

HRESULT VolumeRenderer::createGridBox (void) 
{
    HRESULT hr(S_OK);

    VsInput vertices[] =
    {
        { D3DXVECTOR3( 0, 0, 0 ) },
        { D3DXVECTOR3( 0, 0, 1 ) },
        { D3DXVECTOR3( 0, 1, 0 ) },
        { D3DXVECTOR3( 0, 1, 1 ) },
        { D3DXVECTOR3( 1, 0, 0 ) },
        { D3DXVECTOR3( 1, 0, 1 ) },
        { D3DXVECTOR3( 1, 1, 0 ) },
        { D3DXVECTOR3( 1, 1, 1 ) },
    };

    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    V_RETURN( m_pD3DDevice->CreateBuffer( &bd, &InitData, &m_pGridBoxVertexBuffer ) );


    // Create index buffer
    __DWORD__ indices[] =
    {
        0, 4, 1, 1, 4, 5,
        0, 1, 2, 2, 1, 3,
        4, 6, 5, 6, 7, 5,
        2, 3, 6, 3, 7, 6,
        1, 5, 3, 3, 5, 7,
        0, 2, 4, 2, 6, 4
    };

    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(indices);
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = indices;
    V_RETURN( m_pD3DDevice->CreateBuffer( &bd, &InitData, &m_pGridBoxIndexBuffer ) );

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
    };
    UINT numElements = sizeof(layout)/sizeof(layout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    V_RETURN(m_epCompRayData_Back->GetDesc(&PassDesc));
    V_RETURN( m_pD3DDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pGridBoxLayout ) );

    return hr;
}


HRESULT VolumeRenderer::createScreenQuad(void ) 
{
    HRESULT hr(S_OK);
    // Create our quad input layout
    const D3D10_INPUT_ELEMENT_DESC quadlayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof(quadlayout)/sizeof(quadlayout[0]);

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    V_RETURN(m_epQuadRaycastSmoke->GetDesc( &PassDesc ));
    V_RETURN(m_pD3DDevice->CreateInputLayout( quadlayout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &m_pQuadLayout ));

    // Create a screen quad for all render to texture operations
    VsInput svQuad[4];
    svQuad[0].pos = D3DXVECTOR3(-1.0f, 1.0f, 0.0f );
    svQuad[1].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f );
    svQuad[2].pos = D3DXVECTOR3(-1.0f, -1.0f, 0.0f );
    svQuad[3].pos = D3DXVECTOR3(1.0f, -1.0f, 0.0f );

    D3D10_BUFFER_DESC vbdesc =
    {
        4*sizeof(VsInput),
        D3D10_USAGE_DEFAULT,
        D3D10_BIND_VERTEX_BUFFER,
        0,
        0
    };

    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = svQuad;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    V_RETURN( m_pD3DDevice->CreateBuffer( &vbdesc, &InitData, &m_pQuadVertexBuffer ) );

    return hr;
}

void VolumeRenderer::calculateRenderTextureSize(int screenWidth, int screenHeight)
{
    int maxProjectedSide = int(3.0 * sqrt(3.0)*m_maxDim);
    int maxScreenDim = max(screenWidth, screenHeight);
    
    float screenAspectRatio = ((float)screenWidth)/screenHeight;

    if( maxScreenDim > maxProjectedSide)
    {
        if(screenHeight > screenWidth)
        {
            m_renderTextureHeight = maxProjectedSide;
            m_renderTextureWidth = (int)(screenAspectRatio * maxProjectedSide);
        }
        else
        {
            m_renderTextureWidth = maxProjectedSide;
            m_renderTextureHeight = (int)((1.0f/screenAspectRatio) * maxProjectedSide);
        }
    }
    else
    {
        m_renderTextureWidth = screenWidth;
        m_renderTextureHeight = screenHeight;
    }
}

HRESULT VolumeRenderer::createRayDataResources( int width, int height )
{
    HRESULT hr(S_OK);

    // clean up all resources first
    SAFE_RELEASE(pRayDataTex2D[0]);
    SAFE_RELEASE(pRayDataTex2D[1]);
    SAFE_RELEASE(m_pRayDataSRV[0]);
    SAFE_RELEASE(m_pRayDataSRV[1]);
    SAFE_RELEASE(m_pRayDataRTV[0]);
    SAFE_RELEASE(m_pRayDataRTV[1]);
    SAFE_RELEASE(m_pRayDataSmallTex2D);
    SAFE_RELEASE(m_pRayDataSmallSRV);
    SAFE_RELEASE(m_pRayDataSmallRTV);
    SAFE_RELEASE(m_pRayCastTex2D);
    SAFE_RELEASE(m_pRayCastSRV);
    SAFE_RELEASE(m_pRayCastRTV);
    SAFE_RELEASE(m_pEdgeTex2D);
    SAFE_RELEASE(m_pEdgeSRV);
    SAFE_RELEASE(m_pEdgeRTV);
    SAFE_RELEASE(m_pGlowTex2D);
    SAFE_RELEASE(m_pGlowSRV);
    SAFE_RELEASE(m_pGlowRTV);
    SAFE_RELEASE(m_pGlowTempTex2D);
    SAFE_RELEASE(m_pGlowTempSRV);
    SAFE_RELEASE(m_pGlowTempRTV);


    // find a good resolution for raycasting purposes
    calculateRenderTextureSize(width, height);

    DXGI_FORMAT rayDataFmt = DXGI_FORMAT_R32G32B32A32_FLOAT;
    
    //create the textures
    D3D10_TEXTURE2D_DESC desc;
    desc.ArraySize = 1;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MipLevels = 1;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.Width = width;
    desc.Height = height;
    desc.Format = rayDataFmt;
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL, &pRayDataTex2D[0]));
    if( !m_useFP32Blending )
        V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL, &pRayDataTex2D[1]));
    desc.Width = m_renderTextureWidth;
    desc.Height = m_renderTextureHeight;
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL,&m_pRayDataSmallTex2D));
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL,&m_pRayCastTex2D));
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL,&m_pGlowTex2D));
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL,&m_pGlowTempTex2D));
    desc.Format = DXGI_FORMAT_R32_FLOAT;
    V_RETURN(m_pD3DDevice->CreateTexture2D(&desc,NULL,&m_pEdgeTex2D));

    //create the render target views
    D3D10_RENDER_TARGET_VIEW_DESC DescRT;
    DescRT.Format = rayDataFmt;
    DescRT.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
    DescRT.Texture2D.MipSlice = 0;
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(pRayDataTex2D[0], &DescRT, &m_pRayDataRTV[0]));
    if( !m_useFP32Blending )
        V_RETURN( m_pD3DDevice->CreateRenderTargetView(pRayDataTex2D[1], &DescRT, &m_pRayDataRTV[1]));
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(m_pRayDataSmallTex2D, &DescRT, &m_pRayDataSmallRTV));   
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(m_pRayCastTex2D, &DescRT, &m_pRayCastRTV));
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(m_pGlowTex2D, &DescRT, &m_pGlowRTV));
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(m_pGlowTempTex2D, &DescRT, &m_pGlowTempRTV));
    DescRT.Format = DXGI_FORMAT_R32_FLOAT;
    V_RETURN( m_pD3DDevice->CreateRenderTargetView(m_pEdgeTex2D, &DescRT, &m_pEdgeRTV));

    //create the shader resource views
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;
    SRVDesc.Format = rayDataFmt;
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(pRayDataTex2D[0], &SRVDesc, &m_pRayDataSRV[0]));
    if( !m_useFP32Blending )
        V_RETURN(m_pD3DDevice->CreateShaderResourceView(pRayDataTex2D[1], &SRVDesc, &m_pRayDataSRV[1]));
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(m_pRayDataSmallTex2D, &SRVDesc, &m_pRayDataSmallSRV));
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(m_pRayCastTex2D, &SRVDesc, &m_pRayCastSRV));
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(m_pGlowTex2D, &SRVDesc, &m_pGlowSRV));
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(m_pGlowTempTex2D, &SRVDesc, &m_pGlowTempSRV));
    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    V_RETURN(m_pD3DDevice->CreateShaderResourceView(m_pEdgeTex2D, &SRVDesc, &m_pEdgeSRV));

    return hr;
}

HRESULT VolumeRenderer::createJitterTexture( )
{
    HRESULT hr = S_OK;

    ID3D10Texture2D* jitterTexture = NULL;
    ID3D10ShaderResourceView* jitterTextureSRV = NULL;


    // initial data for jitter texture
    BYTE data[256 * 256];
    for (int i = 0; i < 256 * 256; i++)
    {
        data[i] = (unsigned char) (stx_rand()/float(RAND_MAX)*256);
    }

    D3D10_SUBRESOURCE_DATA dataDesc;
    dataDesc.pSysMem = data;
    dataDesc.SysMemPitch = 256;

    // create the jitter texture
    D3D10_TEXTURE2D_DESC desc;
    desc.Width = 256;
    desc.Height = 256;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8_TYPELESS;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D10_USAGE_IMMUTABLE;
    desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    V( m_pD3DDevice->CreateTexture2D(&desc, &dataDesc, &jitterTexture) );

    // create the shader resource view for the jitter texture
    D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
    ZeroMemory( &descSRV, sizeof(descSRV) );
    descSRV.Format = DXGI_FORMAT_R8_UNORM;
    descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MipLevels = 1;
    descSRV.Texture2D.MostDetailedMip = 0;
    V( m_pD3DDevice->CreateShaderResourceView( jitterTexture, &descSRV, &jitterTextureSRV ) );

    ID3D10EffectShaderResourceVariable *evTexture_jitter;
    V_GET_VARIABLE_RET(m_pEffect, m_effectPath, evTexture_jitter, "jitterTex", AsShaderResource);
    evTexture_jitter->SetResource (jitterTextureSRV);


    SAFE_RELEASE(jitterTexture);
    SAFE_RELEASE(jitterTextureSRV);
    return hr;
}


