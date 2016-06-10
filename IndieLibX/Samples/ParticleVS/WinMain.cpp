//define __MESHRENDERER__ 1
#include <Framework3/IRenderer.h>
#ifdef BUILD_MESHRENDERER
#include <assimp_view.h>
#endif

#include "Particles.h"

D3DXFROMWINEVECTOR3 g_vecChopper = D3DXFROMWINEVECTOR3(0.0f, 50.0f, 0.0f);

// Window class and caption text
char g_szClass[]   = "VSParticlesClass";
char g_szCaption[] = "Vertex Shader Particles Demo by Jim Adams";

// Global projection transformation
D3DXFROMWINEMATRIX g_matProj;

// Helicopter smoke, tree, and people particle emitter objects
cParticleEmitter* g_ChopperEmitter;
cParticleEmitter* g_TreeEmitter;
cParticleEmitter* g_PeopleEmitter;
#ifdef BUILD_MESHRENDERER
// Helicopter, rotor, shadow, and backdrop mesh objects
AssimpView::MeshRenderer* g_ChopperMesh =0;
AssimpView::MeshRenderer* g_RotorMesh   =0;
AssimpView::MeshRenderer* g_ShadowMesh  =0;
AssimpView::MeshRenderer* g_BackdropMesh=0;
#endif
// Helicopter's position and Y-axis rotation
float       g_rotChopper ;

int init(unsigned int width, unsigned int height)
{
	
#if 1
  // Get the projection transformation for use later
  IRenderer::GetRendererInstance()->GetTransform(TS_PROJECTION, g_matProj);

 g_ChopperEmitter=new cParticleEmitter();
	g_TreeEmitter=new cParticleEmitter();
	g_PeopleEmitter=new cParticleEmitter();
#ifdef BUILD_MESHRENDERER
	// Load the helicopter, rotor, and shadow meshes
  g_ChopperMesh=new AssimpView::MeshRenderer("/AdvAniCh12/chopper.X", "SimpleShader");
  g_RotorMesh=new AssimpView::MeshRenderer("/AdvAniCh12/Rotor.X", "SimpleShader");
  g_ShadowMesh=new AssimpView::MeshRenderer("/AdvAniCh12/Shadow.X", "SimpleShader");
  // Load the backdrop mesh
  g_BackdropMesh=new AssimpView::MeshRenderer("/AdvAniCh12/backdrop.x", "SimpleShader");
#endif
#endif
  // Create the particle emitters
  D3DXFROMWINEVECTOR3 O(0.0f, 0.0f, 0.0f);
  g_ChopperEmitter->Create(IRenderer::GetRendererInstance(), &O, EMITTER_CLOUD);
  g_TreeEmitter->Create( IRenderer::GetRendererInstance(),   &O, EMITTER_TREE);
  g_PeopleEmitter->Create(IRenderer::GetRendererInstance(),  &O, EMITTER_PEOPLE);

  // Populate the ground with trees and people
  DWORD Type = PARTICLE_TREE1;
  for(DWORD i=0;i<50;i++) {
    float rot  = (float)(stx_rand() % 628) / 100.0f;
    float dist = (float)(stx_rand() % 400);
    D3DXFROMWINEVECTOR3 o((float)cos(rot)*dist,25.0f,(float)sin(rot)*dist);
    D3DXFROMWINEVECTOR3 t(0.0f, 0.0f, 0.0f);

    g_TreeEmitter->Add(Type++,
                      &o,
                      50.0f, D3DXFROMWINEVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0, &t);
    if(Type > PARTICLE_TREE3)
      Type = PARTICLE_TREE1;

    rot  = (float)(stx_rand() % 628) / 100.0f;
    dist = (float)(stx_rand() % 400);
    D3DXFROMWINEVECTOR3 v1((float)cos(rot)*dist,7.5f,(float)sin(rot)*dist);
    D3DXFROMWINEVECTOR3 v2(0.0f, 0.0f, 0.0f);

    g_PeopleEmitter->Add(PARTICLE_PEOPLE1,
                      &v1,
                      15.0f, D3DXFROMWINEVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), 0, &v2);
  }
