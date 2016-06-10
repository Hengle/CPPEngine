#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "GUIAlphaElement.h"

class GUILabel : public GUIAlphaElement
{
  public:
    GUILabel(const char   *labelString    = 0,
             const char   *callbackString = 0);

    virtual void  render(float clockTick);
    virtual bool  loadXMLSettings(XMLElement *node);
    virtual const Tuple4i &getWindowBounds();
};

#endif
