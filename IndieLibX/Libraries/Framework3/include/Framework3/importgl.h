/* San Angeles Observation OpenGL ES version example
 * Copyright 2004-2005 Jetro Lauha
 * All rights reserved.
 * Web: http://iki.fi/jetro/
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *       Software Foundation; either version 2.1 of the License, or (at
 *       your option) any later version. The text of the GNU Lesser
 *       General Public License is included with this source in the
 *       file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *       the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 *
 * $Id: importgl.h,v 1.4 2005/02/24 20:29:33 tonic Exp $
 * $Revision: 1.4 $
 */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef _MSC_VER
#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED

#ifdef _MSC_VER
	int importGLInit();
	void importGLDeinit();
#else
inline int importGLInit(){return 0;};
inline void importGLDeinit(){};
#endif

#include <STX/STX.h>
#define GL_GLEXT_PROTOTYPES

#if defined(_MSC_VER) && defined(USE_GLES_SHADERS)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
void glFrustum(	double  	left,
 	double  	right,
 	double  	bottom,
 	double  	top,
 	double  	nearVal,
 	double  	farVal);
#elif defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>
#else
#if defined(USE_GLES_SHADERS)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/gl.h>
#if defined(_MSC_VER)
#undef GL_GLEXT_PROTOTYPES
#include <Framework3/glext.linux.h>
#else
#include <GL/glext.h>
#endif
#include <GL/glu.h>
#endif
#endif

#if defined(LINUX) && defined(USE_GLES_SHADERS)
#define glVertexAttribPointer		glVertexAttribPointerARB
#define glEnableVertexAttribArray	glEnableVertexAttribArrayARB
#define glGenBuffers			glGenBuffersARB
#define glBindBuffer			glBindBufferARB
#define glBufferData			glBufferDataARB
#define GL_RGB8_OES			GL_RGB8
#define GL_RGBA8_OES			GL_RGBA8
#define GL_WRITE_ONLY_OES		GL_WRITE_ONLY
#define GL_ARRAY_BUFFER			GL_ARRAY_BUFFER_ARB
#endif

#endif

#if defined(_MSC_VER)
#define DISABLE_IMPORTGL
typedef char GLchar;
#define GLTYPEDEF(retType, funcName, args) typedef retType (APIENTRY * PFNPROC_##funcName) args
#define GLEXTERN(funcName) extern PFNPROC_##funcName funcName
GLTYPEDEF(GLvoid*, glMapBufferARB, (GLenum, GLenum));
GLTYPEDEF(GLboolean, glUnmapBufferARB, (GLenum));
#elif defined(_MSC_VER)
#undef GL_GLEXT_PROTOTYPES
typedef char GLchar;
typedef INT_PTR GLsizeiptrARB;
typedef char         GLcharARB;
typedef unsigned int GLhandleARB;
#define GLTYPEDEF(retType, funcName, args) typedef retType (APIENTRY * PFNPROC_##funcName) args
#define GLEXTERN(funcName) extern PFNPROC_##funcName funcName

#if !defined(USE_GLES_SHADERS)
GLTYPEDEF(void, glBindBufferARB, (GLenum, GLuint));
GLTYPEDEF(void, glBindFramebufferEXT, (GLenum, GLuint));
GLTYPEDEF(void, glBindRenderbufferEXT, (GLenum, GLuint));
GLTYPEDEF(void, glBlendEquationSeparate, (GLenum, GLenum));
GLTYPEDEF(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum));
GLTYPEDEF(void, glBufferDataARB, (GLenum, GLsizeiptrARB, const GLvoid *, GLenum));
GLTYPEDEF(void, glCompressedTexImage2DARB, (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *));
GLTYPEDEF(void, glCompressedTexImage3DARB, (GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *));
GLTYPEDEF(void, glDeleteBuffers, (GLsizei, const GLuint* ));
GLTYPEDEF(void, glDeleteBuffersARB, (GLsizei, const GLuint *));
GLTYPEDEF(void, glDeleteRenderbuffersEXT, (GLsizei, const GLuint *));
GLTYPEDEF(void, glDrawBuffersARB, (GLsizei, const GLenum *));
GLTYPEDEF(void, glFramebufferRenderbufferEXT, (GLenum, GLenum, GLenum, GLuint));
GLTYPEDEF(void, glFramebufferTexture2DEXT, (GLenum, GLenum, GLenum, GLuint, GLint));
GLTYPEDEF(void, glGenBuffersARB, (GLsizei, GLuint *));
GLTYPEDEF(void, glGenFramebuffersEXT, (GLsizei, GLuint *));
GLTYPEDEF(void, glGenRenderbuffersEXT, (GLsizei, GLuint *));
GLTYPEDEF(void, glPointParameterfARB, (GLenum, GLfloat));
GLTYPEDEF(void, glPointParameterfvARB, (GLenum, const GLfloat *));
GLTYPEDEF(void, glRenderbufferStorageEXT, (GLenum, GLenum, GLsizei, GLsizei));
GLTYPEDEF(void, glTexImage3D, (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels));

