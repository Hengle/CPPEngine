/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

#if defined(USE_GLES_SHADERS) && (defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
const char* shdTxt="[Vertex shader]\n"
"attribute mediump vec2 aPosition;\n"
"attribute mediump vec2 aTexCoord;\n"
"varying mediump vec2 vTexCoord;\n"
"uniform highp mat4 worldViewProj;\n"
"void main(){\n"
"	gl_Position = worldViewProj*vec4(aPosition.x, aPosition.y, 0, 1);\n"
"	vTexCoord=aTexCoord;\n"
"}\n"
"[Fragment shader]\n"
"varying mediump vec2 vTexCoord;\n"
"uniform sampler2D Base;\n"
"void main()\n"
"{\n"
"gl_FragColor = texture2D(Base,vTexCoord);\n"
"}\n";
#else
const char* shdTxt="[Vertex shader]"
"struct VsIn {\n"
"	float2 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"ROW_MAJOR float4x4 worldViewProj MVPSEMANTIC;\n"
"VsOut main(VsIn In){\n"
"	VsOut Out=(VsOut)0;\n"
"	Out.position = mul(worldViewProj, float4(In.position.x, In.position.y, 0.0, 1.0));\n"
"	Out.texCoord = In.texCoord;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"sampler2D Base;\n"
"float4 main(VsOut IN): COLOR {\n"
"	return tex2D(Base, IN.texCoord);\n"
"}\n";
#endif

ShaderID shd=-1;
TextureID texture=-1;
VertexFormatID vf=-1;

int init(unsigned int width, unsigned int height)
{
	shd = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, "main", "main");
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	SamplerStateID ss=IRenderer::GetRendererInstance()->Getlinear();
	texture=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, ss);
	return 0;
}

int render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, false, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();
	//		   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setTexture("Base", texture);
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
	return 0;
}

int ApplicationLogic()
{
#if 0
	std::string f=std::string("/");
	std::string file=stx_convertpath(f);
	LOG_PRINT("iosDocumentsFolder(/)=%s\n", file.c_str());
#endif
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "SimpleTexture");	
	IInput*    i=STX_Service::GetInputInstance();
	init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		render();
	}
	return 0;
}