#if 0
  // Setup a light
  D3DLIGHT9 Light;
  ZeroMemory(&Light, sizeof(D3DLIGHT9));
  Light.Type = D3DLIGHT_DIRECTIONAL;
  Light.Diffuse.r = Light.Diffuse.g = Light.Diffuse.b = Light.Diffuse.a = 1.0f;
  Light.Direction = D3DXFROMWINEVECTOR3(0.0f, -1.0f, 0.0f);
  IRenderer::GetRendererInstance()->SetLight(0, &Light);
  IRenderer::GetRendererInstance()->LightEnable(0, TRUE);

  // Play a chopper sound
//  PlaySound("/AdvAniCh12/Chopper.wav", 0, SND_LOOP | SND_ASYNC);
#endif
  

	STX_Service::GetAudioInstance()->Init();
	STX_Service::GetAudioInstance()->Load("/AdvAniCh12/chopper.wav", "Chopper");

  return 0;
}

void DoShutdown()
{
	//
  // Stop the sound
  //PlaySound(0, 0, 0);

  // Release particle emitters
  g_ChopperEmitter->Free();
  g_TreeEmitter->Free();
  g_PeopleEmitter->Free();
#ifdef BUILD_MESHRENDERER
  // Release meshes
  if(g_BackdropMesh) delete g_BackdropMesh;  ;
  if(g_ShadowMesh) delete g_ShadowMesh;
  if(g_RotorMesh) delete g_RotorMesh;
  if(g_ChopperMesh) delete g_ChopperMesh;
#endif
//
}

