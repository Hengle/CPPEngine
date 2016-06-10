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

#include <renderer/sdlframework.h>
#include <renderer/gamelet.h>

SDL_FrameWork::SDL_FrameWork(Gamelet* gamelet) : gamelet(gamelet)
{
  renderer.init();
  gamelet->setRenderer(&renderer);
  gamelet->init();
  running = false;
}

SDL_FrameWork::~SDL_FrameWork()
{
  delete gamelet;
}

IRenderer* r=0;
IInput*    i=0;
int SDL_FrameWork::run()
{
  /* Loop until an SDL_QUIT event is found */
  //while (!running)  
  while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
  {
    i->Update();
    processEvents();
    updateTree();
    drawFrame();
  }

  return 0;
}

void SDL_FrameWork::processEvents()
{
#if 1
  //SDL_Event event;
  
  /* Poll for events */
  //while (SDL_PollEvent(&event))
  {
    //switch (event.type)
    {
      //case SDL_MOUSEMOTION:
      {
        unsigned char button = 0;
        

	
        if(i->IsMouseMotion())
	if(i->IsMouseButtonPressed(MBUTTON_LEFT))
          button += 1;
        if(i->IsMouseMotion())
	if(i->IsMouseButtonPressed(MBUTTON_RIGHT))
          button += 2;
        if(i->IsMouseMotion())
	if(i->IsMouseButtonPressed(MBUTTON_MIDDLE))
          button += 4;
        
        int x = i->GetMouseX();
        int y = i->GetMouseY();

        gamelet->handleEvent(EV_MOUSE, x, y, button);

        //break;
      }
      //case SDL_KEYDOWN:
      if(i->IsKeyPressed())
      {
        gamelet->handleEvent(EV_KEY, i->getKeyCode());
        //break;
      }
      //case SDL_QUIT:
      if(i->Quit())
      {
        gamelet->handleEvent(EV_QUIT);
        running = 1;
        //break;
      }
    }
  }
#endif
}

void SDL_FrameWork::updateTree()
{
}

void SDL_FrameWork::drawFrame()
{
  gamelet->draw();
}

void SDL_FrameWork::quit()
{
#if 0
  SDL_Event ev;
  ev.type = SDL_QUIT;
  SDL_PushEvent(&ev);
#endif
}

#if 0//!defined(__APPLE__)
extern int OTL_main(int argc, char ** argv);
int ApplicationLogic()
{
  int argc0=0;
  char ** argv0=0;
  IRenderer* r=IRenderer::GetRendererInstance(640, 512, "Open Tree Library");
  IInput*    i=STX_Service::GetInputInstance();
  return OTL_main(argc0, argv0);
}
#endif

