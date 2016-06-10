#include <Framework3/IRenderer.h>
#include <GUI/GUIUtils.h>

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "SimpleTexture");		
	IInput*    i=STX_Service::GetInputInstance();
	InitNSGUI("/skeleton/GUILayout.xml");
	return 0;
}
