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
CModelViewerCamera          g_Camera;               // A model viewing camera
ID3D10Effect*               g_pEffect = NULL;       // D3DX effect interface
ID3D10InputLayout*          g_pVertexLayout = NULL; // Vertex Layout for mesh
ID3D10InputLayout*          g_pVertexLayoutArrow = NULL; // Vertex Layout for arrow
bool                        g_bShowHelp = false;    // show help menu

CDXUTMesh10                 g_Mesh;
CDXUTMesh10                 g_MeshArrow;
ID3D10Buffer*               g_MeshLineAdjIB;
ID3D10Buffer*               g_MeshVB;
int                         g_MeshLineAdjIndices;


//effect variables
ID3D10EffectMatrixVariable* g_pWorldVariable                 = NULL;
ID3D10EffectMatrixVariable* g_pWorldViewProj                 = NULL;
ID3D10EffectVectorVariable* g_pEyeVec                        = NULL;
ID3D10EffectVectorVariable* g_pLightVec                      = NULL; 
ID3D10EffectScalarVariable* g_pShellNumberShaderVariable     = NULL;
ID3D10EffectScalarVariable* g_pNumShellsNumberShaderVariable = NULL;
ID3D10EffectScalarVariable* g_pShellIncrementShaderVariable  = NULL;
ID3D10EffectScalarVariable* g_pMaxOpacityShaderVariable      = NULL;
ID3D10EffectScalarVariable* g_pCombStrengthShaderVariable    = NULL;
ID3D10EffectVectorVariable* g_pCombVectorShaderVariable      = NULL;

//raster states for toggeling between wireframe and solid rendering mode, and enabling multisampling
ID3D10RasterizerState * g_pRasterStateWireFrame;
ID3D10RasterizerState * g_pRasterStateSolid;
ID3D10RasterizerState * g_pRasterStateSolidNoCull;

//techniques
ID3D10EffectTechnique*  g_pTechniqueRenderMesh            = NULL;
ID3D10EffectTechnique*  g_pTechniqueRenderShells          = NULL;
ID3D10EffectTechnique*  g_pTechniqueRenderFinsLineAdj     = NULL;
ID3D10EffectTechnique*  g_pTechniqueRenderArrow           = NULL;

D3DXVECTOR3 vecEye;
D3DXVECTOR3 vecLight;
D3DXVECTOR3 eyeInObjectSpace;
D3DXVECTOR3 lightInObjectSpace;
D3DXVECTOR3 g_modelCentroid;
D3DXMATRIX  g_World;

bool                        g_bToggleWireFrame = false;
int                         g_numShells = 15;
float                       g_shellIncrement = 0.2f;
bool                        g_bShowShells = true;
bool                        g_bShowFins = true;
float                       g_maxOpacity = 0.6f;
D3DXVECTOR3                 g_combVector = D3DXVECTOR3(1.0f,-0.3f,0.0f);
float                       g_combStrength = 0.3f;

//textures for rendering fur 
LPCTSTR g_furTextureNames[] = 
{
      L"../../Media/FurTexture/FurTexture00.dds",
      L"../../Media/FurTexture/FurTexture01.dds",
      L"../../Media/FurTexture/FurTexture02.dds",
      L"../../Media/FurTexture/FurTexture03.dds",
      L"../../Media/FurTexture/FurTexture04.dds",
      L"../../Media/FurTexture/FurTexture05.dds",
      L"../../Media/FurTexture/FurTexture06.dds",
      L"../../Media/FurTexture/FurTexture07.dds",
      L"../../Media/FurTexture/FurTexture08.dds",
      L"../../Media/FurTexture/FurTexture09.dds",
      L"../../Media/FurTexture/FurTexture10.dds",
      L"../../Media/FurTexture/FurTexture11.dds",
      L"../../Media/FurTexture/FurTexture12.dds",
      L"../../Media/FurTexture/FurTexture13.dds",
      L"../../Media/FurTexture/FurTexture14.dds",
      L"../../Media/FurTexture/FurTexture15.dds"
};

