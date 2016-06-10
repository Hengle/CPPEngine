#ifdef BUILD_MESHRENDERER
#include <assimp_view.h>
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
#define __MESHRENDERER__ 1


#include <Framework3/IRenderer.h>

#include "stdafx.h"

namespace AssimpView {

struct SVertex
{
	float x,y,z,w,u,v;
};

CDisplay CDisplay::s_cInstance;
#if 0
//extern COLORREF mr->m_aclCustomColors[16] /*= {0}*/;
//extern float mr->m_fLoadTime;

//-------------------------------------------------------------------------------
// Table of colors used for normal vectors. 
//-------------------------------------------------------------------------------
#endif

//-------------------------------------------------------------------------------
// Recursivly count the number of nodes in an asset's node graph
// Used by LoadAsset()
//-------------------------------------------------------------------------------
void GetNodeCount(aiNode* pcNode, unsigned int* piCnt)
{
	*piCnt = *piCnt+1;
	for (unsigned int i = 0; i < pcNode->mNumChildren;++i)
		GetNodeCount(pcNode->mChildren[i],piCnt);
}

//-------------------------------------------------------------------------------
int CDisplay::EnableAnimTools(BOOL hm) 
{
//	EnableWindow(GetDlgItem(mr->m_hDlg,IDC_PLAY),hm);
//	EnableWindow(GetDlgItem(mr->m_hDlg,IDC_SLIDERANIM),hm);
	return 1;
}

//-------------------------------------------------------------------------------
// Fill animation combo box
int CDisplay::FillAnimList(void)
{
#if 0
	if (0 != mr->GetAsset()->pcScene->mNumAnimations)
	{
	// now fill in all animation names
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumAnimations;++i)	{
			SendDlgItemMessage(mr->m_hDlg,IDC_COMBO1,CB_ADDSTRING,0,
				( LPARAM ) mr->GetAsset()->pcScene->mAnimations[i]->mName.data);
		}

		// also add a dummy - 'none'
		SendDlgItemMessage(mr->m_hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)"none");

		// select first
		SendDlgItemMessage(mr->m_hDlg,IDC_COMBO1,CB_SETCURSEL,0,0);

		EnableAnimTools(TRUE);
	}
	else // tools remain disabled
	
		EnableAnimTools(FALSE);
#endif
	return 1;
}
//-------------------------------------------------------------------------------
// Clear the list of animations
int CDisplay::ClearAnimList(void)
{
	// clear the combo box
//	SendDlgItemMessage(mr->m_hDlg,IDC_COMBO1,CB_RESETCONTENT,0,0);
	return 1;
}
//-------------------------------------------------------------------------------
// Clear the tree view
int CDisplay::ClearDisplayList(void)
{
	// clear the combo box
//	TreeView_DeleteAllItems(GetDlgItem(mr->m_hDlg,IDC_TREE1));
	this->Reset();
	return 1;
}
#if 0
//-------------------------------------------------------------------------------
// Add a specific node to the display list
int CDisplay::AddNodeToDisplayList(
	unsigned int iIndex, 
	unsigned int iDepth,
	aiNode* pcNode//,
	//HTREEITEM hRoot
	)
{
#if 0
	ai_assert(0 != pcNode);
	ai_assert(0 != hRoot);

	char chTemp[512];

	if(0 == pcNode->mName.length)	{
		if (iIndex >= 100)	{
			iIndex += iDepth  * 1000;
		}
		else if (iIndex >= 10)
		{
			iIndex += iDepth  * 100;
		}
		else iIndex += iDepth  * 10;
		stx_snprintf(chTemp,"Node %i",iIndex);
	}
	else stx_strlcpy(chTemp,pcNode->mName.data,strlen(pcNode->mName.data)+1);

	TVITEMEXW tvi; 
	TVINSERTSTRUCTW sNew;
	
	wchar_t tmp[512];
	int t = MultiByteToWideChar(CP_UTF8,0,chTemp,-1,tmp,512);
	
	tvi.pszText = tmp;
	tvi.cchTextMax = (int)t;

	tvi.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_IMAGE | TVIF_HANDLE | TVIF_PARAM;
	tvi.iImage = this->m_aiImageList[AI_VIEW_IMGLIST_NODE];
	tvi.iSelectedImage = this->m_aiImageList[AI_VIEW_IMGLIST_NODE];
	tvi.lParam = (LPARAM)5; 

	sNew.itemex = tvi; 
	sNew.hInsertAfter = TVI_LAST; 
	sNew.hParent = hRoot;

	// add the item to the list
	HTREEITEM hTexture = (HTREEITEM)SendMessage(GetDlgItem(mr->m_hDlg,IDC_TREE1), 
		TVM_INSERTITEMW, 
		0,
		(LPARAM)(LPTVINSERTSTRUCT)&sNew);

	// recursively add all child nodes
	++iDepth;
	for (unsigned int i = 0; i< pcNode->mNumChildren;++i){
		AddNodeToDisplayList(i,iDepth,pcNode->mChildren[i],hTexture);
	}

	// add the node to the list
	NodeInfo info;
	info.hTreeItem = hTexture;
	info.psNode = pcNode;
	this->AddNode(info);
#endif
	return 1;
}
#endif
//-------------------------------------------------------------------------------
// Replace the currently selected texture by another one
int CDisplay::ReplaceCurrentTexture(const char* szPath)
{
#if 0
	ai_assert(0 != szPath);

	
	// well ... try to load it
	TextureID piTexture = -1;
	aiString szString;
	stx_strlcpy(szString.data,szPath,strlen(szPath)+1);
	szString.length = strlen(szPath);
	CMaterialManager::Instance(mr).LoadTexture(&piTexture,&szString);

	if (!piTexture)	{
		//CLogDisplay::Instance(mr).AddEntry("[ERROR] Unable to load this texture",
//			D3DCOLOR_ARGB(0xFF,0xFF,0x0,0x0));
		return 0;
	}

	// we must also change the icon of the corresponding tree
	// view item if the default texture was previously set
	TVITEMEX tvi; 
	tvi.mask = TVIF_SELECTEDIMAGE | TVIF_IMAGE;
	tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_MATERIAL];
	tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_MATERIAL];

	TreeView_SetItem(GetDlgItem(mr->m_hDlg,IDC_TREE1),
		m_pcCurrentTexture->hTreeItem);

	// change this in the old aiMaterial structure, too
	Assimp::MaterialHelper* pcMat = (Assimp::MaterialHelper*)
		mr->GetAsset()->pcScene->mMaterials[m_pcCurrentTexture->iMatIndex];
 	
	// update all meshes referencing this material
	for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
	{
		if (this->m_pcCurrentTexture->iMatIndex != mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			continue;

		AssetHelper::MeshHelper* pcMesh = mr->GetAsset()->apcMeshes[i];
		TextureID* tex = 0;
		const char* tex_string  = 0;

		switch (this->m_pcCurrentTexture->iType)
		{
		case aiTextureType_DIFFUSE:
			tex = &pcMesh->piDiffuseTexture;
			tex_string = "DIFFUSE_TEXTURE";
			break;
		case aiTextureType_AMBIENT:
			tex = &pcMesh->piAmbientTexture;
			tex_string = "AMBIENT_TEXTURE";
			break;
		case aiTextureType_SPECULAR:
			tex = &pcMesh->piSpecularTexture;
			tex_string = "SPECULAR_TEXTURE";
			break;
		case aiTextureType_EMISSIVE:
			tex = &pcMesh->piEmissiveTexture;
			tex_string = "EMISSIVE_TEXTURE";
			break;
		case aiTextureType_LIGHTMAP:
			tex = &pcMesh->piLightmapTexture;
			tex_string = "LIGHTMAP_TEXTURE";
			break;
		case aiTextureType_DISPLACEMENT:
		case aiTextureType_REFLECTION:
		case aiTextureType_UNKNOWN:
			break;
		case aiTextureType_SHININESS:
			tex = &pcMesh->piShininessTexture;
			tex_string = "SHININESS_TEXTURE";
			break;
		case aiTextureType_NORMALS:
		case aiTextureType_HEIGHT:

			// special handling here 
			if ((pcMesh->piNormalTexture!=-1) && pcMesh->piNormalTexture != piTexture)	{
				pcMesh->piNormalTexture = piTexture;
				CMaterialManager::Instance(mr).HMtoNMIfNecessary(pcMesh->piNormalTexture,&pcMesh->piNormalTexture,true);
				m_pcCurrentTexture->piTexture = &pcMesh->piNormalTexture;

				if (!pcMesh->bSharedFX)	{
					IRenderer::GetRendererInstance()->SetTexture("NORMAL_TEXTURE",piTexture);
				}
			}
			break;
		default: //case aiTextureType_OPACITY && case aiTextureType_OPACITY | 0x40000000:
			
			tex = &pcMesh->piOpacityTexture;
			tex_string = "OPACITY_TEXTURE";
			break;
		};
		if (tex && *tex && *tex != piTexture)
		{
			*tex = piTexture;
			m_pcCurrentTexture->piTexture = tex;

			//if (!pcMesh->bSharedFX){
				IRenderer::GetRendererInstance()->SetTexture(tex_string,piTexture);
			//}
		}
	}
	// now update the material itself
	aiString szOld;

	aiGetMaterialString(pcMat,AI_MATKEY_TEXTURE(m_pcCurrentTexture->iType,0),&szOld);
	pcMat->AddProperty(&szString,AI_MATKEY_TEXTURE(m_pcCurrentTexture->iType,0));

#if 0
	char szBuffer[512];
	stx_snprintf(szBuffer,"%s%s",szKey,"_old");

	if (AI_SUCCESS != aiGetMaterialString(pcMat, szBuffer, &szOld))
	{
		pcMat->AddProperty(&szOld,szBuffer );
	}
	else if (szString.length == szOld.length && 
		0 == ASSIMP_stricmp(szString.data,szOld.data))
	{
		pcMat->RemoveProperty(szBuffer);
	}
#endif
#endif
	return 1;
}
//-------------------------------------------------------------------------------
#if 0
int CDisplay::AddTextureToDisplayList(unsigned int iType,
	unsigned int iIndex,
	const aiString* szPath,
	HTREEITEM hFX, 
	unsigned int iUVIndex		/*= 0*/,
	const float fBlendFactor	/*= 0.0f*/,
	aiTextureOp eTextureOp		/*= aiTextureOp_Multiply*/,
	unsigned int iMesh		/*= 0*/)
{
#if 0
	ai_assert(0 != szPath);
	//ai_assert(0 != pcMat);

	char chTemp[512];
	char chTempEmb[256];
	const char* sz = stx_strrchr(szPath->data,stx_getOsSeparator());
	if (!sz)sz = stx_strrchr(szPath->data,stx_getOsSeparator());
	if (!sz)
	{
		if ('*' == *szPath->data)
		{
			int iIndex = atoi(szPath->data+1);
			stx_snprintf(chTempEmb,"Embedded #%i",iIndex);
			sz = chTempEmb;
		}
		else
		{
			sz = szPath->data;
		}
	}

	bool bIsExtraOpacity = 0 != (iType & 0x40000000);
	const char* szType;
	TextureID* piTexture;
	switch (iType)
	{
	case aiTextureType_DIFFUSE:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piDiffuseTexture;
		szType = "Diffuse";
		break;
	case aiTextureType_SPECULAR:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piSpecularTexture;
		szType = "Specular";
		break;
	case aiTextureType_AMBIENT:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piAmbientTexture;
		szType = "Ambient";
		break;
	case aiTextureType_EMISSIVE:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piEmissiveTexture;
		szType = "Emissive";
		break;
	case aiTextureType_HEIGHT:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piNormalTexture;
		szType = "Heightmap";
		break;
	case aiTextureType_NORMALS:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piNormalTexture;
		szType = "Normalmap";
		break;
	case aiTextureType_SHININESS:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piShininessTexture;
		szType = "Shininess";
		break;
	case aiTextureType_LIGHTMAP:
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piLightmapTexture;
		szType = "Lightmap";
		break;
	case aiTextureType_DISPLACEMENT:
		piTexture = 0;
		szType = "Displacement";
		break;
	case aiTextureType_REFLECTION:
		piTexture = 0;
		szType = "Reflection";
		break;
	case aiTextureType_UNKNOWN:
		piTexture = 0;
		szType = "Unknown";
		break;
	default: // opacity + opacity | mask
		piTexture = &mr->GetAsset()->apcMeshes[iMesh]->piOpacityTexture;
		szType = "Opacity";
		break;
	};
	if (bIsExtraOpacity)	{
		stx_snprintf(chTemp,"%s %i (<copy of diffuse #1>)",szType,iIndex+1);
	}
	else 
		stx_snprintf(chTemp,"%s %i (%s)",szType,iIndex+1,sz);

	TVITEMEX tvi; 
	TVINSERTSTRUCT sNew;
	tvi.pszText = chTemp;
	tvi.cchTextMax = (int)strlen(chTemp);
	tvi.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_IMAGE | TVIF_HANDLE | TVIF_HANDLE;
	tvi.lParam = (LPARAM)20;

	// find out whether this is the default texture or not

	if (piTexture && *piTexture)	{
		// {9785DA94-1D96-426b-B3CB-BADC36347F5E}
		static const GUID guidPrivateData = 
			{ 0x9785da94, 0x1d96, 0x426b, 
			{ 0xb3, 0xcb, 0xba, 0xdc, 0x36, 0x34, 0x7f, 0x5e } };
/*???
		ai_uint32_t iData = 0;
		DWORD dwSize = 4;
		(*piTexture)->GetPrivateData(guidPrivateData,&iData,&dwSize);

		if (0xFFFFFFFF == iData)
		{
			tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE_INVALID];
			tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE_INVALID];
		}
		else
		{
			tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE];
			tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE];
		}
		*/
	}
	else
	{
		tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE_INVALID];
		tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_TEXTURE_INVALID];
	}

	sNew.itemex = tvi; 
	sNew.hInsertAfter = TVI_LAST; 
	sNew.hParent = hFX;

	// add the item to the list
	HTREEITEM hTexture = (HTREEITEM)SendMessage(GetDlgItem(mr->m_hDlg,IDC_TREE1), 
		TVM_INSERTITEM, 
		0,
		(LPARAM)(LPTVINSERTSTRUCT)&sNew);

	// add it to the list
	CDisplay::TextureInfo sInfo;
	sInfo.iUV = iUVIndex;
	sInfo.fBlend = fBlendFactor;
	sInfo.eOp = eTextureOp;
	sInfo.szPath = szPath->data;
	sInfo.hTreeItem = hTexture;
	sInfo.piTexture = piTexture;
	sInfo.iType = iType;
	sInfo.iMatIndex = mr->GetAsset()->pcScene->mMeshes[iMesh]->mMaterialIndex;
	AddTexture(sInfo);
