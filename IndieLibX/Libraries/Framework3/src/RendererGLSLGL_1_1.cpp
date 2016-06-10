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

#include <Framework3/RendererGLSLGL_1_1.h>
#ifdef __FrameworkGLSLGL_1_1_H__

//include <Framework3/TestFBO.h>
typedef GLvoid (APIENTRY *UNIFORM_FUNC)(GLint location, GLsizei count, const void *value);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
#ifdef USE_HLSL_SHADERS
#include <hlsl2glsl.h>
#endif
#ifdef USE_HLSL_SHADERS

enum TestRun {
	VERTEX,
	FRAGMENT,
	BOTH,
	VERTEX_120,
	FRAGMENT_120,
	VERTEX_FAILURES,
	FRAGMENT_FAILURES,
	NUM_RUN_TYPES
};

const bool kIsVertexShader[NUM_RUN_TYPES] = {
	true,
	false,
	false,
	true,
	false,
	true,
	false,
};

static const EShLanguage kTypeLangs[NUM_RUN_TYPES] = {
	EShLangVertex,
	EShLangFragment,
	EShLangCount,
	EShLangVertex,
	EShLangFragment,
	EShLangVertex,
	EShLangFragment,
};
static std::string GetCompiledShaderText(ShHandle parser)
{
	std::string txt = Hlsl2Glsl_GetShader (parser);
	
	int count = Hlsl2Glsl_GetUniformCount (parser);
	if (count > 0)
	{
		const ShUniformInfo* uni = Hlsl2Glsl_GetUniformInfo(parser);
		txt += "\n// uniforms:\n";
		for (int i = 0; i < count; ++i)
		{
			char buf[1000];
			snprintf(buf,1000,"// %s:%s type %d arrsize %d", uni[i].name, uni[i].semantic?uni[i].semantic:"<none>", uni[i].type, uni[i].arraySize);
			txt += buf;

			if (uni[i].registerSpec)
			{
				txt += " register ";
				txt += uni[i].registerSpec;
			}

			txt += "\n";
		}
	}
	
	return txt;
}
static void replace_string (std::string& target, const std::string& search, const std::string& replace, size_t startPos)
{
	if (search.empty())
		return;
	
	std::string::size_type p = startPos;
	while ((p = target.find (search, p)) != std::string::npos)
	{
		target.replace (p, search.size (), replace);
		p += replace.size ();
	}
}


static bool CheckGLSL (bool vertex, ETargetVersion version, const std::string& source)
{
	const char* sourcePtr = source.c_str();
	std::string newSrc;

	
#	if 0//def __APPLE__
	// Mac core context does not accept any older shader versions, so need to switch to
	// either legacy context or core one.
	const bool need3 = (version >= ETargetGLSL_ES_300);
	bool s_GL3Active = false;
	if (need3)
	{
		if (!s_GL3Active)
			CGLSetCurrentContext(s_GLContext3);
		s_GL3Active = true;
	}
	else
	{
		if (s_GL3Active)
			CGLSetCurrentContext(s_GLContext);
		s_GL3Active = false;
	}
#	endif
	
	
	if (version == ETargetGLSL_ES_100 || version == ETargetGLSL_ES_300)
	{
		newSrc.reserve(source.size());
		if (version == ETargetGLSL_ES_300)
		{
			newSrc += "#version 150\n";
		}
		newSrc += "#define lowp\n";
		newSrc += "#define mediump\n";
		newSrc += "#define highp\n";
		if (version == ETargetGLSL_ES_300)
		{
			newSrc += "#define gl_Vertex _glesVertex\n";
			newSrc += "#define gl_Normal _glesNormal\n";
			newSrc += "#define gl_Color _glesColor\n";
			newSrc += "#define gl_MultiTexCoord0 _glesMultiTexCoord0\n";
			newSrc += "#define gl_MultiTexCoord1 _glesMultiTexCoord1\n";
			newSrc += "#define gl_MultiTexCoord2 _glesMultiTexCoord2\n";
			newSrc += "#define gl_MultiTexCoord3 _glesMultiTexCoord3\n";
			newSrc += "in highp vec4 _glesVertex;\n";
			newSrc += "in highp vec3 _glesNormal;\n";
			newSrc += "in lowp vec4 _glesColor;\n";
			newSrc += "in highp vec4 _glesMultiTexCoord0;\n";
			newSrc += "in highp vec4 _glesMultiTexCoord1;\n";
			newSrc += "in highp vec4 _glesMultiTexCoord2;\n";
			newSrc += "in highp vec4 _glesMultiTexCoord3;\n";
			newSrc += "#define gl_FragData _glesFragData\n";
			newSrc += "out lowp vec4 _glesFragData[4];\n";
		}
		if (version < ETargetGLSL_ES_300)
		{
			newSrc += "#define texture2DLodEXT texture2DLod\n";
			newSrc += "#define texture2DProjLodEXT texture2DProjLod\n";
			newSrc += "#define texture2DGradEXT texture2DGradARB\n";
			newSrc += "#define textureCubeLodEXT textureCubeLod\n";
			newSrc += "#define textureCubeGradEXT textureCubeGradARB\n";
			newSrc += "#define gl_FragDepthEXT gl_FragDepth\n";
			newSrc += "#define gl_LastFragData _glesLastFragData\n";
			newSrc += "varying lowp vec4 _glesLastFragData[4];\n";
			newSrc += "float shadow2DEXT (sampler2DShadow s, vec3 p) { return shadow2D(s,p).r; }\n";
			newSrc += "float shadow2DProjEXT (sampler2DShadow s, vec4 p) { return shadow2DProj(s,p).r; }\n";
			newSrc += "#define sampler2DArrayNV sampler2DArray\n";
			newSrc += "#define texture2DArrayNV texture2DArray\n";
			newSrc += "#define texture2DArrayLodNV texture2DArrayLod\n";
		}
		newSrc += source;
		replace_string (newSrc, "GL_EXT_shader_texture_lod", "GL_ARB_shader_texture_lod", 0);
		replace_string (newSrc, "#extension GL_OES_standard_derivatives : require", "", 0);
		replace_string (newSrc, "#extension GL_EXT_shadow_samplers : require", "", 0);
		replace_string (newSrc, "#extension GL_EXT_frag_depth : require", "", 0);
		replace_string (newSrc, "#extension GL_EXT_shader_framebuffer_fetch : require", "", 0);
		replace_string (newSrc, "#extension GL_EXT_draw_buffers : require", "", 0);
		replace_string (newSrc, "GL_EXT_draw_instanced", "GL_ARB_draw_instanced", 0);
		replace_string (newSrc, "GL_NV_texture_array", "GL_EXT_texture_array", 0);
		replace_string (newSrc, "gl_InstanceIDEXT", "gl_InstanceIDARB", 0);
		
		sourcePtr = newSrc.c_str();
	}
	
	GLuint shader = glCreateShader (vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	glShaderSource (shader, 1, &sourcePtr, NULL);
	glCompileShader (shader);
	GLint status;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
	bool res = true;
	if (status == 0)
	{
		char log[4096];
		GLsizei logLength;
		glGetShaderInfoLog (shader, sizeof(log), &logLength, log);
		LOG_PRINT ("  glsl compile error:\n%s\n", log);
		res = false;
	}
	glDeleteShader (shader);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		LOG_PRINT("  GL error: 0x%x\n", err);
		res = false;
	}
	return res;
}

