/* * * * * * * * * * * * * Author's note * * * * * * * * * * * **
*  |_|	   |_| |_|     |_| |_|_   _|_| |_|     |_|  _|_|_|_|_|	*
*  |_|_ _ _|_| |_|     |_| |_|_|_|_|_| |_|     |_| |_|_ _ _	*
*  |_|_|_|_|_| |_|     |_| |_| |_| |_| |_|     |_|   |_|_|_|_	*
*  |_|	   |_| |_|_ _ _|_| |_|	   |_| |_|_ _ _|_|  _ _ _ _|_|	*
*  |_|	   |_|	 |_|_|_|   |_|	   |_|	 |_|_|_|   |_|_|_|_|	*
*								*
*		      http://www.humus.name			*
*								 *
* This file is a part of the work done by Humus. You are free to   *
* use the code in any way you like, modified, unmodified or copied   *
* into your own work. However, I expect you to respect these points:  *
*  - If you use this file and its contents unmodified, or use a major *
*    part of this file, please credit the author and leave this note. *
*  - For use in anything commercial, please request my approval.     *
*  - Share your work and ideas too as much as you can.		   *
*								 *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/

#include <Framework3/RendererCgGL_1_1.h>

#ifdef __FrameworkCgGL_1_1_H__
#include <Framework3/CgReflection.h>
CGcontext g_CgContext;

  void MyCgErrorCallback( void ) {
  CGerror error;
  const char *string = cgGetLastErrorString(&error);
  if (error != CG_NO_ERROR) {
    LOG_PRINT("%s\n",
      string);
    if (error == CG_COMPILER_ERROR) {
      LOG_PRINT("%s\n", cgGetLastListing(g_CgContext));
    }
	DBG_HALT;
  }
  }



struct ShaderCgGL3 : public Shader
{
	void disApplyTextures();

	ShaderCgGL3() : Shader(){}
	virtual ~ShaderCgGL3(){}
#if 0
        GLhandleARB program;
        GLhandleARB vertexShader;
        GLhandleARB fragmentShader;
#else
        CGprofile* vertexShaderprofile;
        CGprofile* fragmentShaderprofile;
        CGprogram vertexShaderprogram;
        CGprogram fragmentShaderprogram;
#endif
	std::vector<AttribShaderCgGL3 > attribs;
	std::vector<ConstantShaderCgGL3 > uniforms;
        std::vector<SamplerShaderCgGL3  > samplers;

        uint nAttribs;
	uint nUniforms;
        uint nSamplers;

};
#ifdef _WIN32

#endif
#if 0
typedef GLvoid (APIENTRY *UNIFORM_FUNC)(GLint location, GLsizei count, const void *value);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#else
typedef GLvoid (APIENTRY *UNIFORM_FUNC)(CGparameter param, const float *v);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(CGparameter param, const float *matrix);
#endif
//void RendererGL_1_1::
void RendererCgGL_1_1::changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBuffer, const intptr offset)
		{
			#if 1
#if 1
			int minAttrib = 0;
			int maxAttrib = (shaders[selectedShader])->nAttribs - 1;
			const GLsizei glTypes[] =
			{
				GL_FLOAT,
				0, GL_UNSIGNED_BYTE,
			}
			;
			{
			VertexFormatGL cvf = vertexFormats[currentVertexFormat];
			int vertexSize = cvf.vertexSize[stream];
			LOG_FNLN;
			for (int i = minAttrib; i < maxAttrib; i++)
			{
				int currAttrib = (minAttrib + maxAttrib) >> 1;
				int offset_x=((shaders[selectedShader])->attribs[currAttrib]).offset;
				LOG_FNLN;
				LOG_PRINT("stream=%d\n",stream);
				LOG_PRINT("offset=%d\n",offset_x);
				if (stream==offset_x)//(res == 0)
				{
					

			AttribShaderCgGL3* att = (&(shaders[selectedShader])->attribs[0]) + currAttrib;

					LOG_FNLN;
					att->dirty = true;
					
					return;
				}
			}
			}
			#endif
				LOG_FNLN;
				if (currentVertexFormat != VF_NONE)
				{
					char *base = (char *) offset;
					VertexFormatGL cvf = vertexFormats[currentVertexFormat];
					int vertexSize = cvf.vertexSize[stream];
					
					LOG_FNLN;
					for (int i = minAttrib; i < maxAttrib; i++)
					{
						LOG_FNLN;
						if ((shaders[selectedShader])->attribs[i].dirty&&(cvf.generic[i].stream == stream && cvf.generic[i].size))
						{
							LOG_FNLN;
							glVertexAttribPointer(i, cvf.generic[i].size, glTypes[cvf.generic[i].format], GL_TRUE, vertexSize, base + cvf.generic[i].offset);
				checkGlError("");
							glEnableVertexAttribArray (i);
				checkGlError("");
							shaders[selectedShader]->attribs[i].dirty=false;
/*   
// Load the vertex position
   glVertexAttribPointer ( att->loc, offset_x, GL_FLOAT, 
                           GL_FALSE, vertexSize, vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( att->loc, offset_x, GL_FLOAT,
                           GL_FALSE, vertexSize, &vVertices[3] );

   glEnableVertexAttribArray ( att->loc );
   glEnableVertexAttribArray ( att->loc );
*/						}
					}
				}
				
				currentVertexBuffers[stream] = vertexBuffer;
				currentOffsets[stream] = offset;
				activeVertexFormat[stream] = currentVertexFormat;
			#else
			const GLsizei glTypes[] =
			{
				GL_FLOAT,
				0, GL_UNSIGNED_BYTE,
			}
			;
			GLuint vbo = 0;
			if (vertexBuffer != VB_NONE) vbo = (vertexBuffers[vertexBuffer]).vboVB;
			if (vbo != currentVBO)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentVBO = vbo);
				checkGlError("");
			}
			//if (vertexBuffer != currentVertexBuffers[stream] || offset != currentOffsets[stream] || currentVertexFormat != activeVertexFormat[stream])
			{
				if (currentVertexFormat != VF_NONE)
				{
					char *base = (char *) offset;
					VertexFormatGL cvf = vertexFormats[currentVertexFormat];
					int vertexSize = cvf.vertexSize[stream];
					/*
					if (cvf.vertex.stream == stream && cvf.vertex.size)
					{
						glVertexPointer(cvf.vertex.size, glTypes[cvf.vertex.format], vertexSize, base + cvf.vertex.offset);
				checkGlError("");
					}
					if (cvf.normal.stream == stream && cvf.normal.size)
					{
						glNormalPointer(glTypes[cvf.normal.format], vertexSize, base + cvf.normal.offset);
				checkGlError("");
					}
					*/
					for (int i = 0; i < MAX_GENERIC; i++)
					{
						if (cvf.generic[i].stream == stream && cvf.generic[i].size)
						{
							glVertexAttribPointerARB(i, cvf.generic[i].size, glTypes[cvf.generic[i].format], GL_TRUE, vertexSize, base + cvf.generic[i].offset);
				checkGlError("");
							/*{
							GLint loc = glGetAttribLocation ( prog, name);
							glEnableVertexAttribArray (loc);
				checkGlError("");
							}*/
						}
					}
					/*
					for (int i = 0; i < MAX_TEXCOORD; i++)
					{
						if (cvf.texCoord[i].stream == stream && cvf.texCoord[i].size)
						{
							glClientActiveTexture(GL_TEXTURE0 + i);
				checkGlError("");
							glTexCoordPointer(cvf.texCoord[i].size, glTypes[cvf.texCoord[i].format], vertexSize, base + cvf.texCoord[i].offset);
				checkGlError("");
						}
					}*/
				}
				currentVertexBuffers[stream] = vertexBuffer;
				currentOffsets[stream] = offset;
				activeVertexFormat[stream] = currentVertexFormat;
			}
			#endif
		}

