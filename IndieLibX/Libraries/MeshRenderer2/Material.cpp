#include <assimp_view.h>
#ifdef BUILD_MESHRENDERER
//include <Framework3/IRenderer.h>
#include <Framework3/IRenderer.h>
#include <Framework3/RendererHLSLCg.h>
//define BUILD_MESHRENDERER

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
#include <Framework3/IRenderer.h>


#include "stdafx.h"
#define __MESHRENDERER__ 1

#include <Framework3/IRenderer.h>

namespace AssimpView {

extern std::string g_szSimpleShader;

/*static */ CMaterialManager CMaterialManager::s_cInstance;

//-------------------------------------------------------------------------------
// D3DXFROMWINE callback function to fill a texture with a checkers pattern
//
// This pattern is used to mark textures which could not be loaded
//-------------------------------------------------------------------------------
VOID WINAPI FillFunc(D3DXFROMWINEVECTOR4* pOut,
					 CONST D3DXFROMWINEVECTOR2* pTexCoord,
					 CONST D3DXFROMWINEVECTOR2* pTexelSize,
					 LPVOID pData)
{
	//UNREFERENCED_PARAMETER(pData);
	//UNREFERENCED_PARAMETER(pTexelSize);

	// generate a nice checker pattern (yellow/black)
	// size of a square: 32 * 32 px
	unsigned int iX = (unsigned int)(pTexCoord->x * 256.0f);
	unsigned int iY = (unsigned int)(pTexCoord->y * 256.0f);

	bool bBlack = false;
	if ((iX / 32) % 2 == 0)
	{
		if ((iY / 32) % 2 == 0)bBlack = true;
	}
	else
	{
		if ((iY / 32) % 2 != 0)bBlack = true;
	}
	pOut->w = 1.0f;
	if (bBlack)
	{
		pOut->x = pOut->y = pOut->z = 0.0f;
	}
	else
	{
		pOut->x = pOut->y = 1.0f;
		pOut->z = 0.0f;
	}
	return;
}

//-------------------------------------------------------------------------------
int CMaterialManager::UpdateSpecularMaterials()
	{
	if (mr->GetAsset() && mr->GetAsset()->pcScene)
		{
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
			{
			if (aiShadingMode_Phong == mr->GetAsset()->apcMeshes[i]->eShadingMode)
				{
				this->DeleteMaterial(mr->GetAsset()->apcMeshes[i]);
				this->CreateMaterial(mr->GetAsset()->apcMeshes[i],mr->GetAsset()->pcScene->mMeshes[i]);
				}
			}
		}
	return 1;
	}
//-------------------------------------------------------------------------------
int CMaterialManager::SetDefaultTexture(TextureID* p_ppiOut)
{
		Image3 img;
		img.create(
			FORMAT_RGBA8,//???
			256,
		256,1,1);
		*p_ppiOut=IRenderer::GetRendererInstance()->addTexture(
			&img,
			false,
			IRenderer::GetRendererInstance()->Getlinear()
			,STATIC);
	return 1;
}
//-------------------------------------------------------------------------------
bool CMaterialManager::TryLongerPath(char* szTemp,aiString* p_szString)
{
	char szTempB[MAX_PATH];
	stx_strlcpy(szTempB,szTemp,MAX_PATH);

	// go to the beginning of the file name
	char* szFile = stx_strrchr(szTempB,stx_getOsSeparator());
	if (!szFile)szFile = stx_strrchr(szTempB,stx_getOsSeparator());

	char* szFile2 = szTemp + (szFile - szTempB)+1;
	szFile++;
	char* szExt = stx_strrchr(szFile,'.');
	if (!szExt)return false;
	szExt++;
	*szFile = 0;

	stx_strlcat(szTempB,"*.*",MAX_PATH);
	const unsigned int iSize = (const unsigned int) ( szExt - 1 - szFile );
	#if 0
	HANDLE          h;
	WIN32_FIND_DATA info;

	// build a list of files
	h = FindFirstFile(szTempB, &info);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(strcmp(info.cFileName, ".") == 0 || strcmp(info.cFileName, "..") == 0))
			{
				char* szExtFound = stx_strrchr(info.cFileName, '.');
				if (szExtFound)
				{
					++szExtFound;
					if (0 == ASSIMP_stricmp(szExtFound,szExt))
					{
						const unsigned int iSizeFound = (const unsigned int) (
							szExtFound - 1 - info.cFileName);

						for (unsigned int i = 0; i < iSizeFound;++i)
							info.cFileName[i] = (CHAR)tolower(info.cFileName[i]);

						if (0 == memcmp(info.cFileName,szFile2, std::min(iSizeFound,iSize)))
						{
							// we have it. Build the full path ...
							char* sz = stx_strrchr(szTempB,'*');
							*(sz-2) = 0x0;

							stx_strlcat(szTempB,info.cFileName,MAX_PATH);

							// copy the result string back to the aiString
							const size_t iLen = strlen(szTempB);
							size_t iLen2 = iLen+1;
							iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
							stx_memcpy(p_szString->data,szTempB,iLen2);
							p_szString->length = iLen;
							return true;
						}
					}
					// check whether the 8.3 DOS name is matching
					if (0 == ASSIMP_stricmp(info.cAlternateFileName,p_szString->data))
					{
						stx_strlcat(szTempB,info.cAlternateFileName,MAX_PATH);

						// copy the result string back to the aiString
						const size_t iLen = strlen(szTempB);
						size_t iLen2 = iLen+1;
						iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
						stx_memcpy(p_szString->data,szTempB,iLen2);
						p_szString->length = iLen;
						return true;
					}
				}
			}
		}
		while (FindNextFile(h, &info));

		FindClose(h);
	}
	#else
