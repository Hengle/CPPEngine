/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
#include <GUI/GUIUtils.h>
#include <assimp_view.h>

const unsigned int N=15;

#define defaultshader "SimpleShader"
#define alternativeshader "AmbientLighting"
//define alternativeshader "DiffuseLighting"
//define alternativeshader "PhongLighting1"
//define alternativeshader "PhongLighting2"
//define alternativeshader "CompleteEffect"

const char* filename[] =
{
"/trex dx/dino videogame.x",
"/trex dx/dino videogame2.x",
"/skeleton/skeleton.x",
"/assimp--1.0.412-sdk/test.x",
"/assimp--1.0.412-sdk/dwarf.x",
"/AdvAniCh12/backdrop.x",
"/AdvAniCh12/chopper.X",
"/AdvAniCh12/Rotor.X",
"/AdvAniCh12/Shadow.X",
// Following five models require:
// 	the newest version of libMeshRenderer2.a static library.
//	the newest version of IndieLib_resources.zip package.
"/dx9sdk/cube.x",
"/dx9sdk/sphere.x",
"/dx9sdk/teapot.x",
"/dx9sdk/tiger.x",
"/dx9sdk/tiny.x",
"/DXJune2010/Dwarf/Dwarf.x",
//"/DXJune2010/Dwarf/DwarfWithEffectInstance.x",
//"/PM/P-TankHistory/allied/t34_76/t34ds/T34-76.3DS",
0
};

const char* shaders[] =
{
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
defaultshader,
alternativeshader,
defaultshader,
alternativeshader,
defaultshader,
defaultshader,
defaultshader,
0
};

class _3DAnimation : public STXGUI {
public:
	_3DAnimation()
	{
		m_i=0;
		shdname = defaultshader;
		m_Mesh[0]=0;	
		mAngleX = 0;
		mAngleY = 0;
		mSpeedRotation = 0.5;
		mDelta=0;
	}
	virtual ~_3DAnimation()
	{
		for(unsigned int i=0;i<N;i++)
			delete m_Mesh[i];
	}
	unsigned int m_i;
	std::string shdname;
	float	mAngleX;
	float	mAngleY;
	float	mSpeedRotation ;
	float	mDelta;
	AssimpView::MeshRenderer* m_Mesh[N];

	virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
	GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
	if(radioButton->isPressed() && radioButton->isChecked())
	{
		//LOG_PRINT("callbackString=%s\n", callbackString.data);

      		if(callbackString == "dino_videogame") 
		{
			m_i=0;
		}
		else if(callbackString == "dino_videogame2") 
		{
			m_i=1;
		}
		else if(callbackString == "skeleton") 
		{
			m_i=2;
		}
		else if(callbackString == "test") 
		{
			m_i=3;
		}
		else if(callbackString == "dwarf") 
		{
			m_i=4;
		}
		else if(callbackString == "backdrop") 
		{
			m_i=5;
		}
		else if(callbackString == "chopper") 
		{
			m_i=6;
		}
		else if(callbackString == "Rotor") 
		{
			m_i=7;
		}
		else if(callbackString == "Shadow") 
		{
			m_i=8;
		}
		else if(callbackString == "cube") 
		{
			m_i=9;
		}
		else if(callbackString == "sphere") 
		{
			m_i=10;
		}
		else if(callbackString == "teapot") 
		{
			m_i=11;
		}
		else if(callbackString == "tiger") 
		{
			m_i=12;
		}
		else if(callbackString == "tiny") 
		{
			m_i=13;
		}
		else if(callbackString == "Dwarf") 
		{
			m_i=14;
		}
	}
  }
}

	virtual void init(unsigned int, unsigned int)
	{
		for(unsigned int i=0;i<N;i++)
		{
			m_Mesh[i]=0;
#if 0
			m_Mesh[i]=FW3::GetInstance()->GetMesh(filename[i], shaders[i]);
#else
			m_Mesh[i]=new AssimpView::MeshRenderer(filename[i], shaders[i]);
#endif
			if(m_Mesh[i])
			{
				m_Mesh[i]->SetPlay(false);
				m_Mesh[i]->SetAnimIndex(0);
				m_Mesh[i]->SetPlay(true);
			}
		}

		STXGUI::init("/3DAnimation/GUILayout.xml");

	}
	virtual void render(void)
	{
		IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
		IRenderer::GetRendererInstance()->BeginScene();

		D3DXFROMWINEMATRIX matRot;
		D3DXFROMWINEMatrixRotationYawPitchRoll( &matRot,
		                            D3DXFROMWINEToRadian(mAngleX),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );

		m_Mesh[m_i]->render(matRot);

		const char* txt = "Use mouse buttons to rotate the model.";
		IRenderer::GetRendererInstance()->drawText(txt, 10, 10, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		char txt2[128];
		stx_snprintf(txt2, 128, "m_i=%d\n", m_i);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 30, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		mDelta = timeGetTime() / 1000.0f;
		if (STX_Service::GetInputInstance()->IsMouseButtonPressed (MBUTTON_LEFT))
			mAngleX += mSpeedRotation * mDelta;
		if (STX_Service::GetInputInstance()->IsMouseButtonPressed (MBUTTON_RIGHT))
			mAngleY += mSpeedRotation * mDelta;
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
	return 0;
}

