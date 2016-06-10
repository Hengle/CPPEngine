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

#include <Framework3/RendererGLES.h>
#ifdef __RendererGLES_H__

#if 0//defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define LOG_FNLN_X LOG_FNLN
#define LOG_PRINT_X LOG_PRINT_X
#else
#define LOG_FNLN_X
#define LOG_PRINT_X
#endif
		void RendererGLES::SwapBuffersGL(void)
		{
			LOG_PRINT_X("SDL_GL_SwapBuffers:\n");
#if defined(SDL_2_0_4_BUILD)// && !defined(ANDROID)
	//SDL_GL_MakeCurrent(g_SDL_window, g_SDL_context);
	SDL_GL_SwapWindow(g_SDL_window);
#elif defined(SDL_1_2_15_BUILD)
	SDL_GL_SwapBuffers();
#elif defined(_MSC_VER) && !defined(WIN_PHONE_APP)
	SwapBuffers( hDc );
#elif !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR) && !defined(WIN_PHONE_APP)
	if(!m_dpy) m_dpy=STX_Service::GetWindowInstance()->m_dpy;
	if(!m_win) m_win=STX_Service::GetWindowInstance()->m_win;
	//LOG_PRINT_X("m_dpy=%d\n", m_dpy);
	//LOG_PRINT_X("m_win=%d\n", m_win);
	glXSwapBuffers( m_dpy, m_win );
#endif
		}

#define BUFSIZE 80
#ifndef GL_ARB_point_sprite
#define GL_POINT_SPRITE_ARB               0x8861
#define GL_COORD_REPLACE_ARB              0x8862
#endif

		BYTE* IndexBufferGLES::Lock()
	{
		BYTE* m_pLockedData=0;
#if 1//defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			glBindBuffer(GL_ARRAY_BUFFER, vboIB);
   checkGlError("");

			LOG_PRINT_X("glBufferData:\n");
			glBufferData(GL_ARRAY_BUFFER,
				nIndices*indexSize,//???
				0, GL_DYNAMIC_DRAW);
   checkGlError("");
			LOG_PRINT_X("glMapBuffer:\n");
			m_pLockedData = (BYTE *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
   checkGlError("");
		#endif
		return m_pLockedData;
	}
	void IndexBufferGLES::Unlock()
	{
 // Unlock
#if 1//!defined(SDL_1_2_15_BUILD)
        glUnmapBuffer(GL_ARRAY_BUFFER);
   checkGlError("");
#endif
	}
	BYTE* VertexBufferGLES::Lock()
	{
		BYTE* m_pLockedData=0;
#if 1//defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			glBindBuffer(GL_ARRAY_BUFFER, vboVB);
   checkGlError("");
			LOG_PRINT_X("glBufferData:\n");
			glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
   checkGlError("");
			LOG_PRINT_X("glMapBuffer:\n");
			m_pLockedData = (BYTE *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
   checkGlError("");
#endif
		return m_pLockedData;
	}
	void VertexBufferGLES::Unlock()
	{
// Unlock
#if 1//!defined(SDL_1_2_15_BUILD)
		LOG_PRINT_X("glUnmapBuffer:\n");
		glUnmapBuffer(GL_ARRAY_BUFFER);
   checkGlError("");
#endif
    }
	BYTE* TextureGLES::Lock(int m, int& pitch)//int& slicePitch;
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

#if !defined( _MSC_VER) && !defined(__APPLE__) && !defined(_LINUX) && !defined(WIN_PHONE_APP) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
		LOG_PRINT_X("glBindBuffer:\n");
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_EXT, glTexID);
   checkGlError("");
		LOG_PRINT_X("glBufferData:\n");
		glBufferData(GL_PIXEL_UNPACK_BUFFER_EXT,
				img.getMipMappedSize(0, mipMapLevels),//???,
				0, GL_DYNAMIC_DRAW);
   checkGlError("");
		LOG_PRINT_X("glMapBuffer:\n");
		m_pLockedData= ((BYTE *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER_EXT, GL_WRITE_ONLY))+(img.getPixels(m)-img.getPixels(0));
   checkGlError("");//img->getDataOffset(m,format);
#endif
		pitch=img.getPitch(m);
		return m_pLockedData;
	}
	void TextureGLES::Unlock(int m)
	{
		#if !defined( _MSC_VER) && !defined(__APPLE__) && !defined(_LINUX) && !defined(WIN_PHONE_APP) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
			LOG_PRINT_X("glUnmapBuffer:\n");
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_EXT);
   checkGlError("");
//glTexImage2D    (GLenum target, GLint level, GLint internalformat,         GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
//glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,GLenum format, GLenum type, const GLvoid *pixels);
//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, src + i * size);
//glTexImage2D((textures[renderTarget]).glTarget, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
//glTexSubImage2D(GL_TEXTURE_2D, tex.glTarget,i,0,0,tex.width, tex.height,srcFormat, srcType ,0);
#endif
	}


	RendererGLES::RendererGLES(int awidth, int aheight,
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

void RendererGLES::setGLdefaults()
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
	BM_ADD		    = GL_FUNC_ADD;
	BM_SUBTRACT	    = GL_FUNC_SUBTRACT;
	BM_REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT;
#if !defined(_MSC_VER)
	BM_MIN		    = GL_MIN_EXT;
	BM_MAX		    = GL_MAX_EXT;
#endif
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
	SOLID = 0;//???GL_FILL;
	WIREFRAME = 0;//???GL_LINE;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CW);
	glPixelStorei(GL_PACK_ALIGNMENT,   1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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

void RendererGLES::setup2DMode(const float left, const float right, const float top, const float bottom){
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

void RendererGLES::setTexture(const char *textureName, const TextureID texture)
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
	}
LOG_FNLN_X;
}
const GLenum glPrim_GLES[] =
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
bool RendererGLES::supportsR2VB()
{
	return false;
}

bool RendererGLES::supportsHalf()
{
	return bsupportsHalf;
}
bool RendererGLES::supportsPS3()
{
	return false;
}
bool RendererGLES::supportsATI1N()
{
	return false;
}

void RendererGLES::changeFrontFace(const GLenum frontFace)
{
	if (frontFace != currentFrontFace)
	{
		LOG_PRINT_X("glFrontFace:\n");
		glFrontFace(currentFrontFace = frontFace);
   checkGlError("");
	}
}
void RendererGLES::setupSampler(GLenum glTarget, const SamplerStateGL &ss)
{
	LOG_PRINT_X("glTexParameteri:\n");
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, ss.wrapS);
   checkGlError("");
	//if (glTarget != GL_TEXTURE_1D) glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, ss.wrapT);
	#if defined(_LINUX) && !defined(USE_GLES_SHADERS)
	if (glTarget == GL_TEXTURE_3D) {
	LOG_PRINT_X("glTexParameteri:\n");
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_R, ss.wrapR);
   	checkGlError("");
	}
	#elif !defined(_LINUX) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
	if (glTarget == GL_TEXTURE_3D_OES) {
	LOG_PRINT_X("glTexParameteri:\n");
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_R_OES, ss.wrapR);
   	checkGlError("");
	}
	#endif
	LOG_PRINT_X("glTexParameteri:\n");
	glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, ss.magFilter);
   	checkGlError("");






	LOG_PRINT_X("glTexParameteri:\n");
	glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, ss.minFilter);
   	checkGlError("");
	if (ss.aniso > 1 && GL_EXT_texture_filter_anisotropic_supported)
	{
		LOG_PRINT_X("glTexParameteri:\n");
		glTexParameteri(glTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, ss.aniso);
   		checkGlError("");
	}
}