#endif
	return 1;
}

//-------------------------------------------------------------------------------
int CDisplay::AddMaterialToDisplayList(HTREEITEM hRoot, 
	unsigned int iIndex)
{
#if 0
	ai_assert(0 != hRoot);

	aiMaterial* pcMat = mr->GetAsset()->pcScene->mMaterials[iIndex];

	
	// find the first mesh using this material index
	unsigned int iMesh = 0;
	for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
	{
		if (iIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
		{
			iMesh = i;
			break;
		}
	}

	// use the name of the material, if possible
	char chTemp[512];
	aiString szOut;
	if (AI_SUCCESS != aiGetMaterialString(pcMat,AI_MATKEY_NAME,&szOut))
	{
		stx_snprintf(chTemp,"Material %i",iIndex+1);
	}
	else
	{
		stx_snprintf(chTemp,"%s (%i)",szOut.data,iIndex+1);
	}
	TVITEMEXW tvi; 
	TVINSERTSTRUCTW sNew;

	wchar_t tmp[512];
	int t = MultiByteToWideChar(CP_UTF8,0,chTemp,-1,tmp,512);
	
	tvi.pszText = tmp;
	tvi.cchTextMax = (int)t;
	tvi.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_IMAGE | TVIF_HANDLE | TVIF_PARAM ;
	tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_MATERIAL];
	tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_MATERIAL];
	tvi.lParam = (LPARAM)10; 
	//tvi.state = TVIS_EXPANDED | TVIS_EXPANDEDONCE ;

	sNew.itemex = tvi; 
	sNew.hInsertAfter = TVI_LAST; 
	sNew.hParent = hRoot;

	// add the item to the list
	HTREEITEM hTexture = (HTREEITEM)SendMessage(GetDlgItem(mr->m_hDlg,IDC_TREE1), 
		TVM_INSERTITEMW, 
		0,
		(LPARAM)(LPTVINSERTSTRUCT)&sNew);

	// for each texture in the list ... add it
	unsigned int iUV;
	float fBlend;
	aiTextureOp eOp;
	aiString szPath;
	bool bNoOpacity = true;
	for (unsigned int i = 0; i <= AI_TEXTURE_TYPE_MAX;++i)
	{
		unsigned int iNum = 0;
		while (true)
		{
			if (AI_SUCCESS != aiGetMaterialTexture(pcMat,(aiTextureType)i,iNum,
				&szPath,0, &iUV,&fBlend,&eOp))
			{
				break;
			}
			if (aiTextureType_OPACITY == i)bNoOpacity = false;
			AddTextureToDisplayList(i,iNum,&szPath,hTexture,iUV,fBlend,eOp,iMesh);
			++iNum;
		}
	}

	AssetHelper::MeshHelper* pcMesh = mr->GetAsset()->apcMeshes[iMesh];

	if (
		(pcMesh->piDiffuseTexture!=-1) 
		&& 
		pcMesh->piDiffuseTexture == pcMesh->piOpacityTexture 
		&& bNoOpacity)
	{
		// check whether the diffuse texture is not a default texture

		// {9785DA94-1D96-426b-B3CB-BADC36347F5E}
		static const GUID guidPrivateData = 
			{ 0x9785da94, 0x1d96, 0x426b, 
			{ 0xb3, 0xcb, 0xba, 0xdc, 0x36, 0x34, 0x7f, 0x5e } };
/*???
		ai_uint32_t iData = 0;
		DWORD dwSize = 4;
		if(FAILED( pcMesh->piDiffuseTexture->GetPrivateData(guidPrivateData,&iData,&dwSize) ||
			0xffffffff == iData))
		{
			// seems the diffuse texture contains alpha, therefore it has been
			// added to the opacity channel, too. Add a special value ...
			AddTextureToDisplayList(aiTextureType_OPACITY | 0x40000000,
				0,&szPath,hTexture,iUV,fBlend,eOp,iMesh);
		}
		*/
	}

	// add the material to the list
	MaterialInfo info;
	info.hTreeItem = hTexture;
	info.psMaterial = pcMat;
	info.iIndex = iIndex;
	info.piEffect = mr->GetAsset()->apcMeshes[iMesh]->piEffect;
	this->AddMaterial(info);
	#endif
	return 1;
}
#endif
//-------------------------------------------------------------------------------
// Expand all elements in the treeview
int CDisplay::ExpandTree()
{
#if 0
	// expand all materials
	for (std::vector< MaterialInfo >::iterator
		i =  m_asMaterials.begin();
		i != m_asMaterials.end();++i)
	{
		TreeView_Expand(GetDlgItem(mr->m_hDlg,IDC_TREE1),(*i).hTreeItem,TVE_EXPAND);
	}
	// expand all nodes
	for (std::vector< NodeInfo >::iterator
		i =  m_asNodes.begin();
		i != m_asNodes.end();++i)
	{
		TreeView_Expand(GetDlgItem(mr->m_hDlg,IDC_TREE1),(*i).hTreeItem,TVE_EXPAND);
	}
	TreeView_Expand(GetDlgItem(mr->m_hDlg,IDC_TREE1),m_hRoot,TVE_EXPAND);
#endif
	return 1;
}
//-------------------------------------------------------------------------------
// Get image list for tree view
int CDisplay::LoadImageList(void)
{
#if 0
	if (!m_hImageList)
	{
		// First, create the image list we will need.
		// FIX: Need RGB888 color space to display all colors correctly
		HIMAGELIST hIml = ImageList_Create( 16,16,ILC_COLOR24, 5, 0 );

		// Load the bitmaps and add them to the image lists.
		HBITMAP hBmp = LoadBitmap(mr->m_hInstance, MAKEINTRESOURCE(IDB_BFX));
		m_aiImageList[AI_VIEW_IMGLIST_MATERIAL] = ImageList_Add(hIml, hBmp, 0);
		DeleteObject(hBmp);

		hBmp = LoadBitmap(mr->m_hInstance, MAKEINTRESOURCE(IDB_BNODE));
		m_aiImageList[AI_VIEW_IMGLIST_NODE] = ImageList_Add(hIml, hBmp, 0);
		DeleteObject(hBmp);

		hBmp = LoadBitmap(mr->m_hInstance, MAKEINTRESOURCE(IDB_BTX));
		m_aiImageList[AI_VIEW_IMGLIST_TEXTURE] = ImageList_Add(hIml, hBmp, 0);
		DeleteObject(hBmp);

		hBmp = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BTXI));
		m_aiImageList[AI_VIEW_IMGLIST_TEXTURE_INVALID] = ImageList_Add(hIml, hBmp, 0);
		DeleteObject(hBmp);

		hBmp = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BROOT));
		m_aiImageList[AI_VIEW_IMGLIST_MODEL] = ImageList_Add(hIml, hBmp, 0);
		DeleteObject(hBmp);

		// Associate the image list with the tree.
		TreeView_SetImageList(GetDlgItem(mr->m_hDlg,IDC_TREE1), hIml, TVSIL_NORMAL);

		m_hImageList = hIml;
	}
#endif
	return 1;
}
//-------------------------------------------------------------------------------
// Fill tree view
int CDisplay::FillDisplayList(void)
{
#if 0
	LoadImageList();

	// Initialize the tree view window.
	// fill in the first entry
	TVITEMEX tvi; 
	TVINSERTSTRUCT sNew;
	tvi.pszText = "Model";
	tvi.cchTextMax = (int)strlen(tvi.pszText);
	tvi.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_IMAGE | TVIF_HANDLE | TVIF_STATE;
	tvi.state = TVIS_EXPANDED;
	tvi.iImage = m_aiImageList[AI_VIEW_IMGLIST_MODEL];
	tvi.iSelectedImage = m_aiImageList[AI_VIEW_IMGLIST_MODEL];
	tvi.lParam = (LPARAM)0; 

	sNew.itemex = tvi; 
	sNew.hInsertAfter = TVI_ROOT; 
	sNew.hParent = 0;

	// add the root item to the tree
	m_hRoot = (HTREEITEM)SendMessage(GetDlgItem(mr->m_hDlg,IDC_TREE1), 
		TVM_INSERTITEM, 
		0,
		(LPARAM)(LPTVINSERTSTRUCT)&sNew);

	// add each loaded material to the tree
	for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMaterials;++i)
		AddMaterialToDisplayList(m_hRoot,i);

	// now add all loaded nodes recursively
	AddNodeToDisplayList(0,0,mr->GetAsset()->pcScene->mRootNode,m_hRoot);

	// now expand all parent nodes in the tree
	ExpandTree();

	// everything reacts a little bit slowly if D3D is rendering,
	// so give GDI a small hint to leave the couch and work ;-)
	UpdateWindow(mr->m_hDlg);
