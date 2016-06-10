#include <Framework3/IRenderer.h>

#include "Particles.h"

#define __IB__ 0
inline DWORD FLOAT2DWORD(FLOAT f) { return *((DWORD*)&f); }


// Clear static variables to their defaults
DWORD                        cParticleEmitter::m_RefCount  = 0;
    ShaderID cParticleEmitter::m_pShader= -1;    // Vertex shader
    VertexFormatID cParticleEmitter::m_pDecl= -1; // Vertex decl
    TextureID *cParticleEmitter::m_pTextures;      // Textures
int cParticleEmitter::alphaState= -1;

cParticleEmitter::cParticleEmitter()
{
  // Clear object pointers
  m_pDevice      = 0;
  m_VB           = -1;
 #ifdef __IB__
  m_IB           = -1;
#endif
  // Clear particle data
  m_NumParticles = 0;
  m_Particles    = 0;
  if(alphaState==-1)
  	alphaState=IRenderer::GetRendererInstance()->addAlphaState(1,1.0f);
    
}

cParticleEmitter::~cParticleEmitter()
{
  Free();  // Free particles
}

BOOL cParticleEmitter::Create(IRenderer *pDevice,
                              D3DXFROMWINEVECTOR3 *vecPosition,
                              DWORD EmitterType,
                              DWORD NumParticlesPerBuffer)
{
  // Error checking
  if(!(m_pDevice = pDevice))
    return FALSE;

  // Save emitter's position
  m_vecPosition = (*vecPosition);

  // Save type of emitter
  m_EmitterType = EmitterType;

  // Save # of particles in buffer
  m_NumParticles = NumParticlesPerBuffer;

  // Load textures if none already loaded
  if(m_pTextures == 0) {

    m_pTextures = new TextureID[NUM_PARTICLE_TYPES];

	m_pTextures[0]=IRenderer::GetRendererInstance()->addImageLibTexture(
				"/AdvAniCh12/Particle_Smoke.png",
        			false,
				IRenderer::GetRendererInstance()->Getlinear());

	m_pTextures[1]=IRenderer::GetRendererInstance()->addImageLibTexture(
                                "/dx8sdk/tree01S.tga",
                                false,
				IRenderer::GetRendererInstance()->Getlinear());

	m_pTextures[2]=IRenderer::GetRendererInstance()->addImageLibTexture(
                                "/dx8sdk/tree02S.tga",
                                false,
				IRenderer::GetRendererInstance()->Getlinear());

	m_pTextures[3]=IRenderer::GetRendererInstance()->addImageLibTexture(
                                "/dx8sdk/tree35S.tga",
                                false,
				IRenderer::GetRendererInstance()->Getlinear());

	m_pTextures[4]=IRenderer::GetRendererInstance()->addImageLibTexture(
                                "/AdvAniCh12/Particle_People1.png",
                                false,
				IRenderer::GetRendererInstance()->Getlinear());

	m_pTextures[5]=IRenderer::GetRendererInstance()->addImageLibTexture(
                                "/AdvAniCh12/Particle_People2.png",
				false,
				IRenderer::GetRendererInstance()->Getlinear());
  }

#ifdef __IB__
  unsigned short *Ptri=new unsigned short[6*m_NumParticles];;
  for(unsigned short i=0;i<m_NumParticles;i++) {
    Ptri[i*6+0] = i * 4 + 0;
    Ptri[i*6+1] = i * 4 + 1;
    Ptri[i*6+2] = i * 4 + 2;
    Ptri[i*6+3] = i * 4 + 3;
    Ptri[i*6+4] = i * 4 + 2;
    Ptri[i*6+5] = i * 4 + 1;
  }
  // Create the index buffer and fill with face data
  m_IB=m_pDevice->addIndexBuffer(m_NumParticles * 6 , sizeof(short),STATIC,Ptri);
  delete[] Ptri;
#endif
  Ptr=0;
  Ptr0=0;

  // Load the vertex shader if not already done
  if(-1==m_pShader) {

std::string g_szParticleShader = std::string(
	"[Vertex shader]\n"
	"ROW_MAJOR float4x4 WorldViewProjection MVPSEMANTIC;\n"
	"float4 right;\n"
	"float4 up;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Position : POSITION;\n"
		"float2 vecOffset : TEXCOORD0;\n"
		//"float4 Diffuse : TEXCOORD1;\n"
		"float2 uv : TEXCOORD1;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Position : POSITION;\n"
	//	"float4 Diffuse : TEXCOORD0;\n"
		"float2 uv : TEXCOORD0;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
		//"if (IN.Diffuse.rgb == float3(0.0,0.0,0.0)) discard;\n"
		"float3 objPos =  IN.vecOffset.y*up.xyz+ IN.vecOffset.x*right.xyz+ IN.Position;\n"
		"Out.Position = mul( WorldViewProjection,float4( objPos.x, objPos.y ,objPos.z, 1.0f));\n"
		"Out.uv=IN.uv;\n"
		"return Out;\n"
	"}\n"

"[Fragment shader]\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Position : POSITION;\n"
	//	"float4 Diffuse : TEXCOORD0;\n"
		"float2 uv : TEXCOORD0;\n"
		"};\n"

		"sampler2D Sampler;\n"
	"float4 main(VS_OUTPUT IN) : COLOR\n"
	"{\n"
		"float4 OUT = tex2D(Sampler,IN.uv);\n"
		//"if (OUT.rgb == float3(0.0,0.0,0.0)) discard;\n"
		"return OUT;\n"
	"}\n"
	);

	m_pShader=IRenderer::GetRendererInstance()->addHLSLShader(g_szParticleShader.c_str(),"main","main");
	// Vertex declaration
	FormatDesc g_VSParticleDecl[] = {
		0, TYPE_VERTEX, FORMAT_FLOAT, 3,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
		//0, TYPE_TEXCOORD, FORMAT_FLOAT, 4,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
	};
	m_pDecl = IRenderer::GetRendererInstance()->addVertexFormat(g_VSParticleDecl, elementsOf(g_VSParticleDecl),m_pShader);

  }

  // Increase class reference count
  m_RefCount++;

  // Return success
  return TRUE;
}

