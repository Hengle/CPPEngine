#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>
#include "GUISlider.h"
#include "GUIFrame.h"
GUISlider::GUISlider(const char *callback,
                     const int   orientation_) :
                     GUIAlphaElement(callback)
{
  m_Translation=D3DXFROMWINEVECTOR3(0,0,0);
  setDiscDimensions(18, 18);
  setProgress(0.0);
  orientation = orientation_ == VERTICAL ? VERTICAL : HORIZONTAL;
  widgetType  = SLIDER;
  offset      = 0;

  setDimensions((orientation_ == VERTICAL) ?  18.0f : 100.0f,
                (orientation_ == VERTICAL) ?  85.0f :  18.0f);
}

bool GUISlider::loadXMLSettings(XMLElement *element)
{
  if(!element || element->getName() != "Slider")
    {LOG_PRINT("Need a Slider node in the XML File");
  return false;}

  XMLElement *child    = 0,
             *subChild = 0;

  if(child = element->getChildByName("orientation"))
    orientation = (child->getValue() == "VERTICAL") ? VERTICAL : HORIZONTAL;

  if(child = element->getChildByName("progress"))
    setProgress(child->getValuef());

  if(child = element->getChildByName("discWidth"))
    discDimensions.x = child->getValuei();

  if(child = element->getChildByName("discHeight"))
    discDimensions.y = child->getValuei();

  setDimensions((orientation == VERTICAL) ? 18.0f : 100.0f,
                (orientation == VERTICAL) ? 85.0f :  18.0f);
  setDiscDimensions(discDimensions);
  return   GUIAlphaElement::loadXMLSettings(element);
}

void  GUISlider::setDiscDimensions(const Tuple2i& dimensions)
{
  setDiscDimensions(dimensions.x, dimensions.y);
}

void  GUISlider::setDiscDimensions(int width, int height)
{
  discDimensions.set(clampNS(width, 5, 500), clampNS(height, 5, 500));
  setDimensions((orientation == HORIZONTAL) ? dimensions.x            : float(discDimensions.x),
                (orientation == HORIZONTAL) ? float(discDimensions.y) : dimensions.y);
}

const Tuple2i &GUISlider::getDiscDimensions()
{
  return discDimensions;
}

void  GUISlider::setProgress(float zeroToOne)
{
  if(!STX_Service::GetInputInstance()->IsMouseButtonPressed())
    progress = clampNS(zeroToOne, 0.0f, 1.0f);
}

float GUISlider::getProgress()
{
  return progress;
}

void GUISlider::render(float clockTick)
{
  if(!parent || !visible)
    return;

  modifyCurrentAlpha(clockTick);

  if(orientation == VERTICAL)
  {
    renderVertical();
    return;
  }

  GUITexCoordDescriptor *descriptor    = parent->getTexCoordsInfo(SLIDER);
  Tuple2i discXBounds,
          center     = getCenter();


  discXBounds.x  = windowBounds.x;
  discXBounds.x += int(float(windowBounds.z - windowBounds.x)*progress);
  discXBounds.x -= discDimensions.x/2;
  discXBounds.y  = discXBounds.x + discDimensions.x;

  float off=float(offset);
  //IRenderer::GetRendererInstance()->SetTranslation
  m_Translation=D3DXFROMWINEVECTOR3(float(offset), 0, 0);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  IRenderer::GetRendererInstance()->Color4fv(color);

  IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, center.y - 1+m_Translation.y);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, center.y + 1+m_Translation.y);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, center.y - 1+m_Translation.y);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, center.y + 1+m_Translation.y);
#else
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, center.y + 1);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, center.y - 1);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, center.y + 1);
  IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, center.y - 1);
