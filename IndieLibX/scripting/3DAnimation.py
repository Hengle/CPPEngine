#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
import IndieLib

class AppPy:
	def __init__(self):
		return
	
	def init(self):
		IndieLib.InitNSGUI("/skeleton/GUILayout.xml")
		#self.filename = "/trex dx/dino videogame.x"
		#self.filename = "/trex dx/dino videogame2.x"
		self.filename = "/skeleton/skeleton.x"
		#self.filename = "/assimp--1.0.412-sdk/test.x"
		#self.filename = "/assimp--1.0.412-sdk/dwarf.x"
		#self.filename = "/PM/P-TankHistory/allied/t34_76/t34ds/T34-76.3DS"
		self.m_Mesh=IndieLib.FW3.GetInstance().GetMesh(self.filename)
		self.m_Mesh.SetPlay(False)
		self.m_Mesh.SetAnimIndex(0)
		self.m_Mesh.SetPlay(True)

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
					for i in range(1,6):
      						name='Skeleton_%d' % i
						if callbackString == name:
							print i

							self.m_Mesh.SetPlay(False)
							self.m_Mesh.SetAnimIndex(i)
							self.m_Mesh.SetPlay(True)
							break
	def render(self):
		IndieLib.IRenderer.GetRendererInstance().Clear(True, True, IndieLib.D3DXFROMWINEVECTOR4(0.35, 0.53, 0.7, 1.0))
		IndieLib.IRenderer.GetRendererInstance().BeginScene()
		self.actionPerformed()

		w=IndieLib.D3DXFROMWINEMATRIX()
		IndieLib.D3DXFROMWINEMatrixIdentity(w)
		self.m_Mesh.render(w)

		IndieLib.IRenderer.GetRendererInstance().EndScene()
		IndieLib.IRenderer.GetRendererInstance().Present()

if __name__=="__main__":
	app=AppPy()
	IndieLib.IRenderer.GetRendererInstance(640, 512, "3DAnimation")
	app.init()
	while ((not IndieLib.STX_Service.GetInputInstance().OnKeyPress (IndieLib.KEY_ESCAPE)) and (not IndieLib.STX_Service.GetInputInstance().Quit())):
		IndieLib.STX_Service.GetInputInstance().Update()	
		app.render()

