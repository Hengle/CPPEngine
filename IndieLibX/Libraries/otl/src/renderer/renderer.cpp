/*
  Copyright (c) 2004 Pascal Kirchdorfer, Seth Yastrov

  This software is provided 'as-is', without any express or implied warranty. In
  no event will the authors be held liable for any damages arising from the use
  of this software.

  Permission is granted to anyone to use this software for any purpose, including
  commercial applications, and to alter it and redistribute it freely, subject to
  the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim
       that you wrote the original software. If you use this software in a product,
       an acknowledgment in the product documentation would be appreciated but is
       not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/IRenderer.h>

#include "renderer/light.h"
#include "renderer/renderer.h"
#include "renderer/texture.h"
#include "renderer/triangles.h"
#include "renderer/vertices.h"
#include "renderer/tangents.h"

Vertex* Renderer::g_VertexPointer=0;//3
Vertex* Renderer::g_NormalPointer=0;//3
Vertex* Renderer::g_TexCoordPointer=0;//2
Tangent* Renderer::g_TexCoordPointert1=0;//3
Tangent* Renderer::g_TexCoordPointert2=0;//3
unsigned int Renderer::g_triangle_count=0;

unsigned int Renderer::g_vertex_count=0;
unsigned int Renderer::g_vertex_size=0;
BYTE* Renderer::g_vertex_array=0;

ShaderID Renderer::shd=-1;
VertexFormatID Renderer::vf=-1;

TextureID Renderer::depth=-1;
TextureID Renderer::color=-1;

D3DXFROMWINEMATRIX Renderer::g_modelViewProjMatrix;
D3DXFROMWINEMATRIX Renderer::g_modelViewMatrix;
D3DXFROMWINEMATRIX Renderer::g_modelViewMatrixI;
D3DXFROMWINEMATRIX Renderer::g_modelViewMatrixIT;

D3DXFROMWINEVECTOR4 Renderer::g_lightPosition;
D3DXFROMWINEVECTOR4 Renderer::g_lightDiffuse;
D3DXFROMWINEVECTOR4 Renderer::g_lightSpecular;
D3DXFROMWINEVECTOR4 Renderer::g_lightAmbiente;

Renderer::Renderer()
: hasInited(false), last(0), fps(0), wireframe(false), textured(true),
  normals(false), points(false)
{
  D3DXFROMWINEMatrixIdentity(&g_modelViewProjMatrix);
  D3DXFROMWINEMatrixIdentity(&g_modelViewMatrix);
  D3DXFROMWINEMatrixIdentity(&g_modelViewMatrixI);
  D3DXFROMWINEMatrixIdentity(&g_modelViewMatrixIT);

  g_lightPosition=D3DXFROMWINEVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
  g_lightDiffuse=D3DXFROMWINEVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
  g_lightSpecular=D3DXFROMWINEVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
  g_lightAmbiente=D3DXFROMWINEVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}

Renderer::~Renderer()
{
  //if (hasInited)
    //SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Renderer::init()
{
  hasInited = true;
/*
  int width=IRenderer::GetRendererInstance()->GetViewportWidth();
  int height=IRenderer::GetRendererInstance()->GetViewportHeight();

  color = IRenderer::GetRendererInstance()->addRenderTarget(width, height, FORMAT_RGBA8);
  depth = IRenderer::GetRendererInstance()->addRenderDepthStencil(width, height, 16);
*/
}

void Renderer::getViewport(int& x, int& y, int& w, int& h)
{
  h=IRenderer::GetRendererInstance()->GetViewportWidth();//???
  w=IRenderer::GetRendererInstance()->GetViewportHeight();//???
  x=IRenderer::GetRendererInstance()->GetViewportX();//???
  y=IRenderer::GetRendererInstance()->GetViewportY();//???
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->GetViewport\n");
}

void Renderer::setViewport(int x, int y, int w, int h)
{
  IRenderer::GetRendererInstance()->setViewport(x,y,w,h);
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->setViewport\n");
}

