#include "SceneFrame.h"

SceneFrame* g_SceneFrame=0;
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "NatureScene");
	IInput*    i=STX_Service::GetInputInstance();

	SceneFrame scene;
	g_SceneFrame=&scene;
	if(!scene.initialize())
    		return -1;
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit() && scene.isActive())
	{
		if(i->OnKeyPress (KEY_ESCAPE) || i->Quit())
			scene.setActive(false);
		i->Update();
		#if 0
		static bool bRender=true;
		if(i->OnKeyPress (KEY_p))
			bRender=!bRender;
		
		if(!bRender)
			scene.run2();
		else
		#endif
			scene.run();
	}
	return 0;
}

