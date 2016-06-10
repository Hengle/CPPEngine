#include <CIndieLib.h>
float	mDisp = 0.0f;
	float	mSpeed = 0.5f;
	float	mDelta;
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
		GameLogic():IGameLogic(){}
		virtual ~GameLogic(){}
		IND_Animation mAnimationSun;
		IND_Entity2d mSun;
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
		"a_11_Animated_Tile_Scrolling"
		)) return -1;
#endif
	// ----- Animations loading -----

	// Sun animation
	////IND_Entity2d mSun;
	if (!mI->AnimationManager->AddToSurface (&mAnimationSun, "/animations/sun.xml", IND_OPAQUE, IND_32)) return 0;

	// ----- Set the surface and animations into 2d entities -----

	// Sun
	////IND_Entity2d mSun;
	mI->Entity2dManager->Add (&mSun);			// Entity adding
	mSun.SetAnimation (&mAnimationSun);			// Set the animation into the entity

	// ----- Changing the attributes of the 2d entities -----

	mSun.ToggleWrap (1);
	mSun.SetRegion (0, 0, 800, 600);

	// ----- Main Loop -----
	STX_CATCH;
	return 0;

	

	}
virtual int render()
{
	STX_TRY;
		// ----- Input update ----

		mI->Input->Update ();

		// ----- Updating entities attributes  -----

		mDelta = mI->Render->GetFrameTime() / 1000.0f;

		mDisp += mSpeed * mDelta;
		mSun.SetWrapDisplacement (mDisp, mDisp);

		// ----- Render  -----

		mI->Render->BeginScene ();
		mI->Entity2dManager->RenderEntities2d ();
		mI->Render->EndScene ();
		STX_CATCH;
		return 0;
	}

	// ----- Free -----

	};
GameLogic gl;
IGameLogic* glp=&gl;
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
