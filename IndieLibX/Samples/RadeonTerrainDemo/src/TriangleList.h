//-----------------------------------------------------------------------------
// File: TriangleList.h
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _TRIANGLELIST_H
#define _TRIANGLELIST_H

#include "DXErrors.h"
#include "LitVertex.h"
#include "VertexStore.h"

class CTriangleList
{
    void DeleteArrays();

public:
    DWORD m_dwMaterialID;
    TextureID m_pTexID;
    IndexBufferID m_pIndicesIB;
    WORD *m_pIndices;
    DWORD m_dwIndNum;
    DWORD m_dwVBID;

    CTriangleList();
    CTriangleList(FILE *fp);
    ~CTriangleList();
    int Load(FILE *fp);
    int Render( CVertexStore *pVS, int tex);
};

#endif
