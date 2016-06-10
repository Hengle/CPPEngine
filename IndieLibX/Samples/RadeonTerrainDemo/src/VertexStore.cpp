//-----------------------------------------------------------------------------
// File: VertexStore.cpp
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#include "Helper.h"
#include "VertexStore.h"
#include "DXErrors.h"

CVertexStore::CVertexStore(FILE *fp)
{
    fread(&m_dwVBCount, sizeof(unsigned long), 1, fp);
    m_pVB = new VB[m_dwVBCount];
    for(unsigned long i = 0; i < m_dwVBCount; i++)
    {
        fread(&m_pVB[i].dwSize, sizeof(unsigned long), 1, fp);
	LITVERTEX* pVertices = new LITVERTEX[m_pVB[i].dwSize];
	fread(pVertices, sizeof(LITVERTEX), m_pVB[i].dwSize, fp);
        m_pVB[i].pVertices = new LITVERTEX2[m_pVB[i].dwSize];
	for(unsigned int ii = 0; ii < m_pVB[i].dwSize; ii++)
		m_pVB[i].pVertices[ii]=LITVERTEX2(pVertices[ii]);

        m_pVB[i].pVB = -1;
    }
}

CVertexStore::~CVertexStore()
{
/*
    for(unsigned long i = 0; i < m_dwVBCount; i++)
    {
        SAFE_DELETE_ARRAY(m_pVB[i].pVertices);
    }
    SAFE_DELETE_ARRAY(m_pVB);
*/
}

int CVertexStore::Init()
{
	////LOG_PRINT("m_dwVBCount=%d\n", m_dwVBCount);
    for(unsigned long i = 0; i < m_dwVBCount; i++)
    {
#if 0
        if (m_pVB[i].pVB == 0)
        {
            SAFE_RELEASE(m_pVB[i].pVB);

            GameState::m_pd3dDevice->CreateVertexBuffer(m_pVB[i].dwSize*sizeof(LITVERTEX),
                               D3DUSAGE_WRITEONLY,
                               D3DFVF_LVERTEX1,
                               /*D3DPOOL_MANAGED*/D3DPOOL_DEFAULT,
                               &m_pVB[i].pVB,
                               0 );
        }

        LPLITVERTEX pvbVertices;
        if (SUCCEEDED(m_pVB[i].pVB->Lock( 0, m_pVB[i].dwSize*sizeof(LITVERTEX), (void**)&pvbVertices, 0 )))
        {
            memcpy(pvbVertices, m_pVB[i].pVertices, m_pVB[i].dwSize * sizeof(LITVERTEX));
            m_pVB[i].pVB->Unlock();
        }
        else
            return 0;
#else
	m_pVB[i].pVB =	IRenderer::GetRendererInstance()->addVertexBuffer(m_pVB[i].dwSize*sizeof(LITVERTEX2), STATIC, m_pVB[i].pVertices);//,m_pVB[i].pVB);
#endif
    }

    return 1;
}

void CVertexStore::Destroy()
{
/*
    for(unsigned long i = 0; i < m_dwVBCount; i++)
    {
        SAFE_RELEASE(m_pVB[i].pVB);
    }
*/
}

VertexBufferID CVertexStore::GetVertexBuffer(unsigned long index, unsigned long *size)
{
    if (index < m_dwVBCount)
    {
        *size = m_pVB[index].dwSize;
        return m_pVB[index].pVB;
    }
    else
        return -1;
}
