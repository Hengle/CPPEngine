#ifndef __LitVertexVS2PS__

#define __LitVertexVS2PS__



#include "DXErrors.h"

#include "LitVertex.h"



struct LitVertexVS2PS

{

	static bool inited;



	static ShaderID  m_pEffect;

	//static VertexFormatID VERTEXVF;
	static VertexFormatID LVERTEX1VF;

	static void init(void);

	static void shutDown(void);

	static void Begin( float* wvp=0 );

	static void End( void );

	static void SetTexture( TextureID pTexture=-1 );





};

#endif

