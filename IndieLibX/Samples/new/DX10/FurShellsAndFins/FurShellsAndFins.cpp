/*
	Copyright (c) 2020 Tommi Roenty   http://www.tommironty.fi/
	Licensed under The GNU Lesser General Public License, version 2.1:
		http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

class App: public STXGUI
{
public:
    App(){}
    virtual ~App(){}
    int init(const char* aTitle){STXGUI::init("/GUILayout.xml");}
    void render( ){STXGUI::update();}
//----------------------------------------------------------------------------------
// File:   FurShellsAndFins.cpp
// Author: Sarah Tariq
// Email:  sdkfeedback@nvidia.com
// 
// Copyright (c) 2007 NVIDIA Corporation. All rights reserved.
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

#include "resource.h"
#include <vector>
using namespace std;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3DXFROMWINEMATRIX g_CameraEyePt;
D3DXFROMWINEMATRIX g_CameraProj;
D3DXFROMWINEMATRIX g_CameraView;
D3DXFROMWINEMATRIX g_CameraWorld;
               // A model viewing camera
EffectID*               g_pEffect = 0;       // D3DXFROMWINE effect interface
VertexFormatID*          g_pVertexLayout = 0; // Vertex Layout for mesh
VertexFormatID*          g_pVertexLayoutArrow = 0; // Vertex Layout for arrow
bool                        g_bShowHelp = false;    // show help menu

MeshRenderer2                 g_Mesh;
MeshRenderer2                 g_MeshArrow;
exBufferID*               g_MeshLineAdjIB;
exBufferID*               g_MeshVB;
int                         g_MeshLineAdjIndices;


//effect variables
stx_Variable* g_pWorldVariable                 = 0;
stx_Variable* g_pWorldViewProj                 = 0;
stx_Variable* g_pEyeVec                        = 0;
stx_Variable* g_pLightVec                      = 0; 
stx_Variable* g_pShellNumberShaderVariable     = 0;
stx_Variable* g_pNumShellsNumberShaderVariable = 0;
stx_Variable* g_pShellIncrementShaderVariable  = 0;
stx_Variable* g_pMaxOpacityShaderVariable      = 0;
stx_Variable* g_pCombStrengthShaderVariable    = 0;
stx_Variable* g_pCombVectorShaderVariable      = 0;

//raster states for toggeling between wireframe and solid rendering mode, and enabling multisampling
RasterizerStateID * g_pRasterStateWireFrame;
RasterizerStateID * g_pRasterStateSolid;
RasterizerStateID * g_pRasterStateSolidNoCull;

//techniques
stx_Technique*  g_pTechniqueRenderMesh            = 0;
stx_Technique*  g_pTechniqueRenderShells          = 0;
stx_Technique*  g_pTechniqueRenderFinsLineAdj     = 0;
stx_Technique*  g_pTechniqueRenderArrow           = 0;

D3DXFROMWINEVECTOR3 vecEye;
D3DXFROMWINEVECTOR3 vecLight;
D3DXFROMWINEVECTOR3 eyeInObjectSpace;
D3DXFROMWINEVECTOR3 lightInObjectSpace;
D3DXFROMWINEVECTOR3 g_modelCentroid;
D3DXFROMWINEMATRIX  g_World;

bool                        g_bToggleWireFrame = false;
int                         g_numShells = 15;
float                       g_shellIncrement = 0.2f;
bool                        g_bShowShells = true;
bool                        g_bShowFins = true;
float                       g_maxOpacity = 0.6f;
D3DXFROMWINEVECTOR3                 g_combVector = D3DXFROMWINEVECTOR3(1.0f,-0.3f,0.0f);
float                       g_combStrength = 0.3f;

//textures for rendering fur 
std::string g_furTextureNames[] = 
{
      "../../Media/FurTexture/FurTexture00.dds",
      "../../Media/FurTexture/FurTexture01.dds",
      "../../Media/FurTexture/FurTexture02.dds",
      "../../Media/FurTexture/FurTexture03.dds",
      "../../Media/FurTexture/FurTexture04.dds",
      "../../Media/FurTexture/FurTexture05.dds",
      "../../Media/FurTexture/FurTexture06.dds",
      "../../Media/FurTexture/FurTexture07.dds",
      "../../Media/FurTexture/FurTexture08.dds",
      "../../Media/FurTexture/FurTexture09.dds",
      "../../Media/FurTexture/FurTexture10.dds",
      "../../Media/FurTexture/FurTexture11.dds",
      "../../Media/FurTexture/FurTexture12.dds",
      "../../Media/FurTexture/FurTexture13.dds",
      "../../Media/FurTexture/FurTexture14.dds",
      "../../Media/FurTexture/FurTexture15.dds"
};

std::string g_furOffsetNames[] = 
{
      "../../Media/FurTexture/FurTextureOffset00.dds",
      "../../Media/FurTexture/FurTextureOffset01.dds",
      "../../Media/FurTexture/FurTextureOffset02.dds",
      "../../Media/FurTexture/FurTextureOffset03.dds",
      "../../Media/FurTexture/FurTextureOffset04.dds",
      "../../Media/FurTexture/FurTextureOffset05.dds",
      "../../Media/FurTexture/FurTextureOffset06.dds",
      "../../Media/FurTexture/FurTextureOffset07.dds",
      "../../Media/FurTexture/FurTextureOffset08.dds",
      "../../Media/FurTexture/FurTextureOffset09.dds",
      "../../Media/FurTexture/FurTextureOffset10.dds",
      "../../Media/FurTexture/FurTextureOffset11.dds",
      "../../Media/FurTexture/FurTextureOffset12.dds",
      "../../Media/FurTexture/FurTextureOffset13.dds",
      "../../Media/FurTexture/FurTextureOffset14.dds",
      "../../Media/FurTexture/FurTextureOffset15.dds"
};


struct SimpleVertex
{
    D3DXFROMWINEVECTOR3 Pos;
    D3DXFROMWINEVECTOR2 Tex;
};

//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
int OnD3D10CreateDevice()
{
    int hr = 0;
    
    //create the raster states
    D3D10_RASTERIZER_DESC rasterizerState;
    rasterizerState.FillMode = D3D10_FILL_SOLID;
    rasterizerState.CullMode = D3D10_CULL_BACK;
    rasterizerState.FrontCounterClockwise = false;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = false;
    rasterizerState.ScissorEnable = false;
    rasterizerState.MultisampleEnable = true;
    rasterizerState.AntialiasedLineEnable = false;
    hr = IRenderer::GetRendererInstance()->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolid );
    IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateSolid);  
    rasterizerState.CullMode = D3D10_CULL_NONE;
    hr = IRenderer::GetRendererInstance()->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolidNoCull );
	rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
    rasterizerState.CullMode = D3D10_CULL_NONE;
    hr = IRenderer::GetRendererInstance()->CreateRasterizerState( &rasterizerState, &g_pRasterStateWireFrame );

    //Create the effect
    char str[MAX_PATH];
    ( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, "FurShellsAndFins.fx" ) );
    DWORD dwShaderFlags = 0;
    ( D3DXFROMWINE10CreateEffectFromFile(str, 0, 0, "fx_4_0", dwShaderFlags, 0, IRenderer::GetRendererInstance(), 0, 0, &g_pEffect, 0, &hr ) );

    // Obtain the techniques
    g_pTechniqueRenderMesh        = g_pEffect->GetTechniqueByName("RenderMesh");
    g_pTechniqueRenderShells      = g_pEffect->GetTechniqueByName("RenderShells");
    g_pTechniqueRenderFinsLineAdj = g_pEffect->GetTechniqueByName("RenderFinsLineAdj");
    g_pTechniqueRenderArrow       = g_pEffect->GetTechniqueByName("RenderArrow");

    // Obtain the variables
    g_pWorldVariable = g_pEffect->GetVariableByName( "World" )->AsMatrix();
    g_pWorldViewProj = g_pEffect->GetVariableByName( "WorldViewProj" )->AsMatrix();
    g_pEyeVec = g_pEffect->GetVariableByName( "Eye" )->AsVector();
    g_pLightVec = g_pEffect->GetVariableByName( "Light" )->AsVector();
    g_pShellNumberShaderVariable = g_pEffect->GetVariableByName( "shell" )->AsScalar();
    g_pNumShellsNumberShaderVariable = g_pEffect->GetVariableByName( "numShells" )->AsScalar();
    g_pShellIncrementShaderVariable = g_pEffect->GetVariableByName( "shellIncrement" )->AsScalar();
    g_pMaxOpacityShaderVariable = g_pEffect->GetVariableByName( "maxOpacity" )->AsScalar();
    g_pCombStrengthShaderVariable = g_pEffect->GetVariableByName( "combStrength" )->AsScalar(); 
    g_pCombVectorShaderVariable = g_pEffect->GetVariableByName( "combVector" )->AsVector();

    //set the variables
    g_pNumShellsNumberShaderVariable->SetInt(g_numShells + 1);
    g_pShellIncrementShaderVariable->SetFloat(g_shellIncrement);
    g_pMaxOpacityShaderVariable->SetFloat(g_maxOpacity);
    g_pCombStrengthShaderVariable->SetFloat(g_combStrength);
    D3DXFROMWINEVECTOR3 comb( g_combVector.x, g_combVector.y, g_combVector.z );
    D3DXFROMWINEVec3Normalize(&g_combVector,&g_combVector);
    g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);
    ( CDXUTDirectionWidget::StaticOnD3D10CreateDevice( IRenderer::GetRendererInstance() ) );

    //load the mesh for the scene---------------------------------------------------------------------

    // Define the input layout for mesh
    const D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },   
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D10_INPUT_PER_VERTEX_DATA, 0 },

    };
    unsigned int numElements = sizeof(layout)/sizeof(layout[0]);
    D3D10_PASS_DESC PassDesc;
    g_pTechniqueRenderShells->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    ( IRenderer::GetRendererInstance()->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pVertexLayout ) );

    // Set the input layout
    IRenderer::GetRendererInstance()->IASetInputLayout( g_pVertexLayout );

    // Load the mesh
    const D3D10_INPUT_ELEMENT_DESC layoutLoading[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },   
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D10_INPUT_PER_VERTEX_DATA, 0 }
    };

    ( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, "catLarge.x" ) ); 
    ( g_Mesh.Create( IRenderer::GetRendererInstance(), str, (D3D10_INPUT_ELEMENT_DESC*)layoutLoading, numElements,false ) );

    g_Mesh.ConvertToAdjacencyIndices();    //generate triangle adjacency for triangle adjacency method of fin rendering
	CreateLineAdjIndexBuffer(IRenderer::GetRendererInstance());  //generate line adjacency for line adjacency method of fin rendering
	

    //load the mesh for the arrow---------------------------------------------------------------------

    // Create the input layout
    const D3D10_INPUT_ELEMENT_DESC layoutArrow[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };

    numElements = sizeof(layoutArrow)/sizeof(layoutArrow[0]);
    g_pTechniqueRenderArrow->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    ( IRenderer::GetRendererInstance()->CreateInputLayout( layoutArrow, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pVertexLayoutArrow ) );

    ( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, "arrow.x" ) ); 
    ( g_MeshArrow.Create( IRenderer::GetRendererInstance(), str, (D3D10_INPUT_ELEMENT_DESC*)layoutArrow, numElements ) );

    //-------------------------------------------------------------------------------------------------
 
    // Initialize the camera
    g_modelCentroid = D3DXFROMWINEVECTOR3(0.0f, 27.0f, 0.0f);
    vecEye = D3DXFROMWINEVECTOR3( 0.0f, 0.0f, -120.0f);
    g_Camera.SetViewParams( &vecEye, &g_modelCentroid );
    g_Camera.SetEnablePositionMovement(true);
    g_Camera.SetScalers(0.001f,20.f);
    g_Camera.SetModelCenter( g_modelCentroid );

    // Initialize the world matrices
    D3DXFROMWINEMatrixIdentity( &g_World );
    g_Camera.SetWorldMatrix( g_World );
    g_World = *(g_Camera.GetWorldMatrix());

    //initialize eye in object space vector  
    D3DXFROMWINEMATRIX worldInverse;
    D3DXFROMWINEMatrixInverse(&worldInverse,0,&g_World);
    vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
    g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);

    //initialize light in object space vector
    vecLight = D3DXFROMWINEVECTOR3(-100.0f,300.0f,-200.0f);
    vectorMatrixMultiply(&lightInObjectSpace,worldInverse,vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

    //-------------------------------------------------------------------------------------------------
    //load the textures

    //fin texture
    (loadTextureFromFile("../../Media/FurTexture/FurTextureFin.dds","finTexture",IRenderer::GetRendererInstance())); 
    //fin offset
    (loadTextureFromFile("../../Media/FurTexture/FurTextureOffsetFin.dds","finOffset",IRenderer::GetRendererInstance()));  
    //fur color base
    (loadTextureFromFile("../../Media/catColor.dds","colorTexture",IRenderer::GetRendererInstance())); 
    
    //fur texture array
    //load all the textures for the fins into one texture array
    TextureID* furTexture = 0;
    TextureID* furTextureRV = 0;
    ID3D10EffectShaderResourceVariable*   furTextureArray = g_pEffect->GetVariableByName( "furTextureArray" )->AsShaderResource();
    ( LoadTextureArray( IRenderer::GetRendererInstance(), g_furTextureNames, sizeof(g_furTextureNames)/sizeof(g_furTextureNames[0]), &furTexture, &furTextureRV) );
    furTextureArray->SetResource( furTextureRV );

    //SAFE_RELEASE( furTexture );
    //SAFE_RELEASE( furTextureRV );
    furTexture = 0;
    furTextureRV = 0;

    //fur offset texture array
    //load all the offset textures for the fins into one texture array
    furTextureArray = g_pEffect->GetVariableByName( "furOffsetArray" )->AsShaderResource();
    ( LoadTextureArray( IRenderer::GetRendererInstance(), g_furOffsetNames, sizeof(g_furOffsetNames)/sizeof(g_furOffsetNames[0]), &furTexture, &furTextureRV) );
    furTextureArray->SetResource( furTextureRV );

    //SAFE_RELEASE( furTexture );
    //SAFE_RELEASE( furTextureRV );
    furTexture = 0;
    furTextureRV = 0;


    return 0;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
int OnD3D10ResizedSwapChain()
{
    int hr = 0;
    
    //unbind the current backbuffer and depth buffer since they will be deleted
    TextureID* pNULLRT = {0};
    IRenderer::GetRendererInstance()->OMSetRenderTargets( 1, &(pNULLRT), 0 );

    ( g_DialogResourceManager.OnD3D10ResizedSwapChain( IRenderer::GetRendererInstance(), pBufferSurfaceDesc ) );
    ( g_D3DSettingsDlg.OnD3D10ResizedSwapChain( IRenderer::GetRendererInstance(), pBufferSurfaceDesc ) );

    // Setup the camera's parameters
    float fov = D3DXFROMWINE_PI/4;
    float znear = 1.0f;
    float zfar  = 5000.0f;
    float fAspectRatio = static_cast<float>(pBufferSurfaceDesc->Width) / static_cast<float>(pBufferSurfaceDesc->Height);
    g_Camera.SetProjParams( fov , fAspectRatio, znear , zfar );

    return hr;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void OnD3D10FrameRender( ID3D10Device* IRenderer::GetRendererInstance(), double fTime, float fElapsedTime, void* pUserContext )
{    

    //get the current render target and depth stencil view and clear them
    TextureID* pOrigRT = DXUTGetD3D10RenderTargetView();
    TextureID* pOrigDS = DXUTGetD3D10DepthStencilView();
    float ClearColor[4] = { 0.1176f, 0.1176f, 0.1176f, 0.0f }; 
    IRenderer::GetRendererInstance()->ClearRenderTargetView( pOrigRT, ClearColor );
    IRenderer::GetRendererInstance()->ClearDepthStencilView( pOrigDS, D3D10_CLEAR_DEPTH, 1.0, 0 );
    IRenderer::GetRendererInstance()->OMSetRenderTargets( 1, &(pOrigRT), pOrigDS );

    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        return;
    }

    D3DXFROMWINEMATRIX mWorldView;
    D3DXFROMWINEMATRIX mWorldViewProj;

    //------------------------------------------------------------------------------------
    //render the comb directional control, and update the value of the comb vector
    //------------------------------------------------------------------------------------

    g_combVector = g_combDirectionWidget.GetLightDirection();
    D3DXFROMWINEVec3Normalize(&g_combVector,&g_combVector);
    g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);

    //calculate and set the world view projection matrix for transforming the arrow
    float arrowScale = 0.3f;
    D3DXFROMWINEMATRIX mScale;
    D3DXFROMWINEMatrixScaling( &mScale, arrowScale, arrowScale, arrowScale);
    D3DXFROMWINEMATRIX mLookAtMatrix;
    D3DXFROMWINEVECTOR3 mArrowPos =  g_modelCentroid - g_combVector * g_combDirectionWidget.GetRadius();
    D3DXFROMWINEVECTOR3 mOrigin = D3DXFROMWINEVECTOR3(0,0,0);
    D3DXFROMWINEVECTOR3 mUp = D3DXFROMWINEVECTOR3(0, 1, 0);
    D3DXFROMWINEMatrixLookAtLH(&mLookAtMatrix, &mArrowPos, &mOrigin, &mUp);
    D3DXFROMWINEMATRIX mLookAtInv;
    D3DXFROMWINEMatrixInverse(&mLookAtInv, 0, &mLookAtMatrix);
    D3DXFROMWINEMATRIX mWorld = mScale * mLookAtInv * (*g_Camera.GetWorldMatrix());
    D3DXFROMWINEMatrixMultiply( &mWorldView, &mWorld, g_Camera.GetViewMatrix());
    D3DXFROMWINEMatrixMultiply( &mWorldViewProj, &mWorldView, g_Camera.GetProjMatrix() );
    g_pWorldViewProj->SetMatrix( (float*)&mWorldViewProj );

    // Light vec in object space of the arrow 
    D3DXFROMWINEMATRIX mWorldInverse;
    D3DXFROMWINEMatrixInverse(&mWorldInverse, 0, &mWorld);
    vectorMatrixMultiply(&lightInObjectSpace, mWorldInverse, vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);
   
    //render the arrow
    IRenderer::GetRendererInstance()->IASetInputLayout(g_pVertexLayoutArrow);
    g_MeshArrow.Render(IRenderer::GetRendererInstance(),g_pTechniqueRenderArrow);


    //------------------------------------------------------------------------------------
    //render fur
    //------------------------------------------------------------------------------------
    
    // Update matrices and vectors used in the fur shaders

    // World matrix
    g_World = *(g_Camera.GetWorldMatrix());
    g_pWorldVariable->SetMatrix( (float*)&g_World );
    // World*View*Proj matrix
    D3DXFROMWINEMatrixMultiply( &mWorldView, &g_World, g_Camera.GetViewMatrix() );
    D3DXFROMWINEMatrixMultiply( &mWorldViewProj, &mWorldView, g_Camera.GetProjMatrix() );
    g_pWorldViewProj->SetMatrix( (float*)&mWorldViewProj );
    // Eye vec in object space
    vecEye = *g_Camera.GetEyePt();
    D3DXFROMWINEMATRIX worldInverse;
    D3DXFROMWINEMatrixInverse(&worldInverse,0,&g_World);
    vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
    g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);
    // Light vec in object space    
    vectorMatrixMultiply(&lightInObjectSpace, worldInverse, vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

    //set the desired raster state
    g_bToggleWireFrame ? IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateWireFrame): IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateSolid);

    //render the mesh, shells, and the fins
    IRenderer::GetRendererInstance()->IASetInputLayout( g_pVertexLayout );

    //render the mesh
    g_pShellNumberShaderVariable->SetInt(0); 
    g_Mesh.Render( IRenderer::GetRendererInstance(), g_pTechniqueRenderMesh );

	//render the fins
	if(g_bShowFins)
	{   
		//no culling for the fins
	    g_bToggleWireFrame ? IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateWireFrame): IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateSolidNoCull);
     
      	unsigned int stride = g_Mesh.m_uStride;
		unsigned int offset = 0;
		IRenderer::GetRendererInstance()->IASetPrimitiveTopology(PRIM_LINES /* _ADJ */);
		IRenderer::GetRendererInstance()->setVertexBuffer(0, 1, &g_MeshVB, &stride, &offset);
        IRenderer::GetRendererInstance()->setIndexBuffer(g_MeshLineAdjIB, DXGI_FORMAT_R16_UINT, 0);
        g_pTechniqueRenderFinsLineAdj->GetPassByIndex(0)->Apply(0);
        IRenderer::GetRendererInstance()->DrawIndexed(g_MeshLineAdjIndices, 0, 0);

	}
	

    //render shells (0-15)
    if(g_bShowShells)
	{
	    g_bToggleWireFrame ? IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateWireFrame): IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateSolid);
 		for(int i=1;i<=g_numShells;i++)
        {
            g_pShellNumberShaderVariable->SetInt(i); 
            g_Mesh.Render( IRenderer::GetRendererInstance(), g_pTechniqueRenderShells );
        }
	}


    //reset the raster state to solid for rendering the UI 
    IRenderer::GetRendererInstance()->RSSetState(g_pRasterStateSolid);

    TextureID* pNULLRT = {0};
    IRenderer::GetRendererInstance()->OMSetRenderTargets( 1, &(pNULLRT), 0 );
}

