//include <Framework3/IRenderer.h>

#ifdef BUILD_MESHRENDERER

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
#include <Framework3/IRenderer.h>

#include <assimp_view.h>

#include "stdafx.h"
#define __MESHRENDERER__ 1

#include <map>

namespace AssimpView {

MeshRenderer* CMeshRenderer::mr;
CMeshRenderer CMeshRenderer::s_cInstance;
//-------------------------------------------------------------------------------
int CMeshRenderer::DrawUnsorted(unsigned int iIndex)
{
	/*
	pcMesh:		AssetHelper::MeshHelper* helper = mr->GetAsset()->apcMeshes[piNode->mMeshes[i]];

int CMaterialManager::SetupMaterial (
	AssetHelper::MeshHelper* pcMesh,
	const aiMatrix4x4& pcProj,
	const aiMatrix4x4& aiMe,
	const aiMatrix4x4& pcCam,
	const aiVector3D& vPos)
{
	ShaderID piEnd = pcMesh->piEffect;

			IRenderer::GetRendererInstance()->setVertexFormat( m_DefaultVertexDecl);
	*/
	ai_assert(iIndex < mr->GetAsset()->pcScene->mNumMeshes);

	// set vertex and index buffer
	IRenderer::GetRendererInstance()->setVertexBuffer(0,mr->GetAsset()->apcMeshes[iIndex]->piVB);//,0,sizeof(AssetHelper::Vertex));

	IRenderer::GetRendererInstance()->setIndexBuffer(mr->GetAsset()->apcMeshes[iIndex]->piIB);

	Primitives type;
	switch (mr->GetAsset()->pcScene->mMeshes[iIndex]->mPrimitiveTypes) {
		case aiPrimitiveType_POINT:
			type = PRIM_POINTS;break;
		case aiPrimitiveType_LINE:
			type = PRIM_LINES;break;
		case aiPrimitiveType_TRIANGLE:
			type = PRIM_TRIANGLES;break;
	}
	// and draw the mesh
	IRenderer::GetRendererInstance()->DrawIndexedPrimitive(type,
		0,0,
		mr->GetAsset()->pcScene->mMeshes[iIndex]->mNumVertices,0,
		mr->GetAsset()->pcScene->mMeshes[iIndex]->mNumFaces);
/*
LOG_PRINT("piVB=%d\n",mr->GetAsset()->apcMeshes[iIndex]->piVB);
LOG_PRINT("piIB=%d\n",mr->GetAsset()->apcMeshes[iIndex]->piIB);
LOG_PRINT("mNumVertices=%d\n",mr->GetAsset()->pcScene->mMeshes[iIndex]->mNumVertices);
LOG_PRINT("mNumFaces=%d\n",mr->GetAsset()->pcScene->mMeshes[iIndex]->mNumFaces);
*/
return 1;
}
//-------------------------------------------------------------------------------
int CMeshRenderer::DrawSorted(unsigned int iIndex,const aiMatrix4x4& mWorld)
{
	ai_assert(iIndex < mr->GetAsset()->pcScene->mNumMeshes);

	AssetHelper::MeshHelper* pcHelper = mr->GetAsset()->apcMeshes[iIndex]; 
	const aiMesh* pcMesh = mr->GetAsset()->pcScene->mMeshes[iIndex];

	if (!pcHelper || !pcMesh || (pcHelper->piIB==-1))
		return -5;

	if (pcMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
		return DrawUnsorted(iIndex);
	if (pcMesh->HasBones())
		return DrawUnsorted(iIndex);

	// compute the position of the camera in worldspace
	aiMatrix4x4 mWorldInverse = mWorld;
	mWorldInverse.Inverse();
	mWorldInverse.Transpose();
	const aiVector3D vLocalCamera = mWorldInverse * mr->m_sCamera.vPos;

	// well ... this is really funny now. We must compute their distance
	// from the camera. We take the average distance of a face and add it 
	// to a map which sorts it
	std::map<float,unsigned int, std::greater<float> > smap;

	for (unsigned int iFace = 0; iFace < pcMesh->mNumFaces;++iFace)
	{
		const aiFace* pcFace = &pcMesh->mFaces[iFace];
		float fDist = 0.0f;
		for (unsigned int c = 0; c < 3;++c)
		{
			aiVector3D vPos = pcMesh->mVertices[pcFace->mIndices[c]];
			vPos -= vLocalCamera;
			fDist += vPos.SquareLength();
		}
		smap.insert(std::pair<float, unsigned int>(fDist,iFace));
	}

	// now we can lock the index buffer and rebuild it
	INDEXBUFFER_DESC sDesc;
	IRenderer::GetRendererInstance()->GetDesc(pcHelper->piIB,&sDesc);

	if (2 == sDesc.Format)
	{
		::uint16_t* aiIndices=(::uint16_t *)IRenderer::GetRendererInstance()->LockIndexBuffer(pcHelper->piIB);

		for (std::map<float,unsigned int, std::greater<float> >::const_iterator
			i =  smap.begin();
			i != smap.end();++i)
		{
			const aiFace* pcFace =  &pcMesh->mFaces[(*i).second];
			*aiIndices++ = (::uint16_t)pcFace->mIndices[0];
			*aiIndices++ = (::uint16_t)pcFace->mIndices[1];
			*aiIndices++ = (::uint16_t)pcFace->mIndices[2];
		}
	}
	else if (4 == sDesc.Format)
	{
		::uint32_t* aiIndices=(::uint32_t *)IRenderer::GetRendererInstance()->LockIndexBuffer(pcHelper->piIB);

		for (std::map<float,unsigned int, std::greater<float> >::const_iterator
			i =  smap.begin();
			i != smap.end();++i)
		{
			const aiFace* pcFace =  &pcMesh->mFaces[(*i).second];
			*aiIndices++ = (::uint32_t)pcFace->mIndices[0];
			*aiIndices++ = (::uint32_t)pcFace->mIndices[1];
			*aiIndices++ = (::uint32_t)pcFace->mIndices[2];
		}
	}
	IRenderer::GetRendererInstance()->UnLockIndexBuffer(pcHelper->piIB);
	// set vertex and index buffer
#if 0//???
	IRenderer::GetRendererInstance()->setShader(mr->m_piDefaultEffect);
	IRenderer::GetRendererInstance()->setVertexFormat(mr->m_DefaultVertexDecl);
/*
	aiMatrix4x4 aiMe = mr->GetAsset()->mAnimator->GetGlobalTransform( piNode);
	aiMatrix4x4 pcProj = aiMe * mViewProjection;
	aiMatrix4x4 pcCam = aiMe;
	pcCam.Inverse().Transpose();

		IRenderer::GetRendererInstance()->SetMatrix("WorldViewProjection",
			(const D3DXFROMWINEMATRIX*)&pcProj);
		IRenderer::GetRendererInstance()->SetMatrix("World",(const D3DXFROMWINEMATRIX*)&//aiMe
			mWorldInverse);
		IRenderer::GetRendererInstance()->SetMatrix("WorldInverseTranspose",
			(const D3DXFROMWINEMATRIX*)&//pcCam
			mWorldInverse);
*/
#endif
	IRenderer::GetRendererInstance()->setVertexBuffer(0,pcHelper->piVB);//,0,sizeof(AssetHelper::Vertex));

	// and draw the mesh
	IRenderer::GetRendererInstance()->setIndexBuffer(pcHelper->piIB);
	IRenderer::GetRendererInstance()->DrawIndexedPrimitive(PRIM_TRIANGLES,
		0,0,
		pcMesh->mNumVertices,0,
		pcMesh->mNumFaces);

	return 1;
}
};
#else
void TestMeshRenderer(){}
#endif