void Renderer::drawMesh(unsigned num_triangles, const Triangle* triangles)
{
  LOG_FNLN;
  LOG_PRINT("num_triangles=%d\n", num_triangles);
  LOG_PRINT("triangles=%lld\n", triangles);
  LOG_PRINT("g_vertex_count=%d\n", g_vertex_count);
  LOG_PRINT("g_vertex_array=%lld\n", g_vertex_array);
  LOG_PRINT("g_vertex_size=%d\n", g_vertex_size);
  
	if(!g_vertex_count) return;
	if(!num_triangles) return;
	if(!triangles) return;
	if(!g_vertex_array) return;
	if(!g_vertex_size) return;

    LOG_FNLN;
  if (textured)
  {
    //glDepthFunc( GL_LEQUAL );
    //glPolygonMode(GL_FRONT, GL_FILL);
    //glDrawElements(GL_TRIANGLES, num_triangles*3, GL_UNSIGNED_INT, triangles);
    IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(
		PRIM_TRIANGLES, 0,g_vertex_count,num_triangles,
		triangles, triangles,
		4,
		g_vertex_array, g_vertex_array, g_vertex_size);
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP textured\n");
  }

  if (wireframe)
  {
    IRenderer::GetRendererInstance()->Color3f(0.5f,0.4f,0.1f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glDrawElements(GL_TRIANGLES, num_triangles*3, GL_UNSIGNED_INT, triangles);
    IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(
		PRIM_TRIANGLES, 0,g_vertex_count,num_triangles,
		triangles, triangles,
		4,
		g_vertex_array, g_vertex_array, g_vertex_size);
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP wireframe\n");
  }

  if (points)
  {
    IRenderer::GetRendererInstance()->Color3f(0.5f,0.4f,0.1f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    //glDrawElements(GL_TRIANGLES, num_triangles*3, GL_UNSIGNED_INT, triangles);
    IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(
		PRIM_TRIANGLES, 0,g_vertex_count,num_triangles,
		triangles, triangles,
		4,
		g_vertex_array, g_vertex_array, g_vertex_size);
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP points\n");
  }

  //for (GLenum x = glGetError(); x != 0; x=glGetError())
    //printf("%s\n",(const char*)glGetString(x));
}

void Renderer::setupArray(const Vertex* avertices, unsigned int avertices_count, const Tangent* atangents, unsigned int atangents_count)
{

  LOG_FNLN;  
  LOG_PRINT("vertices=%lld\n", avertices);  
  LOG_PRINT("vertices_count=%d\n", avertices_count);
  LOG_PRINT("tangents=%lld\n", atangents);  
  LOG_PRINT("tangents_count=%d\n", atangents_count);

  if (!avertices)
  {
    	g_VertexPointer=0;
    	g_NormalPointer=0;
    	g_TexCoordPointer=0;
	return;
  }
  if (!atangents)
  {
      g_TexCoordPointert1=0;
      g_TexCoordPointert2=0;
  }

  if (avertices_count);
	g_vertex_count=avertices_count;

  if (avertices != vertices)
  {
    vertices = (Vertex*)avertices;
    g_VertexPointer=(Vertex*)&avertices[0].x;
    g_NormalPointer=(Vertex*)&avertices[0].nx;
    g_TexCoordPointer=(Vertex*)&avertices[0].u;

  }
  if (atangents != tangents)
    {
    tangents=(Tangent*)atangents;
    if (atangents)
    {
    g_TexCoordPointert1=(Tangent*)&atangents[0].nsx;
    g_TexCoordPointert2=(Tangent*)&atangents[0].ntx;
    }
  else
  {
    g_TexCoordPointert1=0;
    g_TexCoordPointert2=0;
  }
  }
  LOG_FNLN;
  bool useHalf=false;
	if(atangents && atangents_count)
	{
  LOG_FNLN;
		
		if(shd==-1) 
		{
  LOG_FNLN;
			
  			LOG_FNLN;
  			LOG_PRINT("IRenderer::GetRendererInstance()->addShaderFromFile\n");
  			shd = IRenderer::GetRendererInstance()->addShaderFromFile("/otldata/shaders/parallax.shd", "main", "main");
		}
		g_vertex_size=sizeof(Vertex)+sizeof(Tangent);
		LOG_FNLN;
  		LOG_PRINT("g_vertex_size=%d\n", g_vertex_size);
		if(vf==-1)
		{
  LOG_FNLN;
		FormatDesc fmt[] = {
		0, TYPE_VERTEX, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_NORMAL, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, useHalf? FORMAT_HALF : FORMAT_FLOAT, 2,
		0, TYPE_TANGENT, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_BINORMAL, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3
		};
		
  		LOG_FNLN;
  		LOG_PRINT("IRenderer::GetRendererInstance()->addVertexFormat\n");
  		vf = IRenderer::GetRendererInstance()->addVertexFormat(fmt, elementsOf(fmt), shd);
		IRenderer::GetRendererInstance()->setShader(shd);
  		IRenderer::GetRendererInstance()->setVertexFormat(vf);
		}
	}
	else
	{
  LOG_FNLN;
		if(shd==-1) 
		{
			LOG_FNLN;
  			LOG_PRINT("IRenderer::GetRendererInstance()->addShaderFromFile\n");
  			shd = IRenderer::GetRendererInstance()->addShaderFromFile("/otldata/shaders/normals.shd", "main", "main");
		}
		g_vertex_size=sizeof(Vertex);
		LOG_FNLN;
  		LOG_PRINT("g_vertex_size=%d\n", g_vertex_size);
		if(vf==-1)
		{
  LOG_FNLN;
		FormatDesc fmt[] = {
		0, TYPE_VERTEX, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_NORMAL, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, useHalf? FORMAT_HALF : FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, useHalf? FORMAT_HALF : FORMAT_FLOAT, 2
		};
		
  		LOG_FNLN;
  		LOG_PRINT("IRenderer::GetRendererInstance()->addVertexFormat\n");
  		vf = IRenderer::GetRendererInstance()->addVertexFormat(fmt, elementsOf(fmt), shd);
		IRenderer::GetRendererInstance()->setShader(shd);
  		IRenderer::GetRendererInstance()->setVertexFormat(vf);
		}
	}

  LOG_FNLN;
	if(g_vertex_array)
	{
		delete[] g_vertex_array;
		g_vertex_array=0;
	}

  LOG_FNLN;
	
	{

  LOG_FNLN;

	if((g_VertexPointer && g_NormalPointer && g_TexCoordPointer && g_TexCoordPointert1 && g_TexCoordPointert2) || (g_VertexPointer && g_NormalPointer && g_TexCoordPointer))
		g_vertex_array=new BYTE[g_vertex_size*g_vertex_count];

  LOG_FNLN;

	if(g_VertexPointer && g_NormalPointer && g_TexCoordPointer && g_TexCoordPointert1 && g_TexCoordPointert2)
	{
  LOG_FNLN;
	for(unsigned int i=0;i<g_vertex_count;i++)
	{
memcpy(g_vertex_array+i*g_vertex_size,g_VertexPointer+i,sizeof(float3));
memcpy(g_vertex_array+i*g_vertex_size+sizeof(float3),g_NormalPointer+i,sizeof(float3));
memcpy(g_vertex_array+i*g_vertex_size+2*sizeof(float3),g_TexCoordPointer+i,sizeof(float2));
memcpy(g_vertex_array+i*g_vertex_size+2*sizeof(float3)+sizeof(float2),g_TexCoordPointert1+i,sizeof(float3));
memcpy(g_vertex_array+i*g_vertex_size+3*sizeof(float3)+sizeof(float2),g_TexCoordPointert2+i,sizeof(float3));
	}
	}
	else if(g_VertexPointer && g_NormalPointer && g_TexCoordPointer)
	{  
LOG_FNLN;
	for(unsigned int i=0;i<g_vertex_count;i++)
	{
memcpy(g_vertex_array+i*g_vertex_size,g_VertexPointer+i,sizeof(float3));
memcpy(g_vertex_array+i*g_vertex_size+sizeof(float3),g_NormalPointer+i,sizeof(float3));
memcpy(g_vertex_array+i*g_vertex_size+2*sizeof(float3),g_TexCoordPointer+i,sizeof(float2));
	}
	}
	}
}

void Renderer::setLight(Light* light, unsigned int number)
{
  
#if 0
  if (light)
  {
    //Enable Light
    glEnable( GL_LIGHT0 + number );

    //Setup The Ambient Light
    glLightfv( GL_LIGHT0 + number, GL_AMBIENT, light->getAmbiente() );

    //Setup The Diffuse Light
    glLightfv( GL_LIGHT0 + number, GL_DIFFUSE, light->getDiffuse() );

    //Setup The Diffuse Light
    glLightfv( GL_LIGHT0 + number, GL_SPECULAR, light->getSpecular() );

    //Position The Light
    glLightfv( GL_LIGHT0 + number, GL_POSITION, light->getPos() );
  }
  else
  {
    glDisable( GL_LIGHT0 + number );
  }
#endif
}

void Renderer::getLight(Light* light, unsigned int number)
{
  
#if 0
  if (light)
  {
    float ambiente[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glGetLightfv(GL_LIGHT0 + number, GL_AMBIENT, ambiente);
    light->setAmbiente(ambiente);

    float diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glGetLightfv(GL_LIGHT0 + number, GL_DIFFUSE, diffuse);
    light->setDiffuse(diffuse);

    float specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glGetLightfv(GL_LIGHT0 + number, GL_SPECULAR, specular);
    light->setSpecular(specular);

    float pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glGetLightfv(GL_LIGHT0 + number, GL_POSITION, pos);
    light->setPos(pos);
  }
#endif
}

void Renderer::setTexture(TextureOTL* tex, unsigned int layer)
{
  
#if 0
  glActiveTextureARB(GL_TEXTURE0_ARB+layer);
  glBindTexture(GL_TEXTURE_2D, tex->id);
#else

  LOG_FNLN;
  LOG_PRINT("Tex name=%s\n", tex->name2);
  LOG_PRINT("Tex id=%d\n", tex->id);

  LOG_PRINT("IRenderer::GetRendererInstance()->setTexture\n");
  IRenderer::GetRendererInstance()->setTexture(tex->name2, tex->id);
#endif
}

TextureOTL* Renderer::createTexture(unsigned x, unsigned y)
{
  TextureOTL* tex = new TextureOTL();
  tex->width = x;
  tex->height = y;
#if 0
  unsigned int txtnumber;

  glGenTextures(1, &txtnumber);
  glBindTexture(GL_TEXTURE_2D, txtnumber);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  tex->id = txtnumber;
#else
  
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->addTexture\n");
  tex->id = IRenderer::GetRendererInstance()->addTexture(x, y, 0, FORMAT_RGBA8 /* ??? */, IRenderer::GetRendererInstance()->Getlinear());
#endif
  return tex;
}

void Renderer::render2Texture(unsigned tex, unsigned x, unsigned y)
{
  
#if 0
	if ((color = renderer->addRenderTarget(width, height, FORMAT_RGBA8, FILTER_LINEAR, CLAMP | TRACK_WINDOW_SIZE)) == TEXTURE_NONE) return false;
	if ((depth = renderer->addRenderDepthStencil(width, height, 16, 0, TRACK_WINDOW_SIZE)) == TEXTURE_NONE) return false;


  glBindTexture(GL_TEXTURE_2D,tex);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, x, y, 0);
#endif
}

void Renderer::renderDepth2Texture(unsigned tex, unsigned x, unsigned y)
{
  
#if 0
  glBindTexture(GL_TEXTURE_2D,tex);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, x, y, 0);
#endif
}

void Renderer::clear(float r,float g,float b,float a)
{
  
#if 0
  glClearColor(r, g, b, a);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
#endif
  
  //LOG_PRINT("IRenderer::GetRendererInstance()=%lld\n", IRenderer::GetRendererInstance());
  IRenderer::GetRendererInstance()->Clear ( true, true, D3DXFROMWINEVECTOR4 (r, g, b, a));
  IRenderer::GetRendererInstance()->BeginScene();
  LOG_FNLN;
  LOG_PRINT("BeginScene\n");
  LOG_PRINT("shd=%d\n", shd);
  LOG_PRINT("VertexFormat=%d\n", vf);
  IRenderer::GetRendererInstance()->setShader(shd);
  LOG_FNLN;
  LOG_PRINT("IRenderer::GetRendererInstance()->setShader\n");
  IRenderer::GetRendererInstance()->setVertexFormat(vf);

  LOG_PRINT("IRenderer::GetRendererInstance()->setVertexFormat\n");
}

void Renderer::blit()
{
  
  //SDL_GL_SwapBuffers();
  IRenderer::GetRendererInstance()->EndScene();
  IRenderer::GetRendererInstance()->Present( );
  LOG_FNLN;  
  LOG_PRINT("IRenderer::GetRendererInstance()->EndScene\n");
  LOG_PRINT("IRenderer::GetRendererInstance()->Present\n");
/*
  Renderer::g_VertexPointer=0;//3
  Renderer::g_NormalPointer=0;//3
  Renderer::g_TexCoordPointer=0;//2
  Renderer::g_TexCoordPointert1=0;//3
  Renderer::g_TexCoordPointert2=0;//3
  Renderer::g_triangle_count=0;

  Renderer::g_vertex_count=0;
  Renderer::g_vertex_size=0;
  Renderer::g_vertex_array=0;
*/
  //Renderer::shd=-1;
  //Renderer::vf=-1;
}

void Renderer::enableLight(bool state)
{
  
#if 0
  if (state)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);
#endif
}