void RendererGLES::setTextureSlice(const char *textureName, const TextureID texture, const int slice)
{
	//ASSERT(0);
}

		void RendererGLES::resetToDefaults()
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
				checkGlError("");
			#elif 0
			currentDepthWriteEnable = true;
			glDepthMask(GL_TRUE);
				checkGlError("");
			#endif
			currentDepthWriteEnable = true;
			currentCullMode = CULL_NONE;
			currentFillMode = SOLID;
			currentMultiSampleEnable = true;
			currentScissorEnable = false;
		}
		void RendererGLES::resetVrtl(const uint flags)
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




		void RendererGLES::Clear(bool color, bool depth, D3DXFROMWINEVECTOR4 colors)
		{
			LOG_PRINT_X("Clear:\n");

#if defined(SDL_2_0_4_BUILD)
	SDL_GL_MakeCurrent(g_SDL_window, g_SDL_context);
#endif

//LOG_FNLN_X;
#if !defined(SDL_2_0_4_BUILD)
#if (!defined(QT_BUILD))
			if(color)
			{
LOG_FNLN_X;
#if 0//defined(QT_BUILD)
	qglClearColor(colors.x,colors.y,colors.z,colors.w);
   checkGlError("");
#elif 0//defined(SDL_2_0_4_BUILD)
	LOG_PRINT_X("glClearColor:\n");
	glClearColor(colors.x,colors.y,colors.z,colors.w);
   	checkGlError("");
#elif !defined(__APPLE__)
LOG_FNLN_X;
	LOG_PRINT_X("glClearColor:\n");
	glClearColor(colors.x,colors.y,colors.z,colors.w);
LOG_FNLN_X;
   	checkGlError("");
#endif
LOG_FNLN_X;

			}
LOG_FNLN_X;
			if(depth)
			{
LOG_FNLN_X;
#if !defined(USE_GLES_SHADERS)
				LOG_PRINT_X("glClearDepth:\n");
				glClearDepth(1.0f); 
   				checkGlError("");
#endif
LOG_FNLN_X;
			}
LOG_FNLN_X;
			int f=0;
LOG_FNLN_X;
			if(color) f|=GL_COLOR_BUFFER_BIT;
LOG_FNLN_X;
			if(depth) f|=GL_DEPTH_BUFFER_BIT;
LOG_FNLN_X;
			LOG_PRINT_X("glClear:\n");
			glClear(f);
   			checkGlError("");
			
#endif
#endif
LOG_FNLN_X;
		}
		void RendererGLES::PresentVrtl(bool aB)
		{
//LOG_FNLN_X;
		bool b=true;//???((aB&&(m_pInput!=0)) || ((!aB)&&(m_pInput==0)));
if(b)
{

			IRenderer::OnPresent();

			SwapBuffersGL();

		}
		}

		void RendererGLES::captureScreenshot(Image3& img)
		{
			int w=IRenderer::GetRendererInstance()->GetViewportWidth();
			int h=IRenderer::GetRendererInstance()->GetViewportHeight();
			img.create(FORMAT_RGB8, w, h, 1, 1);
			LOG_PRINT_X("glReadPixels:\n");
			glReadPixels(0, 0, w,h, GL_RGB, GL_UNSIGNED_BYTE, (BYTE *)img.getPixels());
   checkGlError("");
			img.flipY();
		}
		void RendererGLES::SaveImg(Image3& img,const char* fn)
		{
		int l=0;
        l=int(stx_strrchr(fn,'.')-fn);
		std::string fns(fn);
		fns.resize(l);
					char bufpng[256];
                    stx_snprintf(bufpng,256,"%s.png",fns.c_str());
					img.saveImageLibImage(bufpng,false);
		}

		void RendererGLES::SaveScreenShotVrtl(const char* fn)
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
                stx_snprintf(buf,256,"../%s.jpg","ScreenShot.jpg");
                stx_snprintf(gifbuf,256,"../%s.jpg","ScreenShot.gif");
			}
			captureScreenshot(img);
			//img.flipX();
			img.saveImageLibImage(buf,false);
			RendererHLSLCg::OpenILToGIF(img,gifbuf);
		}
		}


		void RendererGLES::changeVertexFormat(const VertexFormatID vertexFormat)
		{
#if 1//defined(USE_HLSL_SHADERS)
			//if (vertexFormat != currentVertexFormat)
			{
				int iii=0;
				static VertexFormatGL zero;
				VertexFormatGL curr = zero, sel = zero;
				if (currentVertexFormat != VF_NONE) curr = vertexFormats[currentVertexFormat];
				if (vertexFormat != VF_NONE) sel = vertexFormats[vertexFormat];
				if ( sel.vertex.size && !curr.vertex.size)
				{
					//glEnableClientState (GL_VERTEX_ARRAY);
					LOG_FNLN;
					LOG_PRINT("glEnableVertexAttribArray(%d)\n",iii);
					glEnableVertexAttribArray(iii);
				checkGlError("");
					iii++;
				}
				if (!sel.vertex.size &&  curr.vertex.size)
				{
					//glDisableClientState(GL_VERTEX_ARRAY);
					LOG_FNLN;
					LOG_PRINT("glDisableVertexAttribArray(%d)\n",iii);
					glDisableVertexAttribArray(iii);
				checkGlError("");
					iii++;
				}
				if ( sel.normal.size && !curr.normal.size){ //glEnableClientState (GL_NORMAL_ARRAY);
					
					LOG_FNLN;
					LOG_PRINT("glEnableVertexAttribArray(%d)\n",iii);
					glEnableVertexAttribArray(iii);
				checkGlError("");
					iii++;}
				if (!sel.normal.size &&  curr.normal.size){ //glDisableClientState(GL_NORMAL_ARRAY);
					
					LOG_FNLN;
					LOG_PRINT("glDisableVertexAttribArray(%d)\n",iii);
					glDisableVertexAttribArray(iii);
				checkGlError("");
					iii++;}
				for (int i = 0; i < MAX_GENERIC; i++)
				{
					if ( sel.generic[i].size && !curr.generic[i].size)
					{
					LOG_FNLN;
					LOG_PRINT("glEnableVertexAttribArray(%d)\n",iii);
					glEnableVertexAttribArray(iii);
				checkGlError("");
					iii++;}
					if (!sel.generic[i].size &&  curr.generic[i].size){
					LOG_FNLN;
					LOG_PRINT("glDisableVertexAttribArray(%d)\n",iii);
					glDisableVertexAttribArray(iii);
				checkGlError("");
					iii++;}
				}
				for (int i = 0; i < MAX_TEXCOORD; i++)
				{
					if ((sel.texCoord[i].size > 0) ^ (curr.texCoord[i].size > 0))
					{
						//glClientActiveTexture(GL_TEXTURE0 + i);
						if (sel.texCoord[i].size > 0)
						{
							//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							LOG_FNLN;
							LOG_PRINT("glEnableVertexAttribArray(%d)\n",iii);
							glEnableVertexAttribArray(iii);
				checkGlError("");
							iii++;
						}
						else
						{
							//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
							LOG_FNLN;
							LOG_PRINT("glDisableVertexAttribArray(%d)\n",iii);
							glDisableVertexAttribArray(iii);
				checkGlError("");
							iii++;
						}
					}
				}
				currentVertexFormat = vertexFormat;
			}
#endif
		}

		void RendererGLES::changeDepthStateVrtl(const DepthStateID depthState, const uint stencilRef)


		{
			if (depthState != currentDepthState)
			{
				if (depthState == DS_NONE)
				{
					if (!currentDepthTestEnable)

					{
						LOG_PRINT_X("glEnable:\n");
						glEnable(GL_DEPTH_TEST);
   checkGlError("");

						currentDepthTestEnable = true;
					}
					if (!currentDepthWriteEnable)
					{
						LOG_PRINT_X("glDepthMask:\n");
						glDepthMask(GL_TRUE);
   checkGlError("");
						currentDepthWriteEnable = true;
					}
					if (currentDepthFunc != LEQUAL)
					{
						LOG_PRINT_X("glDepthFunc:\n");
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
							LOG_PRINT_X("glEnable:\n");
							glEnable(GL_DEPTH_TEST);
   checkGlError("");
							currentDepthTestEnable = true;
						}
						if ((depthStates[depthState]).depthWrite != currentDepthWriteEnable)
						{
							LOG_PRINT_X("glDepthMask:\n");
							glDepthMask((currentDepthWriteEnable = (depthStates[depthState]).depthWrite)? GL_TRUE : GL_FALSE);
   checkGlError("");
						}
						if ((depthStates[depthState]).depthFunc != currentDepthFunc)
						{
							LOG_PRINT_X("glDepthFunc:\n");
							glDepthFunc(currentDepthFunc = (depthStates[depthState]).depthFunc);
   checkGlError("");
						}
					}
					else
					{
						if (currentDepthTestEnable)
						{
							LOG_PRINT_X("glDisable:\n");
							glDisable(GL_DEPTH_TEST);
   checkGlError("");
							currentDepthTestEnable = false;
						}
					}
				}
				currentDepthState = depthState;
			}
		}
		void RendererGLES::changeRasterizerState(const RasterizerStateID rasterizerState)
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
						LOG_PRINT_X("glDisable:\n");
						glDisable(GL_CULL_FACE);
   checkGlError("");
					}

					else
					{
						if (currentCullMode == CULL_NONE) {glEnable(GL_CULL_FACE);
   checkGlError("");}
						LOG_PRINT_X("glCullFace:\n");
						glCullFace(state.cullMode);
   checkGlError("");
					}
					currentCullMode = state.cullMode;
				}
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
					glPolygonMode(GL_FRONT_AND_BACK, currentFillMode = state.fillMode);
   checkGlError("");
				}
				if (state.multiSample != currentMultiSampleEnable)
				{
					if (state.multiSample)
					{
						LOG_PRINT_X("glEnable:\n");
						glEnable(GL_MULTISAMPLE);
   checkGlError("");
					}
					else
					{
						LOG_PRINT_X("glDisable:\n");
						glDisable(GL_MULTISAMPLE);
   checkGlError("");
					}
					currentMultiSampleEnable = state.multiSample;
				}
