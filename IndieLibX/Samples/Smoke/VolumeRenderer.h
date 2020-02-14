//----------------------------------------------------------------------------------
// File:   VolumeRenderer.h
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

#ifndef FLUID3D_VOLUMERENDERER_H
#define FLUID3D_VOLUMERENDERER_H

#include "nvutPCH.h"
#include "Smoke.h"
#include <windows.h>


extern bool                                g_bRenderGlow;
extern float                               g_zNear;
extern float                               g_zFar;
extern float                               g_glowContribution;
extern float                               g_finalIntensityScale; 
extern float                               g_finalAlphaScale;
extern float                               g_smokeColorMultiplier;   
extern float                               g_smokeAlphaMultiplier;
extern int                                 g_RednessFactor; 
extern float                               g_xyVelocityScale;
extern float                               g_zVelocityScale;
extern float                               g_fModelRotation;
extern float                               g_fRotSpeed;
extern D3DXMATRIX                          g_View;
extern D3DXMATRIX                          g_Projection;
extern float                               g_Fovy;

extern ID3D10ShaderResourceView           *g_pSceneDepthSRV;
extern ID3D10ShaderResourceView           *g_pEnvMapSRV;
extern D3DXMATRIX                          g_gridWorld;


//does a volume rendering of a box using specified voxel data
class VolumeRenderer
{
public:

    enum RENDER_MODE
    {
        RM_SMOKE    = 0,
        RM_FIRE     = 1,
        RM_LEVELSET = 2
    };

    VolumeRenderer(ID3D10Device* pd3dDevice);
    virtual ~VolumeRenderer(void);
    
    HRESULT Initialize(int gridWidth, int gridHeight, int gridDepth, RENDER_MODE renderMode);
    HRESULT SetScreenSize(int width, int height);
    void Cleanup(void);
    void Draw(ID3D10ShaderResourceView * pSourceTexSRV);
    const float GetMaxDim(){return m_maxDim;};

protected:

    HRESULT initShaders(void);
    HRESULT createGridBox(void);
    HRESULT createScreenQuad(void);
    void calculateRenderTextureSize(int screenWidth, int screenHeight);
    HRESULT createRayDataResources(int width, int height);
    HRESULT createJitterTexture();

    void computeRayData(void);
    void computeEdgeTexture(void);
    void drawBox(void);
    void drawScreenQuad(void);

    ID3D10Device                *m_pD3DDevice;

    float                       m_gridDim[3];
    float                       m_maxDim;
    D3DXMATRIX                  m_gridMatrix;

    TCHAR                       m_effectPath[MAX_PATH];
    ID3D10Effect                *m_pEffect;
    ID3D10EffectTechnique       *m_pTechnique;

    ID3D10EffectPass            *m_epQuadRaycastLevelSet;
    ID3D10EffectPass            *m_epQuadRaycastFire;
    ID3D10EffectPass            *m_epQuadRaycastSmoke; 
    ID3D10EffectPass            *m_epGlowHorizontal;
    ID3D10EffectPass            *m_epGlowVertical;
    ID3D10EffectPass            *m_epQuadRaycastUpsampleLevelSet;
    ID3D10EffectPass            *m_epQuadRaycastUpsampleFire;
    ID3D10EffectPass            *m_epQuadRaycastUpsampleSmoke;
    ID3D10EffectPass            *m_epCompRayData_Back;
    ID3D10EffectPass            *m_epCompRayData_Front;
    ID3D10EffectPass            *m_epCompRayData_FrontNOBLEND;
    ID3D10EffectPass            *m_epQuadDownSampleRayDataTexture;
    ID3D10EffectPass            *m_epQuadEdgeDetect;


