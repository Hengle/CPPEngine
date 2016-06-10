//define BUILD_MESHRENDERER

#ifdef BUILD_MESHRENDERER
#if !defined(BUILD_ASSIMP)
#include <assimp_view.h>



//include "AssimpPCH.h"

//include <Framework3/RendererHLSLCg.h>

//include "../include/aiAssert.h"
#ifdef __STXWIN32__
#ifndef __GNUC__
#  include "crtdbg.h"
#endif //ndef gcc
#endif

// Set a breakpoint using win32, else line, file and message will be returned and progam ends with
// errrocode = 1
AI_WONT_RETURN void aiAssert (const std::string &message, unsigned int uiLine, const std::string &file)
{
  // moved expression testing out of the function and into the macro to avoid constructing
  // two std::string on every single ai_assert test
//	if (!expression)
  {
    // FIX (Aramis): changed std::cerr to std::cout that the message appears in VS' output window ...
	  std::cout << "File :" << file << ", line " << uiLine << " : " << message << std::endl;

#ifdef _WIN32
#ifndef __GNUC__
		// Set breakpoint
		__debugbreak();
#endif //ndef gcc
#else
		stx_exit (1);
#endif
	}
}
#endif


/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2010, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#define __MESHRENDERER__ 1

#include <Framework3/IRenderer.h>

#include "stdafx.h"


#include <map>
using namespace std;

namespace AssimpView {


//-------------------------------------------------------------------------------
// evil globals
//-------------------------------------------------------------------------------

	AssetHelper *m_pcAsset				= 0;

	double MeshRenderer::m_fFPS						= 0.0f;
#if 0
	ShaderID MeshRenderer::m_piDefaultEffect		= -1;
	ShaderID MeshRenderer::m_piNormalsEffect		= -1;
	ShaderID MeshRenderer::m_piPassThroughEffect	= -1;
	ShaderID MeshRenderer::m_piPatternEffect		= -1;
	ShaderID MeshRenderer::m_piMaterialEffect=-1;

	VertexFormatID MeshRenderer::m_DefaultVertexDecl = -1;
	VertexFormatID MeshRenderer::m_NormalsVertexDecl = -1;
	VertexFormatID MeshRenderer::m_PassThroughVertexDecl = -1;
	VertexFormatID MeshRenderer::m_PatternVertexDecl = -1;
#endif

	bool MeshRenderer::m_bMousePressed				= false;
	bool MeshRenderer::m_bMousePressedR				= false;
	bool MeshRenderer:: m_bMousePressedM				= false;
	bool MeshRenderer::m_bMousePressedBoth			= false;
	float MeshRenderer::m_fElpasedTime				= 0.0f;
//	D3DCAPS9 MeshRenderer::m_sCaps;
bool m_bLoadingFinished				= false;
	float MeshRenderer::m_fWheelPos					= -10.0f;
bool m_bLoadingCanceled				= false;
TextureID m_pcTexture		= -1;
bool m_bPlay						= false;
double m_dCurrent = 0.;

// default pp steps
unsigned int ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates          | // remove degenerated polygons from the import
		aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
		0;

unsigned int ppstepsdefault = ppsteps;

bool nopointslines = false;

extern bool m_bWasFlipped			/*= false*/;

aiMatrix4x4 m_mWorld;
aiMatrix4x4 m_mWorldRotate;
aiVector3D m_vRotateSpeed			= aiVector3D(0.5f,0.5f,0.5f);

// NOTE: The second light direction is no computed from the first
aiVector3D m_avLightDirs[1] =
{	aiVector3D(-0.5f,0.6f,0.2f)  };

//extern ???
D3DCOLOR m_avLightColors[3] =
{
	D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xFF),
	D3DCOLOR_ARGB(0xFF,0xFF,0x00,0x00),
	D3DCOLOR_ARGB(0xFF,0x05,0x05,0x05),
};

POINT MeshRenderer::m_mousePos;
POINT MeshRenderer::m_LastmousePos;
bool MeshRenderer::m_bFPSView						= false;
bool m_bInvert						= false;
unsigned int m_iCurrentColor		= 0;

float m_fLightIntensity				= 1.0f;
float m_fLightColor					= 1.0f;

RenderOptions m_sOptions;
Camera m_sCamera;

//
// Contains the mask image for the HUD
// (used to determine the position of a click)
//
unsigned char* m_szImageMask		= 0;

extern float m_fLoadTime;// = 0.0f;

void CMyLogStream::write(const char* message){LOG_PRINT("%s\n", message);}

		void MeshRenderer::SetTX(float aX){m_fTX=aX;}
		void MeshRenderer::SetTY(float aY){m_fTY=aY;}
		void MeshRenderer::SetTZ(float aZ){m_fTZ=aZ;}
		void MeshRenderer::SetSX(float aScaleX){m_fSX=aScaleX;}
		void MeshRenderer::SetSY(float aScaleY){m_fSy=aScaleY;}
		void MeshRenderer::SetSZ(float aScaleZ){m_fSZ=aScaleZ;}
		void MeshRenderer::SetSpinX(float aAngleX){m_fSpinX=aAngleX;}
		void MeshRenderer::SetSpinY(float aAngleY){m_fSpinY=aAngleY;}
		void MeshRenderer::SetSpinZ(float aAngleZ){m_fSpinZ=aAngleZ;}
	void MeshRenderer::SetPlay(bool a)
	{
		m_bPlay	= a;
	}

	void MeshRenderer::SetAnimIndex( unsigned int pAnimIndex){GetAsset()->mAnimator->SetAnimIndex(pAnimIndex);}
	size_t MeshRenderer::GetCurrentAnimIndex(){return GetAsset()->mAnimator->CurrentAnimIndex();}
	size_t MeshRenderer::GetNumAnimations(){return GetAsset()->mAnimator->NumAnimations();}

	const char* MeshRenderer::GetFileName(){return m_szFileNameMesh.c_str();}

void MeshRenderer::SetRenderingMode(unsigned int i)
{
	if(i<2)
CDisplay::Instance(this).m_iViewMode=i;
else
CDisplay::Instance(this).m_iViewMode=0;
}

AssetHelper * MeshRenderer::GetAsset(){return m_pcAsset;}
void MeshRenderer::render(D3DXFROMWINEMATRIX w)
{
		CDisplay::Instance(this).OnRender(w);
}
	

		MeshRenderer::~MeshRenderer()
		{
	DeleteAsset();
	Assimp::DefaultLogger::kill();
	ShutdownDevice();
	ShutdownD3D();	

	/* cleanup - calling 'aiReleaseImport' is important, as the library 
	   keeps internal resources until the scene is freed again. Not 
	   doing so can cause severe resource leaking. */
	//aiReleaseImport(scene);

	/* We added a log stream to the library, it's our job to disable it
	   again. This will definitely release the last resources allocated
	   by Assimp.*/
	//aiDetachAllLogStreams();
	
		}
