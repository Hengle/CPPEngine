/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

const char *getFormatString(const FORMAT format);

ShaderID effect2;
TextureID texture,texture2,texture3,texture4;

int init(unsigned int width, unsigned int height)
{
	SamplerStateID ss=IRenderer::GetRendererInstance()->GetlinearClamp();
	texture=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/test.bmp"
		,false,ss);
	texture2=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/dx8sdk/tree01S.tga"
		,false,ss);
	texture3=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/AdvAniCh12/Particle_People1.png"
		,false,ss);
	texture4=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/NSGUI/GUIElements.png"
		,false,ss);
	return 0;
}

void drawTexture(TextureID texture, int x, int y, int sx, int sy, bool InScene, float tx, float ty, float stx, float sty)
{
	IRenderer::GetRendererInstance()->InitTex(IRenderer::GetRendererInstance()->GettexShader(),IRenderer::GetRendererInstance()->GettexVF());

	float qx=x;
	float qy=y;
	float dx=sx;
	float dy=sy;

	float* vertices=new float[16];
	bool fq=((tx==0.0f)&&(ty==0.0f)&&(stx==1.0f)&&(sty==1.0f));
	{
		if(fq)
		{
			float verts2[] =
			{
				qx+dx,qy,1.0f, 0.0f
				,qx+dx,qy+dy,1.0f, 1.0f
				,qx,qy,0.0f, 0.0f
				,qx,qy+dy,0.0f, 1.0f
			}
			;
			stx_memcpy(vertices,verts2,16*sizeof(float));
		}
		else
		{
			float verts2[] =
			{
				qx+dx,qy,tx+stx,ty
				,qx+dx,qy+dy,tx+stx,ty+sty
				,qx,qy,tx,ty
				,qx,qy+dy,tx,ty+sty
			}
			;
			stx_memcpy(vertices,verts2,16*sizeof(float));
		}
	}
	if(!InScene)
	{
		IRenderer::GetRendererInstance()->BeginScene();
	}
	IRenderer::GetRendererInstance()->setShader(IRenderer::GetRendererInstance()->GettexShader());
	IRenderer::GetRendererInstance()->setVertexFormat(IRenderer::GetRendererInstance()->GettexVF());
	IRenderer::GetRendererInstance()->setTexture("Base", texture);
	IRenderer::GetRendererInstance()->setShaderConstant4f("colorRGBA", D3DXFROMWINEVECTOR4(1.0f,1.0f,1.0f,1.0f));
	IRenderer::GetRendererInstance()->setBlendState(IRenderer::GetRendererInstance()->GetblendSrcAlpha());
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	IRenderer::GetRendererInstance()->setup2DMode(0, (float) IRenderer::GetRendererInstance()->GetViewportWidth(), 0, (float) IRenderer::GetRendererInstance()->GetViewportHeight());
	IRenderer::GetRendererInstance()->setShaderConstant4f("scaleBias",
		IRenderer::GetRendererInstance()->GetscaleBias2D());

	IRenderer::GetRendererInstance()->DrawPrimitiveUP(
		PRIM_TRIANGLE_STRIP,
		IRenderer::GetRendererInstance()->getPrimitiveCount(PRIM_TRIANGLE_STRIP, 4),
		vertices, vertices, 4*sizeof(float));
	if(!InScene)
	{
		IRenderer::GetRendererInstance()->EndScene();
	}
	delete[] vertices;
}

void drawBlending( void )
{
	static int s=0;
	drawTexture(texture, 0, 0, 400, 300, true, 0.0f, 0.0f, 1.0f, 1.0f);
	drawTexture(texture2, 400, 300, 400, 300, true, 0.0f, 0.0f, 1.0f, 1.0f);
}
	void renderframe(D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, TextureID texid, D3DXFROMWINEVECTOR2 atex=D3DXFROMWINEVECTOR2(0.0f, 0.0f), D3DXFROMWINEVECTOR2 atexsize=D3DXFROMWINEVECTOR2(1.0f, 1.0f)){
	TexVertex dest[4];
	float x=aposition.x;
	float y=aposition.y;
	dest[0].position = D3DXFROMWINEVECTOR2(x + asize.x, y);
	dest[0].texCoord = D3DXFROMWINEVECTOR2(atex.x + atexsize.x, atex.y);
	dest[1].position = D3DXFROMWINEVECTOR2(x + asize.x, y + asize.y);
	dest[1].texCoord = D3DXFROMWINEVECTOR2(atex.x + atexsize.x, atex.y + atexsize.y);
	dest[2].position = D3DXFROMWINEVECTOR2(x, y);
	dest[2].texCoord = D3DXFROMWINEVECTOR2(atex.x, atex.y);
	dest[3].position = D3DXFROMWINEVECTOR2(x, y + asize.y);
	dest[3].texCoord = D3DXFROMWINEVECTOR2(atex.x, atex.y + atexsize.y);
	IRenderer::GetRendererInstance()->drawTextured(PRIM_TRIANGLE_STRIP, dest, 2, texid, IRenderer::GetRendererInstance()->GetblendSrcAlpha(), IRenderer::GetRendererInstance()->GetnoDepthTest());
}

