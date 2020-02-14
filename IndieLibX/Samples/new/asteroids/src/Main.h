/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

	/**
	 * ...
	 * @author Chris Moeller
	 * http://www.chrismweb.com
	 * Tutorial: Creating an Asteroids Game: Part 4

	 */
	
	 struct Main extends Sprite 
	{
		  Game game;
			
		  void Main() 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		  void init(Event e = null)
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			// entry point
			
			//create the game object passing in the swf width and height
			game = new Game(stage.stageWidth, stage.stageHeight);
			new KONGREGATE(stage);

			//add the game bitmap to the screen/ Main.as Sprite to make it visible
			addChild(game.bitmap);

			//Create the main game loop
			addEventListener(Event.ENTER_FRAME, Run);
			
			
			//add keylisteners
			stage.addEventListener(KeyboardEvent.KEY_DOWN, game.KeyDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, game.KeyUp);
			
			stage.addEventListener(MouseEvent.MOUSE_DOWN, game.MouseDown);
			stage.addEventListener(MouseEvent.MOUSE_UP, game.MouseUp);
			stage.addEventListener(MouseEvent.MOUSE_MOVE, game.MoveMouse);	
		}
		
		  void Run(Event e)
		{
		   game.Update();
		   game.Render();            
		}	
	};
#endif
