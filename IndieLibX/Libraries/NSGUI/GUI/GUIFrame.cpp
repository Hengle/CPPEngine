#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>
#include "GUIFrame.h"
#include "GUIFontManager.h"

TextureID             GUIFrame::elementsTexture=-1;
GUIFrame::GUIFrame() : GUIPanel("GUI MAIN PANEL")
{
  listener = 0;
  update   = true;

  //elementsTexture=IRenderer::GetRendererInstance()->addImageLibTexture("/NSGUI/GUIElements.png",false,IRenderer::GetRendererInstance()->GetlinearClamp());

  Image3 img;
  img.loadImageLibImage("/NSGUI/GUIElements.png", false);
  //img.flipY();
  elementsTexture=IRenderer::GetRendererInstance()->addTexture(&img,false,IRenderer::GetRendererInstance()->GetlinearClamp());

}

bool GUIFrame::loadXMLSettings(XMLElement *element)
{

  if(!element || element->getName() != "Panel")
    {LOG_PRINT("Need a Panel node in the xml file");return false;}

  XMLElement *child       =  0,
             *subChild    =  0;
  bool        defaultFont = true;
#if 0
  if(child = element->getChildByName("Texture"))
  {
	NVTexture tex;    
		tex.loadXMLSettings(child);
		elementsTexture=tex.id;
    if(subChild = child->getChildByName("path"))
    {
      NSString path = child->getValue();
      {
	elementsTexture=IRenderer::GetRendererInstance()->addImageLibTexture(
		"/NSGUI/GUIElements.png"//path.data
		,false,IRenderer::GetRendererInstance()->GetlinearClamp()
		);
        GUITexCoordDescriptor::setTextureHeight(texture.getHeight());
        GUITexCoordDescriptor::setTextureWidth (texture.getWidth());
      }
    }
  }
#endif
  for(size_t i = 0; i < element->getChildrenCount(); i++)
  {
    if(!(child = element->getChild(i)))
      continue;

    const NSString &elementName = child->getName();
    if(elementName == "Font")
    {
      int fontIndex = GUIFontManager::addFont(child);
      if(defaultFont && fontIndex >= 0)
      {
        GUIFontManager::setDefaultFont(fontIndex);
        defaultFont = false;
      }
      continue;
    }

    if(elementName == "TexCoordsDesc")
    {
      GUITexCoordDescriptor descriptor;
      descriptor.loadXMLSettings(child);
      addOrReplaceTexCoordsInfo(descriptor);
    }
  }
  return   GUIPanel::loadXMLSettings(element);
}

void  GUIFrame::render(float tick)
{
  if(!visible)
    return;

  size_t t = 0;

  while(updateCount)
  {
    for(t = 0; t < elements.size(); t++)
      elements[t]->forceUpdate(true);
    updateCount--;
  }

  for(t = 0; t < elements.size(); t++)
    elements[t]->render(tick);
}

void GUIFrame::addOrReplaceTexCoordsInfo(GUITexCoordDescriptor &info)
{
  for(size_t t = 0; t < texCoords.size(); t++)
    if(texCoords[t].getType() == info.getType())
    {
      texCoords[t].setTexCoords(info.getTexCoords());
      return;
    }
  texCoords.push_back(info);
}

GUITexCoordDescriptor *GUIFrame::getTexCoordsInfo(int type)
{
  for(size_t t = 0; t < texCoords.size(); t++)
    if(texCoords[t].getType() == type)
      return &texCoords[t];
  return 0;
}

void  GUIFrame::setGUIEventListener(GUIEventListener *listener_)
{
  listener = listener_;
}

GUIEventListener *GUIFrame::getEventsListener()
{
  return listener;
}

void  GUIFrame::setElementsTexture(const char* texturePath)
{
	//elementsTexture=IRenderer::GetRendererInstance()->addImageLibTexture(texturePath,false, IRenderer::GetRendererInstance()->GetlinearClamp());
}

void  GUIFrame::setElementsTexture(const NVTexture &texture)
{
  elementsTexture = texture.id;
}
/*
NVTexture *GUIFrame::getElementsTexture()
{
  return elementsTexture==-1 ? 0 : &elementsTexture;
}
*/
void  GUIFrame::enableGUITexture()
{
  //elementsTexture.activate();
}

void  GUIFrame::disableGUITexture()
{
  //elementsTexture.deactivate();
}

void GUIFrame::forceUpdate(bool update)
{
  updateCount = update ? getTreeHeight() + 1 : 0;
}

void GUIFrame::clear()
{
  //elementsTexture.destroy();
  texCoords.clear();
  GUIPanel::clear();
}

const Tuple4i &GUIFrame::getWindowBounds()
{
  windowBounds.set(0, 0, int(dimensions.x), int(dimensions.y));
  return windowBounds;
}

GUIFrame::~GUIFrame()
{
  clear();
}