bool TestFile (bool vertex,
	const std::string& asourceStr,
	std::string& text,
	const std::string& main,
	bool usePrecision=false,
	bool doCheckGLSL=false)
{
	TestRun type=VERTEX;
	if(!vertex) type=FRAGMENT;
	ETargetVersion version=ETargetGLSL_110;
	const char* entryPoint=main.c_str();
	unsigned options=0;
	bool res = true;
	
	std::string input = asourceStr;
	ShHandle parser = Hlsl2Glsl_ConstructCompiler (kTypeLangs[type]);

	const char* sourceStr = asourceStr.c_str();
		
	int parseOk = Hlsl2Glsl_Parse (parser, sourceStr, version, 0, options);
	const char* infoLog = Hlsl2Glsl_GetInfoLog( parser );

	if (parseOk)
	{
		static EAttribSemantic kAttribSemantic[] = {
			EAttrSemTangent,
		};
		static const char* kAttribString[] = {
			"TANGENT",
		};
		Hlsl2Glsl_SetUserAttributeNames (parser, kAttribSemantic, kAttribString, 1);
		
		int translateOk = Hlsl2Glsl_Translate (parser, entryPoint, version, options);
		const char* infoLog = Hlsl2Glsl_GetInfoLog( parser );
		if (translateOk)
		{
			text = GetCompiledShaderText(parser);
			//LOG_FNLN;
			//LOG_PRINT ("%s\n", text.c_str());
			
			for (size_t i = 0, n = text.size(); i != n; ++i)
			{
			   char c = text[i];
			   
			   if (!isascii(c))
			   {
				   LOG_PRINT ("  contains non-ascii '%c' (0x%02X)\n", c, c);
				   res = false;
			   }
			}

			std::string output;

			if (doCheckGLSL && !CheckGLSL (kIsVertexShader[type], version, text))
			{
				res = false;
			}
		}
		else
		{
			LOG_PRINT ("  translate error: %s\n", infoLog);
			res = false;
		}
	}
	else
	{
		LOG_PRINT ("  parse error: %s\n", infoLog);
		res = false;
	}

	Hlsl2Glsl_DestructCompiler (parser);

	return res;

}
bool TestFile_eitoimi (bool vertex,
	const std::string& inputPath,
	std::string& text,
	const std::string& main,
	bool usePrecision=false,
	bool doCheckGLSL=false)
{
	//text.append("#version 120\n");

	//std::string input=inputPath;

	ShHandle parser = Hlsl2Glsl_ConstructCompiler (vertex ? EShLangVertex : EShLangFragment);


	const char* sourceStr = inputPath.c_str();


	bool res = true;


	int options = 0;


	if (1)//kDumpShaderAST)
		options |= ETranslateOpIntermediate;


	//if (0)//usePrecision)
	//	options |= ETranslateOpUsePrecision;


/* ???
 	ETargetGLSL_110,
	ETargetGLSL_110,
	ETargetGLSL_120,
	ETargetGLSL_120,
	ETargetGLSL_120,
	ETargetGLSL_110,
	ETargetGLSL_110,
*/
	const ETargetVersion version = ETargetGLSL_110;
	int parseOk = Hlsl2Glsl_Parse (parser, sourceStr, version, NULL, options);

	const char* infoLog = Hlsl2Glsl_GetInfoLog( parser );


	/*if (1)//kDumpShaderAST)
	{
		// write output
		outputPath=infoLog;

	}*/


	if (parseOk)
	{


		static EAttribSemantic kAttribSemantic[] = {
			EAttrSemTangent,
		};


		static const char* kAttribString[] = {
			"TANGENT",
		};


		Hlsl2Glsl_SetUserAttributeNames (parser, kAttribSemantic, kAttribString, 1);


		//Hlsl2Glsl_UseUserVaryings (parser, true);


		int translateOk = Hlsl2Glsl_Translate (parser, main.c_str(), version, options);


		const char* infoLog = Hlsl2Glsl_GetInfoLog( parser );


		if (translateOk)
		{


			text.append(Hlsl2Glsl_GetShader (parser));
			LOG_FNLN;
			LOG_PRINT ("%s\n", text.c_str());

		}
		else
		{
			LOG_PRINT ("  (XXX) translate error: %s\n", infoLog);

			////LOG_PRINT("\n<1:>\n%s\n%s\n</1:>\n", main.c_str(), sourceStr);

			STX_STACK_TRACE;
			exit(0);//???
			res = false;
		}
	}
	else
	{
		LOG_PRINT ("  (XXX) parse error: %s\n", infoLog);

			////LOG_PRINT("\n<2:>\n%s\n%s\n</2:>\n", main.c_str(), sourceStr);

		//STX_STACK_TRACE;
		exit(0);//???
		res = false;
	}


	Hlsl2Glsl_DestructCompiler (parser);


	return res;
}
#endif
#ifdef USE_HLSL_SHADERS//def _MSC_VER
bool RendererGLSLGL_1_1::TranslateVertexShader(const std::string& hlsl, const std::string& main, std::string& glsl)
{
	//LOG_PRINT("\nHLSL:\n%s\n",hlsl.c_str());
#if 0
	TDebugOptions debugOptions//=EDebugOpNone;
					=EDebugOpIntermediate;

    // Initialize the HLSL to GLSL translator
    Hlsl2Glsl_Initialize();

    // Construct a vertex shader parser for the translator to use
    ShHandle parserv = Hlsl2Glsl_ConstructParser(EShLangVertex, 0);

    // Construct a Translator to use for final code translation
    ShHandle translator = Hlsl2Glsl_ConstructTranslator(debugOptions);

#if 1
    // Parse the shader  print out the info log on failure
    const char *shaderStrings[1] = { hlsl.c_str() };
    if ( !Hlsl2Glsl_Parse(parserv, shaderStrings, 1, debugOptions) )
    {
       LOG_PRINT( Hlsl2Glsl_GetInfoLog(parserv));
       //DBG_HALT;
    }

    // Now translate the parsed shader
    ShHandle parsers[1] = { parserv };
    if (! Hlsl2Glsl_Translate(translator, parsers, 1, main.c_str(), 0) )
    {
       LOG_PRINT( Hlsl2Glsl_GetInfoLog(translator));
       //DBG_HALT;
    }

    // Finally, get the translated shader source (GLSL)
    const char* text = Hlsl2Glsl_GetShader( translator,EShLangVertex );
#endif
	ShHandle parser = Hlsl2Glsl_ConstructCompiler (1 ? EShLangVertex : EShLangFragment);

	glsl="";
	glsl.append(text);
	LOG_PRINT("\nGLSL:\n%s\n",glsl.c_str());
#if 1
	// ...shutdown
    Hlsl2Glsl_Destruct( parserv );
    Hlsl2Glsl_Destruct( translator );
    Hlsl2Glsl_Finalize( );
#endif
	#endif
	return true;
}
bool RendererGLSLGL_1_1::TranslateFragmentShader(const std::string& hlsl, const std::string& main, std::string& glsl)
{
	//LOG_PRINT("\nHLSL:\n%s\n",hlsl.c_str());
#if 0
	TDebugOptions debugOptions//=EDebugOpNone;
					=EDebugOpIntermediate;

    // Initialize the HLSL to GLSL translator
    //Hlsl2Glsl_Initialize();

    // Construct a vertex shader parser for the translator to use
    //ShHandle parserf = Hlsl2Glsl_ConstructParser(EShLangFragment, 0);

    // Construct a Translator to use for final code translation
    //ShHandle translator = Hlsl2Glsl_ConstructTranslator(debugOptions);

#if 1
    // Parse the shader  print out the info log on failure
    const char *shaderStrings[1] = { hlsl.c_str() };
    if ( !Hlsl2Glsl_Parse(parserf, shaderStrings, 1, debugOptions) )
    {
       LOG_PRINT( Hlsl2Glsl_GetInfoLog(parserf));
       //DBG_HALT;
    }

    // Now translate the parsed shader
    ShHandle parsers[1] = { parserf };
    if (! Hlsl2Glsl_Translate(translator, parsers, 1, 0, main.c_str()) )
    {
       LOG_PRINT( Hlsl2Glsl_GetInfoLog(translator));
       //DBG_HALT;
    }

    // Finally, get the translated shader source (GLSL)
    const char* text = Hlsl2Glsl_GetShader( translator,EShLangFragment );
#endif
	glsl="";

	ShHandle parser = Hlsl2Glsl_ConstructCompiler (0 ? EShLangVertex : EShLangFragment);

	glsl.append(text);
	LOG_PRINT("\nGLSL:\n%s\n",glsl.c_str());
#if 1
    // ...shutdown
    Hlsl2Glsl_Destruct( parserf );
    Hlsl2Glsl_Destruct( translator );
    Hlsl2Glsl_Finalize( );
#endif
	#endif
	return true;
}
#endif

//void RendererGL_1_1::
		void RendererGLSLGL_1_1::changeVertexBufferVrtl(const int stream, const VertexBufferID vertexBuffer, const intptr offset)
		{
#if !defined(USE_HLSL_SHADERS)

			VertexFormatGL cvf = vertexFormats[selectedVertexFormat];

			int vertexSize = cvf.vertexSize[stream];

			if(!vertexSize) return;

			int nGeneric=cvf.maxGeneric;

			int nAttribs=cvf.maxGeneric;

			int nTexCoord=cvf.maxTexCoord;

			char *base = (char *)&vertexBuffers[vertexBuffer].data;//(char *) offset;

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
					for (int i = 0; i < MAX_GENERIC; i++)
					{

						if (cvf.generic[i].stream == stream && cvf.generic[i].size)
						{

							glVertexAttribPointerARB(i, cvf.generic[i].size, glTypes[cvf.generic[i].format], GL_TRUE, vertexSize, base + cvf.generic[i].offset);
				checkGlError("");
						}
					}
					for (int i = 0; i < MAX_TEXCOORD; i++)
					{

						if (cvf.texCoord[i].stream == stream && cvf.texCoord[i].size)
						{

							glClientActiveTexture(GL_TEXTURE0 + i);
				checkGlError("");

							glTexCoordPointer(cvf.texCoord[i].size, glTypes[cvf.texCoord[i].format], vertexSize, base + cvf.texCoord[i].offset);
				checkGlError("");
						}
					}
				}

				currentVertexBuffers[stream] = vertexBuffer;

				currentOffsets[stream] = offset;

				activeVertexFormat[stream] = currentVertexFormat;

			}
#endif

		}