LPCTSTR g_furOffsetNames[] = 
{
      L"../../Media/FurTexture/FurTextureOffset00.dds",
      L"../../Media/FurTexture/FurTextureOffset01.dds",
      L"../../Media/FurTexture/FurTextureOffset02.dds",
      L"../../Media/FurTexture/FurTextureOffset03.dds",
      L"../../Media/FurTexture/FurTextureOffset04.dds",
      L"../../Media/FurTexture/FurTextureOffset05.dds",
      L"../../Media/FurTexture/FurTextureOffset06.dds",
      L"../../Media/FurTexture/FurTextureOffset07.dds",
      L"../../Media/FurTexture/FurTextureOffset08.dds",
      L"../../Media/FurTexture/FurTextureOffset09.dds",
      L"../../Media/FurTexture/FurTextureOffset10.dds",
      L"../../Media/FurTexture/FurTextureOffset11.dds",
      L"../../Media/FurTexture/FurTextureOffset12.dds",
      L"../../Media/FurTexture/FurTextureOffset13.dds",
      L"../../Media/FurTexture/FurTextureOffset14.dds",
      L"../../Media/FurTexture/FurTextureOffset15.dds"
};


struct SimpleVertex
{
    D3DXVECTOR3 Pos;
    D3DXVECTOR2 Tex;
};