    ID3D10EffectShaderResourceVariable  *m_evTexture_rayData;
    ID3D10EffectShaderResourceVariable  *m_evTexture_rayDataSmall;
    ID3D10EffectShaderResourceVariable  *m_evTexture_volume;
    ID3D10EffectShaderResourceVariable  *m_evTexture_rayCast;
    ID3D10EffectShaderResourceVariable  *m_evTexture_edge;
    ID3D10EffectShaderResourceVariable  *m_evTexture_glow;
    ID3D10EffectShaderResourceVariable  *m_evTexture_sceneDepthTex;

    ID3D10EffectScalarVariable  *m_evRTWidth;
    ID3D10EffectScalarVariable  *m_evRTHeight;
    ID3D10EffectScalarVariable  *m_evUseGlow;
    ID3D10EffectMatrixVariable  *m_evWorldViewProjectionMatrix;
    ID3D10EffectMatrixVariable  *m_evInvWorldViewProjectionMatrix;
    ID3D10EffectMatrixVariable  *m_evGrid2WorldMatrix;
    ID3D10EffectScalarVariable  *m_evZNear;
    ID3D10EffectScalarVariable  *m_evZFar;
    ID3D10EffectScalarVariable  *m_evGridScaleFactor;
    ID3D10EffectVectorVariable  *m_evEyeOnGrid;

    ID3D10EffectScalarVariable  *m_evGlowContribution;
    ID3D10EffectScalarVariable  *m_evFinalIntensityScale;
    ID3D10EffectScalarVariable  *m_evFinalAlphaScale;
    ID3D10EffectScalarVariable  *m_evSmokeColorMultiplier;
    ID3D10EffectScalarVariable  *m_evsmokeAlphaMultiplier;
    ID3D10EffectScalarVariable  *m_evRednessFactor;

    ID3D10EffectMatrixVariable  *m_evWorldViewMatrix;
    ID3D10EffectScalarVariable  *m_evTan_FovYhalf;
    ID3D10EffectScalarVariable  *m_evTan_FovXhalf;
    ID3D10EffectScalarVariable  *m_evRaycastBisection;
    ID3D10EffectScalarVariable  *m_evRaycastFilterTricubic;
    ID3D10EffectScalarVariable  *m_evRaycastShaderAsWater;


    ID3D10InputLayout           *m_pGridBoxLayout;
    ID3D10Buffer                *m_pGridBoxVertexBuffer;
    ID3D10Buffer                *m_pGridBoxIndexBuffer;

    ID3D10InputLayout           *m_pQuadLayout;
    ID3D10Buffer                *m_pQuadVertexBuffer;


    ID3D10Texture2D             *pRayDataTex2D[2];
    ID3D10RenderTargetView      *m_pRayDataRTV[2];
    ID3D10ShaderResourceView    *m_pRayDataSRV[2];

    ID3D10Texture2D             *m_pRayDataSmallTex2D;
    ID3D10RenderTargetView      *m_pRayDataSmallRTV;
    ID3D10ShaderResourceView    *m_pRayDataSmallSRV;
    
    ID3D10Texture2D             *m_pRayCastTex2D;
    ID3D10RenderTargetView      *m_pRayCastRTV;
    ID3D10ShaderResourceView    *m_pRayCastSRV;
    
    ID3D10Texture2D             *m_pEdgeTex2D;
    ID3D10RenderTargetView      *m_pEdgeRTV;
    ID3D10ShaderResourceView    *m_pEdgeSRV;
    
    ID3D10Texture2D             *m_pGlowTex2D;
    ID3D10RenderTargetView      *m_pGlowRTV;
    ID3D10ShaderResourceView    *m_pGlowSRV;

    ID3D10Texture2D             *m_pGlowTempTex2D;
    ID3D10RenderTargetView      *m_pGlowTempRTV;
    ID3D10ShaderResourceView    *m_pGlowTempSRV;


    bool                        m_bRaycastBisection;
    bool                        m_bRaycastFilterTricubic;
    bool                        m_bRaycastShadeAsWater;

    RENDER_MODE                 m_eRenderMode;

    int                         m_renderTextureWidth;
    int                         m_renderTextureHeight;

    bool                        m_useFP32Blending;
};


#endif