GLTYPEDEF(GLvoid*, glMapBufferARB, (GLenum, GLenum));
GLTYPEDEF(GLboolean, glUnmapBufferARB, (GLenum));

GLTYPEDEF(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *));
GLTYPEDEF(void, glEnableVertexAttribArray, (GLuint));
GLTYPEDEF(void, glDeleteFramebuffersEXT, (GLsizei, const GLuint *));

GLTYPEDEF(void, glEnableVertexAttribArrayARB, (GLuint));
GLTYPEDEF(void, glDisableVertexAttribArrayARB, (GLuint));
GLTYPEDEF(void, glClientActiveTexture, (GLenum));
#else
GLTYPEDEF(void, glAttachShader, (GLuint program, GLuint shader));
GLTYPEDEF(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar* name));
GLTYPEDEF(void, glBindBuffer, (GLenum, GLuint));
GLTYPEDEF(void, glBindFramebuffer, (GLenum, GLuint));
GLTYPEDEF(void, glBindRenderbuffer, (GLenum, GLuint));
GLTYPEDEF(void, glBlendEquationSeparate, (GLenum, GLenum));
GLTYPEDEF(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum));
GLTYPEDEF(void, glBufferData, (GLenum, GLsizeiptrARB, const GLvoid *, GLenum));
GLTYPEDEF(void, glCompileShader, (GLuint shader));
GLTYPEDEF(void, glCompressedTexImage2D, (GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *));
GLTYPEDEF(GLuint, glCreateProgram, (void));
GLTYPEDEF(GLuint, glCreateShader, (GLenum type));
GLTYPEDEF(void, glDeleteBuffers, (GLsizei, const GLuint *));
GLTYPEDEF(void, glDeleteFramebuffers, (GLsizei, const GLuint *));
GLTYPEDEF(void, glDeleteProgram, (GLuint program));
GLTYPEDEF(void, glDeleteRenderbuffers, (GLsizei, const GLuint *));
GLTYPEDEF(void, glDeleteShader, (GLuint shader));
GLTYPEDEF(void, glDisableVertexAttribArray, (GLuint));
GLTYPEDEF(void, glEnableVertexAttribArray, (GLuint));
GLTYPEDEF(void, glFramebufferRenderbuffer, (GLenum, GLenum, GLenum, GLuint));
GLTYPEDEF(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLin));
GLTYPEDEF(void, glGenBuffers, (GLsizei, GLuint *));
GLTYPEDEF(void, glGenFramebuffers, (GLsizei, GLuint *));
GLTYPEDEF(void, glGenRenderbuffers, (GLsizei, GLuint *));
GLTYPEDEF(void, glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name));
GLTYPEDEF(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name));
GLTYPEDEF(GLint, glGetAttribLocation, (GLuint program, const GLchar* name));
GLTYPEDEF(void, glGetProgramInfoLog, (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog));
GLTYPEDEF(void, glGetProgramiv, (GLuint program, GLenum pname, GLint* params));
GLTYPEDEF(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog));
GLTYPEDEF(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint* params));
GLTYPEDEF(GLint, glGetUniformLocation, (GLuint program, const GLchar* name));
GLTYPEDEF(void, glLinkProgram, (GLuint program));
GLTYPEDEF(void, glRenderbufferStorage, (GLenum, GLenum, GLsizei, GLsizei));
GLTYPEDEF(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar** string, const GLint* length));
GLTYPEDEF(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat* v));
GLTYPEDEF(void, glUniform1i, (GLint location, GLint x));
GLTYPEDEF(void, glUniform1iv, (GLint location, GLsizei count, const GLint* v));
GLTYPEDEF(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat* v));
GLTYPEDEF(void, glUniform2iv, (GLint location, GLsizei count, const GLint* v));
GLTYPEDEF(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat* v));
GLTYPEDEF(void, glUniform3iv, (GLint location, GLsizei count, const GLint* v));
GLTYPEDEF(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat* v));
GLTYPEDEF(void, glUniform4iv, (GLint location, GLsizei count, const GLint* v));
GLTYPEDEF(void, glUniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
GLTYPEDEF(void, glUniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
GLTYPEDEF(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value));
GLTYPEDEF(void, glUseProgram, (GLuint program));
GLTYPEDEF(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *));
#endif

#if !defined(USE_GLES_SHADERS)
GLEXTERN(glBindBufferARB);
GLEXTERN(glBindFramebufferEXT);
GLEXTERN(glBindRenderbufferEXT);
GLEXTERN(glBlendEquationSeparate);
GLEXTERN(glBlendFuncSeparate);
GLEXTERN(glBufferDataARB);
GLEXTERN(glCompressedTexImage2DARB);
GLEXTERN(glCompressedTexImage3DARB);
GLEXTERN(glDeleteBuffers);
GLEXTERN(glDeleteBuffersARB);
GLEXTERN(glDeleteRenderbuffersEXT);
GLEXTERN(glDrawBuffersARB);
GLEXTERN(glFramebufferRenderbufferEXT);
GLEXTERN(glFramebufferTexture2DEXT);
GLEXTERN(glGenBuffersARB);
GLEXTERN(glGenFramebuffersEXT);
GLEXTERN(glGenRenderbuffersEXT);
GLEXTERN(glMapBufferARB);
GLEXTERN(glPointParameterfARB);
GLEXTERN(glPointParameterfvARB);
GLEXTERN(glRenderbufferStorageEXT);
GLEXTERN(glTexImage3D);
GLEXTERN(glUnmapBufferARB);