#endif
				if (state.scissor != currentScissorEnable)
				{
					if (state.scissor)
					{
						LOG_PRINT_X("glEnable:\n");
						glEnable(GL_SCISSOR_TEST);
   checkGlError("");
					}
					else
					{
						LOG_PRINT_X("glDisable:\n");
						glDisable(GL_SCISSOR_TEST);
   checkGlError("");
					}
					currentScissorEnable = state.scissor;
				}
				currentRasterizerState = rasterizerState;
			}
		}




		VertexFormatID RendererGLES::addVertexFormatVrtl(const FormatDesc *formatDesc, const uint nAttribs, const ShaderID shader)
		{
#if 0//defined(USE_HLSL_SHADERS)
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
#elif !defined(__APPLE__)
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
						LOG_PRINT_X("TYPE_GENERIC:\n");
					break;
					case TYPE_TANGENT:
						LOG_PRINT_X("TYPE_TANGENT:\n");
					break;
					case TYPE_BINORMAL:
						LOG_PRINT_X("TYPE_BINORMAL:\n");
					break;
					case TYPE_VERTEX:
						LOG_PRINT_X("TYPE_VERTEX:\n");
					break;
					case TYPE_NORMAL:
						LOG_PRINT_X("TYPE_NORMAL:\n");
					break;
					case TYPE_TEXCOORD:
						LOG_PRINT_X("TYPE_TEXCOORD:\n");
					break;
				};

				switch (formatDesc[i].type)
				{
					case TYPE_GENERIC:
					case TYPE_TANGENT:
					case TYPE_BINORMAL:

					case TYPE_VERTEX:
					//if(vertexFormat.pBase==0) vertexFormat.pBase=&vertexBuffers[vertexFormat.vertex.stream].data[0];
					case TYPE_NORMAL:
					case TYPE_TEXCOORD:
					if(formatDesc[i].type==TYPE_TEXCOORD) nTexCoord++;


					vertexFormat.generic[nGeneric].stream = stream;
					vertexFormat.generic[nGeneric].size   = formatDesc[i].size;
					vertexFormat.generic[nGeneric].offset = vertexFormat.vertexSize[stream];
					vertexFormat.generic[nGeneric].format = formatDesc[i].format;

LOG_PRINT_X("nGeneric=%d\n", nGeneric);
LOG_PRINT_X("stream  =%d\n", stream);
LOG_PRINT_X("size    =%d\n", formatDesc[i].size);
LOG_PRINT_X("offset  =%d\n", vertexFormat.vertexSize[stream]);
LOG_PRINT_X("format  =%d\n", formatDesc[i].format);

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
				LOG_PRINT_X("vertexSize=%d\n", vertexFormat.vertexSize[stream]);
			}
			vertexFormat.shader=shader;
			vertexFormat.maxGeneric = nGeneric;
			vertexFormat.maxTexCoord = nTexCoord;
			vertexFormats.push_back(vertexFormat);
			return vertexFormats.size()-1;
#endif
		}
GLenum usages_GLES[] =
		{
			GL_STATIC_DRAW,
			GL_DYNAMIC_DRAW,
			GL_STREAM_DRAW,
		}
		;
		//STXArray<GLenum,3> GLES::usages(_GLES::usages);
		VertexBufferID RendererGLES::addVertexBufferVrtl(const long size, const BufferAccess bufferAccess, const void *data, VertexBufferID aold)//, const IDataProcessor* p)
		{
LOG_FNLN;
LOG_PRINT("size=%d\n", size);
LOG_PRINT("data=%x\n", data);
LOG_PRINT("aold=%d\n", aold);
//////
if(aold>-1)
{
//////
	LOG_FNLN;
	LOG_PRINT("glDeleteBuffers:\n");
	glDeleteBuffers(1,  &vertexBuffers[aold].vboVB);
   	checkGlError("");
//
//////
	vertexBuffers[aold].vboVB=0;
	vertexBuffers[aold].pVertices=(BYTE*)data;
	LOG_FNLN;
	LOG_PRINT("glGenBuffers:\n");
	glGenBuffers(1, &vertexBuffers[aold].vboVB);
   	checkGlError("");
	
	vertexBuffers[aold].size=size;
//////

	LOG_FNLN;
	LOG_PRINT("glBindBuffer:\n");
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[aold].vboVB);
  	 checkGlError("");
	

}
	VertexBufferGLES vb;
//////
if(aold==-1)
{
	vb.size = size;
/*
LOG_PRINT_X("(void*)&vb.data[0]=%x\n",(void*)&vb.data[0]);
LOG_PRINT_X("data=%x\n",data);
LOG_PRINT_X("size=%x\n",size);
*/
	GLuint vbb;
//////
	LOG_FNLN;
	LOG_PRINT("glGenBuffers:\n");
	glGenBuffers
//ARB
(1, &vbb);
   checkGlError("");
	
	vb.vboVB=vbb;
	vb.pVertices=(BYTE*)data;
	vertexBuffers.push_back(vb);
//////


	LOG_FNLN;
	LOG_PRINT("glBindBuffer:\n");
	glBindBuffer(GL_ARRAY_BUFFER, vbb);
   	checkGlError("");
	

}
//////
	if(data>=0) {
/////
	//vb.data.resize(size);
	//stx_memcpy((void*)&vb.data[0],data,size);//ABC123
	LOG_FNLN;
	LOG_PRINT("glBufferData: size=%d\n", size);
	LOG_PRINT("glBufferData: data=%x\n", data);
	glBufferData(GL_ARRAY_BUFFER, size, data, usages_GLES[bufferAccess]);
   	checkGlError("");
	
}
//////

	LOG_FNLN;
	LOG_PRINT("glBindBuffer:\n");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
   	checkGlError("");
	

