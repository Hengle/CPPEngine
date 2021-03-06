//--------------------------------------------------------------------------------------
// File: Skinning10.cpp
//
// Basic starting point for new Direct3D 10 samples
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//include "resource.h"
/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/IRenderer.h>

#define MAX_BONE_MATRICES 255
#define MAX_SPRITES 500
#define MAX_INSTANCES 500

enum FETCH_TYPE
{
    FT_CONSTANTBUFFER = 0,
    FT_TEXTUREBUFFER,
    FT_TEXTURE,
    FT_BUFFER,
};

struct STREAM_OUT_VERTEX
{
    D3DXFROMWINEVECTOR4 Pos;
    D3DXFROMWINEVECTOR3 Norm;
    D3DXFROMWINEVECTOR2 Tex;
    D3DXFROMWINEVECTOR3 Tangent;
};

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
D3DXFROMWINEMATRIX                  g_Proj;
D3DXFROMWINEMATRIX                  g_View;
D3DXFROMWINEVECTOR3			g_EyePt;

MeshRenderer2                        g_SkinnedMesh;			// The skinned mesh
stx_Effect*                       g_pEffect10 = 0;
VertexFormatID                   g_pSkinnedVertexLayout = 0;
VertexFormatID                   g_pTransformedVertexLayout = 0;

stx_Effect::stx_Technique*              g_pRenderConstantBuffer = 0;
stx_Effect::stx_Technique*              g_pRenderTextureBuffer = 0;
stx_Effect::stx_Technique*              g_pRenderTexture = 0;
stx_Effect::stx_Technique*              g_pRenderBuffer = 0;

stx_Effect::stx_Technique*              g_pRenderConstantBuffer_SO = 0;
stx_Effect::stx_Technique*              g_pRenderTextureBuffer_SO = 0;
stx_Effect::stx_Technique*              g_pRenderTexture_SO = 0;
stx_Effect::stx_Technique*              g_pRenderBuffer_SO = 0;

stx_Effect::stx_Technique*              g_pRenderPostTransformed = 0;

stx_Effect::stx_Variable*         g_pmWorldViewProj = 0;
stx_Effect::stx_Variable*         g_pmWorld = 0;
stx_Effect::stx_Variable*         g_pvLightPos = 0;
stx_Effect::stx_Variable*         g_pvEyePt = 0;
stx_Effect::stx_Variable*         g_pmConstBoneWorld = 0;
stx_Effect::stx_Variable*         g_pmTexBoneWorld = 0;
stx_Effect::stx_Variable* g_pBoneBufferVar = 0;
stx_Effect::stx_Variable* g_ptxBoneTexture = 0;
stx_Effect::stx_Variable* g_ptxDiffuse = 0;
stx_Effect::stx_Variable* g_ptxNormal = 0;

VertexBufferID                        g_pBoneBuffer = 0;
TextureID            g_pBoneBufferRV = 0;
TextureID                     g_pBoneTexture = 0;
TextureID            g_pBoneTextureRV = 0;

VertexBufferID *                      g_ppTransformedVBs = 0;

D3DXFROMWINEVECTOR3                         g_vLightPos = D3DXFROMWINEVECTOR3( 159.47f, 74.23f, 103.60f );
FETCH_TYPE                          g_FetchType = FT_CONSTANTBUFFER;
bool                                g_bUseStreamOut = true;
unsigned int                                g_iInstances = 10;