//???
	#endif
	return false;
}
//-------------------------------------------------------------------------------
int CMaterialManager::FindValidPath(aiString* p_szString)
{
	ai_assert(0 != p_szString);
	////LOG_PRINT("p_szString->data:%s\n", p_szString->data);
	aiString pcpy = *p_szString;
	if ('*' ==  p_szString->data[0])	{
		// '*' as first character indicates an embedded file
		return 5;
	}
	
	char szTemp[MAX_PATH];
	std::string szTemp2, szTemp3;
	
	szTemp2.append(mr->m_szPathTex1);
	szTemp3.append(mr->m_szPathTex2);
#ifdef _MSC_VER
	szTemp2.append("\\\\");
	szTemp3.append("\\\\");
#else
	szTemp2.append("/");
	szTemp3.append("/");
#endif
	
	unsigned int i=0;
	
	if(stx_strlen(p_szString->data)>1)
	if(p_szString->data[0]=='.')
	{
		
		if(p_szString->data[1]=='\\')
			i=2;
		else if(p_szString->data[1]=='/')
			i=2;
			
	}

	
	szTemp2.append(p_szString->data+i);
	
	szTemp3.append(p_szString->data+i);

	
	stx_strlcpy(szTemp, szTemp2.c_str(), MAX_PATH);

	// first check whether we can directly load the file
	//LOG_PRINT("MeshRenderer:Material.cpp:fopen2:%s\n", szTemp2.c_str());
	
	LOG_PRINT("MeshRenderer:Material.cpp:fopen3:%s\n", szTemp3.c_str());

	
	FILE* pFile = fopen(szTemp3.c_str(),"rb");
	#if 0
	if (
		!
		stx_isopened(pFile))
		//fclose(pFile);
	//else
	{
		// check whether we can use the directory of  the asset as relative base
		char szTemp[MAX_PATH], tmp2[MAX_PATH];
		stx_strlcpy(szTemp, mr->m_szFileNameMesh.c_str(),MAX_PATH);

		char* szData = p_szString->data;
		if (*szData == stx_getOsSeparator() || *szData == stx_getOsSeparator())++szData;

		char* szEnd = stx_strrchr(szTemp,stx_getOsSeparator());
		if (!szEnd)
		{
			szEnd = stx_strrchr(szTemp,stx_getOsSeparator());
			if (!szEnd)szEnd = szTemp;
		}
		szEnd++;
		*szEnd = 0;
		stx_strlcat(szEnd,szData,MAX_PATH);

		stx_strlcpy(tmp2,stx_convertpath(mr->m_szFileNameTex.c_str(), 0),MAX_PATH);

		//LOG_PRINT("MeshRenderer:Material.cpp:fopen2:%s\n", tmp2);

		pFile = fopen(szTemp,"rb");
		#if 1
		if (!pFile)
		{
			// convert the string to lower case
			for (unsigned int i = 0;;++i)
			{
				if ('\0' == szTemp[i])break;
				szTemp[i] = (char)tolower(szTemp[i]);
			}

			if(TryLongerPath(szTemp,p_szString))return 1;
			*szEnd = 0;

			// search common sub directories
			stx_strlcat(szEnd,"tex\\",MAX_PATH);
			stx_strlcat(szEnd,szData,MAX_PATH);

			//LOG_PRINT("MeshRenderer:Material.cpp:fopen3:%s\n", szTemp);
			pFile = fopen(szTemp, "rb");
			if (!pFile)
			{
				if(TryLongerPath(szTemp,p_szString))return 1;

				*szEnd = 0;

				stx_strlcat(szEnd,"textures\\",MAX_PATH);
				stx_strlcat(szEnd,szData,MAX_PATH);

				//LOG_PRINT("MeshRenderer:Material.cpp:fopen4:%s\n", szTemp);
				pFile = fopen(szTemp, "rb");
				if (!pFile)
				{
					if(TryLongerPath(szTemp, p_szString))return 1;
				}

				// patch by mark sibly to look for textures files in the asset's base directory.
				const char *path=pcpy.data;
				const char *p=stx_strrchr( path,stx_getOsSeparator() );
				if( !p ) p=stx_strrchr( path,stx_getOsSeparator() );
				if( p ){
					char *q=stx_strrchr( tmp2,stx_getOsSeparator() );
					if( !q ) q=stx_strrchr( tmp2,stx_getOsSeparator() );
					if( q ){
						stx_strlcpy( q+1,p+1,MAX_PATH );
       						//LOG_PRINT("MeshRenderer:Material.cpp:fopen5:%s\n", tmp2);                 			
						if(pFile==fopen( tmp2,"r" ) ){
							fclose( pFile );
							stx_strlcpy(p_szString->data,tmp2,MAX_PATH);
							p_szString->length = strlen(tmp2);
							return 1;
						}
					}
				}
				return 0;
			}
			
		}
		#endif
		fclose(pFile);

		// copy the result string back to the aiString
		const size_t iLen = strlen(szTemp);
		size_t iLen2 = iLen+1;
		iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
		stx_memcpy(p_szString->data,szTemp,iLen2);
		p_szString->length = iLen;

	}
	#endif

		fclose(pFile);

		// copy the result string back to the aiString
		const size_t iLen = strlen(szTemp);

		size_t iLen2 = iLen+1;
		iLen2 = iLen2 > MAXLEN ? MAXLEN : iLen2;
		stx_memcpy(p_szString->data,szTemp,iLen2);

		p_szString->length = iLen;
	return 1;
}
//-------------------------------------------------------------------------------
int CMaterialManager::LoadTexture(TextureID* p_ppiOut,aiString* szPath)
{	
	//???ai_assert(-1 != *p_ppiOut);
	ai_assert(0 != szPath);

	*p_ppiOut = -1;
#if 1

	LOG_PRINT("szPath=%s\n", szPath);
	// first get a valid path to the texture
	if( 5 == FindValidPath(szPath))
	{
		// embedded file. Find its index
		unsigned int iIndex = atoi(szPath->data+1);
		if (iIndex < mr->GetAsset()->pcScene->mNumTextures)
		{
/*			if (0 == mr->GetAsset()->pcScene->mTextures[iIndex]->mHeight)
			{
				// it is an embedded file ... don't need the file format hint,
				// simply let D3DXFROMWINE load the file
				if (FAILED(D3DXFROMWINECreateTextureFromFileInMemoryEx(mr->m_piDevice,
					mr->GetAsset()->pcScene->mTextures[iIndex]->pcData,
					mr->GetAsset()->pcScene->mTextures[iIndex]->mWidth,
					D3DXFROMWINE_DEFAULT,
					D3DXFROMWINE_DEFAULT,
					0,
					D3DUSAGE_AUTOGENMIPMAP,
					D3DFMT_UNKNOWN,
					D3DPOOL_MANAGED,
					D3DXFROMWINE_DEFAULT,
					D3DXFROMWINE_DEFAULT,
					0,
					0,
					0,
					p_ppiOut)))
				{
					std::string sz = "[ERROR] Unable to load embedded texture (#1): ";
					sz.append(szPath->data);
					//CLogDisplay::Instance().AddEntry(sz,D3DCOLOR_ARGB(0xFF,0xFF,0x0,0x0));

					this->SetDefaultTexture(p_ppiOut);
					return 1;
				}
			}
			else*/
			{
				// fill a new texture ...
		Image3 img;

		BYTE* dst=img.create(
			FORMAT_RGBA8,//???
			mr->GetAsset()->pcScene->mTextures[iIndex]->mWidth,
					mr->GetAsset()->pcScene->mTextures[iIndex]->mHeight,1,1);
		//BYTE* dst=img.getPixels();


				// now copy the data to it ... (assume non pow2 to be supported)

//D3DLOCKED_RECT sLock;

	//	sLock.pBits=IRenderer::GetRendererInstance()->LockTexture(*p_ppiOut, 0, sLock.Pitch);

				const aiTexel* pcData = mr->GetAsset()->pcScene->mTextures[iIndex]->pcData;

				for (unsigned int y = 0; y < mr->GetAsset()->pcScene->mTextures[iIndex]->mHeight;++y)
				{
					stx_memcpy(dst,pcData,mr->GetAsset()->pcScene->mTextures[iIndex]->
						mWidth *sizeof(aiTexel));
					dst = (BYTE *)dst + 4*mr->GetAsset()->pcScene->mTextures[iIndex]->mWidth;
					pcData += mr->GetAsset()->pcScene->mTextures[iIndex]->mWidth;
				}
  //  IRenderer::GetRendererInstance()->UnlockTexture(*p_ppiOut, 0);
//				(*p_ppiOut)->GenerateMipSubLevels();

		*p_ppiOut=IRenderer::GetRendererInstance()->addTexture(
			&img,
			false,
			IRenderer::GetRendererInstance()->Getlinear()
			,STATIC);

			}
			return 1;
		}
		else
		{
			std::string sz = "[ERROR] Invalid index for embedded texture: ";
			sz.append(szPath->data);
			//CLogDisplay::Instance().AddEntry(sz,D3DCOLOR_ARGB(0xFF,0xFF,0x0,0x0));

			SetDefaultTexture(p_ppiOut);
			return 1;
		}
	}

    	////LOG_PRINT("mr->m_piDevice=%x\n", mr->m_piDevice);
	//DBG_HALT;
	// then call D3DXFROMWINE to load the texture
	//D3DFMT_A8R8G8B8,
#endif
	std::string f1=szPath->data;
	LOG_FNLN;
	LOG_PRINT("Assimp Mesh Texture:%s\n", f1.c_str());
#if 1
		*p_ppiOut=//IRenderer::GetRendererInstance()->
			IRenderer::GetRendererInstance()->addImageLibTexture(
			f1.c_str(),
			false,
			IRenderer::GetRendererInstance()->Getlinear()
			,STATIC
			);
#else
	Image3 img;
	if (img.loadImageLibImage(szPath->data, false))
	{
		img.createMipMaps();
		*p_ppiOut=IRenderer::GetRendererInstance()->addTexture(&img,false, IRenderer::GetRendererInstance()->Getlinear(),STATIC,0.0f,false);
	}
#endif
	return 1;
}
//-------------------------------------------------------------------------------
void CMaterialManager::DeleteMaterial(AssetHelper::MeshHelper* pcIn)
{
	pcIn->piEffect = -1;
}
//-------------------------------------------------------------------------------
void CMaterialManager::HMtoNMIfNecessary(
	TextureID piTexture,
	TextureID* piTextureOut,
	bool bWasOriginallyHM)
{
	ai_assert(-1 != piTexture);
	ai_assert(-1 != *piTextureOut);

	bool bMustConvert = false;
	uintptr_t iElement = 3;

	*piTextureOut = piTexture;

	// Lock the input texture and try to determine its type.
	// Criterias:
	// - If r,g,b channel are identical it MUST be a height map
	// - If one of the rgb channels is used and the others are empty it
	//   must be a height map, too.
	// - If the average color of the whole image is something inside the
	//   purple range we can be sure it is a normal map
	//
	// - Otherwise we assume it is a normal map
	// To increase performance we take not every pixel

	BYTE* pBits=0;
	int Pitch=0;
	SURFACE_DESC sDesc;
	IRenderer::GetRendererInstance()->GetLevelDesc(piTexture,0, &sDesc);
    pBits=IRenderer::GetRendererInstance()->LockTexture(piTexture, 0, Pitch);

	const int iPitchDiff = (int)Pitch - (int)(sDesc.Width * 4);

	struct SColor
	{
		union
		{
			struct {unsigned char b,g,r,a;};
			char _array[4];
		};
	};
	const SColor* pcData = (const SColor*)pBits;

	union
	{
		const SColor* pcPointer;
		const unsigned char* pcCharPointer;
	};
	pcPointer = pcData;

	// 1. If r,g,b channel are identical it MUST be a height map
	bool bIsEqual = true;
	for (unsigned int y = 0; y <  sDesc.Height;++y)
	{
		for (unsigned int x = 0; x <  sDesc.Width;++x)
		{
			if (pcPointer->b != pcPointer->r || pcPointer->b != pcPointer->g)
			{
				bIsEqual = false;
				break;
			}
			pcPointer++;
		}
		pcCharPointer += iPitchDiff;
	}
	if (bIsEqual)bMustConvert = true;
	else
	{
		// 2. If one of the rgb channels is used and the others are empty it
		//    must be a height map, too.
		pcPointer = pcData;
		while (*pcCharPointer == 0)pcCharPointer++;

		iElement = (uintptr_t)(pcCharPointer - (unsigned char*)pcData) % 4;
		unsigned int aiIndex[3] = {0,1,2};
		if (3 != iElement)aiIndex[iElement] = 3;

		pcPointer = pcData;

		bIsEqual = true;
		if (3 != iElement)
		{
			for (unsigned int y = 0; y <  sDesc.Height;++y)
			{
				for (unsigned int x = 0; x <  sDesc.Width;++x)
				{
					for (unsigned int ii = 0; ii < 3;++ii)
					{
						// don't take the alpha channel into account.
						// if the texture was stored n RGB888 format D3DXFROMWINE has
						// converted it to ARGB8888 format with a fixed alpha channel
						if (aiIndex[ii] != 3 && pcPointer->_array[aiIndex[ii]] != 0)
						{
							bIsEqual = false;
							break;
						}
					}
					pcPointer++;
				}
				pcCharPointer += iPitchDiff;
			}
			if (bIsEqual)bMustConvert = true;
			else
			{
				// If the average color of the whole image is something inside the
				// purple range we can be sure it is a normal map

				// (calculate the average color line per line to prevent overflows!)
				pcPointer = pcData;
				aiColor3D clrColor;
				for (unsigned int y = 0; y <  sDesc.Height;++y)
				{
					aiColor3D clrColorLine;
					for (unsigned int x = 0; x <  sDesc.Width;++x)
					{
						clrColorLine.r += pcPointer->r;
						clrColorLine.g += pcPointer->g;
						clrColorLine.b += pcPointer->b;
						pcPointer++;
					}
					clrColor.r += clrColorLine.r /= (float)sDesc.Width;
					clrColor.g += clrColorLine.g /= (float)sDesc.Width;
					clrColor.b += clrColorLine.b /= (float)sDesc.Width;
					pcCharPointer += iPitchDiff;
				}
				clrColor.r /= (float)sDesc.Height;
				clrColor.g /= (float)sDesc.Height;
				clrColor.b /= (float)sDesc.Height;

				if (!(clrColor.b > 215 &&
					clrColor.r > 100 && clrColor.r < 140 &&
					clrColor.g > 100 && clrColor.g < 140))
				{
					// Unable to detect. Believe the original value obtained from the loader
					if (bWasOriginallyHM)
					{
						bMustConvert = true;
					}
				}
			}
		}
	}

    IRenderer::GetRendererInstance()->UnlockTexture(piTexture, 0);

	// if the input data is assumed to be a height map we'll
	// need to convert it NOW
	/*???
	if (bMustConvert)
	{
		SURFACE_DESC sDesc;
		piTexture->GetLevelDesc(0, &sDesc);

		TextureID piTempTexture;
		Image3* img;
		img->create(
			FORMAT3FromD3D(sDesc.Format),//???
			sDesc.Width,
			sDesc.Height,1,
			piTexture->GetLevelCount());
		piTempTexture=IRenderer::GetRendererInstance()->addTexture(img,false,IRenderer::GetRendererInstance()->Getlinear());

		DWORD dwFlags;
		if (3 == iElement)dwFlags = D3DXFROMWINE_CHANNEL_LUMINANCE;
		else if (2 == iElement)dwFlags = D3DXFROMWINE_CHANNEL_RED;
		else if (1 == iElement)dwFlags = D3DXFROMWINE_CHANNEL_GREEN;
		else /#if (0 == iElement)#/dwFlags = D3DXFROMWINE_CHANNEL_BLUE;

		if(FAILED(D3DXFROMWINEComputeNormalMap(piTempTexture,
			piTexture,0,0,dwFlags,1.0f)))
		{
			//CLogDisplay::Instance().AddEntry(
				"[ERROR] Unable to compute normal map from height map",
				D3DCOLOR_ARGB(0xFF,0xFF,0x0,0x0));

			return;
		}
		*piTextureOut = piTempTexture;
	}
	*/
}
//-------------------------------------------------------------------------------
bool CMaterialManager::HasAlphaPixels(TextureID piTexture)
{
	ai_assert(piTexture>=0);

	BYTE* pBits=0;
	int Pitch=0;
	SURFACE_DESC sDesc;
	IRenderer::GetRendererInstance()->GetLevelDesc(piTexture,0, &sDesc);

    pBits=IRenderer::GetRendererInstance()->LockTexture(piTexture, 0, Pitch);

	const int iPitchDiff = (int)Pitch - (int)(sDesc.Width * 4);

	struct SColor
	{
		unsigned char b,g,r,a;;
	};
	const SColor* pcData = (const SColor*)pBits;

	union
	{
		const SColor* pcPointer;
		const unsigned char* pcCharPointer;
	};
	pcPointer = pcData;
	for (unsigned int y = 0; y <  sDesc.Height;++y)
	{
		for (unsigned int x = 0; x <  sDesc.Width;++x)
		{
			if (pcPointer->a != 0xFF)
			{
    IRenderer::GetRendererInstance()->UnlockTexture(piTexture, 0);
				return true;
			}
			pcPointer++;
		}
		pcCharPointer += iPitchDiff;
	}
    IRenderer::GetRendererInstance()->UnlockTexture(piTexture, 0);
	return false;
}
//-------------------------------------------------------------------------------
int CMaterialManager::CreateMaterial(
	AssetHelper::MeshHelper* pcMesh,const aiMesh* pcSource)
{
#if 1//???
	ai_assert(0 != pcMesh);
	ai_assert(0 != pcSource);

//	ID3DXFROMWINEBuffer* piBuffer;


	// extract all properties from the ASSIMP material structure
	const aiMaterial* pcMat = mr->GetAsset()->pcScene->mMaterials[pcSource->mMaterialIndex];

	//
	// DIFFUSE COLOR --------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialColor(pcMat,AI_MATKEY_COLOR_DIFFUSE,
		(aiColor4D*)&pcMesh->vDiffuseColor))
	{
		pcMesh->vDiffuseColor.x = 1.0f;
		pcMesh->vDiffuseColor.y = 1.0f;
		pcMesh->vDiffuseColor.z = 1.0f;
		pcMesh->vDiffuseColor.w = 1.0f;
	}
	//
	// SPECULAR COLOR --------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialColor(pcMat,AI_MATKEY_COLOR_SPECULAR,
		(aiColor4D*)&pcMesh->vSpecularColor))
	{
		pcMesh->vSpecularColor.x = 1.0f;
		pcMesh->vSpecularColor.y = 1.0f;
		pcMesh->vSpecularColor.z = 1.0f;
		pcMesh->vSpecularColor.w = 1.0f;
	}
	//
	// AMBIENT COLOR --------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialColor(pcMat,AI_MATKEY_COLOR_AMBIENT,
		(aiColor4D*)&pcMesh->vAmbientColor))
	{
		pcMesh->vAmbientColor.x = 0.0f;
		pcMesh->vAmbientColor.y = 0.0f;
		pcMesh->vAmbientColor.z = 0.0f;
		pcMesh->vAmbientColor.w = 1.0f;
	}
	//
	// EMISSIVE COLOR -------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialColor(pcMat,AI_MATKEY_COLOR_EMISSIVE,
		(aiColor4D*)&pcMesh->vEmissiveColor))
	{
		pcMesh->vEmissiveColor.x = 0.0f;
		pcMesh->vEmissiveColor.y = 0.0f;
		pcMesh->vEmissiveColor.z = 0.0f;
		pcMesh->vEmissiveColor.w = 1.0f;
	}

	//
	// Opacity --------------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialFloat(pcMat,AI_MATKEY_OPACITY,&pcMesh->fOpacity))
	{
		pcMesh->fOpacity = 1.0f;
	}

	//
	// Shading Model --------------------------------------------------
	//
	bool bDefault = false;
	if(AI_SUCCESS != aiGetMaterialInteger(pcMat,AI_MATKEY_SHADING_MODEL,(int*)&pcMesh->eShadingMode ))
	{
		bDefault = true;
		pcMesh->eShadingMode = aiShadingMode_Gouraud;
	}


	//
	// Shininess ------------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialFloat(pcMat,AI_MATKEY_SHININESS,&pcMesh->fShininess))
	{
		// assume 15 as default shininess
		pcMesh->fShininess = 15.0f;
	}
	else if (bDefault)pcMesh->eShadingMode  = aiShadingMode_Phong;


	//
	// Shininess strength ------------------------------------------------------
	//
	if(AI_SUCCESS != aiGetMaterialFloat(pcMat,AI_MATKEY_SHININESS_STRENGTH,&pcMesh->fSpecularStrength))
	{
		// assume 1.0 as default shininess strength
		pcMesh->fSpecularStrength = 1.0f;
	}

	aiString szPath;

	aiTextureMapMode mapU(aiTextureMapMode_Wrap),mapV(aiTextureMapMode_Wrap);

	bool bib =false;
	if (pcSource->mTextureCoords[0])
	{

		//
		// DIFFUSE TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_DIFFUSE(0),&szPath))
		{
			LoadTexture(&pcMesh->piDiffuseTexture,&szPath);

			aiGetMaterialInteger(pcMat,AI_MATKEY_MAPPINGMODE_U_DIFFUSE(0),(int*)&mapU);
			aiGetMaterialInteger(pcMat,AI_MATKEY_MAPPINGMODE_V_DIFFUSE(0),(int*)&mapV);
		}

		//
		// SPECULAR TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_SPECULAR(0),&szPath))
		{
			LoadTexture(&pcMesh->piSpecularTexture,&szPath);
		}

		//
		// OPACITY TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_OPACITY(0),&szPath))
		{
			LoadTexture(&pcMesh->piOpacityTexture,&szPath);
		}
		else
		{
			int flags = aiTextureFlags_IgnoreAlpha;//???0;
			aiGetMaterialInteger(pcMat,AI_MATKEY_TEXFLAGS_DIFFUSE(0),&flags);

			// try to find out whether the diffuse texture has any
			// non-opaque pixels. If we find a few, use it as opacity texture
			if ((pcMesh->piDiffuseTexture!=-1) && !(flags & aiTextureFlags_IgnoreAlpha) && HasAlphaPixels(pcMesh->piDiffuseTexture))
			{
				int iVal;

				// NOTE: This special value is set by the tree view if the user
				// manually removes the alpha texture from the view ...
				if (AI_SUCCESS != aiGetMaterialInteger(pcMat,"no_a_from_d",0,0,&iVal))
				{
					pcMesh->piOpacityTexture = pcMesh->piDiffuseTexture;
//					pcMesh->piOpacityTexture->AddRef();
				}
			}
		}

		//
		// AMBIENT TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_AMBIENT(0),&szPath))
		{
			LoadTexture(&pcMesh->piAmbientTexture,&szPath);

		}

		//
		// EMISSIVE TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_EMISSIVE(0),&szPath))
		{
			LoadTexture(&pcMesh->piEmissiveTexture,&szPath);
		}

		//
		// Shininess TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_SHININESS(0),&szPath))
		{
			LoadTexture(&pcMesh->piShininessTexture,&szPath);
		}

		//
		// Lightmap TEXTURE ------------------------------------------------
		//
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_LIGHTMAP(0),&szPath))
		{
			LoadTexture(&pcMesh->piLightmapTexture,&szPath);
		}


		//
		// NORMAL/HEIGHT MAP ------------------------------------------------
		//
		bool bHM = false;
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_NORMALS(0),&szPath))
		{
			LoadTexture(&pcMesh->piNormalTexture,&szPath);
		}
		else
		{
			if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE_HEIGHT(0),&szPath))
			{
				LoadTexture(&pcMesh->piNormalTexture,&szPath);
			}
			else bib = true;
			bHM = true;
		}

		// normal/height maps are sometimes mixed up. Try to detect the type
		// of the texture automatically
		if (pcMesh->piNormalTexture!=-1)
		{
			HMtoNMIfNecessary(pcMesh->piNormalTexture, &pcMesh->piNormalTexture,bHM);
		}
	}

	// check whether a global background texture is contained
	// in this material. Some loaders set this value ...
	if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_GLOBAL_BACKGROUND_IMAGE,&szPath))
	{
//		CBackgroundPainter::Instance().SetTextureBG(szPath.data);
	}

	// BUGFIX: If the shininess is 0.0f disable phong lighting
	// This is a workaround for some meshes in the DX SDK (e.g. tiny.x)
	// FIX: Added this check to the x-loader, but the line remains to
	// catch other loader doing the same ...
	if (0.0f == pcMesh->fShininess){
		pcMesh->eShadingMode = aiShadingMode_Gouraud;
	}

	int two_sided = 0;
	aiGetMaterialInteger(pcMat,AI_MATKEY_TWOSIDED,&two_sided);
	pcMesh->twosided = (two_sided != 0);

	// check whether we have already a material using the same
	// shader. This will decrease loading time rapidly ...
	for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
	{
		if (mr->GetAsset()->pcScene->mMeshes[i] == pcSource)
		{
			break;
		}
		AssetHelper::MeshHelper* pc = mr->GetAsset()->apcMeshes[i];

		if  ((pcMesh->piDiffuseTexture !=-1 ? true : false) !=
			(pc->piDiffuseTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piSpecularTexture !=-1 ? true : false) !=
			(pc->piSpecularTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piAmbientTexture !=-1 ? true : false) !=
			(pc->piAmbientTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piEmissiveTexture !=-1 ? true : false) !=
			(pc->piEmissiveTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piNormalTexture !=-1 ? true : false) !=
			(pc->piNormalTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piOpacityTexture !=-1 ? true : false) !=
			(pc->piOpacityTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piShininessTexture !=-1 ? true : false) !=
			(pc->piShininessTexture !=-1 ? true : false))
			continue;
		if  ((pcMesh->piLightmapTexture !=-1 ? true : false) !=
			(pc->piLightmapTexture !=-1 ? true : false))
			continue;
		if ((pcMesh->eShadingMode != aiShadingMode_Gouraud ? true : false) !=
			(pc->eShadingMode != aiShadingMode_Gouraud ? true : false))
			continue;

		if ((pcMesh->fOpacity != 1.0f ? true : false) != (pc->fOpacity != 1.0f ? true : false))
			continue;

		if (pcSource->HasBones() != mr->GetAsset()->pcScene->mMeshes[i]->HasBones())
			continue;

		// we can reuse this material
		if (pc->piEffect!=-1)
		{
			pcMesh->piEffect = pc->piEffect;
			pc->bSharedFX = pcMesh->bSharedFX = true;
//			pcMesh->piEffect->AddRef();
			return 2;
		}
	}
	m_iShaderCount++;

	//if(mr->m_piDefaultEffect==-1)
	if(0)
	{
typedef struct _D3DXFROMWINEMACRO
{
    LPCSTR Name;
    LPCSTR Definition;

} D3DXFROMWINEMACRO, *LPD3DXFROMWINEMACRO;

	D3DXFROMWINEMACRO sMacro[64];

	// build macros for the HLSL compiler
	unsigned int iCurrent = 0;
	if (pcMesh->piDiffuseTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_DIFFUSE_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;

		if (mapU == aiTextureMapMode_Wrap)
			sMacro[iCurrent].Name = "AV_WRAPU";
		else if (mapU == aiTextureMapMode_Mirror)
			sMacro[iCurrent].Name = "AV_MIRRORU";
		else // if (mapU == aiTextureMapMode_Clamp)
			sMacro[iCurrent].Name = "AV_CLAMPU";

		sMacro[iCurrent].Definition = "1";
		++iCurrent;


		if (mapV == aiTextureMapMode_Wrap)
			sMacro[iCurrent].Name = "AV_WRAPV";
		else if (mapV == aiTextureMapMode_Mirror)
			sMacro[iCurrent].Name = "AV_MIRRORV";
		else // if (mapV == aiTextureMapMode_Clamp)
			sMacro[iCurrent].Name = "AV_CLAMPV";

		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}
	if (pcMesh->piSpecularTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_SPECULAR_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}
	if (pcMesh->piAmbientTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_AMBIENT_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}
	if (pcMesh->piEmissiveTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_EMISSIVE_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}
	char buff[32];
	if (pcMesh->piLightmapTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_LIGHTMAP_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;

		int idx;
		if(AI_SUCCESS == aiGetMaterialInteger(pcMat,AI_MATKEY_UVWSRC_LIGHTMAP(0),&idx) && idx >= 1 && pcSource->mTextureCoords[idx])	{
			sMacro[iCurrent].Name = "AV_TWO_UV";
			sMacro[iCurrent].Definition = "1";
			++iCurrent;

			sMacro[iCurrent].Definition = "IN.TexCoord1";
		}
		else sMacro[iCurrent].Definition = "IN.TexCoord0";
		sMacro[iCurrent].Name = "AV_LIGHTMAP_TEXTURE_UV_COORD";

		++iCurrent;float f= 1.f;
		aiGetMaterialFloat(pcMat,AI_MATKEY_TEXBLEND_LIGHTMAP(0),&f);
		stx_snprintf(buff,32,"%f",f);

		sMacro[iCurrent].Name = "LM_STRENGTH";
		sMacro[iCurrent].Definition = buff;
		++iCurrent;
	}
	if ((pcMesh->piNormalTexture!=-1) && !bib)
	{
		sMacro[iCurrent].Name = "AV_NORMAL_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}
	if (pcMesh->piOpacityTexture!=-1)
	{
		sMacro[iCurrent].Name = "AV_OPACITY_TEXTURE";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;

		if (pcMesh->piOpacityTexture == pcMesh->piDiffuseTexture)
		{
			sMacro[iCurrent].Name = "AV_OPACITY_TEXTURE_REGISTER_MASK";
			sMacro[iCurrent].Definition = "a";
			++iCurrent;
		}
		else
		{
			sMacro[iCurrent].Name = "AV_OPACITY_TEXTURE_REGISTER_MASK";
			sMacro[iCurrent].Definition = "r";
			++iCurrent;
		}
	}

	if (pcMesh->eShadingMode  != aiShadingMode_Gouraud  && !mr->m_sOptions.bNoSpecular)
	{
		sMacro[iCurrent].Name = "AV_SPECULAR_COMPONENT";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;

		if (pcMesh->piShininessTexture!=-1)
		{
			sMacro[iCurrent].Name = "AV_SHININESS_TEXTURE";
			sMacro[iCurrent].Definition = "1";
			++iCurrent;
		}
	}
	if (1.0f != pcMesh->fOpacity)
	{
		sMacro[iCurrent].Name = "AV_OPACITY";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}

	if( pcSource->HasBones())
	{
		sMacro[iCurrent].Name = "AV_SKINNING";
		sMacro[iCurrent].Definition = "0";//???"1";
		++iCurrent;
	}
/*
	// If a cubemap is active, we'll need to lookup it for calculating
	// a physically correct reflection
	if (CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance().GetMode())
	{
		sMacro[iCurrent].Name = "AV_SKYBOX_LOOKUP";
		sMacro[iCurrent].Definition = "1";
		++iCurrent;
	}*/
	sMacro[iCurrent].Name = 0;
	sMacro[iCurrent].Definition = 0;

	//Construct defines from sMacro and compine with mr->m_szMaterialShader string
	std::string extra;

	unsigned int iiCurrent = 0;
	while
	((sMacro[iiCurrent].Name != 0)&&
	(sMacro[iiCurrent].Definition != 0))
	{
		extra.append("#define ");
		extra.append(sMacro[iiCurrent].Name);
		extra.append(" ");
		extra.append(sMacro[iiCurrent].Definition);
		extra.append(";\n");
		iiCurrent++;
	}


	// compile the shader
#if 1
	const char *ShaderName = mr->m_szShaderName.c_str();

	//LOG_PRINT("ShaderName=%s\n", ShaderName);

	ShaderID shd=MeshRendererShadersFactory::GetShader(ShaderName, "main", "main");

	//LOG_PRINT("m_SimpleShader=%s\n", m_SimpleShader);
#elif 0
	const char *m_SimpleShader = MeshRendererShadersFactory::GetShader("SimpleShader");
	mr->m_piDefaultEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		m_SimpleShader,
		"main",
		"main");//D1???
#elif 0
	mr->m_piDefaultEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		g_szMaterialShader.c_str(),
		"MaterialVShader_D1",
		"MaterialPShaderSpecular_D1",0,0,extra.c_str());//D1???
