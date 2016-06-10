/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <FW3.h>
#include <GUI/GUIUtils.h>
#include <assimp_view.h>

// currently these are hardcoded
#if 1
static const std::string filename = "/trex dx/dino videogame.x";
#elif 0
static const std::string filename = "/trex dx/dino videogame2.x";
#elif 0
static const std::string filename = "/skeleton/skeleton.x";
#elif 0
static const std::string filename = "/assimp-3.1.1/models/X/Test.x";
#elif 0
static const std::string filename = "/assimp-3.1.1/models/X/dwarf.x";
#elif 0
static const std::string filename = "/PM/P-TankHistory/allied/t34_76/t34ds/T34-76.3DS";
#endif

class App : public STXGUI {
public:
	App(){}
	virtual ~App(){delete m_Mesh;}
	AssimpView::MeshRenderer* m_Mesh;
	virtual void actionPerformed(GUIEvent &evt){}
	virtual void init(int, int)
	{
		m_Mesh=new AssimpView::MeshRenderer(filename.c_str());
	}
	virtual void render(void)
	{
	IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

	D3DXFROMWINEMATRIX w;
	D3DXFROMWINEMatrixIdentity(&w);
	m_Mesh->render(w);

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present();
	}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "TestMesh");		
	IInput*    i=STX_Service::GetInputInstance();
	App app;
	app.init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();	
		app.render();
	}
	return 0;
}