//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void OnD3D10DestroyDevice( void* pUserContext )
{
    //SAFE_RELEASE( g_pEffect );
    //SAFE_RELEASE( g_pVertexLayout );
    //SAFE_RELEASE( g_pVertexLayoutArrow );
    SAFE_DELETE( g_pTxtHelper );
    //SAFE_RELEASE( g_pRasterStateWireFrame);
    //SAFE_RELEASE( g_pRasterStateSolid);
    //SAFE_RELEASE( g_pRasterStateSolidNoCull);
    //SAFE_RELEASE( g_MeshLineAdjIB);
    //SAFE_RELEASE( g_MeshVB );
    g_Mesh.Destroy();
    g_MeshArrow.Destroy();

}

//--------------------------------------------------------------------------------------
// Helper functions
//--------------------------------------------------------------------------------------

//load a single texture and associated view
int loadTextureFromFile(char file,char shaderTextureName, ID3D10Device* IRenderer::GetRendererInstance())
{

    int hr;

    D3DXFROMWINE10_IMAGE_LOAD_INFO texLoadInfo;
    texLoadInfo.MipLevels = 8;
    texLoadInfo.MipFilter = D3DXFROMWINE10_FILTER_TRIANGLE;
    texLoadInfo.Filter = D3DXFROMWINE10_FILTER_TRIANGLE;
    ID3D10Resource *pRes = 0;

    char str[MAX_PATH];
    (NVUTFindDXSDKMediaFileCch(str, MAX_PATH, file));

    ( D3DXFROMWINE10CreateTextureFromFile(IRenderer::GetRendererInstance(), str, &texLoadInfo, 0, &pRes, &hr ) );
    if( pRes )
    {
        TextureID* texture;
        TextureID* textureRview;
        ID3D10EffectShaderResourceVariable* textureRVar;

        pRes->QueryInterface( __uuidof( TextureID ), (LPVOID*)&texture );
        SURFACE_DESC desc;
        texture->GetDesc( &desc );
        D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
        SRVDesc.Format = desc.Format;
        SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MostDetailedMip = 0;
        SRVDesc.Texture2D.MipLevels = desc.MipLevels;
        (IRenderer::GetRendererInstance()->CreateShaderResourceView( texture, &SRVDesc, &textureRview));
        textureRVar = g_pEffect->GetVariableByName( shaderTextureName )->AsShaderResource();
        textureRVar->SetResource( textureRview );

        //SAFE_RELEASE( texture );
        //SAFE_RELEASE( textureRview );
    }

    //SAFE_RELEASE( pRes );

    return 0;
}