int render()
{
	STX_Service::GetAudioInstance()->Play("Chopper");
	
  static DWORD LastTime = timeGetTime()/1000.0f;
  DWORD Time = timeGetTime()/1000.0f;
  DWORD Elapsed;
  D3DXFROMWINEMATRIX matWorld;
  static DWORD SmokeTimer = 0;

  // Get elapsed time and update last frame's time
  Elapsed = Time - LastTime;
  LastTime = Time;

  // Rotate and move the helicopter
  g_rotChopper = (float)Time / 5000.0f;
  g_vecChopper.x += (float)cos(g_rotChopper) * ((float)Elapsed / 30.0f);
  g_vecChopper.z -= (float)sin(g_rotChopper) * ((float)Elapsed / 30.0f);

  // Call special function that creates smoke under helicopter
  g_ChopperEmitter->HandleSmoke(&g_vecChopper, Elapsed);

  // Call special function that makes people duck when chopper is too close
  g_PeopleEmitter->HandleDucking(&g_vecChopper);

  // Update the helicopter's particle emitter
  g_ChopperEmitter->Process(Elapsed);

    D3DXFROMWINEMATRIX matProj;
    D3DXFROMWINEMatrixPerspectiveFovLH( &matProj, D3DXFROMWINEToRadian( 45.0f ),
                                //640.0f / 480.0f, 0.1f, 100.0f
								IRenderer::GetRendererInstance()->GetAspect(), 0.001f, 20000.0f
								);
    IRenderer::GetRendererInstance()->SetTransform( TS_PROJECTION, matProj );

  // Calculate a view transformation matrix
  D3DXFROMWINEMATRIX matView;
  D3DXFROMWINEVECTOR3 v1(g_vecChopper.x, 80.0f, g_vecChopper.z - 150.0f);
  D3DXFROMWINEVECTOR3 v2(g_vecChopper.x, 0.0f, g_vecChopper.z);
  D3DXFROMWINEVECTOR3 v3(0.0f, 1.0f, 0.0f);

  D3DXFROMWINEMatrixLookAtLH(&matView,
                     &v1,
                     &v2,
                     &v3);
  IRenderer::GetRendererInstance()->SetTransform(TS_VIEW, matView);

  // Clear the device and start drawing the scene
  IRenderer::GetRendererInstance()->Clear(true,true,D3DXFROMWINEVECTOR4(
	  //1,0,1,1
	  		0.35f, 0.53f, 0.7, 1.0f
	  ));//D3DXFROMWINEVECTOR4(0,0,64,255));
  IRenderer::GetRendererInstance()->BeginScene();

    // Turn on lighting
    //IRenderer::GetRendererInstance()->SetRenderState(D3DRS_LIGHTING, TRUE);
#ifdef BUILD_MESHRENDERER
    // Draw the backdrop mesh at origin
    D3DXFROMWINEMatrixIdentity(&matWorld);
    //IRenderer::GetRendererInstance()->SetTransform(TS_WORLD, matWorld);
    if(g_BackdropMesh) g_BackdropMesh->render(matWorld);//0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f);

    // Draw the helicopter's shadow
    D3DXFROMWINEMatrixTranslation(&matWorld, g_vecChopper.x, 0.5f, g_vecChopper.z);
    //IRenderer::GetRendererInstance()->SetTransform(TS_WORLD, matWorld);
    if(g_ShadowMesh) g_ShadowMesh->render(matWorld);//g_vecChopper.x, 0.5f, g_vecChopper.z,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f);

    // Draw the helicopter
    D3DXFROMWINEMatrixRotationYawPitchRoll(&matWorld, g_rotChopper, 0.0f, -0.0872222f);
    matWorld.m[3][0] = g_vecChopper.x;  // Set position manually in matrix
    matWorld.m[3][1] = g_vecChopper.y;
    matWorld.m[3][2] = g_vecChopper.z;
    //IRenderer::GetRendererInstance()->SetTransform(TS_WORLD, matWorld);
    if(g_ChopperMesh) g_ChopperMesh->render(matWorld);//g_vecChopper.x,g_vecChopper.y,g_vecChopper.z,1.0f,1.0f,1.0f,g_rotChopper, 0.0f, -0.0872222);

    // Draw the rotor (rotate and tilt it forward a bit and position above helicopter)
    D3DXFROMWINEMATRIX matRotY, matRotZ, matRotYY;
    D3DXFROMWINEMatrixRotationY(&matRotY, (float)Time / 10.0f);  // Turn rotor
    D3DXFROMWINEMatrixRotationZ(&matRotZ, -0.174444f);           // Angle forward
    D3DXFROMWINEMatrixRotationY(&matRotYY, g_rotChopper);        // Orient to chopper
    D3DXFROMWINEMatrixTranslation(&matWorld, g_vecChopper.x, g_vecChopper.y, g_vecChopper.z);
    //IRenderer::GetRendererInstance()->SetTransform(TS_WORLD, (matRotY * matRotZ * matRotYY * matWorld));
    if(g_RotorMesh) g_RotorMesh->render(g_vecChopper.x, g_vecChopper.y, g_vecChopper.z,1.0f,1.0f,1.0f,0.0f, (float)Time / 10.0f,-0.174444f);
#endif
#if 1
    /////////////////////////////////////////////////////////////////////////
    //
    // Draw all particles last
    //
    /////////////////////////////////////////////////////////////////////////

    // Disable lighting
    //???IRenderer::GetRendererInstance()->SetRenderState(D3DRS_LIGHTING, FALSE);
#if 0
    // Enable alpha testing for transparent blits
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHAREF, 0x08);
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
#else
#endif
    // Begin particle rendering
    //g_TreeEmitter->Begin();//&matView, &g_matProj);
//
    // Draw the tree and people particles
    g_TreeEmitter->Render(1);
	
    g_PeopleEmitter->Render(1);
	
#if 0
    // Enable alpha blending for the smoke particles
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
#else
#endif
	//
    // Draw the smoke particles
    g_ChopperEmitter->Render(1);

	
#if 0
    // Disable alpha testing and alpha blending
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    IRenderer::GetRendererInstance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
#else
#endif
#endif
	STX_Service::GetAudioInstance()->Update();
    // End the scene
    IRenderer::GetRendererInstance()->EndScene();

	//IRenderer::GetRendererInstance()->SaveScreenShot("testss");

  // Present the scene to the user
  IRenderer::GetRendererInstance()->Present();
  //
	return 0;
}

int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "ParticleVS");		
	IInput*    i=STX_Service::GetInputInstance();
	init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();	
		render();
	}
	return 0;
}