//-------------------------------------------------------------------------------
// Entry point for the loader thread
// The laoder thread loads the asset while the progress dialog displays the
// smart progress bar
//-------------------------------------------------------------------------------
	MeshRenderer::MeshRenderer()
	{
		m_Vertex=ePositionTextureVertex;
		m_szShaderName="SimpleShader";
		m_szFileNameMesh="";
		m_szPathTex1="";
		m_szPathTex2="";
	}
	MeshRenderer::MeshRenderer(const char* aFilename, const char*  aShaderName)
	{
		m_szShaderName=aShaderName;
		m_Vertex=ePositionTextureVertex;
		
if((stx_strcmp(aShaderName,"NormalsEffect")==0)
|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
	m_Vertex=ePositionNormalVertex;
else if((stx_strcmp(aShaderName,"CompleteEffect")==0))
	m_Vertex=eCompleteVertex;

		//ePositionNormalTextureVertex,
		

#if !defined(WIN_PHONE_APP) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
	// File
	std::string f1="/MeshRenderer2.log";
	std::string f2=stx_convertpath(f1.c_str());
	const char* f3=f2.c_str();

	// initialise the default logger if neccessary
	Assimp::DefaultLogger::create(f3, Assimp::Logger::VERBOSE);

	m_pcStream = new CMyLogStream();
	Assimp::DefaultLogger::get()->attachStream(m_pcStream,
        Assimp::DefaultLogger::Debugging | Assimp::DefaultLogger::Info |
        Assimp::DefaultLogger::Err | Assimp::DefaultLogger::Warn);
#endif

	m_pcAsset				= 0;
	m_bLoadingFinished				= false;
	m_bLoadingCanceled				= false;
	m_pcTexture		= -1;	
	m_bPlay						= false;
	m_dCurrent = 0.;


// default pp steps
	ppsteps = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates          | // remove degenerated polygons from the import
		aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
		0;


	ppstepsdefault = ppsteps;

	nopointslines = false;

	m_bWasFlipped=false;



// NOTE: The second light direction is no computed from the first
	m_avLightDirs[0] =
	aiVector3D(-0.5f,0.6f,0.2f)  ;


m_avLightColors[0] =D3DXFROMWINEVECTOR4(0xFF,0xFF,0xFF,0xFF);
m_avLightColors[1] =D3DXFROMWINEVECTOR4(0xFF,0xFF,0x00,0x00);
m_avLightColors[2] =D3DXFROMWINEVECTOR4(0xFF,0x05,0x05,0x05);


	m_bInvert						= false;
	m_iCurrentColor		= 0;

	m_fLightIntensity				= 1.0f;
	m_fLightColor					= 1.0f;

//
// Contains the mask image for the HUD
// (used to determine the position of a click)
//
	m_szImageMask		= 0;

	m_fLoadTime = 0.0f;

	if(aFilename)LoadAsset(aFilename);

	}


void MeshRenderer::LoadThreadProc2(std::string& aFileNameMesh)
{
// only ever give us triangles.
                aiPropertyStore* props = aiCreatePropertyStore();
                aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
                
                unsigned int aiPostProccesFlags;
                
                switch (2)
                {
                    case 0:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_Fast;
                        break;                    
                    case 1:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_Quality;
                        break;
                    case 2:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_MaxQuality;
                        break;
                    default:
                        aiPostProccesFlags = aiProcessPreset_TargetRealtime_MaxQuality;
                        break;
                }

		#if 0
		aiPostProccesFlags |= aiProcess_GenNormals;
		#elif 0
		aiPostProccesFlags |= aiProcess_GenSmoothNormals;
		#endif
		aiPostProccesFlags |= aiProcess_ConvertToLeftHanded;
                
                // aiProcess_FlipUVs is needed for VAO / VBOs,  not sure why.
                m_pcAsset->pcScene = (aiScene*) aiImportFileExWithProperties(aFileNameMesh.c_str(), aiPostProccesFlags | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | 0, 0, props);

                aiReleasePropertyStore(props);
}


//-------------------------------------------------------------------------------
// load the current asset
// THe path to the asset is specified in the global path variable
//-------------------------------------------------------------------------------
void MeshRenderer::LoadThreadProc()
{

	m_fSpinX=0.0f;
	m_fSpinY=0.0f;
	m_fSpinZ=0.0f;
	m_fSX=0.5f;
	m_fSy=0.5f;
	m_fSZ=0.5f;
	m_fTX=0.0f;
	m_fTY=0.0f;
	m_fTZ=10.0f;

	double fCur = (double)timeGetTime()/1000.0f;

	std::string m_szFileNameMesh2=stx_convertpath(m_szFileNameMesh);
	
	LOG_PRINT("m_szFileNameMesh2:%s\n", m_szFileNameMesh2.c_str());

#if 1
	LoadThreadProc2(m_szFileNameMesh2);
#elif 0
	m_pcAsset->pcScene = aiImportFile(m_szFileNameMesh2.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
#elif 0
	m_pcAsset->pcScene = (aiScene*)aiImportFile(m_szFileNameMesh2.c_str(), 0);
#elif 0
	m_pcAsset->pcScene = (aiScene*)aiImportFile(m_szFileNameMesh2.c_str(), 
		aiProcess_ConvertToLeftHanded      |
		aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords         // preprocess UV transformations (scaling, translation ...)
);
#elif 0
    aiPropertyStore* props = aiCreatePropertyStore();
    aiSetImportPropertyInteger(props,AI_CONFIG_IMPORT_TER_MAKE_UVS,1);
    aiSetImportPropertyFloat(props,AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,m_smoothAngle);
    aiSetImportPropertyInteger(props,AI_CONFIG_PP_SBP_REMOVE,nopointslines ? aiPrimitiveType_LINE | aiPrimitiveType_POINT : 0 );

    aiSetImportPropertyInteger(props,AI_CONFIG_GLOB_MEASURE_TIME,1);
    //aiSetImportPropertyInteger(props,AI_CONFIG_PP_PTV_KEEP_HIERARCHY,1);

    // Call ASSIMPs C-API to load the file
    m_pcAsset->pcScene = (aiScene*)aiImportFileExWithProperties(m_szFileNameMesh2.c_str(),

        ppsteps | /* configurable pp steps */
        aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
        aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
        aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
        aiProcess_ConvertToLeftHanded	   | // convert everything to D3D left handed space
        aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
        0,
        0,
        props);

    aiReleasePropertyStore(props);
#elif 0
    const unsigned int flags =
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals |
        aiProcess_ValidateDataStructure |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_SortByPType |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_GenUVCoords |
        aiProcess_OptimizeMeshes |
        aiProcess_OptimizeGraph;
	m_pcAsset->pcScene = (aiScene*)aiImportFile(m_szFileNameMesh2.c_str(), flags);
#elif 0
	/* we are taking one of the postprocessing presets to avoid
	   spelling out 20+ single postprocessing flags here. */
	m_pcAsset->pcScene = (aiScene*)aiImportFile(m_szFileNameMesh2.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
#endif
	double fEnd = (double)timeGetTime()/1000.0f;
	m_fLoadTime = (float)((fEnd - fCur) / 1000);
	m_bLoadingFinished = true;

}


int MeshRenderer::LoadAsset()
{


	// set the world and world rotation matrices to the identuty
	AssimpView::MeshRenderer::m_mWorldRotate = aiMatrix4x4();
	AssimpView::MeshRenderer::m_mWorld = aiMatrix4x4();

	//CLogWindow::Instance(this).WriteLine(
//		"****************************************************************************");
	//CLogWindow::Instance(this).WriteLine(szTemp);
	//CLogWindow::Instance(this).WriteLine(
//		"****************************************************************************");
	//CLogWindow::Instance(this).SetAutoUpdate(false);

	// create a helper thread to load the asset
//	DWORD dwID;
	AssimpView::MeshRenderer::m_bLoadingCanceled = false;

	m_pcAsset = new AssetHelper();

	LoadThreadProc();

	// show the progress bar dialog
	//DialogBox(AssimpView::MeshRenderer::m_hInstance,MAKEINTRESOURCE(IDD_LOADDIALOG),
	//	AssimpView::MeshRenderer::m_hDlg,&ProgressMessageProc);

	// update the log window
	//CLogWindow::Instance(this).SetAutoUpdate(true);
	//CLogWindow::Instance(this).Update();

	// now we should have loaded the asset. Check this ...

	AssimpView::MeshRenderer::m_bLoadingFinished = false;

	if (!GetAsset() || !GetAsset()->pcScene)
	{
		if (GetAsset())
		{
			delete m_pcAsset;
			m_pcAsset = 0;
		}
		return 0;
	}


	// allocate a new MeshHelper array and build a new instance
	// for each mesh in the original asset
	GetAsset()->apcMeshes = new AssetHelper::MeshHelper*[GetAsset()->pcScene->mNumMeshes]();

	for (unsigned int i = 0; i < GetAsset()->pcScene->mNumMeshes;++i)
		GetAsset()->apcMeshes[i] = new AssetHelper::MeshHelper();



	// create animator
	GetAsset()->mAnimator = new SceneAnimator( GetAsset()->pcScene);


	// build a new caption string for the viewer
//	SetWindowText(AssimpView::MeshRenderer::m_hDlg,szOut);

	// scale the asset vertices to fit into the viewer window
	ScaleAsset();


	// reset the camera view to the default position
	AssimpView::MeshRenderer::m_sCamera.vPos = aiVector3D(0.0f,0.0f,-10.0f);
	AssimpView::MeshRenderer::m_sCamera.vLookAt = aiVector3D(0.0f,0.0f,1.0f);
	AssimpView::MeshRenderer::m_sCamera.vUp = aiVector3D(0.0f,1.0f,0.0f);
	AssimpView::MeshRenderer::m_sCamera.vRight = aiVector3D(0.0f,1.0f,0.0f);


	// build native D3D vertex/index buffers, textures, materials
	if( 1 != CreateAssetData())
		return 0;

/*
	if (!GetAsset()->pcScene->HasAnimations()) {
		EnableWindow(GetDlgItem(AssimpView::MeshRenderer::m_hDlg,IDC_PLAY),FALSE);
		EnableWindow(GetDlgItem(AssimpView::MeshRenderer::m_hDlg,IDC_SLIDERANIM),FALSE);
	}
	else {
		EnableWindow(GetDlgItem(AssimpView::MeshRenderer::m_hDlg,IDC_PLAY),TRUE);
		EnableWindow(GetDlgItem(AssimpView::MeshRenderer::m_hDlg,IDC_SLIDERANIM),TRUE);
	}
*/

	//CLogDisplay::Instance(this).AddEntry("[OK] The asset has been loaded successfully");

	CDisplay::Instance(this).FillDisplayList();
	CDisplay::Instance(this).FillAnimList();


	CDisplay::Instance(this).FillDefaultStatistics();


	// render the scene once
	//???CDisplay::Instance(this).OnRender();


	GetAsset()->iNormalSet = AssetHelper::ORIGINAL;

	AssimpView::MeshRenderer::m_bWasFlipped = false;

	return 1;
}


//-------------------------------------------------------------------------------
// Delete the loaded asset
// The function does nothing is no asset is loaded
//-------------------------------------------------------------------------------
void MeshRenderer::render()
{
		D3DXFROMWINEMATRIX mMatR;
D3DXFROMWINEMATRIX mMatS;
D3DXFROMWINEMATRIX mMatT;
		D3DXFROMWINEMatrixRotationYawPitchRoll(&mMatR,
			m_fSpinX,
			m_fSpinY,
			m_fSpinZ);
D3DXFROMWINEMatrixScaling(&mMatS,m_fSX,m_fSy,m_fSZ);
D3DXFROMWINEMatrixTranslation(&mMatT,m_fTX,m_fTY,m_fTZ);
render( mMatR* mMatS *mMatT);
}
void MeshRenderer::render(
			float a_fTX,float a_fTY,float a_fTZ,
			float a_fSX,float a_fSy,float a_fSZ,
			float a_fSpinX,
			float a_fSpinY,
			float a_fSpinZ)

{
	 m_fTX=a_fTX;
	 m_fTY=a_fTY;
	 m_fTZ=a_fTZ;
			m_fSX=a_fSX;
			m_fSy=a_fSy;
			m_fSZ=a_fSZ;
			m_fSpinX=a_fSpinX;
			m_fSpinY=a_fSpinY;
			m_fSpinZ=a_fSpinZ;
		D3DXFROMWINEMATRIX mMatR;
D3DXFROMWINEMATRIX mMatS;
D3DXFROMWINEMATRIX mMatT;
		D3DXFROMWINEMatrixRotationYawPitchRoll(&mMatR,
			m_fSpinX,
			m_fSpinY,
			m_fSpinZ);
D3DXFROMWINEMatrixScaling(&mMatS,m_fSX,m_fSy,m_fSZ);
D3DXFROMWINEMatrixTranslation(&mMatT,m_fTX,m_fTY,m_fTZ);
render( mMatR* mMatS *mMatT);
	}




int MeshRenderer::LoadAsset(const char* afilename)
{
	m_szFileNameMesh=afilename;
  	m_szPathTex1=afilename;
	unsigned found = m_szPathTex1.find_last_of("/\\");
  	m_szPathTex1=m_szPathTex1.substr(0,found);
	m_szPathTex2=stx_convertpath(m_szPathTex1);
		
	// set the world and world rotation matrices to the identuty

	m_mWorldRotate = aiMatrix4x4();

	m_mWorld = aiMatrix4x4();

	//CLogWindow::Instance().WriteLine(
//		"****************************************************************************");
	//CLogWindow::Instance().WriteLine(szTemp);
	//CLogWindow::Instance().WriteLine(
//		"****************************************************************************");
	//CLogWindow::Instance().SetAutoUpdate(false);

	// create a helper thread to load the asset
//	DWORD dwID;
	m_bLoadingCanceled = false;

	m_pcAsset = new AssetHelper();

	////LOG_PRINT("GetAsset()=%d\n",GetAsset());

	LoadThreadProc();

/*
	// show the progress bar dialog
	DialogBox(m_hInstance,MAKEINTRESOURCE(IDD_LOADDIALOG),
		m_hDlg,&ProgressMessageProc);
*/
	// update the log window
	//CLogWindow::Instance().SetAutoUpdate(true);
	//CLogWindow::Instance().Update();

	// now we should have loaded the asset. Check this ...
	m_bLoadingFinished = false;
/*
	if (!GetAsset() || !GetAsset()->pcScene)
	{
		if (GetAsset())
		{
			delete GetAsset();
			GetAsset() = 0;
		}
		return 0;
	}
*/
	// allocate a new MeshHelper array and build a new instance
	// for each mesh in the original asset
/*
	//LOG_PRINT("GetAsset()=%d\n",GetAsset());
		//LOG_PRINT("GetAsset()->pcScene=%d\n",GetAsset()->pcScene);
*/

	GetAsset()->apcMeshes = new AssetHelper::MeshHelper*[GetAsset()->pcScene->mNumMeshes]();
	for (unsigned int i = 0; i < GetAsset()->pcScene->mNumMeshes;++i)
		GetAsset()->apcMeshes[i] = new AssetHelper::MeshHelper();


	// create animator
	GetAsset()->mAnimator = new SceneAnimator( GetAsset()->pcScene);

	// scale the asset vertices to fit into the viewer window
	ScaleAsset();

	// reset the camera view to the default position
	m_sCamera.vPos = aiVector3D(0.0f,0.0f,-10.0f);
	m_sCamera.vLookAt = aiVector3D(0.0f,0.0f,1.0f);
	m_sCamera.vUp = aiVector3D(0.0f,1.0f,0.0f);
	m_sCamera.vRight = aiVector3D(0.0f,1.0f,0.0f);

	// build native D3D vertex/index buffers, textures, materials
	if( 1 != CreateAssetData())
	{
		
		return 0;
	}
/*
	if (!GetAsset()->pcScene->HasAnimations()) {
		EnableWindow(GetDlgItem(m_hDlg,IDC_PLAY),FALSE);
		EnableWindow(GetDlgItem(m_hDlg,IDC_SLIDERANIM),FALSE);
	}
	else {
		EnableWindow(GetDlgItem(m_hDlg,IDC_PLAY),TRUE);
		EnableWindow(GetDlgItem(m_hDlg,IDC_SLIDERANIM),TRUE);
	}
*/
	//CLogDisplay::Instance().AddEntry("[OK] The asset has been loaded successfully");
//???	CDisplay::Instance().FillDisplayList();
//???	CDisplay::Instance().FillAnimList();

//???	CDisplay::Instance(&mr).FillDefaultStatistics();

	// render the scene once
	//???CDisplay::Instance().OnRender();

	GetAsset()->iNormalSet = AssetHelper::ORIGINAL;
	m_bWasFlipped = false;

	
	return 1;
}







int MeshRenderer::DeleteAsset(void)
{
	if (!AssimpView::MeshRenderer::GetAsset())return 0;

	// don't anymore know why this was necessary ...
	//???CDisplay::Instance(this).OnRender();

	// delete everything
	DeleteAssetData(false);//???true
	for (unsigned int i = 0; i < AssimpView::MeshRenderer::GetAsset()->pcScene->mNumMeshes;++i)
	{
		delete AssimpView::MeshRenderer::GetAsset()->apcMeshes[i];
	}
	aiReleaseImport(AssimpView::MeshRenderer::GetAsset()->pcScene);
	delete[] AssimpView::MeshRenderer::GetAsset()->apcMeshes;
	delete AssimpView::MeshRenderer::GetAsset()->mAnimator;
	delete AssimpView::MeshRenderer::m_pcAsset;
	AssimpView::MeshRenderer::m_pcAsset = 0;

	// reset the caption of the viewer window
//	SetWindowText(AssimpView::MeshRenderer::m_hDlg,AI_VIEW_CAPTION_BASE);

	// clear UI
	CDisplay::Instance(this).ClearAnimList();
	CDisplay::Instance(this).ClearDisplayList();

	CMaterialManager::Instance(this).Reset();
//	UpdateWindow(AssimpView::MeshRenderer::m_hDlg);
	return 1;
}


//-------------------------------------------------------------------------------
// Calculate the boundaries of a given node and all of its children
// The boundaries are in Worldspace (AABB)
// piNode Input node
// p_avOut Receives the min/max boundaries. Must point to 2 D3DXFROMWINEVECTOR3s
// piMatrix Transformation matrix of the graph at this position
//-------------------------------------------------------------------------------
int MeshRenderer::CalculateBounds(aiNode* piNode, aiVector3D* p_avOut,
	const aiMatrix4x4& piMatrix)
{
	ai_assert(0 != piNode);
	ai_assert(0 != p_avOut);

	aiMatrix4x4 mTemp = piNode->mTransformation;
	mTemp.Transpose();
	aiMatrix4x4 aiMe = mTemp * piMatrix;

	for (unsigned int i = 0; i < piNode->mNumMeshes;++i)
	{
		for( unsigned int a = 0; a < AssimpView::MeshRenderer::GetAsset()->pcScene->mMeshes[
			piNode->mMeshes[i]]->mNumVertices;++a)
		{
			aiVector3D pc =AssimpView::MeshRenderer::GetAsset()->pcScene->mMeshes[piNode->mMeshes[i]]->mVertices[a];

			aiVector3D pc1;
			D3DXFROMWINEVec3TransformCoord((D3DXFROMWINEVECTOR3*)&pc1,(D3DXFROMWINEVECTOR3*)&pc,
				(D3DXFROMWINEMATRIX*)&aiMe);

			p_avOut[0].x = std::min( p_avOut[0].x, pc1.x);
			p_avOut[0].y = std::min( p_avOut[0].y, pc1.y);
			p_avOut[0].z = std::min( p_avOut[0].z, pc1.z);
			p_avOut[1].x = std::max( p_avOut[1].x, pc1.x);
			p_avOut[1].y = std::max( p_avOut[1].y, pc1.y);
			p_avOut[1].z = std::max( p_avOut[1].z, pc1.z);
		}
	}
	for (unsigned int i = 0; i < piNode->mNumChildren;++i)
	{
		CalculateBounds( piNode->mChildren[i], p_avOut, aiMe );
	}
	return 1;
}
//-------------------------------------------------------------------------------
// Scale the asset that it fits perfectly into the viewer window
// The function calculates the boundaries of the mesh and modifies the
// global world transformation matrix according to the aset AABB
//-------------------------------------------------------------------------------
void MeshRenderer::GetBoundingSphereRadius(float& r)
{
	D3DXFROMWINEVECTOR3 ____min;
	D3DXFROMWINEVECTOR3 ____max;
	GetBounds(____min, ____max);
	D3DXFROMWINEVECTOR3 vDelta = ____max-____min;
	r = 0.5f*D3DXFROMWINEVec3Length(&vDelta);
}
void MeshRenderer::GetBounds(D3DXFROMWINEVECTOR3& ____min, D3DXFROMWINEVECTOR3& ____max)
{
	aiVector3D aiVecs[2] = {aiVector3D( 1e10f, 1e10f, 1e10f),
		aiVector3D( -1e10f, -1e10f, -1e10f) };

	if (GetAsset()->pcScene->mRootNode)
	{
		aiMatrix4x4 m;
		CalculateBounds(GetAsset()->pcScene->mRootNode,aiVecs,m);
	}

	____min = *(D3DXFROMWINEVECTOR3*)&aiVecs[0];
	____max = *(D3DXFROMWINEVECTOR3*)&aiVecs[1];
}
int MeshRenderer::ScaleAsset(void)
{
	aiVector3D aiVecs[2] = {aiVector3D( 1e10f, 1e10f, 1e10f),
		aiVector3D( -1e10f, -1e10f, -1e10f) };

	if (GetAsset()->pcScene->mRootNode)
	{
		aiMatrix4x4 m;
		CalculateBounds(GetAsset()->pcScene->mRootNode,aiVecs,m);
	}

	aiVector3D vDelta = aiVecs[1]-aiVecs[0];
	aiVector3D vHalf =  aiVecs[0] + (vDelta / 2.0f);
	float fScale = 10.0f / vDelta.Length();

	AssimpView::MeshRenderer::m_mWorld =  aiMatrix4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-vHalf.x,-vHalf.y,-vHalf.z,1.0f) *
		aiMatrix4x4(
		fScale,0.0f,0.0f,0.0f,
		0.0f,fScale,0.0f,0.0f,
		0.0f,0.0f,fScale,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	return 1;
}

//-------------------------------------------------------------------------------
// Generate a vertex buffer which holds the normals of the asset as
// a list of unconnected lines
// pcMesh Input mesh
// pcSource Source mesh from ASSIMP
//-------------------------------------------------------------------------------
int MeshRenderer::GenerateNormalsAsLineList(AssetHelper::MeshHelper* pcMesh,const aiMesh* pcSource)
{
	ai_assert(0 != pcMesh);
	ai_assert(0 != pcSource);

	if (!pcSource->mNormals)return 0;
#if 0
	// create vertex buffer
	pcMesh->piVBNormals=AssimpView::MeshRenderer::IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::LineVertex) *
		pcSource->mNumVertices * 2,STATIC);//AssetHelper::LineVertex::GetFVF());

	// now fill the vertex buffer with data
	AssetHelper::LineVertex* pbData2=(AssimpView::AssetHelper::LineVertex *)AssimpView::MeshRenderer::IRenderer::GetRendererInstance()->LockVertexBuffer(pcMesh->piVBNormals);

	for (unsigned int x = 0; x < pcSource->mNumVertices;++x)
	{
		pbData2->vPosition = pcSource->mVertices[x];

		++pbData2;

		aiVector3D vNormal = pcSource->mNormals[x];
		vNormal.Normalize();

		// scalo with the inverse of the world scaling to make sure
		// the normals have equal length in each case
		// TODO: Check whether this works in every case, I don't think so
		vNormal.x /= AssimpView::MeshRenderer::m_mWorld.a1*4;
		vNormal.y /= AssimpView::MeshRenderer::m_mWorld.b2*4;
		vNormal.z /= AssimpView::MeshRenderer::m_mWorld.c3*4;

		pbData2->vPosition = pcSource->mVertices[x] + vNormal;

		++pbData2;
	}
AssimpView::MeshRenderer::IRenderer::GetRendererInstance()->UnLockVertexBuffer(pcMesh->piVBNormals);
#else
	AssetHelper::LineVertex* pbData2=new AssimpView::AssetHelper::LineVertex[pcSource->mNumVertices * 2];
	AssetHelper::LineVertex* pbData20=pbData2;
	for (unsigned int x = 0; x < pcSource->mNumVertices;++x)
	{
		pbData2->vPosition = pcSource->mVertices[x];

		++pbData2;

		aiVector3D vNormal = pcSource->mNormals[x];
		vNormal.Normalize();

		// scalo with the inverse of the world scaling to make sure
		// the normals have equal length in each case
		// TODO: Check whether this works in every case, I don't think so
		vNormal.x /= AssimpView::MeshRenderer::m_mWorld.a1*4;
		vNormal.y /= AssimpView::MeshRenderer::m_mWorld.b2*4;
		vNormal.z /= AssimpView::MeshRenderer::m_mWorld.c3*4;

		pbData2->vPosition = pcSource->mVertices[x] + vNormal;

		++pbData2;
	}
	pcMesh->piVBNormals=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::LineVertex) *
		pcSource->mNumVertices * 2,STATIC,pbData20);//AssetHelper::LineVertex::GetFVF());
	delete[] pbData20;
