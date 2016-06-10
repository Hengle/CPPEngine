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

#include <Framework3/RendererGL_1_1.h>
#ifdef __RendererGL_1_1_H__
		BYTE* IndexBufferGL::Lock()
	{
		BYTE* m_pLockedData=0;
            // Lock
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIB);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB,
				nIndices*indexSize,//???
				0, GL_DYNAMIC_DRAW_ARB);
				checkGlError("");
			m_pLockedData = (BYTE *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
				checkGlError("");
		return m_pLockedData;
	}
	void IndexBufferGL::Unlock()
	{
 // Unlock
        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
				checkGlError("");
	}
	BYTE* VertexBufferGL::Lock()
	{
		BYTE* m_pLockedData=0;
            // Lock
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboVB);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, 0, GL_DYNAMIC_DRAW_ARB);
				checkGlError("");
			m_pLockedData = (BYTE *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY);
				checkGlError("");
		return m_pLockedData;
	}
	void VertexBufferGL::Unlock()
	{
// Unlock
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
				checkGlError("");
    }
	BYTE* TextureGL::Lock(int m, int& pitch)//int& slicePitch;
	{


//		int slicePitch;
		Image3 img;
		img.create(format,width, height,depth,mipMapLevels);

#if 0
				uint nMipMaps = img->getMipMapCount();
				uint nSlices = img->isCube()? 6 : 1;
				uint arraySize = img->getArraySize();
				//static D3D10_SUBRESOURCE_DATA texData[1024];
				//D3D10_SUBRESOURCE_DATA *dest = texData;
				//for (uint n = 0; n < arraySize; n++)
				{
				//	for (uint k = 0; k < nSlices; k++)
					{
						for (uint i =
m//0
; i <
m+1//nMipMaps
; i++)
								  {
							if (isCompressedFormat(format))
							{
								pitch = ((img->getWidth(i) + 3) >> 2) * getBytesPerBlock(format);
								slicePitch = pitch * ((img->getHeight(i) + 3) >> 2);
							}
							else
							{
								pitch = img->getWidth(i) * getBytesPerPixel(format);
								slicePitch = pitch * img->getHeight(i);
							}
/*
							dest->pSysMem = img->getPixels(i, n) + k * slicePitch;
							dest->SysMemPitch = pitch;
							dest->SysMemSlicePitch = slicePitch;
							dest++;
*/
						}
					}
				}
#endif

BYTE* m_pLockedData= 0;

#if !defined( _MSC_VER) && !defined(__APPLE__)
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_EXT, glTexID);
				checkGlError("");
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_EXT,
				img.getMipMappedSize(0, mipMapLevels),//???,
				0, GL_DYNAMIC_DRAW_ARB);
				checkGlError("");
		m_pLockedData= ((BYTE *)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_EXT, GL_WRITE_ONLY))+(img.getPixels(m)-img.getPixels(0));//img->getDataOffset(m,format);
				checkGlError("");
#endif
		pitch=img.getPitch(m);
		return m_pLockedData;
	}
	void TextureGL::Unlock(int m)
	{
		#if !defined( _MSC_VER) && !defined(__APPLE__)
glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_EXT);
				checkGlError("");
//glTexImage2D    (GLenum target, GLint level, GLint internalformat,         GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
//glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,GLenum format, GLenum type, const GLvoid *pixels);
//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, src + i * size);
//glTexImage2D((textures[renderTarget]).glTarget, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
//glTexSubImage2D(GL_TEXTURE_2D, tex.glTarget,i,0,0,tex.width, tex.height,srcFormat, srcType ,0);
#endif
	}

void RendererGL_1_1::BeginSceneVrtl(bool aB)
{
#if defined(SDL_2_0_4_BUILD)
            SDL_GL_MakeCurrent(g_SDL_window, g_SDL_context);
            //SDL_GL_GetDrawableSize(g_SDL_window, &w, &h);
#endif
}

		void RendererGL_1_1::SwapBuffersGL(void)
		{
//LOG_PRINT("SDL_GL_SwapBuffers:%s:%s:%d\n",__FILE__,__FUNCTION__,__LINE__);
#if defined(SDL_2_0_4_BUILD)
	SDL_GL_SwapWindow(g_SDL_window);
#elif defined(SDL_1_2_15_BUILD)
	SDL_GL_SwapBuffers();
#elif defined(_MSC_VER)
	SwapBuffers( hDc );
#else
	if(!m_dpy) m_dpy=STX_Service::GetWindowInstance()->m_dpy;
	if(!m_win) m_win=STX_Service::GetWindowInstance()->m_win;
	//LOG_PRINT("m_dpy=%d\n", m_dpy);
	//LOG_PRINT("m_win=%d\n", m_win);
	glXSwapBuffers( m_dpy, m_win );
#endif

		}

#define BUFSIZE 80
#ifndef GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862
#endif

	RendererGL_1_1::RendererGL_1_1(
		int awidth, int aheight,
		bool InitLogFile,
			bool fullScreen
			,int argc, char *argv[]
		): RendererHLSLCg(awidth, aheight,InitLogFile,argc, argv)
{

	// GL cannot use palettes anyway, so convert early.
	//ilEnable (IL_CONV_PAL);

	// Gets rid of dithering on some nVidia-based cards.
	//???ilutEnable (ILUT_OPENGL_CONV);
	
}

void RendererGL_1_1::setGLdefaults()
{
	ZERO		     = GL_ZERO;
	ONE		     = GL_ONE;
	SRC_COLOR	     = GL_SRC_COLOR;
	ONE_MINUS_SRC_COLOR  = GL_ONE_MINUS_SRC_COLOR;
	DST_COLOR	     = GL_DST_COLOR;
	ONE_MINUS_DST_COLOR  = GL_ONE_MINUS_DST_COLOR;
	SRC_ALPHA	     = GL_SRC_ALPHA;
	ONE_MINUS_SRC_ALPHA  = GL_ONE_MINUS_SRC_ALPHA;
	DST_ALPHA	     = GL_DST_ALPHA;
	ONE_MINUS_DST_ALPHA  = GL_ONE_MINUS_DST_ALPHA;
	SRC_ALPHA_SATURATE   = GL_SRC_ALPHA_SATURATE;
	BM_ADD		    = GL_FUNC_ADD_EXT;
	BM_SUBTRACT	    = GL_FUNC_SUBTRACT_EXT;
	BM_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT_EXT;
	BM_MIN		    = GL_MIN_EXT;
	BM_MAX		    = GL_MAX_EXT;
	NEVER	 = GL_NEVER;
	LESS	 = GL_LESS;
	EQUAL	 = GL_EQUAL;
	LEQUAL	 = GL_LEQUAL;
	GREATER  = GL_GREATER;
	NOTEQUAL = GL_NOTEQUAL;
	GEQUAL	 = GL_GEQUAL;
	ALWAYS	 = GL_ALWAYS;
	KEEP	 = GL_KEEP;
	SET_ZERO = GL_ZERO;
	REPLACE  = GL_REPLACE;
	INVERT	 = GL_INVERT;
	INCR	 = GL_INCR_WRAP;
	DECR	 = GL_DECR_WRAP;
	INCR_SAT = GL_INCR;
	DECR_SAT = GL_DECR;
	CULL_NONE  = 0;
	CULL_BACK  = GL_BACK;
	CULL_FRONT = GL_FRONT;
	SOLID = GL_FILL;
	WIREFRAME = GL_LINE;
	glEnable(GL_DEPTH_TEST);
				checkGlError("");
	glDepthFunc(GL_LEQUAL);
				checkGlError("");
	glFrontFace(GL_CW);
				checkGlError("");
	glPixelStorei(GL_PACK_ALIGNMENT,   1);
				checkGlError("");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				checkGlError("");
/*
	SetVersion((const char *) glGetString(GL_VERSION));
	SetVendor((const char *) glGetString(GL_VENDOR));
	SetRenderer((const char *) glGetString(GL_RENDERER));
	SetAntialiasing(false);
	SetMaxTextureSize(4096);
	SetVertexShaderVersion(3);
	SetPixelShaderVersion(3);
*/
}

void RendererGL_1_1::setup2DMode(const float left, const float right, const float top, const float bottom){
/*
	glMatrixMode(GL_PROJECTION);
				checkGlError("");
	glLoadIdentity();
				checkGlError("");
	glOrtho(left, right, bottom, top, -1, 1);
				checkGlError("");

	glMatrixMode(GL_MODELVIEW);
				checkGlError("");
	glLoadIdentity();
				checkGlError("");
*/
	scaleBias2D.x = 2.0f / (right - left);
	scaleBias2D.y = 2.0f / (top - bottom);
	scaleBias2D.z = -1.0f;
	scaleBias2D.w =  1.0f;

}