// loads a texture array and associated view from a series of textures on disk
int LoadTextureArray( ID3D10Device* IRenderer::GetRendererInstance(), std::string* szTextureNames, int iNumTextures, TextureID** ppTex2D, TextureID** ppSRV)
{
    int hr = 0;
    SURFACE_DESC desc;
    ZeroMemory( &desc, sizeof(SURFACE_DESC) );

    char str[MAX_PATH];
    for(int i=0; i<iNumTextures; i++)
    {
        ( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, szTextureNames[i] ) );

        ID3D10Resource *pRes = 0;
        D3DXFROMWINE10_IMAGE_LOAD_INFO loadInfo;
        ZeroMemory( &loadInfo, sizeof( D3DXFROMWINE10_IMAGE_LOAD_INFO ) );
        loadInfo.Width = D3DXFROMWINE_FROM_FILE;
        loadInfo.Height  = D3DXFROMWINE_FROM_FILE;
        loadInfo.Depth  = D3DXFROMWINE_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = 1;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DXFROMWINE10_FILTER_NONE;
        loadInfo.MipFilter = D3DXFROMWINE10_FILTER_NONE;

        (D3DXFROMWINE10CreateTextureFromFile( IRenderer::GetRendererInstance(), str, &loadInfo, 0, &pRes, &hr ));
        if( pRes )
        {
            TextureID* pTemp;
            pRes->QueryInterface( __uuidof( TextureID ), (LPVOID*)&pTemp );
            pTemp->GetDesc( &desc );


            if(DXGI_FORMAT_R8G8B8A8_UNORM != desc.Format)
                return false;

            if(!(*ppTex2D))
            {
                desc.Usage = D3D10_USAGE_DEFAULT;
                desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
                desc.CPUAccessFlags = 0;
                desc.ArraySize = iNumTextures;
                (IRenderer::GetRendererInstance()->CreateTexture2D( &desc, 0, ppTex2D));
            }

            D3D10_MAPPED_TEXTURE2D mappedTex2D;
            for(unsigned int iMip=0; iMip < desc.MipLevels; iMip++)
            {
                pTemp->Map( iMip, D3D10_MAP_READ, 0, &mappedTex2D );
                    
                IRenderer::GetRendererInstance()->UpdateSubresource( (*ppTex2D), 
                                                D3D10CalcSubresource( iMip, i, desc.MipLevels ),
                                                0,
                                                mappedTex2D.pData,
                                                mappedTex2D.RowPitch,
                                                0 );

                pTemp->Unmap( iMip );
            }

            //SAFE_RELEASE( pRes );
            //SAFE_RELEASE( pTemp );

        }
        else
        {
            return false;
        }
    }


    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = desc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
    SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
    SRVDesc.Texture2DArray.ArraySize = iNumTextures;
    (IRenderer::GetRendererInstance()->CreateShaderResourceView( *ppTex2D, &SRVDesc, ppSRV ));


    return hr;
}


