/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <stdio.h>
#include "renderer/renderer.h"
#include "renderer/shader_cg.h"
#include "renderer/light.h"

void ErrorCheck()
{
#if 0//def HAS_CG
  CGerror error = cgGetError();
  if (error)
    printf("CG Error: %s\n", cgGetErrorString(error));
#endif //HAS_CG
}

ShaderCG::ShaderCG()
{
  use_cg = true;
}

ShaderCG::~ShaderCG()
{
}

bool ShaderCG::init(const char* file, const char* name)
{
#if 0//def HAS_CG
  CGcontext context = cgCreateContext();

  profile_vertex = cgGLGetLatestProfile(CG_GL_VERTEX);;
  profile_fragment = cgGLGetLatestProfile(CG_GL_FRAGMENT);;

  cgGLSetOptimalOptions(profile_vertex);
  cgGLSetOptimalOptions(profile_fragment);

  program_vertex   = cgCreateProgramFromFile(context, CG_SOURCE, file_v, profile_vertex  , 0, 0);
  program_fragment = cgCreateProgramFromFile(context, CG_SOURCE, file_f, profile_fragment, 0, 0);

  if (!program_vertex || !program_fragment)
  {
    ErrorCheck();
    use_cg = false;
    printf("Shaders Disabled\n");
    return false;
  }

  Renderer::g_modelViewProjMatrix = cgGetNamedParameter(program_vertex, "ModelViewProj");
  Renderer::g_modelViewMatrix = cgGetNamedParameter(program_vertex, "ModelView");
  Renderer::g_modelViewMatrixI = cgGetNamedParameter(program_vertex, "ModelViewI");
  Renderer::g_modelViewMatrixIT = cgGetNamedParameter(program_vertex, "ModelViewIT");

  Renderer::g_lightPosition = cgGetNamedParameter(program_vertex, "LightPos");
  Renderer::g_lightDiffuse  = cgGetNamedParameter(program_fragment, "lightcolor");
  //Renderer::g_lightAmbiente = cgGetNamedParameter(program_vertex, "LightAmbiente");
  //Renderer::g_lightSpecular = cgGetNamedParameter(program_vertex, "LightSpecular");

  cgGLLoadProgram(program_vertex);
  cgGLLoadProgram(program_fragment);

  ErrorCheck();

  printf("Shaders Enabled\n");

  return true;
#elif 1
  m_name=name;
  id = IRenderer::GetRendererInstance()->addShaderFromFile(file, "main", "main");
  return true;
#else
  printf("Shaders Disabled\n");
  return false;
#endif // HAS_CG
}

void ShaderCG::preRender(Renderer* renderer)
{
#if 0//def HAS_CG
  if (!use_cg) return;

  cgGLEnableProfile(profile_vertex);
  cgGLEnableProfile(profile_fragment);
  cgGLBindProgram(program_vertex);
  cgGLBindProgram(program_fragment);

  if(Renderer::g_modelViewProjMatrix != NULL)
    cgGLSetStateMatrixParameter(Renderer::g_modelViewProjMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

  if(Renderer::g_modelViewMatrix != NULL)
    cgGLSetStateMatrixParameter(Renderer::g_modelViewMatrix, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);

  if(Renderer::g_modelViewMatrixI != NULL)
    cgGLSetStateMatrixParameter(Renderer::g_modelViewMatrixI, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);

  if(Renderer::g_modelViewMatrixIT != NULL)
    cgGLSetStateMatrixParameter(Renderer::g_modelViewMatrixIT, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);

  Light light;
  renderer->getLight(&light, 0);

  if(Renderer::g_lightDiffuse != NULL)
    cgGLSetParameter4fv(Renderer::g_lightDiffuse, light.getDiffuse());

  if(Renderer::g_lightPosition != NULL)
    cgGLSetParameter4fv(Renderer::g_lightPosition, light.getPos());

/*
  if(Renderer::g_lightAmbiente != NULL)
    cgGLSetParameter4fv(Renderer::g_lightAmbiente, light.getAmbiente());

  if(Renderer::g_lightSpecular != NULL)
    cgGLSetParameter4fv(Renderer::g_lightSpecular, light.getSpecular());
*/


  ErrorCheck();

  glDisable(GL_LIGHTING);
#else

  IRenderer::GetRendererInstance()->setShader(id);

  D3DXFROMWINEMATRIX matWorld=IRenderer::GetRendererInstance()->Getww();
  D3DXFROMWINEMATRIX matView=IRenderer::GetRendererInstance()->Getvv();
  D3DXFROMWINEMATRIX matProj=IRenderer::GetRendererInstance()->Getpp();
  D3DXFROMWINEMATRIX matVP = matWorld*(matView) * (matProj);

  Renderer::g_modelViewProjMatrix = matWorld*(matView) * (matProj);
  Renderer::g_modelViewMatrix = matWorld*(matView);
  FLOAT determinant=0.0f;
  D3DXFROMWINEMatrixInverse(&Renderer::g_modelViewMatrixI, &determinant, &Renderer::g_modelViewMatrix);
  D3DXFROMWINEMatrixTranspose(&Renderer::g_modelViewMatrixIT, &Renderer::g_modelViewMatrixI);

  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewProj", Renderer::g_modelViewProjMatrix);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelView", Renderer::g_modelViewMatrix);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewI", Renderer::g_modelViewMatrixI);
  IRenderer::GetRendererInstance()->setShaderConstant4x4f("ModelViewIT", Renderer::g_modelViewMatrixIT);

  IRenderer::GetRendererInstance()->setShaderConstant4f("LightPos", Renderer::g_lightPosition);
  IRenderer::GetRendererInstance()->setShaderConstant4f("lightcolor", Renderer::g_lightDiffuse);
  //IRenderer::GetRendererInstance()->setShaderConstant4x4f("LightAmbiente", Renderer::g_lightAmbiente);
  //IRenderer::GetRendererInstance()->setShaderConstant4x4f("LightSpecular", Renderer::g_lightSpecular);
#endif // HAS_CG
}

void ShaderCG::postRender()
{
#if 0//def HAS_CG
  if (!use_cg) return;

  glEnable(GL_LIGHTING);
  cgGLDisableProfile(profile_vertex);
  cgGLDisableProfile(profile_fragment);

  ErrorCheck();
#endif // HAS_CG
}
