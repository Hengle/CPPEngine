/*
SwapBuffers

Framework3.lib(RendererGLES2.obj) : error LNK2019: unresolved external symbol __imp__SetPixelFormat@12 referenced in function "public: __thiscall RendererGLES2::RendererGLES2(int,int,bool,bool,int,char * * const)" (??0RendererGLES2@@QAE@HH_N0HQAPAD@Z)
Framework3.lib(RendererGLES2.obj) : error LNK2019: unresolved external symbol __imp__ChoosePixelFormat@8 referenced in function "public: __thiscall RendererGLES2::RendererGLES2(int,int,bool,bool,int,char * * const)" (??0RendererGLES2@@QAE@HH_N0HQAPAD@Z)

Framework3.lib(RendererGLES.obj) : error LNK2019: unresolved external symbol __imp__glMapBufferOES@8 referenced in function "public: virtual unsigned char * __thiscall IndexBufferGLES::Lock(void)" (?Lock@IndexBufferGLES@@UAEPAEXZ)
Framework3.lib(RendererGLES.obj) : error LNK2019: unresolved external symbol __imp__glUnmapBufferOES@4 referenced in function "public: virtual void __thiscall IndexBufferGLES::Unlock(void)" (?Unlock@IndexBufferGLES@@UAEXXZ)

*/
/* * * * * * * * * * * * * Author's note * * * * * * * * * * * ***
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

#include <Framework3/RendererGLES2.h>
#ifdef  __FrameworkGLES2_H__

//include <Framework3/TestFBO.h>
typedef GLvoid (APIENTRY *UNIFORM_FUNC)(GLint location, GLsizei count, const void *value);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

#if 0//defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define LOG_FNLN_X LOG_FNLN
#define LOG_PRINT_X LOG_PRINT_X
#else
#define LOG_FNLN_X
#define LOG_PRINT_X
#endif

//void RendererGL_1_1::
		void RendererGLES2::changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBuffer, const intptr offset)
		{
			VertexFormatGL cvf = vertexFormats[selectedVertexFormat];
			char *base = (char *)vertexBuffers[vertexBuffer].pVertices;

#if 0//!defined(USE_HLSL_SHADERS)

			VertexFormatGL cvf = vertexFormats[selectedVertexFormat];

			int vertexSize = cvf.vertexSize[stream];

			if(!vertexSize) return;

			int nGeneric=cvf.maxGeneric;

			int nAttribs=cvf.maxGeneric;

			int nTexCoord=cvf.maxTexCoord;

			char *base = (char *)&vertexBuffers[vertexBuffer].data[0];//(char *) offset;

			const GLsizei glTypes[] =
			{
				GL_FLOAT,
				0, GL_UNSIGNED_BYTE,
			};

			for (uint i = 0; i < nAttribs; i++)
			{

				uint loc=shaders[selectedShader].attribs[i].loc;
				glVertexAttribPointer(loc, cvf.generic[i].size, glTypes[cvf.generic[i].format], GL_FALSE, vertexSize, base + cvf.generic[i].offset);
				checkGlError("");
				glEnableVertexAttribArray (loc);
				checkGlError("");
			}


			currentVertexBuffers[stream] = vertexBuffer;

			currentOffsets[stream] = offset;

			activeVertexFormat[stream] = //current
			selectedVertexFormat;

#else
			const GLsizei glTypes[] =
			{
				GL_FLOAT,
				0, GL_UNSIGNED_BYTE,
			}
			;
			GLuint vbo = 0;
			//char *base = 0;
			//if(stream==0) base=(char *)&vertexBuffers[vertexBuffer].data[0];

			if (vertexBuffer != VB_NONE) vbo = (vertexBuffers[vertexBuffer]).vboVB;
			/*
			if (vbo != currentVBO)
			{
				glBindBuffer(GL_ARRAY_BUFFER, currentVBO = vbo);
				checkGlError("");
			}
			*/
			//if (vertexBuffer != currentVertexBuffers[stream] || offset != currentOffsets[stream] || currentVertexFormat != activeVertexFormat[stream])
			{
				if (currentVertexFormat != VF_NONE)
				{

					//char *base = (char *) offset;

					VertexFormatGL cvf = vertexFormats[currentVertexFormat];

					int vertexSize = cvf.vertexSize[stream];
					//base = (char *) cvf.pBase;
#if 0
					if (cvf.vertex.stream == stream && cvf.vertex.size)
					{

						LOG_PRINT_X("vertex.glVertexAttribPointer:%d,%d,%d,%d,%d,%d:%s:%d\n",ii, cvf.vertex.size, glTypes[cvf.vertex.format], GL_FALSE, vertexSize, base + cvf.vertex.offset,__FUNCTION__,__LINE__);
							glVertexAttribPointer(ii, cvf.vertex.size, glTypes[cvf.vertex.format], GL_FALSE, vertexSize, base + cvf.vertex.offset);
						checkGlError("");
						ii++;
					}
					if (cvf.normal.stream == stream && cvf.normal.size)
					{

						LOG_PRINT_X("normal.glVertexAttribPointer:%d,%d,%d,%d,%d,%d:%s:%d\n",ii, cvf.vertex.size, glTypes[cvf.normal.format], GL_FALSE, vertexSize, base + cvf.normal.offset,__FUNCTION__,__LINE__);
							glVertexAttribPointer(ii, cvf.vertex.size, glTypes[cvf.normal.format], GL_FALSE, vertexSize, base + cvf.normal.offset);
						checkGlError("");
						ii++;
					}
#endif
					int ii=0;
					for (int i = 0; i < MAX_GENERIC; i++)
						if (cvf.generic[i].stream == stream && cvf.generic[i].size) ii++;
					ii--;
					for (int i = 0; i < MAX_GENERIC; i++)
					{

						if (cvf.generic[i].stream == stream && cvf.generic[i].size)
						{
			
                            glVertexAttribPointer(ii, cvf.generic[i].size, glTypes[cvf.generic[i].format], GL_FALSE, vertexSize, base + cvf.generic[i].offset);

/*
void glVertexAttribPointer(GLuint index,  GLint size,  GLenum type,  GLboolean normalized,  GLsizei stride,  const GLvoid * pointer);

GL_APICALL void         GL_APIENTRY glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr);
*/
LOG_FNLN;
LOG_PRINT("glVertexAttribPointer:\n");
LOG_PRINT("index=%d\n",ii);
LOG_PRINT("size=%d,%d\n",i, cvf.generic[i].size);
LOG_PRINT("type=%d\n",glTypes[cvf.generic[i].format]);
LOG_PRINT("stride=%d\n", vertexSize);
LOG_PRINT("base=%x\n", base);
LOG_PRINT("pointer=%x\n", base + cvf.generic[i].offset);

							checkGlError("");
							ii--;
						}
					}
#if 0
					for (int i = 0; i < MAX_TEXCOORD; i++)
					{

						if (cvf.texCoord[i].stream == stream && cvf.texCoord[i].size)
						{

							//glClientActiveTexture(GL_TEXTURE0 + i);
							//glTexCoordPointer
							LOG_PRINT_X("MAX_TEXCOORD;glVertexAttribPointer:%d,%d,%d,%d,%d,%d:%s:%d\n",ii+i, cvf.texCoord[i].size, glTypes[cvf.texCoord[i].format], GL_FALSE, vertexSize, base + cvf.texCoord[i].offset,__FUNCTION__,__LINE__);
							glVertexAttribPointer(ii+i, cvf.texCoord[i].size, glTypes[cvf.texCoord[i].format], GL_TRUE, vertexSize, base + cvf.texCoord[i].offset);
							checkGlError("");
						}
					}
#endif
				}

				currentVertexBuffers[stream] = vertexBuffer;

				currentOffsets[stream] = offset;

				activeVertexFormat[stream] = currentVertexFormat;

			}