void RendererCgGL_1_1::applyConstants()
{
	if (currentShader != SHADER_NONE)
	{
		for (uint i = 0; i < shaders[currentShader]->nUniforms; i++)
		{
			ConstantShaderCgGL3* uni = (&shaders[currentShader]->uniforms[0]) + i;
			if ((uni)->dirty)
			{
#if 0
				if ((uni)->type == CG_PARAMETERCLASS_MATRIX)
				{
					#if 0
					((UNIFORM_MAT_FUNC) uniformFuncs[(uni)->type])
					((uni)->index,
					(uni)->nElements, GL_TRUE,
					(float *) (uni)->data);
					#else
					((UNIFORM_MAT_FUNC) uniformFuncs[(uni)->type])
					((uni)->param,
					(float *) (uni)->data);
					#endif
				}
				else
				{
					#if 0
					((UNIFORM_FUNC) uniformFuncs[(uni)->type])
					((uni)->index,
					(uni)->nElements,
					(float *) (uni)->data);
					#else
					LOG_PRINT("%x %d\n",(uni)->data,(uni)->data);
					((UNIFORM_FUNC) uniformFuncs[(uni)->type])
					(
					(uni)->param,
					(float *) (uni)->data);
					#endif
				}
#else
				switch(uni->type)
				{
                case CONSTANT_TYPE_COUNT:
                    break;
				case CONSTANT_FLOAT:
				case CONSTANT_INT:
				case CONSTANT_BOOL:
					cgGLSetParameter1fv(uni->param,(float *) &uni->data[0]);break;

				case CONSTANT_VEC2:
				case CONSTANT_IVEC2:
				case CONSTANT_BVEC2:
					cgGLSetParameter2fv(uni->param,(float *) &uni->data[0]);break;


				case CONSTANT_VEC3:
				case CONSTANT_IVEC3:
				case CONSTANT_BVEC3:
					cgGLSetParameter3fv(uni->param,(float *) &uni->data[0]);break;

				case CONSTANT_VEC4:
				case CONSTANT_IVEC4:
				case CONSTANT_BVEC4:
					cgGLSetParameter4fv(uni->param,(float *) &uni->data[0]);break;

				case CONSTANT_MAT2:
				case CONSTANT_MAT3:
					break;
				case CONSTANT_MAT4:
    //int s = cgD3D9TypeToSize(cgGetParameterType(uni->param));
    //assert(cgD3D9TypeToSize(cgGetParameterType(uni->param)) == 16 );
	glMatrixMode(GL_PROJECTION);
				checkGlError("");
	//glPushMatrix();
	glLoadIdentity();
				checkGlError("");
	gluPerspective(
		IRenderer::m_fieldOfView,
				   IRenderer::m_aspectRatio,
				   IRenderer::m_zNear, IRenderer::m_zFar);
				checkGlError("");

	glMatrixMode(GL_MODELVIEW);
				checkGlError("");
	//glPushMatrix();
	glLoadIdentity();
				checkGlError("");
	glMultMatrixf((float *) &uni->data[0]);
				checkGlError("");
//cgGLSetMatrixParameterfr(uni->param,(float *) uni->data);break;
#if 0
	glMatrixMode(GL_PROJECTION);
				checkGlError("");
	glPopMatrix();
				checkGlError("");
	glMatrixMode(GL_MODELVIEW);
				checkGlError("");
	glPopMatrix();
				checkGlError("");
#endif
				//default:
					////DBG_HALT;
				};
#endif
				(uni)->dirty = false;
			}
		}/* ???
		for (uint i = 0; i < (shaders[currentShader]).nAttribs; i++)
		{
			AttribShaderGLSLGL3* att = (&(shaders[currentShader]).attribs[0]) + i;
			if ((att)->dirty)
			{
				{
					//((UNIFORM_FUNC) uniformFuncs[(att)->type])((att)->index, (att)->nElements, reinterpret_cast<float*>(&att->data[0]));

   					glVertexAttribPointer ( att->loc, att->offset, GL_FLOAT, GL_FALSE, 0, 0);
				checkGlError(""); 
					glEnableVertexAttribArray (att->loc);
				checkGlError("");


				}
				(att)->dirty = false;
			}
		}*/
	}
}
void RendererCgGL_1_1::setTexture(const char *textureName, const TextureID texture)
{
	if(texture==-1) ////DBG_HALT;
	if(selectedShader == SHADER_NONE) return;

//???	////DBG_HALT;
	int unit = getSamplerUnit(selectedShader, textureName);
	if (unit >= 0)
	{
		selectedTextures[unit] = texture;
		//textures[texture].param=samplerStates[samplerState].param;
	}
}
void RendererCgGL_1_1::setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState
							   		, const int vtstream
		, const VertexBufferID vtbuffer

							   )
{
	if(texture==-1) ////DBG_HALT;
	if(selectedShader == SHADER_NONE) return;

	int unit = getSamplerUnit(selectedShader, textureName);
	if (unit >= 0)
	{
		selectedTextures[unit] = texture;
		selectedSamplerStates[unit] = samplerState;
		textures[texture].param=samplerStates[samplerState].param;
	}
}
const GLenum glPrim[] =
{
	GL_TRIANGLES,
	GL_TRIANGLE_FAN,
	GL_TRIANGLE_STRIP,
	GL_QUADS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_POINTS,
}
;