void cParticleEmitter::Free()
{
  // Decrease reference count and free resources if needed
  if(m_RefCount)
    m_RefCount--;
  if(!m_RefCount) {

    // Release textures
    if(m_pTextures) {
      for(DWORD i=0;i<NUM_PARTICLE_TYPES;i++)
        //ReleaseCOM(m_pTextures[i]);
      delete [] m_pTextures;
      m_pTextures = 0;
    }

    // Release shader data
    //ReleaseCOM(m_pShader);
    //ReleaseCOM(m_pDecl);
  }

  // Clear object pointers
  m_pDevice = 0;
  //ReleaseCOM(m_VB);
  //ReleaseCOM(m_IB);

  // Clear particle data
  m_NumParticles = 0;
  delete m_Particles;
  m_Particles = 0;

    delete[] Ptr0;
}

void cParticleEmitter::Add(DWORD Type, D3DXFROMWINEVECTOR3 *vecPos, float Size, D3DXFROMWINEVECTOR4 Color, DWORD Life, D3DXFROMWINEVECTOR3 *vecVelocity)
{
  // Allocate a particle object and add to head of list
  cParticle *Particle = new cParticle();
  Particle->m_Prev = 0;
  if((Particle->m_Next = m_Particles))
    m_Particles->m_Prev = Particle;
  m_Particles = Particle;

  // Set particle data
  Particle->m_Type   = Type;
  Particle->m_vecPos = (*vecPos);
  Particle->m_Size   = Size;
//  Particle->m_Color  = Color;
  Particle->m_Life   = Life;
  Particle->m_vecVelocity = (*vecVelocity);
}