void render5(const TextureID texture, float qx, float qy, float dx, float dy, float tx=0.0f, float ty=0.0f, float stx=1.0f, float sty=1.0f)
{
	D3DXFROMWINEVECTOR4 c(1.0f, 1.0f, 1.0f, 1.0f);
	IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);

	float sizeX=dx;
	float sizeY=dy;

	IRenderer::GetRendererInstance()->TexCoord2f( sty, sty);
	IRenderer::GetRendererInstance()->Vertex2f(sizeX,-sizeY);
	IRenderer::GetRendererInstance()->TexCoord2f( sty, 0);
	IRenderer::GetRendererInstance()->Vertex2f(sizeX,sizeY);

	IRenderer::GetRendererInstance()->TexCoord2f( 0, sty);
	IRenderer::GetRendererInstance()->Vertex2f( -sizeX,-sizeY);

	IRenderer::GetRendererInstance()->TexCoord2f( 0, 0);
	IRenderer::GetRendererInstance()->Vertex2f( -sizeX,sizeY);

	IRenderer::GetRendererInstance()->End(texture);
}

void render4(const TextureID texture, float qx, float qy, float dx, float dy, float tx=0.0f, float ty=0.0f, float stx=1.0f, float sty=1.0f)
{
	D3DXFROMWINEVECTOR4 c(1.0f, 1.0f, 1.0f, 1.0f);
	IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);

	IRenderer::GetRendererInstance()->TexCoord2f(1, 0);IRenderer::GetRendererInstance()->Vertex2f(qx,qy+dy);
	IRenderer::GetRendererInstance()->TexCoord2f(1, 1);IRenderer::GetRendererInstance()->Vertex2f(qx,qy);
	IRenderer::GetRendererInstance()->TexCoord2f(0, 0);IRenderer::GetRendererInstance()->Vertex2f(qx+dx,qy+dy);
	IRenderer::GetRendererInstance()->TexCoord2f(0, 1);IRenderer::GetRendererInstance()->Vertex2f(qx+dx,qy);

	IRenderer::GetRendererInstance()->End(texture);
}

void render3(const TextureID texture, float qx, float qy, float dx, float dy, float tx=0.0f, float ty=0.0f, float stx=1.0f, float sty=1.0f)
{
	D3DXFROMWINEVECTOR4 c(1.0f, 1.0f, 1.0f, 1.0f);

	float xPos=qx;
	float yPos=qy;
	float height=dx;
		
TexVertex quad[] = {  MAKETEXQUAD2(qx, qy, dx, tx, ty, stx) };

	IRenderer::GetRendererInstance()->drawTexture(texture, qx, qy, dx, dy, true, tx, ty, stx, sty);

	TexVertex verticestex[] = {
	TexVertex(D3DXFROMWINEVECTOR2(qx+dx,qy), D3DXFROMWINEVECTOR2(tx+stx,ty)),
	TexVertex(D3DXFROMWINEVECTOR2(qx+dx,qy+dy), D3DXFROMWINEVECTOR2(tx+stx,ty+sty)),
	TexVertex(D3DXFROMWINEVECTOR2(qx,qy), D3DXFROMWINEVECTOR2(tx,ty)),
	TexVertex(D3DXFROMWINEVECTOR2(qx,qy+dy), D3DXFROMWINEVECTOR2(tx,ty+sty))
				};
	IRenderer::GetRendererInstance()->drawTextured(PRIM_TRIANGLE_STRIP, &verticestex[0], 4, texture, IRenderer::GetRendererInstance()->GetblendSrcAlpha(), IRenderer::GetRendererInstance()->GetnoDepthTest(), &c);
}
int render()
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, false, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));

	IRenderer::GetRendererInstance()->BeginScene();

	//drawBlending();

	IRenderer::GetRendererInstance()->drawLine(0,0,10,10,D3DXFROMWINEVECTOR4(1,1,1,1));
		IRenderer::GetRendererInstance()->drawRect(10,10,20,20,D3DXFROMWINEVECTOR4(1,1,1,1));//,1.0f);
		IRenderer::GetRendererInstance()->drawRoundRect(20,20,30,30,5.0f,D3DXFROMWINEVECTOR4(1,1,1,1));//,1.0f);
		IRenderer::GetRendererInstance()->drawTriangle(30,30,40,40,50,30,D3DXFROMWINEVECTOR4(1,1,1,1));//,1.0f);
		IRenderer::GetRendererInstance()->drawQuad(40,40,40,50,50,50,50,40,D3DXFROMWINEVECTOR4(1,1,1,1));
		IRenderer::GetRendererInstance()->drawCircle(100,100,5.0f,D3DXFROMWINEVECTOR4(1,1,1,1));//,1.0f);

	renderframe(D3DXFROMWINEVECTOR2(100.0f,100.0f), D3DXFROMWINEVECTOR2(20.0f,20.0f), texture2);
	renderframe(D3DXFROMWINEVECTOR2(250.0f,250.0f), D3DXFROMWINEVECTOR2(30.0f,30.0f), texture3);
	renderframe(D3DXFROMWINEVECTOR2(400.0f,400.0f), D3DXFROMWINEVECTOR2(50.0f,50.0f), texture);

	render5(texture4, 150.0f,160.0f,50.0f,50.0f,0.0f, 0.0f,1.0f, 1.0f);
	render5(texture4, 140.0f,150.0f,10.0f,10.0f,0.86f, 1.0f,0.8f, 1.0f);

	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );
	return 0;
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "Blending");		
	IInput*    i=STX_Service::GetInputInstance();
	init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();	
		render();
	}
	return 0;
}