//////
/*
LOG_PRINT_X("vbb=%d\n",vbb);
LOG_PRINT_X("size=%d\n",size);
LOG_PRINT_X("data=%x\n",data);
*/


			if(aold==-1)
			{

				//LOG_PRINT_X("vbid=%d\n",vertexBuffers.size()-1);
				return vertexBuffers.size()-1;
			}
			else if((aold>=0)&&(aold<vertexBuffers.size()))
{
//LOG_PRINT_X("vbid=%d\n",old);
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
						glBindBuffer(GL_ARRAY_BUFFER, currentVBO = 0);
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
//LOG_PRINT_X("vbid=%d\n",-1);

			//DBG_HALT;
			return -1;
}
		}
		IndexBufferID RendererGLES::addIndexBufferVrtl(const uint nIndices, const uint indexSize, const BufferAccess bufferAccess, const void *data, IndexBufferID aold)//, const IDataProcessor* p)
		{
LOG_FNLN_X;
LOG_PRINT_X("nIndices=%d\n", nIndices);
LOG_PRINT_X("indexSize=%d\n", indexSize);
LOG_PRINT_X("data=%x\n", data);
LOG_PRINT_X("aold=%d\n", aold);

			uint size = nIndices * indexSize;
//////
if(aold>-1)
{
//////
	LOG_PRINT_X("glDeleteBuffers:\n");
	glDeleteBuffers(1,  &indexBuffers[aold].vboIB);
   	checkGlError("");
//
//////
	indexBuffers[aold].vboIB=0;
	LOG_PRINT_X("glGenBuffers:\n");
	glGenBuffers(1, &indexBuffers[aold].vboIB);
   	checkGlError("");
	
//////

	LOG_PRINT_X("glBindBuffer:\n");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[aold].vboIB);
   	checkGlError("");
	

//////
	indexBuffers[aold].nIndices = nIndices;
	indexBuffers[aold].indexSize = indexSize;
	indexBuffers[aold].pIndices=(BYTE*)data;
}
	IndexBufferGLES ib;
//////
if(aold==-1)
{
	GLuint vbb;

	ib.nIndices = nIndices;
	ib.indexSize = indexSize;;
	ib.pIndices=(BYTE*)data;
//////
	LOG_PRINT_X("glGenBuffers:\n");
	glGenBuffers(1, &vbb);
   	checkGlError("");
	
	ib.vboIB=vbb;
//////

	LOG_PRINT_X("glBindBuffer:\n");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbb);
   	checkGlError("");
	

//////
	indexBuffers.push_back(ib);
}
//////
//if(old>-1)
	//indexBuffers[old].size=size;

	if(data>=0) {
//////
	//ib.data.resize(size);
	//stx_memcpy((void*)&ib.data[0],data,size);//ABC123
	LOG_FNLN;
	LOG_PRINT("glBufferData: size=%d\n", size);
	LOG_PRINT("glBufferData: data=%d\n", data);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usages_GLES[bufferAccess]);
   	checkGlError("");
	
}
//////

	LOG_PRINT_X("glBindBuffer:\n");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   	checkGlError("");
	

//////
			if(aold==-1)
			{

				//LOG_PRINT_X("vbid=%d\n",vertexBuffers.size()-1);
				return indexBuffers.size()-1;
			}
			else if((aold>=0)&&(aold<indexBuffers.size()))
{
return aold;
}
else
	return -1;
