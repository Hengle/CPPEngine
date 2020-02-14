/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
#include <libnova/sidereal_time.h>

ShaderID shd=-1;
TextureID texture=-1;
VertexFormatID vf=-1;

char names[10][256]={
"Sun",
"Moon",
"Mercury",
"Venus",
"Mars",
"Jupiter",
"Saturn",
"Uranus",
"Neptunus",
"Pluto"
};

int init(const char* aTitle)
{
#if defined(USE_HLSL_SHADERS)
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.shd", "mainVS", "mainPS");
#else
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.GLSL.shd", "main", "main");
#endif
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
	SamplerStateID ss=IRenderer::GetRendererInstance()->Getlinear();
	texture=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, ss);
	//texture=IRenderer::GetRendererInstance()->addImageLibTexture("/treetextures/tree35s.dds", false, ss);
	return 0;
}

void render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();
#if 1
	IRenderer::GetRendererInstance()->setShader(shd);
	IRenderer::GetRendererInstance()->setTexture("Base", texture);
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	IRenderer::GetRendererInstance()->setShaderConstant4f("color", color);
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setVertexFormat(vf);
	//		   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
#else
	IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);

	IRenderer::GetRendererInstance()->Vertex2f(-1.0f, -1.0f); 
	IRenderer::GetRendererInstance()->TexCoord2f(-1.0f, -1.0f);
	IRenderer::GetRendererInstance()->Vertex2f(-1.0f, 1.0f); 
	IRenderer::GetRendererInstance()->TexCoord2f(-1.0f,  1.0f);
	IRenderer::GetRendererInstance()->Vertex2f( 1.0f,  0.0f); 
	IRenderer::GetRendererInstance()->TexCoord2f( 1.0f, -1.0f);
	IRenderer::GetRendererInstance()->Vertex2f( 1.0f,  1.0f);
	IRenderer::GetRendererInstance()->TexCoord2f( 1.0f,  1.0f);

	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->End(texture, &I);
#endif

if(m_pSkyEmitter)
{
	
  D3DXFROMWINEVECTOR3 screenposition;//???
	D3DXFROMWINEVec3ProjectArray(m_pSkyEmitter->screenpositions, stride,m_pSkyEmitter->solarsystempositions, stride,
			    &viewport, &matProj, &matView, &matWorld,
			    10);
	 for(unsigned int i=0;i<10;i++)
	 {
		 if(PointInFrustum(m_pSkyEmitter->solarsystempositions[i].x, m_pSkyEmitter->solarsystempositions[i].y, m_pSkyEmitter->solarsystempositions[i].z))
		 {
		 char buf[256];
		 stx_snprintf(buf, 256, "%s", &names[i][0]);
		IRenderer::GetRendererInstance()->drawText(buf, screenposition.x, screenposition.y, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());

	 }
}}

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("SimpleTexture");	
	IInput*    i=STX_Service::GetInputInstance();
	init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		render();
	}
	return 0;
}
/*
		   	   x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 1.0f, 0.0f,
			 -1.0f,-1.0f, 0.0f, 1.0f,
			 -1.0f, 1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
*/
/*
		  	   x	  y     tx    ty
	float v[16] =  { -1.0f, -1.0f, 0.0f, 1.0f,  
			  1.0f, -1.0f, 1.0f, 1.0f,   
			  1.0f,  1.0f, 1.0f, 0.0f,  
			 -1.0f,  1.0f, 0.0f, 0.0f };
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 2, &v[0], &v[0], 4*sizeof(float));
*/