//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice()
{
    HRESULT hr = S_OK;
    
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
    hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolid );
    pd3dDevice->RSSetState(g_pRasterStateSolid);  
    rasterizerState.CullMode = D3D10_CULL_NONE;
    hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateSolidNoCull );
	rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
    rasterizerState.CullMode = D3D10_CULL_NONE;
    hr = pd3dDevice->CreateRasterizerState( &rasterizerState, &g_pRasterStateWireFrame );

    //Create the effect
    WCHAR str[MAX_PATH];
    V_RETURN( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, L"FurShellsAndFins.fx" ) );
    DWORD dwShaderFlags = 0;
    V_RETURN( D3DX10CreateEffectFromFile(str, NULL, NULL, "fx_4_0", dwShaderFlags, 0, pd3dDevice, NULL, NULL, &g_pEffect, NULL, &hr ) );

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
    D3DXVECTOR3 comb( g_combVector.x, g_combVector.y, g_combVector.z );
    D3DXVec3Normalize(&g_combVector,&g_combVector);
    g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);
    V_RETURN( CDXUTDirectionWidget::StaticOnD3D10CreateDevice( pd3dDevice ) );

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
    UINT numElements = sizeof(layout)/sizeof(layout[0]);
    D3D10_PASS_DESC PassDesc;
    g_pTechniqueRenderShells->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    V_RETURN( pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pVertexLayout ) );

    // Set the input layout
    pd3dDevice->IASetInputLayout( g_pVertexLayout );

    // Load the mesh
    const D3D10_INPUT_ELEMENT_DESC layoutLoading[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },  
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },   
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
        { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D10_INPUT_PER_VERTEX_DATA, 0 }
    };

    V_RETURN( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, L"catLarge.x" ) ); 
    V_RETURN( g_Mesh.Create( pd3dDevice, str, (D3D10_INPUT_ELEMENT_DESC*)layoutLoading, numElements,false ) );

    g_Mesh.ConvertToAdjacencyIndices();    //generate triangle adjacency for triangle adjacency method of fin rendering
	CreateLineAdjIndexBuffer(pd3dDevice);  //generate line adjacency for line adjacency method of fin rendering
	

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
    V_RETURN( pd3dDevice->CreateInputLayout( layoutArrow, numElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &g_pVertexLayoutArrow ) );

    V_RETURN( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, L"arrow.x" ) ); 
    V_RETURN( g_MeshArrow.Create( pd3dDevice, str, (D3D10_INPUT_ELEMENT_DESC*)layoutArrow, numElements ) );

    //-------------------------------------------------------------------------------------------------
 
    // Initialize the camera
    g_modelCentroid = D3DXVECTOR3(0.0f, 27.0f, 0.0f);
    vecEye = D3DXVECTOR3( 0.0f, 0.0f, -120.0f);
    g_Camera.SetViewParams( &vecEye, &g_modelCentroid );
    g_Camera.SetEnablePositionMovement(true);
    g_Camera.SetScalers(0.001f,20.f);
    g_Camera.SetModelCenter( g_modelCentroid );

    // Initialize the world matrices
    D3DXMatrixIdentity( &g_World );
    g_Camera.SetWorldMatrix( g_World );
    g_World = *(g_Camera.GetWorldMatrix());

    //initialize eye in object space vector  
    D3DXMATRIX worldInverse;
    D3DXMatrixInverse(&worldInverse,NULL,&g_World);
    vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
    g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);

    //initialize light in object space vector
    vecLight = D3DXVECTOR3(-100.0f,300.0f,-200.0f);
    vectorMatrixMultiply(&lightInObjectSpace,worldInverse,vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

    //-------------------------------------------------------------------------------------------------
    //load the textures

    //fin texture
    V_RETURN(loadTextureFromFile(L"../../Media/FurTexture/FurTextureFin.dds","finTexture",pd3dDevice)); 
    //fin offset
    V_RETURN(loadTextureFromFile(L"../../Media/FurTexture/FurTextureOffsetFin.dds","finOffset",pd3dDevice));  
    //fur color base
    V_RETURN(loadTextureFromFile(L"../../Media/catColor.dds","colorTexture",pd3dDevice)); 
    
    //fur texture array
    //load all the textures for the fins into one texture array
    ID3D10Texture2D* furTexture = NULL;
    ID3D10ShaderResourceView* furTextureRV = NULL;
    ID3D10EffectShaderResourceVariable*   furTextureArray = g_pEffect->GetVariableByName( "furTextureArray" )->AsShaderResource();
    V_RETURN( LoadTextureArray( pd3dDevice, g_furTextureNames, sizeof(g_furTextureNames)/sizeof(g_furTextureNames[0]), &furTexture, &furTextureRV) );
    furTextureArray->SetResource( furTextureRV );

    SAFE_RELEASE( furTexture );
    SAFE_RELEASE( furTextureRV );
    furTexture = NULL;
    furTextureRV = NULL;

    //fur offset texture array
    //load all the offset textures for the fins into one texture array
    furTextureArray = g_pEffect->GetVariableByName( "furOffsetArray" )->AsShaderResource();
    V_RETURN( LoadTextureArray( pd3dDevice, g_furOffsetNames, sizeof(g_furOffsetNames)/sizeof(g_furOffsetNames[0]), &furTexture, &furTextureRV) );
    furTextureArray->SetResource( furTextureRV );

    SAFE_RELEASE( furTexture );
    SAFE_RELEASE( furTextureRV );
    furTexture = NULL;
    furTextureRV = NULL;


    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain()
{
    HRESULT hr = S_OK;
    
    //unbind the current backbuffer and depth buffer since they will be deleted
    ID3D10RenderTargetView* pNULLRT = {NULL};
    pd3dDevice->OMSetRenderTargets( 1, &(pNULLRT), NULL );

    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBufferSurfaceDesc ) );
    V_RETURN( g_D3DSettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBufferSurfaceDesc ) );

    // Setup the camera's parameters
    float fov = D3DX_PI/4;
    float znear = 1.0f;
    float zfar  = 5000.0f;
    float fAspectRatio = static_cast<float>(pBufferSurfaceDesc->Width) / static_cast<float>(pBufferSurfaceDesc->Height);
    g_Camera.SetProjParams( fov , fAspectRatio, znear , zfar );

    return hr;
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{    

    //get the current render target and depth stencil view and clear them
    ID3D10RenderTargetView* pOrigRT = DXUTGetD3D10RenderTargetView();
    ID3D10DepthStencilView* pOrigDS = DXUTGetD3D10DepthStencilView();
    float ClearColor[4] = { 0.1176f, 0.1176f, 0.1176f, 0.0f }; 
    pd3dDevice->ClearRenderTargetView( pOrigRT, ClearColor );
    pd3dDevice->ClearDepthStencilView( pOrigDS, D3D10_CLEAR_DEPTH, 1.0, 0 );
    pd3dDevice->OMSetRenderTargets( 1, &(pOrigRT), pOrigDS );

    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        return;
    }

    D3DXMATRIX mWorldView;
    D3DXMATRIX mWorldViewProj;

    //------------------------------------------------------------------------------------
    //render the comb directional control, and update the value of the comb vector
    //------------------------------------------------------------------------------------

    g_combVector = g_combDirectionWidget.GetLightDirection();
    D3DXVec3Normalize(&g_combVector,&g_combVector);
    g_pCombVectorShaderVariable->SetFloatVector((float*)&g_combVector);

    //calculate and set the world view projection matrix for transforming the arrow
    float arrowScale = 0.3f;
    D3DXMATRIX mScale;
    D3DXMatrixScaling( &mScale, arrowScale, arrowScale, arrowScale);
    D3DXMATRIX mLookAtMatrix;
    D3DXVECTOR3 mArrowPos =  g_modelCentroid - g_combVector * g_combDirectionWidget.GetRadius();
    D3DXVECTOR3 mOrigin = D3DXVECTOR3(0,0,0);
    D3DXVECTOR3 mUp = D3DXVECTOR3(0, 1, 0);
    D3DXMatrixLookAtLH(&mLookAtMatrix, &mArrowPos, &mOrigin, &mUp);
    D3DXMATRIX mLookAtInv;
    D3DXMatrixInverse(&mLookAtInv, NULL, &mLookAtMatrix);
    D3DXMATRIX mWorld = mScale * mLookAtInv * (*g_Camera.GetWorldMatrix());
    D3DXMatrixMultiply( &mWorldView, &mWorld, g_Camera.GetViewMatrix());
    D3DXMatrixMultiply( &mWorldViewProj, &mWorldView, g_Camera.GetProjMatrix() );
    g_pWorldViewProj->SetMatrix( (float*)&mWorldViewProj );

    // Light vec in object space of the arrow 
    D3DXMATRIX mWorldInverse;
    D3DXMatrixInverse(&mWorldInverse, NULL, &mWorld);
    vectorMatrixMultiply(&lightInObjectSpace, mWorldInverse, vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);
   
    //render the arrow
    pd3dDevice->IASetInputLayout(g_pVertexLayoutArrow);
    g_MeshArrow.Render(pd3dDevice,g_pTechniqueRenderArrow);


    //------------------------------------------------------------------------------------
    //render fur
    //------------------------------------------------------------------------------------
    
    // Update matrices and vectors used in the fur shaders

    // World matrix
    g_World = *(g_Camera.GetWorldMatrix());
    g_pWorldVariable->SetMatrix( (float*)&g_World );
    // World*View*Proj matrix
    D3DXMatrixMultiply( &mWorldView, &g_World, g_Camera.GetViewMatrix() );
    D3DXMatrixMultiply( &mWorldViewProj, &mWorldView, g_Camera.GetProjMatrix() );
    g_pWorldViewProj->SetMatrix( (float*)&mWorldViewProj );
    // Eye vec in object space
    vecEye = *g_Camera.GetEyePt();
    D3DXMATRIX worldInverse;
    D3DXMatrixInverse(&worldInverse,NULL,&g_World);
    vectorMatrixMultiply(&eyeInObjectSpace,worldInverse,vecEye);
    g_pEyeVec->SetFloatVector((float*)eyeInObjectSpace);
    // Light vec in object space    
    vectorMatrixMultiply(&lightInObjectSpace, worldInverse, vecLight);
    g_pLightVec->SetFloatVector((float*)lightInObjectSpace);

    //set the desired raster state
    g_bToggleWireFrame ? pd3dDevice->RSSetState(g_pRasterStateWireFrame): pd3dDevice->RSSetState(g_pRasterStateSolid);

    //render the mesh, shells, and the fins
    pd3dDevice->IASetInputLayout( g_pVertexLayout );

    //render the mesh
    g_pShellNumberShaderVariable->SetInt(0); 
    g_Mesh.Render( pd3dDevice, g_pTechniqueRenderMesh );

	//render the fins
	if(g_bShowFins)
	{   
		//no culling for the fins
	    g_bToggleWireFrame ? pd3dDevice->RSSetState(g_pRasterStateWireFrame): pd3dDevice->RSSetState(g_pRasterStateSolidNoCull);
     
      	unsigned int stride = g_Mesh.m_uStride;
		unsigned int offset = 0;
		pd3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
		pd3dDevice->IASetVertexBuffers(0, 1, &g_MeshVB, &stride, &offset);
        pd3dDevice->IASetIndexBuffer(g_MeshLineAdjIB, DXGI_FORMAT_R16_UINT, 0);
        g_pTechniqueRenderFinsLineAdj->GetPassByIndex(0)->Apply(0);
        pd3dDevice->DrawIndexed(g_MeshLineAdjIndices, 0, 0);

	}
	

    //render shells (0-15)
    if(g_bShowShells)
	{
	    g_bToggleWireFrame ? pd3dDevice->RSSetState(g_pRasterStateWireFrame): pd3dDevice->RSSetState(g_pRasterStateSolid);
 		for(int i=1;i<=g_numShells;i++)
        {
            g_pShellNumberShaderVariable->SetInt(i); 
            g_Mesh.Render( pd3dDevice, g_pTechniqueRenderShells );
        }
	}


    //reset the raster state to solid for rendering the UI 
    pd3dDevice->RSSetState(g_pRasterStateSolid);

    ID3D10RenderTargetView* pNULLRT = {NULL};
    pd3dDevice->OMSetRenderTargets( 1, &(pNULLRT), NULL );
}

