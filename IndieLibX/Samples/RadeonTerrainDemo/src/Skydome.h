

//-----------------------------------------------------------------------------
// File: Skydome.h
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _SKYDOME_H
#define _SKYDOME_H

#include "DXErrors.h"
#include "Model.h"
#include "Material.h"
#include "LitVertex.h"

class CSkyDome : public CModel
{
    bool m_bLoaded;
    unsigned long m_mSkyInd;
    unsigned long m_mGroundInd;
    TextureID m_mSkyID, m_mGroundID;
    // Sky mesh
    LPLITVERTEX2 m_pSkyVert;
    DWORD m_dwSkyNumVert;
    VertexBufferID m_pSkyVB;
    // Ground mesh
    LPLITVERTEX2 m_pGroundVert;
    DWORD m_dwGroundNumVert;
    VertexBufferID m_pGroundVB;

public:
    CSkyDome();
    ~CSkyDome();
    int Load();
    int Init();
    void Destroy();
    int Render();
    unsigned long CountPolys();
};

#endif