#endif
  IRenderer::GetRendererInstance()->End();

  if(descriptor)
  {
    const D3DXFROMWINEVECTOR4 &texCoords = descriptor->getTexCoords();
    parent->enableGUITexture();

    IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, 1.0f-texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x+m_Translation.x, windowBounds.y+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, 1.0f-texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x+m_Translation.x, windowBounds.w+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, 1.0f-texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y+m_Translation.x, windowBounds.y+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, 1.0f-texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y+m_Translation.x, windowBounds.w+m_Translation.y);

    //LOG_PRINT("texCoords.x=%f\n", texCoords.x);
    //LOG_PRINT("texCoords.y=%f\n", texCoords.y);
    //LOG_PRINT("texCoords.z=%f\n", texCoords.z);
    //LOG_PRINT("texCoords.w=%f\n", texCoords.w);

#else
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x, windowBounds.w);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x, windowBounds.y);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y, windowBounds.w);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y, windowBounds.y);
#endif
    IRenderer::GetRendererInstance()->End(GUIFrame::elementsTexture);
  }
  else
  {
    IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x+m_Translation.x, windowBounds.y+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x+m_Translation.x, windowBounds.w+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y+m_Translation.x, windowBounds.y+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y+m_Translation.x, windowBounds.w+m_Translation.y);
#else
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x, windowBounds.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.x, windowBounds.y);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y, windowBounds.w);
    IRenderer::GetRendererInstance()->Vertex2i(discXBounds.y, windowBounds.y);
#endif
    IRenderer::GetRendererInstance()->End();
  }
  //glDisable(GL_BLEND);
  //IRenderer::GetRendererInstance()->SetTranslation
  m_Translation=D3DXFROMWINEVECTOR3(float(-offset), 0, 0);
  label.printCenteredX(center.x + offset+m_Translation.x, windowBounds.w+m_Translation.y);
}

void    GUISlider::renderVertical()
{
  GUITexCoordDescriptor *descriptor    = parent->getTexCoordsInfo(SLIDER);
  Tuple2i discYBounds,
          center     = getCenter();

  discYBounds.x  = windowBounds.w;
  discYBounds.x -= int(float(getHeight())*progress);
  discYBounds.x -= discDimensions.y/2;
  discYBounds.y  = discYBounds.x + discDimensions.y;

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  IRenderer::GetRendererInstance()->Color4fv(color);

  float off=float(+offset);
  //IRenderer::GetRendererInstance()->SetTranslation
  m_Translation=D3DXFROMWINEVECTOR3(0, float(+offset), 0);

  IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
  IRenderer::GetRendererInstance()->Vertex2i(center.x - 1+m_Translation.x, windowBounds.y+m_Translation.y );
  IRenderer::GetRendererInstance()->Vertex2i(center.x - 1+m_Translation.x, windowBounds.w+m_Translation.y);
  IRenderer::GetRendererInstance()->Vertex2i(center.x + 1+m_Translation.x, windowBounds.y+m_Translation.y);
  IRenderer::GetRendererInstance()->Vertex2i(center.x + 1+m_Translation.x, windowBounds.w+m_Translation.y);
#else
  IRenderer::GetRendererInstance()->Vertex2i(center.x - 1, windowBounds.w);
  IRenderer::GetRendererInstance()->Vertex2i(center.x - 1, windowBounds.y );
  IRenderer::GetRendererInstance()->Vertex2i(center.x + 1, windowBounds.w);
  IRenderer::GetRendererInstance()->Vertex2i(center.x + 1, windowBounds.y);
#endif
  IRenderer::GetRendererInstance()->End();

  if(descriptor)
  {
    const D3DXFROMWINEVECTOR4 &texCoords = descriptor->getTexCoords();

    IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, 1.0f-texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, discYBounds.x+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, 1.0f-texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, discYBounds.y+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, 1.0f-texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, discYBounds.x+m_Translation.y);

    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, 1.0f-texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, discYBounds.y+m_Translation.y);

    //LOG_PRINT("texCoords.x=%f\n", texCoords.x);
    //LOG_PRINT("texCoords.y=%f\n", texCoords.y);
    //LOG_PRINT("texCoords.z=%f\n", texCoords.z);
    //LOG_PRINT("texCoords.w=%f\n", texCoords.w);

