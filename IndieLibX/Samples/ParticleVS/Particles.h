#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include <Framework3/IRenderer.h>

// Particle emitter types
#define EMITTER_CLOUD       0
#define EMITTER_TREE        1
#define EMITTER_PEOPLE      2

// Particle types and # of particle types
#define PARTICLE_SMOKE      0
#define PARTICLE_TREE1      1
#define PARTICLE_TREE2      2
#define PARTICLE_TREE3      3
#define PARTICLE_PEOPLE1    4
#define PARTICLE_PEOPLE2    5
#define NUM_PARTICLE_TYPES  6

// Vertex structure for vertex-shader particles
typedef struct {
  D3DXFROMWINEVECTOR3 vecPos;     // Coordinates of particle
  D3DXFROMWINEVECTOR2 vecOffset;  // Corner vertex offset coordinates
  //D3DXFROMWINEVECTOR4       Diffuse;    // Diffuse color of particle
  float       u, v;       // Texture coordinates
} sShaderVertex;

// Particle container class
class cParticle
{
  public:
    DWORD       m_Type;         // Type of particle

    D3DXFROMWINEVECTOR3 m_vecPos;       // Position of particle
    D3DXFROMWINEVECTOR3 m_vecVelocity;  // Velocity of particle

    DWORD       m_Life;         // Life of particle in ms
    float       m_Size;         // Size of particle

    //D3DXFROMWINEVECTOR4       m_Color;        // Diffuse color of particle

    cParticle *m_Prev;          // Prev particle in linked list
    cParticle *m_Next;          // Next particle in linked list

  public:
    // Constructor and destructor to clear/release data
    cParticle()  { m_Prev = 0; m_Next = 0;                }
    ~cParticle() { delete m_Next; m_Next = 0; m_Prev = 0; }
};

// Particle emitter class
class cParticleEmitter
{
  protected:
    IRenderer       *m_pDevice;       // Parent 3-D device
    sShaderVertex *Ptr;
    sShaderVertex *Ptr0;

    DWORD                   m_EmitterType;   // Type of emitter

    VertexBufferID m_VB;            // Particles' vertex buffer
    IndexBufferID  m_IB;            // Particles' index buffer

    D3DXFROMWINEVECTOR3             m_vecPosition;   // Position of emitter
    DWORD                   m_NumParticles;  // Max # particles in buffer
    cParticle              *m_Particles;     // Particle list

    static DWORD            m_RefCount;      // Class reference count

    static ShaderID m_pShader;    // Vertex shader
    static VertexFormatID m_pDecl; // Vertex decl
    static TextureID *m_pTextures;      // Textures
    static int alphaState;

  public:
    cParticleEmitter();
    ~cParticleEmitter();

    BOOL Create(IRenderer *pDevice,
                D3DXFROMWINEVECTOR3 *vecPosition,
                DWORD EmitterType,
                DWORD NumParticlesPerBuffer = 32);
    void Free();

    void Add(DWORD Type, D3DXFROMWINEVECTOR3 *vecPos, float Size,
             D3DXFROMWINEVECTOR4 Color, DWORD Life,
             D3DXFROMWINEVECTOR3 *vecVelocity);
    void ClearAll();
    void Process(DWORD Elapsed);

    // Functions to prepare for particle rendering, wrap up rendering,
    // and to render a batch of particles.
    BOOL Begin();//D3DXFROMWINEMATRIX *matView, D3DXFROMWINEMATRIX *matProj);
    void Render(int bA=0);

    // Specialized functions for various particle types
    void HandleSmoke(D3DXFROMWINEVECTOR3 *vecPos, DWORD Elapsed);
    void HandleDucking(D3DXFROMWINEVECTOR3 *vecPos);
};

#endif
