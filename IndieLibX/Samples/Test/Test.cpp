/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

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

int ApplicationLogic()
{
	LOG_FNLN;
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "SimpleTexture");	
	LOG_FNLN;
	ShaderID shd = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, "main", "main");
	LOG_FNLN;
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	LOG_FNLN;
	VertexFormatID vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);

	LOG_FNLN;
	SamplerStateID ss=IRenderer::GetRendererInstance()->Getlinear();

	LOG_FNLN;
	TextureID texture=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, ss);
	LOG_FNLN;
	ShaderID sha = IRenderer::GetRendererInstance()->addShaderFromFile("/RadeonTerrainDemo/LitVertex.shd", "mainVS", "mainPS");
	LOG_FNLN;
	return 0;
}