#endif
	return 1;
}

//-------------------------------------------------------------------------------
// Create the native D3D representation of the asset: vertex buffers,
// index buffers, materials ...
//-------------------------------------------------------------------------------
int MeshRenderer::CreateAssetData()
{
    ////LOG_PRINT("AssimpView::MeshRenderer::GetAsset()=%x\n",AssimpView::MeshRenderer::GetAsset());
	if (!AssimpView::MeshRenderer::GetAsset())return 0;

	// reset all subsystems
	CMaterialManager::Instance(this).Reset();
	CDisplay::Instance(this).Reset();

	for (unsigned int i = 0; i < AssimpView::MeshRenderer::GetAsset()->pcScene->mNumMeshes;++i)
	{
		const aiMesh* mesh = AssimpView::MeshRenderer::GetAsset()->pcScene->mMeshes[i];

		// create the material for the mesh
		if (-1==AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piEffect)	{
			CMaterialManager::Instance(this).CreateMaterial(
				AssimpView::MeshRenderer::GetAsset()->apcMeshes[i],mesh);
		}
/*
		// create vertex buffer
		AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
			=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::Vertex) *
			mesh->mNumVertices,STATIC);
*/
#if 0
		DWORD dwUsage = 0;
		if ((AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piOpacityTexture!=-1) || 1.0f != AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->fOpacity)
			dwUsage |= D3DUSAGE_STATIC;
#endif
		LOG_FNLN;
		LOG_PRINT("mesh->mPrimitiveTypes=%ld\n", mesh->mPrimitiveTypes);
		unsigned int nidx;
		switch (mesh->mPrimitiveTypes) {
			case aiPrimitiveType_POINT:
				nidx = 1;break;
			case aiPrimitiveType_LINE:
				nidx = 2;break;
			case aiPrimitiveType_TRIANGLE:
				nidx = 3;break;
			default: /* assert(false) ??? */ continue;
		};

		// check whether we can use 16 bit indices
		if (mesh->mNumFaces * 3 >= 65536)	{
			// create 32 bit index buffer
#if 0
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB=IRenderer::GetRendererInstance()->addIndexBuffer( 4 *
				mesh->mNumFaces * nidx,sizeof(DWORD),STATIC);

			// now fill the index buffer
			unsigned int* pbData=(unsigned int*)IRenderer::GetRendererInstance()->LockIndexBuffer(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB);

			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					*pbData++ = mesh->mFaces[x].mIndices[a];
				}
			}
#else
			unsigned int* pbData=new unsigned int[//4 *
				mesh->mNumFaces* nidx];
			unsigned int* pbData0=pbData;
			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					*pbData++ = mesh->mFaces[x].mIndices[a];
				}
			}
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB=IRenderer::GetRendererInstance()->addIndexBuffer( //4 *
				mesh->mNumFaces * nidx,sizeof(unsigned int),STATIC,pbData0);
			delete[] pbData0;