void RendererGL_1_1::setTexture(const char *textureName, const TextureID texture)
{
	if(texture==-1) //DBG_HALT;
	if(selectedShader == SHADER_NONE) return;

	int unit = getSamplerUnit(selectedShader, textureName);
	if (unit >= 0)
	{
		selectedTextures[unit] = texture;
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
bool RendererGL_1_1::supportsR2VB()
{
	return false;
}

bool RendererGL_1_1::supportsHalf()
{
	return bsupportsHalf;
}
bool RendererGL_1_1::supportsPS3()
{
	return false;
}
bool RendererGL_1_1::supportsATI1N()
{
	return false;
}

void RendererGL_1_1::changeFrontFace(const GLenum frontFace)
{
	if (frontFace != currentFrontFace)
	{
		glFrontFace(currentFrontFace = frontFace);
	}
}
void RendererGL_1_1::setupSampler(GLenum glTarget, const SamplerStateGL &ss)
{
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, ss.wrapS);
				checkGlError("");
	if (glTarget != GL_TEXTURE_1D) {glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, ss.wrapT);
				checkGlError("");}
	if (glTarget == GL_TEXTURE_3D) {glTexParameteri(glTarget, GL_TEXTURE_WRAP_R, ss.wrapR);
				checkGlError("");}
	glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, ss.magFilter);
				checkGlError("");






	glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, ss.minFilter);
	if (ss.aniso > 1 && GL_EXT_texture_filter_anisotropic_supported)
	{
		glTexParameteri(glTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, ss.aniso);
				checkGlError("");
	}
}

