#ifndef GUI_EVENT_H
#define GUI_EVENT_H

#include "../Tools/MathUtils.h"
#include "../Tools/NamedObject.h"

class GUIRectangle;
class GUIEvent
{
  public:
    GUIEvent();
    GUIEvent(GUIRectangle *element);
    GUIEvent(const GUIEvent& copy);
#ifndef SWIG
    GUIEvent &operator= (const GUIEvent& copy);
#endif
    GUIRectangle *getEventSource();
    const NSString &getCallbackString();
    const char* getCallbackCString();

    int getWidgetType();

  private:
    GUIRectangle *eventSource;
    NSString        callbackString;
};

class GUIFrame;
GUIEvent*  PollGUIEvent();
void PushGUIEvent(GUIEvent* evt);
bool GUIEventsEmpty();
void DisposeGUIEvent(GUIEvent* evt);

#endif
