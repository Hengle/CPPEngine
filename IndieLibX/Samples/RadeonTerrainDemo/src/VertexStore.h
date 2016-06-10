//-----------------------------------------------------------------------------
// File: VertexStore.h
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _VERTEXSTORE_H
#define _VERTEXSTORE_H

#include "DXErrors.h"
#include "LitVertex.h"
#include "Helper.h"

typedef struct tagVB
{
    DWORD dwSize;
    LITVERTEX2 *pVertices;
    VertexBufferID pVB;
} VB;

class CVertexStore
{
    VB *m_pVB;
    DWORD m_dwVBCount;

public:
    CVertexStore(FILE *fp);
    ~CVertexStore();

    int Init();
    void Destroy();
    VertexBufferID GetVertexBuffer(unsigned long index, unsigned long *size);
};

#endif