#endif
	return 1;
}
//-------------------------------------------------------------------------------
// Main render loop
int CDisplay::OnRender(D3DXFROMWINEMATRIX& m)
{
	// update possible animation
	if( mr->GetAsset())
	{
		static double lastPlaying = 0.;

		ai_assert( mr->GetAsset()->mAnimator);
		if (mr->m_bPlay) {
			mr->m_dCurrent += clock()/ double( CLOCKS_PER_SEC)   -lastPlaying;

			double time = mr->m_dCurrent;
			aiAnimation* mAnim = mr->GetAsset()->mAnimator->CurrentAnim();
			if(  mAnim && mAnim->mDuration > 0.0) {
				double tps = mAnim->mTicksPerSecond ? mAnim->mTicksPerSecond : 25.f;
				time = fmod( time, mAnim->mDuration/tps);
				//SendDlgItemMessage(mr->m_hDlg,IDC_SLIDERANIM,TBM_SETPOS,TRUE,LPARAM(10000 * (time/(mAnim->mDuration/tps))));			
			}

			mr->GetAsset()->mAnimator->Calculate( time );
			lastPlaying = mr->m_dCurrent;
		}
	}
	//LOG_PRINT("mr->m_piDevice=%d\n",mr->m_piDevice);

#if 1
	switch (m_iViewMode)
	{
	case VIEWMODE_FULL:
	case VIEWMODE_NODE:
		RenderFullScene(m);
		break;
	case VIEWMODE_MATERIAL:
		RenderMaterialView(m);
		break;
	//case VIEWMODE_TEXTURE:
	//	RenderTextureView(m);
		break;
	};
#else
	IRenderer::GetRendererInstance()->drawTexture(0);
#endif
	// Now render the log display in the upper right corner of the window
//	//CLogDisplay::Instance(mr).OnRender();


	// don't remove this, problems on some older machines (AMD timing bug)
	//???Sleep(10);
#if defined(OS_Debian)
	STX_Service::GetTimerInstance()->stx_Delay(1000);
#else
	STX_Service::GetTimerInstance()->stx_Delay(10);
#endif
	return 1;
}	
//-------------------------------------------------------------------------------
// Update UI
#if 0
void UpdateColorFieldsInUI()
{
	
	InvalidateRect(GetDlgItem(mr->m_hDlg,IDC_LCOLOR1),0,TRUE);
	InvalidateRect(GetDlgItem(mr->m_hDlg,IDC_LCOLOR2),0,TRUE);
	InvalidateRect(GetDlgItem(mr->m_hDlg,IDC_LCOLOR3),0,TRUE);

	UpdateWindow(GetDlgItem(mr->m_hDlg,IDC_LCOLOR1));
	UpdateWindow(GetDlgItem(mr->m_hDlg,IDC_LCOLOR2));
	UpdateWindow(GetDlgItem(mr->m_hDlg,IDC_LCOLOR3));
	
}
#endif
//-------------------------------------------------------------------------------
// FIll statistics UI
int CDisplay::FillDefaultStatistics(void)
{
#if 0
	/*
	if (!mr->GetAsset())
	{
		// clear all stats edit controls
		SetDlgItemText(mr->m_hDlg,IDC_EVERT,"0");
		SetDlgItemText(mr->m_hDlg,IDC_EFACE,"0");
		SetDlgItemText(mr->m_hDlg,IDC_EMAT,"0");
		SetDlgItemText(mr->m_hDlg,IDC_ENODE,"0");
		SetDlgItemText(mr->m_hDlg,IDC_ESHADER,"0");
		SetDlgItemText(mr->m_hDlg,IDC_ETEX,"0");
		return 1;
	}
*/
	// get the number of vertices/faces in the model
	unsigned int iNumVert = 0;
	unsigned int iNumFaces = 0;
	for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
	{
		iNumVert += mr->GetAsset()->pcScene->mMeshes[i]->mNumVertices;
		iNumFaces += mr->GetAsset()->pcScene->mMeshes[i]->mNumFaces;
	}/*
	// and fill the statistic edit controls
	char szOut[1024];
	stx_snprintf(szOut,"%i",(int)iNumVert);
	SetDlgItemText(mr->m_hDlg,IDC_EVERT,szOut);
	stx_snprintf(szOut,"%i",(int)iNumFaces);
	SetDlgItemText(mr->m_hDlg,IDC_EFACE,szOut);
	stx_snprintf(szOut,"%i",(int)mr->GetAsset()->pcScene->mNumMaterials);
	SetDlgItemText(mr->m_hDlg,IDC_EMAT,szOut);
	stx_snprintf(szOut,"%i",(int)mr->GetAsset()->pcScene->mNumMeshes);
	SetDlgItemText(mr->m_hDlg,IDC_EMESH,szOut);
*/
	// need to get the number of nodes
	iNumVert = 0;
	GetNodeCount(mr->GetAsset()->pcScene->mRootNode,&iNumVert);
	stx_snprintf(szOut,"%i",(int)iNumVert);
	SetDlgItemText(mr->m_hDlg,IDC_ENODEWND,szOut);

	// now get the number of unique shaders generated for the asset
	// (even if the environment changes this number won't change)
	stx_snprintf(szOut,"%i", CMaterialManager::Instance(mr).GetShaderCount());
	SetDlgItemText(mr->m_hDlg,IDC_ESHADER,szOut);

	stx_snprintf(szOut,"%.5f",(float)mr->m_fLoadTime);
	SetDlgItemText(mr->m_hDlg,IDC_ELOAD,szOut);

//	UpdateColorFieldsInUI();
	UpdateWindow(mr->m_hDlg);
#endif
	return 1;
}
//-------------------------------------------------------------------------------
// Reset UI
int CDisplay::Reset(void)
{
	// clear all lists
	m_asMaterials.clear();
	m_asTextures.clear();
	m_asNodes.clear();

//	m_hRoot = 0;

	return OnSetupNormalView();
}
//-------------------------------------------------------------------------------
// reset to standard statistics view
#if 0
void ShowNormalUIComponents()
{
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_NUMNODES),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ENODEWND),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_NUMSHADERS),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_LOADTIME),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ESHADER),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ELOAD),SW_SHOW);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_VIEWMATRIX),SW_HIDE);
}
#endif
//-------------------------------------------------------------------------------
int CDisplay::OnSetupNormalView()
{/*
	if (VIEWMODE_NODE == m_iViewMode)
	{
		ShowNormalUIComponents();
	}
*/
	/*
	// now ... change the meaning of the statistics fields back
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMVERTS),"Verts:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMNODES),"Nodes:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMFACES),"Faces:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMSHADERS),"Shd:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMATS),"Mats:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMESHES),"Mesh:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_LOADTIME),"Time:");
*/
	FillDefaultStatistics();
	
	SetViewMode(VIEWMODE_FULL);

	// for debugging
	m_pcCurrentMaterial = 0;
	m_pcCurrentTexture = 0;
	m_pcCurrentNode = 0;

	// redraw the color fields in the UI --- their purpose has possibly changed
//	UpdateColorFieldsInUI();
//	UpdateWindow(mr->m_hDlg);
	return 1;
}
//-------------------------------------------------------------------------------
int CDisplay::OnSetupNodeView(NodeInfo* pcNew)
{
#if 0
	ai_assert(0 != pcNew);

	if (m_pcCurrentNode == pcNew)return 2;
	
	// now ... change the meaning of the statistics fields back
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMVERTS),"Verts:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMFACES),"Faces:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMATS),"Mats:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMESHES),"Mesh:");

	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_NUMNODES),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ENODEWND),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_NUMSHADERS),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_LOADTIME),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ESHADER),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_ELOAD),SW_HIDE);
	ShowWindow(GetDlgItem(mr->m_hDlg,IDC_VIEWMATRIX),SW_SHOW);

	char szTemp[1024];
	stx_snprintf(szTemp,
		"%.2f %.2f %.2f\r\n"
		"%.2f %.2f %.2f\r\n"
		"%.2f %.2f %.2f\r\n"
		"%.2f %.2f %.2f\r\n",
		pcNew->psNode->mTransformation.a1,
		pcNew->psNode->mTransformation.b1,
		pcNew->psNode->mTransformation.c1,
		pcNew->psNode->mTransformation.a2,
		pcNew->psNode->mTransformation.b2,
		pcNew->psNode->mTransformation.c2,
		pcNew->psNode->mTransformation.a3,
		pcNew->psNode->mTransformation.b3,
		pcNew->psNode->mTransformation.c3,
		pcNew->psNode->mTransformation.a4,
		pcNew->psNode->mTransformation.b4,
		pcNew->psNode->mTransformation.c4);
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_VIEWMATRIX),szTemp);


	m_pcCurrentNode = pcNew;
	SetViewMode(VIEWMODE_NODE);
#endif
	return 1;
}
//-------------------------------------------------------------------------------
int CDisplay::OnSetupMaterialView(MaterialInfo* pcNew)
{
	ai_assert(0 != pcNew);

	if (m_pcCurrentMaterial == pcNew)return 2;

//	if (VIEWMODE_NODE == m_iViewMode)
//		ShowNormalUIComponents();

	m_pcCurrentMaterial = pcNew;
	SetViewMode(VIEWMODE_MATERIAL);

	// redraw the color fields in the UI --- their purpose has possibly changed
//	UpdateColorFieldsInUI();
//	UpdateWindow(mr->m_hDlg);
	return 1;
}
//-------------------------------------------------------------------------------
int CDisplay::OnSetupTextureView(TextureInfo* pcNew)
{
#if 0
	ai_assert(0 != pcNew);

	if (this->m_pcCurrentTexture == pcNew)return 2;
/*
	if (VIEWMODE_NODE == this->m_iViewMode)
	{
		ShowNormalUIComponents();
	}
*/
	if ((aiTextureType_OPACITY | 0x40000000) == pcNew->iType)
	{
		// for opacity textures display a warn message
		//CLogDisplay::Instance(mr).AddEntry("[INFO] This texture is not existing in the "
//			"original mesh",D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0));
		//CLogDisplay::Instance(mr).AddEntry("It is a copy of the alpha channel of the first "
//			"diffuse texture",D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0));
	}

	// check whether the pattern background effect is supported
//	if (mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(3,0))
	{
		//CLogDisplay::Instance(mr).AddEntry("[WARN] The background shader won't work "
//			"on your system, it required PS 3.0 hardware. A default color is used ...",
//			D3DCOLOR_ARGB(0xFF,0xFF,0x00,0));
	}

	this->m_fTextureZoom = 1000.0f;
	this->m_vTextureOffset.x = this->m_vTextureOffset.y = 0.0f;

	this->m_pcCurrentTexture = pcNew;
	this->SetViewMode(VIEWMODE_TEXTURE);

	// now ... change the meaning of the statistics fields
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMVERTS),"Width:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMNODES),"Height:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMFACES),"Format:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMSHADERS),"MIPs:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMATS),"UV:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_NUMMESHES),"Blend:");
	SetWindowText(GetDlgItem(mr->m_hDlg,IDC_LOADTIME),"Op:");

	// and fill them with data
	SURFACE_DESC sDesc;
	if (pcNew->piTexture && *pcNew->piTexture) {
		
		IRenderer::GetRendererInstance()->GetLevelDesc((*pcNew->piTexture),0,&sDesc);
		char szTemp[128];

		stx_snprintf(szTemp,"%i",sDesc.Width);
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_EVERT),szTemp);

		stx_snprintf(szTemp,"%i",sDesc.Height);
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_ENODEWND),szTemp);

		stx_snprintf(szTemp,"%i",
			IRenderer::GetRendererInstance()->GetLevelCount((*pcNew->piTexture)));
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_ESHADER),szTemp);

		stx_snprintf(szTemp,"%i",pcNew->iUV);
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_EMAT),szTemp);

		stx_snprintf(szTemp,"%f",pcNew->fBlend);
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_EMESH),szTemp);

		const char* szOp;
		switch (pcNew->eOp)
		{
		case aiTextureOp_Add:
			szOp = "add";break;
		case aiTextureOp_Subtract:
			szOp = "sub";break;
		case aiTextureOp_Divide:
			szOp = "div";break;
		case aiTextureOp_SignedAdd:
			szOp = "addsign";break;
		case aiTextureOp_SmoothAdd:
			szOp = "addsmooth";break;
		default: szOp = "mul";
		};
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_ELOAD),szOp);

		// NOTE: Format is always ARGB8888 since other formats are
		// converted to this format ...
		SetWindowText(GetDlgItem(mr->m_hDlg,IDC_EFACE),"ARGB8");