struct App
{
    App(){}
    virtual ~App(){}

//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
int  OnD3D10CreateDevice()
{
    int hr;

    // Read the D3DXFROMWINE effect file
    g_pEffect10=IRenderer::GetRendererInstance()->addEffectFromFile("/Skinning10/Skinning10.fx");

    // Get effects techniques
    g_pRenderConstantBuffer = g_pEffect10->GetTechniqueByName( "RenderConstantBuffer" );
    g_pRenderTextureBuffer = g_pEffect10->GetTechniqueByName( "RenderTextureBuffer" );
    g_pRenderTexture = g_pEffect10->GetTechniqueByName( "RenderTexture" );
    g_pRenderBuffer = g_pEffect10->GetTechniqueByName( "RenderBuffer" );

    g_pRenderConstantBuffer_SO = g_pEffect10->GetTechniqueByName( "RenderConstantBuffer_SO" );
    g_pRenderTextureBuffer_SO = g_pEffect10->GetTechniqueByName( "RenderTextureBuffer_SO" );
    g_pRenderTexture_SO = g_pEffect10->GetTechniqueByName( "RenderTexture_SO" );
    g_pRenderBuffer_SO = g_pEffect10->GetTechniqueByName( "RenderBuffer_SO" );

    g_pRenderPostTransformed = g_pEffect10->GetTechniqueByName( "RenderPostTransformed" );

    // Get effects variables
    g_pmWorldViewProj = g_pEffect10->GetVariableByName( "g_mWorldViewProj" );
    g_pmWorld = g_pEffect10->GetVariableByName( "g_mWorld" );
    g_pvLightPos = g_pEffect10->GetVariableByName( "g_vLightPos" );
    g_pvEyePt = g_pEffect10->GetVariableByName( "g_vEyePt" );
    g_pmConstBoneWorld = g_pEffect10->GetVariableByName( "g_mConstBoneWorld" );
    g_pmTexBoneWorld = g_pEffect10->GetVariableByName( "g_mTexBoneWorld" );
    g_pBoneBufferVar = g_pEffect10->GetVariableByName( "g_BufferBoneWorld" );
    g_ptxBoneTexture = g_pEffect10->GetVariableByName( "g_txTexBoneWorld" );
    g_ptxDiffuse = g_pEffect10->GetVariableByName( "g_txDiffuse" );
    g_ptxNormal = g_pEffect10->GetVariableByName( "g_txNormal" );

    // Define our vertex data layout for skinned objects
	#if 0
    const D3D10_INPUT_ELEMENT_DESC skinnedlayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,    0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0,      12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "BONES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0,         16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,      20, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,       32, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,     40, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
	#else
	{FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 4,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 4,
		0, TYPE_NORMAL, FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
		0, TYPE_TANGENT, FORMAT_FLOAT, 3
	};
	g_pSkinnedVertexLayout = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), -1);}
	#endif
    // Define our vertex data layout for post-transformed objects
	{FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 4,
		0, TYPE_NORMAL, FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
		0, TYPE_TANGENT, FORMAT_FLOAT, 3
	};
	g_pTransformedVertexLayout = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), -1);}

    // Load the animated mesh
    g_SkinnedMesh.Load("Soldier/soldier.sdkmesh");
    // ??? g_SkinnedMesh.LoadAnimation( "Soldier/soldier.sdkmesh_anim" );
    D3DXFROMWINEMATRIX mIdentity;
    D3DXFROMWINEMatrixIdentity( &mIdentity );
    g_SkinnedMesh.TransformBindPose( &mIdentity );

    // Create a bone texture
    // It will be updated more than once per frame (in a typical game) so make it dynamic ???
	Image3 img;
	img.create(FORMAT_RGBA32f, MAX_BONE_MATRICES * 4, 1, 1, 1);
	g_pBoneTexture=IRenderer::GetRendererInstance()->addTexture(&img, false, IRenderer::GetRendererInstance()->Getlinear());

    // Create a bone matrix buffer
    // It will be updated more than once per frame (in a typical game) so make it dynamic
    g_pBoneBuffer=IRenderer::GetRendererInstance()->addVertexBuffer(MAX_BONE_MATRICES * sizeof( D3DXFROMWINEMATRIX ), DYNAMIC);

    // Again, we need a resource view to use it in the shader ???

    // Create VBs that will hold all of the skinned vertices that need to be streamed out
    g_ppTransformedVBs = new VertexBufferID [ g_SkinnedMesh.GetNumMeshes() ];
    if( !g_ppTransformedVBs )
        return 1;
    for( unsigned int m = 0; m < g_SkinnedMesh.GetNumMeshes(); m++ )
    {
        unsigned int iStreamedVertexCount = ( unsigned int )g_SkinnedMesh.GetNumVertices( m, 0 );
	g_ppTransformedVBs[m]=IRenderer::GetRendererInstance()->addVertexBuffer(iStreamedVertexCount * sizeof( STREAM_OUT_VERTEX ), STATIC); // ???
    }

    // Setup the camera's view parameters
    D3DXFROMWINEVECTOR3 vecEye( 2.0f, 0.5f, 2.0f );
    D3DXFROMWINEVECTOR3 vecAt ( 0.0f, 0.5f, -1.0f );
    D3DXFROMWINEVECTOR3 vecUp ( 0.0f, 1.0f, 0.0f );
    D3DXFROMWINEMatrixLookAtLH(&g_View, &vecEye, &vecAt, &vecUp );

    return 0;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