void RendererGL_1_1::setTextureSlice(const char *textureName, const TextureID texture, const int slice)
{
	//ASSERT(0);
}

		void RendererGL_1_1::resetToDefaults()
		{
			reset0ToDefaults();
			for (uint i = 0; i < MAX_TEXTUREUNIT; i++)
			{
				currentTextures[i] = TEXTURE_NONE;
			}
			for (uint i = 0; i < MAX_SAMPLERSTATE; i++)
			{
				currentSamplerStates[i] = SS_NONE;
			}
			currentSrcFactorRGB = currentSrcFactorAlpha = ONE;
			currentDstFactorRGB = currentDstFactorAlpha = ZERO;
			currentBlendModeRGB = currentBlendModeAlpha = BM_ADD;
			currentMask = ALL;
			currentBlendEnable = false;
			currentAlphaToCoverageEnable = false;
			currentDepthFunc = LEQUAL;
			currentDepthTestEnable = true;
			#if 0
			currentDepthWriteEnable = false;
			glDepthMask(GL_FALSE);
			#elif 0
			currentDepthWriteEnable = true;
			glDepthMask(GL_TRUE);
			#endif
			currentDepthWriteEnable = true;
			currentCullMode = CULL_NONE;
			currentFillMode = SOLID;
			currentMultiSampleEnable = true;
			currentScissorEnable = false;
		}
		void RendererGL_1_1::resetVrtl(const uint flags)
		{
			reset0(flags);
			if (flags & RESET_TEX)
			{
				for (uint i = 0; i < MAX_TEXTUREUNIT; i++)
				{
					selectedTextures[i] = TEXTURE_NONE;
				}
			}
		}




		void RendererGL_1_1::Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors)
		{
//
#if 1//!defined(SDL_2_0_4_BUILD)
#if (!defined(QT_BUILD))
			if(color)
			{
#if 0//defined(QT_BUILD)
	qglClearColor(colors.x,colors.y,colors.z,colors.w);
#elif 0//defined(SDL_2_0_4_BUILD)
	glClearColor(colors.x,colors.y,colors.z,colors.w);
				checkGlError("");
#else
				glClearColor(colors.x,colors.y,colors.z,colors.w);
				checkGlError("");
#endif

			}
			if(depth)
			{
				glClearDepth(1.0f);
				checkGlError("");
			}
			int f=0;
			if(color) f|=GL_COLOR_BUFFER_BIT;
			if(depth) f|=GL_DEPTH_BUFFER_BIT;
			glClear(f);
				checkGlError("");

				checkGlError("");
#endif
#endif
		}
		void RendererGL_1_1::PresentVrtl(bool aB)
		{
//
		bool b=true;//???((aB&&(m_pInput!=0)) || ((!aB)&&(m_pInput==0)));
if(b)
{

			IRenderer::OnPresent();

			SwapBuffersGL();

		}
		}

		void RendererGL_1_1::captureScreenshot(Image3& img)
		{
			int w=IRenderer::GetRendererInstance()->GetViewportWidth();
			int h=IRenderer::GetRendererInstance()->GetViewportHeight();
			img.create(FORMAT_RGB8, w, h, 1, 1);
			glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (BYTE *)img.getPixels());
				checkGlError("");
			img.flipY();
		}
		void RendererGL_1_1::SaveImg(Image3& img,const char* fn)
		{
		int l=0;
        l=int(stx_strrchr(fn,'.')-fn);
		std::string fns(fn);
		fns.resize(l);
					char bufpng[256];
                    stx_snprintf(bufpng,256,"%s.png",fns.c_str());
					img.saveImageLibImage(bufpng,false);
		}

		void RendererGL_1_1::SaveScreenShotVrtl(const char* fn)
		{
		if(fn==0)return;

		{
			Image3 img;
			char buf[256];
			char gifbuf[256];
			if(fn!=0)
			{
                stx_snprintf(buf,256,"../%s.jpg",fn);
                stx_snprintf(gifbuf,256,"../%s.gif",fn);

			}
			else
			{
                stx_snprintf(buf,256,"../%s","ScreenShot.jpg");
                stx_snprintf(gifbuf,256,"../%s","ScreenShot.gif");
			}
			captureScreenshot(img);
			img.flipX();
			img.saveImageLibImage(buf,false);
			RendererHLSLCg::OpenILToGIF(img,gifbuf);
		}
		}


		void RendererGL_1_1::changeVertexFormat(const VertexFormatID vertexFormat)
		{
#if defined(USE_HLSL_SHADERS)
			//if (vertexFormat != currentVertexFormat)
			{
				static VertexFormatGL zero;
				VertexFormatGL curr = zero, sel = zero;
				if (currentVertexFormat != VF_NONE) curr = vertexFormats[currentVertexFormat];
				if (vertexFormat != VF_NONE) sel = vertexFormats[vertexFormat];
				if ( sel.vertex.size && !curr.vertex.size)
				{
					glEnableClientState (GL_VERTEX_ARRAY);
				checkGlError("");
				}
				if (!sel.vertex.size &&  curr.vertex.size)
				{
					glDisableClientState(GL_VERTEX_ARRAY);
				checkGlError("");
				}
				if ( sel.normal.size && !curr.normal.size) {glEnableClientState(GL_NORMAL_ARRAY);
				checkGlError("");}
				if (!sel.normal.size &&  curr.normal.size) {glDisableClientState(GL_NORMAL_ARRAY);
				checkGlError("");}
				for (int i = 0; i < MAX_GENERIC; i++)
				{
					if ( sel.generic[i].size && !curr.generic[i].size) {glEnableVertexAttribArrayARB(i);
				checkGlError("");}
					if (!sel.generic[i].size &&  curr.generic[i].size) {glDisableVertexAttribArrayARB(i);
				checkGlError("");}
				}
				for (int i = 0; i < MAX_TEXCOORD; i++)
				{
					if ((sel.texCoord[i].size > 0) ^ (curr.texCoord[i].size > 0))
					{
						glClientActiveTexture(GL_TEXTURE0 + i);
				checkGlError("");
						if (sel.texCoord[i].size > 0)
						{
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				checkGlError("");
						}
						else
						{
							glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				checkGlError("");
						}
					}
				}
				currentVertexFormat = vertexFormat;
			}
#endif
		}

		void RendererGL_1_1::changeDepthStateVrtl(const DepthStateID depthState, const uint stencilRef)


		{
			if (depthState != currentDepthState)
			{
				if (depthState == DS_NONE)
				{
					if (!currentDepthTestEnable)

					{
						glEnable(GL_DEPTH_TEST);
				checkGlError("");

						currentDepthTestEnable = true;
					}
					if (!currentDepthWriteEnable)
					{
						glDepthMask(GL_TRUE);
				checkGlError("");
						currentDepthWriteEnable = true;
					}
					if (currentDepthFunc != LEQUAL)
					{
						glDepthFunc(currentDepthFunc = LEQUAL);
				checkGlError("");
					}
				}
				else
				{

					if ((depthStates[depthState]).depthTest)
					{
						if (!currentDepthTestEnable)
						{
							glEnable(GL_DEPTH_TEST);
				checkGlError("");
							currentDepthTestEnable = true;
						}
						if ((depthStates[depthState]).depthWrite != currentDepthWriteEnable)
						{
							glDepthMask((currentDepthWriteEnable = (depthStates[depthState]).depthWrite)? GL_TRUE : GL_FALSE);
				checkGlError("");
						}
						if ((depthStates[depthState]).depthFunc != currentDepthFunc)
						{
							glDepthFunc(currentDepthFunc = (depthStates[depthState]).depthFunc);
				checkGlError("");
						}
					}
					else
					{
						if (currentDepthTestEnable)
						{
							glDisable(GL_DEPTH_TEST);
				checkGlError("");
							currentDepthTestEnable = false;
						}
					}
				}
				currentDepthState = depthState;
			}
		}
		void RendererGL_1_1::changeRasterizerState(const RasterizerStateID rasterizerState)
		{
			if (rasterizerState != currentRasterizerState)
			{
				RasterizerStateGL state;
				if (rasterizerState == RS_NONE)
				{
					state.cullMode = CULL_NONE;
					state.fillMode = SOLID;
					state.multiSample = true;
					state.scissor = false;
				}
				else
				{
					state = (rasterizerStates[rasterizerState]);
				}
				if (state.cullMode != currentCullMode)
				{
					if (state.cullMode == CULL_NONE)
					{
						glDisable(GL_CULL_FACE);
				checkGlError("");
					}

					else
					{
						if (currentCullMode == CULL_NONE) {glEnable(GL_CULL_FACE);
				checkGlError("");}
						glCullFace(state.cullMode);
				checkGlError("");
					}
					currentCullMode = state.cullMode;
				}
				if (state.fillMode != currentFillMode)
				{
					glPolygonMode(GL_FRONT_AND_BACK, currentFillMode = state.fillMode);
				checkGlError("");
				}
				if (state.multiSample != currentMultiSampleEnable)
				{
					if (state.multiSample)
					{
						glEnable(GL_MULTISAMPLE);
				checkGlError("");
					}
					else
					{
						glDisable(GL_MULTISAMPLE);
				checkGlError("");
					}
					currentMultiSampleEnable = state.multiSample;
				}
				if (state.scissor != currentScissorEnable)
				{
					if (state.scissor)
					{
						glEnable(GL_SCISSOR_TEST);
				checkGlError("");
					}
					else
					{
						glDisable(GL_SCISSOR_TEST);
				checkGlError("");
					}
					currentScissorEnable = state.scissor;
				}
				currentRasterizerState = rasterizerState;
			}
		}




		VertexFormatID RendererGL_1_1::addVertexFormatVrtl(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader)
		{

#if defined(USE_HLSL_SHADERS)
			VertexFormatGL vertexFormat;
			memset(&vertexFormat, 0, sizeof(vertexFormat));
			uint nGeneric  = 0;
			uint nTexCoord = 0;

			for (uint i = 0; i < nAttribs; i++)
			{
				if (formatDesc[i].type == TYPE_VERTEX)
				{
					nGeneric = 1;
					break;
				}
			}

			for (uint i = 0; i < nAttribs; i++)
			{
				int stream = formatDesc[i].stream;
				switch (formatDesc[i].type)
				{
					case TYPE_GENERIC:
					case TYPE_TANGENT:
					case TYPE_BINORMAL:
					vertexFormat.generic[nGeneric].stream = stream;
					vertexFormat.generic[nGeneric].size   = formatDesc[i].size;
					vertexFormat.generic[nGeneric].offset = vertexFormat.vertexSize[stream];
					vertexFormat.generic[nGeneric].format = formatDesc[i].format;
					nGeneric++;
					break;
					case TYPE_VERTEX:
					vertexFormat.vertex.stream = stream;
					vertexFormat.vertex.size   = formatDesc[i].size;
					vertexFormat.vertex.offset = vertexFormat.vertexSize[stream];
					vertexFormat.vertex.format = formatDesc[i].format;
					break;
					case TYPE_NORMAL:
					vertexFormat.normal.stream = stream;
					vertexFormat.normal.size   = formatDesc[i].size;
					vertexFormat.normal.offset = vertexFormat.vertexSize[stream];
					vertexFormat.normal.format = formatDesc[i].format;
					break;
					case TYPE_TEXCOORD:
					vertexFormat.texCoord[nTexCoord].stream = stream;
					vertexFormat.texCoord[nTexCoord].size	 = formatDesc[i].size;
					vertexFormat.texCoord[nTexCoord].offset = vertexFormat.vertexSize[stream];
					vertexFormat.texCoord[nTexCoord].format = formatDesc[i].format;
					nTexCoord++;
					break;
				}
				vertexFormat.vertexSize[stream] += formatDesc[i].size * getFormatSize(formatDesc[i].format);
			}

			vertexFormat.maxGeneric = nGeneric;
			vertexFormat.maxTexCoord = nTexCoord;
			vertexFormats.push_back(vertexFormat);

			return vertexFormats.size()-1;
#else

			VertexFormatGL vertexFormat;

			memset(&vertexFormat, 0, sizeof(vertexFormat));

			uint nGeneric  = 0;
			uint nTexCoord = 0;

			/*
			for (uint i = 0; i < nAttribs; i++)
			{
				int stream = formatDesc[i].stream;
				if (formatDesc[i].type == TYPE_VERTEX)
				{
					vertexFormat.generic[nGeneric].stream = stream;
					vertexFormat.generic[nGeneric].size   = formatDesc[i].size;
					vertexFormat.generic[nGeneric].offset = vertexFormat.vertexSize[stream];
					vertexFormat.generic[nGeneric].format = formatDesc[i].format;
					nGeneric = 1;
					break;
				}
			}*/

			for (uint i = 0; i < nAttribs; i++)
			{
				int stream = formatDesc[i].stream;
				switch (formatDesc[i].type)
				{
					case TYPE_GENERIC:
					case TYPE_TANGENT:
					case TYPE_BINORMAL:

					case TYPE_VERTEX:
					case TYPE_NORMAL:
					case TYPE_TEXCOORD:
					if(formatDesc[i].type==TYPE_TEXCOORD) nTexCoord++;


					vertexFormat.generic[nGeneric].stream = stream;
					vertexFormat.generic[nGeneric].size   = formatDesc[i].size;
					vertexFormat.generic[nGeneric].offset = vertexFormat.vertexSize[stream];
					vertexFormat.generic[nGeneric].format = formatDesc[i].format;
					nGeneric++;
					break;/*
					case TYPE_VERTEX:
					vertexFormat.vertex.stream = stream;
					vertexFormat.vertex.size   = formatDesc[i].size;
					vertexFormat.vertex.offset = vertexFormat.vertexSize[stream];
					vertexFormat.vertex.format = formatDesc[i].format;
					break;
					case TYPE_NORMAL:
					vertexFormat.normal.stream = stream;
					vertexFormat.normal.size   = formatDesc[i].size;
					vertexFormat.normal.offset = vertexFormat.vertexSize[stream];
					vertexFormat.normal.format = formatDesc[i].format;
					break;
					case TYPE_TEXCOORD:
					vertexFormat.texCoord[nTexCoord].stream = stream;
					vertexFormat.texCoord[nTexCoord].size	 = formatDesc[i].size;
					vertexFormat.texCoord[nTexCoord].offset = vertexFormat.vertexSize[stream];
					vertexFormat.texCoord[nTexCoord].format = formatDesc[i].format;
					nTexCoord++;
					break;*/
				}

				vertexFormat.vertexSize[stream] += formatDesc[i].size * getFormatSize(formatDesc[i].format);
			}

			vertexFormat.shader=shader;

			vertexFormat.maxGeneric = nGeneric;

#ifdef USE_HLSL_SHADERS
			vertexFormat.maxTexCoord = nTexCoord;
#endif

			vertexFormats.push_back(vertexFormat);

			return vertexFormats.size()-1;

#endif

		}