//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
    SAFE_RELEASE( g_pEffect );
    SAFE_RELEASE( g_pVertexLayout );
    SAFE_RELEASE( g_pVertexLayoutArrow );
    SAFE_DELETE( g_pTxtHelper );
    SAFE_RELEASE( g_pRasterStateWireFrame);
    SAFE_RELEASE( g_pRasterStateSolid);
    SAFE_RELEASE( g_pRasterStateSolidNoCull);
    SAFE_RELEASE( g_MeshLineAdjIB);
    SAFE_RELEASE( g_MeshVB );
    g_Mesh.Destroy();
    g_MeshArrow.Destroy();

}

//--------------------------------------------------------------------------------------
// Helper functions
//--------------------------------------------------------------------------------------

//load a single texture and associated view
HRESULT loadTextureFromFile(LPCWSTR file,LPCSTR shaderTextureName, ID3D10Device* pd3dDevice)
{

    HRESULT hr;

    D3DX10_IMAGE_LOAD_INFO texLoadInfo;
    texLoadInfo.MipLevels = 8;
    texLoadInfo.MipFilter = D3DX10_FILTER_TRIANGLE;
    texLoadInfo.Filter = D3DX10_FILTER_TRIANGLE;
    ID3D10Resource *pRes = NULL;

    WCHAR str[MAX_PATH];
    V_RETURN(NVUTFindDXSDKMediaFileCch(str, MAX_PATH, file));

    V_RETURN( D3DX10CreateTextureFromFile(pd3dDevice, str, &texLoadInfo, NULL, &pRes, &hr ) );
    if( pRes )
    {
        ID3D10Texture2D* texture;
        ID3D10ShaderResourceView* textureRview;
        ID3D10EffectShaderResourceVariable* textureRVar;

        pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&texture );
        D3D10_TEXTURE2D_DESC desc;
        texture->GetDesc( &desc );
        D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
        SRVDesc.Format = desc.Format;
        SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MostDetailedMip = 0;
        SRVDesc.Texture2D.MipLevels = desc.MipLevels;
        V_RETURN (pd3dDevice->CreateShaderResourceView( texture, &SRVDesc, &textureRview));
        textureRVar = g_pEffect->GetVariableByName( shaderTextureName )->AsShaderResource();
        textureRVar->SetResource( textureRview );

        SAFE_RELEASE( texture );
        SAFE_RELEASE( textureRview );
    }

    SAFE_RELEASE( pRes );

    return S_OK;
}