/*

			glGenBuffers(1, &ib.vboIB);
				checkGlError("");
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.vboIB);
				checkGlError("");
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GLES::usages[bufferAccess]);
				checkGlError("");
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
		GLint minFilters_GLES[] =
		{
			GL_NEAREST,
			GL_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR,
		}
		;
		SamplerStateID RendererGLES::addSamplerStateVrtl(const Filter filter, const AddressMode s, const AddressMode t, const AddressMode r, const float lod, const uint maxAniso, const int compareFunc, const float *border_color)
		{
			if (compareFunc != 0 || border_color != 0) return SS_NONE;
			SamplerStateGL samplerState;
			samplerState.minFilter = minFilters_GLES[filter];
			samplerState.magFilter = (filter == NEAREST)? GL_NEAREST : GL_LINEAR;
			samplerState.wrapS = (s == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.wrapT = (t == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.wrapR = (r == WRAP)? GL_REPEAT : GL_CLAMP_TO_EDGE;
			samplerState.aniso = hasAniso(filter)? min((uint) maxAnisotropic, maxAniso) : 1;
			samplerState.lod = lod;
			samplerStates.push_back(samplerState);
			return samplerStates.size()-1;
		}
		DepthStateID RendererGLES::addDepthState(const bool depthTest, const bool depthWrite, const int depthFunc, const bool stencilTest,
		const uint8 stencilMask, const int stencilFunc, const int stencilFail, const int depthFail, const int stencilPass)
		{
			DepthStateGL depthState;
			depthState.depthTest  = depthTest;
			depthState.depthWrite = depthWrite;
			depthState.depthFunc  = depthFunc;
			depthStates.push_back(depthState);
			return depthStates.size()-1;
		}
		RasterizerStateID RendererGLES::addRasterizerStateVrtl(const int cullMode, const int fillMode, const bool multiSample, const bool scissor)
		{
			RasterizerStateGL rasterizerState;
			rasterizerState.cullMode = cullMode;
			rasterizerState.fillMode = fillMode;
			rasterizerState.multiSample = multiSample;
			rasterizerState.scissor = scissor;
			rasterizerStates.push_back(rasterizerState);
			return rasterizerStates.size()-1;
		}
		static const GLint internalFormats_GLES[] =
		{

			0,
			0,//???GL_INTENSITY8,
			GL_LUMINANCE_ALPHA,
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			GL_RGB8,
			GL_RGBA8,
#elif defined(SDL_2_0_4_BUILD)// || defined(USE_GLES_SHADERS)
			GL_RGB8_OES,
			GL_RGBA8_OES,
#elif defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
			GL_RGB,
			GL_RGBA,
#else
			GL_RGB8,
			GL_RGBA8,
#endif
			0,//???GL_INTENSITY16,
			0,//???GL_LUMINANCE16_ALPHA16,
			0,//???GL_RGB16,
			0,//???GL_RGBA16,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,//???GL_INTENSITY_FLOAT16_ATI,
			0,//???GL_LUMINANCE_ALPHA_FLOAT16_ATI,
			0,//???GL_RGB_FLOAT16_ATI,
			0,//???GL_RGBA_FLOAT16_ATI,
			0,//???GL_INTENSITY_FLOAT32_ATI,
			0,//???GL_LUMINANCE_ALPHA_FLOAT32_ATI,
			0,//???GL_RGB_FLOAT32_ATI,
			0,//???GL_RGBA_FLOAT32_ATI,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//???GL_RGB5,
			GL_RGBA4,
			0,//???GL_RGB10_A2,
			0,//???GL_DEPTH_COMPONENT16,
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			GL_DEPTH_COMPONENT24,
			GL_DEPTH24_STENCIL8,
#elif !defined(_LINUX) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
			GL_DEPTH_COMPONENT24_OES,
			GL_DEPTH24_STENCIL8_OES,
#else
			0,
			0,
#endif
			0,
#if !defined(_MSC_VER) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
			GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
#else
			0,
#endif
			0,//???GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
			0,//???GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
			0,
#ifdef _MSC_VER
GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI,
#else
0,
#endif
		}
		;
		static const GLenum srcFormats_GLES[] =
		{
			0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA
		}
		;
		static const GLenum srcTypes_GLES[] =
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
			0, 0, 0, 0,//???GL_UNSIGNED_SHORT_5_6_5,
			0,//???GL_UNSIGNED_SHORT_4_4_4_4_REV,
			0,//???GL_UNSIGNED_INT_2_10_10_10_REV,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_INT,
#if defined(_LINUX) && !defined(USE_GLES_SHADERS)
			GL_UNSIGNED_INT_24_8,
#elif !defined(_LINUX) && !defined(__APPLE__)
			GL_UNSIGNED_INT_24_8_OES,
#else
			0,
#endif
			0, 0,
			0,
			0,
			0,
			0,
		}
		;

		static const char* internalFormats_GLES_[] =
		{

			0,
			0,//???"GL_INTENSITY8",
			"GL_LUMINANCE_ALPHA",
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			"GL_RGB8",
			"GL_RGBA8",
#elif defined(SDL_2_0_4_BUILD)// || defined(USE_GLES_SHADERS)
			"GL_RGB8_OES",
			"GL_RGBA8_OES",
#elif defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
			"GL_RGB",
			"GL_RGBA",
#else
			"GL_RGB8",
			"GL_RGBA8",
#endif
			0,//???GL_INTENSITY16,
			0,//???GL_LUMINANCE16_ALPHA16,
			0,//???GL_RGB16,
			0,//???GL_RGBA16,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,//???GL_INTENSITY_FLOAT16_ATI,
			0,//???GL_LUMINANCE_ALPHA_FLOAT16_ATI,
			0,//???GL_RGB_FLOAT16_ATI,
			0,//???GL_RGBA_FLOAT16_ATI,
			0,//???GL_INTENSITY_FLOAT32_ATI,
			0,//???GL_LUMINANCE_ALPHA_FLOAT32_ATI,
			0,//???GL_RGB_FLOAT32_ATI,
			0,//???GL_RGBA_FLOAT32_ATI,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//???GL_RGB5,
			"GL_RGBA4",
			0,//???GL_RGB10_A2,
			0,//???GL_DEPTH_COMPONENT16,
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
			"GL_DEPTH_COMPONENT24",
			"GL_DEPTH24_STENCIL8",
#elif !defined(_LINUX) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
			"GL_DEPTH_COMPONENT24_OES",
			"GL_DEPTH24_STENCIL8_OES",
#else
			0,
			0,
#endif
			0,
#if !defined(_MSC_VER) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
			"GL_COMPRESSED_RGB_S3TC_DXT1_EXT",
#else
			0,
#endif
			0,//???GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
			0,//???GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
			0,
#ifdef _MSC_VER
"GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI",
#else
0,
#endif
		}
		;
		static const char* srcFormats_GLES_[] =
		{
			0, "GL_LUMINANCE", "GL_LUMINANCE_ALPHA", "GL_RGB", "GL_RGBA"
		}
		;
		static const char* srcTypes_GLES_[] =
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
			0, 0, 0, 0,//???GL_UNSIGNED_SHORT_5_6_5,
			0,//???GL_UNSIGNED_SHORT_4_4_4_4_REV,
			0,//???GL_UNSIGNED_INT_2_10_10_10_REV,
			"GL_UNSIGNED_SHORT",
			"GL_UNSIGNED_INT",
#if defined(_LINUX) && !defined(USE_GLES_SHADERS)
			"GL_UNSIGNED_INT_24_8",
#elif !defined(_LINUX) && !defined(__APPLE__)
			"GL_UNSIGNED_INT_24_8_OES",
#else
			0,
#endif
			0, 0,
			0,
			0,
			0,
			0,
		}
		;

		void RendererGLES::setSamplerState(const char *samplerName, const SamplerStateID samplerState)
		{
		}
		void RendererGLES::applySamplerStates()
		{
		}


int RendererGLES::GetTextureWidth(TextureID id)
{
	assert(id<textures.size());
	return textures[id].width;
}
int RendererGLES::GetTextureHeight(TextureID id)
{
	assert(id<textures.size());
	return textures[id].height;
}

		TextureID RendererGLES::addTexture(Image3* img, const bool useMips, const SamplerStateID samplerState, uint flags, float3 alpha)
		{
			//ASSERT(samplerState != SS_NONE);
			
			TextureGLES tex;
			
			memset(&tex, 0, sizeof(TextureGLES));
			
	//img->BindImage();
	tex.ss=samplerState;
	tex.width=img->getWidth();
	tex.height=img->getHeight();
	tex.depth=img->getDepth();
			
	if(useMips&&img->getMipMapCount()<2)
		img->createMipMaps();
			
	tex.mipMapLevels=img->getMipMapCount();
	const FORMAT format = img->getFormat();
	tex.format=format;
			
#if 0
					if(alpha>0.0f)
					{
#if 1
						if(getChannelCount(format)==4){img->removeChannels(true,true,true,false);}
						img->addChannel(alpha);
#elif !defined(__APPLE__)
						img->format(FORMAT_RGBA8);
						img->setAlpha(alpha);
						format = img->getFormat();
#endif
					}
#endif
			GLenum srcFormat = srcFormats_GLES[getChannelCount(format)];
			GLenum srcType = srcTypes_GLES[format];
			GLint internalFormat = internalFormats_GLES[format];
//???endif
			const char* srcFormat_ = srcFormats_GLES_[getChannelCount(format)];
			const char* srcType_ = srcTypes_GLES_[format];
			const char* internalFormat_ = internalFormats_GLES_[format];
			
			if ((flags & HALF_FLOAT) != 0 && format >= FORMAT_I32f && format <= FORMAT_RGBA32f)
			{
				internalFormat = internalFormats_GLES[format - (FORMAT_I32f - FORMAT_I16f)];
			}
			
			tex.lod = (samplerStates[samplerState]).lod;
			//tex.glTarget = img->isCube()? GL_TEXTURE_CUBE_MAP : img->is3D()? GL_TEXTURE_3D : img->is2D()? GL_TEXTURE_2D : 0;//???GL_TEXTURE_1D;
			tex.glTarget = img->is2D()? GL_TEXTURE_2D : 0;//???GL_TEXTURE_1D;
			LOG_PRINT_X("glGenTextures:\n");
			
			glGenTextures(1, &tex.glTexID);
   checkGlError("");
			
			//LOG_PRINT_X("tex.glTexID=%d\n",tex.glTexID);
			LOG_PRINT_X("glBindTexture:\n");
			glBindTexture(tex.glTarget, tex.glTexID);
   checkGlError("");
			
			setupSampler(tex.glTarget, samplerStates[samplerState]);
			
			tex.samplerState = samplerState;

			const ubyte *src;
			int mipMapLevel = 0;
////LOG_FNLN_X
			
			while ((src = (useMips? img->getPixels(mipMapLevel) : img->getPixels())) != 0)
			{
			
				//img->BindImage(mipMapLevel);
				if (img->isCube())
				{
			
					int size = img->getMipMappedSize(mipMapLevel, 1) / 6;
					for (uint i = 0; i < 6; i++)
					{
			
						if (isCompressedFormat(format))
						{
							LOG_PRINT_X("glCompressedTexImage2D:\n");
							glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, size, (BYTE *)(src + i * size));
   checkGlError("");
						}
						else
						{
LOG_FNLN;
LOG_PRINT("glTexImage2D:\n");
LOG_PRINT("glTarget=%d\n", GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
LOG_PRINT("mipMapLevel=%d\n", mipMapLevel);
LOG_PRINT("internalFormat=%s\n", internalFormat_);
LOG_PRINT("WIDTH=%d\n", img->getWidth(mipMapLevel));
LOG_PRINT("HEIGHT=%d\n", img->getHeight(mipMapLevel));
LOG_PRINT("srcFormat=%s\n", srcFormat_);
LOG_PRINT("srcType=%s\n", srcType_);
LOG_PRINT("data=%x\n", (BYTE *)(src + i * size));
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, (BYTE *)(src + i * size));
   checkGlError("");
						}
			
					}
			
				}
				else if (img->is3D())
				{
			
					if (isCompressedFormat(format))
					{
#if !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
						LOG_PRINT_X("glCompressedTexImage3DOES:\n");
						glCompressedTexImage3DOES(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), img->getDepth(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), (BYTE *)(src));
   checkGlError("");
#endif
					}
					else
					{
			
#if !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
						LOG_PRINT_X("glTexImage3D:\n");
						glTexImage3D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), img->getDepth(mipMapLevel), 0, srcFormat, srcType, (BYTE *)(src));
   checkGlError("");
#endif
			
					}
			
				}
				else if (img->is2D())

				{
			
					if (isCompressedFormat(format))
					{
						LOG_PRINT_X("glCompressedTexImage2D:\n");
						glCompressedTexImage2D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, img->getMipMappedSize(mipMapLevel, 1), (BYTE *)(src));
   checkGlError("");
					}
					else
					{
LOG_FNLN;
LOG_PRINT("glTexImage2D:\n");
LOG_PRINT("glTarget=%d\n", tex.glTarget);
LOG_PRINT("mipMapLevel=%d\n", mipMapLevel);
LOG_PRINT("internalFormat=%s\n", internalFormat_);
LOG_PRINT("WIDTH=%d\n", img->getWidth(mipMapLevel));
LOG_PRINT("HEIGHT=%d\n", img->getHeight(mipMapLevel));
LOG_PRINT("srcFormat=%s\n", srcFormat_);
LOG_PRINT("srcType=%s\n", srcType_);
LOG_PRINT("data=%x\n", (BYTE *)src);

						glTexImage2D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), img->getHeight(mipMapLevel), 0, srcFormat, srcType, (BYTE *)(src));
   checkGlError("");
	LOG_FNLN;
					}
				}/*
				else
				{
					glTexImage1D(tex.glTarget, mipMapLevel, internalFormat, img->getWidth(mipMapLevel), 0, srcFormat, srcType, (BYTE *)(src));
				checkGlError("");
				}*/
			
				if(!useMips)
					break;

				mipMapLevel++;
			}