#elif 0
			mr->m_piDefaultEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		g_szDefaultShader.c_str(),
		"DefaultVShader",
		"DefaultPShaderSpecular_D1",0,0,extra.c_str());//D1???
#endif
//mr->m_piMaterialEffect=mr->m_piDefaultEffect;
#if 1
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
	/*
	{0, TYPE_NORMAL, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 3},//???
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 4},//???
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 4}//???
	*/
#endif
    };
	//mr->m_DefaultVertexDecl = IRenderer::GetRendererInstance()->addVertexFormat(Fmt, elementsOf(Fmt), mr->m_piDefaultEffect);//???
#endif
	}
	if(0)//-1== mr->m_piMaterialEffect)
	{

		// get the name of the material and use it in the log message
		if(AI_SUCCESS == aiGetMaterialString(pcMat,AI_MATKEY_NAME,&szPath) &&
			'\0' != szPath.data[0])
		{
			std::string sz = "[ERROR] Unable to load material: ";
			sz.append(szPath.data);
			//CLogDisplay::Instance().AddEntry(sz);
		}
		else
		{
			//CLogDisplay::Instance().AddEntry("Unable to load material: UNNAMED");
		}
		return 0;
	} else
	{
		/*
		// use Fixed Function effect when working with shaderless cards
		if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
		{
			////LOG_PRINT("mr->m_piDefaultEffect=%d\n",mr->m_piDefaultEffect);
			IRenderer::GetRendererInstance()->SetTechnique( MaterialFX_FFh);
		}*/
	}