void cParticleEmitter::ClearAll()
{
  // Clear particle data
  delete m_Particles;
  m_Particles = 0;
}

BOOL cParticleEmitter::Begin()
{

  // Error checking
  if(!m_pDevice || (m_pShader==-1) || (m_pDecl==-1))
    return FALSE;
  

  // Set vertex shader, declaration, and stream sources
  m_pDevice->setShader(m_pShader);
  m_pDevice->setVertexFormat(m_pDecl);

	D3DXFROMWINEMATRIX matView=m_pDevice->Getvv();
	D3DXFROMWINEMATRIX matProj=m_pDevice->Getpp();
#if 1
  D3DXFROMWINEMATRIX matVP = (matView) * (matProj);
  D3DXFROMWINEMatrixTranspose(&matVP, &matVP);
#else
  D3DXFROMWINEMATRIX matVP = (matProj)*(matView);
#endif
  m_pDevice->setShaderConstant4x4f("WorldViewProjection", matVP);

  // Get right and up vectors from view transformation and store as constants
  D3DXFROMWINEVECTOR4 vecRight, vecUp;
  D3DXFROMWINEVECTOR4 vr(matView.m[0][0], matView.m[1][0], matView.m[2][0],0.0f);
  D3DXFROMWINEVECTOR4 vu(matView.m[0][1], matView.m[1][1], matView.m[2][1], 0.0f);

  D3DXFROMWINEVec4Normalize(&vecRight, &vr);
  D3DXFROMWINEVec4Normalize(&vecUp,    &vu);
  m_pDevice->setShaderConstant4f("right", vecRight);
  m_pDevice->setShaderConstant4f("up", vecUp);
  

  return TRUE;
}

