#include <CIndieLib.h>

/*
==================
Main
==================
*/


CIndieLib * mI = 0;
CIndieLib * g_pIndieLib = 0;
class GameLogic : public IGameLogic
{
	public:
		GameLogic():IGameLogic(){;}
		virtual ~GameLogic(){;}
		
		virtual int init(unsigned int width, unsigned int height)
		{
			STX_INIT_SEGVCATCH;
			STX_TRY;
			g_pIndieLib = mI = CIndieLib::Instance();
	// ----- IndieLib intialization -----

#if 0
	//CIndieLib * mI = CIndieLib::Instance();
	if (!CIndieLib::Instance()->Init ()) return 0;
#else
	
//CIndieLib * mI = CIndieLib::Instance();
	if (!CIndieLib::Instance()->
		Init (
		"a_01_Installing"
		)) return -1;
#endif
	// ----- Main Loop -----
	STX_CATCH;
	return 0;


	}
virtual int render()
{
	STX_TRY;
		// ----- Input Update ----

		mI->Input->Update ();

		// -------- Render -------

		mI->Render->ClearViewPort (60, 60, 60);
		mI->Render->BeginScene ();
		mI->Render->EndScene ();
		//mI->Render->ShowFpsInWindowTitle();
		STX_CATCH;
		return 0;
	}

	// ----- Indielib End -----

	};
GameLogic gl;
IGameLogic* glp=&gl;
#include <STX/STXCPP.h>
int init(unsigned int width, unsigned int height){return glp->init(0,0);}
int render(){return glp->render();}
int IndieLib()
{
	glp->init(0,0);
	while (!mI->Input->OnKeyPress (KEY_ESCAPE) && !mI->Input->Quit())
	{
		glp->render();
	}
	mI->End();


	return 0;
}