/*???
		// check whether this is the default texture
		if (pcNew->piTexture)
		{
			// {9785DA94-1D96-426b-B3CB-BADC36347F5E}
			static const GUID guidPrivateData = 
			{ 0x9785da94, 0x1d96, 0x426b, 
			{ 0xb3, 0xcb, 0xba, 0xdc, 0x36, 0x34, 0x7f, 0x5e } };

			ai_uint32_t iData = 0;
			DWORD dwSize = 4;
			(*pcNew->piTexture)->GetPrivateData(guidPrivateData,&iData,&dwSize);

			if (0xFFFFFFFF == iData)
			{
				//CLogDisplay::Instance(mr).AddEntry("[ERROR] Texture could not be loaded. "
					"The displayed texture is a default texture",
					D3DCOLOR_ARGB(0xFF,0xFF,0,0));
				return 0;
			}
		}
		*/
	}
	
	// redraw the color fields in the UI --- their purpose has possibly changed
//	UpdateColorFieldsInUI();
	UpdateWindow(mr->m_hDlg);
#endif
	return 1;
}
#if 0
//-------------------------------------------------------------------------------
int CDisplay::OnSetup(HTREEITEM p_hTreeItem)
{
	// search in our list for the item
	union	{
		TextureInfo* pcNew;
		NodeInfo* pcNew2;
		MaterialInfo* pcNew3;	
	};

	pcNew = 0;
	for (std::vector<TextureInfo>::iterator i =  m_asTextures.begin();i != m_asTextures.end();++i){
		if (p_hTreeItem == (*i).hTreeItem)	{
			pcNew = &(*i);
			break;
		}
	}
	if (pcNew)	{
		return OnSetupTextureView(pcNew);
	}

	// seach the node list
	for (std::vector<NodeInfo>::iterator i =  m_asNodes.begin(); i != m_asNodes.end();++i){
		if (p_hTreeItem == (*i).hTreeItem)	{
			pcNew2 = &(*i);
			break;
		}
	}
	if (pcNew2)	{
		return OnSetupNodeView(pcNew2);
	}

	// seach the material list
	for (std::vector<MaterialInfo>::iterator i =  m_asMaterials.begin();i != m_asMaterials.end();++i){
		if (p_hTreeItem == (*i).hTreeItem){
			pcNew3 = &(*i);
			break;
		}
	}
	if (pcNew3)	{
		return OnSetupMaterialView(pcNew3);
	}
	return OnSetupNormalView();
}
//-------------------------------------------------------------------------------
int CDisplay::ShowTreeViewContextMenu(HTREEITEM hItem)
{
	ai_assert(0 != hItem);

	HMENU hDisplay = 0;

	// search in our list for the item
	TextureInfo* pcNew = 0;
	for (std::vector<TextureInfo>::iterator
		i =  m_asTextures.begin();
		i != m_asTextures.end();++i)
	{
		if (hItem == (*i).hTreeItem)	{
			pcNew = &(*i);
			break;
		}
	}
/*	if (pcNew)
	{
		HMENU hMenu = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_TXPOPUP));
		hDisplay = GetSubMenu(hMenu,0);
	}
*/
	// search in the material list for the item
	MaterialInfo* pcNew2 = 0;
	for (std::vector<MaterialInfo>::iterator
		i =  m_asMaterials.begin();
		i != m_asMaterials.end();++i)
	{
		if (hItem == (*i).hTreeItem)	{
			pcNew2 = &(*i);
			break;
		}
	}
#if 0
	if (pcNew2)
	{
		HMENU hMenu = LoadMenu(m_hInstance,MAKEINTRESOURCE(IDR_MATPOPUP));
		hDisplay = GetSubMenu(hMenu,0);
	}
	if (0 != hDisplay)
	{
		// select this entry (this should all OnSetup())
		TreeView_Select(GetDlgItem(mr->m_hDlg,IDC_TREE1),hItem,TVGN_CARET);

		// FIX: Render the scene once that the correct texture/material
		// is displayed while the context menu is active
		OnRender();

		POINT sPoint;
		GetCursorPos(&sPoint);
		TrackPopupMenu(hDisplay, TPM_LEFTALIGN, sPoint.x, sPoint.y, 0,
			mr->m_hDlg,0);
	}
#endif
	return 1;
}
#endif
#if 0
//-------------------------------------------------------------------------------
int Cisplay::HandleTreeViewPopup(WPARAM wParam,LPARAM lParam)
{
	
	// get the current selected material
	std::vector<Info> apclrOut;
	const char* szMatKey;

	switch (LOWORD(wParam))
	{
	case ID_SOLONG_CLEARDIFFUSECOLOR:
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
		{
			if (this->m_pcCurrentMaterial->iIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			{
				apclrOut.push_back( Info( &mr->GetAsset()->apcMeshes[i]->vDiffuseColor,
					mr->GetAsset()->apcMeshes[i],"DIFFUSE_COLOR"));
			}
		}
		szMatKey = "$clr.diffuse";
		break;
	case ID_SOLONG_CLEARSPECULARCOLOR:
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
		{
			if (this->m_pcCurrentMaterial->iIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			{
				apclrOut.push_back( Info( &mr->GetAsset()->apcMeshes[i]->vSpecularColor,
					mr->GetAsset()->apcMeshes[i],"SPECULAR_COLOR"));
			}
		}
		szMatKey = "$clr.specular";
		break;
	case ID_SOLONG_CLEARAMBIENTCOLOR:
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
		{
			if (this->m_pcCurrentMaterial->iIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			{
				apclrOut.push_back( Info( &mr->GetAsset()->apcMeshes[i]->vAmbientColor,
					mr->GetAsset()->apcMeshes[i],"AMBIENT_COLOR"));
			}
		}
		szMatKey = "$clr.ambient";
		break;
	case ID_SOLONG_CLEAREMISSIVECOLOR:
		for (unsigned int i = 0; i < mr->GetAsset()->pcScene->mNumMeshes;++i)
		{
			if (this->m_pcCurrentMaterial->iIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			{
				apclrOut.push_back( Info( &mr->GetAsset()->apcMeshes[i]->vEmissiveColor,
					mr->GetAsset()->apcMeshes[i],"EMISSIVE_COLOR"));
			}
		}
		szMatKey = "$clr.emissive";
		break;
	default:

		// let the next function do this ... no spaghetti code ;-)
		//HandleTreeViewPopup2(wParam,lParam);
	};
	if (!apclrOut.empty())
	{
		aiColor4D clrOld = *((aiColor4D*)(apclrOut.front().pclrColor));

		CHOOSECOLOR clr;
		clr.lStructSize = sizeof(CHOOSECOLOR);
		clr.hwndOwner = mr->m_hDlg;
		clr.Flags = CC_RGBINIT | CC_FULLOPEN;
		clr.rgbResult = RGB(
			clamp<unsigned char>(clrOld.r * 255.0f),
			clamp<unsigned char>(clrOld.g * 255.0f),
			clamp<unsigned char>(clrOld.b * 255.0f));
		clr.lpCustColors = mr->m_aclCustomColors;
		clr.lpfnHook = 0;
		clr.lpTemplateName = 0;
		clr.lCustData = 0;

		ChooseColor(&clr);

		clrOld.r = (float)(((unsigned int)clr.rgbResult)       & 0xFF) / 255.0f;
		clrOld.g = (float)(((unsigned int)clr.rgbResult >> 8)  & 0xFF) / 255.0f;
		clrOld.b = (float)(((unsigned int)clr.rgbResult >> 16) & 0xFF) / 255.0f;

		// update the color values in the mesh instances and
		// update all shaders ...
		for (std::vector<Info>::iterator
			i =  apclrOut.begin();
			i != apclrOut.end();++i)
		{
			*((*i).pclrColor) = *((D3DXFROMWINEVECTOR4*)&clrOld);
			if (!(*i).pMesh->bSharedFX)
			{
					h=(*i).pMesh->piEffect->GetParameterByName(0,(*i).szShaderParam);
				(*i).pMesh->piEffect->SetVector(					
					h,(*i).pclrColor);
			}
		}

		// change the material key ...
		Assimp::MaterialHelper* pcMat = (Assimp::MaterialHelper*)mr->GetAsset()->pcScene->mMaterials[
			this->m_pcCurrentMaterial->iIndex];
		pcMat->AddProperty<aiColor4D>(&clrOld,1,szMatKey,0,0);

		if (ID_SOLONG_CLEARSPECULARCOLOR == LOWORD(wParam) &&
			aiShadingMode_Gouraud == apclrOut.front().pMesh->eShadingMode)
		{
			//CLogDisplay::Instance(mr).AddEntry("[INFO] You have just changed the specular "
				"material color",D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0));
			//CLogDisplay::Instance(mr).AddEntry(
				"This is great, especially since there is currently no specular shading",
				D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0));
		}
	}
	return 1;
}
#endif

#ifndef _WIN32
#define CALLBACK
#define LPARAM int
#endif

