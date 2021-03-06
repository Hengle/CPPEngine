/*
  Copyright (c) 2018 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

const char* filename[] =
{
"/www.shadertoy.com/Simplicity.shd",
"/www.shadertoy.com/Simplicity3D.shd",
"/www.shadertoy.com/BallOfFire.shd",
#if 1
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

MeshRenderer2 shape3D[3];
//std::vector<std::string> ShaderFiles;
std::vector<ShaderID> shd;
std::vector<VertexFormatID> vf;
//std::vector<std::string> TextureFiles;
std::vector<TextureID> tex;
unsigned int s_i=0;
unsigned int t_i=0;
unsigned int s_i_=2;

int init(const char* aTitle)
{
	LOG_FNLN;
	//stx_InitShape3D("/www.shadertoy.com/Shaders.xml", ShaderFiles, shd, vf, TextureFiles, tex);
	shape3D[0].CreateTexturedQuad(2.0f, 2.0f, eShaderNone);
        shape3D[1].CreateTexturedBox(1.0f, 1.0f, 1.0f, eShaderNone);
        shape3D[2].CreateSphere(1.0f, eShaderNone);

	LOG_FNLN;
	for(unsigned int i=0;i<elementsOf(filename);i++)
	{
		shd.push_back(IRenderer::GetRendererInstance()->addShaderFromFile(filename[i], "main", "main"));
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
		vf.push_back(IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd[shd.size()-1]));
	}
	LOG_FNLN;

	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/NatureScene/Sky/clouds.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/Smoke/FireTransferFunction.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/Smoke/fireBase.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/2d/flame.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/1D/fire_gradient3.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/NVIDIA_Corporation/1D/FireGrade.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/R2VB-Sort/FirePalette.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	//tex.push_back(IRenderer::GetRendererInstance()->addImageLibTexture("/textures/ATI_SDK/Spectrum.png", false, IRenderer::GetRendererInstance()->Getlinear()));
	LOG_FNLN;
	LOG_PRINT("shd.size()=%d\n", shd.size());
	LOG_PRINT("vf.size()=%d\n", vf.size());
	LOG_FNLN;
	return 0;
}

void render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();
	if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F5))
	{
		if(!s_i_)
			s_i_=elementsOf(shape3D);
		else
			s_i_--;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F6))
	{
		s_i_++;
		if(s_i_>elementsOf(shape3D))
			s_i_=0;
	}
	if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F1))
	{
		if(!s_i)
			s_i=shd.size()-1;
		else
			s_i--;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F2))
	{
		s_i++;
		if(s_i>shd.size()-1)
			s_i=0;
	}
	if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F3))
	{
		if(!t_i)
			t_i=tex.size()-1;
		else
			t_i--;
	}
	else if(STX_Service::GetInputInstance()->OnKeyPress (STX_KEY_F4))
	{
		t_i++;
		if(t_i>tex.size()-1)
			t_i=0;
	}
#if 1
	float mAngleX=0.0f;
	float mAngleY=0.0f;
	stx_GetAngles(mAngleX, mAngleY);

		D3DXFROMWINEMATRIX W,R,S;
		D3DXFROMWINEMatrixRotationYawPitchRoll( &R,
		                            D3DXFROMWINEToRadian(mAngleX),
		                            D3DXFROMWINEToRadian(mAngleY),
		                            0.0f );


		D3DXFROMWINEVECTOR4 silver=stx_GetRGBA(eSilver);
		D3DXFROMWINEVECTOR4 AmbientColor=silver;
		D3DXFROMWINEVECTOR4 DiffuseColor=0.1f*silver;
		D3DXFROMWINEVECTOR4 LightDir(0.0f, 0.0f, -1.0f, 1.0f);
		D3DXFROMWINEVECTOR4 EyeDir(0.0f, 0.0f, 1.0f, 1.0f);

	//D3DXFROMWINEMatrixIdentity(&R);
	float s=0.25f*4.0f;
	D3DXFROMWINEMatrixScaling(&S, s, s, s);
	W=R*S;
	#if 1
	LOG_FNLN;
	LOG_PRINT("s_i=%d\n", s_i);
	LOG_PRINT("shd.size()=%d\n", shd.size());
	LOG_PRINT("vf.size()=%d\n", vf.size());
	LOG_PRINT("shd[%d]=%d\n", s_i, shd[s_i]);
	LOG_PRINT("vf[%d]=%d\n", s_i, vf[s_i]);
	#endif
        shape3D[s_i_].BeginDraw(&W, -1, shd[s_i], vf[s_i], AmbientColor, DiffuseColor, LightDir, EyeDir);

	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", W);
#endif
	//IRenderer::GetRendererInstance()->setShader(shd[s_i]);
	//IRenderer::GetRendererInstance()->setVertexFormat(vf[s_i]);
	D3DXFROMWINEVECTOR2 mouse((float)STX_Service::GetInputInstance()->GetMouseX(), (float)STX_Service::GetInputInstance()->GetMouseY());
	D3DXFROMWINEVECTOR2 resolution(IRenderer::GetRendererInstance()->GetViewportWidth(), IRenderer::GetRendererInstance()->GetViewportHeight());
	static float start=timeGetTime();
	float time=.00025 * (timeGetTime() - start );	
	IRenderer::GetRendererInstance()->setTexture("iChannel0", tex[0]);
	IRenderer::GetRendererInstance()->setTexture("iChannel1", tex[1]);
	IRenderer::GetRendererInstance()->setTexture("iChannel2", tex[2]);
	IRenderer::GetRendererInstance()->setShaderConstant2f("iResolution", resolution);
	IRenderer::GetRendererInstance()->setShaderConstant1f("iTime", time);
	IRenderer::GetRendererInstance()->setShaderConstant2f("iMouse", mouse);
	IRenderer::GetRendererInstance()->setShaderConstant2f("iChannelResolution", resolution);

	#if 0
	IRenderer::GetRendererInstance()->setShaderConstant1f("iGlobalTime", );                  // shader playback time (in seconds)
	IRenderer::GetRendererInstance()->setShaderConstant4f("iMouse", );                       // mouse pixel coords
	IRenderer::GetRendererInstance()->setShaderConstant4f("iDate", );                        // (year, month, day, time in seconds)
	IRenderer::GetRendererInstance()->setShaderConstant1f("iSampleRate", );                  // sound sample rate (i.e., 44100)
	IRenderer::GetRendererInstance()->setShaderConstant2f("ifFragCoordOffsetUniform", );     // used for tiled based hq rendering
	IRenderer::GetRendererInstance()->setShaderConstant1f("iTimeDelta", );                   // render time (in seconds)
	IRenderer::GetRendererInstance()->setShaderConstant1i("iFrame", );                       // shader playback frame
	IRenderer::GetRendererInstance()->setShaderConstant1f("iFrameRate", );
	#endif
	shape3D[s_i_].EndDraw();


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

	IRenderer::GetRendererInstance()->drawText("F3-key: previous texture", 10, 130, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	IRenderer::GetRendererInstance()->drawText("F4-key: next texture", 10, 150, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

		char txt2[512];
		stx_snprintf(txt2, 512, "s_i=%d: %s\n", s_i, filename[s_i]);
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 170, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
#if 0
		char txt2[256];
		stx_snprintf(txt2, 256, "Shader=%s\n", ShaderFiles[s_i].c_str());
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 90, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
		stx_snprintf(txt2, 256, "Texture=%s\n", TextureFiles[t_i].c_str());
		IRenderer::GetRendererInstance()->drawText(txt2, 10, 110, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
#endif

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("BallOfFire");	
	IInput*    i=STX_Service::GetInputInstance();
	init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		render();
	}
	return 0;
}