//	if( piBuffer) piBuffer->Release();
	LOG_FNLN;
	LOG_PRINT("ShaderName=%d\n",mr->m_szShaderName.c_str());
	ShaderID shd = MeshRendererShadersFactory::GetShader(mr->m_szShaderName.c_str(), "main", "main");
	IRenderer::GetRendererInstance()->setShader(shd);

	// now commit all constants to the shader
	//
	// This is not necessary for shared shader. Shader constants for
	// shared shaders are automatically recommited before the shader
	// is being used for a particular mesh

	if (1.0f != pcMesh->fOpacity)
	{
		//???IRenderer::GetRendererInstance()->SetFloat("TRANSPARENCY",pcMesh->fOpacity);
	}
	if (pcMesh->eShadingMode  != aiShadingMode_Gouraud && !mr->m_sOptions.bNoSpecular)
	{
		IRenderer::GetRendererInstance()->SetFloat("SPECULARITY",pcMesh->fShininess);//???
		IRenderer::GetRendererInstance()->SetFloat("SPECULAR_STRENGTH",pcMesh->fSpecularStrength);
	}

	IRenderer::GetRendererInstance()->SetVector("DIFFUSE_COLOR",&pcMesh->vDiffuseColor);
	//IRenderer::GetRendererInstance()->SetVector("SPECULAR_COLOR",&pcMesh->vSpecularColor);
	IRenderer::GetRendererInstance()->SetVector("AMBIENT_COLOR",&pcMesh->vAmbientColor);
	IRenderer::GetRendererInstance()->SetVector("EMISSIVE_COLOR",&pcMesh->vEmissiveColor);

	if (pcMesh->piDiffuseTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("DIFFUSE_SAMPLER",pcMesh->piDiffuseTexture);
	}
	if (pcMesh->piOpacityTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("OPACITY_SAMPLER",pcMesh->piOpacityTexture);
	}
	if (pcMesh->piSpecularTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("SPECULAR_SAMPLER",pcMesh->piSpecularTexture);
	}
	if (pcMesh->piAmbientTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("AMBIENT_SAMPLER",pcMesh->piAmbientTexture);
	}
	if (pcMesh->piEmissiveTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("EMISSIVE_SAMPLER",pcMesh->piEmissiveTexture);
	}
	if (pcMesh->piNormalTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("NORMAL_SAMPLER",pcMesh->piNormalTexture);
	}
	if (pcMesh->piShininessTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("SHININESS_SAMPLER",pcMesh->piShininessTexture);
	}
	if (pcMesh->piLightmapTexture!=-1)
	{
		IRenderer::GetRendererInstance()->SetTexture("LIGHTMAP_SAMPLER",pcMesh->piLightmapTexture);
	}
