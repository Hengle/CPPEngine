#ifndef _SNOWSYSTEM_H_
#define _SNOWSYSTEM_H_

#include <Framework/Util/ProgrammerArt.h>

#define ROWS_PER_DIP 64
#define MAX_WIDTH 1024

#if (ROWS_PER_DIP * MAX_WIDTH <= 65536)
#define INDEX_TYPE ushort
#else
#define INDEX_TYPE uint
#endif


class SnowSystem {
public:
	SnowSystem(){}
	bool load(__DWORD__ maxtexwidth, __DWORD__ maxtexheight, bool debugruntime, bool supportsHalf);
	bool reset();

	void draw(const D3DXFROMWINEMATRIX &mvp, D3DXFROMWINEMATRIX &mv, float frametime);

private:
	ShaderID particleShader, physics, initPhysics, makeQuads;
	TextureID randTex, particle, palette;

	VertexFormatID particleVF, physicsVF;
	VertexBufferID quadCoordVB;
	IndexBufferID particleIndices;

	TextureID rtPos[2], rtDir[2], r2vbRT;
	int currRT, otherRT;
	int rtWidth, rtHeight;
};

#endif