//-------------------------------------------------------------------------------
int CALLBACK TreeViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (lParamSort == lParam1)return -1;
	if (lParamSort == lParam2)return 1;
	return 0;
}
#if 0
//-------------------------------------------------------------------------------
int CDisplay::HandleTreeViewPopup2(WPARAM wParam,LPARAM lParam)
{
	char szFileName[MAX_PATH];
	DWORD dwTemp = MAX_PATH;

	switch (LOWORD(wParam))
	{
	case ID_HEY_REPLACE:
		{
		// get a path to a new texture
		if(ERROR_SUCCESS != RegQueryValueEx(mr->m_hRegistry,"ReplaceTextureSrc",0,0,
			(BYTE*)szFileName,&dwTemp))
		{
			// Key was not found. Use C:
			stx_strlcpy(szFileName,"",1);
		}
		else
		{
			// need to remove the file name
			char* sz = stx_strrchr(szFileName,stx_getOsSeparator());
			if (!sz)sz = stx_strrchr(szFileName,stx_getOsSeparator());
			if (!sz)*sz = 0;
		}
		OPENFILENAME sFilename1 = {
			sizeof(OPENFILENAME),
			mr->m_hDlg,GetModule//Handle(0), 
			"Textures\0*.png;*.dds;*.tga;*.bmp;*.tif;*.ppm;*.ppx;*.jpg;*.jpeg;*.exr\0*.*\0", 
			0, 0, 1, 
			szFileName, MAX_PATH, 0, 0, 0, 
			"Replace this texture",
			OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, 
			0, 1, ".jpg", 0, 0, 0
		};
		if(GetOpenFileName(&sFilename1) == 0) return 0;

		// Now store the file in the registry
		RegSetValueExA(mr->m_hRegistry,"ReplaceTextureSrc",0,REG_SZ,(const BYTE*)szFileName,MAX_PATH);
		this->ReplaceCurrentTexture(szFileName);
		}
		return 1;


	case ID_HEY_EXPORT:
		{
		if(ERROR_SUCCESS != RegQueryValueEx(mr->m_hRegistry,"TextureExportDest",0,0,
			(BYTE*)szFileName,&dwTemp))
		{
			// Key was not found. Use C:
			stx_strlcpy(szFileName,"",1);
		}
		else
		{
			// need to remove the file name
			char* sz = stx_strrchr(szFileName,stx_getOsSeparator());
			if (!sz)sz = stx_strrchr(szFileName,stx_getOsSeparator());
			if (!sz)*sz = 0;
		}
		OPENFILENAME sFilename1 = {
			sizeof(OPENFILENAME),
			mr->m_hDlg,GetModule//Handle(0), 
			"Textures\0*.png;*.dds;*.bmp;*.tif;*.pfm;*.jpg;*.jpeg;*.hdr\0*.*\0", 0, 0, 1, 
			szFileName, MAX_PATH, 0, 0, 0, 
			"Export texture to file",
			OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, 
			0, 1, ".png", 0, 0, 0
		};
		if(GetSaveFileName(&sFilename1) == 0) return 0;

		// Now store the file in the registry
		RegSetValueExA(mr->m_hRegistry,"TextureExportDest",0,REG_SZ,(const BYTE*)szFileName,MAX_PATH);

		// determine the file format ...
		D3DXFROMWINEIMAGE_FILEFORMAT eFormat = D3DXFROMWINEIFF_PNG;
		const char* sz = stx_strrchr(szFileName,'.');
		if (sz)
		{
			++sz;
			if (0 == Assimp::ASSIMP_stricmp(sz,"pfm"))eFormat = D3DXFROMWINEIFF_PFM;
			else if (0 == Assimp::ASSIMP_stricmp(sz,"dds"))eFormat = D3DXFROMWINEIFF_DDS;
			else if (0 == Assimp::ASSIMP_stricmp(sz,"jpg"))eFormat = D3DXFROMWINEIFF_JPG;
			else if (0 == Assimp::ASSIMP_stricmp(sz,"jpeg"))eFormat = D3DXFROMWINEIFF_JPG;
			else if (0 == Assimp::ASSIMP_stricmp(sz,"hdr"))eFormat = D3DXFROMWINEIFF_HDR;
			else if (0 == Assimp::ASSIMP_stricmp(sz,"bmp"))eFormat = D3DXFROMWINEIFF_BMP;
		}

		// get a pointer to the first surface of the current texture
		IDirect3DSurface9* pi = 0;
		(*this->m_pcCurrentTexture->piTexture)->GetSurfaceLevel(0,&pi);
		if(!pi || FAILED(D3DXFROMWINESaveSurfaceToFile(szFileName,eFormat,pi,0,0)))
		{
			//CLogDisplay::Instance(mr).AddEntry("[ERROR] Unable to export texture",
				D3DCOLOR_ARGB(0xFF,0xFF,0,0));
		}
		else
		{
			//CLogDisplay::Instance(mr).AddEntry("[INFO] The texture has been exported",
				D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0));
		}
		if(pi)pi->Release();
		}
		return 1;

	case ID_HEY_REMOVE:
		{

		if(IDYES != MessageBox(mr->m_hDlg,"To recover the texture you need to reload the model. Do you wish to continue?",
			"Remove texture",MB_YESNO)) {
			return 1;
		}

		Assimp::MaterialHelper* pcMat = (Assimp::MaterialHelper*)mr->GetAsset()->pcScene->mMaterials[
			m_pcCurrentTexture->iMatIndex];

		unsigned int s;
		if (m_pcCurrentTexture->iType == (aiTextureType_OPACITY | 0x40000000))
		{
			// set a special property to indicate that no alpha channel is required
			int iVal = 1;
			pcMat->AddProperty<int>(&iVal,1,"no_a_from_d",0,0);
			s = aiTextureType_OPACITY;
		}
		else s = m_pcCurrentTexture->iType;
		pcMat->RemoveProperty(AI_MATKEY_TEXTURE(m_pcCurrentTexture->iType,0));

		// need to update all meshes associated with this material
		for (unsigned int i = 0;i < mr->GetAsset()->pcScene->mNumMeshes;++i)
		{
			if (m_pcCurrentTexture->iMatIndex == mr->GetAsset()->pcScene->mMeshes[i]->mMaterialIndex)
			{
				CMaterialManager::Instance(mr).DeleteMaterial(mr->GetAsset()->apcMeshes[i]);
				CMaterialManager::Instance(mr).CreateMaterial(mr->GetAsset()->apcMeshes[i],mr->GetAsset()->pcScene->mMeshes[i]);
			}
		}
		// find the corresponding MaterialInfo structure
		const unsigned int iMatIndex = m_pcCurrentTexture->iMatIndex;
		for (std::vector<MaterialInfo>::iterator
			a =  m_asMaterials.begin();
			a != m_asMaterials.end();++a)
		{
			if (iMatIndex == (*a).iIndex)
			{
				// good news. we will also need to find all other textures
				// associated with this item ...
				for (std::vector<TextureInfo>::iterator
					n =  m_asTextures.begin();
					n != m_asTextures.end();++n)
				{
					if ((*n).iMatIndex == iMatIndex)
					{
						n =  m_asTextures.erase(n);
						if (m_asTextures.end() == n)break;
					}
				}
				// delete this material from all lists ...
				TreeView_DeleteItem(GetDlgItem(mr->m_hDlg,IDC_TREE1),(*a).hTreeItem);
				this->m_asMaterials.erase(a);
				break;
			}
		}

		// add the new material to the list and make sure it will be fully expanded
		AddMaterialToDisplayList(m_hRoot,iMatIndex);
		HTREEITEM hNewItem = m_asMaterials.back().hTreeItem;
		TreeView_Expand(GetDlgItem(mr->m_hDlg,IDC_TREE1),hNewItem,TVE_EXPAND);

		// we need to sort the list, materials come first, then nodes
		TVSORTCB sSort;
		sSort.hParent = m_hRoot;
		sSort.lParam = 10;
		sSort.lpfnCompare = &TreeViewCompareFunc;
		TreeView_SortChildrenCB(GetDlgItem(mr->m_hDlg,IDC_TREE1),&sSort,0);

		// the texture was selected, but the silly user has just deleted it
		// ... go back to normal viewing mode
		TreeView_Select(GetDlgItem(mr->m_hDlg,IDC_TREE1),m_hRoot,TVGN_CARET);
		return 1;
		}
	}
	return 0;
}
#endif
//-------------------------------------------------------------------------------
// Setup stereo view
int CDisplay::SetupStereoView()
{
	if (0 != mr->GetAsset() && 0 != mr->GetAsset()->pcScene->mRootNode)
	{
/*???		// enable the RED, GREEN and ALPHA channels
		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_ALPHA |
			D3DCOLORWRITEENABLE_GREEN);
*/
		// move the camera a little bit to the left
		mr->m_sCamera.vPos -= mr->m_sCamera.vRight * 0.03f;
	}
	return 1;
}
//-------------------------------------------------------------------------------
// Do the actual rendering pass for the stereo view
int CDisplay::RenderStereoView(const aiMatrix4x4& m)
{
	// and rerender the scene
	if (0 != mr->GetAsset() && 0 != mr->GetAsset()->pcScene->mRootNode)
	{
/*???		// enable the BLUE, GREEN and ALPHA channels
		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_ALPHA |
			D3DCOLORWRITEENABLE_BLUE);
*/
		// clear the z-buffer
	//	IRenderer::GetRendererInstance()->Clear(true,true,//false,true,
	//		float4(1,0,1,1));

		// move the camera a little bit to the right
		mr->m_sCamera.vPos += mr->m_sCamera.vRight * 0.06f;

		RenderNode(mr->GetAsset()->pcScene->mRootNode,m,false);
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
//???		IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthWrite());
		RenderNode(mr->GetAsset()->pcScene->mRootNode,m,true);
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

		// (move back to the original position)
		mr->m_sCamera.vPos -= mr->m_sCamera.vRight * 0.03f;

		// reenable all channels
/*???		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_ALPHA |
			D3DCOLORWRITEENABLE_BLUE);*/
	}
	return 1;
}
#if 0
//-------------------------------------------------------------------------------
// Process input for the texture view
int CDisplay::HandleInputTextureView()
{
	//HandleMouseInputTextureView();
	//HandleKeyboardInputTextureView();
	return 1;
}
//-------------------------------------------------------------------------------
// Get input for the current state
int CDisplay::HandleInput()
{
	if(CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance(mr).GetMode())
		//HandleMouseInputSkyBox();

	// handle input commands
	//HandleMouseInputLightRotate();
	//HandleMouseInputLightIntensityAndColor();
	if(mr->m_bFPSView)
	{
		//HandleMouseInputFPS();
		//HandleKeyboardInputFPS();
	}
	else //HandleMouseInputLocal();

	// compute auto rotation depending on the time which has passed
	if (mr->m_sOptions.bRotate)
	{
		aiMatrix4x4 mMat;
		D3DXFROMWINEMatrixRotationYawPitchRoll((D3DXFROMWINEMATRIX*)&mMat,
			mr->m_vRotateSpeed.x * mr->m_fElpasedTime,
			mr->m_vRotateSpeed.y * mr->m_fElpasedTime,
			mr->m_vRotateSpeed.z * mr->m_fElpasedTime);
		mr->m_mWorldRotate = mr->m_mWorldRotate * mMat;
	}

	// //Handle rotations of light source(s)
	if (mr->m_sOptions.bLightRotate)
	{
		aiMatrix4x4 mMat;
		D3DXFROMWINEMatrixRotationYawPitchRoll((D3DXFROMWINEMATRIX*)&mMat,
			mr->m_vRotateSpeed.x * mr->m_fElpasedTime * 0.5f,
			mr->m_vRotateSpeed.y * mr->m_fElpasedTime * 0.5f,
			mr->m_vRotateSpeed.z * mr->m_fElpasedTime * 0.5f);

		D3DXFROMWINEVec3TransformNormal((D3DXFROMWINEVECTOR3*)&mr->m_avLightDirs[0],
			(D3DXFROMWINEVECTOR3*)&mr->m_avLightDirs[0],(D3DXFROMWINEMATRIX*)&mMat);

		// 2 lights to rotate?
		if (mr->m_sOptions.b3Lights)
		{
			D3DXFROMWINEVec3TransformNormal((D3DXFROMWINEVECTOR3*)&mr->m_avLightDirs[1],
				(D3DXFROMWINEVECTOR3*)&mr->m_avLightDirs[1],(D3DXFROMWINEMATRIX*)&mMat);

			mr->m_avLightDirs[1].Normalize();
		}
		mr->m_avLightDirs[0].Normalize();
	}
	return 1;
}
//-------------------------------------------------------------------------------
// Process input for an empty scen view to allow for skybox rotations
int CDisplay::HandleInputEmptyScene()
{
	if(CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance(mr).GetMode())
	{
		if (mr->m_bFPSView)
		{
			//HandleMouseInputFPS();
			//HandleKeyboardInputFPS();
		}
		//HandleMouseInputSkyBox();

		// need to store the last mouse position in the global variable
		// //HandleMouseInputFPS() is doing this internally
		if (!mr->m_bFPSView)
		{
			mr->m_LastmousePos.x = mr->m_mousePos.x;
			mr->m_LastmousePos.y = mr->m_mousePos.y;
		}
	}
	return 1;
}
#endif