/*
	if (CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance().GetMode()){
		IRenderer::GetRendererInstance()->SetTexture("lw_tex_envmap",CBackgroundPainter::Instance().GetTexture());
	}*/
#endif
	return 1;
}
//-------------------------------------------------------------------------------
int CMaterialManager::SetupMaterial (
	AssetHelper::MeshHelper* pcMesh,
	const aiMatrix4x4& pcProj,
	const aiMatrix4x4& aiMe,
	const aiMatrix4x4& pcCam,
	const aiVector3D& vPos)
{
	ai_assert(0 != pcMesh);
//	if (-1==pcMesh->piEffect)return 0;

#if 1
	// setup wireframe/solid rendering mode
	if (mr->m_sOptions.eDrawMode == RenderOptions::WIREFRAME)
	{
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetfillWireFrame());
	}
	else
	{
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetfillSolid());
	}

	if (mr->m_sOptions.bCulling)
	{
//		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullFront());//Front???
	}
	else
	{
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullNone());
	}
#endif
		//ShaderID piEnd = mr->m_piDefaultEffect;//
		ShaderID shd = MeshRendererShadersFactory::GetShader(mr->m_szShaderName.c_str(), "main", "main");
		//mr->m_piMaterialEffect;//???
		//???pcMesh->piEffect;
	//if (-1==piEnd) return 1;
	IRenderer::GetRendererInstance()->setShader(shd);
