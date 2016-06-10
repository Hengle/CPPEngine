/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
//package com.IndieLib.swig;

import com.IndieLib.swig.STX_Service;
import com.IndieLib.swig.IndieLib;
import com.IndieLib.swig.CIndieLib;

public class a_01_Installing {
   public static void main(String argv[]) {
     System.loadLibrary("IndieLib");
     init(0,0);
     while (true) {
	if(STX_Service.GetInputInstance().IsKeyDown(IndieLib.KEY_ESCAPE))
		return;
	render();
     }
   }
   public static boolean init(int width, int height) {
		CIndieLib.Instance();
		CIndieLib.Instance().Init("a_01_Installing");
   }

   public static int render() {
		STX_Service.GetInputInstance().Update ();
		CIndieLib.Instance().Render.ClearViewPort (60, 60, 60);
		CIndieLib.Instance().Render.BeginScene ();
		CIndieLib.Instance().Render.EndScene ();
		//IRenderer.GetRendererInstance().ShowFpsInWindowTitle();
		return 0;
   }
 }

