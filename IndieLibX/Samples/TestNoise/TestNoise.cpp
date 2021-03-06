/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

const char* filename[] =
{
"/glslsandbox.com/e#59382.0.shd",
#if 0
"/www.shadertoy.com/Simplicity3D.shd",
"/www.shadertoy.com/Simplicity.shd",
//"/Shape3D/vbomb.fx",
//"/Shape3D/Flame.fx",
//"/Shape3D/fireball.fx",
"/RenderMonkey/Lava.shd",
"/RenderMonkey/DarkSky.shd",
"/RenderMonkey/Explosion.shd",
//"/RenderMonkey/FireBall.shd",
"/RenderMonkey/Inferno.shd",
"/RenderMonkey/BlueSky.shd",
"/RenderMonkey/Ocean.shd",
"/R2VB-Sort/particles.shd",
"/vnoise.shd",
//"/DDSAnimation/DDSAnimation.shd",
"/HLSL-noise2/HLSL-noise3.fx",
//"/vnoise2.shd",
"/noise/noise3D2.shd",
"/noise/noise3D.shd",
"/noise/noise2D.shd",
//"/Framework3/Effect.shd",
//"/Framework3/Effect2.shd",
"/www.shadertoy.com/explosionno12.shd",
"/www.shadertoy.com/PyroclasticFireball.shd",
//"/www.shadertoy.com/dustynebula1.shd",
//"/www.shadertoy.com/explosionno13.shd",
//"/HLSL-noise/HLSL-noise2.shd",
//"/HLSL-noise/HLSL-noise5.shd",
//"/HLSL-noise/HLSL-noise1.shd",
//"/HLSL-noise/HLSL-noise3.shd",
//"/HLSL-noise/HLSL-noise4.shd"
#else
"/www.shadertoy.com/Simplicity.shd",
"/www.shadertoy.com/Simplicity3D.shd",
"/www.shadertoy.com/StructuredSamplingDiagram.shd",
"/www.shadertoy.com/301FireShaderRemix3.shd",
"/www.shadertoy.com/AdaptiveSamplingDiagram.shd",
"/www.shadertoy.com/pyroclasticexplosion.shd",
"/www.shadertoy.com/PyroclasticFireball.shd",
"/www.shadertoy.com/SpaceElevator.shd",
"/www.shadertoy.com/BallOfFire.shd",
"/www.shadertoy.com/Simplestarfield.shd",
"/www.shadertoy.com/AlienBeacon.shd",
"/www.shadertoy.com/Common.shd",
"/www.shadertoy.com/dustynebula1.shd",
"/www.shadertoy.com/FakeVolumetricClouds.shd",
"/www.shadertoy.com/flame.shd",
"/www.shadertoy.com/Flameinthewind.shd",
"/www.shadertoy.com/fires.shd",
"/www.shadertoy.com/Flamethrower.shd",
"/www.shadertoy.com/301FireShaderRemix2.shd",
"/www.shadertoy.com/explosionno12.shd",
"/www.shadertoy.com/FireShader.shd",
"/www.shadertoy.com/RaymarchingSampleCode.shd",
"/www.shadertoy.com/Xyptonjtroz.shd",
"/www.shadertoy.com/301FireShaderRemix1.shd",
"/www.shadertoy.com/CheapCloudFlythrough.shd",
"/www.shadertoy.com/CheapCurvature.shd",
"/www.shadertoy.com/HashwithoutSine.shd",
"/www.shadertoy.com/explosionno13.shd",
"/www.shadertoy.com/tex2Dlod/StructuredVolSampling.shd",
"/www.shadertoy.com/tex2Dlod/explosioneffect.shd",
"/www.shadertoy.com/tex2Dlod/SamplePinning.shd",
"/www.shadertoy.com/tex2Dlod/supernova.shd",
"/www.shadertoy.com/tex2Dlod/Clouds.shd",
"/www.shadertoy.com/tex2Dlod/CloudySpikeBall.shd",
"/www.shadertoy.com/tex2Dlod/campfire.shd",
"/www.shadertoy.com/tex2Dlod/ProtoplanetaryDisk.shd",
"/www.shadertoy.com/tex2Dlod/CloudTen.shd",
"/www.shadertoy.com/tex2Dlod/SupernovaRemnant.shd",
"/www.shadertoy.com/tex2Dlod/volumetricexplosion.shd",
"/www.shadertoy.com/tex2Dlod/Watchitburn.shd",
"/www.shadertoy.com/tex2Dlod/DustyNebula4.shd",
"/www.shadertoy.com/tex2Dlod/EmissionClouds.shd",
"/www.shadertoy.com/tex2Dlod/gasexplosion.shd",
"/www.shadertoy.com/tex2Dlod/Hell.shd",
"/www.shadertoy.com/tex2Dlod/type2supernova.shd",
"/www.shadertoy.com/tex2Dlod/anatomyofanexplosion.shd",
"/www.shadertoy.com/tex2Dlod/DustyNebula3.shd",
#endif
};