void RendererGLSLGL_1_1::applyConstants()
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
					((UNIFORM_MAT_FUNC) uniformFuncs[(uni)->type])((uni)->index, (uni)->nElements, GL_TRUE, reinterpret_cast<float*>(&uni->data[0]));
				}
				else
				{
					((UNIFORM_FUNC) uniformFuncs[(uni)->type])((uni)->index, (uni)->nElements, reinterpret_cast<float*>(&uni->data[0]));
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


					LOG_PRINT("glVertexAttribPointer:\n");
					LOG_PRINT("loc=%d\n",att->loc);
					LOG_PRINT("offset=%d\n",att->offset);
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

void RendererGLSLGL_1_1::setTexture(const char *textureName, const TextureID texture)
{
	if(texture==-1) return;//DBG_HALT;
	if(selectedShader == -1) return;

	int unit = getSamplerUnit(selectedShader, textureName);
	if (unit >= 0)
	{
		selectedTextures[unit] = texture;
		//LOG_PRINT("%s\n",textureName);
	}
}
void RendererGLSLGL_1_1::setTexture(const char *textureName, const TextureID texture, const SamplerStateID samplerState, const int vtstream, const VertexBufferID vtbuffer)
{
	if(texture==-1) //DBG_HALT;
	if(selectedShader == SHADER_NONE) return;

	int unit = getSamplerUnit(selectedShader, textureName);
	if (unit >= 0)
	{
		selectedTextures[unit] = texture;
		selectedSamplerStates[unit] = samplerState;
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
void RendererGLSLGL_1_1::DrawPrimitive(Primitives PrimitiveType,UINT StartVertex,UINT PrimitiveCount, const uint flags)
		{
			//
			apply();
			//glDrawArrays(glPrim[PrimitiveType], StartVertex,getVertexCount(PrimitiveType,PrimitiveCount));

				//LOG_PRINT("=%d\n",getVertexCount(PrimitiveType,PrimitiveCount));

				//LOG_PRINT("=%d\n",selectedVertexFormat);

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
		void RendererGLSLGL_1_1::DrawIndexedPrimitive(Primitives PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount, const uint flags)
		{
			//
			//if((currentIndexBuffer<0)||(currentIndexBuffer>=indexBuffers.size()))return;
			apply();
			#if 0
			LOG_PRINT("=%d\n",			getIndicesCount(PrimitiveType));
			LOG_PRINT("=%d\n",			primCount);
			LOG_PRINT("=%d\n",			(indexBuffers[currentIndexBuffer]).indexSize);
			LOG_PRINT("=%d\n",			BUFFER_OFFSET((indexBuffers[currentIndexBuffer]).indexSize * startIndex));
			#endif


				//LOG_PRINT("=%d\n",selectedVertexFormat);

			glDrawElements(
			glPrim[PrimitiveType],
			getIndicesCount(PrimitiveType)*primCount,
			((indexBuffers[selectedIndexBuffer]).indexSize==2)? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
			BUFFER_OFFSET((indexBuffers[selectedIndexBuffer]).indexSize * startIndex));
				checkGlError("");

			reset(flags);
			nDrawCalls++;
		}
void RendererGLSLGL_1_1::changeShader(const ShaderID shader)
		{
			if (shader != currentShader)
			{
				if (shader == SHADER_NONE)
				{
					glUseProgramObjectARB(0);
				checkGlError("");
					//LOG_PRINT("glUseProgramObjectARB(0)\n");
				}
				else
				{
					glUseProgramObjectARB((shaders[shader]).program);
				checkGlError("");
					//LOG_PRINT("shader %d:glUseProgramObjectARB(%d)\n",shader,(shaders[shader]).program);
				}
				currentShader = shader;
			}
		}
void RendererGLSLGL_1_1::setShaderConstantRaw(const char *name, const void *data, const int size)
		{
			if(selectedShader==-1)return;
			{
			int minUniform = 0;
			int maxUniform = (shaders[selectedShader]).nUniforms - 1;

			while (minUniform <= maxUniform)
			{
				int currUniform = (minUniform + maxUniform) >> 1;
				int res = strcmp(name, ((shaders[selectedShader]).uniforms[currUniform]).name.c_str());
				if (res == 0)
				{


			ConstantShaderGLSLGL3* uni = (&(shaders[selectedShader]).uniforms[0]) + currUniform;

					if (memcmp(&uni->data[0], data, size))
					{
						uni->data.resize(size);
						stx_memcpy(&uni->data[0], data, size);
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
			#if 0
			{
			int minAttrib = 0;
			int maxAttrib = (shaders[selectedShader]).nAttribs - 1;

			//
			while (minAttrib <= maxAttrib)
			{
				int currAttrib = (minAttrib + maxAttrib) >> 1;
				int res = strcmp(name, ((shaders[selectedShader]).attribs[currAttrib]).name.c_str());


				LOG_PRINT("name1=%s\n",name);
				LOG_PRINT("name2=%s\n",((shaders[selectedShader]).attribs[currAttrib]).name.c_str());

				if (res == 0)
				{


			AttribShaderGLSLGL3* att = (&(shaders[selectedShader]).attribs[0]) + currAttrib;

					//
					//if (memcmp(&att->data[0], data, size))
					{
						//memcpy(&att->data[0], data, size);

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
		int samplerCompareGL2(const void *sampler0, const void *sampler1)
		{
			return strcmp(((SamplerShaderGLSLGL3 *) sampler0)->name.c_str(), ((SamplerShaderGLSLGL3 *) sampler1)->name.c_str());
		}
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

#if 1
		bool samplerCompGL2std(const SamplerShaderGLSLGL3 & s0, const SamplerShaderGLSLGL3 &s1)
		{
       if(!(s0.name.c_str()&& s1.name.c_str()))return false;
       int i=strcmp(s0.name.c_str(), s1.name.c_str());
       if(i>0)
	      return false;
       if(i<0)
	      return true;
       return false;
		}
		bool constantCompGL2std(const ConstantShaderGLSLGL3 & s0, const ConstantShaderGLSLGL3 &s1)
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
		bool attribCompGL2std(const AttribShaderGLSLGL3 & s0, const AttribShaderGLSLGL3 &s1)
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

	  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &active_attribs);
				checkGlError("");
	  glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_length);
				checkGlError("");

	  name = (char*)malloc(max_length + 1);
	  //LOG_PRINT("Active vertex shader inputs:\n");
	  GLint stride=0;
	  for (i = 0; i < active_attribs; i++) {
	    GLint size;
	    GLenum type;

	    glGetActiveAttrib(prog, i, max_length + 1, 0,
	                      &size, &type, name);
				checkGlError("");
		stride+=size;
		};
	  for (i = 0; i < active_attribs; i++) {
	    GLint size;
	    GLenum type;

	    glGetActiveAttrib(prog, i, max_length + 1, 0,
	                      &size, &type, name);
				checkGlError("");
	    //LOG_PRINT("Vertex input attribute %s of type %d size %d is at location %d, stride %d\n", name, type, size, glGetAttribLocation(prog, name),stride);
	  }
	  free(name);
	}
		ShaderID RendererGLSLGL_1_1::addGLSLShader(const char *vsText, const char *gsText, const char *fsText,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags)

		{
			

			
			const char * mainvs=vsName;

			
			const char * mainps=psName;



			 //  if(!TranslateVertexShader(glslvs.c_str(), mainvs, glslvs)) return SHADER_NONE;//DBG_HALT;
			//const char * vsText=glslvs.c_str();


//if(!TranslateFragmentShader(glslfs.c_str(), mainps, glslfs)) return SHADER_NONE;//DBG_HALT;//
     //replaceAll(glslfs,rm,r);
			//const char * fsText=glslfs.c_str();

			
			
			ShaderGLSLGL3 shader;

			
			
			const GLcharARB *shaderStrings[6];

			
			int strIndex = 0;

			
			char line[16];

			
			GLint vsResult, fsResult, linkResult;

			
			char infoLog[2048];

			
			GLint len, infoLogPos = 0;




			
			
			if (GL_ARB_shading_language_100_supported)
			{


				
			
			static char versionString[16] = {0};

				
			static bool first = true;


				
			if (first)
				{


					
			
			const char *version = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION_ARB);

					
					if (version)
					{

	
			#if 0
			int major = atoi(version);
                        int minor = atoi(stx_strchr(version, '.') + 1);
                        stx_snprintf(versionString,16, "#version %d%d\n", major, minor);
    #else

        
					stx_snprintf(versionString,16, "#version 120\n");

    #endif



}

								
					first = false;


}

							
					//shaderStrings[strIndex++] = versionString;


}

			
#if defined(_MSC_VER)
			shaderStrings[strIndex++] ="precision highp float;\n";
#endif

#if 0
			
			if (extra) shaderStrings[strIndex++] = extra;

			
			if (header) shaderStrings[strIndex++] = header;

			
			shaderStrings[strIndex++] = line;
#endif

			
			shader.vertexShader = 0;

			
			shader.fragmentShader = 0;

			
			shader.program = glCreateProgramObjectARB();

			checkGlError("");

			//


						
			if (vsText != 0)
			{

		
					
				shader.vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
                stx_snprintf(line,16, "//line %d\n", vsLine);

				checkGlError("");



				
			shaderStrings[strIndex] = vsText;

			#if 0
			LOG_FNLN;
			for(unsigned int i=0;i<(strIndex+1);i++)
			{
				LOG_PRINT("shaderStrings[%d]=%s\n", i, shaderStrings[i]);
			}
			//LOG_PRINT("vsText:\n%s\n", vsText);
			#endif
				
			glShaderSourceARB(shader.vertexShader, strIndex + 1, shaderStrings, 0);

				checkGlError("");

				glCompileShaderARB(shader.vertexShader);


				checkGlError("");

				glGetObjectParameterivARB(shader.vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &vsResult);

				checkGlError("");

				if (vsResult)
				{

					
			glAttachObjectARB(shader.program, shader.vertexShader);

				checkGlError("");

				}
				else
				{

                    
			infoLogPos += stx_snprintf(infoLog + infoLogPos,2048, "Vertex shader error:\n");

				
			}

				glGetInfoLogARB(shader.vertexShader, sizeof(infoLog) - infoLogPos, &len, infoLog + infoLogPos);

				checkGlError("");

				infoLogPos += len;

			
			}
			else vsResult = GL_TRUE;

			//

						
			if (fsText != 0)
			{

			
				
			shader.fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

                
			stx_snprintf(line,16, "//line %d\n", fsLine);

				
			shaderStrings[strIndex] = fsText;

			#if 0
			LOG_FNLN;
			for(unsigned int i=0;i<(strIndex+1);i++)
			{
				LOG_PRINT("shaderStrings[%d]=%s\n", i, shaderStrings[i]);
			}
			//LOG_PRINT("fsText:\n%s\n", fsText);
			#endif

			glShaderSourceARB(shader.fragmentShader, strIndex + 1, shaderStrings, 0);

				checkGlError("");

				glCompileShaderARB(shader.fragmentShader);

				checkGlError("");

				glGetObjectParameterivARB(shader.fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &fsResult);

				checkGlError("");

				if (fsResult)
				{

					glAttachObjectARB(shader.program, shader.fragmentShader);

				checkGlError("");

				}
				else
				{

                    
			infoLogPos += stx_snprintf(infoLog + infoLogPos,2048, "Fragment shader error:\n");

				
			}

				
			glGetInfoLogARB(shader.fragmentShader, sizeof(infoLog) - infoLogPos, &len, infoLog + infoLogPos);

				checkGlError("");

				infoLogPos += len;

			
			}
			else fsResult = GL_TRUE;

			//
			
			if (vsResult && fsResult)
			{


				
			for (int i = 0; i < nAttributes; i++)
				{

					if (attributeNames[i]) glBindAttribLocationARB(shader.program, i, attributeNames[i]);

				}


				
			glLinkProgramARB(shader.program);

				checkGlError("");

				glGetObjectParameterivARB(shader.program, GL_OBJECT_LINK_STATUS_ARB, &linkResult);

				checkGlError("");

				glGetInfoLogARB(shader.program, sizeof(infoLog) - infoLogPos, &len, infoLog + infoLogPos);

				checkGlError("");

				infoLogPos += len;

				
			LOG_PRINT("%s", infoLog);

			if (linkResult)
				{
		
			GLuint currProgram = 

#ifdef __APPLE__
handle_to_uint((currentShader == SHADER_NONE)? 0 : (shaders[currentShader]).program);
#else
(currentShader == SHADER_NONE)? 0 : (shaders[currentShader]).program;
#endif
					
#ifdef __APPLE__
glUseProgram(handle_to_uint(shader.program));
#else
glUseProgram(shader.program);
#endif

				checkGlError("");

					GLint uniformCount, maxLength;

					
			GLint numActiveAttribs,maxAttribNameLength = 0;

					
			glGetObjectParameterivARB(shader.program, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &uniformCount);


				checkGlError("");

					glGetObjectParameterivARB(shader.program, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxLength);


				checkGlError("");




					std::vector<SamplerShaderGLSLGL3> samplers;

					
			std::vector<ConstantShaderGLSLGL3 > uniforms;

					
			std::vector<AttribShaderGLSLGL3 > attribs;



					
			samplers.resize(uniformCount);

					
			uniforms.resize(uniformCount);


					
			attribs.resize(uniformCount);


					
			int nSamplers = 0;

					
			int nUniforms = 0;

					
			int nAttribs = 0;

					
			std::vector<char> bbuf(maxLength);

					
			char *name = (char *)&bbuf[0];


{
#ifdef __APPLE__
GLint prog=handle_to_uint(shader.program);
#else
GLint prog=shader.program;
#endif

			GLint maxAttribNameLength,numActiveAttribs = 0;


			glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);


				checkGlError("");

glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);


				checkGlError("");


std::vector<GLchar> nameData(maxAttribNameLength);


			for(int attrib = 0; attrib < numActiveAttribs; ++attrib)
{
	
	//LOG_PRINT("attrib=%d\n", attrib);
	
	GLint arraySize = 0;
	
	GLenum type = 0;
	
	GLsizei actualLength = 0;
	
	glGetActiveAttrib(prog, attrib, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
	
	std::string name((char*)&nameData[0], actualLength );

				checkGlError("");

	//LOG_PRINT("name=%s\n", name.c_str());

  	
	GLuint loc = glGetAttribLocation ( prog, name.c_str());

				checkGlError("");


	/*

	LOG_PRINT("name=%s\n", name.c_str());
	LOG_PRINT("loc=%d\n",loc);
	*/
if (name.c_str()!=0)
{

	//LOG_PRINT("name=%s\n", name.c_str());

//LOG_PRINT("loc=%d\n",loc);

	//LOG_PRINT("nAttribs=%d\n", nAttribs);

	
	attribs[attrib].loc=loc;
	/*
	LOG_PRINT("loc=%d\n", loc);
	LOG_PRINT("name=%s\n", name.c_str());
	LOG_PRINT("nAttribs=%d\n", nAttribs);
	*/
	//LOG_PRINT("attribs[%d].name=%s\n", attrib, attribs[attrib].name.c_str());
	//attribs[attrib].name=name;
	
	nAttribs++;


	}
else
{

	//LOG_PRINT("name=%s\n", name.c_str());

	//LOG_PRINT("loc=%d\n",loc);

}

}

}
	


					
			for (int i = 0; i < uniformCount; i++)
					{

						GLenum type;
						GLint length, size;
						


#ifdef __APPLE__
glGetActiveUniform(handle_to_uint(shader.program), i, maxLength, &length, &size, &type, name);
#else
glGetActiveUniform(shader.program, i, maxLength, &length, &size, &type, name);
#endif


				checkGlError("");
						//LOG_PRINT("u=%s\n",name);
						if (type >= GL_SAMPLER_1D && type <= GL_SAMPLER_2D_RECT_SHADOW_ARB)
						{


#ifdef __APPLE__
GLint location = glGetUniformLocation(handle_to_uint(shader.program), name);
#else
GLint location = glGetUniformLocation(shader.program, name);
#endif

							

				checkGlError("");
							glUniform1i(location, nSamplers);

				checkGlError("");
							samplers[nSamplers].index = nSamplers;
							samplers[nSamplers].name=std::string( name);
							//LOG_PRINT("u=%s\n",name);
							nSamplers++;
						}
						else
						{
							if (strncmp(name, "gl_", 3) != 0)
							{
                                char *bracket = stx_strchr(name, '[');
								if (bracket == 0 || (bracket[1] == '0' && bracket[2] == ']'))
								{
									if (bracket)
									{
										*bracket = '\0';
										length = (GLint) (bracket - name);
									}
#ifdef __APPLE__
uniforms[nUniforms].index = glGetUniformLocation(handle_to_uint(shader.program), name);
#else
uniforms[nUniforms].index = glGetUniformLocation(shader.program, name);
#endif

				checkGlError("");
									uniforms[nUniforms].type = getConstantType(type);
									uniforms[nUniforms].nElements = size;
									uniforms[nUniforms].name=std::string( name);
									nUniforms++;
								}
								else if (bracket != 0 && bracket[1] > '0')
								{
									*bracket = '\0';
									for (int i = nUniforms - 1; i >= 0; i--)
									{
										if (strcmp(uniforms[i].name.c_str(), name) == 0)
										{
											int index = atoi(bracket + 1) + 1;
											if (index > uniforms[i].nElements)
											{
												uniforms[i].nElements = index;
											}
										}
									}
								}
							}
						}
					}

					
			glUseProgram(currProgram);

				checkGlError("");

					//delete[] name;


					
			samplers.resize(nSamplers);
					
			uniforms.resize(nUniforms);
					
			attribs.resize(nAttribs);


					
			#if 0
					qsort(&samplers[0], nSamplers, sizeof(SamplerShaderGLSLGL3),  samplerCompShaderGLSLGL3);
					qsort(&uniforms[0], nUniforms, sizeof(ConstantShaderGLSLGL3), constantCompShaderGLSLGL3);
					#elif 1

					
			if(samplers.size()>1) std::sort(samplers.begin(), samplers.end(), samplerCompGL2std);

					
			if(uniforms.size()>1) std::sort(uniforms.begin(), uniforms.end(), constantCompGL2std);

					
			//if(attribs.size()>1) std::sort(attribs.begin(), attribs.end(), attribCompGL2std);


					#endif

					
			for (int i = 0; i < nUniforms; i++)
					{
						//LOG_PRINT("i: %d\n",i);
						int constantSize = constantTypeSizes[uniforms[i].type] * uniforms[i].nElements;
/*						LOG_PRINT("name %s\n",uniforms[i].name.c_str());
						LOG_PRINT("index %d\n",uniforms[i].index);
						LOG_PRINT("type %d\n",uniforms[i].type);
						LOG_PRINT("nE %d\n",uniforms[i].nElements);
						LOG_PRINT("cs %d\n",constantSize);
*/
						uniforms[i].data.resize(constantSize);

						memset((void*)&uniforms[i].data[0], 0, constantSize);

						uniforms[i].dirty = false;
					}


					
			shader.attribs = attribs;
					shader.uniforms  = uniforms;
					
			shader.samplers  = samplers;
					
			shader.nAttribs = nAttribs;
					
			shader.nUniforms = nUniforms;
					
			shader.nSamplers = nSamplers;

					
			shaders.push_back(shader);

#ifdef __APPLE__
active_vertex_shader_inputs(handle_to_uint(shader.program));
#else
active_vertex_shader_inputs(shader.program);
#endif					
			
					//exit(0);

					
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
		ShaderID RendererGLSLGL_1_1::addHLSLShaderVrtl(const char *vsText, const char *gsText, const char *fsText,
		const char *vsName, const char *psName,
		const int vsLine, const int gsLine, const int fsLine,
		const char *header, const char *extra, const char *fileName, const char **attributeNames, const int nAttributes, const uint flags)
		{


			#if 1
			std::string shaderStrings="#define WSIGN +\n#define ROW_MAJOR\n#define MVPSEMANTIC\n";

			std::string vsText0=shaderStrings;

			std::string fsText0=shaderStrings;

			if (vsText) vsText0.append(std::string(vsText));

			if (fsText) fsText0.append(std::string(fsText));

			////LOG_PRINT("\n<3:>\n%s\n%s\n</3:>\n", vsName, vsText0.c_str());
			////LOG_PRINT("\n<4:>\n%s\n%s\n</4:>\n", psName, fsText0.c_str());
#endif
#if 0

	HLSL2GLSLBoostTranslator c;
	Shader* cs=0;
if(currentShader!=SHADER_NONE)
cs=&shaders[currentShader];
		ShaderGLSLGL3* shader=(ShaderGLSLGL3*)c.addHLSLShader(cs,vsText0.c_str(), gsText, fsText0.c_str(),
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);

		if((shader->GLSLVS==0)||(shader->GLSLFS==0))DBG_HALT;

        if((stx_strcmp(shader->GLSLVS,"")==0)||(stx_strcmp(shader->GLSLFS,"")==0))DBG_HALT;

		ShaderID id=addGLSLShader(
			shader->GLSLVS,
			0,
			shader->GLSLFS,
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);
delete shader;
					return id;
}
#elif defined(USE_HLSL_SHADERS)

	HLSL2GLSLHLSL2GLSLTranslator c;

	Shader* cs=0;

if(currentShader!=SHADER_NONE)
cs=&shaders[currentShader];


		ShaderGLSLGL3* shader=(ShaderGLSLGL3*)c.addHLSLShader(cs,vsText0.c_str(),
			gsText, fsText0.c_str(),
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);

#if 0

//STX_STACK_TRACE;
//exit(0);
#endif

		ShaderID id=addGLSLShader(
			shader->GLSLVS.c_str(),
			0,
			shader->GLSLFS.c_str(),
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);


delete shader;

#elif !defined(USE_HLSL_SHADERS)
#if 0
		LOG_PRINT("addGLSLShader\n");
		exit(0);
#elif 1
#if 1

//STX_STACK_TRACE;
//exit(0);
#endif

		ShaderID id=addGLSLShader(
			vsText0.c_str(),
			0,
			fsText0.c_str(),
		vsName, psName,
		vsLine, gsLine, fsLine,
		header, extra, fileName, attributeNames, nAttributes, flags);

#endif
#endif
		return id;
}

		static const GLenum targets_GLSL_1_1[] =
		{
GL_TEXTURE_1D,
GL_TEXTURE_2D,
GL_TEXTURE_3D,
GL_TEXTURE_CUBE_MAP
		};

		static const char* targets_GLSL_1_1_[] =
		{
"GL_TEXTURE_1D",
"GL_TEXTURE_2D",
"GL_TEXTURE_3D",
"GL_TEXTURE_CUBE_MAP"
		};

void RendererGLSLGL_1_1::applyTextures()
{
	for (uint i = 0; i < MAX_TEXTUREUNIT; i++){
		TextureID texture = selectedTextures[i];
		TextureID currTex = currentTextures[i];
		if (texture != currTex){
			/* if (texture == TEXTURE_NONE){
				//LOG_PRINT("glDisable texture\n")
				LOG_PRINT("glDisable %d\n",textures[currTex].glTexID);
				glDisable(textures[currTex].glTarget);
				glBindTexture(textures[currTex].glTarget, 0);
			} */
			if (texture != TEXTURE_NONE) {
				/*
				LOG_PRINT("glEnable textureID=%d, glTarget=%d\n",texture,textures[texture].glTarget);
				LOG_PRINT("glBindTexture\n");
				*/
				//glActiveTextureARB(GL_TEXTURE0 + i);
#if 0
				LOG_FNLN;
				LOG_PRINT("i=%d\n",i);
				LOG_PRINT("texture=%d\n",texture);
				LOG_PRINT("currTex=%d\n",currTex);
				LOG_PRINT("textureN=%d\n",textures.size());
				LOG_PRINT("glTarget=%d\n",textures[texture].glTarget);
				LOG_PRINT("glTarget_=%s\n",targets_GLSL_1_1_[textures[texture].glTarget]);
#endif
				if(texture+1>textures.size())
					continue;

				glEnable(textures[texture].glTarget);
				checkGlError("");
				glBindTexture(textures[texture].glTarget, textures[texture].glTexID);
				checkGlError("");

			}
			currentTextures[i] = texture;
		}
		else if(0)
		{
			//LOG_PRINT("glDisable %d\n",textures[currTex].glTexID);
			glDisable(textures[currTex].glTarget);
				checkGlError("");
			glBindTexture(textures[currTex].glTarget, 0);
				checkGlError("");
		}
	}
}

		int RendererGLSLGL_1_1::getSamplerUnit(const ShaderID shader, const char *samplerName) const
				{
					//ASSERT(shader != -1);
					std::vector<SamplerShaderGLSLGL3 >samplers = (shaders[shader])
					.samplers;
					int minSampler = 0;
					int maxSampler = (shaders[shader]).nSamplers - 1;
					while (minSampler <= maxSampler)
					{
						int currSampler = (minSampler + maxSampler) >> 1;
						int res = strcmp(samplerName, (samplers[currSampler]).name.c_str());
						if (res == 0)
						{
							return (samplers[currSampler]).index;
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


		RendererGLSLGL_1_1::RendererGLSLGL_1_1(
		int awidth, int aheight,
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

	hDc = GetDC( STX_Service::GetWindowInstance()->GetHWND() );
	PixelFormat = ChoosePixelFormat( m_hDC, &pfd );
	SetPixelFormat( m_hDC, PixelFormat, &pfd);

	hGlrc = wglCreateContext( hDc);

	wglMakeCurrent( m_hDC, hGlrc );


#else
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
	//initExtensions(m_dpy);
	importGLInit();

#endif

			GLint units = 1;
#ifdef _WIN32
			if (GL_ARB_fragment_shader_supported || GL_ARB_fragment_program_supported)
			{
				glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &units);
				checkGlError("");
			}
			else
#endif
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
			reset(0);
			memset(&activeVertexFormat, VF_NONE, sizeof(activeVertexFormat));
			uniformFuncs[CONSTANT_FLOAT] = (void *) glUniform1fvARB;
			uniformFuncs[CONSTANT_VEC2]  = (void *) glUniform2fvARB;
			uniformFuncs[CONSTANT_VEC3]  = (void *) glUniform3fvARB;
			uniformFuncs[CONSTANT_VEC4]  = (void *) glUniform4fvARB;
			uniformFuncs[CONSTANT_INT]   = (void *) glUniform1ivARB;
			uniformFuncs[CONSTANT_IVEC2] = (void *) glUniform2ivARB;
			uniformFuncs[CONSTANT_IVEC3] = (void *) glUniform3ivARB;
			uniformFuncs[CONSTANT_IVEC4] = (void *) glUniform4ivARB;
			uniformFuncs[CONSTANT_BOOL]  = (void *) glUniform1ivARB;
			uniformFuncs[CONSTANT_BVEC2] = (void *) glUniform2ivARB;
			uniformFuncs[CONSTANT_BVEC3] = (void *) glUniform3ivARB;
			uniformFuncs[CONSTANT_BVEC4] = (void *) glUniform4ivARB;
			uniformFuncs[CONSTANT_MAT2]  = (void *) glUniformMatrix2fvARB;
			uniformFuncs[CONSTANT_MAT3]  = (void *) glUniformMatrix3fvARB;
			uniformFuncs[CONSTANT_MAT4]  = (void *) glUniformMatrix4fvARB;
			bsupportsHalf=false;
			bsupportsR2VB=false;
			overrideR2VB=true;
			overrideHalf=true;

		//defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);
//LOG_PRINT("GLSLGL_1_1Renderer\n");
#if 0
    // Initialize the HLSL to GLSL translator
    Hlsl2Glsl_Initialize();

    // Construct a vertex shader parser for the translator to use
    /*ShHandle*/ parserv = Hlsl2Glsl_ConstructParser(EShLangVertex, 0);
    /*ShHandle*/ parserf = Hlsl2Glsl_ConstructParser(EShLangFragment, 0);
	TDebugOptions debugOptions//=EDebugOpNone;
					=EDebugOpIntermediate;

    // Construct a Translator to use for final code translation
    /*ShHandle*/ translator = Hlsl2Glsl_ConstructTranslator(debugOptions);
#endif

			mOk=true;
		}
		RendererGLSLGL_1_1::~RendererGLSLGL_1_1()
		{
			STX_TRY;
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
				glDeleteObjectARB((shaders[i]).vertexShader);
				checkGlError("");
				glDeleteObjectARB((shaders[i]).fragmentShader);
				checkGlError("");
				glDeleteObjectARB((shaders[i]).program);
				checkGlError("");
			}
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
			for (uint i = 0; i < textures.getCount(); i++)
			{

				removeTexture(i);
			}
			if (fbo) {glDeleteFramebuffersEXT(1, &fbo);
				checkGlError("");}
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
#if 0
	// ...shutdown
    Hlsl2Glsl_Destruct( parserv );
    Hlsl2Glsl_Destruct( parserf );
	Hlsl2Glsl_Destruct( translator );
    Hlsl2Glsl_Finalize( );
#endif
		STX_CATCH;
		}

#ifdef USE_HLSL_SHADERS
#ifdef __BOOST__
HLSL2GLSLBoostTranslator::HLSL2GLSLBoostTranslator():HLSL2GLSLTranslator()//,currentShader(-1)
{
}
HLSL2GLSLBoostTranslator::~HLSL2GLSLBoostTranslator()
{
}
void
parsevs(char const* filename,std::string& out
	  );
void
parsefs(char const* filename,std::string& out
	  );
Shader* HLSL2GLSLBoostTranslator::addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header, const char *extra, const char *fileName,
const char **attributeNames, const int nAttributes, const uint flags)

{
	ShaderGLSLGL3* shader=new ShaderGLSLGL3();
	std::string outvs;
	std::string outfs;

	parsevs(vsText0,outvs);
	parsefs(fsText0,outfs);

	shader->HLSLVS=std::string(vsText0);
	shader->HLSLFS=std::string(fsText0);

	shader->GLSLVS=std::string(outvs.c_str());
	shader->GLSLFS=std::string(outfs.c_str());
/*
    if(stx_strcmp(shader->GLSLVS,"#")==0)
		LOG_PRINT("\n%s\n",shader->HLSLVS);
	else
		LOG_PRINT("\n%s\n",shader->GLSLVS);

    if(stx_strcmp(shader->GLSLFS,"#")==0)
		LOG_PRINT("\n%s\n",shader->HLSLFS);
	else
		LOG_PRINT("\n%s\n",shader->GLSLFS);
*/
	return shader;
}
#endif
HLSL2GLSLTranslator::HLSL2GLSLTranslator():IShaderTranslator()//,currentShader(-1)

{
}
HLSL2GLSLTranslator::~HLSL2GLSLTranslator()
{
}

HLSL2GLSLHLSL2GLSLTranslator::HLSL2GLSLHLSL2GLSLTranslator():HLSL2GLSLTranslator()//,currentShader(-1)
{
}
HLSL2GLSLHLSL2GLSLTranslator::~HLSL2GLSLHLSL2GLSLTranslator()
{
}
bool HLSL2GLSLHLSL2GLSLTranslator::TranslateVertexShader(const std::string& hlsl, const std::string& main, std::string& glsl)
{

	//LOG_PRINT("\nHLSL:\n%s\n",hlsl.c_str());

	glsl="";

TestFile (true,
	hlsl,
	glsl,main);

	LOG_FNLN;
	LOG_PRINT("\nGLSL:\n%s\n",glsl.c_str());

	return true;
}
bool HLSL2GLSLHLSL2GLSLTranslator::TranslateFragmentShader(const std::string& hlsl, const std::string& main, std::string& glsl)
{

	//LOG_PRINT("\nHLSL:\n%s\n",hlsl.c_str());

	glsl="";
TestFile (false,
	hlsl,
	glsl,main);

	LOG_FNLN;
	LOG_PRINT("\nGLSL:\n%s\n",glsl.c_str());

	return true;
}


Shader* HLSL2GLSLHLSL2GLSLTranslator::addHLSLShader(Shader* currentShader, const char *vsText0, const char *gsText,
const char *fsText0, const char *vsName, const char *psName,
const int vsLine, const int gsLine, const int fsLine,
const char *header, const char *extra, const char *fileName,
const char **attributeNames, const int nAttributes, const uint flags)
{

	ShaderGLSLGL3* shader=new ShaderGLSLGL3();

shader->HLSLVS=std::string(vsText0);

shader->HLSLFS=std::string(fsText0);

	std::string glslvs=vsText0;
	std::string glslfs=fsText0;
      std::string rm="row_major";
      std::string r= "	       ";

      replaceAll(glslvs,rm,r);

      replaceAll(glslfs,rm,r);
      rm="matrix";
      r= "float4x4";

      replaceAll(glslvs,rm,r);

	replaceAll(glslfs,rm,r);

static bool init=true;

if(init)
{

	init=false;
	////
	#if defined(USE_HLSL_SHADERS)

	Hlsl2Glsl_Initialize ();

	#endif
////
}

			   if(!TranslateVertexShader(glslvs.c_str(), vsName, glslvs)) return 0;//DBG_HALT;

if(!TranslateFragmentShader(glslfs.c_str(), psName, glslfs)) return 0;//DBG_HALT;//

shader->GLSLVS=std::string(glslvs.c_str());

shader->GLSLFS=std::string(glslfs.c_str());

	return shader;
}


		/*static*/ IShaderTranslator * IShaderTranslator::Get(tShaderTranslator a )
{
	IShaderTranslator * pShaderTranslator = 0;
	switch(a)
	{
			case eHLSL2GLSLHLSL2GLSL:
		pShaderTranslator = new HLSL2GLSLHLSL2GLSLTranslator();
			break;
#ifdef __BOOST__
			case eHLSL2GLSLBoost:
		pShaderTranslator = new HLSL2GLSLBoostTranslator();
			break;F
#endif
			default:
		pShaderTranslator = new HLSL2GLSLHLSL2GLSLTranslator();
						break;
	};
	return pShaderTranslator;
}
#ifdef __BOOST__
#undef CONST
#undef VOID
		#if 1

/*=============================================================================
    Copyright (c) 2002-2004 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

		using namespace boost::spirit::classic;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// parsing helper function

//  Here's our comment rule
struct skip_vsgrammar : public grammar<skip_vsgrammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(skip_vsgrammar const& /*self*/)
        {
#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

            skip
                =   space_p
                |   "//" >> *(anychar_p - '\n') >> '\n'     // C++ comment
                |   "/*" >> *(anychar_p - "*/") >> "*/"     // C comment
                |   "#line" >> *(anychar_p - '\n') >> '\n'  // added for correctly
                                                            // handling preprocessed
                |   "#pragma" >> *(anychar_p - '\n') >> '\n'// files from Intel V5.0.1
                                                            // on W2K
                ;
#else
            skip
                =   space_p
                |   comment_p("//")                 // C++ comment
                |   comment_p("/*", "*/")           // C comment
                |   comment_p("#line")              // added for correctly
                                                    // handling preprocessed
                |   comment_p("#pragma")            // files from Intel V5.0.1
                                                    // on W2K
                ;
#endif
        }

        rule<ScannerT> skip;

        rule<ScannerT> const&
        start() const { return skip; }
    };
};
namespace boost { namespace spirit { namespace classic { namespace repository
{
    // vs_grammar, as its name suggests, is a very simple lexer that
    // recognises c++ lexical entities such as keywords, identifiers,
    // preprocessor directives, etc., and calls user supplied semantic actions
    // per recognized entity. vs_grammar works on the character level.
    // Be sure to wrap the grammar in a lexeme_d when parsing with a skip
    // parser. See cpp_to_html and quickdoc for applications using this
    // grammar.

    template <typename Actions>
    struct vs_grammar
        : grammar<vs_grammar<Actions> >
    {
        vs_grammar(Actions& actions)
            : actions(actions) {}

        template <typename ScannerT>
        struct definition
        {
#if 0
            definition(vs_grammar const& self)
            {
                Actions& actions = self.actions;

			program
                    =  *space_p >>
                       *(   preprocessor    [actions.tostring]
                        |   comment         [actions.tostring]
                        |   keyword         [actions.tostring]
                        |   identifier      [actions.identifier]
                        |   special         [actions.tostring]

                        |   string          [actions.tostring]
                        |   literal         [actions.tostring]
                        |   number          [actions.tostring]
                        |   anychar_p       [actions.unexpected]
                        )
                    ;

                preprocessor
                    =   '#' >> *space_p >> identifier
                    ;

                comment
                    =   +((comment_p("//") | comment_p("/*", "*/"))
                        >> *space_p)
                    ;

                keyword
                    =   keywords >> (eps_p - (alnum_p | '_')) >> *space_p
                    ;   // make sure we recognize whole words only
#if 0
static const char * const yytname[] = {   "$","error","$undefined.","ATTRIBUTE",
"CONST_QUAL","BOOL_TYPE","FLOAT_TYPE","INT_TYPE","STRING_TYPE","BREAK","CONTINUE",
"DO","ELSE","FOR","IF","DISCARD","RETURN","BVEC2","BVEC3","BVEC4","IVEC2","IVEC3",
"IVEC4","VEC2","VEC3","VEC4","MATRIX2","MATRIX3","MATRIX4","IN_QUAL","OUT_QUAL",
"INOUT_QUAL","UNIFORM","VARYING","STRUCT","VOID_TYPE","WHILE","SAMPLER1D","SAMPLER2D",
"SAMPLER3D","SAMPLERCUBE","SAMPLER1DSHADOW","SAMPLER2DSHADOW","SAMPLERGENERIC",
"VECTOR","MATRIX","REGISTER","TEXTURE","SAMPLERSTATE","IDENTIFIER","TYPE_NAME",
"FLOATCONSTANT","INTCONSTANT","BOOLCONSTANT","STRINGCONSTANT","FIELD_SELECTION",
"LEFT_OP","RIGHT_OP","INC_OP","DEC_OP","LE_OP","GE_OP","EQ_OP","NE_OP","AND_OP",
"OR_OP","XOR_OP","MUL_ASSIGN","DIV_ASSIGN","ADD_ASSIGN","MOD_ASSIGN","LEFT_ASSIGN",
"RIGHT_ASSIGN","AND_ASSIGN","XOR_ASSIGN","OR_ASSIGN","SUB_ASSIGN","LEFT_PAREN",
"RIGHT_PAREN","LEFT_BRACKET","RIGHT_BRACKET","LEFT_BRACE","RIGHT_BRACE","DOT",
"COMMA","COLON","EQUAL","SEMICOLON","BANG","DASH","TILDE","PLUS","STAR","SLASH",
"PERCENT","LEFT_ANGLE","RIGHT_ANGLE","VERTICAL_BAR","CARET","AMPERSAND","QUESTION",
"variable_identifier","primary_expression","postfix_expression","integer_expression",
"function_call","function_call_or_method","function_call_generic","function_call_header_no_parameters",
"function_call_header_with_parameters","function_call_header","function_identifier",
"unary_expression","unary_operator","multiplicative_expression","additive_expression",
"shift_expression","relational_expression","equality_expression","and_expression",
"exclusive_or_expression","inclusive_or_expression","logical_and_expression",
"logical_xor_expression","logical_or_expression","conditional_expression","assignment_expression",
"assignment_operator","expression","constant_expression","declaration","function_prototype",
"function_declarator","function_header_with_parameters","function_header","parameter_declarator",
"parameter_declaration","parameter_qualifier","parameter_type_specifier","init_declarator_list",
"single_declaration","fully_specified_type","type_qualifier","type_specifier",
"type_specifier_nonarray","struct_specifier","struct_declaration_list","struct_declaration",
"struct_declarator_list","struct_declarator","initializer","declaration_statement",
"statement","simple_statement","compound_statement","@1","@2","statement_no_new_scope",
"compound_statement_no_new_scope","statement_list","expression_statement","selection_statement",
"selection_rest_statement","condition","iteration_statement","@3","@4","@5",
"for_init_statement","conditionopt","for_rest_statement","jump_statement","translation_unit",
"external_declaration","function_definition","@6","initialization_list","initializer_list",
"annotation","annotation_list","annotation_item","ann_type","ann_literal","ann_numerical_constant",
"ann_literal_constructor","ann_value_list","ann_literal_init_list","register_specifier",
"semantic","type_info","sampler_initializer","sampler_init_list","sampler_init_item",
""
#endif
                keywords
                    = "bool", "break",

					//"bvec2","bvec3","bvec4",

					"continue",
					"discard", "do", "else" "float",
					"float2","float3","float4","float4x4",
					"for", "if", "in", "inout",
					"int",

					//"ivec2","ivec3","ivec4",
					"matrix",
					//"matrix2","matrix3",
					//"matrix4",

					"out", "register", "return", "sampler1d",
					"sampler2d", "sampler3d","samplercube","sampler1dshadow",
					"sampler2dshadow","samplergeneric", "samplerstate", "string",
					"struct", "texture", "uniform",
					//"varying",
					"vector",

					//"vec2","vec3","vec4",

					"void", "while"
                    ;

                special
                    =   +chset_p("~!%^&*()+={[}]:;,<.>?/|\\-") >> *space_p
                    ;

                string
                    =   !as_lower_d['l'] >> confix_p('"', *c_escape_ch_p, '"')
                        >> *space_p
                    ;

                literal
                    =   !as_lower_d['l'] >> confix_p('\'', *c_escape_ch_p, '\'')
                        >> *space_p
                    ;

                number
                    =   (   real_p
                        |   as_lower_d["0x"] >> hex_p
                        |   '0' >> oct_p
                        )
                        >>  *as_lower_d[chset_p("ldfu")]
                        >>  *space_p
                    ;

                identifier
                    =   ((alpha_p | '_') >> *(alnum_p | '_'))
                        >> *space_p
                    ;
            }

            rule<ScannerT>
                program, preprocessor, comment, special, string, literal,
                number, identifier, keyword;

            symbols<>
                keywords;

            rule<ScannerT> const&
            start() const
            {
                return program;
            }
#else
        definition(vs_grammar const& self) :
            ELLIPSIS("..."), RIGHT_ASSIGN(">>="), LEFT_ASSIGN("<<="),
            ADD_ASSIGN("+="), SUB_ASSIGN("-="), MUL_ASSIGN("*="),
            DIV_ASSIGN("/="), MOD_ASSIGN("%="), AND_ASSIGN("&="),
            XOR_ASSIGN("^="), OR_ASSIGN("|="), RIGHT_OP(">>"), LEFT_OP("<<"),
            INC_OP("++"), DEC_OP("--"), PTR_OP("->"), AND_OP("&&"),
            OR_OP("||"), LE_OP("<="), GE_OP(">="), EQ_OP("=="), NE_OP("!="),
            SEMICOLON(';'),
            COMMA(','), COLON(':'), ASSIGN('='), LEFT_PAREN('('),
            RIGHT_PAREN(')'), DOT('.'), ADDROF('&'), BANG('!'), TILDE('~'),
            MINUS('-'), PLUS('+'), STAR('*'), SLASH(stx_getOsSeparator()), PERCENT('%'),
            LT_OP('<'), GT_OP('>'), XOR('^'), OR('|'), QUEST('?')
        {
			Actions& actions = self.actions;
        // C keywords
            keywords =
                     "bool", "break",

					//"bvec2","bvec3","bvec4",

					"continue",
					"discard", "do", "else" "float",
					"float2","float3","float4","float4x4",
					"for", "if", "in", "inout",
					"int",

					//"ivec2","ivec3","ivec4",
					"matrix",
					//"matrix2","matrix3",
					//"matrix4",

					"out", "register", "return", "sampler1d",
					"sampler2d", "sampler3d","samplercube","sampler1dshadow",
					"sampler2dshadow","samplergeneric", "samplerstate", "string",
					"struct", "texture", "uniform",
					//"varying",
					"vector",

					//"vec2","vec3","vec4",

					"void", "while"
					;
        // C operators
            LEFT_BRACE = chlit<>('{') | strlit<>("<%");
            RIGHT_BRACE = chlit<>('}') | strlit<>("%>");
            LEFT_BRACKET = chlit<>('[') | strlit<>("<:");
            RIGHT_BRACKET = chlit<>(']') | strlit<>(":>");

        // Tokens

            BREAK =      strlit<>("break");
            CASE =       strlit<>("case");

            CONST =      strlit<>("const");
            CONTINUE =   strlit<>("continue");
            DEFAULT =    strlit<>("default");
            DO =         strlit<>("do");


            ELSE =       strlit<>("else");

            FOR =        strlit<>("for");
            FLOAT =      strlit<>("float");
            GOTO =       strlit<>("goto");
            IF =         strlit<>("if");
            INT =        strlit<>("int");


            RETURN =     strlit<>("return");

            SIGNED =     strlit<>("signed");


            STRUCT =     strlit<>("struct");
            SWITCH =     strlit<>("switch");

            UNION =      strlit<>("union");
            UNSIGNED =   strlit<>("unsigned");
            VOID =       strlit<>("void");

            WHILE =      strlit<>("while");

        // C identifiers
            IDENTIFIER =
                lexeme_d[
                    ((alpha_p | '_' | '$') >> *(alnum_p | '_' | '$'))
                    - (keywords >> anychar_p - (alnum_p | '_' | '$'))
                ]
                ;

        // string literals
            STRING_LITERAL_PART =
                lexeme_d[
                    !chlit<>('L') >> chlit<>('\"') >>
                    *( strlit<>("\\\"") | anychar_p - chlit<>('\"') ) >>
                    chlit<>('\"')
                ]
                ;

            STRING_LITERAL = +STRING_LITERAL_PART;

        // integer constants
            INT_CONSTANT_HEX
                = lexeme_d[
                    chlit<>('0')
                    >> as_lower_d[chlit<>('x')]
                    >> +xdigit_p
                    >> !as_lower_d[chlit<>('l') | chlit<>('u')]
                ]
                ;

            INT_CONSTANT_OCT
                = lexeme_d[
                    chlit<>('0')
                    >> +range<>('0', '7')
                    >> !as_lower_d[chlit<>('l') | chlit<>('u')]
                ]
                ;

            INT_CONSTANT_DEC
                = lexeme_d[
                    +digit_p
                    >> !as_lower_d[chlit<>('l') | chlit<>('u')]
                ]
                ;

            INT_CONSTANT_CHAR
                = lexeme_d[
                    !chlit<>('L') >> chlit<>('\'') >>
                    longest_d[
                            anychar_p
                        |   (   chlit<>(stx_getOsSeparator())
                                >> chlit<>('0')
                                >> repeat_p(0, 2)[range<>('0', '7')]
                            )
                        |   (chlit<>(stx_getOsSeparator()) >> anychar_p)
                    ] >>
                    chlit<>('\'')
                ]
                ;

            INT_CONSTANT =
                    INT_CONSTANT_HEX
                |   INT_CONSTANT_OCT
                |   INT_CONSTANT_DEC
                |   INT_CONSTANT_CHAR
                ;

        // float constants
            FLOAT_CONSTANT_1    // 12345[eE][+-]123[lLfF]?
                = lexeme_d[
                    +digit_p
                    >> (chlit<>('e') | chlit<>('E'))
                    >> !(chlit<>('+') | chlit<>('-'))
                    >> +digit_p
                    >> !as_lower_d[chlit<>('l') | chlit<>('f')]
                ]
                ;

            FLOAT_CONSTANT_2    // .123([[eE][+-]123)?[lLfF]?
                = lexeme_d[
                    *digit_p
                    >> chlit<>('.')
                    >> +digit_p
                    >> !(   (chlit<>('e') | chlit<>('E'))
                            >> !(chlit<>('+') | chlit<>('-'))
                            >> +digit_p
                        )
                    >> !as_lower_d[chlit<>('l') | chlit<>('f')]
                ]
                ;

            FLOAT_CONSTANT_3    // 12345.([[eE][+-]123)?[lLfF]?
                = lexeme_d[
                    +digit_p
                    >> chlit<>('.')
                    >> *digit_p
                    >> !(   (chlit<>('e') | chlit<>('E'))
                            >> !(chlit<>('+') | chlit<>('-'))
                            >> +digit_p
                        )
                    >> !as_lower_d[chlit<>('l') | chlit<>('f')]
                ]
                ;

            FLOAT_CONSTANT =
                    FLOAT_CONSTANT_1
                |   FLOAT_CONSTANT_2
                |   FLOAT_CONSTANT_3
                ;

            CONSTANT = longest_d[FLOAT_CONSTANT | INT_CONSTANT];

        // Rules
#if 0
			/*
1>c:\boost-trunk\boost\spirit\home\classic\core\scanner\scanner.hpp(132) :
error C2780: '
void `anonymous-namespace'::processtostring::operator ()(IteratorT,IteratorT) const'
: expects 2 arguments - 1 provided
			*/
            primary_expression
                = IDENTIFIER [actions.identifier]
                | CONSTANT    [actions.tostring]
                | STRING_LITERAL    [actions.tostring]
                | LEFT_PAREN     [actions.tostring]>> expression     [actions.tostring]>> RIGHT_PAREN    [actions.tostring]
                ;
#else
            primary_expression
                = IDENTIFIER
                | CONSTANT
                | STRING_LITERAL
                | LEFT_PAREN     >> expression     >> RIGHT_PAREN
                ;
#endif
/*
			program
                    =  *space_p >>
                       *(   preprocessor    [actions.tostring]
                        |   comment         [actions.tostring]
                        |   keyword         [actions.tostring]
                        |   identifier      [actions.identifier]
                        |   special         [actions.tostring]
                        |   string          [actions.tostring]
                        |   literal         [actions.tostring]
                        |   number          [actions.tostring]
                        |   anychar_p       [actions.unexpected]
                        )
                    ;
*/
        // left recursion --> right recursion
        //    postfix_expression
        //        = primary_expression
        //        | postfix_expression >>
        //            (
        //                LEFT_BRACKET >> expression >> RIGHT_BRACKET
        //            |   LEFT_PAREN >> !argument_expression_list >> RIGHT_PAREN
        //            |   DOT >> IDENTIFIER
        //            |   PTR_OP >> IDENTIFIER
        //            |   INC_OP
        //            |   DEC_OP
        //            )
        //        ;
            postfix_expression
                = primary_expression >> postfix_expression_helper
                ;

            postfix_expression_helper
                =   (
                        LEFT_BRACKET >> expression >> RIGHT_BRACKET
                    |   LEFT_PAREN >> !argument_expression_list >> RIGHT_PAREN
                    |   DOT >> IDENTIFIER
                    |   PTR_OP >> IDENTIFIER
                    |   INC_OP
                    |   DEC_OP
                    ) >>
                    postfix_expression_helper
                | epsilon_p
                ;

            argument_expression_list
                = assignment_expression >> *(COMMA >> assignment_expression)
                ;

            unary_expression
                = postfix_expression
                | INC_OP >> unary_expression
                | DEC_OP >> unary_expression
                | unary_operator >> cast_expression >>
                    (
                        unary_expression
                    |   LEFT_PAREN >> type_name >> RIGHT_PAREN
                    )
                ;

            unary_operator

                = ADDROF
                | STAR
                | PLUS
                | MINUS
                | TILDE
                | BANG
                ;

            cast_expression
                = LEFT_PAREN >> type_name >> RIGHT_PAREN >> cast_expression
                | unary_expression
                ;

        // left recursion --> right recursion
        //    multiplicative_expression
        //        = cast_expression
        //        | multiplicative_expression >>
        //        (
        //                STAR >> cast_expression
        //            |   SLASH >> cast_expression
        //            |   PERCENT >> cast_expression
        //        )
        //        ;
            multiplicative_expression
                = cast_expression >> multiplicative_expression_helper
                ;

            multiplicative_expression_helper

                =   (
                        STAR >> cast_expression
                    |   SLASH >> cast_expression
                    |   PERCENT >> cast_expression
                    ) >>
                    multiplicative_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    additive_expression
        //        = multiplicative_expression
        //        | additive_expression >>
        //        (
        //                PLUS >> multiplicative_expression
        //            |   MINUS >> multiplicative_expression
        //        )
        //        ;
            additive_expression
                = multiplicative_expression >> additive_expression_helper
                ;

            additive_expression_helper
                =   (
                        PLUS >> multiplicative_expression
                    |   MINUS >> multiplicative_expression
                    ) >>
                    additive_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    shift_expression
        //        = additive_expression
        //        | shift_expression >>
        //        (
        //                LEFT_OP >> additive_expression
        //            |   RIGHT_OP >> additive_expression
        //        )
        //        ;
            shift_expression
                = additive_expression >> shift_expression_helper
                ;

            shift_expression_helper
                =   (
                        LEFT_OP >> additive_expression
                    |   RIGHT_OP >> additive_expression
                    ) >>
                    shift_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    relational_expression
        //        = shift_expression
        //        | relational_expression >>
        //        (
        //                LT_OP >> shift_expression
        //            |   GT_OP >> shift_expression
        //            |   LE_OP >> shift_expression
        //            |   GE_OP >> shift_expression
        //        )
        //        ;
            relational_expression
                = shift_expression >> relational_expression_helper
                ;

            relational_expression_helper
                =   (
                        LT_OP >> shift_expression
                    |   GT_OP >> shift_expression
                    |   LE_OP >> shift_expression
                    |   GE_OP >> shift_expression
                    ) >>
                    relational_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    equality_expression
        //        = relational_expression
        //        | equality_expression >>
        //        (
        //                EQ_OP >> relational_expression
        //            |   NE_OP >> relational_expression
        //        )
        //        ;
            equality_expression
                = relational_expression >> equality_expression_helper
                ;

            equality_expression_helper
                =   (
                        EQ_OP >> relational_expression
                    |   NE_OP >> relational_expression
                    ) >>
                    equality_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    and_expression
        //        = equality_expression
        //        | and_expression >> ADDROF >> equality_expression
        //        ;
            and_expression
                = equality_expression >> and_expression_helper
                ;

            and_expression_helper
                = ADDROF >> equality_expression >> and_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    exclusive_or_expression
        //        = and_expression
        //        | exclusive_or_expression >> XOR >> and_expression
        //        ;
            exclusive_or_expression
                = and_expression >> exclusive_or_expression_helper
                ;

            exclusive_or_expression_helper
                = XOR >> and_expression >> exclusive_or_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    inclusive_or_expression
        //        = exclusive_or_expression
        //        | inclusive_or_expression >> OR >> exclusive_or_expression
        //        ;
            inclusive_or_expression
                = exclusive_or_expression >> inclusive_or_expression_helper
                ;

            inclusive_or_expression_helper
                = OR >> exclusive_or_expression >> inclusive_or_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    logical_and_expression
        //        = inclusive_or_expression
        //        | logical_and_expression >> AND_OP >> inclusive_or_expression
        //        ;
            logical_and_expression
                = inclusive_or_expression >> logical_and_expression_helper
                ;

            logical_and_expression_helper
                = AND_OP >> inclusive_or_expression >> logical_and_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    logical_or_expression
        //        = logical_and_expression
        //        | logical_or_expression >> OR_OP >> logical_and_expression
        //        ;
            logical_or_expression
                = logical_and_expression >> logical_or_expression_helper
                ;

            logical_or_expression_helper
                = OR_OP >> logical_and_expression >> logical_or_expression_helper
                | epsilon_p
                ;

        // left recursion --> right recursion
        //    conditional_expression
        //        = logical_or_expression
        //        | logical_or_expression >> QUEST >> expression >> COLON >>
        //        conditional_expression
        //        ;
            conditional_expression
                = logical_or_expression >> conditional_expression_helper
                ;

            conditional_expression_helper
                = QUEST >> expression >> COLON
                    >> conditional_expression >> conditional_expression_helper
                | epsilon_p
                ;

            assignment_expression
                = unary_expression >> assignment_operator >> assignment_expression
                | conditional_expression
                ;

            assignment_operator
                = ASSIGN
                | MUL_ASSIGN
                | DIV_ASSIGN
                | MOD_ASSIGN
                | ADD_ASSIGN
                | SUB_ASSIGN
                | LEFT_ASSIGN
                | RIGHT_ASSIGN
                | AND_ASSIGN
                | XOR_ASSIGN
                | OR_ASSIGN
                ;

        // left recursion --> right recursion
        //    expression
        //        = assignment_expression
        //        | expression >> COMMA >> assignment_expression
        //        ;
            expression
                = assignment_expression >> expression_helper
                ;

            expression_helper
                = COMMA >> assignment_expression >> expression_helper
                | epsilon_p
                ;

            constant_expression
                = conditional_expression
                ;

            declaration
                = declaration_specifiers >> !init_declarator_list >> SEMICOLON
                ;

            declaration_specifiers
                =   (
                        type_specifier
                    |   type_qualifier
                    ) >>
                    !declaration_specifiers
                ;

            init_declarator_list
                = init_declarator >> *(COMMA >> init_declarator)
                ;

            init_declarator
                = declarator >> !(ASSIGN >> initializer)
                ;


            type_specifier
                = INT
                | FLOAT
                | SIGNED
                | UNSIGNED
                | struct_or_union_specifier
        //        | TYPE_NAME
                ;

            struct_or_union_specifier
                = struct_or_union >>
                    (
                        IDENTIFIER >> !(LEFT_BRACE >> struct_declaration_list
                            >> RIGHT_BRACE)

                    |   LEFT_BRACE >> struct_declaration_list >> RIGHT_BRACE
                    )
                ;

            struct_or_union
                = STRUCT
                ;

            struct_declaration_list
                = +struct_declaration
                ;

            struct_declaration
                = specifier_qualifier_list >> struct_declarator_list >> SEMICOLON
                ;

            specifier_qualifier_list
                =   (
                        type_specifier
                    |   type_qualifier
                    ) >>
                    !specifier_qualifier_list
                ;

            struct_declarator_list
                = struct_declarator >> *(COMMA >> struct_declarator)
                ;

            struct_declarator
                = declarator || (COLON >> constant_expression)
                ;



            enumerator_list
                = enumerator >> *(COMMA >> enumerator)
                ;

            enumerator
                = IDENTIFIER >> !(ASSIGN >> constant_expression)
                ;

            type_qualifier
                = CONST

                ;

            declarator
                = !pointer >> direct_declarator
                ;

        // left recursion --> right recursion
        //    direct_declarator
        //        = IDENTIFIER
        //        | LEFT_PAREN >> declarator >> RIGHT_PAREN
        //        | direct_declarator >>
        //            (
        //                LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
        //            |   LEFT_PAREN >>
        //                (
        //                    parameter_type_list >> RIGHT_PAREN
        //                |   identifier_list >> RIGHT_PAREN
        //                |   RIGHT_PAREN
        //                )
        //            )
        //        ;
            direct_declarator
                =   (
                        IDENTIFIER
                    |   LEFT_PAREN >> declarator >> RIGHT_PAREN
                    ) >>
                    direct_declarator_helper
                ;

            direct_declarator_helper
                =   (
                        LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
                    |   LEFT_PAREN >>
                    !   (
                            parameter_type_list
                        |   identifier_list
                        ) >> RIGHT_PAREN
                    ) >> direct_declarator_helper
                | epsilon_p
                ;

            pointer
                = STAR >> !(type_qualifier_list || pointer)
                ;

            type_qualifier_list
                = +type_qualifier
                ;

            parameter_type_list
                = parameter_list >> !(COMMA >> ELLIPSIS)
                ;

            parameter_list
                = parameter_declaration >> *(COMMA >> parameter_declaration)
                ;

            parameter_declaration
                = declaration_specifiers >>
                   !(
                        declarator
                    |   abstract_declarator
                    )
                ;

            identifier_list
                = IDENTIFIER >> *(COMMA >> IDENTIFIER)
                ;

            type_name
                = specifier_qualifier_list >> !abstract_declarator
                ;

            abstract_declarator
                = pointer || direct_abstract_declarator
                ;

        // left recursion --> right recursion
        //    direct_abstract_declarator
        //        = LEFT_PAREN >>
        //            (
        //                abstract_declarator >> RIGHT_PAREN
        //            |   !parameter_type_list >> RIGHT_PAREN
        //            )
        //        | LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
        //        | direct_abstract_declarator >>
        //            (
        //                LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
        //            |   LEFT_PAREN >> !parameter_type_list >> RIGHT_PAREN
        //            )
        //        ;
            direct_abstract_declarator
                =   (
                        LEFT_PAREN >>
                        (
                            abstract_declarator >> RIGHT_PAREN
                        |   !parameter_type_list >> RIGHT_PAREN
                        )
                    |    LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
                    ) >>
                    direct_abstract_declarator_helper
                ;

            direct_abstract_declarator_helper
                =   (
                        LEFT_BRACKET >> !constant_expression >> RIGHT_BRACKET
                    |   LEFT_PAREN >> !parameter_type_list >> RIGHT_PAREN
                    ) >>
                    direct_abstract_declarator_helper
                | epsilon_p
                ;

            initializer
                = assignment_expression
                | LEFT_BRACE >> initializer_list >> !COMMA >> RIGHT_BRACE
                ;

            initializer_list
                = initializer >> *(COMMA >> initializer)
                ;

            statement
                = labeled_statement
                | compound_statement
                | expression_statement
                | selection_statement
                | iteration_statement
                | jump_statement
                ;

            labeled_statement
                = IDENTIFIER >> COLON >> statement
                | CASE >> constant_expression >> COLON >> statement
                | DEFAULT >> COLON >> statement
                ;

            compound_statement
                = LEFT_BRACE >> !(declaration_list || statement_list) >> RIGHT_BRACE
                ;

            declaration_list
                = +declaration
                ;

            statement_list
                = +statement
                ;

            expression_statement
                = !expression >> SEMICOLON
                ;

            selection_statement
                = IF >> LEFT_PAREN >> expression >> RIGHT_PAREN >> statement
                    >> !(ELSE >> statement)
                | SWITCH >> LEFT_PAREN >> expression >> RIGHT_PAREN >> statement
                ;

            iteration_statement
                = WHILE >> LEFT_PAREN >> expression >> RIGHT_PAREN >>  statement
                | DO >> statement >> WHILE >> LEFT_PAREN >> expression
                    >> RIGHT_PAREN >> SEMICOLON
                | FOR >> LEFT_PAREN >> expression_statement >> expression_statement
                    >> !expression >> RIGHT_PAREN >> statement
                ;

            jump_statement
                = GOTO >> IDENTIFIER >> SEMICOLON
                | CONTINUE >> SEMICOLON
                | BREAK >> SEMICOLON
                | RETURN >> !expression >> SEMICOLON
                ;

            function_definition
                =  !declaration_specifiers
                >>  declarator
                >> !declaration_list
                >>  compound_statement
                ;

            external_declaration
                = function_definition
                | declaration
                ;

        // parser start symbol
            translation_unit
                = *external_declaration
                ;
        }

    // keywords
        symbols<> keywords;

    // operators
        strlit<>
                ELLIPSIS, RIGHT_ASSIGN, LEFT_ASSIGN, ADD_ASSIGN, SUB_ASSIGN,
                MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, AND_ASSIGN, XOR_ASSIGN,
                OR_ASSIGN, RIGHT_OP, LEFT_OP, INC_OP, DEC_OP, PTR_OP, AND_OP,
                OR_OP, LE_OP, GE_OP, EQ_OP, NE_OP;
        chlit<>
                SEMICOLON, COMMA, COLON, ASSIGN, LEFT_PAREN, RIGHT_PAREN,
                DOT, ADDROF, BANG, TILDE, MINUS, PLUS, STAR, SLASH, PERCENT,
                LT_OP, GT_OP, XOR, OR, QUEST;

        rule<ScannerT>
                LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET;

    // terminals
        rule<ScannerT>
                 BREAK, CASE,  CONST, CONTINUE, DEFAULT, DO,
                ELSE,  FOR, FLOAT, GOTO, IF, INT,
                RETURN,  SIGNED,  STRUCT, SWITCH,
                UNION, UNSIGNED, VOID, WHILE, IDENTIFIER,
                STRING_LITERAL_PART, STRING_LITERAL, INT_CONSTANT_HEX, INT_CONSTANT,
                INT_CONSTANT_OCT, INT_CONSTANT_DEC, INT_CONSTANT_CHAR,
                FLOAT_CONSTANT,FLOAT_CONSTANT_1, FLOAT_CONSTANT_2, FLOAT_CONSTANT_3,
                CONSTANT;

    // nonterminals
        rule<ScannerT>
                primary_expression, postfix_expression, postfix_expression_helper,
                argument_expression_list, unary_expression, unary_operator,
                cast_expression,
                multiplicative_expression, multiplicative_expression_helper,
                additive_expression, additive_expression_helper,
                shift_expression, shift_expression_helper,
                relational_expression, relational_expression_helper,
                equality_expression, equality_expression_helper,
                and_expression, and_expression_helper,
                exclusive_or_expression, exclusive_or_expression_helper,
                inclusive_or_expression, inclusive_or_expression_helper,
                logical_and_expression, logical_and_expression_helper,
                logical_or_expression, logical_or_expression_helper,
                conditional_expression, conditional_expression_helper,
                assignment_expression, assignment_operator,
                expression, expression_helper, constant_expression, declaration,
                declaration_specifiers, init_declarator_list, init_declarator,
                 type_specifier, struct_or_union_specifier,
                struct_or_union, struct_declaration_list, struct_declaration,
                specifier_qualifier_list, struct_declarator_list,
                struct_declarator, enum_specifier, enumerator_list, enumerator,
                type_qualifier, declarator,
                direct_declarator, direct_declarator_helper, pointer,
                type_qualifier_list, parameter_type_list, parameter_list,
                parameter_declaration, identifier_list, type_name,
                abstract_declarator,
                direct_abstract_declarator, direct_abstract_declarator_helper,
                initializer, initializer_list, statement, labeled_statement,
                compound_statement, declaration_list, statement_list,
                expression_statement, selection_statement, iteration_statement,
                jump_statement, translation_unit, external_declaration,
                function_definition;

        rule<ScannerT> const&
        start() const { return translation_unit; }
#endif
        };
        Actions& actions;
    };
}}}} // namespace boost::spirit::repository

using namespace std;
using namespace boost::spirit::classic;
using namespace boost::spirit::classic::repository;

namespace
{
    template <typename CharT>
    void print_char(CharT ch, std::string& out)
    {
        switch (ch)
        {
			/*
            case '<': out << "&lt;";    break;
            case '>': out << "&gt;";    break;
            case '&': out << "&amp;";   break;
            case '"': out << "&quot;";  break;
				*/
			char buf[2];
			buf[0]=ch;
			buf[1]='\0';
            default:  out.append( buf);        break;
        }
    }

    struct process
    {
        process(char const* name, std::string& out)
            : name(name), out(out) {}

        template <typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
            //out << "<span class=" << name << ">";
			IteratorT first0=first;
#if 0
			while (first0 != last)
                print_char(*first0++, out);
#else
			std::string buf;
			//IteratorT first0=first;
            while (first0 != last)
			{
			char buf2[2];
			buf2[0]=*first0++;
			buf2[1]='\0';
                buf.append(buf2);
			}
            if(stx_strcmp(buf.c_str(),"float2")==0)
				buf="vec2";
            if(stx_strcmp(buf.c_str(),"float3")==0)
				buf="vec3";
            else if(stx_strcmp(buf.c_str(),"float4")==0)
				buf="vec4";
            else if(stx_strcmp(buf.c_str(),"float4x4")==0)
				buf="mat4";

            else if(stx_strcmp(buf.c_str(),"mod")==0)
				buf="fmod";
            else if(stx_strcmp(buf.c_str(),"inversesqrt")==0)
				buf="rsqrt";
            else if(stx_strcmp(buf.c_str(),"fract")==0)
				buf="frac";
            else if(stx_strcmp(buf.c_str(),"mix")==0)
				buf="lerp";
            else if(stx_strcmp(buf.c_str(),"*")==0)
				buf="mul";
            else if(stx_strcmp(buf.c_str(),"dFdx")==0)
				buf="ddx";
            else if(stx_strcmp(buf.c_str(),"dFdy")==0)
				buf="ddy";

#endif
			//out << "</span>";
			out.append(buf);
        }

        char const* name;
        std::string& out;
    };

    struct processtostring
    {
        processtostring(char const* name, std::string& out)
            : name(name), out(out) {}

        template <typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
            //out << "<span class=" << name << ">";
			IteratorT first0=first;
#if 0
			while (first0 != last)
                print_char(*first0++, out);
#else
			std::string buf;
			//IteratorT first0=first;
            while (first0 != last)
			{
			char buf2[2];
			buf2[0]=*first0++;
			buf2[1]='\0';
                buf.append(buf2);
			}
#endif
			//out << "</span>";
			out.append(buf);
        }

        char const* name;
        std::string& out;
    };


	struct unexpected_char
    {
        unexpected_char(std::string& out)
        : out(out) {}

        template <typename CharT>
        void operator()(CharT) const
        {
            out.append("#"); // print out an unexpected_char character
        }

        std::string& out;
    };

    struct cpp_to_html_actions
    {
        cpp_to_html_actions(std::string& out)
            : preprocessor("preprocessor", out)
            , comment("comment", out)
            , keyword("keyword", out)
            , identifier("identifier", out)
            , special("special", out)
            , string("string", out)
            , literal("literal", out)
            , number("number", out)
            , unexpected(out)
			, tostring("tostring", out)
        {}

		processtostring tostring;
        process
            preprocessor, comment, keyword, identifier,
            special, string, literal, number;
        unexpected_char unexpected;
    };
}

using namespace boost::spirit::classic::repository;
void
parsefs(char const* filename,std::string& out
	  )
{
#if 0
    ifstream in(filename);

    if (!in)
    {
        cerr << "Could not open input file: " << filename << endl;
        return;
    }

    in.unsetf(ios::skipws); //  Turn of white space skipping on the stream

    vector<char> vec;
    std::copy(
        istream_iterator<char>(in),
        istream_iterator<char>(),
        std::back_inserter(vec));
#else
	vector<char> vec;
    vec.resize(stx_strlen(filename)+1);
	unsigned int i=0;
    for(i=0;i<stx_strlen(filename);i++)
		vec[i]=*(filename+i);
	vec[i]='\0';
#endif
    vector<char>::const_iterator start = vec.begin();
    vector<char>::const_iterator end = vec.end();

    skip_vsgrammar skip;
#if 0
    parse_info<vector<char>::const_iterator> result =
        parse(start, end, g, skip);
#else
    cpp_to_html_actions actions(out);
	::vs_grammar<cpp_to_html_actions> p(actions);
    parse_info<vector<char>::const_iterator> result =
        parse(start, end, p
		, skip);
#endif
    if (result.full)
        LOG_PRINT("\n%s\n", /*filename ,*/ " Parses OK");
    else {
        LOG_PRINT("\n%s\n", /*filename ,*/ " Fails Parsing");

		LOG_PRINT("\n\n\n%s\n\n\n",string(start,result.stop).c_str());
		LOG_PRINT("\n\n\n%s\n\n\n",string(result.stop, end).c_str());
/*
        for (int i = 0; i < 50; i++)
        {
            if (result.stop == end)
                break;
            LOG_PRINT("%c"); *result.stop++;
        }*/
        //LOG_PRINT("\n\n\n");
    }
}
void
parsevs(char const* filename,std::string& out
	  )
{
#if 0
    ifstream in(filename);

    if (!in)
    {
        cerr << "Could not open input file: " << filename << endl;
        return;
    }

    in.unsetf(ios::skipws); //  Turn of white space skipping on the stream

    vector<char> vec;
    std::copy(
        istream_iterator<char>(in),
        istream_iterator<char>(),
        std::back_inserter(vec));
#else
	vector<char> vec;
    vec.resize(stx_strlen(filename)+1);
	unsigned int i=0;
    for(i=0;i<stx_strlen(filename);i++)
		vec[i]=*(filename+i);
	vec[i]='\0';
#endif
    vector<char>::const_iterator start = vec.begin();
    vector<char>::const_iterator end = vec.end();

    skip_vsgrammar skip;
#if 0
    parse_info<vector<char>::const_iterator> result =
        parse(start, end, g, skip);
#else
    cpp_to_html_actions actions(out);
	::vs_grammar<cpp_to_html_actions> p(actions);
    parse_info<vector<char>::const_iterator> result =
        parse(start, end, p
		, skip);
#endif
    if (result.full)
        LOG_PRINT("\n%s\n", /*filename ,*/ " Parses OK");
    else {
        LOG_PRINT("\n%s\n", /*filename ,*/ " Fails Parsing");

		LOG_PRINT("\n\n\n%s\n\n\n",string(start,result.stop).c_str());
		LOG_PRINT("\n\n\n%s\n\n\n",string(result.stop, end).c_str());
/*
        for (int i = 0; i < 50; i++)
        {
            if (result.stop == end)
                break;
            LOG_PRINT("%c"); *result.stop++;
        }*/
        //LOG_PRINT("\n\n\n");
    }
}
#endif
#endif
#endif
#endif