#endif

		}
void RendererGLES2::applyConstants()
{
	if (currentShader != SHADER_NONE)
	{
		for (uint i = 0; i < (shaders[currentShader]).nUniforms; i++)
		{
			ConstantShaderGLSLGL3* uni = (&(shaders[currentShader]).uniforms[0]) + i;
			if ((uni)->dirty)
			{
				if ((uni)->type >= CONSTANT_MAT2)
				{
					LOG_FNLN_X;
					LOG_PRINT_X("uni->name:%s\n",uni->name.c_str());
					LOG_PRINT_X("uni->type:%d\n",uni->type);
					LOG_PRINT_X("uni->index:%d\n",uni->index);
					LOG_PRINT_X("uni->nElements:%d\n",uni->nElements);
					LOG_PRINT_X("uni->data:%x\n",reinterpret_cast<BYTE*>(&uni->data[0]));

					((UNIFORM_MAT_FUNC) uniformFuncs[(uni)->type])((uni)->index, (uni)->nElements, GL_FALSE, reinterpret_cast<float*>(&uni->data[0]));
					//glUniformMatrix4fv( (uni)->index, (uni)->nElements, GL_FALSE, reinterpret_cast<float*>(&uni->data[0]) );
					checkGlError("");
					LOG_FNLN_X;
				}
				else
				{
					LOG_PRINT_X("uni->name:%s:%s:%d\n",uni->name.c_str(),__FUNCTION__,__LINE__);
					((UNIFORM_FUNC) uniformFuncs[(uni)->type])((uni)->index, (uni)->nElements, reinterpret_cast<float*>(&uni->data[0]));
					checkGlError("");
				}
				(uni)->dirty = false;
			}
		}
		//
		#if 0
		for (uint i = 0; i < (shaders[currentShader]).nAttribs; i++)
		{
			//
			AttribShaderGLSLGL3* att = (&(shaders[currentShader]).attribs[0]) + i;
			if ((att)->dirty)
			{
				{
					//((UNIFORM_FUNC) uniformFuncs[(att)->type])((att)->index, (att)->nElements, reinterpret_cast<float*>(&att->data[0]));


					LOG_PRINT_X("glVertexAttribPointer:\n");
					LOG_PRINT_X("loc=%d\n",att->loc);
					LOG_PRINT_X("offset=%d\n",att->offset);
					glVertexAttribPointer ( att->loc, att->offset, GL_FLOAT, GL_FALSE, 0, 0);
				checkGlError("");

					glEnableVertexAttribArray (att->loc);
				checkGlError("");


				}
				(att)->dirty = false;
			}
		}
		#endif
	}
}