void cParticleEmitter::Render(int bA)
{
  DWORD LastTexture = -1;
  

  // Error checking
  if(!m_pDevice ||  !m_Particles)
    return;

  int bM=0;

  // MUST CALL BEGIN BEFORE CALLING RENDER!!!
  

  // Go through each type of particle to draw
  // Chance to optimize - speed this up
  for(DWORD i=0;i<NUM_PARTICLE_TYPES;i++) {

    // Start at first particle
    cParticle *Particle = m_Particles;

    // Set a count for flushing buffer
    DWORD Num = 0;
  

    // Lock the vertex buffer for use
	if(Ptr==0)
	{
		Ptr=new sShaderVertex[m_NumParticles * 4];
		Ptr0=Ptr;
	}
	else
		Ptr=Ptr0;
    //m_VB->Lock(0, 0, (void**)&Ptr, D3DLOCK_DISCARD);
  

    // Loop for all particles
    while((Particle != 0)&&(((Ptr-Ptr0)/4)<m_NumParticles)) {
  

      // Do types match?
      if(Particle->m_Type == i) {
  

	  //iiii=(Ptr-Ptr0)/4;
        // Set the type's texture (if not done already)
        //if(i != LastTexture) {
        //  LastTexture = i;
        //}

        // Copy over particle data
        float HalfSize = Particle->m_Size * 0.5f;

#ifdef __IB__
        Ptr[0].vecPos    = Particle->m_vecPos;
        Ptr[0].vecOffset = D3DXFROMWINEVECTOR2(-HalfSize, HalfSize);
//        Ptr[0].Diffuse   = Particle->m_Color;
        Ptr[0].u         = 0.0f;
        Ptr[0].v         = 0.0f;
        Ptr[1].vecPos    = Particle->m_vecPos;
        Ptr[1].vecOffset = D3DXFROMWINEVECTOR2(HalfSize, HalfSize);
  //      Ptr[1].Diffuse   = Particle->m_Color;
        Ptr[1].u         = 1.0f;
        Ptr[1].v         = 0.0f;
        Ptr[2].vecPos    = Particle->m_vecPos;
        Ptr[2].vecOffset = D3DXFROMWINEVECTOR2(-HalfSize, -HalfSize);
    //    Ptr[2].Diffuse   = Particle->m_Color;
        Ptr[2].u         = 0.0f;
        Ptr[2].v         = 1.0f;
        Ptr[3].vecPos    = Particle->m_vecPos;
        Ptr[3].vecOffset = D3DXFROMWINEVECTOR2(HalfSize, -HalfSize);
      //  Ptr[3].Diffuse   = Particle->m_Color;
        Ptr[3].u         = 1.0f;
        Ptr[3].v         = 1.0f;
		//LOG_PRINT("i=%d\n",(Ptr-Ptr0)/4);
        Ptr+=4;
#else
        Ptr[0].vecPos    = Particle->m_vecPos;
        Ptr[0].vecOffset = D3DXFROMWINEVECTOR2(HalfSize, -HalfSize);
//        Ptr[0].Diffuse   = Particle->m_Color;
        Ptr[0].u         = 1.0f;
        Ptr[0].v         = 1.0f;

        Ptr[1].vecPos    = Particle->m_vecPos;
        Ptr[1].vecOffset = D3DXFROMWINEVECTOR2(HalfSize, HalfSize);
  //      Ptr[1].Diffuse   = Particle->m_Color;
        Ptr[1].u         = 1.0f;
        Ptr[1].v         = 0.0f;

        Ptr[2].vecPos    = Particle->m_vecPos;
        Ptr[2].vecOffset = D3DXFROMWINEVECTOR2(-HalfSize, -HalfSize);
    //    Ptr[2].Diffuse   = Particle->m_Color;
        Ptr[2].u         = 0.0f;
        Ptr[2].v         = 1.0f;

        Ptr[3].vecPos    = Particle->m_vecPos;
        Ptr[3].vecOffset = D3DXFROMWINEVECTOR2(-HalfSize, HalfSize);
      //  Ptr[3].Diffuse   = Particle->m_Color;
        Ptr[3].u         = 0.0f;
        Ptr[3].v         = 0.0f;
		//LOG_PRINT("i=%d\n",(Ptr-Ptr0)/4);
        Ptr+=4;
#endif
		

        // Increase vertex count and flush buffer if full
        Num++;
        if(Num >= m_NumParticles) {
  

          // Unlock buffer and render polygons
          //m_VB->Unlock();

  m_VB=m_pDevice->addVertexBuffer(m_NumParticles * 4 * sizeof(sShaderVertex),STATIC,Ptr0,m_VB);
    

	Begin();
	  

	m_pDevice->setTexture("Sampler", m_pTextures[i]);
  // Set the stream sources
  m_pDevice->setVertexBuffer(0,m_VB);
  #ifdef __IB__
  m_pDevice->setIndexBuffer(m_IB);
#endif
	IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetnoDepthTest());
	//if(bA) 
    	//IRenderer::GetRendererInstance()->setBlendState(IRenderer::GetRendererInstance()->GetblendSrcAlpha());
	
    IRenderer::GetRendererInstance()->changeAlphaState(alphaState);

#ifdef __IB__
          m_pDevice->DrawIndexedPrimitive(PRIM_TRIANGLES, 0, 0, Num*4, 0, Num*2);
#else
          m_pDevice->DrawPrimitive(PRIM_TRIANGLES, 0, Num*2);
#endif
          // Lock vertex buffer again
          //m_VB->Lock(0, 0, (void**)&Ptr, D3DLOCK_DISCARD);

          // Clear vertex count
          Num=0;
  

		}
  

	  }
  

      // Go to next particle
      Particle = Particle->m_Next;
  

	}
  

    // Unlock vertex buffer
    //m_VB->Unlock();
  // Create the vertex buffer
  m_VB=m_pDevice->addVertexBuffer(m_NumParticles * 4 * sizeof(sShaderVertex),STATIC,Ptr0,m_VB);

  

    // Render any polygons left
    if(Num)
    {
  

		Begin();
  

  m_pDevice->setTexture("Sampler", m_pTextures[i]);
  // Set the stream sources
  m_pDevice->setVertexBuffer(0,m_VB);
 #ifdef __IB__
  m_pDevice->setIndexBuffer(m_IB);
#endif
	IRenderer::GetRendererInstance()->setRasterizerState(IRenderer::GetRendererInstance()->GetnoDepthTest());

#ifdef __IB__
      m_pDevice->DrawIndexedPrimitive(PRIM_TRIANGLES, 0, 0, Num*4, 0, Num*2);
#else
      m_pDevice->DrawPrimitive(PRIM_TRIANGLES, 0, Num*2);
#endif
	}
	  

  }
    

}

