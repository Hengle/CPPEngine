#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
import IndieLib

class TestSpine:
	def __init__(self):

		self.name=["GoblinWalk","RaptorWalk","RaptorEmpty","RaptorGungrab","SpineBoyTest","SpineBoyWalk","SpineBoyJump","SpineBoyRun"]
		self.mouseLocked=False
		self.guiFrame
		self.userControls
		self.irender=7
		self.guiFrame=IndieLib.GUIFrame()
	def init(self, width,  height):
		initSpine2DAnimations()
  		IndieLib.InitNSGUI("/spine/GUILayout.xml")

	def actionPerformed(self):
		IndieLib.UpdateNSGUI()
		loop=not IndieLib.GUIEventsEmpty()
		while loop:
			evt=IndieLib.PollGUIEvent()
			loop=not IndieLib.GUIEventsEmpty()
			if evt:
				callbackString=evt.getCallbackCString()
				if callbackString=="Unknown":
					continue
				if IndieLib.isClicked(evt):
					self.irender=0
					for i in range(0,len(self.name)):
      						if callbackString == self.name[i]:
							self.irender=i
							break

	def render(self ):
		f=128.0
		IndieLib.IRenderer.GetRendererInstance().Clear(True, False, IndieLib.D3DXFROMWINEVECTOR4 (f/256.0, f/256.0, f/256.0,1))
		IndieLib.IRenderer.GetRendererInstance().BeginScene()
		self.actionPerformed()
		renderSpine2DAnimations (self.name[self.irender])

		IndieLib.IRenderer.GetRendererInstance().EndScene()
		IndieLib.IRenderer.GetRendererInstance().Present( )

if __name__=="__main__":
	app=TestSpine()
	app.init(640, 512)
	IndieLib.IRenderer.GetRendererInstance(640, 512, "testspine")		
	while ((not IndieLib.STX_Service.GetInputInstance().OnKeyPress (IndieLib.KEY_ESCAPE)) and (not IndieLib.STX_Service.GetInputInstance().Quit())):
		IndieLib.STX_Service.GetInputInstance().Update()	
		app.render()