int  OnD3D10ResizedSwapChain()
{
    int hr;

    // Setup the camera's projection parameters
    float fAspectRatio = IRenderer::GetRendererInstance()->GetAspectRatio();
    D3DXFROMWINEMatrixPerspectiveFovLH(&g_Proj, D3DXFROMWINE_PI / 4, fAspectRatio, 0.1f, 1000.0f );

    return 0;
}


//--------------------------------------------------------------------------------------
// Get the world matrix for this particular mesh instance
//--------------------------------------------------------------------------------------
void GetInstanceWorldMatrix( unsigned int iInstance, D3DXFROMWINEMATRIX* pmWorld )
{
    int iSide = ( unsigned int )sqrtf( ( float )g_iInstances );

    int iX = iInstance % iSide;
    int iZ = iInstance / iSide;

    float xStart = -iSide * 0.25f;
    float zStart = -iSide * 0.25f;

    D3DXFROMWINEMatrixTranslation( pmWorld, xStart + iX * 0.5f, 0, zStart + iZ * 0.5f );
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void  OnD3D10FrameRender(double fTime, float fElapsedTime)
{
    D3DXFROMWINEMATRIX mWorldViewProj;
    D3DXFROMWINEMATRIX mViewProj;
    D3DXFROMWINEMATRIX mWorld;
    D3DXFROMWINEMATRIX mView;
    D3DXFROMWINEMATRIX mProj;

    // Clear the depth stencil
	IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.176f, 0.196f, 0.667f, 0.0f));

    // Get the projection & view matrix from the camera class
    D3DXFROMWINEMatrixIdentity( &mWorld );
    mProj = g_Proj;
    mView = g_View;
    mViewProj = mView * mProj;

    // Set general effect values
    g_pvLightPos->SetFloatVector( ( float* )&g_vLightPos );
    D3DXFROMWINEVECTOR3 vEye = g_EyePt;
    g_pvEyePt->SetFloatVector( ( float* )&vEye );

    // Set technique
    stx_Effect::stx_Technique* pTech = 0;
    if( g_bUseStreamOut )
    {
        switch( g_FetchType )
        {
            case FT_CONSTANTBUFFER:
                pTech = g_pRenderConstantBuffer_SO;
                break;
            case FT_TEXTUREBUFFER:
                pTech = g_pRenderTextureBuffer_SO;
                break;
            case FT_TEXTURE:
                pTech = g_pRenderTexture_SO;
                break;
            case FT_BUFFER:
                pTech = g_pRenderBuffer_SO;
                break;
        };
    }
    else
    {
        switch( g_FetchType )
        {
            case FT_CONSTANTBUFFER:
                pTech = g_pRenderConstantBuffer;
                break;
            case FT_TEXTUREBUFFER:
                pTech = g_pRenderTextureBuffer;
                break;
            case FT_TEXTURE:
                pTech = g_pRenderTexture;
                break;
            case FT_BUFFER:
                pTech = g_pRenderBuffer;
                break;
        };
    }

    VertexBufferID  pBuffers[1];
    unsigned int stride[1];
    unsigned int offset[1] = { 0 };

    if( g_bUseStreamOut )
    {
        //
        // Skin the vertices and stream them out
        //
        for( unsigned int m = 0; m < g_SkinnedMesh.GetNumMeshes(); m++ )
        {
            // Turn on stream out
            pBuffers[0] = g_ppTransformedVBs[m];
            // ??? IRenderer::GetRendererInstance()->SOSetTargets( 1, pBuffers, offset );

            // Set vertex Layout
            //IRenderer::GetRendererInstance()->setVertexFormat( g_pSkinnedVertexLayout );

            // Set source vertex buffer
            IRenderer::GetRendererInstance()->setVertexBuffer( 0, g_SkinnedMesh.GetVB10( m, 0 ) );
		// When skinning vertices, we don't care about topology.
            // Treat the entire vertex buffer as list of points.

            // Set the bone matrices
            // ??? SetBoneMatrices( g_FetchType, m );

            // Render the vertices as an array of points
            TECHNIQUE_DESC techDesc;
            pTech->GetDesc( &techDesc );
            for( unsigned int p = 0; p < techDesc.Passes; ++p )
            {
                pTech->GetPassByIndex( p )->Apply( 0 );
                IRenderer::GetRendererInstance()->Draw( PRIM_POINTS, ( unsigned int )g_SkinnedMesh.GetNumVertices( m, 0 ), 0 );
            }
        }

        // Turn off stream out
        pBuffers[0] = 0;
        // ??? IRenderer::GetRendererInstance()->SOSetTargets( 1, pBuffers, offset );


        //
        // Render the transformed mesh from the streamed out vertices
        //

        for( unsigned int m = 0; m < g_SkinnedMesh.GetNumMeshes(); m++ )
        {
            // Set IA parameters
            IRenderer::GetRendererInstance()->setVertexBuffer( 0, g_ppTransformedVBs[m] );
            IRenderer::GetRendererInstance()->setIndexBuffer( g_SkinnedMesh.GetIB10( m ));

            // Set vertex Layout
            //IRenderer::GetRendererInstance()->setVertexFormat( g_pTransformedVertexLayout );

            // Set materials
            SDKMESH_SUBSET* pSubset = 0;
            SDKMESH_MATERIAL* pMat = 0;

            TECHNIQUE_DESC techDesc;
            g_pRenderPostTransformed->GetDesc( &techDesc );

            for( unsigned int iInstance = 0; iInstance < g_iInstances; iInstance++ )
            {
                // Set effect variables
                GetInstanceWorldMatrix( iInstance, &mWorld );
                mWorldViewProj = mWorld * mViewProj;
                g_pmWorldViewProj->SetMatrix( ( float* )&mWorldViewProj );
                g_pmWorld->SetMatrix( ( float* )&mWorld );

                for( unsigned int p = 0; p < techDesc.Passes; ++p )
                {
                    for( unsigned int subset = 0; subset < g_SkinnedMesh.GetNumSubsets( m ); subset++ )
                    {
                        pSubset = g_SkinnedMesh.GetSubset( m, subset );

                        pMat = g_SkinnedMesh.GetMaterial( pSubset->MaterialID );
                        if( pMat )
                        {
                            g_ptxDiffuse->SetResource( pMat->pDiffuseRV10 );
                            g_ptxNormal->SetResource( pMat->pNormalRV10 );
                        }

                        g_pRenderPostTransformed->GetPassByIndex( p )->Apply( 0 );
                        IRenderer::GetRendererInstance()->DrawIndexed( PRIM_TRIANGLES, ( unsigned int )pSubset->IndexCount, ( unsigned int )pSubset->IndexStart,
                                                 ( unsigned int )pSubset->VertexStart );
                    }
                }
            }
        }

        //clear out the vb bindings for the next pass
    }
    else
    {
        for( unsigned int iInstance = 0; iInstance < g_iInstances; iInstance++ )
        {
            // Set effect variables
            GetInstanceWorldMatrix( iInstance, &mWorld );
            mWorldViewProj = mWorld * mViewProj;
            g_pmWorldViewProj->SetMatrix( ( float* )&mWorldViewProj );
            g_pmWorld->SetMatrix( ( float* )&mWorld );

            // Set vertex Layout
            //IRenderer::GetRendererInstance()->setVertexFormat( g_pSkinnedVertexLayout );

            // Render the meshes
            for( unsigned int m = 0; m < g_SkinnedMesh.GetNumMeshes(); m++ )
            {
                IRenderer::GetRendererInstance()->setVertexBuffer( 0, g_SkinnedMesh.GetVB10( m, 0 ) );
                IRenderer::GetRendererInstance()->setIndexBuffer( g_SkinnedMesh.GetIB10( m ) );

                TECHNIQUE_DESC techDesc;
                pTech->GetDesc( &techDesc );
                SDKMESH_SUBSET* pSubset = 0;
                SDKMESH_MATERIAL* pMat = 0;
                Primitives PrimType;

                // Set the bone matrices
                SetBoneMatrices( g_FetchType, m );

                for( unsigned int p = 0; p < techDesc.Passes; ++p )
                {
                    for( unsigned int subset = 0; subset < g_SkinnedMesh.GetNumSubsets( m ); subset++ )
                    {
                        pSubset = g_SkinnedMesh.GetSubset( m, subset );

                        PrimType = g_SkinnedMesh.GetPrimitiveType( ( SDKMESH_PRIMITIVE_TYPE )
                                                                     pSubset->PrimitiveType );

                        pMat = g_SkinnedMesh.GetMaterial( pSubset->MaterialID );
                        if( pMat )
                        {
                            g_ptxDiffuse->SetResource( pMat->pDiffuseRV10 );
                            g_ptxNormal->SetResource( pMat->pNormalRV10 );
                        }

                        pTech->GetPassByIndex( p )->Apply( 0 );
                        IRenderer::GetRendererInstance()->DrawIndexed( PrimType, ( unsigned int )pSubset->IndexCount, ( unsigned int )pSubset->IndexStart,
                                                 ( unsigned int )pSubset->VertexStart );
                    }
                }
            }//nummeshes
        }
    }
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void  OnFrameMove( double fTime, float fElapsedTime)
{
    // Update the camera's position based on user input

    D3DXFROMWINEMATRIX mIdentity;
    D3DXFROMWINEMatrixIdentity( &mIdentity );
    g_SkinnedMesh.TransformMesh( &mIdentity, fTime );
}