void RendererCgGL_1_1::DrawPrimitive(Primitives PrimitiveType,UINT StartVertex,UINT PrimitiveCount, const uint flags)
		{
			

			apply();

						

		//LOG_PRINT("=%d\n",getVertexCount(PrimitiveType,PrimitiveCount));

			glDrawArrays(glPrim[PrimitiveType], StartVertex,
#if 0
				PrimitiveCount*getIndicesCount(PrimitiveType)
#else
				getVertexCount(PrimitiveType,PrimitiveCount)
#endif
				);
				checkGlError("");
			
			disApplyTextures();
			reset(flags);
			nDrawCalls++;
		}
		void RendererCgGL_1_1::DrawIndexedPrimitive(Primitives PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount, const uint flags)
		{
			//if((currentIndexBuffer<0)||(currentIndexBuffer>=indexBuffers.size()))return;
			apply();
			#if 0
			LOG_PRINT("=%d\n",			getIndicesCount(PrimitiveType));
			LOG_PRINT("=%d\n",			primCount);
			LOG_PRINT("=%d\n",			(indexBuffers[currentIndexBuffer]).indexSize);
			LOG_PRINT("=%d\n",			BUFFER_OFFSET((indexBuffers[currentIndexBuffer]).indexSize * startIndex));
			#endif

			

			glDrawElements(
			glPrim[PrimitiveType],
			getIndicesCount(PrimitiveType)*primCount,
			((indexBuffers[currentIndexBuffer]).indexSize==2)? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
			BUFFER_OFFSET((indexBuffers[currentIndexBuffer]).indexSize * startIndex));
				checkGlError("");
			
			disApplyTextures();
			reset(flags);
			nDrawCalls++;
		}
void RendererCgGL_1_1::changeShader(const ShaderID shader)
		{
			if (shader != currentShader)
			{
				#if 0
				if (shader == SHADER_NONE)
				{
					__glUseProgramObjectARB(0);
				checkGlError("");
				}
				else
				{
					__glUseProgramObjectARB(shaders[shader]->program);
				checkGlError("");
				}
				#else
				if ((shader == SHADER_NONE)&& (-1 != currentShader))
				{
					cgGLDisableProfile( *shaders[currentShader]->vertexShaderprofile );
					cgGLDisableProfile( *shaders[currentShader]->fragmentShaderprofile );
				}
				else
				{
					cgGLBindProgram( shaders[shader]->vertexShaderprogram );
					cgGLEnableProfile( *shaders[shader]->vertexShaderprofile );
					cgGLBindProgram( shaders[shader]->fragmentShaderprogram );
					cgGLEnableProfile( *shaders[shader]->fragmentShaderprofile );
				}
				#endif
				currentShader = shader;
			}
		}
