/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
//include <lua.hpp>	
//include <string>
#include <Framework3/IRenderer.h>
#include <GUI/GUIUtils.h>
#include <assimp_view.h>
using namespace AssimpView;

// currently these are hardcoded
#if 0
static const std::string filename = "/trex dx/dino videogame.x";
#elif 0
static const std::string filename = "/trex dx/dino videogame2.x";
#elif 1
static const std::string filename = "/skeleton/skeleton.x";
#elif 0
static const std::string filename = "/assimp--1.0.412-sdk/test.x";
#elif 0
static const std::string filename = "/assimp--1.0.412-sdk/dwarf.x";
#elif 0
static const std::string filename = "/PM/P-TankHistory/allied/t34_76/t34ds/T34-76.3DS";
#endif

class _3DAnimation : public STXGUI {
public:
	_3DAnimation()
	{
		m_Mesh=0;
		mAngleX = 0;
		mAngleY = 0;
		mSpeedRotation = 0.5;
		mDelta=0;
	}
	virtual ~_3DAnimation(){delete m_Mesh;}
	MeshRenderer* m_Mesh;
	float	mAngleX;
	float	mAngleY;
	float	mSpeedRotation ;
	float	mDelta;
	virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
	GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
	unsigned int i=0;
	if(radioButton->isPressed() && radioButton->isChecked())
	{
		while(i<5)
		{
			char buf[256] = {0};
			stx_snprintf(buf, 256, "Skeleton_%d\0", i+1);
      			if(callbackString == buf) 
			{
				m_Mesh->SetPlay(false);
				m_Mesh->SetAnimIndex(i);
				m_Mesh->SetPlay(true);
				break;
			}
			i++;
		}
	}
  }
}
	virtual void init(int, int)
	{
#if 0
		lua_State *L = lua_open();
		lua_openlibs(L);
		lua_dofile(L, "script.lua");
		std::string filename = "";
		lua_getglobal(L, "filename");
		if(lua_isstring(L, lua_gettop(L))){
			filename = lua_tostring(L, lua_gettop(L));
			m_Mesh=new MeshRenderer(filename.c_str());
		}else{
			LOG_PRINT("lua variable \"filename\" was not a string!\n");
		}
		lua_close();
#elif 0
		m_Mesh=FW3::GetInstance()->GetMesh(filename.c_str(), "SimpleShader");
#else
		m_Mesh=new MeshRenderer(filename.c_str(), "SimpleShader");
#endif

		m_Mesh->SetPlay(false);
		m_Mesh->SetAnimIndex(0);
		m_Mesh->SetPlay(true);
		size_t n=m_Mesh->GetNumAnimations();
		LOG_PRINT("NumAnimations=%d\n", n);

		STXGUI::init("/skeleton/GUILayout.xml");
	}
	virtual void render(void)
	{

	IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

	D3DXFROMWINEMATRIX w;
	D3DXFROMWINEMatrixIdentity(&w);
	m_Mesh->render(w);
	STXGUI::update();

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present();
	}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "3DAnimation");		
	IInput*    i=STX_Service::GetInputInstance();
	_3DAnimation app;
	app.init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();	
		app.render();
	}
	delete app.m_Mesh;
	return 0;
}

