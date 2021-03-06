/*
  Copyright (c) 2019 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include "R2VBAnimation.h"

#define __USE_HALF__

#ifdef __USE_HALF__
	#define VERTEX_FORMAT					FORMAT_RGBA16f
	#define BONE_INDEX_FORMAT				FORMAT_RGBA16f
	#define VERTEX_WEIGHT_FORMAT			FORMAT_RGBA16f
	#define BONE_MATRIX_FORMAT				FORMAT_RGBA16f
	#define FRAMETIME_MATRIX_FORMAT			FORMAT_RGBA16f
	#define R2VB_VBO_FORMAT					FORMAT_RGBA32f
	#define R2VB_TEXCOORD_FORMAT			FORMAT_FLOAT
	
	#define VERTEX_VALUE_FORMAT				half
	#define BONE_INDEX_VALUE_FORMAT			half
	#define VERTEX_WEIGHT_VALUE_FORMAT		half
	#define BONE_MATRIX_VALUE_FORMAT		half
	#define FRAMETIME_MATRIX_VALUE			half
	#define R2VB_VERTEX_FORMAT				FORMAT_FLOAT
	#define R2VB_VERTEX_VALUE_FORMAT		float
	#define R2VB_TEXCOORD_VALUE_FORMAT		float
#else
	#define VERTEX_FORMAT					FORMAT_RGBA32f
	#define BONE_INDEX_FORMAT				FORMAT_RGBA32f
	#define VERTEX_WEIGHT_FORMAT			FORMAT_RGBA32f
	#define BONE_MATRIX_FORMAT				FORMAT_RGBA32f
	#define FRAMETIME_MATRIX_FORMAT			FORMAT_RGBA32f
	#define R2VB_VBO_FORMAT					FORMAT_RGBA32f
	#define R2VB_TEXCOORD_FORMAT			FORMAT_FLOAT

	#define VERTEX_VALUE_FORMAT				float
	#define BONE_INDEX_VALUE_FORMAT			float
	#define VERTEX_WEIGHT_VALUE_FORMAT		float
	#define BONE_MATRIX_VALUE_FORMAT		float
	#define FRAMETIME_MATRIX_VALUE			float
	#define R2VB_VERTEX_FORMAT				FORMAT_FLOAT
	#define R2VB_VERTEX_VALUE_FORMAT		float
	#define R2VB_TEXCOORD_VALUE_FORMAT		float
#endif

//-----------------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------------
R2VBAnimation::R2VBAnimation()
{
	startTime = 0.0f; 
	endTime = 0.0f;
	time = 0.0f;
	interpolate = 0.0f;
	transformMatrix = identity4();
	maxTextureSize = MAX_TEX_SIZE;
}
//-----------------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------------
R2VBAnimation::~R2VBAnimation()
{
}
//-----------------------------------------------------------------------------------------
// Load skinning vertex data into texture. it includes vertex position, bone index, 
// weight value.
//-----------------------------------------------------------------------------------------
bool R2VBAnimation::loadSkinningVertexMap(char* name)
{
	FILE	*fp;
	__DWORD__	NumVertices, NumBones;
	D3DXFROMWINEVECTOR4	*vertexOffset;
	D3DXFROMWINEVECTOR4	*vertexNormal;
LOG_FNLN;
LOG_PRINT("fopen:%s\n", name);
	fp = fopen(name, "rb");
	if (!fp)
		return false;

	fread(&NumBones, sizeof(__DWORD__), 1, fp);
	fread(&NumVertices, sizeof(__DWORD__), 1, fp);

	vertexOffset = new D3DXFROMWINEVECTOR4[NumVertices];
	vertexNormal = new D3DXFROMWINEVECTOR4[NumVertices];

	for (uint i=0; i<NumVertices; i++)
	{
		fread(&vertexOffset[i], sizeof(D3DXFROMWINEVECTOR4), 1, fp);
		fseek(fp, sizeof(D3DXFROMWINEVECTOR4)*3, SEEK_CUR);
		fread(&vertexNormal[i], sizeof(D3DXFROMWINEVECTOR4), 1, fp);
		vertexNormal[i].w = 0.0f;		
	}

	Image3 tex;{
	// 1 pixel can store 1 vertex positions, 1 pixel can store 1 vertex normal, one skinning vertex == 2 pixels.
	VERTEX_VALUE_FORMAT* lpC = (VERTEX_VALUE_FORMAT *)tex.create(VERTEX_FORMAT, vertexTextureWidth*2, vertexTextureHeight, 1, 0);
	{
		for (uint j=0; j<NumVertices; j++)
		{
			*lpC++ = VERTEX_VALUE_FORMAT(vertexOffset[j].x);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexOffset[j].y);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexOffset[j].z);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexOffset[j].w);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexNormal[j].x);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexNormal[j].y);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexNormal[j].z);
			*lpC++ = VERTEX_VALUE_FORMAT(vertexNormal[j].w);
		}
	}
	skinningVertex = IRenderer::GetRendererInstance()->addTexture(&tex, false, IRenderer::GetRendererInstance()->GetnearestClamp());}{

	// 1 pixel can store 4 bone index data, one skinning vertex bone index data == 1 pixel.
	BONE_INDEX_VALUE_FORMAT* lpC = (BONE_INDEX_VALUE_FORMAT *)tex.create(BONE_INDEX_FORMAT, vertexTextureWidth, vertexTextureHeight, 1, 0);
	{
		float V[4];
		for (uint i=0; i<NumVertices; i++)
		{
			for (int j=0; j<4; j++)
			{
				fread(&V[j], sizeof(float), 1, fp);
				V[j] *= 4.0f;
				V[j] /= ((float)NumBones*4.0f);
				*lpC++ = BONE_INDEX_VALUE_FORMAT(V[j]);
			}
		}
	}
	vertexBoneIndex = IRenderer::GetRendererInstance()->addTexture(&tex, false, IRenderer::GetRendererInstance()->GetnearestClamp());}{

	// 1 pixel can store 4 weight value, one skinning vertex weight value == 1 pixel.
	VERTEX_WEIGHT_VALUE_FORMAT* lpC = (VERTEX_WEIGHT_VALUE_FORMAT *)tex.create(VERTEX_WEIGHT_FORMAT, vertexTextureWidth, vertexTextureHeight, 1, 0);
	{
		for (uint i=0; i<NumVertices; i++)
		{
			float fV[4];
			fread(&fV[0], sizeof(float), 1, fp);
			*lpC++ = VERTEX_WEIGHT_VALUE_FORMAT(fV[0]);
			fread(&fV[1], sizeof(float), 1, fp);
			*lpC++ = VERTEX_WEIGHT_VALUE_FORMAT(fV[1]);
			fread(&fV[2], sizeof(float), 1, fp);
			*lpC++ = VERTEX_WEIGHT_VALUE_FORMAT(fV[2]);
			fread(&fV[3], sizeof(float), 1, fp);
			*lpC++ = VERTEX_WEIGHT_VALUE_FORMAT(fV[3]);
		}
	}
	vertexWeight = IRenderer::GetRendererInstance()->addTexture(&tex, false, IRenderer::GetRendererInstance()->GetnearestClamp());}

	fclose(fp);

	delete [] vertexOffset;
	delete [] vertexNormal;

	return true;
}
//-----------------------------------------------------------------------------------------
// Load bone animation matrixes into texture.
//-----------------------------------------------------------------------------------------
bool R2VBAnimation::loadBoneAnimationMap(char* name)
{
	FILE	*fp;
LOG_FNLN;
LOG_PRINT("fopen:%s\n", name);
	fp = fopen(name, "rb");
	if (!fp)
		return false;

	fread(&numBones, sizeof(__DWORD__), 1, fp);
	fread(&numFrames, sizeof(__DWORD__), 1, fp);
	for (uint i=0; i<numFrames; i++)
		fread(bonesMatrix[i], sizeof(D3DXFROMWINEMATRIX), numBones, fp);

	fclose(fp);

	Image3 tex;
	{BONE_MATRIX_VALUE_FORMAT* lpC = (BONE_MATRIX_VALUE_FORMAT *)tex.create(BONE_MATRIX_FORMAT, 4*numBones, numFrames, 1, 0);
	{
		for (uint f=0; f<numFrames; f++)
			for (__DWORD__ i=0; i<numBones; i++)
			{
				float *lpM = &bonesMatrix[f][i].m[0][0];
				for (int i=0; i<16; i++)
					*lpC++ = BONE_MATRIX_VALUE_FORMAT(lpM[i]);
			}
	}
	boneAnimation = IRenderer::GetRendererInstance()->addTexture(&tex, false, IRenderer::GetRendererInstance()->GetnearestClamp());}

	// Create render target for storage of bone's transform matrices.
	// 4 pixels == 1 matrix4x4, so (4*numBones)x1 floating-point texture can store (numBones) matrices of bone.
	boneMatrix = IRenderer::GetRendererInstance()->addRenderTarget(4*numBones, 1, BONE_MATRIX_FORMAT, IRenderer::GetRendererInstance()->GetnearestClamp());

	return true;
}
//-----------------------------------------------------------------------------------------
// Load model triangle and vertex data, that will be used on vertex shader animation.
// We also create R2VB vertex buffer here.
//-----------------------------------------------------------------------------------------
bool R2VBAnimation::load(char *modelname, char *diffuse, __DWORD__ maxtexsize, bool debugruntime, const char* aPath) 
{
/*
/new/x/x/R2VB_VTF/R2VB-Animation/
/new/x/x/R2VB_VTF/R2VB-ShadowVolume/
/new/x/x/R2VB_VTF/R2VB-Footprint/
*/
	char buf[256];
	stx_snprintf(buf, 256, "%s/animation.shd");
	// Load shaders
	if ((animationSH = IRenderer::GetRendererInstance()->addShaderFromFile(buf, "main", "main")) == SHADER_NONE) return false;
	stx_snprintf(buf, 256, "%s/skinning.shd");
	if ((skinningSH = IRenderer::GetRendererInstance()->addShaderFromFile(buf, "main", "main")) == SHADER_NONE) return false;
	stx_snprintf(buf, 256, "%s/model.shd");
	if ((modelSH = IRenderer::GetRendererInstance()->addShaderFromFile(buf, "main", "main")) == SHADER_NONE) return false;

	FILE	*fp;
	D3DXFROMWINEVECTOR2	*lpUVList;
	char Path[256];

	maxTextureSize = maxtexsize;
	sprintf(Path, "%s%s.mdl", MODEL_PATH, modelname);
