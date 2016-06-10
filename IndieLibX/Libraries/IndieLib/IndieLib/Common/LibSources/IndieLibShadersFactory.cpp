/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <stdio.h>
#include <stdlib.h>
#include <Defines.h>

const char* IndieLibShadersFactory::GetShader(const char* aShaderName)
{
/*
	switch(a)
	{
	case eGLES_Shader_android:
*/
#if defined(USE_GLES_SHADERS) && (defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
			if(stx_strcmp(aShaderName,"texSHD1")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD1\n"
"attribute mediump vec3 aPosition;\n"
"attribute mediump vec4 aColor;\n"
"//varying mediump vec4 vPosition;\n"
"varying mediump vec4 vColor;\n"
"//ROW_MAJOR mediump mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mediump mat4 worldViewProj;\n"
"uniform mediump int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vColor=aColor;\n"

"}\n"
"[Fragment shader]\n"
"//varying mediump vec4 vPosition;\n"
"varying mediump vec4 vColor;\n"
"void main(){\n"
"	gl_FragColor = vColor;\n"
"}\n";
		else if(stx_strcmp(aShaderName,"texSHD2")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD2\n"
"attribute mediump vec3 aPosition;\n"
"attribute mediump vec2 aTexCoord;\n"
"//varying mediump vec4 vPosition;\n"
"varying mediump vec2 vTexCoord;\n"
"//ROW_MAJOR mediump mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mediump mat4 worldViewProj;\n"
"uniform mediump int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]\n"
"//varying mediump vec4 vPosition;\n"
"varying mediump vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
		else if(stx_strcmp(aShaderName,"texSHD3")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD3\n"
"attribute mediump vec3 aPosition;\n"
"attribute mediump vec2 aTexCoord;\n"
"//varying mediump vec4 vPosition;\n"
"varying mediump vec2 vTexCoord;\n"
"//ROW_MAJOR mediump mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mediump mat4 worldViewProj;\n"
"uniform mediump int bbb;\n"
"void main(){\n"
"   if(bbb==0)\n"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else\n"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"
"}\n"
"[Fragment shader]\n"
"//varying mediump vec4 position;\n"
"varying mediump vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
/*
		break;
	case eGLES_Shader_Ubuntu:
*/
#elif defined(USE_GLES_SHADERS) && defined(_LINUX)
			if(stx_strcmp(aShaderName,"texSHD1")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD1\n"
"attribute vec3 aPosition;\n"
"attribute vec4 aColor;\n"
"//varying vec4 vPosition;\n"
"varying vec4 vColor;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vColor=aColor;\n"

"}\n"
"[Fragment shader]\n"
"//varying vec4 vPosition;\n"
"varying vec4 vColor;\n"
"void main(){\n"
"	gl_FragColor = vColor;\n"
"}\n";
		else if(stx_strcmp(aShaderName,"texSHD2")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD2\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
		else if(stx_strcmp(aShaderName,"texSHD3")==0)
				return "[Vertex shader]\n"
"//IndieLib GLESShaders texSHD3\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)\n"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else\n"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"
"}\n"
"[Fragment shader]\n"
"//varying vec4 position;\n"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
/*
		break;
		case eHLSL_Shader:
*/
#elif defined(USE_HLSL_SHADERS)
			if(stx_strcmp(aShaderName,"texSHD1")==0)
				return "[Vertex shader]\n"
"//IndieLib HLSLShaders texSHD1\n"
"struct VsIn {\n"
"	float3 position: POSITION;\n"
"	float4 color: TEXCOORD;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float4 color: TEXCOORD0;\n"
"};\n"
"ROW_MAJOR float4x4 worldViewProj MVPSEMANTIC;\n"
"int bbb=0;\n"
"VsOut main(VsIn In){\n"
"	VsOut Out;\n"
"   if(bbb==0)"
"	Out.position = mul(  worldViewProj,float4(In.position, 1));\n"
"	else"
"	Out.position = mul(  float4(In.position, 1),worldViewProj);\n"
"	Out.color=In.color;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float4 color: TEXCOORD0;\n"
"};\n"
"float4 main(VsOut IN): COLOR {\n"
"	return IN.color;\n"
"}\n";
			else if(stx_strcmp(aShaderName,"texSHD2")==0)
				return "[Vertex shader]\n"
"//IndieLib HLSLShaders texSHD2\n"
"struct VsIn {\n"
"	float3 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"ROW_MAJOR float4x4 worldViewProj MVPSEMANTIC;\n"
"int bbb=0;\n"
"VsOut main(VsIn In){\n"
"	VsOut Out;\n"
"   if(bbb==0)"
"	Out.position = mul(  worldViewProj,float4(In.position, 1));\n"
"	else"
"	Out.position = mul(  float4(In.position, 1),worldViewProj);\n"
"	Out.texCoord=In.texCoord;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"sampler2D Sampler;\n"
"float4 main(VsOut IN): COLOR {\n"
"	return tex2D(Sampler, IN.texCoord);\n"
"}\n";
			else if(stx_strcmp(aShaderName,"texSHD3")==0)
				return "[Vertex shader]\n"
"//IndieLib HLSLShaders texSHD3\n"
"struct VsIn {\n"
"	float3 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"ROW_MAJOR float4x4 worldViewProj MVPSEMANTIC;\n"
"int bbb=0;\n"
"VsOut main(VsIn In){\n"
"	VsOut Out;\n"
"   if(bbb==0)"
"	Out.position = mul(  worldViewProj,float4(In.position, 1));\n"
"	else"
"	Out.position = mul(  float4(In.position, 1),worldViewProj);\n"
"	Out.texCoord=In.texCoord;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD;\n"
"};\n"
"sampler2D Sampler;\n"
"float4 main(VsOut IN): COLOR {\n"
"	return tex2D(Sampler, IN.texCoord);\n"
"}\n";
/*
		break;
	case eGLSL_Shader:
*/
#else
			if(stx_strcmp(aShaderName,"texSHD1")==0)
				return "[Vertex shader]\n"
"//IndieLib GLSLShaders texSHD1\n"
"attribute vec3 aPosition;\n"
"attribute vec4 aColor;\n"
"//varying vec4 vPosition;\n"
"varying vec4 vColor;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vColor=aColor;\n"

"}\n"
"[Fragment shader]\n"
"//varying vec4 vPosition;\n"
"varying vec4 vColor;\n"
"void main(){\n"
"	gl_FragColor = vColor;\n"
"}\n";
	else if(stx_strcmp(aShaderName,"texSHD2")==0)
				return "[Vertex shader]\n"
"//IndieLib GLSLShaders texSHD2\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
	else if(stx_strcmp(aShaderName,"texSHD3")==0)
				return "[Vertex shader]\n"
"//IndieLib GLSLShaders texSHD3\n"
"attribute vec3 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 vPosition;\n"
"varying vec2 vTexCoord;\n"
"//ROW_MAJOR mat4 worldViewProj MVPSEMANTIC;\n"
"uniform mat4 worldViewProj;\n"
"uniform int bbb;\n"
"void main(){\n"
"   if(bbb==0)\n"
"	gl_Position = worldViewProj*vec4(aPosition, 1);\n"
"	else\n"
"	gl_Position = vec4(aPosition, 1)*worldViewProj;\n"
"	vTexCoord=aTexCoord;\n"
"}\n"
"[Fragment shader]\n"
"//varying vec4 position;\n"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Sampler;\n"
"void main(){\n"
"	gl_FragColor = texture2D(Sampler, vTexCoord);\n"
"}\n";
#endif
	//break;
	//};
	return 0;
}

