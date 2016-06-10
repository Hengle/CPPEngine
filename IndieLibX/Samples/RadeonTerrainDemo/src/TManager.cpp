//-----------------------------------------------------------------------------
// File: TManager.cpp
//
//  Terrain Demo.
//
// Copyright (c) 2000 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#include "Helper.h"
#include "TManager.h"
#include "DXErrors.h"

CTextureManager::CTextureManager()
{
	for(unsigned long i = 0; i < MAX_TEXTURES; i++)
		m_Textures[i]=0;
	//ZeroMemory(m_Textures, sizeof(m_Textures));
}

CTextureManager::~CTextureManager()
{
    for(unsigned long i = 0; i < MAX_TEXTURES; i++)
    {
        DeleteTexture(i);
    }
}

CTexture *CTextureManager::GetTexture(unsigned long n)
{
    if (n >= MAX_TEXTURES)
        return 0;
    return m_Textures[n];
}

unsigned long CTextureManager::AddTexture(CTexture *texture)
{
    for(unsigned long i = 0; i < MAX_TEXTURES; i++)
    {
        if (m_Textures[i] == 0)
        {
            m_Textures[i] = texture;
            return i;
        }
    }
    return 0;
}

int CTextureManager::DeleteTexture(unsigned long n)
{
    if (n >= MAX_TEXTURES)
        return 0;
    SAFE_DELETE(m_Textures[n]);

    return 1;
}

int CTextureManager::RestoreAll()
{
    for(unsigned long i = 0; i < MAX_TEXTURES; i++)
    {
        if (m_Textures[i] != 0)
        {
            if (FAILED(m_Textures[i]->Restore()))
                return 0;
        }
    }

    return 1;
}

void CTextureManager::InvalidateAll()
{
    for(unsigned long i = 0; i < MAX_TEXTURES; i++)
    {
        if (m_Textures[i] != 0)
        {
            m_Textures[i]->Invalidate();
        }
    }
}