/*
"float4 vecLightDir;\n"
"float4 vecEye;\n"
*/

	IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection", (const D3DXFROMWINEMATRIX*)&pcProj);

	IRenderer::GetRendererInstance()->SetMatrix("World", (const D3DXFROMWINEMATRIX*)&aiMe);
	IRenderer::GetRendererInstance()->SetMatrix("WorldInverseTranspose", (const D3DXFROMWINEMATRIX*)&pcCam);

	D3DXFROMWINEVECTOR4 apcVec[5];
	memset(apcVec,0,sizeof(apcVec));
	apcVec[0].x = mr->m_avLightDirs[0].x;
	apcVec[0].y = mr->m_avLightDirs[0].y;
	apcVec[0].z = mr->m_avLightDirs[0].z;
	apcVec[0].w = 0.0f;
	apcVec[1].x = mr->m_avLightDirs[0].x * -1.0f;
	apcVec[1].y = mr->m_avLightDirs[0].y * -1.0f;
	apcVec[1].z = mr->m_avLightDirs[0].z * -1.0f;
	apcVec[1].w = 0.0f;
	D3DXFROMWINEVec4Normalize(&apcVec[0],&apcVec[0]);
	D3DXFROMWINEVec4Normalize(&apcVec[1],&apcVec[1]);
	IRenderer::GetRendererInstance()->SetVector("vecLightDir",apcVec);