void vectorMatrixMultiply(D3DXFROMWINEVECTOR3* vecOut,const D3DXFROMWINEMATRIX matrix,const D3DXFROMWINEVECTOR3 vecIn)
{
    vecOut->x = vecIn.x*matrix(0,0) + vecIn.y*matrix(1,0) + vecIn.z*matrix(2,0) + matrix(3,0);
    vecOut->y = vecIn.x*matrix(0,1) + vecIn.y*matrix(1,1) + vecIn.z*matrix(2,1) + matrix(3,1);
    vecOut->z = vecIn.x*matrix(0,2) + vecIn.y*matrix(1,2) + vecIn.z*matrix(2,2) + matrix(3,2);
}


class lineAdjacency
{
public:
	short vertex1;
	short vertex2;
	short opposite1;
	short opposite2;
	lineAdjacency(){};
	lineAdjacency(int v1, int v2, int o1, int o2){ vertex1=short(v1); vertex2=short(v2); opposite1=short(o1); opposite2=short(o2);};
	bool operator==(lineAdjacency l2);
};

bool lineAdjacency::operator ==(lineAdjacency l2)
{
	if(( l2.vertex1==vertex1 && l2.vertex2==vertex2) || (l2.vertex1==vertex2 && l2.vertex2==vertex1))
	    if(( l2.opposite1==opposite1 && l2.opposite2==opposite2) || (l2.opposite1==opposite2 && l2.opposite2==opposite1))
			return true;
	return false;
}