GLenum usages[] =
		{
			GL_STATIC_DRAW,
			GL_DYNAMIC_DRAW,
			GL_STREAM_DRAW,
		}
		;
		//STXArray<GLenum,3> usages(_usages);
		VertexBufferID RendererGL_1_1::addVertexBufferVrtl(const long size, const BufferAccess bufferAccess, const void *data, VertexBufferID aold)//, const IDataProcessor* p)
		{
//////
if(aold>-1)
{
//////
	glDeleteBuffers(1,  &vertexBuffers[aold].vboVB);
				checkGlError("");
//
//////
	vertexBuffers[aold].vboVB=0;
	glGenBuffersARB(1, &vertexBuffers[aold].vboVB);
				checkGlError("");

	vertexBuffers[aold].size=size;
//////
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertexBuffers[aold].vboVB);
				checkGlError("");

}
//////
VertexBufferGL vb;
if(aold==-1)
{
	vb.size = size;
	GLuint vbb=0;
//////
	glGenBuffersARB
//ARB
(1, &vbb);
				checkGlError("");

	vb.vboVB=vbb;
	vertexBuffers.push_back(vb);
//////

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbb);
				checkGlError("");

}
//////
	if(data) {
//////
vb.data=(BYTE*)data;
glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, usages[bufferAccess]);
				checkGlError("");

}
//////
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				checkGlError("");

//////
/*
LOG_PRINT("vbb=%d\n",vbb);
LOG_PRINT("size=%d\n",size);
LOG_PRINT("data=%d\n",data);
*/


			if(aold==-1)
			{

				//LOG_PRINT("vbid=%d\n",vertexBuffers.size()-1);
				return vertexBuffers.size()-1;
			}
			else if((aold>=0)&&(aold<vertexBuffers.size()))
{
//LOG_PRINT("vbid=%d\n",old);
return aold;
}
#if 0
			else if(0)
			{
				VertexBufferID tmpVertexBuffers[MAX_VERTEXSTREAM];
				intptr tmpOffsets[MAX_VERTEXSTREAM];
				for(int i=0;i<MAX_VERTEXSTREAM;i++)
				{
					tmpVertexBuffers[i]=currentVertexBuffers[i];
					tmpOffsets[i]=currentOffsets[i];
				}
				//int vf=currentVertexFormat;
				int vf=selectedVertexFormat;
				setVertexFormat(-1);
				for(int i=0;i<MAX_VERTEXSTREAM;i++)
				{
					if(-1!=currentVertexBuffers[i])
					{
						glBindBufferARB(GL_ARRAY_BUFFER_ARB, currentVBO = 0);
				checkGlError("");

						currentVertexBuffers[i] = -1;
					}
				}
				deleteVertexBuffer(old);//???
				vertexBuffers[old]=vb;
				setVertexFormat(vf);
				for(int i=0;i<MAX_VERTEXSTREAM;i++)
				{
					setVertexBuffer(i,tmpVertexBuffers[i],tmpOffsets[i]);
				}
				return old;
			}
#endif
			else
{
//LOG_PRINT("vbid=%d\n",-1);

			//DBG_HALT;
			return -1;
}
		}
		IndexBufferID RendererGL_1_1::addIndexBufferVrtl(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID aold)//, const IDataProcessor* p)
		{
			uint size = nIndices * indexSize;
//////
if(aold>-1)
{
//////
	glDeleteBuffers(1,  &indexBuffers[aold].vboIB);
				checkGlError("");
//
//////
	indexBuffers[aold].vboIB=0;
	glGenBuffersARB(1, &indexBuffers[aold].vboIB);
				checkGlError("");

//////
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffers[aold].vboIB);
				checkGlError("");

//////
	indexBuffers[aold].nIndices = nIndices;
	indexBuffers[aold].indexSize = indexSize;
}
//////
	IndexBufferGL ib;
if(aold==-1)
{
	GLuint vbb=0;
	ib.nIndices = nIndices;
	ib.indexSize = indexSize;
//////
	glGenBuffersARB(1, &vbb);
				checkGlError("");

	ib.vboIB=vbb;
//////
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbb);
				checkGlError("");

//////
	indexBuffers.push_back(ib);
}
//////
//if(old>-1)
	//indexBuffers[old].size=size;

	if(data) {
//////
ib.data=(BYTE*)data;
glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size, data, usages[bufferAccess]);
				checkGlError("");

}
//////
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
				checkGlError("");

//////
			if(aold==-1)
			{

				//LOG_PRINT("vbid=%d\n",vertexBuffers.size()-1);
				return indexBuffers.size()-1;
			}
			else if((aold>=0)&&(aold<indexBuffers.size()))
{
return aold;
}
else
	return -1;