//--------------------------------------------------------------------------------------
// SetBoneMatrices
//
// This function handles the various ways of sending bone matrices to the shader.
//		FT_CONSTANTBUFFER:
//			With this approach, the bone matrices are stored in a constant buffer.
//			The shader will index into the constant buffer to grab the correct
//			transformation matrices for each vertex.
//		FT_TEXTUREBUFFER:
//			This approach shows the differences between constant buffers and tbuffers.
//			tbuffers are special constant buffers that are accessed like textures.
//			They should give better random access performance.
//		FT_TEXTURE:
//			When FT_TEXTURE is specified, the matrices are loaded into a 1D texture.
//			This is different from a tbuffer in that an actual texture fetch is used
//			instead of a lookup into a constant buffer.
//		FT_BUFFER:
//			This loads the matrices into a buffer that is bound to the shader.  The
//			shader calls Load on the buffer object to load the different matrices from
//			the stream.  This should give better linear access performance.
//--------------------------------------------------------------------------------------
void SetBoneMatrices( FETCH_TYPE ft, unsigned int iMesh )
{
    switch( ft )
    {
        case FT_CONSTANTBUFFER:
            for( unsigned int i = 0; i < g_SkinnedMesh.GetNumInfluences( iMesh ); i++ )
            {
                D3DXFROMWINEMATRIX* pMat = g_SkinnedMesh.GetMeshInfluenceMatrix( 0, i );
                g_pmConstBoneWorld->SetMatrixArray( ( float* )pMat, i, 1 );
            }
            break;
        case FT_TEXTUREBUFFER:
            for( unsigned int i = 0; i < g_SkinnedMesh.GetNumInfluences( iMesh ); i++ )
            {
                D3DXFROMWINEMATRIX* pMat = g_SkinnedMesh.GetMeshInfluenceMatrix( 0, i );
                g_pmTexBoneWorld->SetMatrixArray( ( float* )pMat, i, 1 );
            }
            break;
        case FT_TEXTURE:
        {
		//D3DXFROMWINEMATRIX Matrices[g_SkinnedMesh.GetNumInfluences( iMesh )];
            D3DXFROMWINEMATRIX* pMatrices=0;
            int hr = 0;
		int pitch;
		pMatrices=(D3DXFROMWINEMATRIX*)IRenderer::GetRendererInstance()->LockTexture(g_pBoneTexture, 0, pitch);
            for( unsigned int i = 0; i < g_SkinnedMesh.GetNumInfluences( iMesh ); i++ )
            {
                pMatrices[i] = *g_SkinnedMesh.GetMeshInfluenceMatrix( 0, i );
            }
    		IRenderer::GetRendererInstance()->UnlockTexture(g_pBoneTexture, 0);

            g_ptxBoneTexture->SetResource( g_pBoneTextureRV );
        }
            break;
        case FT_BUFFER:
        {
		//D3DXFROMWINEMATRIX Matrices[g_SkinnedMesh.GetNumInfluences( iMesh )];
            D3DXFROMWINEMATRIX* pMatrices=0;
            int hr = 0;
            pMatrices=(D3DXFROMWINEMATRIX*)IRenderer::GetRendererInstance()->LockVertexBuffer(g_pBoneBuffer);
            for( unsigned int i = 0; i < g_SkinnedMesh.GetNumInfluences( iMesh ); i++ )
            {
                pMatrices[i] = *g_SkinnedMesh.GetMeshInfluenceMatrix( 0, i );
            }
            IRenderer::GetRendererInstance()->UnLockVertexBuffer(g_pBoneBuffer);

            g_pBoneBufferVar->SetResource( g_pBoneBufferRV );
        }
            break;
    };
};};
App app;
int init(const char* aTitle)
{
    //app.OnD3D10CreateDevice();	
	return 0;
}

void render()
{
   double fTime=0;
   float fElapsedTime=0.0f;
   //app.OnD3D10FrameRender(fTime, fElapsedTime);
   IRenderer::GetRendererInstance()->Present();
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("Skinning10");
	IInput*    i=STX_Service::GetInputInstance();
	STX_Service::GetAudioInstance()->Init();
	init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		render();
	}
	return 0;
}