void cParticleEmitter::Process(DWORD Elapsed)
{
  cParticle *Particle = m_Particles;

  // Loop through all particles
  while(Particle != 0) {

    // Remember next particle
    cParticle *NextParticle = Particle->m_Next;

    // Set flag to remove from list
    BOOL RemoveFromList = FALSE;

    // Decrease life of particle, but not if life == 0
    if(Particle->m_Life) {
      if(Particle->m_Life <= Elapsed)
        RemoveFromList = TRUE;
      else
        Particle->m_Life -= Elapsed;
    }

    // Calculate scalar to use for velocity
    float Scalar = (float)Elapsed / 1000.0f;

    // Add velocity to particle positions
    Particle->m_vecPos += (Particle->m_vecVelocity * Scalar);

    // Remove particle from list if flagged
    if(RemoveFromList == TRUE) {

      // Have previous particle skip past one being deleted
      // or set new root if particle to be removed is the root
      if(Particle->m_Prev)
        Particle->m_Prev->m_Next = Particle->m_Next;
      else
        m_Particles = Particle->m_Next;

      // Set next particle's previous pointer
      if(Particle->m_Next)
        Particle->m_Next->m_Prev = Particle->m_Prev;

      // Delete particle
      Particle->m_Prev = 0;
      Particle->m_Next = 0;
      delete Particle;
    }

    // Go to next particle
    Particle = NextParticle;
  }
}


////////////////////////////////////////////////////////////////////////////
//
// Specialized particle functions
//
////////////////////////////////////////////////////////////////////////////
void cParticleEmitter::HandleSmoke(D3DXFROMWINEVECTOR3 *vecPos, DWORD Elapsed)
{
  static DWORD Timer = 0;

  // Update smoke timer and add a smoke particle as needed
  Timer += Elapsed;
  if(Timer > 66) {
    Timer = 0;

    // Pick a random direction to move particle
    float rot = (float)((stx_rand() % 628) / 100.0f);
	static D3DXFROMWINEVECTOR3 v1=D3DXFROMWINEVECTOR3(vecPos->x, 5.0f, vecPos->z);
	static D3DXFROMWINEVECTOR3 v2=D3DXFROMWINEVECTOR3((float)cos(rot)*20.0f, 0.0f, (float)sin(rot)*20.0f);
    Add(PARTICLE_SMOKE, &v1,
        10.0f, D3DXFROMWINEVECTOR4(255.0f, 34.0f, 34.0f, 34.0f), 1000,
        &v2);
  }
}

void cParticleEmitter::HandleDucking(D3DXFROMWINEVECTOR3 *vecPos)
{
  // Change people's particle types if chopper is close to them
  cParticle *Particle = m_Particles;
  while(Particle) {

    // Calculate distance from position to person (only on x/z axes)
    D3DXFROMWINEVECTOR3 vecDiff = Particle->m_vecPos - (*vecPos);
    float Dist = vecDiff.x*vecDiff.x + vecDiff.z*vecDiff.z;

    // If too close, make person duck, otherwise stand them up
    if(Dist < (40.0f * 40.0f))
      Particle->m_Type = PARTICLE_PEOPLE2;
    else
      Particle->m_Type = PARTICLE_PEOPLE1;

    // Go to next particle
    Particle = Particle->m_Next;
  }
}