/*

			glGenBuffersARB(1, &ib.vboIB);
				checkGlError("");
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib.vboIB);
				checkGlError("");
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, size, data, usages[bufferAccess]);
				checkGlError("");
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
				checkGlError("");
			if(old==-1)
			{
				indexBuffers.push_back(ib);
				return indexBuffers.size()-1;
			}
			else
			{
				deleteIndexBuffer(old);
				indexBuffers[old]=ib;
				return old;
			}
			return -1;
*/
		}
		GLint minFilters[] =
		{
			GL_NEAREST,
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
		}
		;
		SamplerStateID RendererGL_1_1::addSamplerStateVrtl(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod, const uint maxAniso, const int compareFunc, const float *border_color)
		{
			if (compareFunc != 0 || border_color != 0) return SS_NONE;
			SamplerStateGL samplerState;
			samplerState.minFilter = minFilters[filter];
			samplerState.magFilter = (filter == NEAREST)? GL_NEAREST : GL_LINEAR;
			samplerState.wrapS = (s == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.wrapT = (t == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.wrapR = (r == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.aniso = hasAniso(filter)? min((uint) maxAnisotropic, maxAniso) : 1;
			samplerState.lod = lod;
			samplerStates.push_back(samplerState);
			return samplerStates.size()-1;
		}
		DepthStateID RendererGL_1_1::addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc, const bool stencilTest,
		const uint8 stencilMask, const int stencilFunc, const int stencilFail, const int depthFail, const int stencilPass)
		{
			DepthStateGL depthState;
			depthState.depthTest  = depthTest;
			depthState.depthWrite = depthWrite;
			depthState.depthFunc  = depthFunc;
			depthStates.push_back(depthState);
			return depthStates.size()-1;
		}
		RasterizerStateID RendererGL_1_1::addRasterizerStateVrtl(const int cullMode, const int fillMode, const bool multiSample, const bool scissor)
		{
			RasterizerStateGL rasterizerState;
			rasterizerState.cullMode = cullMode;
			rasterizerState.fillMode = fillMode;
			rasterizerState.multiSample = multiSample;
			rasterizerState.scissor = scissor;
			rasterizerStates.push_back(rasterizerState);
			return rasterizerStates.size()-1;
		}
		static const GLint internalFormats[] =
		{

			0,
			GL_INTENSITY8,
			GL_LUMINANCE8_ALPHA8,
			GL_RGB8,
			GL_RGBA8,
			GL_INTENSITY16,
			GL_LUMINANCE16_ALPHA16,
			GL_RGB16,
			GL_RGBA16,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			GL_INTENSITY_FLOAT16_ATI,
			GL_LUMINANCE_ALPHA_FLOAT16_ATI,
			GL_RGB_FLOAT16_ATI,
			GL_RGBA_FLOAT16_ATI,
			GL_INTENSITY_FLOAT32_ATI,
			GL_LUMINANCE_ALPHA_FLOAT32_ATI,
			GL_RGB_FLOAT32_ATI,
			GL_RGBA_FLOAT32_ATI,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, GL_RGB5,
			GL_RGBA4,
			GL_RGB10_A2,
			GL_DEPTH_COMPONENT16,
			GL_DEPTH_COMPONENT24,
			GL_DEPTH24_STENCIL8_EXT,
			0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
			0,
#ifdef _MSC_VER
GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI,
#else
0,
#endif
		}
		;
		static const GLenum srcFormats[] =
		{
			0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA
		}
		;
		static const GLenum srcTypes[] =
		{
			0,
			GL_UNSIGNED_BYTE,
			GL_UNSIGNED_BYTE,
			GL_UNSIGNED_BYTE,
			GL_UNSIGNED_BYTE,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_SHORT,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			GL_FLOAT,
			GL_FLOAT,
			GL_FLOAT,
			GL_FLOAT,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0, 0, 0, GL_UNSIGNED_SHORT_5_6_5,
			GL_UNSIGNED_SHORT_4_4_4_4_REV,
			GL_UNSIGNED_INT_2_10_10_10_REV,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_INT,
			GL_UNSIGNED_INT_24_8_EXT,
			0, 0,
			0,
			0,
			0,
			0,
		}
		;

		static const char* internalFormats_[] =
		{

			0,
			"GL_INTENSITY8",
			"GL_LUMINANCE8_ALPHA8",
			"GL_RGB8",
			"GL_RGBA8",
			"GL_INTENSITY16",
			"GL_LUMINANCE16_ALPHA16",
			"GL_RGB16",
			"GL_RGBA16",
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			"GL_INTENSITY_FLOAT16_ATI",
			"GL_LUMINANCE_ALPHA_FLOAT16_ATI",
			"GL_RGB_FLOAT16_ATI",
			"GL_RGBA_FLOAT16_ATI",
			"GL_INTENSITY_FLOAT32_ATI",
			"GL_LUMINANCE_ALPHA_FLOAT32_ATI",
			"GL_RGB_FLOAT32_ATI",
			"GL_RGBA_FLOAT32_ATI",
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "GL_RGB5",
			"GL_RGBA4",
			"GL_RGB10_A2",
			"GL_DEPTH_COMPONENT16",
			"GL_DEPTH_COMPONENT24",
			"GL_DEPTH24_STENCIL8_EXT",
			0, "GL_COMPRESSED_RGB_S3TC_DXT1_EXT",
			"GL_COMPRESSED_RGBA_S3TC_DXT3_EXT",
			"GL_COMPRESSED_RGBA_S3TC_DXT5_EXT",
			0,
#ifdef _MSC_VER
"GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI",
#else
0,
#endif
		}
		;
		static const char* srcFormats_[] =
		{
			0, "GL_LUMINANCE", "GL_LUMINANCE_ALPHA", "GL_RGB", "GL_RGBA"
		}
		;
		static const char* srcTypes_[] =
		{
			0,
			"GL_UNSIGNED_BYTE",
			"GL_UNSIGNED_BYTE",
			"GL_UNSIGNED_BYTE",
			"GL_UNSIGNED_BYTE",
			"GL_UNSIGNED_SHORT",
			"GL_UNSIGNED_SHORT",
			"GL_UNSIGNED_SHORT",
			"GL_UNSIGNED_SHORT",
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			"GL_FLOAT",
			"GL_FLOAT",
			"GL_FLOAT",
			"GL_FLOAT",
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0, 0, 0, "GL_UNSIGNED_SHORT_5_6_5",
			"GL_UNSIGNED_SHORT_4_4_4_4_REV",
			"GL_UNSIGNED_INT_2_10_10_10_REV",
			"GL_UNSIGNED_SHORT",
			"GL_UNSIGNED_INT",
			"GL_UNSIGNED_INT_24_8_EXT",
			0, 0,
			0,
			0,
			0,
			0,
		}
		;



		void RendererGL_1_1::setSamplerState(const char *samplerName, const SamplerStateID samplerState)
		{
		}
		void RendererGL_1_1::applySamplerStates()
		{
		}


int RendererGL_1_1::GetTextureWidth(TextureID id)
{
	assert(id<textures.size());
	return textures[id].width;
}
int RendererGL_1_1::GetTextureHeight(TextureID id)
{
	assert(id<textures.size());
	return textures[id].height;
}

		TextureID RendererGL_1_1::addTexture(Image3* img, const bool useMips, const SamplerStateID samplerState, uint flags, float3 alpha)
		{
			#if 0//defined(ANDROID)
			#define LOG_FNLN_A LOG_FNLN
			#define LOG_PRINT_A LOG_PRINT
			#else
			#define LOG_FNLN_A
			#define LOG_PRINT_A
			#endif
			
			//ASSERT(samplerState != SS_NONE);
			LOG_FNLN_A;
			TextureGL tex;
			LOG_FNLN_A;
			memset(&tex, 0, sizeof(TextureGL));
			LOG_FNLN_A;
	//img->BindImage();
	tex.ss=samplerState;
	tex.width=img->getWidth();
	tex.height=img->getHeight();
	tex.depth=img->getDepth();
			LOG_FNLN_A;
	if(useMips&&img->getMipMapCount()<2)
		img->createMipMaps();
			LOG_FNLN_A;
	tex.mipMapLevels=img->getMipMapCount();
	const FORMAT format = img->getFormat();
	tex.format=format;
			LOG_FNLN_A;
#if 0
					if(alpha>0.0f)
					{
#if 1
						if(getChannelCount(format)==4){img->removeChannels(true,true,true,false);}
						img->addChannel(alpha);
#else
						img->format(FORMAT_RGBA8);
						img->setAlpha(alpha);
						format = img->getFormat();
#endif
					}
#endif
			GLenum srcFormat = srcFormats[getChannelCount(format)];
			GLenum srcType = srcTypes[format];
			GLint internalFormat = internalFormats[format];
			const char* srcFormat_ = srcFormats_[getChannelCount(format)];
			const char* srcType_ = srcTypes_[format];
			const char* internalFormat_ = internalFormats_[format];
			LOG_FNLN_A;
			if ((flags & HALF_FLOAT) != 0 && format >= FORMAT_I32f && format <= FORMAT_RGBA32f)
			{
				internalFormat = internalFormats[format - (FORMAT_I32f - FORMAT_I16f)];
			}
			LOG_FNLN_A;

			tex.lod = (samplerStates[samplerState]).lod;
			tex.glTarget = img->isCube()? GL_TEXTURE_CUBE_MAP : img->is3D()? GL_TEXTURE_3D : img->is2D()? GL_TEXTURE_2D : GL_TEXTURE_1D;
			LOG_FNLN_A;
			glGenTextures(1, &tex.glTexID);
				checkGlError("");
			LOG_FNLN_A;
			//LOG_PRINT("tex.glTexID=%d\n",tex.glTexID);
			glBindTexture(tex.glTarget, tex.glTexID);
				checkGlError("");
			LOG_FNLN_A;
			setupSampler(tex.glTarget, samplerStates[samplerState]);
			LOG_FNLN_A;
			tex.samplerState = samplerState;

	// Upload it all
	ubyte *src;
	int mipMapLevel = 0;
	while ((src = img->getPixels(mipMapLevel)) != NULL){
		if (img->isCube()){
			int size = img->getMipMappedSize(mipMapLevel, 1) / 6;
			for (uint i = 0; i < 6; i++){
				if (isCompressedFormat(format)){
					glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, size, src + i * size);
				} else {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, src + i * size);
				}
			}
		} else if (img->is3D()){
			if (isCompressedFormat(format)){
				glCompressedTexImage3DARB(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), img->getDepth(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), src);
			} else {
				glTexImage3D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), img->getDepth(mipMapLevel), 0, srcFormat, srcType, src);
			}
		} else if (img->is2D()){
			if (isCompressedFormat(format)){
				glCompressedTexImage2DARB(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), src);
			} else {
				glTexImage2D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, src);
			}
		} else {
			glTexImage1D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), 0, srcFormat, srcType, src);
		}
				
				if((!useMips) || (img->getMipMapCount()<2))
					break;
				mipMapLevel++;
			LOG_FNLN_A;
			}
////LOG_FNLN
			LOG_FNLN_A;
			glBindTexture(tex.glTarget, 0);
				checkGlError("");
			LOG_FNLN_A;