GLEXTERN(glVertexAttribPointer);
GLEXTERN(glEnableVertexAttribArray);
GLEXTERN(glDeleteFramebuffersEXT);

	GLEXTERN(glEnableVertexAttribArrayARB);
	GLEXTERN(glDisableVertexAttribArrayARB);
	GLEXTERN(glClientActiveTexture);
	GLEXTERN(glMapBufferARB);
	GLEXTERN(glUnmapBufferARB);
#else
GLEXTERN(glAttachShader);
GLEXTERN(glBindAttribLocation);
GLEXTERN(glBindBuffer);
GLEXTERN(glBindFramebuffer);
GLEXTERN(glBindRenderbuffer);
GLEXTERN(glBlendEquationSeparate);
GLEXTERN(glBlendFuncSeparate);
GLEXTERN(glBufferData);
GLEXTERN(glCompileShader);
GLEXTERN(glCompressedTexImage2D);
GLEXTERN(glCreateProgram);
GLEXTERN(glCreateShader);
GLEXTERN(glDeleteBuffers);
GLEXTERN(glDeleteFramebuffers);
GLEXTERN(glDeleteProgram);
GLEXTERN(glDeleteRenderbuffers);
GLEXTERN(glDeleteShader);
GLEXTERN(glDisableVertexAttribArray);
GLEXTERN(glEnableVertexAttribArray);
GLEXTERN(glFramebufferRenderbuffer);
GLEXTERN(glFramebufferTexture2D);
GLEXTERN(glGenBuffers);
GLEXTERN(glGenFramebuffers);
GLEXTERN(glGenRenderbuffers);
GLEXTERN(glGetActiveAttrib);
GLEXTERN(glGetActiveUniform);
GLEXTERN(glGetAttribLocation);
GLEXTERN(glGetProgramInfoLog);
GLEXTERN(glGetProgramiv);
GLEXTERN(glGetShaderInfoLog);
GLEXTERN(glGetShaderiv);
GLEXTERN(glGetUniformLocation);
GLEXTERN(glLinkProgram);
GLEXTERN(glRenderbufferStorage);
GLEXTERN(glShaderSource);
GLEXTERN(glUniform1fv);
GLEXTERN(glUniform1i);
GLEXTERN(glUniform1iv);
GLEXTERN(glUniform2fv);
GLEXTERN(glUniform2iv);
GLEXTERN(glUniform3fv);
GLEXTERN(glUniform3iv);
GLEXTERN(glUniform4fv);
GLEXTERN(glUniform4iv);
GLEXTERN(glUniformMatrix2fv);
GLEXTERN(glUniformMatrix3fv);
GLEXTERN(glUniformMatrix4fv);
GLEXTERN(glUseProgram);
GLEXTERN(glVertexAttribPointer);
#endif

#elif (defined(_MSC_VER) && defined(USE_GLES_SHADERS)) || defined(__APPLE__) || defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define glDeleteObject
#include <math.h>
//define glFrustumf
inline void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tanf(fovy * 3.141592654f / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

#if !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
    glFrustum ((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
#endif
}
#elif defined(QT_BUILD)
#include <QtOpenGL>
#endif

#if defined(ANDROID)// || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)

#define	glTexImage3D			glTexImage3DOES
#define	glTexSubImage3D			glTexSubImage3DOES
#define	glCopyTexSubImage3D		glCopyTexSubImage3DOES

#define	glCompressedTexImage3D		glCompressedTexImage3DOES
#define	glCompressedTexSubImage3D	glCompressedTexSubImage3DOES
#define	glFramebufferTexture3D		glFramebufferTexture3DOES

#define	glMapBuffer			glMapBufferOES
#define	glUnmapBuffer			glUnmapBufferOES
#define	glGetBufferPointerv		glGetBufferPointervOES

#define	glClearDepth			glClearDepthf

#define GL_RGB5_A1GL_RGB5_A1

#define glDeleteVertexArrays	glDeleteVertexArraysOES
#define glGenVertexArrays	glGenVertexArraysOES
#define glBindVertexArray	glBindVertexArrayOES
#endif

extern int GL_ARB_shading_language_100_supported;
extern int GL_ARB_fragment_shader_supported;
extern int GL_ARB_fragment_program_supported;
extern int GL_ARB_draw_buffers_supported;
extern int GL_EXT_texture_filter_anisotropic_supported;

#if 0//(defined(_MSC_VER) || defined(LINUX)) && defined(USE_GLES_SHADERS)
#define glMapBuffer			glMapBufferARB
#define glUnmapBuffer    		glUnmapBufferARB
#elif 0
#define glMapBuffer			glMapBufferOES
#define glUnmapBuffer			glUnmapBufferOES
#elif 1
#define glMapBuffer
#define glUnmapBuffer
#endif
#endif
#endif