#if 0
	apcVec[0].x = ((mr->m_avLightColors[0]. >> 16)	& 0xFF) / 255.0f;
	apcVec[0].y = ((mr->m_avLightColors[0] >> 8)	& 0xFF) / 255.0f;
	apcVec[0].z = ((mr->m_avLightColors[0])			& 0xFF) / 255.0f;
#else
	apcVec[0].x = mr->m_avLightColors[0].x;
	apcVec[0].y = mr->m_avLightColors[0].y;
	apcVec[0].z= mr->m_avLightColors[0].z;
#endif
	apcVec[0].w = 1.0f;

	if( mr->m_sOptions.b3Lights)
	{
#if 0
		apcVec[1].x = ((mr->m_avLightColors[1] >> 16) & 0xFF) / 255.0f;
		apcVec[1].y = ((mr->m_avLightColors[1] >> 8) & 0xFF) / 255.0f;
		apcVec[1].z = ((mr->m_avLightColors[1]) & 0xFF) / 255.0f;
#else
	apcVec[1].x = mr->m_avLightColors[1].x;
	apcVec[1].y = mr->m_avLightColors[1].y;
	apcVec[1].z= mr->m_avLightColors[1].z;
#endif
		apcVec[1].w = 0.0f;
	} else
	{
		apcVec[1].x = 0.0f;
		apcVec[1].y = 0.0f;
		apcVec[1].z = 0.0f;
		apcVec[1].w = 0.0f;
	}

	apcVec[0] *= mr->m_fLightIntensity;
	apcVec[1] *= mr->m_fLightIntensity;
	//IRenderer::GetRendererInstance()->SetVectorArray("afLightColor",apcVec,5);