////LOG_FNLN_X
			
			LOG_PRINT_X("glBindTexture:\n");
			glBindTexture(tex.glTarget, 0);
				checkGlError("");
			
////LOG_FNLN_X
			return textures.add(tex);
		}
		TextureID RendererGLES::addRenderTargetVrtl(const int width, const int height, const int depth, const int mipMapCount, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags)
		{
			if (isCompressedFormat(format) || arraySize > 1) return false;
			if (mipMapCount > 1) return false;
			TextureGLES tex;
			memset(&tex, 0, sizeof(TextureGLES));
			tex.glTarget = (flags & CUBEMAP)? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
			tex.flags  = flags;
			tex.format = format;
			LOG_PRINT_X("glGenTextures:\n");
			glGenTextures(1, &tex.glTexID);
   checkGlError("");
			LOG_PRINT_X("glBindTexture:\n");
			glBindTexture(tex.glTarget, tex.glTexID);
   checkGlError("");
			setupSampler(tex.glTarget, samplerStates[samplerState]);
			tex.samplerState = samplerState;
			TextureID rt = textures.add(tex);
			setRenderTargetSize(rt, width, height);
			LOG_PRINT_X("glBindTexture:\n");
			glBindTexture(tex.glTarget, 0);
   checkGlError("");
			return rt;
		}
		TextureID RendererGLES::addRenderDepthVrtl(const int width, const int height, const int arraySize, const FORMAT format, const int msaaSamples, const SamplerStateID samplerState, uint flags)
		{
			if (arraySize > 1) return false;
			TextureGLES tex;
			memset(&tex, 0, sizeof(TextureGLES));
			tex.glTarget = GL_RENDERBUFFER;
			tex.format = format;
			LOG_PRINT_X("glGenRenderbuffers:\n");
			glGenRenderbuffers(1, &tex.glDepthID);
   checkGlError("");
			TextureID rt = textures.add(tex);
			setRenderTargetSize(rt, width, height);
			return rt;
		}
		void RendererGLES::setRenderTargetSize(const TextureID renderTarget, const int width, const int height)
		{
			(textures[renderTarget]).width	= width;
			(textures[renderTarget]).height = height;
			if ((textures[renderTarget]).glTarget == GL_RENDERBUFFER)
			{
				FORMAT format = (textures[renderTarget]).format;
				LOG_PRINT_X("glBindRenderbuffer:\n");
				glBindRenderbuffer(GL_RENDERBUFFER, (textures[renderTarget]).glDepthID);
   checkGlError("");
				LOG_PRINT_X("glRenderbufferStorage:\n");
				glRenderbufferStorage(GL_RENDERBUFFER, internalFormats_GLES[format], width, height);
   checkGlError("");
				LOG_PRINT_X("glBindRenderbuffer:\n");
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
   checkGlError("");
			}
			else
			{
				FORMAT format = (textures[renderTarget]).format;
				GLint internalFormat = internalFormats_GLES[format];
				GLenum srcFormat = srcFormats_GLES[getChannelCount(format)];
				GLenum srcType = srcTypes_GLES[format];

				const char* internalFormat_ = internalFormats_GLES_[format];
				const char* srcFormat_ = srcFormats_GLES_[getChannelCount(format)];
				const char* srcType_ = srcTypes_GLES_[format];

				if (isDepthFormat(format))
				{
					if (isStencilFormat(format))
					{
						#if defined(_LINUX) && !defined(USE_GLES_SHADERS)
						srcFormat = GL_DEPTH_STENCIL;
                        			#elif !defined(_LINUX) && !defined(__APPLE__)
						srcFormat = GL_DEPTH_STENCIL_OES;
						#else
						srcFormat = 0;
						#endif
					}
					else
					{
						srcFormat = GL_DEPTH_COMPONENT;


					}
				}

				if (isFloatFormat(format)) srcType = GL_FLOAT;
				LOG_PRINT_X("glBindTexture:\n");
				glBindTexture((textures[renderTarget]).glTarget, (textures[renderTarget]).glTexID);
   checkGlError("");
				if ((textures[renderTarget]).flags & CUBEMAP)
				{
					for (int i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; i++){
LOG_FNLN;
LOG_PRINT("glTexImage2D:\n");
LOG_PRINT("glTarget=%d\n", i);
LOG_PRINT("mipMapLevel=%d\n", 0);
LOG_PRINT("internalFormat=%s\n", internalFormat_);
LOG_PRINT("WIDTH=%d\n", width);
LOG_PRINT("HEIGHT=%d\n", height);
LOG_PRINT("srcFormat=%s\n", srcFormat_);
LOG_PRINT("srcType=%s\n", srcType_);
LOG_PRINT("data=%x\n", 0);
					glTexImage2D(i, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
   checkGlError("");}
				}
				else
				{
LOG_FNLN;
LOG_PRINT("glTexImage2D:\n");
LOG_PRINT("glTarget=%d\n", (textures[renderTarget]).glTarget);
LOG_PRINT("mipMapLevel=%d\n", 0);
LOG_PRINT("internalFormat=%s\n", internalFormat_);
LOG_PRINT("WIDTH=%d\n", width);
LOG_PRINT("HEIGHT=%d\n", height);
LOG_PRINT("srcFormat=%s\n", srcFormat_);
LOG_PRINT("srcType=%s\n", srcType_);
LOG_PRINT("data=%x\n", 0);
					glTexImage2D((textures[renderTarget]).glTarget, 0, internalFormat, width, height, 0, srcFormat, srcType, 0);
   checkGlError("");
				}
				LOG_PRINT_X("glBindTexture:\n");
				glBindTexture((textures[renderTarget]).glTarget, 0);
   checkGlError("");
			}
		}
		bool RendererGLES::resizeRenderTarget(const TextureID renderTarget, const int width, const int height, const int depth, const int mipMapCount, const int arraySize)
		{
			if (depth > 1 || arraySize > 1 || mipMapCount > 1) return false;
			if (width != (textures[renderTarget]).width || height != (textures[renderTarget]).height)
			{
				setRenderTargetSize(renderTarget, width, height);
			}
			return true;
		}
		bool RendererGLES::generateMipMaps(const TextureID renderTarget)
		{
			return false;
		}
		void RendererGLES::changeRenderTargetsVrtl(const TextureID *colorRTs, const uint nRenderTargets, const TextureID depthRT, const int depthSlice, const int *slices)
		{
			apply();
			if (fbo == 0) glGenFramebuffers(1, &fbo);
			LOG_PRINT_X("glBindFramebuffer:\n");
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   checkGlError("");
			for (uint i = 0; i < nRenderTargets; i++)
			{
				TextureID colorRT = colorRTs[i];
				if ((textures[colorRT]).flags & CUBEMAP)
				{
					if (colorRT != currentColorRT[i] || currentColorRTSlice[i] != slices[i])
					{
						LOG_PRINT_X("glFramebufferTexture2D:\n");
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + slices[i], (textures[colorRT]).glTexID, 0);
   checkGlError("");
						currentColorRTSlice[i] = slices[i];
					}
				}
				else
				{
					if (colorRT != currentColorRT[i])
					{
						LOG_PRINT_X("glFramebufferTexture2D:\n");
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (textures[colorRT]).glTexID, 0);
   checkGlError("");
						currentColorRT[i] = colorRT;
					}
				}
			}
			if (nRenderTargets != nCurrentRenderTargets)
			{
				for (uint i = nRenderTargets; i < nCurrentRenderTargets; i++)
				{
					LOG_PRINT_X("glFramebufferTexture2D:\n");
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
   checkGlError("");
					currentColorRT[i] = TEXTURE_NONE;
					currentColorRTSlice[i] = NO_SLICE;
				}
#if defined(SDL_1_2_15_BUILD) && !defined(USE_GLES_SHADERS)
					glDrawBuffer(GL_NONE);
   checkGlError("");
					LOG_PRINT_X("glDrawBuffer:\n");
					glReadBuffer(GL_NONE);
   checkGlError("");
				}
				else
				{
					LOG_PRINT_X("glDrawBuffers:\n");
					glDrawBuffers(nRenderTargets, drawBuffers);
   checkGlError("");
					LOG_PRINT_X("glReadBuffer:\n");
					glReadBuffer(GL_COLOR_ATTACHMENT0);
   checkGlError("");
				}
