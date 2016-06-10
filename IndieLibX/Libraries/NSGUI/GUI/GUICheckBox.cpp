#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>
#include "GUICheckBox.h"
#include "GUIFrame.h"

GUICheckBox::GUICheckBox(const char *cbs) : GUIAlphaElement(cbs)
{
  setMinAlphaMark(0.0f);
  setDimensions(20, 20);
  setAlphaMark(1.0f);
  setBGColor(0.8f, 0.8f, 0.8f);
  setChecked(false);
  widgetType = CHECK_BOX;

  secondaryTexDesc = CHECK_BOX_MARK;
  primaryTexDesc   = CHECK_BOX;
  markOffset       = 0;
  markRatio        = 0.85f;
  extra            = 0;
}

bool GUICheckBox::loadXMLSettings(XMLElement *element)
{
  if(widgetType == CHECK_BOX)
  if(!element || element->getName() != "CheckBox")
    {LOG_PRINT("Need a CheckBox node in the xml file");return false;}

  if(widgetType == RADIO_BUTTON)
  if(!element || element->getName() != "RadioButton")
    {LOG_PRINT("Need a RadioButton node in the xml file");return false;}

  XMLElement *child    = 0,
             *subChild = 0;

	

  if(child = element->getChildByName("checked"))
    setChecked((child->getValue() == "true"));

  if(child = element->getChildByName("BGColor"))
    XMLElement::loadRX_GY_BZf(*child, bgColor);

  setBGColor(bgColor);
  return GUIAlphaElement::loadXMLSettings(element);
}

void  GUICheckBox::setMinAlphaMark(float mark)
{
  minAlphaMark = clampNS(mark, 0.0f, 1.0f);
}

float  GUICheckBox::getMinAlphaMark()
{
  return minAlphaMark;
}

void  GUICheckBox::setAlphaMark(float mark)
{
  alphaMark = clampNS(mark, minAlphaMark, 1.0f);
}

float  GUICheckBox::getAlphaMark()
{
  return alphaMark;
}

void  GUICheckBox::setChecked(bool enabledArg)
{
  if(active)
    checked = enabledArg;
}

bool  GUICheckBox::isChecked()
{
  return checked;
}

void  GUICheckBox::setBGColor(const D3DXFROMWINEVECTOR3& color)
{
  setBGColor(color.x, color.y, color.z);
}

void  GUICheckBox::setBGColor(float x, float y, float z)
{
  bgColor=D3DXFROMWINEVECTOR3(clampNS(x, 0.0f, 1.0f),
              clampNS(y, 0.0f, 1.0f),
              clampNS(z, 0.0f, 1.0f));
}

const D3DXFROMWINEVECTOR3 &GUICheckBox::getBGColor()
{
  return bgColor;
}

void GUICheckBox::render(float clockTick)
{
  if(!parent || !visible)
    return;

  modifyCurrentAlpha(clockTick);
  getCenter();

  GUITexCoordDescriptor *descriptor     = parent->getTexCoordsInfo(primaryTexDesc),
                        *descriptorCB   = parent->getTexCoordsInfo(secondaryTexDesc);

  int yCenter = (windowBounds.y + windowBounds.w)/2;

	D3DXFROMWINEVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	// Radio button
	if(widgetType == RADIO_BUTTON)
	{
		float xPos = windowBounds.x - 9;
		float yTop = windowBounds.y - 9;
		IRenderer::GetRendererInstance()->drawCircle(xPos + 17, yTop + 20, 7, col, 2);
		if(isChecked())
			IRenderer::GetRendererInstance()->drawCircle(xPos + 17, yTop + 20, 4, col);
	} else {
		// Checkbox
		float xPos = windowBounds.x - 9;
		float yTop = windowBounds.y - 9;
		IRenderer::GetRendererInstance()->drawRect(xPos + 8, yTop + 11, xPos + 24, yTop + 28, col, 2);
		if(isChecked())
		{
			// Check
			IRenderer::GetRendererInstance()->drawLine(xPos + 11, yTop + 18, xPos + 15, yTop + 25, col, 2);
			IRenderer::GetRendererInstance()->drawLine(xPos + 15, yTop + 25, xPos + 21, yTop + 14, col, 2);
		}
	}

  //glDisable(GL_BLEND);
  label.printCenteredY(windowBounds.x + extra + 2, yCenter);
}

const Tuple4i &GUICheckBox::getWindowBounds()
{
  if(parent && update)
  {
    label.computeDimensions();
    extra = int(float(label.getHeight())*1.25);
    dimensions=D3DXFROMWINEVECTOR2(float(label.getWidth() + extra), float(extra));
    markOffset = extra - int(dimensions.y*markRatio);
    GUIRectangle::computeWindowBounds();
  }
  return windowBounds;
}
void GUICheckBox::addItem(const NSString &item)
{

  for(size_t t = 0; t < items.size(); t++)
    if(items[t] == item)
      return;

  items.push_back(item);
}
int  GUICheckBox::getItemIndex(const NSString &item)
{
  for(size_t t = 0; t < items.size(); t++)
    if(items[t] == item)
      return int(t);

  return -1;
}
void GUICheckBox::checkMouseEvents(int extraInfo, bool bits)
{
  GUIRectangle::checkMouseEvents(extraInfo, true);
  if(!mouseOver)
	return;

  if(GUIRectangle::isPressed() || GUIRectangle::isClicked()) 
  {
	GUIEvent e(this);
	GUIRectangle     *sourceRectangle = e.getEventSource();
	NSString callbackString=sourceRectangle->getCallbackString();
	int i=((GUIPanel*)parent)->getWidgetIndexByCallbackString(callbackString);
	
	DWORD t1=((GUIPanel*)parent)->getTime();
	DWORD t2=timeGetTime();
	if((t2-t1)<1000)
		return;

	((GUIPanel*)parent)->setTime(t2);
	((GUIPanel*)parent)->setCurrentElement(i);
	
	setChecked(!checked);
	pressed = false;
    	clicked = false;
	//((GUIPanel*)parent)->notify(this);
	//LOG_FNLN;
	//LOG_PRINT("callbackString=%s,%d,%d\n", callbackString.data, i, c);
	return;
  }
}