#endif
		}
		else	{
			// create 16 bit index buffer
#if 0
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB=IRenderer::GetRendererInstance()->addIndexBuffer( 2 *
				mesh->mNumFaces*nidx,sizeof(WORD),STATIC);

			// now fill the index buffer
			uint16_t* pbData=(uint16_t *)IRenderer::GetRendererInstance()->LockIndexBuffer(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB);

			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					*pbData++ = (uint16_t)mesh->mFaces[x].mIndices[a];
				}
			}
		}
		IRenderer::GetRendererInstance()->UnLockIndexBuffer(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB);
#else
			::uint16_t* pbData=new ::uint16_t[//2 *
				mesh->mNumFaces*nidx];
			::uint16_t* pbData0=pbData;
			for (unsigned int x = 0; x < mesh->mNumFaces;++x)
			{
				for (unsigned int a = 0; a < nidx;++a)
				{
					*pbData++ = (::uint16_t)mesh->mFaces[x].mIndices[a];
				}
			}
AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB=IRenderer::GetRendererInstance()->addIndexBuffer( //2*
				mesh->mNumFaces*nidx,sizeof(::uint16_t),STATIC,pbData0);
		delete[] pbData0;
		}
#endif
		// collect weights on all vertices. Quick and careless
		std::vector<std::vector<aiVertexWeight> > weightsPerVertex( mesh->mNumVertices);
		for( unsigned int a = 0; a < mesh->mNumBones; a++)	{
			const aiBone* bone = mesh->mBones[a];
			for( unsigned int b = 0; b < bone->mNumWeights; b++)
				weightsPerVertex[bone->mWeights[b].mVertexId].push_back( aiVertexWeight( a, bone->mWeights[b].mWeight));
		}