#else
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, discYBounds.y);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.x, texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, discYBounds.x);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, texCoords.w);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, discYBounds.y);
    IRenderer::GetRendererInstance()->TexCoord2f(texCoords.z, texCoords.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, discYBounds.x);
#endif
    IRenderer::GetRendererInstance()->End(GUIFrame::elementsTexture);
  }
  else
  {
    IRenderer::GetRendererInstance()->Begin(PRIM_TRIANGLE_STRIP);
#if 1
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, discYBounds.x+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x+m_Translation.x, discYBounds.y+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, discYBounds.x+m_Translation.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z+m_Translation.x, discYBounds.y+m_Translation.y);
#else
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, discYBounds.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.x, discYBounds.x);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, discYBounds.y);
    IRenderer::GetRendererInstance()->Vertex2i(windowBounds.z, discYBounds.x);
#endif
    IRenderer::GetRendererInstance()->End();
  }
  //glDisable(GL_BLEND);
  //IRenderer::GetRendererInstance()->SetTranslation
  m_Translation=D3DXFROMWINEVECTOR3(0, float(-offset), 0);
}

const Tuple4i &GUISlider::getWindowBounds()
{
  if(parent && update)
  {
    GUIRectangle::computeWindowBounds();
    label.computeDimensions();
    realWindowBounds    = windowBounds;

    if(orientation == HORIZONTAL)
    {
      int difference = label.getWidth() -  getWidth();

      realWindowBounds.x -= discDimensions.x/2;
      realWindowBounds.z += discDimensions.x/2;

      if(difference > 0)
      {
        difference /= 2;
        realWindowBounds.x -= difference;
        realWindowBounds.z += difference;
      }

      realWindowBounds.w += int(label.getHeight()/1.25f);
      offset = clampNS(windowBounds.x - realWindowBounds.x, 0, 500);
    }
    else
    {
      offset               = discDimensions.y/2;
      realWindowBounds.y  -= discDimensions.y/2;
      realWindowBounds.w  += discDimensions.y/2;
    }
  }
  return realWindowBounds;
}

void GUISlider::checkMouseEvents(int extraInfo, bool reservedBits)
{

  GUIEventListener *eventsListener = getEventsListener();
  bool nRelease = STX_Service::GetInputInstance()->IsMouseButtonReleased();

  windowBounds.x += (orientation == HORIZONTAL) ? offset : 0;
  windowBounds.y += (orientation != HORIZONTAL) ? offset : 0;
  windowBounds.z += (orientation == HORIZONTAL) ? offset : 0;
  windowBounds.w += (orientation != HORIZONTAL) ? offset : 0;

  GUIRectangle::checkMouseEvents(extraInfo, reservedBits);
  if(!mouseOver)
	return;

  if(GUIRectangle::isPressed() || GUIRectangle::isClicked()) 
  {
    progress = (orientation == HORIZONTAL) ?
                clampNS(float(STX_Service::GetInputInstance()->GetMouseX() - windowBounds.x)/ (windowBounds.z - windowBounds.x), 0.0f, 1.0f) :
                clampNS(1.0f - float(STX_Service::GetInputInstance()->GetMouseY() - windowBounds.y)/ (windowBounds.w - windowBounds.y), 0.0f, 1.0f);

    //if(eventsListener)
	{
      GUIEvent* event = new GUIEvent(this);
      if(eventsListener)
	eventsListener->actionPerformed(*event);
	else
      		PushGUIEvent(event);
    }
  }

  windowBounds.x -= (orientation == HORIZONTAL) ? offset : 0;
  windowBounds.y -= (orientation != HORIZONTAL) ? offset : 0;
  windowBounds.z -= (orientation == HORIZONTAL) ? offset : 0;
  windowBounds.w -= (orientation != HORIZONTAL) ? offset : 0;
}
