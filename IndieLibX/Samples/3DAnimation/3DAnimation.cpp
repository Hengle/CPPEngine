/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
//include <VulkanModel.hpp>

const char* filename[] =
{
"/skeleton/skeleton.x",
"/skeleton/skeleton.bin.x",
0
};

class _3DAnimation : public STXGUI {
public:
	_3DAnimation(){}
	virtual ~_3DAnimation(){}
	MeshRenderer2 m_Mesh;
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
				m_Mesh.SetPlay(false);
				m_Mesh.SetAnimIndex(i);
				m_Mesh.SetPlay(true);
				break;
			}
			i++;
		}
	}
  }
}
	virtual void init(const char* aTitle="")
	{

		STXGUI::init("/skeleton/GUILayout.xml");
#if 0
		FW3::GetInstance()->GetMesh(m_Mesh. filename[0], eSimpleShader);
#else
		m_Mesh.Load(filename[0], eSimpleShader, true, true);
#endif

		m_Mesh.SetPlay(false);
		m_Mesh.SetAnimIndex(0);
		m_Mesh.SetPlay(true);
		size_t n=m_Mesh.GetNumAnimations();
		LOG_PRINT("NumAnimations=%d\n", n);
		LOG_PRINT("m_Mesh.vertices.size()=%d\n", m_Mesh.vertices.size());
		LOG_PRINT("m_Mesh.indices.size()=%d\n", m_Mesh.indices.size());
		LOG_PRINT("m_Mesh.meshes[0].vertices.size()=%d\n", m_Mesh.meshes[0].vertices.size());
		LOG_PRINT("m_Mesh.meshes[0].indices.size()=%d\n", m_Mesh.meshes[0].indices.size());
	if(0)
	{
	std::vector<stx_Material> vt;
	std::vector<__WORD__> g_indices2;
	std::vector<stx_Mesh> mesh_;
	stx_Mesh m;
	m.vertices=m_Mesh.meshes[0].vertices;
	m.indices=m_Mesh.meshes[0].indices;

	for(unsigned int i=0;i<m_Mesh.meshes[0].textures.size();i++)
		if(m_Mesh.meshes[0].textureNames[i]=="BONES.BMP")
			for(unsigned int j=0;j<m_Mesh.meshes[0].rindices.size()/3;j++)
				g_indices2.push_back(i);
		else
			for(unsigned int j=0;j<m_Mesh.meshes[0].rindices.size()/3;j++)
				g_indices2.push_back(0);

	mesh_.push_back(m);
	stx_Materials_(vt, g_indices2, mesh_);
	stx_writeXFile("/assimp--1.0.412-sdk/test2.x", mesh_);
	}
	}
	virtual void render(void)
	{

	IRenderer::GetRendererInstance()->Clear(true,true, D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

	D3DXFROMWINEMATRIX mvp, R, S, T;
	float mAngleX=0.0f;
	float mAngleY=0.0f;
	if
	(0)
	//(std::string(filename[0])=="/Orcs/Bonfire_wOrcs.x")
	{
    		float s=1.0f;
    		D3DXFROMWINEVECTOR3 t(0.0f, 0.0f, -7.5f);
		D3DXFROMWINEMatrixRotationYawPitchRoll( &R,
		                            D3DXFROMWINEToRadian(mAngleX+18.37f),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );
		D3DXFROMWINEMatrixScaling( &S, s, s, s);
		D3DXFROMWINEMatrixTranslation( &T, t.x, t.y, t.z);
    		mvp= R*S*T;
	} else {
		stx_GetAngles(mAngleX, mAngleY, 0.5f);
		D3DXFROMWINEMatrixRotationYawPitchRoll( &mvp,
		                            D3DXFROMWINEToRadian(mAngleX),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );
		}

	m_Mesh.render(&mvp);
	STXGUI::update();

		const char* txt = "Use mouse buttons to rotate the model.";
		IRenderer::GetRendererInstance()->drawText(txt, 10, 10,
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(),
			IRenderer::GetRendererInstance()->GetlinearClamp(),
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(),
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		txt = "CTRL-r resets the scene.";
		IRenderer::GetRendererInstance()->drawText(txt, 10, 30,
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(),
			IRenderer::GetRendererInstance()->GetlinearClamp(),
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(),
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present();
	}
};
int ApplicationLogic()
{
	//LOG_PRINT("elementsOf(filename)=%d\n", elementsOf(filename));
	IRenderer* r=IRenderer::GetRendererInstance("3DAnimation");
	IInput*    i=STX_Service::GetInputInstance();
	_3DAnimation app;
	app.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		app.render();
	}
	return 0;
}
