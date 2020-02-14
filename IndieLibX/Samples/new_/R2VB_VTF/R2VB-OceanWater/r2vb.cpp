/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/IRenderer.h>

	//-----------------------------------------------------------------------------
// File: r2vb.cpp
// Copyright (c) 2006 ATI Technologies Inc. All rights reserved.
//-----------------------------------------------------------------------------

#include <assert.h>
#include <windows.h>
#include <D3DX9.h>

#include "r2vb.h"

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=0; } }

typedef struct {
    IRenderer::GetRendererInstance()       pdev;
    VertexBufferID vbdummy;
} R2VB, *PR2VB;


//---------------------------------------------------------------------------
// r2vbCheckCap
//
// Checks to see if the device supports render to vertex buffer
//---------------------------------------------------------------------------
int r2vbCheckCap(LPDIRECT3D9 pd3d)
{
    return pd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8,
                                   0, D3DRTYPE_SURFACE, 
                                   (FORMAT) R2VB_FOURCC_R2VB);
}



//---------------------------------------------------------------------------
// r2vbInit
//
// Initializes render to vertex buffer support
//---------------------------------------------------------------------------
int r2vbInit (IRenderer::GetRendererInstance() pdev, __DWORD__ size, PR2VBHANDLE phr2vb)
{   
    int hr = !D3D_OK; 
    
    assert(phr2vb);
    *phr2vb = 0;

    PR2VB pr2vb = new R2VB;
    if (pr2vb)
    {
        pr2vb->pdev = pdev;
        if(hr = pdev->addVertexFormat(
                size, D3DUSAGE_WRITEONLY, 0,
                D3DPOOL_DEFAULT, &(pr2vb->vbdummy), 0))
        {   
            return hr;
        }
        hr = D3D_OK; 
    }
    
    // Npatching is not supported with R2VB
    pdev->SetNPatchMode(0);

    // enabled the R2VB back-door for the device
    pdev->SetRenderState(D3DRS_POINTSIZE, r2vbGlbEnable_Set (TRUE));

    // initialize to the disabled state
    for( int s = 0; s < 16; s++)
    {   __DWORD__ dw = r2vbVStrm2SmpMap_Set (s , R2VB_VSMP_OVR_DIS);
        // Signal the driver to that next setVertexBuffer is a tex VB
        pdev->SetRenderState(D3DRS_POINTSIZE, dw);
    }

    *phr2vb = (PR2VBHANDLE) pr2vb;
    return hr;
}


//---------------------------------------------------------------------------
// r2vbTerm
//
// Disables render to vb and releases r2vb data
//---------------------------------------------------------------------------
int r2vbTerm (R2VBHANDLE    hr2vb)
{
    PR2VB pr2vb = (PR2VB) hr2vb;

    if (!pr2vb)
        return D3DERR_NOTFOUND;

    IRenderer::GetRendererInstance() pdev = pr2vb->pdev;

    // disable the R2VB back-door for the device
    pdev->SetRenderState(D3DRS_POINTSIZE, r2vbGlbEnable_Set (FALSE));

    SAFE_RELEASE(pr2vb->vbdummy);
    delete pr2vb;
    return D3D_OK; 
}


//-----------------------------------------------------------------------------
//  r2vbSetStreamTexSource
//
//   Set the vertex stream source to a texture
//-----------------------------------------------------------------------------
int r2vbSetStreamTexSource(
        R2VBHANDLE              hr2vb,
        __DWORD__                   stream,
        __DWORD__                   sampler,
        TextureID      ptex,
        __DWORD__                   offset,
        __DWORD__                   stride)
{
    PR2VB pr2vb = (PR2VB) hr2vb;
    IRenderer::GetRendererInstance() pdev = pr2vb->pdev;
    int hr = D3D_OK;
    
    hr = pdev->setTexture("DMAP"\, " + sampler, ptex);

    // Use default sampler state
    if SUCCEEDED(hr)
    {
        __DWORD__ dw = r2vbVStrm2SmpMap_Set (stream, sampler);
        // Signal the driver to that next setVertexBuffer is a tex VB
        hr = pdev->SetRenderState(D3DRS_POINTSIZE, dw);
    }

    if SUCCEEDED(hr)
    {
        hr = pdev->setVertexBuffer(stream, pr2vb->vbdummy, offset, stride);
    }

    return hr;
}

//---------------------------------------------------------------------------
// r2vbDisTexSource
//
// Disables render to vertex buffer for this stream
//---------------------------------------------------------------------------
int r2vbDisTexSource(
        R2VBHANDLE              hr2vb,
        __DWORD__                   stream)
{
    PR2VB pr2vb = (PR2VB) hr2vb;
    IRenderer::GetRendererInstance() pdev;

    if (pr2vb)
    {
        pdev = pr2vb->pdev;
        pdev->setTexture("DMAP"\, " + stream, 0);

        __DWORD__ dw = r2vbVStrm2SmpMap_Set (stream, R2VB_VSMP_OVR_DIS);

        // Signal the driver to that next setVertexBuffer is a tex VB
        pdev->SetRenderState(D3DRS_POINTSIZE, dw);
    }

    return D3D_OK;
}


	void render()
	{
		drawFrame();
	}	
	int init(const char* aTitle)
	{
		setup();
		resetCamera();
		load();
		return 0;
	}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("R2VB ");	
	IInput*    i=STX_Service::GetInputInstance();
	App app;
	app.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		app.render();
	}
	return 0;
}
	