////LOG_FNLN
			return textures.add(tex);
		}
		TextureID RendererGL_1_1::addRenderTargetVrtl(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags)
		{
			if (isCompressedFormat(format) || arraySize > 1) return false;
			if (mipMapCount > 1) return false;
			TextureGL tex;
			memset(&tex, 0, sizeof(TextureGL));
			tex.glTarget = (flags & CUBEMAP)? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
			tex.flags  = flags;
			tex.format = format;
			glGenTextures(1, &tex.glTexID);
				checkGlError("");
			glBindTexture(tex.glTarget, tex.glTexID);
				checkGlError("");
			setupSampler(tex.glTarget, samplerStates[samplerState]);
			tex.samplerState = samplerState;
			TextureID rt = textures.add(tex);
			setRenderTargetSize(rt, width, height);
			glBindTexture(tex.glTarget, 0);
				checkGlError("");
			return rt;
		}
		TextureID RendererGL_1_1::addRenderDepthVrtl(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags)
		{
			if (arraySize > 1) return false;
			TextureGL tex;
			memset(&tex, 0, sizeof(TextureGL));
			tex.glTarget = GL_RENDERBUFFER_EXT;
			tex.format = format;
			glGenRenderbuffersEXT(1, &tex.glDepthID);
				checkGlError("");
			TextureID rt = textures.add(tex);
			setRenderTargetSize(rt, width, height);
			return rt;
		}
		void RendererGL_1_1::setRenderTargetSize(const TextureID renderTarget, const int width, const int height)
		{
			(textures[renderTarget]).width	= width;
			(textures[renderTarget]).height = height;
			if ((textures[renderTarget]).glTarget == GL_RENDERBUFFER_EXT)
			{
				FORMAT format = (textures[renderTarget]).format;
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, (textures[renderTarget]).glDepthID);
				checkGlError("");
				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalFormats[format], width, height);
				checkGlError("");
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
				checkGlError("");
			}
			else
			{
				FORMAT format = (textures[renderTarget]).format;
				GLint internalFormat = internalFormats[format];
				GLenum srcFormat = srcFormats[getChannelCount(format)];
				GLenum srcType = srcTypes[format];
				if (isDepthFormat(format))
				{
					if (isStencilFormat(format))
					{

						srcFormat = GL_DEPTH_STENCIL_EXT;
					}
					else
					{
						srcFormat = GL_DEPTH_COMPONENT;


					}
				}

				if (isFloatFormat(format)) srcType = GL_FLOAT;
				glBindTexture((textures[renderTarget]).glTarget, (textures[renderTarget]).glTexID);
				if ((textures[renderTarget]).flags & CUBEMAP)
				{
					for (int i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; i++)
					glTexImage2D(i, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
				checkGlError("");
				}
				else
				{
					glTexImage2D((textures[renderTarget]).glTarget, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
				checkGlError("");
				}
				glBindTexture((textures[renderTarget]).glTarget, 0);
				checkGlError("");
			}
		}
		bool RendererGL_1_1::resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize)
		{
			if (depth > 1 || arraySize > 1 || mipMapCount > 1) return false;
			if (width != (textures[renderTarget]).width || height != (textures[renderTarget]).height)
			{
				setRenderTargetSize(renderTarget, width, height);
			}
			return true;
		}
		bool RendererGL_1_1::generateMipMaps(const TextureID renderTarget)
		{
			return false;
		}
		void RendererGL_1_1::changeRenderTargetsVrtl(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int depthSlice, const int *slices)
		{
			apply();
			if (fbo == 0) glGenFramebuffersEXT(1, &fbo);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
				checkGlError("");
			for (uint i = 0; i < nRenderTargets; i++)
			{
				TextureID colorRT = colorRTs[i];
				if ((textures[colorRT]).flags & CUBEMAP)
				{
					if (colorRT != currentColorRT[i] || currentColorRTSlice[i] != slices[i])
					{
						glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + slices[i], (textures[colorRT]).glTexID, 0);
				checkGlError("");
						currentColorRTSlice[i] = slices[i];
					}
				}
				else
				{
					if (colorRT != currentColorRT[i])
					{
						glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, (textures[colorRT]).glTexID, 0);
				checkGlError("");
						currentColorRT[i] = colorRT;
					}
				}
			}
			if (nRenderTargets != nCurrentRenderTargets)
			{
				for (uint i = nRenderTargets; i < nCurrentRenderTargets; i++)
				{
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, 0, 0);
				checkGlError("");
					currentColorRT[i] = TEXTURE_NONE;
					currentColorRTSlice[i] = NO_SLICE;
				}
				if (nRenderTargets == 0)
				{
					glDrawBuffer(GL_NONE);
				checkGlError("");
					glReadBuffer(GL_NONE);
				checkGlError("");
				}
				else
				{
					glDrawBuffersARB(nRenderTargets, drawBuffers);
				checkGlError("");
					glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
				checkGlError("");
				}
				nCurrentRenderTargets = nRenderTargets;
			}
			if (depthRT != currentDepthRT)
			{
				if (depthRT >= 0 && (textures[depthRT]).glTarget != GL_RENDERBUFFER_EXT)
				{
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, (textures[depthRT]).glTexID, 0);
				checkGlError("");
					if (isStencilFormat((textures[depthRT]).format))
					{
						glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, (textures[depthRT]).glTexID, 0);
				checkGlError("");
					}
					else
					{
						glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
				checkGlError("");
					}
				}
				else
				{
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, (depthRT < 0)? 0 : (textures[depthRT]).glTexID);
				checkGlError("");
					if (depthRT >= 0 && isStencilFormat((textures[depthRT]).format))
					{
						glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, (textures[depthRT]).glTexID);
				checkGlError("");
					}
					else
					{
						glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
				checkGlError("");
					}
				}
				currentDepthRT = depthRT;
			}
			if (nRenderTargets > 0)
			{
				if ((textures[colorRTs[0]]).flags & CUBEMAP)
				{
					changeFrontFace(GL_CCW);
				}
				else
				{
					changeFrontFace(GL_CW);
				}
				glViewport(0, 0, (textures[colorRTs[0]]).width, (textures[colorRTs[0]]).height);
				checkGlError("");
			}
			else if(depthRT >= 0)
			{
				changeFrontFace(GL_CW);
				glViewport(0, 0, (textures[depthRT]).width, (textures[depthRT]).height);
				checkGlError("");
			}
			reset(0);
		}
		void RendererGL_1_1::changeToMainFramebuffer()
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
				checkGlError("");
			glViewport(m_viewport.X,m_viewport.Y, m_viewport.Width, m_viewport.Height);
				checkGlError("");
			changeFrontFace(GL_CW);
		}
		void RendererGL_1_1::Init0(void)
		{
	if(defaultFont == -1)
	{
		//defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);
	}

					}

UINT32 RendererGL_1_1::GetAvailableTextureMem( )
{
/*
        GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
        GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
        GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
        GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
        GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B

        VBO_FREE_MEMORY_ATI                     0x87FB
        TEXTURE_FREE_MEMORY_ATI                 0x87FC
        RENDERBUFFER_FREE_MEMORY_ATI            0x87FD

int total, current;
 glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total);
 glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &current);

 Local param:Int[4]
 param[0]=0; param[1]=0; param[2]=0; param[3]=0;
 glGetIntegerv (TEXTURE_FREE_MEMORY_ATI, Varptr (param[0]))

// param[0] : 

// param[1] : 

// param[2] : Total auxiliary memory free.

// param[3] : Largest auxiliary free block.
*/
	return 0;
}

		int RendererGL_1_1::removeTexture(const TextureID texture)
		{
			if(texture>=textures.size()) return -1;
			if ((textures[texture]).glTarget)
			{
				if ((textures[texture]).glTarget == GL_RENDERBUFFER_EXT)
				{
					glDeleteRenderbuffersEXT(1, &(textures[texture]).glDepthID);
				checkGlError("");
				}
				else
				{
					glDeleteTextures(1, &(textures[texture]).glTexID);
				checkGlError("");
				}
				(textures[texture]).glTarget = 0;
			}
	int r=0;
	for(unsigned int i=0;i<textures.size();i++)
	{
		if(textures[i].glTarget) r++;
	}
	return r;

		}

		int RendererGL_1_1::deleteVertexBuffer(const VertexBufferID vertexBuffer)
		{
			if(vertexBuffer>=vertexBuffers.size()) return -1;
			if ((vertexBuffers[vertexBuffer]).vboVB)
			{
				glDeleteBuffersARB(1, &(vertexBuffers[vertexBuffer]).vboVB);
				checkGlError("");

				(vertexBuffers[vertexBuffer]).vboVB = 0;
			}
	int r=0;
	for(unsigned int i=0;i<vertexBuffers.size();i++)
	{
		if(vertexBuffers[i].vboVB) r++;
	}
	return r;
		}
		int RendererGL_1_1::deleteIndexBuffer(const IndexBufferID indexBuffer)
		{
			if(indexBuffer>=indexBuffers.size()) return -1;
			if ((indexBuffers[indexBuffer]).vboIB)
			{
				glDeleteBuffersARB(1, &(indexBuffers[indexBuffer]).vboIB);
				checkGlError("");

				(indexBuffers[indexBuffer]).vboIB = 0;
			}
				int r=0;
	for(unsigned int i=0;i<indexBuffers.size();i++)
	{
		if(indexBuffers[i].vboIB) r++;
	}
	return r;

		}