typedef vector<lineAdjacency> lineAdjacencyVector;

//create an index buffer for reprensenting edges and opposite vertices as line adjacency
//this technique is assuming that the original index buffer of the mesh is in the triangle adjacency format
int CreateLineAdjIndexBuffer(ID3D10Device* IRenderer::GetRendererInstance())
{

	int hr = 0;
    int* indices;
	ID3DX10MeshBuffer* indexBuffer;
    (g_Mesh.m_pMesh10->GetIndexBuffer(&indexBuffer));
    (indexBuffer->Map((void**)&indices, 0));

	int numTriangles = g_Mesh.m_pMesh10->GetFaceCount();
    
	vector<lineAdjacency> adj;
	
	int numVertices = g_Mesh.m_pMesh10->GetVertexCount();
	vector<lineAdjacencyVector> candidates;
	for( int i=0;i<numVertices;i++ )
 	{  
		vector<lineAdjacency> laV;
		laV.reserve(10);
		candidates.push_back(laV);
	}


	bool found;
	int index;
	for (int t = 0; t < numTriangles; ++t, indices += 6) 
	{   
		found=false;
		lineAdjacency l1(indices[0],indices[2],indices[1],indices[4]);
		index = min(l1.vertex1,l1.vertex2);
		for(unsigned int i=0;i<candidates.at(index).size();i++)
		{
			if( l1 == candidates.at(index).at(i) )
				found=true;
		}
		if(!found)
		{
			adj.push_back(l1);
			candidates.at(index).push_back(l1);
		}

		
		found=false;
		lineAdjacency l2(indices[2],indices[4],indices[3],indices[0]);
		index = min(l2.vertex1,l2.vertex2);
		for(unsigned int i=0;i<candidates.at(index).size();i++)
		{
			if( l2 == candidates.at(index).at(i) )
				found=true;
		}
		if(!found)
		{
			adj.push_back(l2);
			candidates.at(index).push_back(l2);
		}


		found=false;
		lineAdjacency l3(indices[4],indices[0],indices[2],indices[5]);
		index = min(l3.vertex1,l3.vertex2);
		for(unsigned int i=0;i<candidates.at(index).size();i++)
		{
			if( l3 == candidates.at(index).at(i) )
				found=true;
		}
		if(!found)
		{
			adj.push_back(l3);
			candidates.at(index).push_back(l3);
		}
    }

    indexBuffer->Release();
	//SAFE_RELEASE(indexBuffer);

	//make an index buffer from the data above
	D3D10_SUBRESOURCE_DATA initialData;
	g_MeshLineAdjIndices = int(adj.size())*4;
	short (*newIndices) = new short[g_MeshLineAdjIndices];
	int ind = 0;
	for(unsigned int i=0;i<adj.size();i++)
	{  
		newIndices[ind++] = short(adj.at(i).vertex1);
		newIndices[ind++] = short(adj.at(i).vertex2);
		newIndices[ind++] = short(adj.at(i).opposite1);
		newIndices[ind++] = short(adj.at(i).opposite2);
	}    
    
    initialData.pSysMem = newIndices;
    D3D10_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = g_MeshLineAdjIndices * sizeof(short);
    bufferDesc.Usage = D3D10_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    (IRenderer::GetRendererInstance()->CreateBuffer(&bufferDesc, &initialData, &g_MeshLineAdjIB));
    delete [] newIndices;

  	g_Mesh.m_pMesh10->GetDeviceVertexBuffer(0,&g_MeshVB);

	return hr;
	
}



virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
    GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
    if(radioButton->isPressed() && radioButton->isChecked())
    {
		if(callbackString == "")
      	{
		}
	}
	}
}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("");
	IInput*    i=STX_Service::GetInputInstance();
	App app;
	app.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		app.render();
	}
	return 0;
}
    