class _Shape3D
{
	TextureID texID;
	MeshRenderer2 shape3D[3];
	std::vector<ShaderID> shader;
	//std::vector<std::string> ShaderFiles;
	std::vector<VertexFormatID> vf;
    std::vector<TextureID> tex;
	unsigned int m_i, s_i;
public:
int init(const char* aTitle)
{
    	s_i=2;
	m_i=0;
	shape3D[0].CreateTexturedQuad(2.0f, 2.0f, eShaderNone);
        shape3D[1].CreateTexturedBox(1.0f, 1.0f, 1.0f, eShaderNone);
        shape3D[2].CreateSphere(1.0f, eShaderNone);
	texID=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, IRenderer::GetRendererInstance()->Getlinear());

	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/NatureScene/Sky/clouds.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/Smoke/FireTransferFunction.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/Smoke/fireBase.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/2d/flame.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/1D/fire_gradient3.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/1D/FireGrade.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/R2VB-Sort/FirePalette.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/ATI_SDK/Spectrum.png", false, IRenderer::GetRendererInstance()->Getlinear()));

	for(unsigned int i=0;i<elementsOf(filename);i++)
	{
		shader.push_back(IRenderer::GetRendererInstance()->addShaderFromFile(filename[i], "main", "main"));
		//ShaderFiles.push_back(filename[i]);
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
			0, TYPE_NORMAL,   FORMAT_FLOAT, 3,
			0, TYPE_BINORMAL, FORMAT_FLOAT, 3,
			0, TYPE_TANGENT,  FORMAT_FLOAT, 3,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 3,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
		};
		vf.push_back(IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shader[shader.size()-1]));
	}

	return 0;
}

void render( )
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F2))
	{
		if(m_i<elementsOf(filename)-1)
			m_i++;
			//if(m_i>=elementsOf(filename))
			//	m_i=0;
		else
			m_i=0;
	}
	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F1))
	{
		if(!m_i)
			m_i=elementsOf(filename)-1;
		else
			m_i--;
	}
	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F3))
		s_i=0;
	if(STX_Service::GetInputInstance()->OnKeyPress(STX_KEY_F4))
		s_i=1;

	float mAngleX=266.0f;
	float mAngleY=250.0f;
	stx_GetAngles(mAngleX, mAngleY);

		D3DXFROMWINEMATRIX matRot;
		D3DXFROMWINEMatrixRotationYawPitchRoll( &matRot,
		                            D3DXFROMWINEToRadian(mAngleX),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );

					D3DXFROMWINEVECTOR4 a(0.1f, 0.0f, 0.0f, 1.0f);
					D3DXFROMWINEVECTOR4 d(1.0f, 0.0f, 0.0f, 1.0f);
					D3DXFROMWINEVECTOR4 l(0.0f, 1.0f, -1.0f, 1.0f);
					D3DXFROMWINEVECTOR4 e(0.0f, 0.0f, -1.0f, 1.0f);
					D3DXFROMWINEVECTOR4 gold=stx_GetRGBA(eGold);
					D3DXFROMWINEVECTOR4 silver=stx_GetRGBA(eSilver);
					a=silver;
					d=silver;
	
	shape3D[s_i].BeginDraw(&matRot, -1, shader[m_i], vf[m_i], a, d, l, e);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", matRot);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("modelViewProjection", matRot);
#if 1
	D3DXFROMWINEVECTOR2 vMouse((float)STX_Service::GetInputInstance()->GetMouseX(), (float)STX_Service::GetInputInstance()->GetMouseY());
	D3DXFROMWINEVECTOR2 vResolution(IRenderer::GetRendererInstance()->GetViewportWidth(), IRenderer::GetRendererInstance()->GetViewportHeight());
	static float start=timeGetTime();
	float time=.00025 * (timeGetTime() - start );
	IRenderer::GetRendererInstance()->setShaderConstant2f("iMouse", vMouse);
	IRenderer::GetRendererInstance()->setShaderConstant2f("iResolution", vResolution);
	IRenderer::GetRendererInstance()->setShaderConstant2f("iChannelResolution", vResolution);
	IRenderer::GetRendererInstance()->setShaderConstant1f("iTime", time);
	IRenderer::GetRendererInstance()->setTexture("iChannel0", tex[0]);
	IRenderer::GetRendererInstance()->setTexture("iChannel1", tex[1]);
	IRenderer::GetRendererInstance()->setTexture("iChannel2", tex[2]);
#else
	IRenderer::GetRendererInstance()->setShaderConstant1f("time", time);
int iI=0;
sampler2D tExplosion;
#endif
	shape3D[s_i].EndDraw();


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

		char buf[64];
		stx_snprintf(buf, 64, "mAngleX=%f\n", mAngleX); 
		IRenderer::GetRendererInstance()->drawText(buf, 10, 50, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
		stx_snprintf(buf, 64, "mAngleY=%f\n", mAngleY); 
		IRenderer::GetRendererInstance()->drawText(buf, 10, 70, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->drawText("F1-key: previous shader", 10, 90, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->drawText("F2-key: next shader", 10, 110, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		char txt2[512];
		stx_snprintf(txt2, 512, "m_i=%d: %s\n", m_i, filename[m_i]);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 130, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
}
};
int ApplicationLogic()
{	
	IRenderer* r=IRenderer::GetRendererInstance("TestNoise");	
	IInput*    i=STX_Service::GetInputInstance();
	_Shape3D shape3D;
	shape3D.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		shape3D.render();
	}
	return 0;
}