void Renderer::enableTransparency(bool state)
{
  
#if 0
  if (state) glEnable(GL_BLEND);
  else glDisable(GL_BLEND);
#endif
}

void Renderer::enableAlphaTest(bool state)
{
  
#if 0
  if (state) glEnable(GL_ALPHA_TEST);
  else glDisable(GL_ALPHA_TEST);
#endif
}

void Renderer::enablePointSprites(bool state)
{
  
#if 0
  if (state)
  {
    points = true;
    textured = false;
    glEnable(GL_POINT_SPRITE_ARB);
  }
  else 
  {
    points = false;
    textured = true;
    glDisable(GL_POINT_SPRITE_ARB);
  }
#endif
}

void Renderer::sleep(unsigned time)
{
  
  //SDL_Delay(time);
  STX_Service::GetTimerInstance()->stx_Delay(time, 0);
}

void Renderer::updateFPS(const char* name, unsigned vert, unsigned tria)
{
  
#if 0
  fps++;

  unsigned now = SDL_GetTicks();

  if( SDL_GetTicks() - last > 1000 )
  {
    //titel is limited to 256 (char length
    sprintf(title,"%s FPS: %.2f Vertices: %i Faces: %i", name, fps/(float)(now - last)*1000, vert, tria);
    SDL_WM_SetCaption(title,title);
    last = SDL_GetTicks();
    fps = 0;
  }
#endif
}