//-------------------------------------------------------------------------------
// Draw the HUD on top of the scene
int CDisplay::DrawHUD()
{
#if 0
  // HACK: (thom) can't get the effect to work on non-shader cards, therefore deactivated for the moment
  if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
    return 1;

  IRenderer::GetRendererInstance()->setShader(mr->m_piPassThroughEffect);
	// get the dimension of the back buffer
	RECT sRect;
	GetWindowRect(GetDlgItem(mr->m_hDlg,IDC_RT),&sRect);
	sRect.right -= sRect.left;
	sRect.bottom -= sRect.top;

	// commit the texture to the shader
	// FIX: Necessary because the texture view is also using this shader
	//mr->m_piPassThroughEffect
		IRenderer::GetRendererInstance()->SetTexture("TEXTURE_2D",mr->m_pcTexture);
#if 0
	// NOTE: The shader might be used for other purposes, too.
	// So ensure the right technique is there
	if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
PassThrough_FFh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThrough_FF");
		mr->m_piPassThroughEffect->SetTechnique( PassThrough_FFh);
	}
	else
	{
PassThroughh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThrough");
		mr->m_piPassThroughEffect->SetTechnique(PassThroughh);
	}
#endif
	SURFACE_DESC sDesc;
	IRenderer::GetRendererInstance()->GetLevelDesc(mr->m_pcTexture,0,&sDesc);
	SVertex as[4];
	float fHalfX = ((float)sRect.right-(float)sDesc.Width) / 2.0f;
	float fHalfY = ((float)sRect.bottom-(float)sDesc.Height) / 2.0f;
	as[1].x = fHalfX;
	as[1].y = fHalfY;
	as[1].z = 0.2f;
	as[1].w = 1.0f;
	as[1].u = 0.0f;
	as[1].v = 0.0f;

	as[3].x = (float)sRect.right-fHalfX;
	as[3].y = fHalfY;
	as[3].z = 0.2f;
	as[3].w = 1.0f;
	as[3].u = 1.0f;
	as[3].v = 0.0f;

	as[0].x = fHalfX;
	as[0].y = (float)sRect.bottom-fHalfY;
	as[0].z = 0.2f;
	as[0].w = 1.0f;
	as[0].u = 0.0f;
	as[0].v = 1.0f;

	as[2].x = (float)sRect.right-fHalfX;
	as[2].y = (float)sRect.bottom-fHalfY;
	as[2].z = 0.2f;
	as[2].w = 1.0f;
	as[2].u = 1.0f;
	as[2].v = 1.0f;

	as[0].x -= 0.5f;as[1].x -= 0.5f;as[2].x -= 0.5f;as[3].x -= 0.5f;
	as[0].y -= 0.5f;as[1].y -= 0.5f;as[2].y -= 0.5f;as[3].y -= 0.5f;

	// draw the screen-filling squad
//	DWORD dw2;IRenderer::GetRendererInstance()->GetFVF(&dw2);
	IRenderer::GetRendererInstance()->setVertexFormat(D3DFVF_XYZRHW | D3DFVF_TEX1);
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLES,2,
		&as,&as,sizeof(SVertex));
#endif
//	IRenderer::GetRendererInstance()->setVertexFormat(dw2);
	return 1;
}
//-------------------------------------------------------------------------------
// Render the full scene, all nodes
int CDisplay::RenderFullScene(D3DXFROMWINEMATRIX& w)
{
	
	// reset the color index used for drawing normals
	mr->m_iCurrentColor = 0;

	aiMatrix4x4 pcProj;
	mr->GetProjectionMatrix(pcProj);

	vPos = mr->GetCameraMatrix(mViewProjection);
	mViewProjection = mViewProjection * pcProj;
#if 0
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
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullFront());//Back());//Front???
	}
	else 
	{
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetcullNone());
	}
#endif
#if 0
	// for high-quality mode, enable anisotropic texture filtering
	if (mr->m_sOptions.bLowQuality) {
		for (DWORD d = 0; d < 8;++d) {
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		}
	}
	else {
		for (DWORD d = 0; d < 8;++d) {
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

			IRenderer::GetRendererInstance()->SetSamplerState(d,D3DSAMP_MAXANISOTROPY,mr->m_sCaps.MaxAnisotropy);
		}
	}
#else
	//IRenderer::GetRendererInstance()->Getlinear()
#endif
	// draw the scene background (clear and texture 2d)
//	CBackgroundPainter::Instance(mr).OnPreRender();

	// setup the stereo view if necessary
	if (mr->m_sOptions.bStereoView)
		SetupStereoView();
	

	// draw all opaque objects in the scene
	aiMatrix4x4 m=aiMatrix4x4();
	// compute auto rotation depending on the time which has passed
	if (mr->m_sOptions.bRotate)
	{
		aiMatrix4x4 mMatR;
aiMatrix4x4 mMatS;
aiMatrix4x4 mMatT;
#if 0
#if 0
		static float Time=timeGetTime()/1000.0f;
		mr->m_fElpasedTime=timeGetTime()/1000.0f-Time;
		D3DXFROMWINEMatrixRotationYawPitchRoll((D3DXFROMWINEMATRIX*)&mMat,
			mr->m_vRotateSpeed.x * mr->m_fElpasedTime,
			mr->m_vRotateSpeed.y * mr->m_fElpasedTime,
			mr->m_vRotateSpeed.z * mr->m_fElpasedTime);
#else
		D3DXFROMWINEMatrixRotationYawPitchRoll((D3DXFROMWINEMATRIX*)&mMatR,
			g_pApp->m_fSpinX_L,
			g_pApp->m_fSpinY_L,
			0.0f);
#endif
D3DXFROMWINEMatrixScaling((D3DXFROMWINEMATRIX*)&mMatS,0.5f,0.5f,0.5f);
D3DXFROMWINEMatrixTranslation((D3DXFROMWINEMATRIX*)&mMatT,0.0f,0.0f,
g_pApp->m_fZ+10.0f);
#elif 0
		D3DXFROMWINEMatrixRotationYawPitchRoll((D3DXFROMWINEMATRIX*)&mMatR,
			mr->m_fSpinX,
			mr->m_fSpinY,
			mr->m_fSpinZ);
D3DXFROMWINEMatrixScaling((D3DXFROMWINEMATRIX*)&mMatS,mr->m_fSX,mr->m_fSy,mr->m_fSZ);
D3DXFROMWINEMatrixTranslation((D3DXFROMWINEMATRIX*)&mMatT,mr->m_fTX,mr->m_fTY,mr->m_fTZ);
#endif
mr->m_mWorldRotate = /*mr->m_mWorldRotate **/ *(aiMatrix4x4*)&w;//mMatS * mMatR* mMatT;
/*	
		static aiMatrix4x4 mMat0=aiMatrix4x4();
		aiMatrix4x4 mMat;
		D3DXFROMWINEMatrixTranslation((D3DXFROMWINEMATRIX*)&mMat,
			mr->m_vTransSpeed.x * mr->m_fElpasedTime,
			mr->m_vTransSpeed.y * mr->m_fElpasedTime,
			mr->m_vTransSpeed.z * mr->m_fElpasedTime);

*/
	}
		m =  mr->m_mWorld * mr->m_mWorldRotate;

	if (0)//???0 != mr->GetAsset() && 0 != mr->GetAsset()->pcScene->mRootNode)
	{
		//HandleInput();


		RenderNode(mr->GetAsset()->pcScene->mRootNode,m,false);
	}

	// if a cube texture is loaded as background image, the user
	// should be able to rotate it even if no asset is loaded
	//HandleInputEmptyScene();

	// draw the scene background
//	CBackgroundPainter::Instance(mr).OnPostRender();

	// draw all non-opaque objects in the scene
	if (0 != mr->GetAsset() && 0 != mr->GetAsset()->pcScene->mRootNode)
	{
		// disable the z-buffer
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
//???		IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthWrite());
		RenderNode(mr->GetAsset()->pcScene->mRootNode,m,true);
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	}

	// setup the stereo view if necessary
	if (mr->m_sOptions.bStereoView) 
		RenderStereoView(m);

	// render the skeleton if necessary
	if (mr->m_sOptions.bSkeleton && 0 != mr->GetAsset() && 0 != mr->GetAsset()->pcScene->mRootNode) {
		//ShaderID piEnd = mr->m_piNormalsEffect;
		ShaderID shd = MeshRendererShadersFactory::GetShader("NormalsEffect", "main", "main");
		IRenderer::GetRendererInstance()->setShader(shd);

		// disable the z-buffer
//		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
//???			IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthWrite());

		if (mr->m_sOptions.eDrawMode != RenderOptions::WIREFRAME) {
		//	IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZENABLE,FALSE);
//???			IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
		}

		//IRenderer::GetRendererInstance()->setVertexFormat( mr->m_DefaultVertexDecl);

 FormatDesc Fmt[] = {
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
    };
		VertexFormatID vf=MeshRendererShadersFactory::GetVertexFormat(mr->m_szShaderName.c_str(), shd);
		IRenderer::GetRendererInstance()->setVertexFormat( vf);

		// this is very similar to the code in SetupMaterial()
		aiMatrix4x4 pcProj = m * mViewProjection;

		D3DXFROMWINEVECTOR4 vVector(1.f,0.f,0.f,1.f);
		IRenderer::GetRendererInstance()->SetVector("OUTPUT_COLOR",&vVector);
		IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection", (const D3DXFROMWINEMATRIX*)&pcProj);

		RenderSkeleton(mr->GetAsset()->pcScene->mRootNode,m,m);

		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		//IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ZENABLE,TRUE);

		
	}

	// draw the HUD texture on top of the rendered scene using
	// pre-projected vertices
	//if (!mr->m_bFPSView && mr->GetAsset() && mr->m_pcTexture)
	//	DrawHUD();
	
	return 1;
}
//-------------------------------------------------------------------------------
int CDisplay::RenderMaterialView(D3DXFROMWINEMATRIX& w)
{
	return 1;
}
//-------------------------------------------------------------------------------
// Render animation skeleton
int CDisplay::RenderSkeleton (aiNode* piNode,const aiMatrix4x4& piMatrix, const aiMatrix4x4& parent)
{
	aiMatrix4x4 me = mr->GetAsset()->mAnimator->GetGlobalTransform( piNode);

	me.Transpose();
	//me *= piMatrix;

	if (piNode->mParent) {
		AssetHelper::LineVertex data[2];
		data[0].dColorDiffuse = data[1].dColorDiffuse = D3DCOLOR_ARGB(0xff,0xff,0,0);
		
		data[0].vPosition.x = parent.d1;
		data[0].vPosition.y = parent.d2;
		data[0].vPosition.z = parent.d3;

		data[1].vPosition.x = me.d1;
		data[1].vPosition.y = me.d2;
		data[1].vPosition.z = me.d3;
static VertexBufferID vb=-1;
		IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_LINES,1,&data,&data,sizeof(AssetHelper::LineVertex),vb);
	}

	// render all child nodes
	for (unsigned int i = 0; i < piNode->mNumChildren;++i)
		RenderSkeleton(piNode->mChildren[i],piMatrix, me );

	return 1;
}
//-------------------------------------------------------------------------------
// Render a single node
int CDisplay::RenderNode (aiNode* piNode,const aiMatrix4x4& piMatrix,
	bool bAlpha /*= false*/)
{
	
	aiMatrix4x4 aiMe = mr->GetAsset()->mAnimator->GetGlobalTransform( piNode);

	aiMe.Transpose();
	aiMe *= piMatrix;

	bool bChangedVM = false;
	if (VIEWMODE_NODE == m_iViewMode && m_pcCurrentNode)
	{
		if (piNode != m_pcCurrentNode->psNode)
		{
			// directly call our children
			for (unsigned int i = 0; i < piNode->mNumChildren;++i)
				RenderNode(piNode->mChildren[i],piMatrix,bAlpha );
			
			return 1;
		}
		m_iViewMode = VIEWMODE_FULL;
		bChangedVM = true;
	}

	aiMatrix4x4 pcProj = aiMe * mViewProjection;

	aiMatrix4x4 pcCam = aiMe;
	pcCam.Inverse().Transpose();
#if 0
	//if (mr->m_sOptions.bRenderMats) LOG_PRINT("M\n");
	//if (bAlpha) LOG_PRINT("A\n");
	// VERY UNOPTIMIZED, much stuff is redundant. Who cares?
	
	if (0)//???(!mr->m_sOptions.bRenderMats && !bAlpha)
	{
		// this is very similar to the code in SetupMaterial()
		ShaderID piEnd = mr->m_piDefaultEffect;
		IRenderer::GetRendererInstance()->setShader(piEnd);
		// commit transformation matrices to the shader
		IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection",
			(const D3DXFROMWINEMATRIX*)&pcProj);
		IRenderer::GetRendererInstance()->SetMatrix("World",(const D3DXFROMWINEMATRIX*)&aiMe);
		IRenderer::GetRendererInstance()->SetMatrix("WorldInverseTranspose",
			(const D3DXFROMWINEMATRIX*)&pcCam);
/*
		if ( CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance(mr).GetMode())
		{
			pcCam = pcCam * pcProj;
			IRenderer::GetRendererInstance()->SetMatrix("ViewProj",(const D3DXFROMWINEMATRIX*)&pcCam);
			pcCam.Inverse();
			IRenderer::GetRendererInstance()->SetMatrix("InvViewProj",(const D3DXFROMWINEMATRIX*)&pcCam);
		}
*/
		// commit light colors and direction to the shader
		D3DXFROMWINEVECTOR4 apcVec[5];
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
		IRenderer::GetRendererInstance()->SetVectorArray("afLightDir",apcVec,5);

		apcVec[0].x = ((mr->m_avLightColors[0] >> 16) & 0xFF) / 255.0f;
		apcVec[0].y = ((mr->m_avLightColors[0] >> 8) & 0xFF) / 255.0f;
		apcVec[0].z = ((mr->m_avLightColors[0]) & 0xFF) / 255.0f;
		apcVec[0].w = 1.0f;

		if( mr->m_sOptions.b3Lights)
		{
			apcVec[1].x = ((mr->m_avLightColors[1] >> 16) & 0xFF) / 255.0f;
			apcVec[1].y = ((mr->m_avLightColors[1] >> 8) & 0xFF) / 255.0f;
			apcVec[1].z = ((mr->m_avLightColors[1]) & 0xFF) / 255.0f;
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
		IRenderer::GetRendererInstance()->SetVectorArray("afLightColor",apcVec,5);

		apcVec[0].x = vPos.x;
		apcVec[0].y = vPos.y;
		apcVec[0].z = vPos.z;
		IRenderer::GetRendererInstance()->SetVector( "vCameraPos",&apcVec[0]);
#if 0
		// setup the best technique 
		if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
		{
			DefaultFXSpecular_FFh=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_FF");
				mr->m_piDefaultEffect->SetTechnique( DefaultFXSpecular_FFh);
		} else
		if (mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(3,0) || mr->m_sOptions.bLowQuality)
		{
			if (mr->m_sOptions.b3Lights)
			{
				DefaultFXSpecular_PS20_D2h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_PS20_D2");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_PS20_D2h);
			}
			else 
			{
				DefaultFXSpecular_PS20_D1h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_PS20_D1");
					IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_PS20_D1h);
			}
		}
		else
		{
			if (mr->m_sOptions.b3Lights)
			{
				DefaultFXSpecular_D2h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_D2");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_D2h);
			}
			else {
				DefaultFXSpecular_D1h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_D1");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_D1h);
			}
		}
#else
//		IRenderer::GetRendererInstance()->setShader(mr->m_piDefaultEffect);
#endif
	}
