#if (!defined AV_MAIN_H_INCLUDED)
#define AV_MAIN_H_INCLUDED

#include <STX/STX.h>
#include <STX/STXCPP.h>
#include <STX/d3dx9mathFromWine.h>
#include <Framework3/IRenderer.h>

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

// include resource definitions
#include "resource.h"
// Include ASSIMP headers (XXX: do we really need all of them?)
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/ai_assert.h>
#include <assimp/cfileio.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/DefaultLogger.hpp>

//include <AssimpPCH.h> /* HACK */
 
#include <MaterialSystem.h>   // aiMaterial class
#include <StringComparison.h> // ASSIMP_stricmp and ASSIMP_strincmp

#include <time.h>

#ifndef SWIG
using namespace Assimp;
#endif

namespace AssimpView
{

#include "AssetHelper.h"
#include "Camera.h"
#include "RenderOptions.h"
#include "Shaders.h"
//include "Background.h"
//include "LogDisplay.h"
//include "LogWindow.h"
#include "Display.h"
#include "MeshRenderer.h"
#include "MaterialManager.h"

} // end of namespace AssimpView - for a while

// outside of namespace, to help Intellisense and solve boost::metatype_stuff_miracle
#include "AnimEvaluator.h"
#include "SceneAnimator.h"

namespace AssimpView
{
#ifndef SWIG
class CMyLogStream : public Assimp::LogStream
{
public:
	/**	@brief	Implementation of the abstract method	*/
	void write(const char* message);
};
#endif
	enum tVertex
	{
		ePositionTextureVertex,
		ePositionNormalVertex,
		ePositionNormalTextureVertex,
		eCompleteVertex
	};
struct MeshRenderer
{
	void SetTX(float aX);
	void SetTY(float aY);
	void SetTZ(float aZ);
	void SetSX(float aScaleX);
	void SetSY(float aScaleY);
	void SetSZ(float aScaleZ);
	void SetSpinX(float aAngleX);
	void SetSpinY(float aAngleY);
	void SetSpinZ(float aAngleZ);
	void SetPlay(bool a);
	void SetAnimIndex( unsigned int pAnimIndex);
	size_t GetCurrentAnimIndex();
	size_t GetNumAnimations();
	const char* GetFileName();
	void SetShader(const char* aShaderName){m_szShaderName=aShaderName;}
	MeshRenderer();
	MeshRenderer(const char* aFilename, const char* aShader);
	virtual ~MeshRenderer();
	int LoadAsset(const char* filename);
	void render();
	void render(D3DXFROMWINEMATRIX w);
	void render(
		float a_fTX,float a_fTY,float a_fTZ,
		float a_fSX,float a_fSy,float a_fSZ,
		float a_fSpinX,
		float a_fSpinY,
		float a_fSpinZ);
#ifndef SWIG
	Assimp::LogStream* m_pcStream;
	float m_fLoadTime;
	bool m_bWasFlipped;

// Evil globals


	//static VertexFormatID M_DefaultVertexDecl /*= 0*/;
	static double m_fFPS							/*= 0.0f*/;
	std::string m_szShaderName;
	std::string m_szFileNameMesh;						/*= 0.0f*/;
	std::string m_szPathTex1;						/*= 0.0f*/;
	std::string m_szPathTex2;
#if 0
	static ShaderID m_piDefaultEffect		/*= 0*/;
	static ShaderID m_piNormalsEffect		/*= 0*/;
	static ShaderID m_piPassThroughEffect	/*= 0*/;
	static ShaderID m_piPatternEffect		/*= 0*/;
	static ShaderID m_piMaterialEffect;

	static VertexFormatID m_DefaultVertexDecl;// = -1;
	static VertexFormatID m_NormalsVertexDecl;// = -1;
	static VertexFormatID m_PassThroughVertexDecl;// = -1;
	static VertexFormatID m_PatternVertexDecl;// = -1;
#endif
	static bool m_bMousePressed					/*= false*/;
	static bool m_bMousePressedR				/*= false*/;
	static bool m_bMousePressedM				/*= false*/;
	static bool m_bMousePressedBoth				/*= false*/;
	static float m_fElpasedTime					/*= 0.0f*/;
	//static D3DCAPS9 m_sCaps;
	bool m_bLoadingFinished				/*= false*/;
	static float m_fWheelPos					/*= -10.0f*/;
	bool m_bLoadingCanceled				/*= false*/;
	TextureID m_pcTexture		/*= 0*/;