D3DXFROMWINEMATRIX g_P;
D3DXFROMWINEMATRIX g_V;
void Renderer::setPerspective(float fov, float ratio, float nearplane, float farplane)
{
  
#if 0
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov, ratio, nearplane, farplane);
#endif
  float fov2=fov*D3DXFROMWINE_PI/180.0f;
  LOG_FNLN;  
  LOG_PRINT("IRenderer::GetRendererInstance()->buildPerspectiveMatrix\n");
  //IRenderer::GetRendererInstance()->buildPerspectiveMatrix(fov, ratio, nearplane, farplane);
  LOG_PRINT("fov=%f\n", fov2);
  LOG_PRINT("ratio=%f\n", ratio);
  LOG_PRINT("nearplane=%f\n", nearplane);
  LOG_PRINT("farplane=%f\n", farplane);
  D3DXFROMWINEMatrixPerspectiveFovLH(&g_P, fov2, ratio, nearplane, farplane);
}

void Renderer::setCamera(opentree::otMatrix view)
{
  
#if 0
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLoadMatrixf(view);
#endif
  LOG_FNLN;  
  LOG_PRINT("IRenderer::GetRendererInstance()->setShaderConstant\n");
  //IRenderer::GetRendererInstance()->Setmv(*((D3DXFROMWINEMATRIX*)view.M));
  g_V=*((D3DXFROMWINEMATRIX*)view.M);

  D3DXFROMWINEMATRIX matProj=g_P;
  D3DXFROMWINEMATRIX matView=g_V;

  g_modelViewProjMatrix = (matView) * (matProj);
  g_modelViewMatrix = (matView);
  FLOAT determinant=0.0f;
  D3DXFROMWINEMatrixInverse(&g_modelViewMatrixI, &determinant, &g_modelViewMatrix);
  D3DXFROMWINEMatrixTranspose(&g_modelViewMatrixIT, &g_modelViewMatrixI);

  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewProj", g_modelViewProjMatrix);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelView", g_modelViewMatrix);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewI", g_modelViewMatrixI);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewIT", g_modelViewMatrixIT);

  IRenderer::GetRendererInstance()->setShaderConstant4f("LightPos", g_lightPosition);
  IRenderer::GetRendererInstance()->setShaderConstant4f("lightcolor", g_lightDiffuse);
  //IRenderer::GetRendererInstance()->setShaderConstant4x4f("LightAmbiente", g_lightAmbiente);
  //IRenderer::GetRendererInstance()->setShaderConstant4x4f("LightSpecular", g_lightSpecular);
}

