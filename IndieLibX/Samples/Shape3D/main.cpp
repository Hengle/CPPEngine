/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
#include <GUI/GUIUtils.h>
#include <FW3.h>
#include <Shape3D.h>

class _Shape3D: public STXGUI
{
	static const unsigned int N=3;
	Shape3D shape3D[N];
	unsigned int m_i;
	float	mAngleX;
	float	mAngleY;
	float	mSpeedRotation ;
	float	mDelta;
public:
int init(unsigned int width, unsigned int height)
{
	m_i=0;	
		mAngleX = 0;
		mAngleY = 0;
		mSpeedRotation = 0.5;
		mDelta=0;
	
        shape3D[0].CreateTexturedBox(1.0f, 1.0f, 1.0f, "AmbientLighting");
        shape3D[1].CreateBox(1.0f, 1.0f, 1.0f, "AmbientLighting");
	shape3D[2].CreateCylinder(1.0f, 1.0f, 1.0f, 20, 20, "AmbientLighting");
        //shape3D[3].CreateSphere(1.0f, 20, 20, "AmbientLighting");
	
	STXGUI::init("/Shape3D/GUILayout.xml");
}

int render( )
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

		D3DXFROMWINEMATRIX matRot;
		D3DXFROMWINEMatrixRotationYawPitchRoll( &matRot,
		                            D3DXFROMWINEToRadian(mAngleX),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );
	
        shape3D[m_i].Draw(matRot);

		mDelta = timeGetTime() / 1000.0f;
		if (STX_Service::GetInputInstance()->IsMouseButtonPressed (MBUTTON_LEFT))
			mAngleX += mSpeedRotation * mDelta;
		if (STX_Service::GetInputInstance()->IsMouseButtonPressed (MBUTTON_RIGHT))
			mAngleY += mSpeedRotation * mDelta;

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
	
	STXGUI::update();
	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );

	return 0;
}
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
		if(callbackString == "TexturedBox")
      		{
			m_i = 0;
		}
         	if(callbackString == "Box")
      		{
			m_i = 1;
		}/*
         	if(callbackString == "Sphere")
      		{
			m_i = 2;
		}*/
         	if(callbackString == "Cylinder")
      		{
			m_i = 2;
		}
    }
  }
}
};
int ApplicationLogic()
{	
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "Shape3D");		
	IInput*    i=STX_Service::GetInputInstance();
	_Shape3D shape3D;
	shape3D.init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		shape3D.render();
	}
	return 0;
}