	aiMatrix4x4 m_mWorld;
	aiMatrix4x4 m_mWorldRotate;
	aiVector3D m_vRotateSpeed			/*= aiVector3D(0.5f,0.5f,0.5f)*/;
	aiVector3D m_avLightDirs[1] /* =
		{	aiVector3D(-0.5f,0.6f,0.2f) ,
			aiVector3D(-0.5f,0.5f,0.5f)} */;


	static POINT m_mousePos						/*= {0,0};*/;
	static POINT m_LastmousePos					/*= {0,0}*/;
	static bool m_bFPSView						/*= false*/;
	bool m_bInvert						/*= false*/;
	unsigned int m_iCurrentColor			/*= 0*/;

	// NOTE: The light intensity is separated from the color, it can
	// directly be manipulated using the middle mouse button.
	// When the user chooses a color from the palette the intensity
	// is reset to 1.0
	// index[2] is the ambient color
	float m_fLightIntensity				/*=0.0f*/;
//	D3DCOLOR
	D3DXFROMWINEVECTOR4	 m_avLightColors[3];

	RenderOptions m_sOptions;
	Camera m_sCamera;
	AssetHelper *m_pcAsset				/*= 0*/;

	//
	// Contains the mask image for the HUD
	// (used to determine the position of a click)
	//
	// The size of the image is identical to the size of the main
	// HUD texture
	//
	unsigned char* m_szImageMask			/*= 0*/;

	float m_fACMR /*= 3.0f*/;

	bool m_bPlay						/*= false*/;

	double m_dCurrent;
	float m_smoothAngle /*= 80.f*/;

	unsigned int ppsteps,ppstepsdefault;
	bool nopointslines;

	tVertex m_Vertex;

	AssetHelper * GetAsset();
	
	int GenerateNormalsAsLineList(AssetHelper::MeshHelper* pcMesh,const aiMesh* pcSource);
	#endif
	void LoadThreadProc();
	void LoadThreadProc2(std::string& aFileNameMesh);
	int LoadAsset();
	int DeleteAsset(void);
	int CalculateBounds(aiNode* piNode, aiVector3D* p_avOut,
	const aiMatrix4x4& piMatrix);
	int ScaleAsset(void);
	void GetBounds(D3DXFROMWINEVECTOR3& ____min, D3DXFROMWINEVECTOR3& ____max);
	void GetBoundingSphereRadius(float& r);
	int CreateAssetData();
	int DeleteAssetData(bool bNoMaterials);
	int SetupFPSView();
	int InitD3D(void);
	int ShutdownD3D(void);
	int ShutdownDevice(void);
	int CreateHUDTexture();
	int CreateDevice (bool p_bMultiSample,bool p_bSuperSample,bool bHW = true);
	int CreateDevice (void);
	int GetProjectionMatrix (aiMatrix4x4& p_mOut);
	aiVector3D GetCameraMatrix (aiMatrix4x4& p_mOut);
	float m_fSpinX, m_fSpinY, m_fSpinZ,m_fSX,m_fSy,m_fSZ,m_fTX,m_fTY,m_fTZ;
	void SetRenderingMode(unsigned int i=0);
};
};

struct MeshRendererShadersFactory
{
protected:
    	static std::map<std::string, ShaderID> m_MeshRendererShadersFactory_Shadercache;
	static ShaderID find_from_MeshRendererShadersFactory_Shadercache(const char* fileName);
	static void insert_to_MeshRendererShadersFactory_Shadercache(const char* fileName, ShaderID id);
public:
	static const char* GetShaderTxt(const char* aShaderName);
	static ShaderID GetShader(const char* aShaderName, const char *vsName, const char *psName);

protected:
    	static std::map<std::string, VertexFormatID> m_MeshRendererShadersFactory_VertexFormatcache;
	static VertexFormatID find_from_MeshRendererShadersFactory_VertexFormatcache(const char* fileName);
	static void insert_to_MeshRendererShadersFactory_VertexFormatcache(const char* fileName, VertexFormatID id);
public:
	static VertexFormatID GetVertexFormat(const char* ShaderName, const ShaderID shd);
};
#endif // !! AV_MAIN_H_INCLUDED

