/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

stx_Effect* g_pEffect=0;

int init(const char* aTitle)
{
#if 0//def _MSC_VER
	{std::ofstream out("__tmp__.xml");
	out << "<?xml version=\"1.0\"?>\n";
	out << "<Body>\n";
	out << "<Textures>\n";
	out << "	    <Texture sampler=\"Base\" file=\"/test.bmp\" dimension=\"2\" mipmap=\"false\" SamplerState=\"linear\" />\n";
	out << "</Textures>\n";
	out << "<Techniques>\n";
	out << "<technique name=\"Simple\">\n";
	out << "    <pass name=\"P0\">\n";
	out << "        <DepthState name=\"NoDepthTest\" />\n";
	out << "	    <RasterizerState name=\"NoCullMS\" />\n";
	out << "        <DepthStencilState name=\"NoDepthStencil\" X=\"0\" />\n";
	out << "        <BlendState name=\"NoBlending\" R=\"0.0\" G=\"0.0\" B=\"0.0\" A=\"0.0\" X=\"0xFFFFFFFF\" />\n";
	out << "	    <PixelShader profile=\"ps_3_3\" name=\"mainPS\" />\n";
	out << "        <VertexShader profile=\"vs_3_3\" name=\"mainVS\" />\n";
	out << "        <GeometryShader profile=\"gs_4_0\" name=\"\" />\n";
	out << "        <HullShader profile=\"hs_5_0\" name=\"\" />\n";
	out << "        <DomainShader profile=\"ds_5_0\" name=\"\" />\n";
	out << "        <ComputeShader profile=\"cs_5_0\" name=\"\" />\n";
	out << "	<VertexFormat>\n";
	out << "        	<FormatDesc Type=\"VERTEX\" 	Format=\"FLOAT2\" />\n";
	out << "		<FormatDesc Type=\"TEXCOORD\"	Format=\"FLOAT2\" />\n";
	out << "	</VertexFormat>\n";
	out << "    </pass>\n";
	out << "</technique>\n";
	out << "</Techniques>\n";
	out << "</Body>\n";
	out.close();}
		LOG_FNLN;
	TiXmlDocument doc("__tmp__.xml");
		LOG_FNLN;
	doc.LoadFile();
		LOG_FNLN;
	TiXmlHandle docHandle( &doc );
		LOG_FNLN;
	TiXmlElement* child2 = docHandle.FirstChild("Body").Element();
		LOG_FNLN;
	if ( !child2 )
	{
		LOG_FNLN;
		stx_exit(0);
	}
		LOG_FNLN;
	child2 = docHandle.FirstChild("Body").FirstChild("Techniques").Element();
		LOG_FNLN;
	if ( !child2 )
	{
		LOG_FNLN;
		stx_exit(0);
	}
		LOG_FNLN;
	child2 = docHandle.FirstChild("Body").FirstChild("Techniques").Child("technique", 0 ).Element();
		LOG_FNLN;
	if ( !child2 )
	{
		LOG_FNLN;
		stx_exit(0);
	}
	LOG_FNLN;
	stx_exit(0);
#endif
//LOG_FNLN;
    g_pEffect=IRenderer::GetRendererInstance()->addEffectFromFile("/SimpleTexture/SimpleTexture.fx");
//LOG_FNLN;
    if(!g_pEffect)
        stx_exit(0);
//LOG_FNLN;
	return 0;
}

void render()
{
//LOG_FNLN;
	float f=128.0f/256.0f;
//LOG_FNLN;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
//LOG_FNLN;
	IRenderer::GetRendererInstance()->BeginScene();
//LOG_FNLN;
    unsigned int cPasses=0;
    unsigned int Value=0;
	unsigned int iPass=0;
//LOG_FNLN;
	g_pEffect->Begin(&cPasses, Value);
//LOG_FNLN;
	g_pEffect->BeginPass(iPass);
//LOG_FNLN;
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	g_pEffect->SetVector("color", &color);
//LOG_FNLN;
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
//LOG_FNLN;
	g_pEffect->SetMatrix("worldViewProj", &I);
//LOG_FNLN;
	//		           x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			          1.0f, 1.0f, 1.0f, 0.0f,
			         -1.0f,-1.0f, 0.0f, 1.0f,
			         -1.0f, 1.0f, 0.0f, 0.0f };
//LOG_FNLN;
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
//LOG_FNLN;
	g_pEffect->EndPass();
//LOG_FNLN;
	g_pEffect->End();
//LOG_FNLN;
	IRenderer::GetRendererInstance()->EndScene();
//LOG_FNLN;
	IRenderer::GetRendererInstance()->Present( );
//LOG_FNLN;
	//stx_exit(0);
}

int ApplicationLogic()
{
//LOG_FNLN;
	IRenderer* r=IRenderer::GetRendererInstance("SimpleEffect");	
//LOG_FNLN;
	IInput*    i=STX_Service::GetInputInstance();
//LOG_FNLN;
	init("");
//LOG_FNLN;
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
//LOG_FNLN;
		i->Update();
//LOG_FNLN;
		render();
//LOG_FNLN;
	}
//LOG_FNLN;
	return 0;
}