// loads a texture array and associated view from a series of textures on disk
HRESULT LoadTextureArray( ID3D10Device* pd3dDevice, LPCTSTR* szTextureNames, int iNumTextures, ID3D10Texture2D** ppTex2D, ID3D10ShaderResourceView** ppSRV)
{
    HRESULT hr = S_OK;
    D3D10_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof(D3D10_TEXTURE2D_DESC) );

    WCHAR str[MAX_PATH];
    for(int i=0; i<iNumTextures; i++)
    {
        V_RETURN( NVUTFindDXSDKMediaFileCch( str, MAX_PATH, szTextureNames[i] ) );

        ID3D10Resource *pRes = NULL;
        D3DX10_IMAGE_LOAD_INFO loadInfo;
        ZeroMemory( &loadInfo, sizeof( D3DX10_IMAGE_LOAD_INFO ) );
        loadInfo.Width = D3DX_FROM_FILE;
        loadInfo.Height  = D3DX_FROM_FILE;
        loadInfo.Depth  = D3DX_FROM_FILE;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = 1;
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_NONE;
        loadInfo.MipFilter = D3DX10_FILTER_NONE;

        V_RETURN(D3DX10CreateTextureFromFile( pd3dDevice, str, &loadInfo, NULL, &pRes, &hr ));
        if( pRes )
        {
            ID3D10Texture2D* pTemp;
            pRes->QueryInterface( __uuidof( ID3D10Texture2D ), (LPVOID*)&pTemp );
            pTemp->GetDesc( &desc );


            if(DXGI_FORMAT_R8G8B8A8_UNORM != desc.Format)
                return false;

            if(!(*ppTex2D))
            {
                desc.Usage = D3D10_USAGE_DEFAULT;
                desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
                desc.CPUAccessFlags = 0;
                desc.ArraySize = iNumTextures;
                V_RETURN(pd3dDevice->CreateTexture2D( &desc, NULL, ppTex2D));
            }

            D3D10_MAPPED_TEXTURE2D mappedTex2D;
            for(UINT iMip=0; iMip < desc.MipLevels; iMip++)
            {
                pTemp->Map( iMip, D3D10_MAP_READ, 0, &mappedTex2D );
                    
                pd3dDevice->UpdateSubresource( (*ppTex2D), 
                                                D3D10CalcSubresource( iMip, i, desc.MipLevels ),
                                                NULL,
                                                mappedTex2D.pData,
                                                mappedTex2D.RowPitch,
                                                0 );

                pTemp->Unmap( iMip );
            }

            SAFE_RELEASE( pRes );
            SAFE_RELEASE( pTemp );

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
    V_RETURN(pd3dDevice->CreateShaderResourceView( *ppTex2D, &SRVDesc, ppSRV ));


    return hr;
}


void vectorMatrixMultiply(D3DXVECTOR3* vecOut,const D3DXMATRIX matrix,const D3DXVECTOR3 vecIn)
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
HRESULT CreateLineAdjIndexBuffer(ID3D10Device* pd3dDevice)
{

	HRESULT hr = S_OK;
    int* indices;
	ID3DX10MeshBuffer* indexBuffer;
    V_RETURN(g_Mesh.m_pMesh10->GetIndexBuffer(&indexBuffer));
    V_RETURN(indexBuffer->Map((void**)&indices, 0));

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
		for(UINT i=0;i<candidates.at(index).size();i++)
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
		for(UINT i=0;i<candidates.at(index).size();i++)
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
		for(UINT i=0;i<candidates.at(index).size();i++)
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
	SAFE_RELEASE(indexBuffer);

	//make an index buffer from the data above
	D3D10_SUBRESOURCE_DATA initialData;
	g_MeshLineAdjIndices = int(adj.size())*4;
	short (*newIndices) = new short[g_MeshLineAdjIndices];
	int ind = 0;
	for(UINT i=0;i<adj.size();i++)
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
    V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &g_MeshLineAdjIB));
    delete [] newIndices;

  	g_Mesh.m_pMesh10->GetDeviceVertexBuffer(0,&g_MeshVB);

	return hr;
	
}
wWinMain