#if 0
void SetMVP(void* data0)
{
	float* data=(float*)data0;
	if(0)
	{
    glMatrixMode( GL_MODELVIEW );
	//glPushMatrix();
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, -4.0f );
    glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
    glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );

	glMatrixMode( GL_PROJECTION );
	//glPushMatrix();
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f );
	}
}
#endif
void RendererCgGL_1_1::setShaderConstantRaw(const char *name, const void *data, const int size)
		{
			if(selectedShader==-1)return;
			int minUniform = 0;
			int maxUniform = shaders[selectedShader]->nUniforms - 1;
			
			while (minUniform <= maxUniform)
			{
				int currUniform = (minUniform + maxUniform) >> 1;
				int res = strcmp(name, (shaders[selectedShader]->uniforms[currUniform]).name.c_str());
				if (res == 0)
				{
					

			ConstantShaderCgGL3* uni = (&shaders[selectedShader]->uniforms[0]) + currUniform;

			float m[4][4];
			float d[4][4];

			stx_memcpy(d, data, sizeof(float)*16);
			int i,j;
    for (i=0; i<4; i++)
        for (j=0; j<4; j++)
			m[i][j] =
			d[j][i];

					if (stx_memcmp(&uni->data[0], m, size))
					{
						uni->data.resize(size);
						stx_memcpy((void*)&uni->data[0], m, size);

#if 0
if(dynamic_cast<D3DXFROMWINEMATRIX*>((void *)data))
	SetMVP(data);
else if(dynamic_cast<mat4*>((void *)data))
	SetMVP(data);
#endif
						uni->dirty = true;
					}
					return;
				}
				else if (res > 0)
				{
					minUniform = currUniform + 1;
				}
				else
				{
					maxUniform = currUniform - 1;
				}
			}
		}

					int samplerCompareGL2Cg(const void *sampler0, const void *sampler1)
		{
			return strcmp(((SamplerShaderCgGL3 *) sampler0)->name.c_str(), ((SamplerShaderCgGL3 *) sampler1)->name.c_str());
		}
		#if 0
		ConstantType getConstantType(GLenum type)
		{
			switch (type)
			{
				case GL_FLOAT:		return CONSTANT_FLOAT;
				case GL_FLOAT_VEC2_ARB: return CONSTANT_VEC2;
				case GL_FLOAT_VEC3_ARB: return CONSTANT_VEC3;
				case GL_FLOAT_VEC4_ARB: return CONSTANT_VEC4;
				case GL_INT:		return CONSTANT_INT;
				case GL_INT_VEC2_ARB:	return CONSTANT_IVEC2;
				case GL_INT_VEC3_ARB:	return CONSTANT_IVEC3;
				case GL_INT_VEC4_ARB:	return CONSTANT_IVEC4;
				case GL_BOOL_ARB:	return CONSTANT_BOOL;
				case GL_BOOL_VEC2_ARB:	return CONSTANT_BVEC2;
				case GL_BOOL_VEC3_ARB:	return CONSTANT_BVEC3;
				case GL_BOOL_VEC4_ARB:	return CONSTANT_BVEC4;
				case GL_FLOAT_MAT2_ARB: return CONSTANT_MAT2;
				case GL_FLOAT_MAT3_ARB: return CONSTANT_MAT3;
				case GL_FLOAT_MAT4_ARB: return CONSTANT_MAT4;
			}
			return (ConstantType) -1;
		}
		#else
		ConstantType getConstantTypeCg(CGtype type)
		{
			switch (type)
			{
				case CG_BOOL	: return CONSTANT_BOOL;
				case CG_INT		: return CONSTANT_INT;
				case CG_HALF	:
				case CG_FLOAT : return CONSTANT_FLOAT;
				case CG_HALF2				:
				case CG_FLOAT2			:
				return CONSTANT_VEC2;
				case CG_HALF3				:
				case CG_FLOAT3			:
				return CONSTANT_VEC3;
				case CG_HALF4				:
				case CG_FLOAT4			:
				return CONSTANT_VEC4;
				case CG_HALF4x4 		:
				case CG_FLOAT4x4		:
                return CONSTANT_MAT4;
                case CG_UNKNOWN_TYPE:
                case CG_STRUCT:
                case CG_ARRAY:
                case CG_TYPELESS_STRUCT:
                case CG_TYPE_START_ENUM:
                case CG_HALF1x1:
                case CG_HALF1x2:
                case CG_HALF1x3:
                case CG_HALF1x4:
                case CG_HALF2x1:
                case CG_HALF2x2:
                case CG_HALF2x3:
                return CONSTANT_INT;//???
			}
			return (ConstantType) -1;
		}
		#endif

		bool samplerCgCompGL2std(const SamplerShaderCgGL3 & s0, const SamplerShaderCgGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
		bool attribCgCompGL2std(const AttribShaderCgGL3 & s0, const AttribShaderCgGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
		bool constantCgCompGL2std(const ConstantShaderCgGL3 & s0, const ConstantShaderCgGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}

		ShaderID RendererCgGL_1_1::addHLSLShaderVrtl(
			const char *vsText0,
			const char *gsText,
			const char *fsText0,
		const char *vsName,
		const char *psName,
		const int vsLine,
		const int gsLine,
		const int fsLine,
		const char *header,
		const char *extra,
		const char *fileName,
		const char **attributeNames,
		const int nAttributes,
		const uint flags)
		{
			if ( (vsText0 == 0 && fsText0 == 0) || gsText != 0) return SHADER_NONE;


			std::string vsText=vsText0;
			std::string psText=fsText0;
      std::string rm="matrix";
      std::string r= "float4x4";
      replaceAll(vsText,rm,r);
      replaceAll(psText,rm,r);

			const char * mainvs=vsName;
			const char * mainps=psName;
			ShaderCgGL3* shader=new ShaderCgGL3();
			//const GLcharARB *shaderStrings[6];
			int strIndex = 0;
			char line[16];
			GLint vsResult, fsResult;//, linkResult;
			char infoLog[2048];
			GLint infoLogPos = 0;
			if (GL_ARB_shading_language_100_supported)
			{
				static char versionString[16] = {0};
				static bool first = true;
				if (first)
				{
					const char *version = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);
					if (version)
					{
						int major = atoi(version);
                        int minor = atoi(stx_strchr(version, '.') + 1);
                        stx_snprintf(versionString,16, "#version %d%d\n", major, minor);
						LOG_PRINT("%s\n",versionString);
					}
					first = false;
				}
				//shaderStrings[strIndex++] = versionString;
			}
			std::string vsh;
			vsh.append(
			"#define saturate(x) clamp(x,0.0,1.0)\n"
			"#define lerp mix\n");
			if (extra) vsh.append(extra);
			if (header) vsh.append(header);

			//???shaderStrings[strIndex++] = line;
			if (vsText.c_str() != 0)
			{
			vsh.append(
			"#define ROW_MAJOR row_major\n"
			"#define MVPSEMANTIC  : state.matrix.mvp\n"
			"#define WSIGN -\n");

#if 0
CG_PROFILE_MACRO(DX9Vertex11,vs_1_1,VS_1_1,"vs_1_1",6153,1)
CG_PROFILE_MACRO(DX9Vertex20,vs_2_0,VS_2_0,"vs_2_0",6154,1)
CG_PROFILE_MACRO(DX9Vertex2x,vs_2_x,VS_2_X,"vs_2_x",6155,1)
CG_PROFILE_MACRO(DX9Vertex2sw,vs_2_sw,VS_2_SW,"vs_2_sw",6156,1)
CG_PROFILE_MACRO(DX9Vertex30,vs_3_0,VS_3_0,"vs_3_0",6157,1)
CG_PROFILE_MACRO(DX9VertexHLSL,hlslv, HLSLV,"hlslv",6158,1)
CG_PROFILE_MACRO(DX9Pixel11,ps_1_1,PS_1_1,"ps_1_1",6159,0)
CG_PROFILE_MACRO(DX9Pixel12,ps_1_2,PS_1_2,"ps_1_2",6160,0)
CG_PROFILE_MACRO(DX9Pixel13,ps_1_3,PS_1_3,"ps_1_3",6161,0)
CG_PROFILE_MACRO(DX9Pixel20,ps_2_0,PS_2_0,"ps_2_0",6162,0)
CG_PROFILE_MACRO(DX9Pixel2x,ps_2_x,PS_2_X,"ps_2_x",6163,0)
CG_PROFILE_MACRO(DX9Pixel2sw,ps_2_sw,PS_2_SW,"ps_2_sw",6164,0)
CG_PROFILE_MACRO(DX9Pixel30,ps_3_0,PS_3_0,"ps_3_0",6165,0)
CG_PROFILE_MACRO(DX9PixelHLSL,hlslf,HLSLF,"hlslf",6166,0)
CG_PROFILE_MACRO(DX10Vertex40,vs_4_0,VS_4_0,"vs_4_0",6167,1)
CG_PROFILE_MACRO(DX10Pixel40,ps_4_0,PS_4_0,"ps_4_0",6168,0)
CG_PROFILE_MACRO(Generic,           generic, GENERIC, "generic", 7002,0)

CG_PROFILE_MACRO(Vertex,vp20,VP20,"vp20",6146,1)
CG_PROFILE_MACRO(Fragment20,fp20,FP20,"fp20",6147,0)
CG_PROFILE_MACRO(Vertex30,vp30,VP30,"vp30",6148,1)
CG_PROFILE_MACRO(Fragment,fp30,FP30,"fp30",6149,0)
CG_PROFILE_MACRO(ARBVertex,arbvp1,ARBVP1,"arbvp1",6150,1)
CG_PROFILE_MACRO(Fragment40,fp40,FP40,"fp40",6151,0)
CG_PROFILE_MACRO(ARBFragment,arbfp1,ARBFP1,"arbfp1",7000,0)
CG_PROFILE_MACRO(Vertex40,vp40,VP40,"vp40",7001,1)
CG_PROFILE_MACRO(GLSLVertex,glslv,GLSLV,"glslv",7007,1)
CG_PROFILE_MACRO(GLSLFragment,glslf,GLSLF,"glslf",7008,0)
CG_PROFILE_MACRO(GLSLCombined, glslc, GLSLC, "glslc", 7009, 0)
CG_PROFILE_MACRO(GPUFragment,gpu_fp,GPU_FP,"gp4fp",7010,0)
CG_PROFILE_MACRO(GPUVertex,gpu_vp,GPU_VP,"gp4vp",7011,1)
CG_PROFILE_MACRO(GPUGeometry,gpu_gp,GPU_GP,"gp4gp",7012,0)
#endif
				#if 1
				CGprofile vertexShaderprofile;
				if( cgGLIsProfileSupported(CG_PROFILE_VP40) )
				{
				vertexShaderprofile = CG_PROFILE_VP40;
				LOG_PRINT("VP40\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_VP30) )
				{
				vertexShaderprofile = CG_PROFILE_VP30;
				LOG_PRINT("VP30\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_VP20) )
				{
				vertexShaderprofile = CG_PROFILE_VP20;
				LOG_PRINT("VP20\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_ARBVP1) )
				{
				vertexShaderprofile = CG_PROFILE_ARBVP1;
				LOG_PRINT("ARBVP1\n");
				}
				else
				{
					LOG_PRINT("Failed to initialize vertex shader! Hardware doesn't support any of the required vertex shading extensions!\n"
					);
					////DBG_HALT;
					return -1;
				}
				shader->vertexShaderprofile=new CGprofile(vertexShaderprofile);

				if(extra)vsh.append(extra);
				vsh.append(vsText.c_str());
 				LOG_PRINT("\n%s\n",vsh.c_str());
				shader->vertexShaderprogram = cgCreateProgram( g_CgContext,
				CG_SOURCE,
				vsh.c_str(),
				*shader->vertexShaderprofile,
				mainvs,
				0 );
				#else
				if (vsResult)
				{
				}
				else
				{
                    infoLogPos += stx_snprintf(infoLog + infoLogPos, "Vertex shader error:\n");
				}
				infoLogPos += len;
				#endif
			}
			else vsResult = GL_TRUE;
			if (psText.c_str() != 0)
			{
                stx_snprintf(line,16, "#line %d\n", fsLine);
				//shaderStrings[strIndex] = psText.c_str();
				CGprofile fragmentShaderprofile;
				#if 1
				if( cgGLIsProfileSupported(CG_PROFILE_FP40) )
				{
				fragmentShaderprofile = CG_PROFILE_FP40;
				LOG_PRINT("FP40\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_FP30) )
				{
				fragmentShaderprofile = CG_PROFILE_FP30;
				LOG_PRINT("FP30\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_FP20) )
				{
				fragmentShaderprofile = CG_PROFILE_FP20;
				LOG_PRINT("FP20\n");
				}
				else if( cgGLIsProfileSupported(CG_PROFILE_ARBFP1) )
				{
				fragmentShaderprofile = CG_PROFILE_ARBFP1;
				LOG_PRINT("ARBFP1\n");
				}

				else
				{
					LOG_PRINT("Failed to initialize pixel shader! Hardware doesn't support any of the required pixel shading extensions!\n"
					);
					////DBG_HALT;
					return -1;
				}
				shader->fragmentShaderprofile = new CGprofile(fragmentShaderprofile);
				std::string psh;
			psh.append(
			"#define saturate(x) clamp(x,0.0,1.0)\n"
			"#define lerp mix\n");
			if (extra) psh.append(extra);
			if (header) psh.append(header);

				psh.append(psText.c_str());
 				LOG_PRINT("\n%s\n",psh.c_str());
				shader->fragmentShaderprogram = cgCreateProgram( g_CgContext,
				CG_SOURCE,
				psh.c_str(),
				*shader->fragmentShaderprofile,
				mainps,
				0 );
				#else
				if (fsResult)
				{
				}
				else
				{
                    infoLogPos += stx_snprintf(infoLog + infoLogPos, "Fragment shader error:\n");
				}
				infoLogPos += len;
				#endif
			}
			else fsResult = GL_TRUE;
			#if 0
			if (vsResult && fsResult)
			{
				for (int i = 0; i < nAttributes; i++)
				{
					if (attributeNames[i]) glBindAttribLocationARB(shader->program, i, attributeNames[i]);
				}
				infoLogPos += len;
				LOG_PRINT("%s", infoLog);
				if (linkResult)
				{
					GLuint currProgram = (currentShader == SHADER_NONE)? 0 : shaders[currentShader]->program;
					#else
					{
						{
							#endif
							GLint uniformCountv,uniformCountf;//, maxLength;
							std::vector<SamplerShaderCgGL3	> samplers;
							std::vector<ConstantShaderCgGL3 > uniforms;
							std::vector<AttribShaderCgGL3 > attribs;
							
							CgReflection* pv=new CgReflection();
							pv->parse(shader->vertexShaderprogram,uniformCountv);
							
							CgReflection* pf=new CgReflection();
							pf->parse(shader->fragmentShaderprogram,uniformCountf);
							
							GLint uniformCount=pv->tu.size()+pf->tu.size();
							samplers.resize(uniformCount);
							uniforms.resize(uniformCount);
							attribs.resize(uniformCount);
							int nSamplers = 0;
							int nUniforms = 0;
							int nAttribs = 0;
							#if 1
							CgReflection* pp=pv;
							
#if 0
// ???

{
GLint prog=shader.program;
GLint maxAttribNameLength,numActiveAttribs = 0;
glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);

std::vector<GLchar> nameData(maxAttribNameLength);
for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
{
	GLint arraySize = 0;
	GLenum type = 0;
	GLsizei actualLength = 0;
	glGetActiveAttrib(prog, attrib, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
	std::string name((char*)&nameData[0], actualLength - 1);
  	GLint loc = glGetAttribLocation ( prog, name.c_str());

	attribs[nAttribs].loc=loc;
	attribs[nAttribs].offset=attrib;
	attribs[nAttribs].index = nAttribs;
	attribs[nAttribs].name=std::string( name);
	nAttribs++;

   // Load the vertex position
   //glVertexAttribPointer ( loc, 3, GL_FLOAT, GL_FALSE, stride, vVec);
   //glVertexAttribPointer ( loc, 2, GL_FLOAT, GL_FALSE, stride, &vVec[3] );

   //glEnableVertexAttribArray ( loc );
   ////glDisableVertexAttribArray ( loc );
}}

#endif
							for (int ii = 0; ii < uniformCount; ii++)
							{
								int i=ii;
								if(i>=pv->tu.size())
								{
									pp=pf;
									i=i-pv->tu.size();
								}
								GLint length=pp->n[i].length();
								if (pp->tu[i] != -1)
								{
									
									samplers[nSamplers];
									samplers[nSamplers].index = nSamplers;
									samplers[nSamplers].param=pp->p[i];
									samplers[nSamplers].name=std::string( pp->n[i].c_str());
									nSamplers++;
									
								}
								else
								{
									{
										{
											
											uniforms[nUniforms];
																						uniforms[nUniforms].index = nUniforms;
											uniforms[nUniforms].type = getConstantTypeCg(pp->t[i]);
											uniforms[nUniforms].nElements = pp->rows[i]*pp->columns[i];
											uniforms[nUniforms].param=pp->p[i];
											uniforms[nUniforms].name=std::string(pp->n[i].c_str());
											nUniforms++;
											
										}
									}
								}
							}
							delete pv;
							delete pf;
							
							#else
							#endif
							#if 0
							__glUseProgram(currProgram);
							#else
							cgGLLoadProgram( shader->vertexShaderprogram );
							cgGLLoadProgram( shader->fragmentShaderprogram);
							cgGLBindProgram( shader->vertexShaderprogram );
							cgGLEnableProfile( *shader->vertexShaderprofile );
							cgGLBindProgram( shader->fragmentShaderprogram );
							cgGLEnableProfile( *shader->fragmentShaderprofile );
							#endif
							samplers.resize(nSamplers);
							uniforms.resize(nUniforms);
							attribs.resize(nAttribs);
							#if 1//???
							
							if(uniforms.size()>1) std::sort(uniforms.begin(), uniforms.end(), constantCgCompGL2std);
							
							if(samplers.size()>1) std::sort(samplers.begin(), samplers.end(), samplerCgCompGL2std);
							if(attribs.size()>1) std::sort(attribs.begin(), attribs.end(), attribCgCompGL2std);
							
							#endif
							for (int i = 0; i < nUniforms; i++)
							{
								int constantSize = constantTypeSizes[uniforms[i].type] * uniforms[i].nElements;
								uniforms[i].data .resize(constantSize);
								memset((void*)&uniforms[i].data[0], 0, constantSize);
								uniforms[i].dirty = false;
							}
							shader->attribs  = attribs;
							shader->uniforms  = uniforms;
							shader->samplers  = samplers;
							shader->nAttribs = nAttribs;
							shader->nUniforms = nUniforms;
							shader->nSamplers = nSamplers;
							shaders.push_back(shader);

							return shaders.size()-1;
						}
					}
#ifdef _MSC_VER
ErrorMsg(infoLog);
#else
					LOG_PRINT("\n%s\n",infoLog);
#endif
					return SHADER_NONE;
				}
				static CGparameter currTexParam[MAX_TEXTUREUNIT];
				//static bool valid[MAX_TEXTUREUNIT];

				void RendererCgGL_1_1::applyTextures()
				{
					LOG_FNLN;
					if(selectedShader==-1)return;
					LOG_FNLN;
					if(!shaders.size())return;
					LOG_FNLN;
					if (!shaders[selectedShader]->samplers.size())return;

//if (selectedShader==2) LOG_PRINT("2\n");

					LOG_FNLN;
					for (uint i = 0; i < MAX_TEXTUREUNIT; i++)
					{
						TextureID texture = selectedTextures[i];
//						TextureID currTex = currentTextures[i];
if (texture != TEXTURE_NONE)
							{
						int su=-1;
						LOG_FNLN;
						for(unsigned int iii=0;iii<shaders[selectedShader]->samplers.size();iii++)
						{
						su=getSamplerUnit(selectedShader,shaders[selectedShader]->samplers[iii].name.c_str());
						if(su!=-1)break;
						}
                        if(su==-1)// &&(texture == currTex))
						{
							return;//???DBG_HALT;
						}

						LOG_FNLN;
				if(texture+1>textures.size())
					continue;
						textures[texture].param=shaders[selectedShader]->samplers[su].param;

	cgGLSetTextureParameter( textures[texture].param, textures[texture].glTexID );
	cgGLEnableTextureParameter( textures[texture].param );
LOG_PRINT("cgGLEnableTextureParameter %d\n",texture);
							currentTextures[i] = texture;
						}
					}
				}

				void RendererCgGL_1_1::disApplyTextures()
				{
					for (uint i = 0; i < MAX_TEXTUREUNIT; i++)
					{
						//TextureID texture = selectedTextures[i];
						TextureID currTex = currentTextures[i];
						if( -1 != currTex)
						{
LOG_PRINT("cgGLDisableTextureParameter %d\n",currTex);								cgGLDisableTextureParameter( textures[currTex].param );
								//currentTextures[i]=-1;
						}
					}
				}

int RendererCgGL_1_1::getSamplerUnit(const ShaderID shader, const char *samplerName) const
				{
                    //ASSERT(shader != SHADER_NONE);
					//std::vector<SamplerShaderCgGL3 >samplers = (shaders[shader]->samplers;
					int minSampler = 0;
					int maxSampler = shaders[shader]->nSamplers - 1;
					while (minSampler <= maxSampler)
					{
						int currSampler = (minSampler + maxSampler) >> 1;
						int res = strcmp(samplerName, (shaders[shader]->samplers[currSampler]).name.c_str());
						if (res == 0)
						{
							int r= (shaders[shader]->samplers[currSampler]).index;//???
							return r;
						}
						else if (res > 0)
						{
							minSampler = currSampler + 1;
						}
						else
						{
							maxSampler = currSampler - 1;
						}
					}
					return -1;
				}
void MyCgErrorCallback( void );

		RendererCgGL_1_1::RendererCgGL_1_1(int awidth, int aheight, 
		bool InitLogFile,
			bool fullScreen,
int argc, char *argv[]
) : mOk(false), RendererGL_1_1(awidth, aheight,InitLogFile,fullScreen,argc,argv)
{
	//std::string str=miscParams["window"]
	//


#ifdef _MSC_VER
	GLuint PixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize	   = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	#if 0
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	#else
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	#endif
	pfd.iLayerType = PFD_MAIN_PLANE;
	HWND w=STX_Service::GetWindowInstance()->GetHWND();
	hDc = GetDC( w );
	PixelFormat = ChoosePixelFormat( m_hDC, &pfd );
	SetPixelFormat( m_hDC, PixelFormat, &pfd);

	hGlrc = wglCreateContext( hDc);
	
	wglMakeCurrent( m_hDC, hGlrc );
	
	//???initExtensions(hDc);
	importGLInit();
	
	#else
	//initGLee();
#if 0
	if (info.subsystem!=SDL_SYSWM_X11)
	{
		LOG_PRINT("System is not X11!\n");
		LOG_PRINT("%d!=%d\n", (int)info.subsystem , (int)SDL_SYSWM_X11 <<'\n');
		return;
	}

	if(!dpy)
	{
		m_dpy=info.info.x11.display;
		m_win=info.info.x11.window;
		int errorbase,tEventbase;
		if (glXQueryExtension(dpy,&errorbase,&tEventbase) == False)

		{
			LOG_PRINT("OpenGL through GLX not supported.\n");
			return;
		}
		int configuration[]=
		{
			GLX_DOUBLEBUFFER,GLX_RGBA,GLX_DEPTH_SIZE ,12, GLX_RED_SIZE,1,
			GLX_BLUE_SIZE,1,GLX_GREEN_SIZE,1,None
		}
		;
		XVisualInfo *vi=glXChooseVisual(dpy,DefaultScreen(dpy),configuration);
		if(vi== 0)
		{
			LOG_PRINT("Could not initialize Visual.\n");
			return;
		}
		ctx=glXCreateContext(dpy,vi,0,True);
		if(ctx== 0)
		{
			LOG_PRINT("Could not initialize GL context.\n");
			return;
		}
		if (!glXMakeCurrent(m_dpy,m_win,ctx))
		{
			m_dpy=0;
			return;
		}
	}
#endif
	Display* d=STX_Service::GetWindowInstance()->GetDisplay();
	//initExtensions(d);
	//
	
	#endif

			GLint units = 1;
			if (GL_ARB_fragment_shader_supported || GL_ARB_fragment_program_supported)
			{
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &units);
				checkGlError("");
			}
			else
			{
				glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
				checkGlError("");
			}
			nImageUnits = units;
			memset(&textureLod, 0, sizeof(textureLod));
			if (GL_ARB_draw_buffers_supported)
			{
				GLint mrt = 1;
				glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &mrt);
				checkGlError("");
				nMRTs = mrt;
			}
			if (nMRTs > MAX_MRTS) nMRTs = MAX_MRTS;
			for (uint i = 0; i < nMRTs; i++)
			{
				drawBuffers[i] = GL_COLOR_ATTACHMENT0_EXT + i;
			}
			if (GL_EXT_texture_filter_anisotropic_supported)
			{
				GLint aniso = 1;
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
				checkGlError("");
				maxAnisotropic = aniso;
			}
			fbo = 0;
			currentFrontFace = GL_CW;
			currentVBO = 0;
			texVF	= VF_NONE;
			plainShader = SHADER_NONE;
			plainVF = VF_NONE;
			setGLdefaults();
			resetToDefaults();
			memset(&activeVertexFormat, VF_NONE, sizeof(activeVertexFormat));
			#if 0
			uniformFuncs[CONSTANT_FLOAT] = (void *) __glUniform1fvARB;
			uniformFuncs[CONSTANT_VEC2]  = (void *) __glUniform2fvARB;
			uniformFuncs[CONSTANT_VEC3]  = (void *) __glUniform3fvARB;
			uniformFuncs[CONSTANT_VEC4]  = (void *) __glUniform4fvARB;
			uniformFuncs[CONSTANT_INT]   = (void *) __glUniform1ivARB;
			uniformFuncs[CONSTANT_IVEC2] = (void *) __glUniform2ivARB;
			uniformFuncs[CONSTANT_IVEC3] = (void *) __glUniform3ivARB;
			uniformFuncs[CONSTANT_IVEC4] = (void *) __glUniform4ivARB;
			uniformFuncs[CONSTANT_BOOL]  = (void *) __glUniform1ivARB;
			uniformFuncs[CONSTANT_BVEC2] = (void *) __glUniform2ivARB;
			uniformFuncs[CONSTANT_BVEC3] = (void *) __glUniform3ivARB;
			uniformFuncs[CONSTANT_BVEC4] = (void *) __glUniform4ivARB;
			uniformFuncs[CONSTANT_MAT2]  = (void *) __glUniformMatrix2fvARB;
			uniformFuncs[CONSTANT_MAT3]  = (void *) __glUniformMatrix3fvARB;
			uniformFuncs[CONSTANT_MAT4]  = (void *) __glUniformMatrix4fvARB;
			#elif 0

			uniformFuncs[CONSTANT_FLOAT] = (CGGL_API void* CGGLENTRY) cgGLSetParameter1fv;
			uniformFuncs[CONSTANT_VEC2]  = (CGGL_API void* CGGLENTRY) cgGLSetParameter2fv;
			uniformFuncs[CONSTANT_VEC3]  = (CGGL_API void* CGGLENTRY) cgGLSetParameter3fv;
			uniformFuncs[CONSTANT_VEC4]  = (CGGL_API void* CGGLENTRY) cgGLSetParameter4fv;
			uniformFuncs[CONSTANT_INT]   = (CGGL_API void* CGGLENTRY) cgGLSetParameter1fv;
			uniformFuncs[CONSTANT_IVEC2] = (CGGL_API void* CGGLENTRY) cgGLSetParameter2fv;
			uniformFuncs[CONSTANT_IVEC3] = (CGGL_API void* CGGLENTRY) cgGLSetParameter3fv;
			uniformFuncs[CONSTANT_IVEC4] = (CGGL_API void* CGGLENTRY) cgGLSetParameter4fv;
			uniformFuncs[CONSTANT_BOOL]  = (CGGL_API void* CGGLENTRY) cgGLSetParameter1fv;
			uniformFuncs[CONSTANT_BVEC2] = (CGGL_API void* CGGLENTRY) cgGLSetParameter2fv;
			uniformFuncs[CONSTANT_BVEC3] = (CGGL_API void* CGGLENTRY) cgGLSetParameter3fv;
			uniformFuncs[CONSTANT_BVEC4] = (CGGL_API void* CGGLENTRY) cgGLSetParameter4fv;
			uniformFuncs[CONSTANT_MAT2]  = (CGGL_API void* CGGLENTRY) cgGLSetMatrixParameterfr;
			uniformFuncs[CONSTANT_MAT3]  = (CGGL_API void* CGGLENTRY) cgGLSetMatrixParameterfr;
			uniformFuncs[CONSTANT_MAT4]  = (CGGL_API void* CGGLENTRY) cgGLSetMatrixParameterfr;
			#endif
			bsupportsHalf=false;
			bsupportsR2VB=false;
			overrideR2VB=true;
			overrideHalf=true;


		//defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);

				g_CgContext = cgCreateContext();
  cgGLSetDebugMode(CG_FALSE);
  cgSetParameterSettingMode(g_CgContext, CG_DEFERRED_PARAMETER_SETTING);
				cgGLRegisterStates(g_CgContext);
				cgSetErrorCallback(MyCgErrorCallback);
				;
LOG_PRINT("CgGL_1_1Renderer\n");
			mOk=true;
		}

		RendererCgGL_1_1::~RendererCgGL_1_1()
		{
			STX_TRY;
			apply();
			for (uint i = 0; i < shaders.getCount(); i++)
			{
				for (uint j = 0; j < shaders[i]->nSamplers; j++)
				{
					//delete shaders[i]->samplers[j].name;
				}
				for (uint j = 0; j < shaders[i]->nUniforms; j++)
				{
					//delete shaders[i]->uniforms[j].name;
					shaders[i]->uniforms[j].data.clear();
				}
				for (uint j = 0; j < shaders[i]->nAttribs; j++)
				{
					shaders[i]->attribs[j].data.clear();
				}
				shaders[i]->attribs.clear();
				shaders[i]->samplers.clear();
				shaders[i]->uniforms.clear();
				#if 0
				__glDeleteObjectARB(shaders[i]->vertexShader);
				__glDeleteObjectARB(shaders[i]->fragmentShader);
				__glDeleteObjectARB(shaders[i]->program);
				#else
				cgDestroyProgram(shaders[i]->vertexShaderprogram);
				cgDestroyProgram(shaders[i]->fragmentShaderprogram);
				#endif
			}
#ifdef _MSC_VER
			for (uint i = 0; i < vertexBuffers.getCount(); i++)
			{
				glDeleteBuffersARB(1, &(vertexBuffers[i]).vboVB);
				checkGlError("");
			}
			for (uint i = 0; i < indexBuffers.getCount(); i++)
			{
				glDeleteBuffersARB(1, &(indexBuffers[i]).vboIB);
				checkGlError("");
			}

			if (fbo) {glDeleteFramebuffersEXT(1, &fbo);
				checkGlError("");}
#endif
for (uint i = 0; i < textures.getCount(); i++)
			{
				removeTexture(i);
			}
		shaders.clear();
		textures.clear();
		vertexBuffers.clear();
		indexBuffers.clear();
		vertexFormats.clear();
		samplerStates.clear();
		blendStates.clear();
		alphaStates.clear();
		depthStates.clear();
		rasterizerStates.clear();	
		STX_CATCH;
		}

#endif
#if 0
		struct ShaderCg : public SShader
{
virtual void DumpUniforms();
virtual CGCGbool interestingCgProgramParameter(CGparameter parameter)
{
  return cgIsParameterReferenced(parameter);
}
#if 1
virtual CGCGbool interestingCgProgramParameters(CGprogram program, CGenum nameSpace)
{
  CGparameter parameter;
  for (parameter = cgGetFirstParameter(program, nameSpace);
    parameter;
    parameter = cgGetNextParameter(parameter)) {
      if (interestingCgProgramParameter(parameter)) {
	return CGCG_TRUE;
      }
    }
    return CGCG_FALSE;
}
#endif
	virtual void saveCgProgram(CGprogram program, CGCGbool unabridged,
									std::vector<ShaderParameter*>& v,
									std::vector<std::string>& s,
									std::vector<int>& i,
  std::vector<int>& tu,
  int& tui);
virtual void saveCgPrograms(CGprogram program,CGcontext context, CGCGbool unabridged,
									std::vector<ShaderParameter*>& v,
									std::vector<std::string>& s,
									std::vector<int>& i,
  std::vector<int>& tu,
  int& tui);
	virtual void SetInt( const char* n, const int f){setShaderConstant1i(n,f);}
	virtual void SetFloat( const char* n, const float f){setShaderConstant1f(n,f);}
	virtual void SetMatrix( const char* n, const mat4* f)
	{
		setShaderConstant4x4f(n,*f);
		//setMVPMatrix(n,f);//???
	}
	virtual void SetVector( const char* n, const float4 f){setShaderConstant4f(n,f);}
	virtual void setShaderConstant1i(const char *name, const int constant);
	virtual void setShaderConstant1f(const char *name, const float constant);
	virtual void setShaderConstant2f(const char *name, const vec2 &constant);
		virtual void setShaderConstant3f(const char *name, const vec3 &constant);
	virtual void setShaderConstant4f(const char *name, const vec4 &constant);
	virtual void setShaderConstant4x4f(const char *name, const mat4 &constant);
	virtual void setShaderConstant1i(const int idx, const int constant);
	virtual void setShaderConstant1f(const int idx, const float constant);
	virtual void setShaderConstant2f(const int idx, const vec2 &constant);
		virtual void setShaderConstant3f(const int idx, const vec3 &constant);
	virtual void setShaderConstant4f(const int idx, const vec4 &constant);
	virtual void setShaderConstant4x4f(const int idx, const mat4 &constant);
	virtual void setShaderConstant1i(ShaderParameter* param, const int constant);
	virtual void setShaderConstant1f(ShaderParameter* param, const float constant);
	virtual void setShaderConstant2f(ShaderParameter* param, const vec2 &constant);
		virtual void setShaderConstant3f(ShaderParameter* param, const vec3 &constant);
	virtual void setShaderConstant4f(ShaderParameter* param, const vec4 &constant);
	virtual void setShaderConstant4x4f(ShaderParameter* param, const mat4 &constant);
	int getSamplerUnit(int shader, const char* textureName)
	{
		//DBG_HALT;
		for(int i=0;i<FProgramParameterNamesSize();i++)
		{
			if(
                stx_strncmp(
				FProgramParameterNames[i].c_str(),
				textureName
                ,stx_strlen(textureName)+1
				)==0)
			{
				return i;
			}
		}
		for(int i=0;i<VProgramParameterNamesSize();i++)
		{
			if(
                stx_strncmp(
				VProgramParameterNames[i].c_str(),
				textureName
                ,stx_strlen(textureName)+1
				)==0)
			{
				return ftui+i;//???
			}
		}
		return -1;
	}
	bool vertexS;
	CGeffect m_CgEffect;
    CGtechnique m_CgTechnique;
	CGprofile m_VProfile;
	CGprofile m_FProfile;
	CGprogram m_ProgramV;
	CGprogram m_ProgramF;
	std::string m_ProgramVN;
	std::string m_ProgramFN;
	int vtui,ftui;
	ShaderCg():SShader(),vtui(0),ftui(0),vertexS(false){}
	virtual ~ShaderCg();
	/*
	virtual ShaderID instance(const char* filename)=0;
	virtual ShaderID instance(const char* fileVS,const char* fileFS,const char* mainVS,const char* mainFS)=0;
	virtual ShaderID instanceTxt(const char* txtVS,const char* txtFS,const char* mainVS,const char* mainFS)=0;
*/
	virtual const char *parameterClassString(CGparameterclass pc);
virtual void dumpCgParameterInfo(CGparameter parameter, int unabridged);
virtual void dumpCgProgramParameter(CGparameter parameter, CGCGbool unabridged);
virtual void dumpCgProgramParameters(CGprogram m_Program,
				    CGenum nameSpace,
				    char const *nameSpaceName,
				    CGCGbool unabridged);
virtual void saveCgVProgramParameters();
virtual void saveCgFProgramParameters();
virtual void saveCgParameterInfo(CGparameter parameter, int unabridged,
									std::vector<ShaderParameter*>& v,
									std::vector<std::string>& s,
									std::vector<int>& i,
  std::vector<int>& tu,
  int& tui
									);
virtual void saveCgProgramParameter(CGparameter parameter, CGCGbool unabridged,
									std::vector<ShaderParameter*>& v,
									std::vector<std::string>& s,
									std::vector<int>& i,
  std::vector<int>& tu,
  int& tui									);
virtual void saveCgProgramParameters(CGprogram m_Program,
				    CGenum nameSpace,
				    char const *nameSpaceName,
				    CGCGbool unabridged,
									std::vector<ShaderParameter*>& v,
									std::vector<std::string>& s,
									std::vector<int>& i,
  std::vector<int>& tu,
  int& tui									);
};
#endif

