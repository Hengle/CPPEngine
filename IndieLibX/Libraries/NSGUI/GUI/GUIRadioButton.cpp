#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>
#include "GUIRadioButton.h"
#include "GUIPanel.h"

GUIRadioButton::GUIRadioButton(const char* cbs) : GUICheckBox(cbs)
{
  secondaryTexDesc = CHECK_RB_MARK;
  primaryTexDesc   = RADIO_BUTTON;
  widgetType       = RADIO_BUTTON;
  markRatio        = 0.75f;
}

void GUIRadioButton::checkMouseEvents(int extraInfo, bool bits)
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
	int c=((GUIPanel*)parent)->getCurrentElement();
	if(c!=i)
	{
		((GUIPanel*)parent)->setCurrentElement(i);
		setChecked(!checked);
		pressed = false;
    		clicked = false;
		((GUIPanel*)parent)->notify(this);
		//LOG_FNLN;
		//LOG_PRINT("callbackString=%s,%d,%d\n", callbackString.data, i, c);
	}
	return;
  }
}