#endif
D3DXFROMWINEVECTOR4 m_aclNormalColors[14] = 
{
	D3DXFROMWINEVECTOR4(0xFF / 255.0f,0xFF / 255.0f,0xFF / 255.0f, 1.0f), // white

	D3DXFROMWINEVECTOR4(0xFF / 255.0f,0x00 / 255.0f,0x00 / 255.0f,1.0f), // red
	D3DXFROMWINEVECTOR4(0x00 / 255.0f,0xFF / 255.0f,0x00 / 255.0f,1.0f), // green
	D3DXFROMWINEVECTOR4(0x00 / 255.0f,0x00 / 255.0f,0xFF / 255.0f,1.0f), // blue

	D3DXFROMWINEVECTOR4(0xFF / 255.0f,0xFF / 255.0f,0x00 / 255.0f,1.0f), // yellow
	D3DXFROMWINEVECTOR4(0xFF / 255.0f,0x00 / 255.0f,0xFF / 255.0f,1.0f), // magenta
	D3DXFROMWINEVECTOR4(0x00 / 255.0f,0xFF / 255.0f,0xFF / 255.0f,1.0f), // wtf

	D3DXFROMWINEVECTOR4(0xFF / 255.0f,0x60 / 255.0f,0x60 / 255.0f,1.0f), // light red
	D3DXFROMWINEVECTOR4(0x60 / 255.0f,0xFF / 255.0f,0x60 / 255.0f,1.0f), // light green
	D3DXFROMWINEVECTOR4(0x60 / 255.0f,0x60 / 255.0f,0xFF / 255.0f,1.0f), // light blue

	D3DXFROMWINEVECTOR4(0xA0 / 255.0f,0x00 / 255.0f,0x00 / 255.0f,1.0f), // dark red
	D3DXFROMWINEVECTOR4(0x00 / 255.0f,0xA0 / 255.0f,0x00 / 255.0f,1.0f), // dark green
	D3DXFROMWINEVECTOR4(0x00 / 255.0f,0x00 / 255.0f,0xA0 / 255.0f,1.0f), // dark blue

	D3DXFROMWINEVECTOR4(0x88 / 255.0f,0x88 / 255.0f,0x88 / 255.0f, 1.0f) // gray
};

	D3DXFROMWINEVECTOR4 vVector = m_aclNormalColors[mr->m_iCurrentColor];
	if (++mr->m_iCurrentColor == 14)
	{
		mr->m_iCurrentColor = 0;
	}
	if (! (!mr->m_sOptions.bRenderMats && bAlpha  ))
	{
		for (unsigned int i = 0; i < piNode->mNumMeshes;++i)
		{
			const aiMesh* mesh = mr->GetAsset()->pcScene->mMeshes[piNode->mMeshes[i]];
			AssetHelper::MeshHelper* helper = mr->GetAsset()->apcMeshes[piNode->mMeshes[i]];

			// don't render the mesh if the render pass is incorrect
			if (mr->m_sOptions.bRenderMats && (helper->piOpacityTexture || helper->fOpacity != 1.0f) && !mesh->HasBones())
			{
				if (!bAlpha)continue;
			}
			else if (bAlpha)continue;

			// Upload bone matrices. This maybe is the wrong place to do it, but for the heck of it I don't understand this code flow
			if( mesh->HasBones())
			{
				if( helper->piEffect!=-1)
				{
					static float matrices[4*4*60];
					float* tempmat = matrices;
					const std::vector<aiMatrix4x4>& boneMats = mr->GetAsset()->mAnimator->GetBoneMatrices( piNode, i);
					ai_assert( boneMats.size() == mesh->mNumBones);

					for( unsigned int a = 0; a < mesh->mNumBones; a++)
					{
						const aiMatrix4x4& mat = boneMats[a];
						*tempmat++ = mat.a1; *tempmat++ = mat.a2; *tempmat++ = mat.a3; *tempmat++ = mat.a4;  
						*tempmat++ = mat.b1; *tempmat++ = mat.b2; *tempmat++ = mat.b3; *tempmat++ = mat.b4;
						*tempmat++ = mat.c1; *tempmat++ = mat.c2; *tempmat++ = mat.c3; *tempmat++ = mat.c4; 
						*tempmat++ = mat.d1; *tempmat++ = mat.d2; *tempmat++ = mat.d3; *tempmat++ = mat.d4; 
						//tempmat += 4;
					}

					if( mr->m_sOptions.bRenderMats)
					{
						//helper->piEffect
							IRenderer::GetRendererInstance()->SetMatrixTransposeArray( "gBoneMatrix", (D3DXFROMWINEMATRIX*)matrices, 60);
					} else
					{
						//mr->m_piDefaultEffect
							IRenderer::GetRendererInstance()->SetMatrixTransposeArray( "gBoneMatrix", (D3DXFROMWINEMATRIX*)matrices, 60);
					}
				}
			} else
			{
				// upload identity matrices instead. Only the first is ever going to be used in meshes without bones
				if( !mr->m_sOptions.bRenderMats)
				{
					D3DXFROMWINEMATRIX identity( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
					IRenderer::GetRendererInstance()->SetMatrixTransposeArray( "gBoneMatrix", &identity, 1);
				}
			}

			// now setup the material
			if (mr->m_sOptions.bRenderMats)
			{
				CMaterialManager::Instance(mr).SetupMaterial( helper, pcProj, aiMe, pcCam, vPos);
			}
#if 1
	ShaderID shd = -1;
	//if (mr->m_sOptions.bRenderMats) LOG_PRINT("M\n");
	//if (bAlpha) LOG_PRINT("A\n");
	// VERY UNOPTIMIZED, much stuff is redundant. Who cares?
	
	if (!mr->m_sOptions.bRenderMats && !bAlpha)
	{
		// this is very similar to the code in SetupMaterial()
		//ShaderID piEnd = mr->m_piDefaultEffect;
		shd = MeshRendererShadersFactory::GetShader(mr->m_szShaderName.c_str(), "main", "main");

		IRenderer::GetRendererInstance()->setShader(shd);
		// commit transformation matrices to the shader
		IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection",
			(const D3DXFROMWINEMATRIX*)&pcProj);
		IRenderer::GetRendererInstance()->SetMatrix("World",(const D3DXFROMWINEMATRIX*)&aiMe);
		IRenderer::GetRendererInstance()->SetMatrix("WorldInverseTranspose",
			(const D3DXFROMWINEMATRIX*)&pcCam);
/*
		if ( CBackgroundPainter::TEXTURE_CUBE == CBackgroundPainter::Instance(mr).GetMode())
		{
			pcCam = pcCam * pcProj;
			IRenderer::GetRendererInstance()->SetMatrix("ViewProj",(const D3DXFROMWINEMATRIX*)&pcCam);
			pcCam.Inverse();
			IRenderer::GetRendererInstance()->SetMatrix("InvViewProj",(const D3DXFROMWINEMATRIX*)&pcCam);
		}
*/

		// commit light colors and direction to the shader
		D3DXFROMWINEVECTOR4 apcVec[5];
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
		//IRenderer::GetRendererInstance()->SetVectorArray("afLightDir",apcVec,5);
#if 0
		apcVec[0].x = ((mr->m_avLightColors[0] >> 16) & 0xFF) / 255.0f;
		apcVec[0].y = ((mr->m_avLightColors[0] >> 8) & 0xFF) / 255.0f;
		apcVec[0].z = ((mr->m_avLightColors[0]) & 0xFF) / 255.0f;
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

		apcVec[0].x = vPos.x;
		apcVec[0].y = vPos.y;
		apcVec[0].z = vPos.z;
		IRenderer::GetRendererInstance()->SetVector( "vCameraPos",&apcVec[0]);
#if 0
		// setup the best technique 
		if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
		{
			DefaultFXSpecular_FFh=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_FF");
				mr->m_piDefaultEffect->SetTechnique( DefaultFXSpecular_FFh);
		} else
		if (mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(3,0) || mr->m_sOptions.bLowQuality)
		{
			if (mr->m_sOptions.b3Lights)
			{
				DefaultFXSpecular_PS20_D2h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_PS20_D2");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_PS20_D2h);
			}
			else 
			{
				DefaultFXSpecular_PS20_D1h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_PS20_D1");
					IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_PS20_D1h);
			}
		}
		else
		{
			if (mr->m_sOptions.b3Lights)
			{
				DefaultFXSpecular_D2h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_D2");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_D2h);
			}
			else {
				DefaultFXSpecular_D1h=mr->m_piDefaultEffect->GetTechniqueByName("DefaultFXSpecular_D1");
				IRenderer::GetRendererInstance()->SetTechnique(DefaultFXSpecular_D1h);
			}
		}
#else
//		IRenderer::GetRendererInstance()->setShader(mr->m_piDefaultEffect);
#endif
	}
#endif

 FormatDesc Fmt[] = {
	{0, TYPE_VERTEX, FORMAT_FLOAT, 3},
	{0, TYPE_TEXCOORD, FORMAT_FLOAT, 2},
    };

	VertexFormatID vf=MeshRendererShadersFactory::GetVertexFormat(mr->m_szShaderName.c_str(), shd);
			IRenderer::GetRendererInstance()->setVertexFormat( vf);

			if (bAlpha)
				CMeshRenderer::Instance(mr).DrawSorted(piNode->mMeshes[i],aiMe);
				//CMeshRenderer::Instance(mr).DrawUnsorted(piNode->mMeshes[i]);
			else 
				CMeshRenderer::Instance(mr).DrawUnsorted(piNode->mMeshes[i]);
/*
			// now end the material
			if (mr->m_sOptions.bRenderMats)
			{
				CMaterialManager::Instance(mr).EndMaterial( helper);
			}
*/
			// render normal vectors?
			if (mr->m_sOptions.bRenderNormals && helper->piVBNormals)
			{
				// this is very similar to the code in SetupMaterial()
				//ShaderID piEnd = mr->m_piNormalsEffect;
				ShaderID shd = MeshRendererShadersFactory::GetShader("NormalsEffect", "main", "main");
				IRenderer::GetRendererInstance()->setShader(shd);

				IRenderer::GetRendererInstance()->SetVector("OUTPUT_COLOR",&vVector);
				IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection", (const D3DXFROMWINEMATRIX*)&pcProj);

				IRenderer::GetRendererInstance()->setVertexBuffer(0, helper->piVBNormals);//, 0, sizeof(AssetHelper::LineVertex));
				IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_LINES,0, mr->GetAsset()->pcScene->mMeshes[piNode->mMeshes[i]]->mNumVertices);

			}
		}
	}
	// render all child nodes
	for (unsigned int i = 0; i < piNode->mNumChildren;++i)
		RenderNode(piNode->mChildren[i],piMatrix,bAlpha );
	
	// need to reset the viewmode?
	if (bChangedVM)
		m_iViewMode = VIEWMODE_NODE;
	return 1;
}
//-------------------------------------------------------------------------------
int CDisplay::RenderPatternBG()
{
	
	if (0)//mr->m_piPatternEffect==-1)
	{
		// the pattern effect won't work on ps_2_0 cards
		/*if (mr->m_sCaps.PixelShaderVersion >= D3DPS_VERSION(3,0))
		{
			// seems we have not yet compiled this shader.
			// and NOW is the best time to do that ...
		mr->m_piCheckerBackgroundEffect=IRenderer::GetRendererInstance()->addHLSLShader(
		mr->m_szCheckerBackgroundShader.c_str(),"DefaultVShader","MakePattern_PS");
		}
		else*/
		{
			// clear the color buffer in magenta
			// (hopefully this is ugly enough that every ps_2_0 cards owner
			//  runs to the next shop to buy himself a new card ...)
		//	IRenderer::GetRendererInstance()->Clear(true,true,
		//		float4(0xFF,0xFF,0,0xFF));
			return 1;
		}
	}

	// clear the depth buffer only
	//IRenderer::GetRendererInstance()->Clear(true,true,//false,true,
	//	float4(0xFF,0xFF,0,0xFF));

	ShaderID shd=MeshRendererShadersFactory::GetShader("PatternEffect", "main", "main");
	IRenderer::GetRendererInstance()->setShader(shd);
	// setup the colors to be used ...
	IRenderer::GetRendererInstance()->SetVector("COLOR_ONE",&m_avCheckerColors[0]);
	IRenderer::GetRendererInstance()->SetVector("COLOR_TWO",&m_avCheckerColors[1]);
/*
	RECT sRect;
	GetWindowRect(GetDlgItem(mr->m_hDlg,IDC_RT),&sRect);
	sRect.right -= sRect.left;
	sRect.bottom -= sRect.top;
*/
	struct SVertex
	{
		float x,y,z,w;
	};
	// build the screen-filling rectangle
	SVertex as[4];
	as[1].x = 0.0f;
	as[1].y = 0.0f;
	as[1].z = 0.2f;
	as[3].x = (float)IRenderer::GetRendererInstance()->GetViewportWidth();
	as[3].y = 0.0f;
	as[3].z = 0.2f;
	as[0].x = 0.0f;
	as[0].y = (float)IRenderer::GetRendererInstance()->GetViewportHeight();
	as[0].z = 0.2f;
	as[2].x = (float)IRenderer::GetRendererInstance()->GetViewportWidth();
	as[2].y = (float)IRenderer::GetRendererInstance()->GetViewportHeight();
	as[2].z = 0.2f;

	as[0].w = 1.0f;
	as[1].w = 1.0f;
	as[2].w = 1.0f;
	as[3].w = 1.0f;
	
	as[0].x -= 0.5f;as[1].x -= 0.5f;as[2].x -= 0.5f;as[3].x -= 0.5f;
	as[0].y -= 0.5f;as[1].y -= 0.5f;as[2].y -= 0.5f;as[3].y -= 0.5f;

	// draw the rectangle
//	DWORD dw2;IRenderer::GetRendererInstance()->GetFVF(&dw2);
	IRenderer::GetRendererInstance()->setVertexFormat(D3DFVF_XYZRHW);
static VertexBufferID vb=-1;
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP,2,
		&as,&as,sizeof(SVertex),vb);