#if 0
/*
		// create vertex buffer
		AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
			=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::Vertex) *
			mesh->mNumVertices,STATIC);
*/
		// now fill the vertex buffer
		AssetHelper::Vertex* pbData2=(AssimpView::AssetHelper::Vertex *)
			IRenderer::GetRendererInstance()->LockVertexBuffer(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB);

		for (unsigned int x = 0; x < mesh->mNumVertices;++x)
		{
			pbData2->vPosition = mesh->mVertices[x];

			if (0 == mesh->mNormals)
				pbData2->vNormal = aiVector3D(0.0f,0.0f,0.0f);
			else pbData2->vNormal = mesh->mNormals[x];
#if 0
			if (0 == mesh->mTangents)	{
				pbData2->vTangent = aiVector3D(0.0f,0.0f,0.0f);
				pbData2->vBitangent = aiVector3D(0.0f,0.0f,0.0f);
			}
			else	{
				pbData2->vTangent = mesh->mTangents[x];
				pbData2->vBitangent = mesh->mBitangents[x];
			}

			if (mesh->HasVertexColors( 0))	{
				pbData2->dColorDiffuse = D3DCOLOR_ARGB(
					((unsigned char)std::max( std::min( mesh->mColors[0][x].a * 255.0f, 255.0f),0.0f)),
					((unsigned char)std::max( std::min( mesh->mColors[0][x].r * 255.0f, 255.0f),0.0f)),
					((unsigned char)std::max( std::min( mesh->mColors[0][x].g * 255.0f, 255.0f),0.0f)),
					((unsigned char)std::max( std::min( mesh->mColors[0][x].b * 255.0f, 255.0f),0.0f)));
			}
			else pbData2->dColorDiffuse = D3DCOLOR_ARGB(0xFF,0xff,0xff,0xff);

			// ignore a third texture coordinate component
			if (mesh->HasTextureCoords( 0))	{
				pbData2->vTextureUV = aiVector2D(
					mesh->mTextureCoords[0][x].x,
					mesh->mTextureCoords[0][x].y);
			}
			else pbData2->vTextureUV = aiVector2D(0.5f,0.5f);

			if (mesh->HasTextureCoords( 1))	{
				pbData2->vTextureUV2 = aiVector2D(
					mesh->mTextureCoords[1][x].x,
					mesh->mTextureCoords[1][x].y);
			}
			else pbData2->vTextureUV2 = aiVector2D(0.5f,0.5f);
#endif
			// Bone indices and weights
			if( mesh->HasBones())	{
				unsigned char boneIndices[4] = { 0, 0, 0, 0 };
				unsigned char boneWeights[4] = { 0, 0, 0, 0 };
				ai_assert( weightsPerVertex[x].size() <= 4);
				for( unsigned int a = 0; a < weightsPerVertex[x].size(); a++)
				{
					boneIndices[a] = weightsPerVertex[x][a].mVertexId;
					boneWeights[a] = (unsigned char) (weightsPerVertex[x][a].mWeight * 255.0f);
				}

				stx_memcpy( pbData2->mBoneIndices, boneIndices, sizeof( boneIndices));
				stx_memcpy( pbData2->mBoneWeights, boneWeights, sizeof( boneWeights));
			} else
			{
				memset( pbData2->mBoneIndices, 0, sizeof( pbData2->mBoneIndices));
				memset( pbData2->mBoneWeights, 0, sizeof( pbData2->mBoneWeights));
			}

			++pbData2;
		}
