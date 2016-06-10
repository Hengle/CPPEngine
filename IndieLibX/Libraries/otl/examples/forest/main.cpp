#include <renderer/sdlframework.h>

#include "forestgamelet.h"

int ApplicationLogic()
{
  int argc0=0;
  char ** argv0=0;
  IRenderer* r=IRenderer::GetRendererInstance(640, 512, "Forest");
  IInput*    i=STX_Service::GetInputInstance();
  return SDL_FrameWork(new ForestGamelet()).run();
}