LOG_FNLN;
LOG_PRINT("fopen:%s\n", Path);
	fp = fopen(Path, "rb");
	if (!fp)
		return false;

	fread(&numVertices, sizeof(__DWORD__), 1, fp);

	vertexTextureWidth = numVertices;
	vertexTextureHeight = 1;
	if (vertexTextureWidth > (maxTextureSize/2))
	{
		vertexTextureHeight = (vertexTextureWidth/(maxTextureSize/2));
		if (vertexTextureWidth%(maxTextureSize/2))
			vertexTextureHeight++;
		if (vertexTextureHeight%2)
			vertexTextureHeight++;
		vertexTextureWidth = (maxTextureSize/2);
	}	
	lpUVList = new D3DXFROMWINEVECTOR2[numVertices];

	// Create vertex declarations
	FormatDesc modelVertexAttribs[] = {
		1, TYPE_VERTEX,   R2VB_VERTEX_FORMAT, 4,
		1, TYPE_NORMAL,   R2VB_VERTEX_FORMAT, 4,
		0, TYPE_TEXCOORD, R2VB_TEXCOORD_FORMAT, 2,
	};
	modelVF = IRenderer::GetRendererInstance()->addVertexFormat(modelVertexAttribs, elementsOf(modelVertexAttribs), modelSH);
	
	for (__DWORD__ i=0; i<numVertices; i++) 
	{
		fseek(fp, sizeof(D3DXFROMWINEVECTOR4)*7, SEEK_CUR);
	}
	// Create the texture coord of model
	std::vector<R2VB_TEXCOORD_VALUE_FORMAT> uvdest(10*numVertices);
		fread(lpUVList, sizeof(D3DXFROMWINEVECTOR2), numVertices, fp);
		for (uint j=0; j<numVertices; j++)
		{
			uvdest[10*j+8] = R2VB_TEXCOORD_VALUE_FORMAT(lpUVList[j].x);
			uvdest[10*j+9] = R2VB_TEXCOORD_VALUE_FORMAT(lpUVList[j].y);
		}
	UVBuffer = IRenderer::GetRendererInstance()->addVertexBuffer(numVertices * 10 * sizeof(R2VB_TEXCOORD_VALUE_FORMAT), STATIC, &uvdest[0]);

	fread(&numTriangles, sizeof(__DWORD__), 1, fp);
	pTriangleList = new __WORD__[numTriangles*3];

	//craete index buffer of model
	if ((IB = IRenderer::GetRendererInstance()->addIndexBuffer(numTriangles*3, sizeof(__WORD__))) == IB_NONE) return false;
	__WORD__ *iDest = (__WORD__ *) IRenderer::GetRendererInstance()->lockIndexBuffer(IB);
		fread(pTriangleList, sizeof(__WORD__), numTriangles*3, fp);
		memcpy(iDest, pTriangleList, sizeof(__WORD__)*numTriangles*3);
	IRenderer::GetRendererInstance()->unlockIndexBuffer(IB);

	/*
		Create render target for the model animation.
		Since these render targets will be used as vertex buffers we need to
		signal this special usage to the driver with the D3DUSAGE_DMAP flag.
	*/
	R2VB_VBO = IRenderer::GetRendererInstance()->addRenderTarget(vertexTextureWidth*2, vertexTextureHeight, R2VB_VBO_FORMAT, IRenderer::GetRendererInstance()->GetnearestClamp());

	/*
		We need a dummy vertex buffer when we specify stride and offset when calling SetStreamSource()
		for R2VB. Usually we can just pass any buffer we have laying around, but the debug runtime does
		additional validation, so we have to ensure the dummy vertex buffer is large enough to be valid
		for the draw call in this case, even though it's not going to be used. For the release runtime
		we stick with the smaller size to save memory.
	*/
	int size = (vertexTextureWidth*2) * vertexTextureHeight * 4 * sizeof(R2VB_VERTEX_VALUE_FORMAT);
	if ((R2VB_DummyVBO = IRenderer::GetRendererInstance()->addVertexBuffer(size)) == VB_NONE) return false;

	fclose(fp);

	delete [] lpUVList;
	delete [] pTriangleList;

	sprintf(Path, "%s%s.skn", MODEL_PATH, modelname);
	loadSkinningVertexMap(Path);
	sprintf(Path, "%s%s.ani", MODEL_PATH, modelname);
	loadBoneAnimationMap(Path);

	// Load textures
	sprintf(Path, "%s%s", TEX_PATH, diffuse);
	if ((diffuseMap = IRenderer::GetRendererInstance()->addImageLibTexture(Path, false, FILTER_TRILINEAR_ANISO)) == TEXTURE_NONE) return false;
	
	FormatDesc quadAttribs[] = {
		0, TYPE_VERTEX,   FORMAT_FLOAT, 4,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
	};
	animationVF = IRenderer::GetRendererInstance()->addVertexFormat(quadAttribs, elementsOf(quadAttribs), animationSH);
	skinningVF = IRenderer::GetRendererInstance()->addVertexFormat(quadAttribs, elementsOf(quadAttribs), skinningSH);

	float quadV[4][6] = {
		-1,  1, 0, 1, 0, 0,
		 1,  1, 0, 1, 1, 0,
		 1, -1, 0, 1, 1, 1,
		-1, -1, 0, 1, 0, 1,
	};
	// Create the quad vertex buffer
	if ((QuadVB = IRenderer::GetRendererInstance()->addVertexBuffer(4 * 6 * (sizeof(float)))) == VB_NONE) return false;
	float *dest = (float *) IRenderer::GetRendererInstance()->lockVertexBuffer(QuadVB);
		memcpy(dest, quadV, sizeof(float)*6*4);
	IRenderer::GetRendererInstance()->unlockVertexBuffer(QuadVB);

	time = 0.0f;
	interpolate = 0.0f;
	
	return true;
}
//-----------------------------------------------------------------------------------------
// Use pixel shader to calculate new bone matrixes.
//-----------------------------------------------------------------------------------------
void R2VBAnimation::psCalculateAnimation(float t, float interp)
{
	IRenderer::GetRendererInstance()->changeRenderTarget(boneMatrix);

		IRenderer::GetRendererInstance()->setShader(animationSH);
		IRenderer::GetRendererInstance()->setVertexFormat(animationVF);
		IRenderer::GetRendererInstance()->setVertexBuffer(0, QuadVB);
		IRenderer::GetRendererInstance()->setTexture("boneAnimation", boneAnimation);
		IRenderer::GetRendererInstance()->setDepthFunc(eDEPTH_NONE);
		

		IRenderer::GetRendererInstance()->setShaderConstant4f("time_interp", D3DXFROMWINEVECTOR4(t-1, t, interp, 0.0f));
		IRenderer::GetRendererInstance()->setShaderConstant1f("XBias", 0.1f/(numBones*4.0f));
		IRenderer::GetRendererInstance()->setShaderConstant1f("iBoneAnimationHeight", 1.0f/numFrames);
		IRenderer::GetRendererInstance()->setShaderConstant4x4f("TransM", transformMatrix);
		
		IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLE_FAN, 0, 2);
}
//-----------------------------------------------------------------------------------------
// Use pixel shader to transform vertices of model.
//-----------------------------------------------------------------------------------------
void R2VBAnimation::psSkinning()
{
	IRenderer::GetRendererInstance()->changeRenderTarget(R2VB_VBO);

		IRenderer::GetRendererInstance()->setShader(skinningSH);
		IRenderer::GetRendererInstance()->setVertexFormat(skinningVF);
		IRenderer::GetRendererInstance()->setVertexBuffer(0, QuadVB);
		IRenderer::GetRendererInstance()->setTexture("skinningVertex", skinningVertex);
		IRenderer::GetRendererInstance()->setTexture("vertexBoneIndex", vertexBoneIndex);
		IRenderer::GetRendererInstance()->setTexture("vertexWeight", vertexWeight);
		IRenderer::GetRendererInstance()->setTexture("boneMatrix", boneMatrix);
		IRenderer::GetRendererInstance()->setDepthFunc(eDEPTH_NONE);

		IRenderer::GetRendererInstance()->setShaderConstant1f("xBias", 0.1f/(vertexTextureWidth*2.0f));
		IRenderer::GetRendererInstance()->setShaderConstant4f("bias", D3DXFROMWINEVECTOR4(0.1f/(numBones*4.0f), 1.1f/(numBones*4.0f), 2.1f/(numBones*4.0f), 3.1f/(numBones*4.0f)));

		IRenderer::GetRendererInstance()->DrawPrimitive(PRIM_TRIANGLE_FAN, 0, 2);

	IRenderer::GetRendererInstance()->changeToMainFramebuffer();
}
//-----------------------------------------------------------------------------------------
// Play the animation of player.
//-----------------------------------------------------------------------------------------
void R2VBAnimation::play(float step, bool bloop)
{		
	interpolate += step;
	if (interpolate >= 1.0f)
	{
		interpolate = 0.0f;
		time++;
		if (time >= endTime)
		{
			if (bloop)
			{
				time = startTime+1.0f;
			}
			else
			{
				time = endTime-1.0f;
			}
		}
	}
	psCalculateAnimation(time+1, interpolate);
	psSkinning();
}
//-----------------------------------------------------------------------------------------
// Render model using R2VB vertex buffer.
//-----------------------------------------------------------------------------------------
void R2VBAnimation::draw(D3DXFROMWINEMATRIX &mvp)
{
	IRenderer::GetRendererInstance()->setShader(modelSH);
	IRenderer::GetRendererInstance()->setVertexFormat(modelVF);
	//IRenderer::GetRendererInstance()->setVertexBuffer(0, R2VB_DummyVBO);
	IRenderer::GetRendererInstance()->setVertexBuffer(0, UVBuffer);
	IRenderer::GetRendererInstance()->setIndexBuffer(IB);
	// Bind the render target to the DMAP sampler
	IRenderer::GetRendererInstance()->setTexture("DMAP", R2VB_VBO);
	IRenderer::GetRendererInstance()->setTexture("diffuseMap", diffuseMap);
	//IRenderer::GetRendererInstance()->setDepthFunc(LESS);
	
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("mvp", mvp);
	D3DXFROMWINEVECTOR3 v3(-1, -1, -1);
	D3DXFROMWINEVECTOR3 n3=normalize(v3);
	D3DXFROMWINEVECTOR4 v4(n3, 1);
	IRenderer::GetRendererInstance()->setShaderConstant4f("LightDir", v4);//lit.position);
	IRenderer::GetRendererInstance()->setShaderConstant4f("LightColor", D3DXFROMWINEVECTOR4(1, 1, 1, 1));

	IRenderer::GetRendererInstance()->DrawIndexedPrimitive(PRIM_TRIANGLES, 0, 0, numVertices, 0, numTriangles);
}