#if 0
	apcVec[0].x = ((mr->m_avLightColors[2] >> 16)	& 0xFF) / 255.0f;
	apcVec[0].y = ((mr->m_avLightColors[2] >> 8)	& 0xFF) / 255.0f;
	apcVec[0].z = ((mr->m_avLightColors[2])			& 0xFF) / 255.0f;
#else
	apcVec[0].x = mr->m_avLightColors[2].x;
	apcVec[0].y = mr->m_avLightColors[2].y;
	apcVec[0].z= mr->m_avLightColors[2].z;
#endif
	apcVec[0].w = 1.0f;
#if 0
	apcVec[1].x = ((mr->m_avLightColors[2] >> 16)	& 0xFF) / 255.0f;
	apcVec[1].y = ((mr->m_avLightColors[2] >> 8)	& 0xFF) / 255.0f;
	apcVec[1].z = ((mr->m_avLightColors[2])			& 0xFF) / 255.0f;
#else
	apcVec[1].x = mr->m_avLightColors[2].x;
	apcVec[1].y = mr->m_avLightColors[2].y;
	apcVec[1].z= mr->m_avLightColors[2].z;
#endif
	apcVec[1].w = 0.0f;

	// FIX: light intensity doesn't apply to ambient color
	//apcVec[0] *= mr->m_fLightIntensity;
	//apcVec[1] *= mr->m_fLightIntensity;
	//???IRenderer::GetRendererInstance()->SetVectorArray("afLightColorAmbient",apcVec,5);


	apcVec[0].x = vPos.x;
	apcVec[0].y = vPos.y;
	apcVec[0].z = vPos.z;
	IRenderer::GetRendererInstance()->SetVector( "vecEye", apcVec);

	// if the effect instance is shared by multiple materials we need to
	// recommit its whole state once per frame ...
	if (1)//???pcMesh->bSharedFX)
	{
		// now commit all constants to the shader
		if (1.0f != pcMesh->fOpacity)
		{
			//???IRenderer::GetRendererInstance()->SetFloat("TRANSPARENCY",pcMesh->fOpacity);
		}
		if (pcMesh->eShadingMode  != aiShadingMode_Gouraud)
		{
			IRenderer::GetRendererInstance()->SetFloat("SPECULARITY",pcMesh->fShininess);
			IRenderer::GetRendererInstance()->SetFloat("SPECULAR_STRENGTH",pcMesh->fSpecularStrength);
		}

		IRenderer::GetRendererInstance()->SetVector("DIFFUSE_COLOR",&pcMesh->vDiffuseColor);
		//IRenderer::GetRendererInstance()->SetVector("SPECULAR_COLOR",&pcMesh->vSpecularColor);
		IRenderer::GetRendererInstance()->SetVector("AMBIENT_COLOR",&pcMesh->vAmbientColor);
		IRenderer::GetRendererInstance()->SetVector("EMISSIVE_COLOR",&pcMesh->vEmissiveColor);

		if (pcMesh->piOpacityTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("OPACITY_SAMPLER",pcMesh->piOpacityTexture);
		}
		if (pcMesh->piDiffuseTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("DIFFUSE_SAMPLER",pcMesh->piDiffuseTexture);
		}
		if (pcMesh->piSpecularTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("SPECULAR_SAMPLER",pcMesh->piSpecularTexture);
		}
		if (pcMesh->piAmbientTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("AMBIENT_SAMPLER",pcMesh->piAmbientTexture);
		}
		if (pcMesh->piEmissiveTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("EMISSIVE_SAMPLER",pcMesh->piEmissiveTexture);
		}
		if (pcMesh->piNormalTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("NORMAL_SAMPLER",pcMesh->piNormalTexture);
		}
		if (pcMesh->piShininessTexture!=-1)
		{
			IRenderer::GetRendererInstance()->SetTexture("SHININESS_SAMPLER",pcMesh->piShininessTexture);
		}
		if (pcMesh->piLightmapTexture!=-1)
		{
				IRenderer::GetRendererInstance()->SetTexture("LIGHTMAP_SAMPLER",pcMesh->piLightmapTexture);
		}
/*		if (CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance().GetMode())
		{
			IRenderer::GetRendererInstance()->SetTexture("lw_tex_envmap",CBackgroundPainter::Instance().GetTexture());
		}*/
	}

	// disable culling, if necessary
	if (pcMesh->twosided && mr->m_sOptions.bCulling) {
//		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullNone());
	}
#if 0
	// setup the correct shader technique to be used for drawing
	if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
			mr->m_piDefaultEffect->SetTechnique( MaterialFXSpecular_FFh);
	} else
	if (mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(3,0) || mr->m_sOptions.bLowQuality)
	{
		if (mr->m_sOptions.b3Lights)
		{
			IRenderer::GetRendererInstance()->SetTechnique(MaterialFXSpecular_PS20_D2h);
		}
		else
		{
				IRenderer::GetRendererInstance()->SetTechnique(MaterialFXSpecular_PS20_D1h);
		}
	}
	else
	{
		if (mr->m_sOptions.b3Lights)
		{
			IRenderer::GetRendererInstance()->SetTechnique(MaterialFXSpecular_D2h);
		}
		else
		{
				IRenderer::GetRendererInstance()->SetTechnique(MaterialFXSpecular_D1h);
		}
	}
#else

#endif
	return 1;
}
//-------------------------------------------------------------------------------
int CMaterialManager::EndMaterial (AssetHelper::MeshHelper* pcMesh)
{
	ai_assert(0 != pcMesh);
	if (-1==pcMesh->piEffect)return 0;

	// reenable culling if necessary
	if (pcMesh->twosided && mr->m_sOptions.bCulling) {
//		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullFront());//Front???
	}

	return 1;
}
}; // end namespace AssimpView
#endif