int RendererGL_1_1::texturePitchVrtl(const TextureID id,int level){return 0;}
BYTE *RendererGL_1_1::lockTexture(const TextureID id,int level, int& pitch){return 0;}
BYTE *RendererGL_1_1::lockVertexBuffer(const VertexBufferID id){
		return 0;}
BYTE *RendererGL_1_1::lockIndexBuffer(const IndexBufferID id){
		return 0;}
BYTE *RendererGL_1_1::unlockTextureVrtl(const TextureID id,int level){return 0;}
BYTE *RendererGL_1_1::unlockVertexBuffer(const VertexBufferID id){
		return 0;}
BYTE *RendererGL_1_1::unlockIndexBuffer(const IndexBufferID id){
		return 0;}

#if 0
HRESULT RendererGL_1_1::ProcessTexture( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return textureProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGL_1_1::LockTextureDeviceObject(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGL_1_1::opyToResourceTexture(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGL_1_1::UnLockTextureDeviceObject(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].UnLockDeviceObject();
}


HRESULT RendererGL_1_1::ProcessVertexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return vertexBufferProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGL_1_1::LockVertexBufferDeviceObject(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGL_1_1::opyToResourceVertexBuffer(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGL_1_1::UnLockVertexBufferDeviceObject(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].UnLockDeviceObject();
}


HRESULT RendererGL_1_1::ProcessIndexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return indexBufferProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGL_1_1::LockIndexBufferDeviceObject(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGL_1_1::opyToResourceIndexBuffer(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGL_1_1::UnLockIndexBufferDeviceObject(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].UnLockDeviceObject();
}

ProcessorID RendererGL_1_1::addTextureProcessor( TextureID& ppTexture, int g_SkipMips )
	{
		static int id=-1;
			CTextureProcessorGL tp(this,ppTexture);//,g_pResourceReuseCache, g_SkipMips );

			id=textureProcessors.add(tp);
		return id;
	}

ProcessorID RendererGL_1_1::addVertexBufferProcessor( VertexBufferID& ppBuffer, int iSizeBytes,   void* pData)

	{
		static int id=-1;
			CVertexBufferProcessorGL tl(this,ppBuffer);//, iSizeBytes, pData);
			id=vertexBufferProcessors.add(tl);
		return id;
	}

ProcessorID RendererGL_1_1::addIndexBufferProcessor( IndexBufferID& ppBuffer, int iSizeBytes, void* pData)
	{
		static int id=-1;
			CIndexBufferProcessorGL tl(this,ppBuffer);//, iSizeBytes,4/*???*/, pData);
			id=indexBufferProcessors.add(tl);
		return id;
	}

#endif
void RendererGL_1_1::viewport(int x, int y, int viewportWidth, int viewportHeight){
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glViewport(x, y, viewportWidth, viewportHeight);
       // changeFrontFace(GL_CW);
  m_viewport.X=x;
  m_viewport.Y=y;
  m_viewport.Width=viewportWidth;
  m_viewport.Height=viewportHeight;

}
#if 0

void	RendererGL_1_1::CreateTextureFromFile_Serial(  CHAR* szFileName,
						TextureID* ppTexture,
						//, void* pContext
						CPackedFile*		    g_PackFile,
						UINT g_SkipMips
						)
{
    LoaderID pLoader=addTextureLoader( szFileName, g_PackFile );
    ProcessorID pProcessor=addTextureProcessor( *ppTexture,// g_pResourceReuseCache
		 g_SkipMips );

    void* pLocalData;
    SIZE_T Bytes;
    if( FAILED( RendererGL_1_1::HLSLCg::LoadTexture(pLoader) ) )
	goto Error;
    if( FAILED( DecompressTexture( pLoader,&pLocalData, &Bytes ) ) )
	goto Error;
    if( FAILED( ProcessTexture( pLoader,pLocalData, Bytes ) ) )
	goto Error;
    if( FAILED( LockTextureDeviceObject(pProcessor) ) )
	goto Error;
    if( FAILED( CopyToResourceTexture(pProcessor) ) )
	goto Error;
    if( FAILED( UnLockTextureDeviceObject(pProcessor) ) )
	goto Error;

    goto Finish;

Error:
    SetProcessorResourceError(pProcessor);
Finish:
    DestroyProcessor(pProcessor);
    DestroyLoader(pLoader);
}

void	RendererGL_1_1::CreateVertexBuffer_Serial(
						VertexBufferID& ppBuffer,
						UINT iSizeBytes,
					    //DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
					    //void* pContext
						)
{
    LoaderID pLoader=addVertexBufferLoader();
    ProcessorID pProcessor=addVertexBufferProcessor( ppBuffer, iSizeBytes,  //ibFormat,
								   pData );

    void* pLocalData;
    SIZE_T Bytes;
    if( FAILED( LoadVertexBuffer(pLoader) ) )
	goto Error;
    if( FAILED( DecompressVertexBuffer(pLoader, &pLocalData, &Bytes ) ) )
	goto Error;
    if( FAILED( ProcessVertexBuffer(pProcessor, pLocalData, Bytes ) ) )
	goto Error;
    if( FAILED( LockVertexBufferDeviceObject(pProcessor) ) )
	goto Error;
    if( FAILED( CopyToResourceVertexBuffer(pProcessor) ) )
	goto Error;
    if( FAILED( UnLockVertexBufferDeviceObject(pProcessor) ) )
	goto Error;

    goto Finish;

Error:
    SetProcessorResourceError(pProcessor);
Finish:
    DestroyProcessor(pProcessor);
    DestroyLoader(pLoader);
}

void	RendererGL_1_1::CreateIndexBuffer_Serial(
						IndexBufferID& ppBuffer,
						UINT iSizeBytes,
					    //DWORD Usage, D3DFORMAT ibFormat, D3DPOOL Pool,
						void* pData
					    //void* pContext
						)
{
    LoaderID pLoader=addIndexBufferLoader();
    ProcessorID pProcessor=addIndexBufferProcessor( ppBuffer, iSizeBytes,  //ibFormat,
								   pData );

    void* pLocalData;
    SIZE_T Bytes;
    if( FAILED( LoadIndexBuffer(pLoader) ) )
	goto Error;
    if( FAILED( DecompressIndexBuffer(pLoader, &pLocalData, &Bytes ) ) )
	goto Error;
    if( FAILED( ProcessIndexBuffer(pProcessor, pLocalData, Bytes ) ) )
	goto Error;
    if( FAILED( LockIndexBufferDeviceObject(pProcessor) ) )
	goto Error;
    if( FAILED( CopyToResourceIndexBuffer(pProcessor) ) )
	goto Error;
    if( FAILED( UnLockIndexBufferDeviceObject(pProcessor) ) )
	goto Error;

    goto Finish;

Error:
    SetProcessorResourceError(pProcessor);
Finish:
    DestroyProcessor(pProcessor);
    DestroyLoader(pLoader);
}
#endif
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
float	RendererGL_1_1::GetMaxPointSpriteSize()
{
//
    // If you want to know the std::max size that a point sprite can be set
    // to, do this.
	//

    // Query for the std::max point size supported by the hardware
    float maxSize = 0.0f;
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
				checkGlError("");
    glPointSize( maxSize );
				checkGlError("");
	return maxSize;
}

void	RendererGL_1_1::BeginPointSpriteRendering()
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

    glEnable( GL_DEPTH_TEST );
				checkGlError("");
    glDepthMask( GL_FALSE );
				checkGlError("");


	glEnable( GL_BLEND );
				checkGlError("");
    glBlendFunc( GL_DST_ALPHA, GL_ONE );
				checkGlError("");

    //
	// Set up the OpenGL state machine for using point sprites...
	//

    // This is how will our point sprite's size will be modified by
    // distance from the viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
				checkGlError("");

    // Query for the std::max point size supported by the hardware
    float maxSize = 0.0f;
    glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
				checkGlError("");

    // Clamp size to 100.0f or the sprites could get a little too big on some
    // of the newer graphic cards. My ATI card at home supports a std::max point
    // size of 1024.0f!
    if( maxSize > 100.0f )
        maxSize = 100.0f;

    glPointSize( maxSize );//in OTL 10 ???
				checkGlError("");

    // The alpha of a point is calculated to allow the fading of points
    // instead of shrinking them past a defined threshold size. The threshold
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to
    // the minimum and maximum point sizes.
    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
				checkGlError("");

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
				checkGlError("");
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );
				checkGlError("");

    // Specify point sprite texture coordinate replacement mode for each
    // texture unit
    glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
				checkGlError("");

    //
	// Render point sprites...
	//

    glEnable( GL_POINT_SPRITE_ARB );
				checkGlError("");
}

void	RendererGL_1_1::EndPointSpriteRendering()
{
    //
    // Reset OpenGL states...
	//

    glDepthMask( GL_TRUE );
				checkGlError("");
    glDisable( GL_BLEND );
				checkGlError("");

	glDisable( GL_POINT_SPRITE_ARB );
				checkGlError("");
}
    BYTE* RendererGL_1_1::LockTexture(TextureID id, int m, int& pitch)
    {
		return textures[id].Lock(m,pitch);
	}
    void RendererGL_1_1::UnlockTexture(TextureID id, int m)
    {
		textures[id].Unlock(m);
	}
    BYTE* RendererGL_1_1::LockVertexBuffer(VertexBufferID id)
    {
		
		return vertexBuffers[id].Lock();
	}
    void RendererGL_1_1::UnLockVertexBuffer(VertexBufferID id)
    {
		
		vertexBuffers[id].Unlock();
	}
    BYTE* RendererGL_1_1::LockIndexBuffer(IndexBufferID id)
    {
		//
		return indexBuffers[id].Lock();
	}
    void RendererGL_1_1::UnLockIndexBuffer(IndexBufferID id)
    {
		//
		indexBuffers[id].Unlock();
	}

	void RendererGL_1_1::GetLevelDesc(TextureID id, unsigned int level,SURFACE_DESC* sDesc)
	{
		sDesc->Width=textures[id].width;
		sDesc->Height=textures[id].height;
		int i=0;
		while(i<level)
		{
		sDesc->Width = sDesc->Width >> 1;
        sDesc->Height = sDesc->Height >> 1;
		i++;
		}
	}
	unsigned int RendererGL_1_1::GetLevelCount(TextureID id)
	{
		return textures[id].mipMapLevels;
	}
	void RendererGL_1_1::GetDesc(VertexBufferID id,VERTEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=vertexBuffers[id].size;
	}
	void RendererGL_1_1::GetDesc(IndexBufferID id, INDEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=indexBuffers[id].nIndices*indexBuffers[id].indexSize;
		sDesc->Format=indexBuffers[id].indexSize;
	}

		void RendererGL_1_1::changeBlendStateVrtl(const BlendStateID blendState, const uint sampleMask)
        {
        if (blendState != currentBlendState){
            if (blendState == BS_NONE || !blendStates[blendState].blendEnable){
                if (currentBlendEnable){
                    glDisable(GL_BLEND);
				checkGlError("");
                    currentBlendEnable = false;
                }
            } else {
                if (blendStates[blendState].blendEnable){
                    if (!currentBlendEnable){
                        glEnable(GL_BLEND);
				checkGlError("");
                        currentBlendEnable = true;
                    }
                    if (blendStates[blendState].srcFactorRGB   != currentSrcFactorRGB ||
                        blendStates[blendState].dstFactorRGB   != currentDstFactorRGB ||
                        blendStates[blendState].srcFactorAlpha != currentSrcFactorAlpha ||
                        blendStates[blendState].dstFactorAlpha != currentDstFactorAlpha){

                        glBlendFuncSeparate(
                            currentSrcFactorRGB   = blendStates[blendState].srcFactorRGB,
                            currentDstFactorRGB   = blendStates[blendState].dstFactorRGB,
                            currentSrcFactorAlpha = blendStates[blendState].srcFactorAlpha,
                            currentDstFactorAlpha = blendStates[blendState].dstFactorAlpha);
				checkGlError("");
                    }
                    if (blendStates[blendState].blendModeRGB   != currentBlendModeRGB ||
                        blendStates[blendState].blendModeAlpha != currentBlendModeAlpha){
                        glBlendEquationSeparate(
                            currentBlendModeRGB   = blendStates[blendState].blendModeRGB,
                            currentBlendModeAlpha = blendStates[blendState].blendModeAlpha);
				checkGlError("");
                    }
                }
            }

            int mask = ALL;
            bool alphaToCoverage = false;
            if (blendState != BS_NONE){
                mask = blendStates[blendState].mask;
                alphaToCoverage = blendStates[blendState].alphaToCoverageEnable;
            }

            if (mask != currentMask){
                glColorMask((mask & RED) & 1, ((mask & GREEN) >> 1) & 1, ((mask & BLUE) >> 2) & 1, ((mask & ALPHA) >> 3) & 1);
				checkGlError("");

                currentMask = mask;
            }

            if (alphaToCoverage != currentAlphaToCoverageEnable)
            {
                if (alphaToCoverage)
                    {glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
				checkGlError("");}
                else
                    {glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
				checkGlError("");}

                currentAlphaToCoverageEnable = alphaToCoverage;
            }

            currentBlendState = blendState;
        }
    }

void RendererGL_1_1::changeAlphaState(const AlphaStateID blendState)
{
	if (blendState != currentAlphaState)
	{
		if (blendState == -1 || !(alphaStates[blendState]).blendEnable)
		{
			if (currentAlphaEnable)
			{
				glDisable(GL_ALPHA_TEST);
				checkGlError("");
    			currentAlphaEnable = false;
			}
		}
		else
		{
			if ((alphaStates[blendState]).blendEnable)
			{
				if (!currentAlphaEnable)
				{
					glEnable(GL_ALPHA_TEST);
				checkGlError("");
    				currentAlphaEnable = true;
    //  reference = clamp(referenceArg, 0.0f, 1.0f);
    glAlphaFunc(GL_GEQUAL, 0.5f);//???

				}
			}
		}
		currentAlphaState = blendState;

}
}

		BlendStateID RendererGL_1_1::addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask, const bool alphaToCoverage)
		{
			BlendStateGL blendState;
			blendState.srcFactorRGB   = srcFactorRGB;
			blendState.dstFactorRGB   = destFactorRGB;
			blendState.srcFactorAlpha = srcFactorAlpha;
			blendState.dstFactorAlpha = destFactorAlpha;
			blendState.blendModeRGB   = blendModeRGB;
			blendState.blendModeAlpha = blendModeAlpha;
			blendState.mask = mask;
			blendState.blendEnable = (srcFactorRGB != ONE || destFactorRGB != ZERO || srcFactorAlpha != ONE || destFactorAlpha != ZERO);
			blendState.alphaEnable = (srcFactorRGB != ONE || destFactorRGB != ZERO || srcFactorAlpha != ONE || destFactorAlpha != ZERO);
			blendState.alphaToCoverageEnable = alphaToCoverage;
			blendStates.push_back(blendState);
			return blendStates.size()-1;
		}

AlphaStateID RendererGL_1_1::addAlphaState(
		const int func,
		const float ref
	)
{
	AlphaStateGL blendState;
    blendState.blendEnable = true;
	blendState.alphaFunc=GEQUAL;
	alphaStates.push_back(blendState);
	return alphaStates.size()-1;
}
float RendererGL_1_1::getTextWidth(const FontID font, const char *str, int length) const {
	if (font < 0) return 0;
    if (length < 0) length = (int) stx_strlen(str);

	Character *chars = fonts[font].chars;

	float len = 0;
	for (int i = 0; i < length; i++){
		len += chars[(unsigned char) str[i]].ratio;
	}

	return len;
}

void RendererGL_1_1::drawArrays(const Primitives primitives, const int firstVertex, const int nVertices){
	LOG_PRINT("glDrawArrays\n");
	glDrawArrays(glPrim[primitives], firstVertex, nVertices);
				checkGlError("");

	nDrawCalls++;
}

void RendererGL_1_1::drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices){
	uint indexSize = indexBuffers[selectedIndexBuffer].indexSize;

	LOG_PRINT("glDrawElements\n");
	glDrawElements(glPrim[primitives], nIndices, indexSize == 2? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, BUFFER_OFFSET(indexSize * firstIndex));
				checkGlError("");

	nDrawCalls++;
}


		void RendererGL_1_1::changeIndexBuffer(const IndexBufferID indexBuffer)
		{
			////if (indexBuffer != currentIndexBuffer)
			{
				if (indexBuffer == IB_NONE)
				{
					glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
				checkGlError("");
				}
				else
				{
					glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, (indexBuffers[indexBuffer]).vboIB);
				checkGlError("");
				}
				currentIndexBuffer = indexBuffer;
			}
		}
#endif

