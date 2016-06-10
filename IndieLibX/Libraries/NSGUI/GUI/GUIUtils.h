#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <Framework3/IRenderer.h>
#include <STX/STXCPP.h>
#include <STX/CInput.h>

#include "../Tools/NSString.h"

#include "GUITexCoordDescriptor.h"
#include "GUIClippedRectangle.h"
#include "GUIMaterialSurface.h"
#include "GUITabbedPanel.h"
#include "GUIRadioButton.h"
#include "GUIFontManager.h"
#include "GUIRectangle.h"
#include "GUISeparator.h"
#include "GUICheckBox.h"
#include "GUIComboBox.h"
#include "GUITextBox.h"
#include "GUISlider.h"
#include "GUIButton.h"
#include "GUIPanel.h"
#include "GUIFrame.h"
#include "GUILabel.h"
#include "GUIText.h"
#include "GUIFont.h"

#include "GUIEventListener.h"

/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
	
*/
class STXGUI : public GUIEventListener
{
public:
	GUIFrame* GetGuiFrame();
	STXGUI();
	virtual ~STXGUI();
#ifndef SWIG
	static GUIFrame* guiFrame;
	static GUIPanel* userControls;
	bool mouseLocked;
#endif
	virtual void mouseScrolled     ();
	virtual void mouseExit         ();	
	virtual void update();
	virtual void init(const char* afile, const char* aUserControls="User Controls");
	virtual void actionPerformed(GUIEvent &evt)=0;
};

void InitNSGUI(const char* aGUILayout, const char* aUserControls="User Controls");
void UpdateNSGUI(const char* aUserControls="User Controls");
GUIEvent*  PollGUIEvent();
void PushGUIEvent(GUIEvent* evt);
bool GUIEventsEmpty();
void DisposeGUIEvent(GUIEvent* evt);
bool isClicked(GUIEvent &evt);
GUIRectangle* GetGUIRectangle(GUIEvent &evt);
#endif