IRenderer::GetRendererInstance()->UnLockVertexBuffer(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB);
#else
/*
		// create vertex buffer
		AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
			=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::Vertex) *
			mesh->mNumVertices,STATIC);
*/
		// now fill the vertex buffer
		if(m_Vertex==ePositionTextureVertex)
		{
			AssetHelper::PositionTextureVertex* pbData2=new AssimpView::AssetHelper::PositionTextureVertex[mesh->mNumVertices];
			AssetHelper::PositionTextureVertex* pbData20=pbData2;
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
            			pbData2->vPosition = mesh->mVertices[x];
            			if (mesh->HasTextureCoords( 0))	{
                			pbData2->vTextureUV = aiVector2D(
                    				mesh->mTextureCoords[0][x].x,
                    				mesh->mTextureCoords[0][x].y);
            			}
            			else pbData2->vTextureUV = aiVector2D(0.5f,0.5f);
            			++pbData2;
			}
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
				=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::PositionTextureVertex) *
				mesh->mNumVertices,STATIC,pbData20);
				delete[] pbData20;
		}
		else if(m_Vertex==ePositionNormalVertex)
		{
			AssetHelper::PositionNormalVertex* pbData2=new AssimpView::AssetHelper::PositionNormalVertex[mesh->mNumVertices];
			AssetHelper::PositionNormalVertex* pbData20=pbData2;
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
            			pbData2->vPosition = mesh->mVertices[x];
				if (0 == mesh->mNormals)
                			pbData2->vNormal = aiVector3D(0.0f,0.0f,0.0f);
				else pbData2->vNormal = mesh->mNormals[x];
            			++pbData2;
			}
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
				=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::PositionNormalVertex) *
				mesh->mNumVertices,STATIC,pbData20);
				delete[] pbData20;
		}
		else if(m_Vertex==ePositionNormalTextureVertex)
		{
			AssetHelper::PositionNormalTextureVertex* pbData2=new AssimpView::AssetHelper::PositionNormalTextureVertex[mesh->mNumVertices];
			AssetHelper::PositionNormalTextureVertex* pbData20=pbData2;
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
            			pbData2->vPosition = mesh->mVertices[x];
				if (0 == mesh->mNormals)
                			pbData2->vNormal = aiVector3D(0.0f,0.0f,0.0f);
				else pbData2->vNormal = mesh->mNormals[x];
            			if (mesh->HasTextureCoords( 0))	{
                			pbData2->vTextureUV = aiVector2D(
                    				mesh->mTextureCoords[0][x].x,
                    				mesh->mTextureCoords[0][x].y);
            			}
            			else pbData2->vTextureUV = aiVector2D(0.5f,0.5f);
            			++pbData2;
			}
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
				=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::PositionNormalTextureVertex) *
				mesh->mNumVertices,STATIC,pbData20);
				delete[] pbData20;
		}
		else if(m_Vertex==eCompleteVertex)
		{
			AssetHelper::CompleteVertex* pbData2=new AssimpView::AssetHelper::CompleteVertex[mesh->mNumVertices];
			AssetHelper::CompleteVertex* pbData20=pbData2;
			for (unsigned int x = 0; x < mesh->mNumVertices;++x)
        		{
            			pbData2->vPosition = mesh->mVertices[x];
				if (0 == mesh->mNormals)
                			pbData2->vNormal = aiVector3D(0.0f,0.0f,0.0f);
				else pbData2->vNormal = mesh->mNormals[x];

            			if (0 == mesh->mTangents)	{
               				pbData2->vTangent = aiVector3D(0.0f,0.0f,0.0f);
                			pbData2->vBitangent = aiVector3D(0.0f,0.0f,0.0f);
            			}
            			else	{
                			pbData2->vTangent = mesh->mTangents[x];
                			pbData2->vBitangent = mesh->mBitangents[x];
            			}

            			if (mesh->HasVertexColors( 0))	{
                			pbData2->dColorDiffuse = aiColor4t<unsigned char>(
                    				((unsigned char)max( min( mesh->mColors[0][x].a * 255.0f, 255.0f),0.0f)),
                    				((unsigned char)max( min( mesh->mColors[0][x].r * 255.0f, 255.0f),0.0f)),
                    				((unsigned char)max( min( mesh->mColors[0][x].g * 255.0f, 255.0f),0.0f)),
                    				((unsigned char)max( min( mesh->mColors[0][x].b * 255.0f, 255.0f),0.0f)));
           			}
            			else pbData2->dColorDiffuse = aiColor4t<unsigned char>(0xFF,0xff,0xff,0xff);

            			// ignore a third texture coordinate component
            			if (mesh->HasTextureCoords( 0))	{
                			pbData2->vTextureUV = aiVector2D(
                    				mesh->mTextureCoords[0][x].x,
                    				mesh->mTextureCoords[0][x].y);
            			}
            			else pbData2->vTextureUV = aiVector2D(0.5f,0.5f);

            			if (mesh->HasTextureCoords( 1))	{
                			pbData2->vTextureUV2 = aiVector2D(
                    				mesh->mTextureCoords[1][x].x,
                   				mesh->mTextureCoords[1][x].y);
            			}
            			else pbData2->vTextureUV2 = aiVector2D(0.5f,0.5f);
            		++pbData2;
			}
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB
				=IRenderer::GetRendererInstance()->addVertexBuffer(sizeof(AssetHelper::CompleteVertex) *
				mesh->mNumVertices,STATIC,pbData20);
				delete[] pbData20;
       		}
#if 0
            // Bone indices and weights
            if( mesh->HasBones())	{
                unsigned char boneIndices[4] = { 0, 0, 0, 0 };
                unsigned char boneWeights[4] = { 0, 0, 0, 0 };
                ai_assert( weightsPerVertex[x].size() <= 4);
                for( unsigned int a = 0; a < weightsPerVertex[x].size(); a++)
                {
                    boneIndices[a] = weightsPerVertex[x][a].mVertexId;
                    boneWeights[a] = (unsigned char) (weightsPerVertex[x][a].mWeight * 255.0f);
                }

                memcpy( pbData2->mBoneIndices, boneIndices, sizeof( boneIndices));
                memcpy( pbData2->mBoneWeights, boneWeights, sizeof( boneWeights));
            } else
            {
                memset( pbData2->mBoneIndices, 0, sizeof( pbData2->mBoneIndices));
                memset( pbData2->mBoneWeights, 0, sizeof( pbData2->mBoneWeights));
            }