#endif
				nCurrentRenderTargets = nRenderTargets;
			}
			if (depthRT != currentDepthRT)
			{
				if (depthRT >= 0 && (textures[depthRT]).glTarget != GL_RENDERBUFFER)
				{
					LOG_PRINT_X("glFramebufferTexture2D:\n");
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (textures[depthRT]).glTexID, 0);
   checkGlError("");
					if (isStencilFormat((textures[depthRT]).format))
					{
						LOG_PRINT_X("glFramebufferTexture2D:\n");
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, (textures[depthRT]).glTexID, 0);
   checkGlError("");
					}
					else
					{
						LOG_PRINT_X("glFramebufferTexture2D:\n");
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
   checkGlError("");
					}
				}
				else
				{
					LOG_PRINT_X("glFramebufferRenderbuffer:\n");
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, (depthRT < 0)? 0 : (textures[depthRT]).glTexID);
   checkGlError("");
					if (depthRT >= 0 && isStencilFormat((textures[depthRT]).format))
					{
						LOG_PRINT_X("glFramebufferRenderbuffer:\n");
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, (textures[depthRT]).glTexID);
   checkGlError("");
					}
					else
					{
						LOG_PRINT_X("glFramebufferRenderbuffer:\n");
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
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
				LOG_PRINT_X("glViewport:\n");
				glViewport(0, 0, (textures[colorRTs[0]]).width, (textures[colorRTs[0]]).height);
   checkGlError("");
			}
			else if(depthRT >= 0)
			{
				changeFrontFace(GL_CW);
				LOG_PRINT_X("glViewport:\n");
				glViewport(0, 0, (textures[depthRT]).width, (textures[depthRT]).height);
   checkGlError("");
			}
			reset(0);
		}
		void RendererGLES::changeToMainFramebuffer()
		{
			LOG_PRINT_X("glBindFramebuffer:\n");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
   checkGlError("");
			LOG_PRINT_X("glViewport:\n");
			glViewport(m_viewport.X,m_viewport.Y, m_viewport.Width, m_viewport.Height);
   checkGlError("");
			changeFrontFace(GL_CW);
		}
		void RendererGLES::Init0(void)
		{
	if(defaultFont == -1)
	{
		//defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);
	}

					}