void RendererGLES2::setTexture(const char *textureName, const TextureID texture)
{
LOG_FNLN_X;
	if(texture==-1) return;//DBG_HALT;
LOG_FNLN_X;
	if(selectedShader == -1) return;
LOG_FNLN_X;

	int unit = getSamplerUnit(selectedShader, textureName);
LOG_FNLN_X;
	if (unit >= 0)
	{
LOG_FNLN_X;
		selectedTextures[unit] = texture;
		//LOG_PRINT_X("%s\n",textureName);
LOG_FNLN_X;
	}
LOG_FNLN_X;
}
void RendererGLES2::setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState, const int vtstream, const VertexBufferID vtbuffer)
{
LOG_FNLN_X;
	if(texture==-1) //DBG_HALT;
LOG_FNLN_X;
	if(selectedShader == SHADER_NONE) return;
LOG_FNLN_X;

	int unit = getSamplerUnit(selectedShader, textureName);
LOG_FNLN_X;
	if (unit >= 0)
	{
LOG_FNLN_X;
		selectedTextures[unit] = texture;
LOG_FNLN_X;
		selectedSamplerStates[unit] = samplerState;
LOG_FNLN_X;
	}
LOG_FNLN_X;
}
const GLenum glPrim[] =
{
	GL_TRIANGLES,
	GL_TRIANGLE_FAN,
	GL_TRIANGLE_STRIP,
	0,//???GL_QUADS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_POINTS,
}
;
void RendererGLES2::DrawPrimitive(Primitives PrimitiveType,UINT StartVertex,UINT PrimitiveCount, const uint flags)
		{
			apply();
			LOG_PRINT_X("glPrim[PrimitiveType]=%d\n",glPrim[PrimitiveType]);
			LOG_PRINT_X("StartVertex=%d\n",StartVertex);
			LOG_PRINT_X("getVertexCount(PrimitiveType,PrimitiveCount)=%d\n",getVertexCount(PrimitiveType,PrimitiveCount));
			LOG_PRINT_X("glDrawArrays:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
			//glDrawArrays (GLenum mode, GLint first, GLsizei count);
			//glDrawArrays (GLenum mode, GLint first, GLsizei count);
			//glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);

			glDrawArrays(glPrim[PrimitiveType], StartVertex,
#if 0
				PrimitiveCount*getIndicesCount(PrimitiveType)
#else
				getVertexCount(PrimitiveType,PrimitiveCount)
#endif
				);
			checkGlError("");
			reset(flags);
			nDrawCalls++;
		}
		void RendererGLES2::DrawIndexedPrimitive(Primitives PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount, const uint flags)
		{
			unsigned short* usd=(unsigned short*) indexBuffers[selectedIndexBuffer].pIndices;
			unsigned int  * uid=(unsigned int*)   indexBuffers[selectedIndexBuffer].pIndices;

			if(indexBuffers[selectedIndexBuffer].indexSize==2){
				glDrawElements(
					glPrim[PrimitiveType],
					getIndicesCount(PrimitiveType)*primCount,
					GL_UNSIGNED_SHORT,
					(usd+startIndex)//???
				);
				checkGlError("");}
			else{
				glDrawElements(
					glPrim[PrimitiveType],
					getIndicesCount(PrimitiveType)*primCount,
					GL_UNSIGNED_INT,
					(uid+startIndex)//???
				);
			checkGlError("");}
			reset(flags);
			nDrawCalls++;
		}
void RendererGLES2::changeShader(const ShaderID shader)
		{
			if (shader != currentShader)
			{
				if (shader == SHADER_NONE)
				{
					LOG_PRINT_X("glUseProgram(0):%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
					glUseProgram(0);
					checkGlError("");
					//LOG_PRINT_X("glUseProgramObject(0)\n");
				}
				else
				{
					LOG_PRINT_X("glUseProgram(%d):%s:%d\n",(shaders[shader]).program,__FUNCTION__,__LINE__);
					


#ifdef __APPLE__
glUseProgram(handle_to_uint((shaders[shader]).program));
#else
glUseProgram((shaders[shader]).program);
#endif


					checkGlError("");
					//LOG_PRINT_X("shader %d:glUseProgramObject(%d)\n",shader,(shaders[shader]).program);
				}
				currentShader = shader;
			}
		}
void RendererGLES2::setShaderConstantRaw(const char *name, const void *data, const int size)
		{
LOG_FNLN_X;
LOG_PRINT_X("name:%s\n", name);
LOG_PRINT_X("data:%x\n", data);
LOG_PRINT_X("size:%d\n", size);

			if(selectedShader==-1)return;
			{
LOG_FNLN_X;
LOG_PRINT_X("shaders.size():%d\n", shaders.size());
LOG_PRINT_X("shaders[%d].nUniforms:%d\n", selectedShader, shaders[selectedShader].nUniforms);
			int minUniform = 0;
LOG_FNLN_X;
			int maxUniform = (shaders[selectedShader]).nUniforms - 1;
LOG_FNLN_X;

			while (minUniform <= maxUniform)
			{
LOG_FNLN_X;
				int currUniform = (minUniform + maxUniform) >> 1;
LOG_FNLN_X;
				int res = strcmp(name, ((shaders[selectedShader]).uniforms[currUniform]).name.c_str());
LOG_FNLN_X;
				if (res == 0)
				{
LOG_FNLN_X;


			ConstantShaderGLSLGL3* uni = (&(shaders[selectedShader]).uniforms[0]) + currUniform;
LOG_FNLN_X;

					if (stx_memcmp(&uni->data[0], data, size))
					{
LOG_FNLN_X;
						uni->data.resize(size);
LOG_FNLN_X;
						stx_memcpy(&uni->data[0], data, size);//ABC123
LOG_FNLN_X;
						uni->dirty = true;
LOG_FNLN_X;
					}
LOG_FNLN_X;
					return;
				}
				else if (res > 0)
				{
LOG_FNLN_X;
					minUniform = currUniform + 1;
LOG_FNLN_X;
				}
				else
				{
LOG_FNLN_X;
					maxUniform = currUniform - 1;
LOG_FNLN_X;
				}
LOG_FNLN_X;
			}
LOG_FNLN_X;
			}
LOG_FNLN_X;
			#if 0
			{
			int minAttrib = 0;
			int maxAttrib = (shaders[selectedShader]).nAttribs - 1;

			//
			while (minAttrib <= maxAttrib)
			{
				int currAttrib = (minAttrib + maxAttrib) >> 1;
				int res = strcmp(name, ((shaders[selectedShader]).attribs[currAttrib]).name.c_str());


				LOG_PRINT_X("name1=%s\n",name);
				LOG_PRINT_X("name2=%s\n",((shaders[selectedShader]).attribs[currAttrib]).name.c_str());

				if (res == 0)
				{


			AttribShaderGLSLGL3* att = (&(shaders[selectedShader]).attribs[0]) + currAttrib;

					//
					//if (memcmp(&att->data[0], data, size))
					{
						//stx_memcpy(&att->data[0], data, size);

						att->dirty = true;
					}
					return;
				}
				else if (res > 0)
				{
					minAttrib = currAttrib + 1;
				}
				else
				{
					maxAttrib = currAttrib - 1;
				}
			}
			}
			#endif
		}
		int samplerCompareGLES(const void *sampler0, const void *sampler1)
		{
			return strcmp(((SamplerShaderGLSLGL3 *) sampler0)->name.c_str(), ((SamplerShaderGLSLGL3 *) sampler1)->name.c_str());
		}
		ConstantType getConstantType_GLES(GLenum type)
		{
			switch (type)
			{
				case GL_FLOAT:		return CONSTANT_FLOAT;
				case GL_FLOAT_VEC2: return CONSTANT_VEC2;
				case GL_FLOAT_VEC3: return CONSTANT_VEC3;
				case GL_FLOAT_VEC4: return CONSTANT_VEC4;
				case GL_INT:		return CONSTANT_INT;
				case GL_INT_VEC2:	return CONSTANT_IVEC2;
				case GL_INT_VEC3:	return CONSTANT_IVEC3;
				case GL_INT_VEC4:	return CONSTANT_IVEC4;
				case GL_BOOL:	return CONSTANT_BOOL;
				case GL_BOOL_VEC2:	return CONSTANT_BVEC2;
				case GL_BOOL_VEC3:	return CONSTANT_BVEC3;
				case GL_BOOL_VEC4:	return CONSTANT_BVEC4;
				case GL_FLOAT_MAT2: return CONSTANT_MAT2;
				case GL_FLOAT_MAT3: return CONSTANT_MAT3;
				case GL_FLOAT_MAT4: return CONSTANT_MAT4;
			}
			return (ConstantType) -1;
		}

#if 1
		bool samplerCompGLESstd(const SamplerShaderGLSLGL3 & s0, const SamplerShaderGLSLGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
		bool constantCompGLESstd(const ConstantShaderGLSLGL3 & s0, const ConstantShaderGLSLGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
#if 0
		bool attribCompGLESstd(const AttribShaderGLSLGL3 & s0, const AttribShaderGLSLGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
#endif
#endif
inline void active_vertex_shader_inputs(GLuint prog)
	{
	//GLuint prog=shaders[id/*currentShader*/]).program;
	  char *name;
	  GLint active_attribs, max_length;
	  unsigned i;

//arch-mips/usr/include/GLES2/gl2.h:#define GL_ACTIVE_ATTRIBUTES
//arch-mips/usr/include/GLES2/gl2.h:#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH

	  LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	  checkGlError("");
	LOG_PRINT_X("active_attribs=%d\n", active_attribs);
	  LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
	  checkGlError("");
	LOG_PRINT_X("active_attribs max_length=%d\n", max_length);

	if(active_attribs==-1) return;
	if(max_length==-1) return;
	if(1)
	{

	  name = (char*)malloc(max_length + 1);
	  //LOG_PRINT_X("Active vertex shader inputs:\n");
	  GLint stride=0;
	  for (i = 0; i < active_attribs; i++) {
	    GLint size;
	    GLenum type;

	    LOG_PRINT_X("glGetActiveAttrib:%d:%s:%s:%d\n",i,__FILE__,__FUNCTION__,__LINE__);
	  	glGetActiveAttrib(prog, i, max_length + 1, 0,
	                      &size, &type, name);
		checkGlError("");
		stride+=size;
		};
	  for (i = 0; i < active_attribs; i++) {
	    GLint size;
	    GLenum type;

	    LOG_PRINT_X("glGetActiveAttrib:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  	glGetActiveAttrib(prog, i, max_length + 1, 0,
	                      &size, &type, name);
	    checkGlError("");
	//LOG_PRINT_X("Vertex input attribute %s of type %d size %d is at location %d, stride %d\n", name, type, size, glGetAttribLocation(prog, name),stride);

		LOG_PRINT_X("glBindAttribLocation(%d,%d,%s):%s:%s:%d\n",prog, i, name,__FILE__,__FUNCTION__,__LINE__);
	  	glBindAttribLocation(prog, i, name);
		checkGlError("");
		}
	  free(name);
	}
	}
inline GLuint esLoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;

   // Create the shader object
   LOG_PRINT_X("glCreateShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);

LOG_PRINT_X("shaderSrc:\n%s\n", shaderSrc);

	shader = glCreateShader ( type );

    if (!shader) {
	LOG_FNLN_X;
        checkGlError("glCreateShader1");
        return 0;
    }

   // Load the shader source
   LOG_PRINT_X("glShaderSource:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glShaderSource ( shader, 1, &shaderSrc, 0 );
   checkGlError("");
   // Compile the shader
   LOG_PRINT_X("glCompileShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glCompileShader ( shader );

	LOG_FNLN;
	LOG_PRINT("glCompileShader(%d)\n", shader);

   // Check the compile status
   LOG_PRINT_X("glGetShaderiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
   checkGlError("");

   if ( !compiled )
   {
      GLint infoLen = 0;

      LOG_PRINT_X("glGetShaderiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
   checkGlError("");

      if ( infoLen > 1 )
      {
         char* infoLog = (char*)malloc (sizeof(char) * infoLen );

         LOG_PRINT_X("glGetShaderInfoLog:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetShaderInfoLog ( shader, infoLen, 0, infoLog );
   checkGlError("");
         LOG_PRINT_X ( "Error compiling shader:\n%s\n", infoLog );

         free ( infoLog );
      }

      LOG_PRINT_X("glDeleteShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glDeleteShader ( shader );
   checkGlError("");
      return 0;
   }

   return shader;

}

GLuint loadShaderGLES2(GLenum shaderType, const char* pSource) {
    LOG_FNLN_X;
LOG_PRINT_X("pSource:\n%s\n", pSource);

    LOG_FNLN_X;
	  GLuint shader = glCreateShader(shaderType);

    if (!shader) {
	LOG_FNLN_X;
        checkGlError("glCreateShader2");
        return 0;
    }

    if (shader) {
    LOG_FNLN_X;
	  glShaderSource(shader, 1, &pSource, 0);
   checkGlError("");
    LOG_FNLN_X;
	  glCompileShader(shader);
				checkGlError("");

	LOG_FNLN;
	LOG_PRINT("glCompileShader(%d)\n", shader);

        GLint compiled = 0;
    LOG_FNLN_X;
	  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   checkGlError("");
    LOG_FNLN_X;
        if (!compiled) {
    LOG_FNLN_X;
            GLint infoLen = 0;
    LOG_FNLN_X;
	  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
   checkGlError("");
    LOG_FNLN_X;
            if (infoLen) {
    LOG_FNLN_X;
                char* buf = (char*) malloc(infoLen);
    LOG_FNLN_X;
                if (buf) {
    LOG_FNLN_X;
	  		glGetShaderInfoLog(shader, infoLen, 0, buf);
   checkGlError("");
    LOG_FNLN_X;
                    LOG_PRINT_X("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
			//???STXBACKTRACE
			LOG_PRINT_X("\n%s\n",pSource);
    LOG_FNLN_X;
                    free(buf);
    LOG_FNLN_X;
                }
    LOG_FNLN_X;
	  	glDeleteShader(shader);
   checkGlError("");
    LOG_FNLN_X;
                shader = 0;
    LOG_FNLN_X;
            }
    LOG_FNLN_X;
        }
    LOG_FNLN_X;
    }
    LOG_FNLN_X;
    return shader;
}

#if 0
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShaderGLES2(GL_VERTEX_SHADER, pVertexSource);

    GLuint pixelShader = loadShaderGLES2(GL_FRAGMENT_SHADER, pFragmentSource);

    LOG_PRINT_X("glCreateProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	GLuint program = glCreateProgram();
				checkGlError("");
    if (program) {
        LOG_PRINT_X("glAttachShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glAttachShader(program, vertexShader);
   checkGlError("");
        LOG_PRINT_X("glAttachShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glAttachShader(program, pixelShader);
   checkGlError("");
        LOG_PRINT_X("glLinkProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glLinkProgram(program);
   checkGlError("");
        GLint linkStatus = GL_FALSE;
        LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
   checkGlError("");
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
   checkGlError("");
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    LOG_PRINT_X("glGetProgramInfoLog:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  		glGetProgramInfoLog(program, bufLength, 0, buf);
   checkGlError("");
                    LOG_PRINT_X("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            LOG_PRINT_X("glDeleteProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glDeleteProgram(program);
   checkGlError("");
            program = 0;
        }
    }
    return program;
}
#endif


		ShaderID RendererGLES2::addGLSLShader(const char *vsText, const char *gsText, const char *fsText,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags)

		{
LOG_FNLN_X;
#if defined(SDL_2_0_4_BUILD)
SDL_GL_MakeCurrent(g_SDL_window, g_SDL_context);
#endif
LOG_FNLN_X;

		  
			//if (!GLSL_supported || (vsText0 == 0 && fsText0 == 0) || gsText != 0)
			//	return SHADER_NONE;
			const char * mainvs=vsName;
LOG_FNLN_X;
			const char * mainps=psName;
LOG_FNLN_X;
		       

			 //  if(!TranslateVertexShader(glslvs.c_str(), mainvs, glslvs)) return SHADER_NONE;//DBG_HALT;
			//const char * vsText=glslvs.c_str();


//if(!TranslateFragmentShader(glslfs.c_str(), mainps, glslfs)) return SHADER_NONE;//DBG_HALT;//
     //replaceAll(glslfs,rm,r);
			//const char * fsText=glslfs.c_str();
			   
			ShaderGLSLGL3 shader;
LOG_FNLN_X;
			const GLchar *shaderStrings[6];
LOG_FNLN_X;
			int strIndex = 0;
LOG_FNLN_X;
			char line[16];
LOG_FNLN_X;
			GLint vsResult, fsResult, linkResult;
LOG_FNLN_X;
			char infoLog[2048];
LOG_FNLN_X;
			GLint len, infoLogPos = 0;
LOG_FNLN_X;
			

#if 1
			if (GL_ARB_shading_language_100_supported)
			{
LOG_FNLN_X;

				static char versionString[16] = {0};
LOG_FNLN_X;
				static bool first = true;
LOG_FNLN_X;
				if (first)
				{
LOG_FNLN_X;
//NDK: const char* versionStr = (const char*)glGetString(GL_VERSION);
					const char *version = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION/*_ARB*/);
				checkGlError("");
					if (version)
					{
LOG_FNLN_X;

						int major = atoi(version);
LOG_FNLN_X;
                        int minor = atoi(stx_strchr(version, '.') + 1);
LOG_FNLN_X;
                        stx_snprintf(versionString,16, "#version %d%d\n", major, minor);
LOG_FNLN_X;

					}
LOG_FNLN_X;
					first = false;
LOG_FNLN_X;

				}
LOG_FNLN_X;
				shaderStrings[strIndex++] = versionString;
LOG_FNLN_X;

			}
#ifdef _MSC_VER
			shaderStrings[strIndex++] ="precision mediump float;\n";
#else
			//shaderStrings[strIndex++] ="mediump float;\n";
#endif
//#define row_major\n";
				//"#version 120\n"
			//	"";//precision mediump float;\n";
#else
			shaderStrings[strIndex++] =
				"#version 120\n";
LOG_FNLN_X;
				//"precision mediump float;\n";
#endif

#if 0//???
			"#define saturate(x) clamp(x,0.0,1.0)\n"
			"#define lerp mix\n";
#endif


#if 0
			shaderStrings[strIndex++] =
			"#define ROW_MAJOR\n"
			"#define MVPSEMANTIC\n"
			"#define WSIGN +\n";
#endif

			if (extra) shaderStrings[strIndex++] = extra;
LOG_FNLN_X;

			if (header) shaderStrings[strIndex++] = header;
LOG_FNLN_X;

			shaderStrings[strIndex++] = line;
LOG_FNLN_X;




    GLuint vertexShader = loadShaderGLES2(GL_VERTEX_SHADER, vsText);
LOG_FNLN_X;

    GLuint pixelShader = loadShaderGLES2(GL_FRAGMENT_SHADER, fsText);
LOG_FNLN_X;

    LOG_PRINT_X("glCreateProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	GLuint sp0=glCreateProgram();
   checkGlError("");
		LOG_FNLN;
		LOG_PRINT("glCreateProgram()=%d\n",sp0);
    GLint linkStatus = GL_FALSE;
LOG_FNLN_X;
	if (sp0) {
        LOG_PRINT_X("glAttachShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glAttachShader(sp0, vertexShader);
   checkGlError("");
        LOG_PRINT_X("glAttachShader:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glAttachShader(sp0, pixelShader);
   checkGlError("");
        LOG_PRINT_X("glLinkProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glLinkProgram(sp0);
   checkGlError("");

        LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(sp0, GL_LINK_STATUS, &linkStatus);

	if(linkStatus != GL_FALSE)
	{
		LOG_FNLN;
		LOG_PRINT("linkStatus != GL_FALSE\n");
/*
../../../Libraries/Framework3/src/RendererGLES2.cpp:941:18: error: assigning to 'GLhandleARB' (aka 'void *') from incompatible type 'GLuint' (aka 'unsigned int')
                shader.program = sp0;
*/
		shader.program = sp0;
	} else {
		LOG_FNLN;
		LOG_PRINT("linkStatus == GL_FALSE\n");
		shader.program = 0;
		sp0=0;
	}

   checkGlError("");
        if (linkStatus != GL_TRUE) {
LOG_FNLN_X;
            GLint bufLength = 0;
            LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetProgramiv(sp0, GL_INFO_LOG_LENGTH, &bufLength);
   checkGlError("");
            if (bufLength) {
LOG_FNLN_X;
                char* buf = (char*) malloc(bufLength);
LOG_FNLN_X;
                if (buf) {
                    LOG_PRINT_X("glGetProgramInfoLog:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  		glGetProgramInfoLog(sp0, bufLength, 0, buf);
   			checkGlError("");
                    LOG_PRINT_X("Could not link program:\n%s\n", buf);
LOG_FNLN_X;
                    free(buf);
LOG_FNLN_X;
                }
LOG_FNLN_X;
            }
            //LOG_PRINT_X("glDeleteProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  //glDeleteProgram(sp0);
   checkGlError("");
LOG_FNLN_X;
        }
LOG_FNLN_X;
            //shader.program = sp0;
LOG_FNLN_X;
    }
LOG_FNLN_X;


				if (linkStatus)
				{
LOG_FNLN_X;
					
#ifdef __APPLE__
GLint currProgram = (currentShader == SHADER_NONE)? 0 : handle_to_uint((shaders[currentShader]).program);
GLint sp=handle_to_uint(shader.program);
#else
LOG_FNLN_X;
GLint currProgram = (currentShader == SHADER_NONE)? 0 : (shaders[currentShader]).program;
LOG_FNLN_X;
GLint sp=shader.program;
LOG_FNLN_X;
#endif
					//LOG_PRINT_X("(shaders[%d]).program:%d:%s:%s:%d\n",currentShader,(shaders[currentShader]).program,__FILE__,__FUNCTION__,__LINE__);
					LOG_PRINT_X("currProgram:%d\n",currProgram);
	  				LOG_PRINT_X("sp:%d\n",sp);
	  				
					LOG_PRINT_X("glUseProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glUseProgram(sp);
   checkGlError("");
					GLint uniformCount, maxLength;

                        		//glGetShaderiv
					LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glGetProgramiv(sp, GL_ACTIVE_UNIFORMS/*_ARB*/, &uniformCount);
   checkGlError("");
					//glGetShaderiv
					LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glGetProgramiv(sp, GL_ACTIVE_UNIFORM_MAX_LENGTH/*_ARB*/, &maxLength);
   checkGlError("");

					std::vector<SamplerShaderGLSLGL3	> samplers;
LOG_FNLN_X;
					std::vector<ConstantShaderGLSLGL3 > uniforms;
LOG_FNLN_X;
					std::vector<AttribShaderGLSLGL3 > attribs;
LOG_FNLN_X;

					
					samplers.resize(uniformCount);
LOG_FNLN_X;
					uniforms.resize(uniformCount);
LOG_FNLN_X;
					attribs.resize(uniformCount);
LOG_FNLN_X;
					
					int nSamplers = 0;
LOG_FNLN_X;
					int nUniforms = 0;
LOG_FNLN_X;
					int nAttribs = 0;
LOG_FNLN_X;
					std::vector<char> bbuf(maxLength);
LOG_FNLN_X;
					char *name = (char *)&bbuf[0];
LOG_FNLN_X;
					
{


#ifdef __APPLE__
GLint prog=handle_to_uint(shader.program);
#else
GLint prog=shader.program;
LOG_FNLN_X;
#endif

GLint maxAttribNameLength,numActiveAttribs = 0;
LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
   checkGlError("");
LOG_PRINT_X("glGetProgramiv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
   checkGlError("");

std::vector<GLchar> nameData(maxAttribNameLength);
LOG_FNLN_X;
for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
{
LOG_FNLN_X;
	GLint arraySize = 0;
LOG_FNLN_X;
	GLenum type = 0;
LOG_FNLN_X;
	GLsizei actualLength = 0;
	LOG_PRINT_X("glGetActiveAttrib:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  glGetActiveAttrib(prog, attrib, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
   checkGlError("");
	std::string name((char*)&nameData[0], actualLength );
  	LOG_PRINT_X("glGetAttribLocation(%d,%s):%s:%s:%d\n",prog, name.c_str(),__FILE__,__FUNCTION__,__LINE__);
	  GLint loc = glGetAttribLocation ( prog, name.c_str());
   checkGlError("");


	LOG_FNLN;
	LOG_PRINT("Set: Attrib name=%s\n", name.c_str());
	LOG_PRINT("Set: Attrib loc=%d\n",loc);

	attribs[nAttribs].name=name;
	attribs[nAttribs].loc=loc;
LOG_FNLN_X;
	//attribs[nAttribs].name=std::string( name);
	nAttribs++;
LOG_FNLN_X;
}}
LOG_FNLN_X;


					for (int i = 0; i < uniformCount; i++)
					{
LOG_FNLN_X;
						GLenum type;
LOG_FNLN_X;
						GLint length, size;
						LOG_PRINT_X("glGetActiveUniform:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  					

#ifdef __APPLE__
glGetActiveUniform(handle_to_uint(shader.program), i, maxLength, &length, &size, &type, name);
#else
glGetActiveUniform(shader.program, i, maxLength, &length, &size, &type, name);
LOG_FNLN_X;
#endif

   checkGlError("");
						//LOG_PRINT_X("u=%s\n",name);
						//if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_RECT_SHADOW_ARB)
						if (type >= GL_SAMPLER_2D && type <= GL_SAMPLER_CUBE)
						{

#ifdef __APPLE__
GLint location = glGetUniformLocation(handle_to_uint(shader.program), name);
#else
LOG_FNLN_X;
GLint location = glGetUniformLocation(shader.program, name);
#endif

   checkGlError("");
							LOG_PRINT_X("glUniform1i:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  						glUniform1i(location, nSamplers);
   checkGlError("");
							samplers[nSamplers].index = nSamplers;
LOG_FNLN_X;
							samplers[nSamplers].name=std::string( name);
LOG_FNLN_X;
							nSamplers++;
LOG_FNLN_X;
						}
						else
						{
LOG_FNLN_X;
							if (strncmp(name, "gl_", 3) != 0)
							{
LOG_FNLN_X;
                                char *bracket = stx_strchr(name, '[');
LOG_FNLN_X;
								if (bracket == 0 || (bracket[1] == '0' && bracket[2] == ']'))
								{
LOG_FNLN_X;
									if (bracket)
									{
LOG_FNLN_X;
										*bracket = '\0';
LOG_FNLN_X;
										length = (GLint) (bracket - name);
LOG_FNLN_X;
									}
LOG_FNLN_X;
																	
#ifdef __APPLE__
uniforms[nUniforms].index = glGetUniformLocation(handle_to_uint(shader.program), name);
#else
LOG_FNLN_X;
uniforms[nUniforms].index = glGetUniformLocation(shader.program, name);
#endif
				checkGlError("");
									uniforms[nUniforms].type = getConstantType_GLES(type);
LOG_FNLN_X;
									uniforms[nUniforms].nElements = size;
LOG_FNLN_X;
									uniforms[nUniforms].name=std::string( name);
LOG_FNLN;
LOG_PRINT("Set: uniform name=%s\n", uniforms[nUniforms].name.c_str());
LOG_PRINT("Set: uniform index=%d\n", uniforms[nUniforms].index);
									nUniforms++;
LOG_FNLN_X;
								}
								else if (bracket != 0 && bracket[1] > '0')
								{
LOG_FNLN_X;
									*bracket = '\0';
LOG_FNLN_X;
									for (int i = nUniforms - 1; i >= 0; i--)
									{
LOG_FNLN_X;
										if (strcmp(uniforms[i].name.c_str(), name) == 0)
										{
LOG_FNLN_X;
											int index = atoi(bracket + 1) + 1;
LOG_FNLN_X;
											if (index > uniforms[i].nElements)
											{
LOG_FNLN_X;
												uniforms[i].nElements = index;
LOG_FNLN_X;
											}
LOG_FNLN_X;
										}
LOG_FNLN_X;
									}
LOG_FNLN_X;
								}
LOG_FNLN_X;
							}
LOG_FNLN_X;
						}
LOG_FNLN_X;
					}
LOG_FNLN_X;

					LOG_PRINT_X("glUseProgram:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glUseProgram(currProgram);
   checkGlError("");
					
					//delete[] name;
					

					attribs.resize(nAttribs);
LOG_FNLN_X;
					samplers.resize(nSamplers);
LOG_FNLN_X;
					uniforms.resize(nUniforms);
LOG_FNLN_X;

					
					#if 1
					if(samplers.size()>1) std::sort(samplers.begin(), samplers.end(), samplerCompGLESstd);
LOG_FNLN_X;
					if(uniforms.size()>1) std::sort(uniforms.begin(), uniforms.end(), constantCompGLESstd);
LOG_FNLN_X;
					//if(attribs.size()>1) std::sort(attribs.begin(), attribs.end(), attribCompGLESstd);

					#endif
					
					for (int i = 0; i < nUniforms; i++)
					{
LOG_FNLN_X;
						int constantSize = constantTypeSizes[uniforms[i].type] * uniforms[i].nElements;
LOG_FNLN_X;
						uniforms[i].data.resize(constantSize);
LOG_FNLN_X;
						memset((void*)&uniforms[i].data[0], 0, constantSize);
LOG_FNLN_X;
						uniforms[i].dirty = false;
LOG_FNLN_X;
					}
LOG_FNLN_X;
					
					shader.uniforms  = uniforms;
LOG_FNLN_X;
					shader.attribs  = attribs;
LOG_FNLN_X;
					shader.samplers  = samplers;
LOG_FNLN_X;
					shader.nUniforms = nUniforms;
LOG_FNLN_X;
					shader.nAttribs = nAttribs;
LOG_FNLN_X;
					shader.nSamplers = nSamplers;
LOG_FNLN_X;

					shaders.push_back(shader);
LOG_FNLN_X;
					
					
   // Free up no longer needed shader resources
#ifdef __APPLE__
	active_vertex_shader_inputs(handle_to_uint(shader.program));
   glDeleteShader ( handle_to_uint(shader.vertexShader) );
   glDeleteShader ( handle_to_uint(shader.fragmentShader) );
#else
	active_vertex_shader_inputs(shader.program);
LOG_FNLN_X;
   glDeleteShader ( shader.vertexShader );
LOG_FNLN_X;
   glDeleteShader ( shader.fragmentShader );
LOG_FNLN_X;
#endif
					return shaders.size()-1;
LOG_FNLN_X;
				}
LOG_FNLN_X;
            //}
			
#ifdef _MSC_VER
ErrorMsg(infoLog);
#else
LOG_FNLN_X;
					LOG_PRINT_X("\n%s\n",infoLog);
LOG_FNLN_X;
#endif
			
			return SHADER_NONE;
		}
		ShaderID RendererGLES2::addHLSLShaderVrtl(const char *vsText, const char *gsText, const char *fsText,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags)
		{
LOG_FNLN_X;
			#if 1
			std::string vsText0;
			std::string fsText0;
#if defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
			std::string shaderStrings=
			"#define WSIGN +\n"
			"#define ROW_MAJOR\n"
			"#define MVPSEMANTIC\n"
			"#ifdef GL_ES\n"
			"precision mediump float;\n"
			"#endif\n";
#else
			std::string shaderStrings=
			"#define WSIGN +\n"
			"#define ROW_MAJOR\n"
			"#define MVPSEMANTIC\n";
#endif



			vsText0.append(shaderStrings);
			fsText0.append(shaderStrings);
			vsText0.append(vsText);
			fsText0.append(fsText);

#endif
	ShaderID id=-1;
LOG_FNLN_X;
		id=addGLSLShader(
			vsText0.c_str(),
			0,
			fsText0.c_str(),
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);
LOG_FNLN_X;
		return id;
}

void RendererGLES2::applyTextures()
{
	for (uint i = 0; i < MAX_TEXTUREUNIT; i++){
		TextureID texture = selectedTextures[i];
		TextureID currTex = currentTextures[i];
		if (texture != currTex){
			/* if (texture == TEXTURE_NONE){
				//LOG_PRINT_X("glDisable texture\n")
				LOG_PRINT_X("glDisable %d\n",textures[currTex].glTexID);
				glDisable(textures[currTex].glTarget);
				checkGlError("");
				glBindTexture(textures[currTex].glTarget, 0);
				checkGlError("");
			} */
			if (texture != TEXTURE_NONE) {

				LOG_PRINT_X("glEnable textureID=%d, glTarget=%d\n",textures[texture].glTexID, textures[texture].glTarget);
				LOG_PRINT_X("glBindTexture:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  					//glActiveTextureARB(GL_TEXTURE0 + i);
				glEnable(textures[texture].glTarget);
   checkGlError("");
				glBindTexture(textures[texture].glTarget, textures[texture].glTexID);
   checkGlError("");

			}
			currentTextures[i] = texture;
		}
		else if(0)
		{
			//LOG_PRINT_X("glDisable %d\n",textures[currTex].glTexID);
			LOG_PRINT_X("glDisable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  		glDisable(textures[currTex].glTarget);
   checkGlError("");
			LOG_PRINT_X("glBindTexture:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  		glBindTexture(textures[currTex].glTarget, 0);
   checkGlError("");
		}
	}
}

		int RendererGLES2::getSamplerUnit(const ShaderID shader, const char *samplerName) const
				{
LOG_FNLN_X;
LOG_PRINT_X("shader(id)=%d\n", shader);
LOG_PRINT_X("shaders.size()=%d\n", shaders.size());

if(!shaders.size()) return -1;

LOG_PRINT_X("shaders[%d].samplers.size()=%d\n", shader, shaders[shader].samplers.size());


					//ASSERT(shader != -1);
					std::vector<SamplerShaderGLSLGL3 >samplers = (shaders[shader])
					.samplers;
LOG_FNLN_X;
					int minSampler = 0;
LOG_FNLN_X;
					int maxSampler = (shaders[shader])
					.nSamplers - 1;
LOG_FNLN_X;
					while (minSampler <= maxSampler)
					{
LOG_FNLN_X;
						int currSampler = (minSampler + maxSampler) >> 1;
LOG_FNLN_X;
						int res = strcmp(samplerName, (samplers[currSampler]).name.c_str());
LOG_FNLN_X;
						if (res == 0)
						{
LOG_FNLN_X;
							return (samplers[currSampler]).index;
						}
						else if (res > 0)
						{
LOG_FNLN_X;
							minSampler = currSampler + 1;
						}
						else
						{
LOG_FNLN_X;
							maxSampler = currSampler - 1;
						}
LOG_FNLN_X;
					}
LOG_FNLN_X;
					return -1;
		}


		RendererGLES2::RendererGLES2(int awidth, int aheight,
		bool InitLogFile,
			bool fullScreen,
int argc, char *argv[]
) : mOk(false), RendererGLES(awidth, aheight,InitLogFile,fullScreen,argc,argv)
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

	hDc = GetDC( STX_Service::GetWindowInstance()->GetHWND() );
	PixelFormat = ChoosePixelFormat( m_hDC, &pfd );
	SetPixelFormat( m_hDC, PixelFormat, &pfd);

	hGlrc = wglCreateContext( hDc);
	
	wglMakeCurrent( m_hDC, hGlrc );
	
	
#else
#if 0
	if (info.subsystem!=SDL_SYSWM_X11)
	{
		LOG_PRINT_X("System is not X11!\n");
		LOG_PRINT_X("%d!=%d\n", (int)info.subsystem , (int)SDL_SYSWM_X11 <<'\n');
		return;
	}
	if(!dpy)
	{
		m_dpy=info.info.x11.display;
		m_win=info.info.x11.window;
		int errorbase,tEventbase;
		if (glXQueryExtension(dpy,&errorbase,&tEventbase) == False)
		{
			LOG_PRINT_X("OpenGL through GLX not supported.\n");
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
			LOG_PRINT_X("Could not initialize Visual.\n");
			return;
		}
		ctx=glXCreateContext(dpy,vi,0,True);
		if(ctx== 0)
		{
			LOG_PRINT_X("Could not initialize GL context.\n");
			return;
		}
		if (!glXMakeCurrent(m_dpy,m_win,ctx))
		{
			m_dpy=0;
			return;
		}
	}
	#endif
	////initExtensions(m_dpy);
	//importGLInit();
	//
#endif

			GLint units = 1;
#if 0//def _WIN32
			if (GL_ARB_fragment_shader_supported || GL_ARB_fragment_program_supported)
			{
				LOG_PRINT_X("glGetIntegerv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &units);
   checkGlError("");
			}
			else
#endif
#if !defined(USE_GLES_SHADERS) && !defined(SDL_2_0_4_BUILD)
			{
				LOG_PRINT_X("glGetIntegerv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &units);
   checkGlError("");
			}
#endif
			nImageUnits = units;
			memset(&textureLod, 0, sizeof(textureLod));
#if !defined(USE_GLES_SHADERS) && !defined(SDL_2_0_4_BUILD)
			if (GL_ARB_draw_buffers_supported)
			{
				GLint mrt = 1;
				LOG_PRINT_X("glGetIntegerv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glGetIntegerv(GL_MAX_DRAW_BUFFERS, &mrt);
   checkGlError("");
				nMRTs = mrt;
			}
#endif
			if (nMRTs > MAX_MRTS) nMRTs = MAX_MRTS;
			for (uint i = 0; i < nMRTs; i++)
			{
				drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			#ifndef ANDROID
			if (GL_EXT_texture_filter_anisotropic_supported)
			{
				GLint aniso = 1;
				LOG_PRINT_X("glGetIntegerv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
   checkGlError("");
				maxAnisotropic = aniso;
			}
			#endif
			fbo = 0;
			currentFrontFace = GL_CW;
			currentVBO = 0;
			texVF	= VF_NONE;
			plainShader = SHADER_NONE;
			plainVF = VF_NONE;
			setGLdefaults();
			resetToDefaults();
			reset(0);
			memset(&activeVertexFormat, VF_NONE, sizeof(activeVertexFormat));
			uniformFuncs[CONSTANT_FLOAT] = (void *) glUniform1fv;
			uniformFuncs[CONSTANT_VEC2]  = (void *) glUniform2fv;
			uniformFuncs[CONSTANT_VEC3]  = (void *) glUniform3fv;
			uniformFuncs[CONSTANT_VEC4]  = (void *) glUniform4fv;
			uniformFuncs[CONSTANT_INT]   = (void *) glUniform1iv;
			uniformFuncs[CONSTANT_IVEC2] = (void *) glUniform2iv;
			uniformFuncs[CONSTANT_IVEC3] = (void *) glUniform3iv;
			uniformFuncs[CONSTANT_IVEC4] = (void *) glUniform4iv;
			uniformFuncs[CONSTANT_BOOL]  = (void *) glUniform1iv;
			uniformFuncs[CONSTANT_BVEC2] = (void *) glUniform2iv;
			uniformFuncs[CONSTANT_BVEC3] = (void *) glUniform3iv;
			uniformFuncs[CONSTANT_BVEC4] = (void *) glUniform4iv;
			uniformFuncs[CONSTANT_MAT2]  = (void *) glUniformMatrix2fv;
			uniformFuncs[CONSTANT_MAT3]  = (void *) glUniformMatrix3fv;
			uniformFuncs[CONSTANT_MAT4]  = (void *) glUniformMatrix4fv;
			bsupportsHalf=false;
			bsupportsR2VB=false;
			overrideR2VB=true;
			overrideHalf=true;
			mOk=true;
		}
		RendererGLES2::~RendererGLES2()
		{
			//STX_TRY;
			apply();
			for (uint i = 0; i < shaders.getCount(); i++)
			{
				for (uint j = 0; j < (shaders[i]).nSamplers; j++)
				{
					//delete (shaders[i]).samplers[j].name;
				}
				for (uint j = 0; j < (shaders[i]).nUniforms; j++)
				{
					//delete (shaders[i]).uniforms[j].name;
					shaders[i].uniforms[j].data.clear();
				}
				(shaders[i]).samplers.clear();
				(shaders[i]).uniforms.clear();
				(shaders[i]).attribs.clear();
#if !defined(USE_GLES_SHADERS) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
				LOG_PRINT_X("glDeleteObject:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteObject((shaders[i]).vertexShader);
   				checkGlError("");
				LOG_PRINT_X("glDeleteObject:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteObject((shaders[i]).fragmentShader);
   				checkGlError("");
				LOG_PRINT_X("glDeleteObject:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteObject((shaders[i]).program);
   				checkGlError("");
#endif
			}
			for (uint i = 0; i < vertexBuffers.getCount(); i++)
			{
				LOG_PRINT_X("glDeleteBuffers:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteBuffers(1, &(vertexBuffers[i]).vboVB);
   checkGlError("");
			}
			for (uint i = 0; i < indexBuffers.getCount(); i++)
			{
				LOG_PRINT_X("glDeleteBuffers:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteBuffers(1, &(indexBuffers[i]).vboIB);
   checkGlError("");
			}
			for (uint i = 0; i < textures.getCount(); i++)
			{

				removeTexture(i);
			}
			if (fbo) {LOG_PRINT_X("glDeleteFramebuffers:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  			glDeleteFramebuffers(1, &fbo);}
   			checkGlError("");
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
		//STX_CATCH;
		}

		void RendererGLES2::changeIndexBuffer(const IndexBufferID indexBuffer)
		{
			////if (indexBuffer != currentIndexBuffer)
			{
				if (indexBuffer == IB_NONE)
				{
					LOG_PRINT_X("glBindBuffer:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   					checkGlError("");
				}
				else
				{
					LOG_PRINT_X("glBindBuffer:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	  				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (indexBuffers[indexBuffer]).vboIB);
   					checkGlError("");
				}
				currentIndexBuffer = indexBuffer;
			}
		}
float	RendererGLES2::GetMaxPointSpriteSize()
{
//
    // If you want to know the std::max size that a point sprite can be set
    // to, do this.
	//

    // Query for the std::max point size supported by the hardware
    float maxSize = 0.0f;
#if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)
    LOG_PRINT_X("glGetFloatv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
   checkGlError("");
    LOG_PRINT_X("glPointSize:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointSize( maxSize );
   checkGlError("");
#endif
	return maxSize;
}

void	RendererGLES2::BeginPointSpriteRendering()
{
	//
	// Enabling GL_DEPTH_TEST and setting glDepthMask to GL_FALSE makes the
    // Z-Buffer read-only, which helps remove graphical artifacts generated
    // from  rendering a list of particles that haven't been sorted by
    // distance to the eye.
	//
    // Enabling GL_BLEND and setting glBlendFunc to GL_DST_ALPHA with GL_ONE
    // allows particles, which overlap, to alpha blend with each other
    // correctly.
	//

    LOG_PRINT_X("glEnable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glEnable( GL_DEPTH_TEST );
   checkGlError("");
    LOG_PRINT_X("glDepthMask:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glDepthMask( GL_FALSE );
   checkGlError("");


	LOG_PRINT_X("glEnable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glEnable( GL_BLEND );
   checkGlError("");
    LOG_PRINT_X("glBlendFunc:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glBlendFunc( GL_DST_ALPHA, GL_ONE );
   checkGlError("");
#if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)

    //
	// Set up the OpenGL state machine for using point sprites...
	//

    // This is how will our point sprite's size will be modified by
    // distance from the viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    LOG_PRINT_X("glPointParameterfvARB:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
   checkGlError("");

    // Query for the std::max point size supported by the hardware
    float maxSize = 0.0f;
    LOG_PRINT_X("glGetFloatv:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
   checkGlError("");

    // Clamp size to 100.0f or the sprites could get a little too big on some
    // of the newer graphic cards. My ATI card at home supports a std::max point
    // size of 1024.0f!
    if( maxSize > 100.0f )
        maxSize = 100.0f;

    LOG_PRINT_X("glPointSize:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointSize( maxSize );
   checkGlError("");//in OTL 10 ???

    // The alpha of a point is calculated to allow the fading of points
    // instead of shrinking them past a defined threshold size. The threshold
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to
    // the minimum and maximum point sizes.
    LOG_PRINT_X("glPointParameterfARB:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
   checkGlError("");

    LOG_PRINT_X("glPointParameterfARB:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
   checkGlError("");
    LOG_PRINT_X("glPointParameterfARB:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
   checkGlError("");

    // Specify point sprite texture coordinate replacement mode for each
    // texture unit
    LOG_PRINT_X("glTexEnvf:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glTexEnvf( GL_POINT_SPRITE_OES, GL_COORD_REPLACE_ARB, GL_TRUE );
   checkGlError("");

    //
	// Render point sprites...
	//
//???if defined(OS_IPHONE)
    LOG_PRINT_X("glEnable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glEnable( GL_POINT_SPRITE_OES );
   checkGlError("");
#endif
}

void	RendererGLES2::EndPointSpriteRendering()
{
    //
    // Reset OpenGL states...
	//

    LOG_PRINT_X("glDepthMask:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glDepthMask( GL_TRUE );
   checkGlError("");
    LOG_PRINT_X("glDisable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glDisable( GL_BLEND );
   checkGlError("");
#if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)
	LOG_PRINT_X("glDisable:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
	glDisable( GL_POINT_SPRITE_OES );
   checkGlError("");
#endif
}
#endif