#endif
#endif
		// now generate the second vertex buffer, holding all normals
		if (!AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVBNormals)	{
			GenerateNormalsAsLineList(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i],mesh);
		}
	}
	return 1;
}

//-------------------------------------------------------------------------------
// Delete all effects, textures, vertex buffers ... associated with
// an asset
//-------------------------------------------------------------------------------
int MeshRenderer::DeleteAssetData(bool bNoMaterials)
{
	if (!AssimpView::MeshRenderer::GetAsset())return 0;


	// TODO: Move this to a proper destructor
	for (unsigned int i = 0; i < AssimpView::MeshRenderer::GetAsset()->pcScene->mNumMeshes;++i)
	{
		if(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB)
		{
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVB = -1;
		}
		if(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVBNormals)
		{
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piVBNormals = -1;
		}
		if(AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB)
		{
			AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piIB = -1;
		}

		// TODO ... unfixed memory leak
		// delete storage eventually allocated to hold a copy
		// of the original vertex normals
		//if (AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->pvOriginalNormals)
		//{
		//	delete[] AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->pvOriginalNormals;
		//}

		if (!bNoMaterials)
		{
			if((AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piEffect!=-1))
			{
				AssimpView::MeshRenderer::GetAsset()->apcMeshes[i]->piEffect = -1;
			}
			if((GetAsset()->apcMeshes[i]->piDiffuseTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piDiffuseTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piNormalTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piNormalTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piSpecularTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piSpecularTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piAmbientTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piAmbientTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piEmissiveTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piEmissiveTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piOpacityTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piOpacityTexture = -1;
			}
			if((GetAsset()->apcMeshes[i]->piShininessTexture!=-1))
			{
				GetAsset()->apcMeshes[i]->piShininessTexture = -1;
			}
		}
	}
	return 1;
}


//-------------------------------------------------------------------------------
// Switch beetween zoom/rotate view and the standatd FPS view
// mr->m_bFPSView specifies the view mode to setup
//-------------------------------------------------------------------------------
int MeshRenderer::SetupFPSView()
{
	if (!m_bFPSView)
	{
		m_sCamera.vPos = aiVector3D(0.0f,0.0f,m_fWheelPos);
		m_sCamera.vLookAt = aiVector3D(0.0f,0.0f,1.0f);
		m_sCamera.vUp = aiVector3D(0.0f,1.0f,0.0f);
		m_sCamera.vRight = aiVector3D(0.0f,1.0f,0.0f);
	}
	else
	{
		m_fWheelPos = m_sCamera.vPos.z;
		m_sCamera.vPos = aiVector3D(0.0f,0.0f,-10.0f);
		m_sCamera.vLookAt = aiVector3D(0.0f,0.0f,1.0f);
		m_sCamera.vUp = aiVector3D(0.0f,1.0f,0.0f);
		m_sCamera.vRight = aiVector3D(0.0f,1.0f,0.0f);
	}
	return 1;
}

//-------------------------------------------------------------------------------
// Initialize the IDIrect3D interface
// Called by the WinMain
//-------------------------------------------------------------------------------
int MeshRenderer::InitD3D(void)
{

	return 1;
}


//-------------------------------------------------------------------------------
// Release the interface.
// NOTE: Assumes that the device has already been deleted
//-------------------------------------------------------------------------------
int MeshRenderer::ShutdownD3D(void)
{
	ShutdownDevice();

	return 1;
}


//-------------------------------------------------------------------------------
// Shutdown the D3D devie object and all resources associated with it
// NOTE: Assumes that the asset has already been deleted
//-------------------------------------------------------------------------------
int MeshRenderer::ShutdownDevice(void)
{
	// release other subsystems
//	CBackgroundPainter::Instance(this).ReleaseNativeResource();
	//CLogDisplay::Instance(this).ReleaseNativeResource();

	// deleted the one channel image allocated to hold the HUD mask
	delete[] m_szImageMask;
	m_szImageMask = 0;

	return 1;
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int MeshRenderer::CreateHUDTexture()
{
#if 0
	// lock the memory resource ourselves
	HRSRC res = FindResource(0,MAKEINTRESOURCE(IDR_HUD),RT_RCDATA);
	HGLOBAL hg = LoadResource(0,res);
	void* pData = LockResource(hg);

	if(FAILED(D3DXFROMWINECreateTextureFromFileInMemoryEx(mr->m_piDevice,
		pData,SizeofResource(0,res),
		D3DXFROMWINE_DEFAULT_NONPOW2,
		D3DXFROMWINE_DEFAULT_NONPOW2,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DXFROMWINE_DEFAULT,
		D3DXFROMWINE_DEFAULT,
		0,
		0,
		0,
		&mr->m_pcTexture)))
	{
		//CLogDisplay::Instance(this).AddEntry("[ERROR] Unable to load HUD texture",
			D3DCOLOR_ARGB(0xFF,0xFF,0,0));

		mr->m_pcTexture  = 0;
		mr->m_szImageMask = 0;

		UnlockResource(hg);
		FreeResource(hg);
		return 0;
	}

	UnlockResource(hg);
	FreeResource(hg);

	SURFACE_DESC sDesc;
	mr->m_pcTexture->GetLevelDesc(0,&sDesc);


	// lock the memory resource ourselves
	res = FindResource(0,MAKEINTRESOURCE(IDR_HUDMASK),RT_RCDATA);
	hg = LoadResource(0,res);
	pData = LockResource(hg);

	TextureID pcTex;
	if(FAILED(D3DXFROMWINECreateTextureFromFileInMemoryEx(mr->m_piDevice,
		pData,SizeofResource(0,res),
		sDesc.Width,
		sDesc.Height,
		1,
		0,
		D3DFMT_L8,
		D3DPOOL_MANAGED, // unnecessary
		D3DXFROMWINE_DEFAULT,
		D3DXFROMWINE_DEFAULT,
		0,
		0,
		0,
		&pcTex)))
	{
		//CLogDisplay::Instance(this).AddEntry("[ERROR] Unable to load HUD mask texture",
			D3DCOLOR_ARGB(0xFF,0xFF,0,0));
		mr->m_szImageMask = 0;

		UnlockResource(hg);
		FreeResource(hg);
		return 0;
	}

	UnlockResource(hg);
	FreeResource(hg);

	// lock the texture and copy it to get a pointer
	D3DLOCKED_RECT sRect;

    sRect.pBits=IRenderer::GetRendererInstance()->LockTexture(pcTex, 0, sRect.Pitch);

	unsigned char* szOut = new unsigned char[sDesc.Width * sDesc.Height];
	unsigned char* _szOut = szOut;

	unsigned char* szCur = (unsigned char*) sRect.pBits;
	for (unsigned int y = 0; y < sDesc.Height;++y)
	{
		stx_memcpy(_szOut,szCur,sDesc.Width);

		szCur += sRect.Pitch;
		_szOut += sDesc.Width;
	}
    IRenderer::GetRendererInstance()->UnlockTexture(pcTex, 0);

	mr->m_szImageMask = szOut;
#endif
	return 1;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int MeshRenderer::CreateDevice (bool p_bMultiSample,bool p_bSuperSample,bool bHW /*= true*/)
{

	// create a vertex declaration to match the vertex
	//D3DVERTEXELEMENT9* vdecl = AssetHelper::Vertex::GetDeclarationElements();

/*
	// get the capabilities of the device object
	IRenderer::GetRendererInstance()->GetDeviceCaps(&mr->m_sCaps);
	if(mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(3,0))
	{
		//EnableWindow(GetDlgItem(mr->m_hDlg,IDC_LOWQUALITY),FALSE);
	}
*/
	// compile the default material shader (gray gouraud/phong)
	//m_piDefaultEffect=-1;
#if 0
	IRenderer::GetRendererInstance()->addHLSLShader(
		mr->m_szDefaultShader.c_str(),"DefaultVShader","DefaultPShaderSpecular_D1");//D1???
    FormatDesc Fmt[] = {
#if 0
{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
{ 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
{ 0, 68, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
{ 0, 72, D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#else
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
	{0, TYPE_NORMAL, FORMAT_FLOAT, 3},
	/*
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 4},//???
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
	*/
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 4},//???
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 4}//???
#endif
    };
#endif
	//m_DefaultVertexDecl = -1;//IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt),mr->m_piDefaultEffect);//???
	//IRenderer::GetRendererInstance()->setVertexFormat( mr->m_DefaultVertexDecl);

#if 0
	// use Fixed Function effect when working with shaderless cards
	if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
			DefaultFXSpecular_FFh=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_FF");
		mr->m_piDefaultEffect->SetTechnique( DefaultFXSpecular_FFh);


		//"DefaultVShader_FF","");//???
	}
#endif
	// create the shader used to draw the HUD
#if 0
mr->m_piPassThroughEffect=IRenderer::GetRendererInstance()->addHLSLShader(mr->m_szPassThroughShader.c_str(),"DefaultVShader","PassThrough_PS");
    FormatDesc Fmt[] = {
#if 0
{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
{ 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
{ 0, 68, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
{ 0, 72, D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#else
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2}
#endif
    };
	gPassThroughVertexDecl = IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt),mr->m_piPassThroughEffect);//???
#endif
#if 0
	// use Fixed Function effect when working with shaderless cards
	if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
		mr->m_piPassThroughEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		mr->m_szPassThroughShader.c_str(),"DefaultVShader","");//???
    FormatDesc Fmt[] = {
#if 0
{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
{ 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
{ 0, 68, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
{ 0, 72, D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#else
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
#endif
    };
	gPassThroughVertexDecl = IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt),mr->m_piPassThroughEffect);//???
	}
#endif
	// create the shader used to visualize normal vectors
#if 0
	???mr->m_piNormalsEffect=IRenderer::GetRendererInstance()->addHLSLShader(mr->m_szNormalsShader.c_str(),"RenderNormalsVS","RenderNormalsPS");
    FormatDesc Fmt[] = {
#if 0
{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
{ 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
{ 0, 68, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
{ 0, 72, D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#else
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
#endif
    };
	gNormalsVertexDecl = IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt),mr->m_piNormalsEffect);//???
#endif

	//MessageBox( mr->m_hDlg, "Failed to create vertex declaration", "Init", MB_OK);
#if 0
	// use Fixed Function effect when working with shaderless cards
	if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
		mr->m_piNormalsEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		mr->m_szNormalsShader.c_str(),"RenderNormalsVS_FF","");//???
    FormatDesc Fmt[] = {
#if 0
{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
{ 0, 28, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
{ 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
{ 0, 52, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
{ 0, 68, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
{ 0, 72, D3DDECLTYPE_UBYTE4N, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#else
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
#endif
    };
	gNormalsVertexDecl = IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt),mr->m_piNormalsEffect);//???
	}
#endif
//???	IRenderer::GetRendererInstance()->SetRenderState(D3DRS_DITHERENABLE,TRUE);

	// create the texture for the HUD
//	CreateHUDTexture();
//	CBackgroundPainter::Instance(this).RecreateNativeResource();
	//CLogDisplay::Instance(this).RecreateNativeResource();

	//mr->m_piPassThroughEffect
		IRenderer::GetRendererInstance()->SetTexture(
	"TEXTURE_2D",m_pcTexture);
	return 1;
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int MeshRenderer::CreateDevice (void)
{
	return CreateDevice(m_sOptions.bMultiSample,
		m_sOptions.bSuperSample);
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
int MeshRenderer::GetProjectionMatrix (aiMatrix4x4& p_mOut)
{
	/*
	//LOG_PRINT("aiMatrix4x4=%d\n",sizeof(aiMatrix4x4));
	//LOG_PRINT("mat4=%d\n",sizeof(mat4));
	//LOG_PRINT("D3DXFROMWINEMATRIX=%d\n",sizeof(D3DXFROMWINEMATRIX));
*/
#if 0
	const float fFarPlane = 100.0f;
	const float fNearPlane = 0.1f;
#else
	const float fFarPlane = 10000.0f;
	const float fNearPlane = 0.001f;
#endif
	const float fFOV = (float)(45.0 * 0.0174532925);
	const float fAspect = IRenderer::GetRendererInstance()->GetAspect();
#if 1
	const float s = 1.0f / tanf(fFOV * 0.5f);
	const float Q = fFarPlane / (fFarPlane - fNearPlane);

	p_mOut = aiMatrix4x4(
		s / fAspect, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, Q, 1.0f,
		0.0f, 0.0f, -Q * fNearPlane, 0.0f);
#else//???
IRenderer::GetRendererInstance()->buildPerspectiveMatrix(fFOV/0.0174532925,
                                   fAspect,
                                   fNearPlane, fFarPlane);
p_mOut = *(aiMatrix4x4*)&IRenderer::GetRendererInstance()->Getp();
#endif
	return 1;
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
aiVector3D MeshRenderer::GetCameraMatrix (aiMatrix4x4& p_mOut)
{
	D3DXFROMWINEMATRIX view;
	D3DXFROMWINEMatrixIdentity( &view );

	D3DXFROMWINEVec3Normalize( (D3DXFROMWINEVECTOR3*)&m_sCamera.vLookAt, (D3DXFROMWINEVECTOR3*)&m_sCamera.vLookAt );
	D3DXFROMWINEVec3Cross( (D3DXFROMWINEVECTOR3*)&m_sCamera.vRight, (D3DXFROMWINEVECTOR3*)&m_sCamera.vUp, (D3DXFROMWINEVECTOR3*)&m_sCamera.vLookAt );
	D3DXFROMWINEVec3Normalize( (D3DXFROMWINEVECTOR3*)&m_sCamera.vRight, (D3DXFROMWINEVECTOR3*)&m_sCamera.vRight );
	D3DXFROMWINEVec3Cross( (D3DXFROMWINEVECTOR3*)&m_sCamera.vUp, (D3DXFROMWINEVECTOR3*)&m_sCamera.vLookAt, (D3DXFROMWINEVECTOR3*)&m_sCamera.vRight );
	D3DXFROMWINEVec3Normalize( (D3DXFROMWINEVECTOR3*)&m_sCamera.vUp, (D3DXFROMWINEVECTOR3*)&m_sCamera.vUp );

	view.m[0][0] = m_sCamera.vRight.x;
	view.m[0][1] = m_sCamera.vUp.x;
	view.m[0][2] = m_sCamera.vLookAt.x;
	view.m[0][3] = 0.0f;

	view.m[1][0] = m_sCamera.vRight.y;
	view.m[1][1] = m_sCamera.vUp.y;
	view.m[1][2] = m_sCamera.vLookAt.y;
	view.m[1][3] = 0.0f;

	view.m[2][0] = m_sCamera.vRight.z;
	view.m[2][1] = m_sCamera.vUp.z;
	view.m[2][2] = m_sCamera.vLookAt.z;
	view.m[2][3] = 0.0f;

	view.m[3][0] = -D3DXFROMWINEVec3Dot( (D3DXFROMWINEVECTOR3*)&m_sCamera.vPos, (D3DXFROMWINEVECTOR3*)&m_sCamera.vRight );
	view.m[3][1] = -D3DXFROMWINEVec3Dot( (D3DXFROMWINEVECTOR3*)&m_sCamera.vPos, (D3DXFROMWINEVECTOR3*)&m_sCamera.vUp );
	view.m[3][2] = -D3DXFROMWINEVec3Dot( (D3DXFROMWINEVECTOR3*)&m_sCamera.vPos, (D3DXFROMWINEVECTOR3*)&m_sCamera.vLookAt );
	view.m[3][3] =  1.0f;

	stx_memcpy(&p_mOut,&view,sizeof(aiMatrix4x4));

	return m_sCamera.vPos;
}

};
#endif