UINT32 RendererGLES::GetAvailableTextureMem( )
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

		int RendererGLES::removeTexture(const TextureID texture)
		{
			if(texture>=textures.size()) return -1;
			if ((textures[texture]).glTarget)
			{
				if ((textures[texture]).glTarget == GL_RENDERBUFFER)
				{
					LOG_PRINT_X("glDeleteRenderbuffers:\n");
					glDeleteRenderbuffers(1, &(textures[texture]).glDepthID);
   checkGlError("");
				}
				else
				{
					LOG_PRINT_X("glDeleteTextures:\n");
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

		int RendererGLES::deleteVertexBuffer(const VertexBufferID vertexBuffer)
		{
			if(vertexBuffer>=vertexBuffers.size()) return -1;
			if ((vertexBuffers[vertexBuffer]).vboVB)
			{
				LOG_PRINT_X("glDeleteBuffers:\n");
				glDeleteBuffers(1, &(vertexBuffers[vertexBuffer]).vboVB);
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
		int RendererGLES::deleteIndexBuffer(const IndexBufferID indexBuffer)
		{
			if(indexBuffer>=indexBuffers.size()) return -1;
			if ((indexBuffers[indexBuffer]).vboIB)
			{
				LOG_PRINT_X("glDeleteBuffers:\n");
				glDeleteBuffers(1, &(indexBuffers[indexBuffer]).vboIB);
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
int RendererGLES::texturePitchVrtl(const TextureID id,int level){return 0;}
BYTE *RendererGLES::lockTexture(const TextureID id,int level, int& pitch){return 0;}
BYTE *RendererGLES::lockVertexBuffer(const VertexBufferID id){LOG_FNLN_X;
		return 0;}
BYTE *RendererGLES::lockIndexBuffer(const IndexBufferID id){LOG_FNLN_X;
		return 0;}
BYTE *RendererGLES::unlockTextureVrtl(const TextureID id,int level){return 0;}
BYTE *RendererGLES::unlockVertexBuffer(const VertexBufferID id){LOG_FNLN_X;
		return 0;}
BYTE *RendererGLES::unlockIndexBuffer(const IndexBufferID id){LOG_FNLN_X;
		return 0;}

#if 0
HRESULT RendererGLES::ProcessTexture( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return textureProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGLES::LockTextureDeviceObject(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGLES::opyToResourceTexture(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGLES::UnLockTextureDeviceObject(ProcessorID pProcessor)
{
	return textureProcessors[pProcessor].UnLockDeviceObject();
}


HRESULT RendererGLES::ProcessVertexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return vertexBufferProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGLES::LockVertexBufferDeviceObject(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGLES::opyToResourceVertexBuffer(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGLES::UnLockVertexBufferDeviceObject(ProcessorID pProcessor)
{
	return vertexBufferProcessors[pProcessor].UnLockDeviceObject();
}


HRESULT RendererGLES::ProcessIndexBuffer( ProcessorID pProcessor,void*pLocalData, SIZE_T Bytes )
{
	return indexBufferProcessors[pProcessor].Process( pLocalData, Bytes);
}
HRESULT RendererGLES::LockIndexBufferDeviceObject(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].LockDeviceObject();
}
HRESULT RendererGLES::opyToResourceIndexBuffer(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].CopyToResource();
}
HRESULT RendererGLES::UnLockIndexBufferDeviceObject(ProcessorID pProcessor)
{
	return indexBufferProcessors[pProcessor].UnLockDeviceObject();
}

ProcessorID RendererGLES::addTextureProcessor( TextureID& ppTexture, int g_SkipMips )
	{
		static int id=-1;
			CTextureProcessorGL tp(this,ppTexture);//,g_pResourceReuseCache, g_SkipMips );

			id=textureProcessors.add(tp);
		return id;
	}

ProcessorID RendererGLES::addVertexBufferProcessor( VertexBufferID& ppBuffer, int iSizeBytes,   void* pData)

	{
		static int id=-1;
			CVertexBufferProcessorGL tl(this,ppBuffer);//, iSizeBytes, pData);
			id=vertexBufferProcessors.add(tl);
		return id;
	}

ProcessorID RendererGLES::addIndexBufferProcessor( IndexBufferID& ppBuffer, int iSizeBytes, void* pData)
	{
		static int id=-1;
			CIndexBufferProcessorGL tl(this,ppBuffer);//, iSizeBytes,4/*???*/, pData);
			id=indexBufferProcessors.add(tl);
		return id;
	}

#endif
void RendererGLES::viewport(int x, int y, int viewportWidth, int viewportHeight){
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(x, y, viewportWidth, viewportHeight);
       // changeFrontFace(GL_CW);
  m_viewport.X=x;
  m_viewport.Y=y;
  m_viewport.Width=viewportWidth;
  m_viewport.Height=viewportHeight;

}
#if 0

void	RendererGLES::CreateTextureFromFile_Serial(  CHAR* szFileName,
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
    if( FAILED( RendererGLES::HLSLCg::LoadTexture(pLoader) ) )
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

void	RendererGLES::CreateVertexBuffer_Serial(
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

void	RendererGLES::CreateIndexBuffer_Serial(
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

    BYTE* RendererGLES::LockTexture(TextureID id, int m, int& pitch)
    {
		return textures[id].Lock(m,pitch);
	}
    void RendererGLES::UnlockTexture(TextureID id, int m)
    {
		textures[id].Unlock(m);
	}
    BYTE* RendererGLES::LockVertexBuffer(VertexBufferID id)
    {
		LOG_FNLN_X;
		return vertexBuffers[id].Lock();
	}
    void RendererGLES::UnLockVertexBuffer(VertexBufferID id)
    {
		LOG_FNLN_X;
		vertexBuffers[id].Unlock();
	}
    BYTE* RendererGLES::LockIndexBuffer(IndexBufferID id)
    {
		LOG_FNLN_X;
		return indexBuffers[id].Lock();
	}
    void RendererGLES::UnLockIndexBuffer(IndexBufferID id)
    {
		LOG_FNLN_X;
		indexBuffers[id].Unlock();
	}

	void RendererGLES::GetLevelDesc(TextureID id, unsigned int level,SURFACE_DESC* sDesc)
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
	unsigned int RendererGLES::GetLevelCount(TextureID id)
	{
		return textures[id].mipMapLevels;
	}
	void RendererGLES::GetDesc(VertexBufferID id,VERTEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=vertexBuffers[id].size;
	}
	void RendererGLES::GetDesc(IndexBufferID id, INDEXBUFFER_DESC* sDesc)
	{
		sDesc->Size=indexBuffers[id].nIndices*indexBuffers[id].indexSize;
		sDesc->Format=indexBuffers[id].indexSize;
	}

		void RendererGLES::changeBlendStateVrtl(const BlendStateID blendState, const uint sampleMask)
        {
        if (blendState != currentBlendState){
            if (blendState == BS_NONE || !blendStates[blendState].blendEnable){
                if (currentBlendEnable){
                    LOG_PRINT_X("glDisable:\n");
			glDisable(GL_BLEND);
   checkGlError("");
                    currentBlendEnable = false;
                }
            } else {
                if (blendStates[blendState].blendEnable){
                    if (!currentBlendEnable){
                        LOG_PRINT_X("glEnable:\n");
			glEnable(GL_BLEND);
   checkGlError("");
                        currentBlendEnable = true;
                    }
                    if (blendStates[blendState].srcFactorRGB   != currentSrcFactorRGB ||
                        blendStates[blendState].dstFactorRGB   != currentDstFactorRGB ||
                        blendStates[blendState].srcFactorAlpha != currentSrcFactorAlpha ||
                        blendStates[blendState].dstFactorAlpha != currentDstFactorAlpha){

                        LOG_PRINT_X("glBlendFuncSeparate:\n");
			glBlendFuncSeparate(
                            currentSrcFactorRGB   = blendStates[blendState].srcFactorRGB,
                            currentDstFactorRGB   = blendStates[blendState].dstFactorRGB,
                            currentSrcFactorAlpha = blendStates[blendState].srcFactorAlpha,
                            currentDstFactorAlpha = blendStates[blendState].dstFactorAlpha);
   checkGlError("");
                    }
                    if (blendStates[blendState].blendModeRGB   != currentBlendModeRGB ||
                        blendStates[blendState].blendModeAlpha != currentBlendModeAlpha){
                        LOG_PRINT_X("glBlendEquationSeparate:\n");
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
                LOG_PRINT_X("glColorMask:\n");
		glColorMask((mask & RED) & 1, ((mask & GREEN) >> 1) & 1, ((mask & BLUE) >> 2) & 1, ((mask & ALPHA) >> 3) & 1);
   		checkGlError("");

                currentMask = mask;
            }

            if (alphaToCoverage != currentAlphaToCoverageEnable)
            {
                if (alphaToCoverage) {
                    LOG_PRINT_X("glEnable:\n");
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
   checkGlError("");
                } else {
                    LOG_PRINT_X("glDisable:\n");
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
   checkGlError("");
		}
                currentAlphaToCoverageEnable = alphaToCoverage;
            }

            currentBlendState = blendState;
        }
    }

void RendererGLES::changeAlphaState(const AlphaStateID blendState)
{
	if (blendState != currentAlphaState)
	{
		if (blendState == -1 || !(alphaStates[blendState]).blendEnable)
		{
			if (currentAlphaEnable)
			{
                #if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)
				LOG_PRINT_X("glDisable:\n");
				glDisable(GL_ALPHA_TEST);
   checkGlError("");
				#endif
    			currentAlphaEnable = false;
			}
		}
		else
		{
			if ((alphaStates[blendState]).blendEnable)
			{
				if (!currentAlphaEnable)
				{
                #if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)
				LOG_PRINT_X("glEnable:\n");
				glEnable(GL_ALPHA_TEST);
   checkGlError("");
				#endif
    				currentAlphaEnable = true;
    //  reference = clamp(referenceArg, 0.0f, 1.0f);
    #if !defined(SDL_2_0_4_BUILD) && !defined(USE_GLES_SHADERS)
    LOG_PRINT_X("glAlphaFunc:\n");
	glAlphaFunc(GL_GEQUAL, 0.5f);//???
   checkGlError("");
	#endif
				}
			}
		}
		currentAlphaState = blendState;

}
}


		BlendStateID RendererGLES::addBlendState(const int srcFactorRGB, const int destFactorRGB, const int srcFactorAlpha, const int destFactorAlpha, const int blendModeRGB, const int blendModeAlpha, const int mask, const bool alphaToCoverage)
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

AlphaStateID RendererGLES::addAlphaState(
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
float RendererGLES::getTextWidth(const FontID font, const char *str, int length) const {
	if (font < 0) return 0;
    if (length < 0) length = (int) stx_strlen(str);

	Character *chars = fonts[font].chars;

	float len = 0;
	for (int i = 0; i < length; i++){
		len += chars[(unsigned char) str[i]].ratio;
	}

	return len;
}

void RendererGLES::drawArrays(const Primitives primitives, const int firstVertex, const int nVertices){
	LOG_PRINT_X("glDrawArrays:\n");
	//glDrawArrays (GLenum mode, GLint first, GLsizei count);
	glDrawArrays(glPrim_GLES[primitives], firstVertex, nVertices);
   checkGlError("");

	nDrawCalls++;
}

void RendererGLES::drawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices){
	uint indexSize = indexBuffers[selectedIndexBuffer].indexSize;

	//glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			unsigned short* usd=(unsigned short*) indexBuffers[selectedIndexBuffer].pIndices;
			unsigned int  * uid=(unsigned int*)   indexBuffers[selectedIndexBuffer].pIndices;
			if(indexSize == 2){
				glDrawElements(
					glPrim_GLES[primitives],
					nIndices,
			 		GL_UNSIGNED_SHORT,
			 		(usd+firstIndex)//???
				);
				checkGlError("");}
			else{
				glDrawElements(
					glPrim_GLES[primitives],
					nIndices,
			 		GL_UNSIGNED_INT,
			 		(uid+firstIndex)//???
				);	
				checkGlError("");}
	nDrawCalls++;
}

#if 0
		void RendererGLES2::changeIndexBuffer(const IndexBufferID indexBuffer)
		{
			////if (indexBuffer != currentIndexBuffer)
			{
				if (indexBuffer == IB_NONE)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				checkGlError("");
				}
				else
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (indexBuffers[indexBuffer]).vboIB);
				checkGlError("");
				}
				currentIndexBuffer = indexBuffer;
			}
		}
#endif
#endif