//	IRenderer::GetRendererInstance()->setVertexFormat(dw2);

	return 1;
}
int CDisplay::OnRender(void)
{
		D3DXFROMWINEMATRIX w;
		D3DXFROMWINEMatrixIdentity(&w);
		return OnRender(w);
}
//-------------------------------------------------------------------------------
int CDisplay::RenderTextureView(D3DXFROMWINEMATRIX& w)
{
	if (!mr->GetAsset() || !mr->GetAsset()->pcScene)return 0;

	// handle input
	this->//HandleInputTextureView();

	// render the background
	RenderPatternBG();

	// it might be that there is no texture ...
	if (!m_pcCurrentTexture->piTexture)
	{
		// FIX: no such log message. it would be repeated to often
		////CLogDisplay::Instance(mr).AddEntry("Unable to display texture. Image is unreachable.",
		//	D3DCOLOR_ARGB(0xFF,0xFF,0,0));
		return 0;
	}

	ShaderID shd=MeshRendererShadersFactory::GetShader("PassThroughEffect", "main", "main");
	IRenderer::GetRendererInstance()->setShader(shd);
/*
	RECT sRect;
	GetWindowRect(GetDlgItem(mr->m_hDlg,IDC_RT),&sRect);
	sRect.right -= sRect.left;
	sRect.bottom -= sRect.top;
*/
	// commit the texture to the shader
	//mr->m_piPassThroughEffect->
		IRenderer::GetRendererInstance()->SetTexture("TEXTURE_2D",*m_pcCurrentTexture->piTexture);
#if 0
	if (aiTextureType_OPACITY == m_pcCurrentTexture->iType)
	{
		PassThroughAlphaFromRh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThroughAlphaFromR");
		mr->m_piPassThroughEffect->SetTechnique(PassThroughAlphaFromRh);
	}
	else if ((aiTextureType_OPACITY | 0x40000000) == m_pcCurrentTexture->iType)
	{
		PassThroughAlphaFromAh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThroughAlphaFromA");
		mr->m_piPassThroughEffect->SetTechnique(PassThroughAlphaFromAh);
	}
	else if( mr->m_sCaps.PixelShaderVersion < D3DPS_VERSION(2,0))
	{
		PassThrough_FFh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThrough_FF");
		mr->m_piPassThroughEffect->SetTechnique( PassThrough_FFh);
	}
	else
	{
		PassThroughh=mr->m_piPassThroughEffect->GetTechniqueByName("PassThrough");
		mr->m_piPassThroughEffect->SetTechnique(PassThroughh);
	}
#endif
	if (aiTextureType_HEIGHT == m_pcCurrentTexture->iType ||
		aiTextureType_NORMALS == m_pcCurrentTexture->iType)
	{
		// manually disable alpha blending
//???		IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	}

	// build a rectangle which centers the texture
	// scaling is OK, but no stretching
	SURFACE_DESC sDesc;
	if ( m_pcCurrentTexture->piTexture && *m_pcCurrentTexture->piTexture) { /* just a dirty fix */
		
		IRenderer::GetRendererInstance()->GetLevelDesc((*m_pcCurrentTexture->piTexture),0,&sDesc);

		struct SVertex{float x,y,z,w,u,v;};
		SVertex as[4];

		const float nx = (float)IRenderer::GetRendererInstance()->GetViewportWidth();
		const float ny = (float)IRenderer::GetRendererInstance()->GetViewportHeight();
		const float  x = (float)0.000001;
		const float  y = (float)0.000001;
		float f = std::min((nx-30) / x,(ny-30) / y) * (m_fTextureZoom/1000.0f);

		float fHalfX = (nx - (f * x)) / 2.0f;
		float fHalfY = (ny - (f * y)) / 2.0f;
		as[1].x = fHalfX + m_vTextureOffset.x;
		as[1].y = fHalfY + m_vTextureOffset.y;
		as[1].z = 0.2f;
		as[1].w = 1.0f;
		as[1].u = 0.0f;
		as[1].v = 0.0f;
		as[3].x = nx-fHalfX + m_vTextureOffset.x;
		as[3].y = fHalfY + m_vTextureOffset.y;
		as[3].z = 0.2f;
		as[3].w = 1.0f;
		as[3].u = 1.0f;
		as[3].v = 0.0f;
		as[0].x = fHalfX + m_vTextureOffset.x;
		as[0].y = ny-fHalfY + m_vTextureOffset.y;
		as[0].z = 0.2f;
		as[0].w = 1.0f;
		as[0].u = 0.0f;
		as[0].v = 1.0f;
		as[2].x = nx-fHalfX + m_vTextureOffset.x;
		as[2].y = ny-fHalfY + m_vTextureOffset.y;
		as[2].z = 0.2f;
		as[2].w = 1.0f;
		as[2].u = 1.0f;
		as[2].v = 1.0f;
		as[0].x -= 0.5f;as[1].x -= 0.5f;as[2].x -= 0.5f;as[3].x -= 0.5f;
		as[0].y -= 0.5f;as[1].y -= 0.5f;as[2].y -= 0.5f;as[3].y -= 0.5f;

		// draw the rectangle
//		DWORD dw2;IRenderer::GetRendererInstance()->GetFVF(&dw2);
		IRenderer::GetRendererInstance()->setVertexFormat(D3DFVF_XYZRHW | D3DFVF_TEX1);
		IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP,2,
			&as,&as,sizeof(SVertex));
//		IRenderer::GetRendererInstance()->setVertexFormat(dw2);
	}

	// do we need to draw UV coordinates?
	return 1;
}
};
#endif