TextureOTL* Renderer::loadTexture(const char* filename)
{
  
  TextureOTL* tex = new TextureOTL();

#if 0
  SDL_Surface *img = IMG_Load(filename);

  unsigned texture = 0;

  if(!img) 
  {
    return 0;
  }

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  tex->width = img->w;
  tex->height = img->h;

  if (img->format->BytesPerPixel == 3) // RGB 24bit
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
  }
  else if (img->format->BytesPerPixel == 4) // RGBA 32bit
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
  }
  else
  {
    SDL_FreeSurface(img);
    return 0;
  }

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  SDL_FreeSurface(img);

  tex->id = texture;
#else
  
  LOG_FNLN;  
  LOG_PRINT("IRenderer::GetRendererInstance()->addImageLibTexture\n");

/*
/otldata/textures/

bark_h.jpg  leaf.png             vertical_bark.jpg
bark.jpg    leaf_tria.png        vertical_bark_n.jpg
bark_n.jpg  vertical_bark_h.jpg  yellow_maple.png

*/

  tex->id = IRenderer::GetRendererInstance()->addImageLibTexture(
		filename
		,false,IRenderer::GetRendererInstance()->Getlinear()
		);
  tex->width=IRenderer::GetRendererInstance()->GetTextureWidth (tex->id);
  tex->height=IRenderer::GetRendererInstance()->GetTextureHeight(tex->id);

  if(str_ends_with(filename, "_n.jpg"))
	tex->setName2("TexNormal");
  else if(str_ends_with(filename, "_h.jpg"))
	tex->setName2("TexHeight");
  else
	tex->setName2("TexDiffuse");
#endif
  return tex;
}

