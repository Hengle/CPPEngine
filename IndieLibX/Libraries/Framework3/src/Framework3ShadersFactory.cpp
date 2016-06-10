/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <stdio.h>
#include <stdlib.h>
#include <Framework3/IRenderer.h>

const char* Framework3ShaderFactory::GetShader(const char* aShaderName)
{
/*
	switch(a)
	{
	case eGLES_Shader_android:
*/
#if defined(USE_GLES_SHADERS) && (defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
			if(stx_strcmp(aShaderName,"texSHD0")==0)
				return "[Vertex shader]"
"//IndieLib GLESShaders texSHD0\n"
"attribute mediump vec2 aPosition;\n"
"attribute mediump vec2 aTexCoord;\n"
"//varying mediump vec4 position;\n"
"varying mediump vec2 vTexCoord;\n"
"uniform mediump vec4 scaleBias=mediump vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"void main(){\n"
"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
"	gl_Position.w= WSIGN 1;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]"
"varying mediump vec2 vTexCoord;\n"
"uniform mediump sampler2D Base;\n"
"uniform mediump vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(){\n"
"	gl_FragColor = texture2D(Base, vTexCoord)* colorRGBA;\n"
"}\n";
		else if(stx_strcmp(aShaderName,"plainSHD1")==0)
				return "[Vertex shader]"
"//IndieLib GLESShaders plainSHD1\n"
"uniform mediump vec4 scaleBias=mediump vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute mediump vec2 aPosition;\n"
"//varying mediump vec4 position;\n"
"void main(){\n"
	"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//	"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"
	
	"}\n"
"[Fragment shader]"
"//varying mediump vec4 position;\n"
"uniform mediump vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
		else if(stx_strcmp(aShaderName,"plainSHD2")==0)
				return "[Vertex shader]"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute mediump vec3 aPosition;\n"
"void main(){\n"
	"	gl_Position.xyz = aPosition.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"	
	"}\n"
"[Fragment shader]"
"uniform mediump vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(VsOut In): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
/*
		break;
	case eGLES_Shader_Ubuntu:
*/
#elif defined(USE_GLES_SHADERS) && defined(_LINUX)
			if(stx_strcmp(aShaderName,"texSHD0")==0)
				return "[Vertex shader]"
"//IndieLib GLESShaders texSHD0\n"
"attribute vec2 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 position;\n"
"varying vec2 vTexCoord;\n"
"uniform vec4 scaleBias=mediump vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"void main(){\n"
"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
"	gl_Position.w= WSIGN 1;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Base;\n"
"uniform vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(){\n"
"	gl_FragColor = texture2D(Base, vTexCoord)* colorRGBA;\n"
"}\n";
		else if(stx_strcmp(aShaderName,"plainSHD1")==0)
				return "[Vertex shader]"
"//IndieLib GLESShaders plainSHD1\n"
"uniform vec4 scaleBias=mediump vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute vec2 aPosition;\n"
"//varying vec4 position;\n"
"void main(){\n"
	"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//	"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"
	
	"}\n"
"[Fragment shader]"
"//varying vec4 position;\n"
"uniform vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
		else if(stx_strcmp(aShaderName,"plainSHD2")==0)
				return "[Vertex shader]"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute vec3 aPosition;\n"
"void main(){\n"
	"	gl_Position.xyz = aPosition.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"	
	"}\n"
"[Fragment shader]"
"uniform vec4 colorRGBA=mediump vec4(1,1,1,1);\n"
"void main(VsOut In): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
/*
		break;
		case eHLSL_Shader:
*/
#elif defined(USE_HLSL_SHADERS)
			if(stx_strcmp(aShaderName,"texSHD0")==0)
				return "[Vertex shader]"
"//IndieLib HLSLShaders texSHD0\n"
"struct VsIn {\n"
"	float2 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"float4 scaleBias=float4(0.003,-0.003,-1,1);\n"
//"float4 translation=float4(0,0,0,1);\n"
//"float invertY=0.0;\n"
"VsOut main(VsIn In){\n"
"	VsOut Out=(VsOut)0;\n"
"	Out.position.xy = In.position.xy * scaleBias.xy + scaleBias.zw;\n"
//"	Out.position.z=0;\n"
//"	//Out.position.xyz = Out.position.xyz + translation.xyz;\n"
"	Out.position.w= WSIGN 1;\n"
//"	if(invertY==1.0)\n"
//"	{\n"
//"		Out.texCoord.x=In.texCoord.x;\n"
//"		Out.texCoord.y=1.0-In.texCoord.y;\n"
//"	}\n"
//"	else\n"
"		Out.texCoord=In.texCoord;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]"
"struct VsOut {\n"
"	float4 position: POSITION;\n"
"	float2 texCoord: TEXCOORD0;\n"
"};\n"
"sampler2D Base;\n"
"float4 colorRGBA=float4(1,1,1,1);\n"
"float4 main(VsOut IN): COLOR {\n"
"	return tex2D(Base, IN.texCoord)* colorRGBA;\n"
"}\n";
			else if(stx_strcmp(aShaderName,"plainSHD1")==0)
				return "[Vertex shader]"
"//IndieLib HLSLShaders plainSHD1\n"
"float4 scaleBias=float4(0.003,-0.003,-1,1);\n"
//"float4 translation=float4(0,0,0,1);\n"
"struct VsIn {\n"
"float2 position: POSITION;\n"
"};\n"
"struct VsOut {\n"
"float4 position: POSITION;\n"
"};\n"
"VsOut main(VsIn In){\n"
"	VsOut Out=(VsOut)0;\n"
	"	Out.position.xy = In.position.xy * scaleBias.xy + scaleBias.zw;\n"
//"	Out.position.z=0;\n"
//	"	//Out.position.xyz = Out.position.xyz + translation.xyz;\n"
	"	Out.position.w= WSIGN 1;\n"
	"	return Out;\n"
	"}\n"
"[Fragment shader]"
"struct VsOut {\n"
"float4 position: POSITION;\n"
"};\n"
"float4 colorRGBA=float4(1,1,1,1);\n"
"float4 main(VsOut In): COLOR{\n"
	"	return colorRGBA;\n"
	"}\n";
			else if(stx_strcmp(aShaderName,"plainSHD2")==0)
				return "[Vertex shader]"
//"float4 translation=float4(0,0,0,1);\n"
"struct VsIn {\n"
"float3 position: POSITION;\n"
"};\n"
"struct VsOut {\n"
"float4 position: POSITION;\n"
"};\n"
"VsOut main(VsIn In){\n"
"	VsOut Out=(VsOut)0;\n"
	"	Out.position.xyz = In.position.xyz;\n"
	"	Out.position.w= WSIGN 1;\n"
	"	return Out;\n"
	"}\n"
"[Fragment shader]"
"struct VsOut {\n"
"float4 position: POSITION;\n"
"};\n"
"float4 colorRGBA=float4(1,1,1,1);\n"
"float4 main(VsOut In): COLOR{\n"
	"	return colorRGBA;\n"
	"}\n";
/*
		break;
	case eGLSL_Shader:
*/
#else
			if(stx_strcmp(aShaderName,"texSHD0")==0)
				return "[Vertex shader]"
"//IndieLib GLSLShaders texSHD0\n"
"attribute vec2 aPosition;\n"
"attribute vec2 aTexCoord;\n"
"//varying vec4 position;\n"
"varying vec2 vTexCoord;\n"
"uniform vec4 scaleBias=vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"void main(){\n"
"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
"	gl_Position.w= WSIGN 1;\n"
"	vTexCoord=aTexCoord;\n"

"}\n"
"[Fragment shader]"
"varying vec2 vTexCoord;\n"
"uniform sampler2D Base;\n"
"uniform vec4 colorRGBA=vec4(1,1,1,1);\n"
"void main(){\n"
"	gl_FragColor = texture2D(Base, vTexCoord)* colorRGBA;\n"
"}\n";
	else if(stx_strcmp(aShaderName,"plainSHD1")==0)
				return "[Vertex shader]"
"//IndieLib GLSLShaders plainSHD1\n"
"uniform vec4 scaleBias=vec4(0.003,-0.003,-1,1);\n"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute vec2 aPosition;\n"
"//varying vec4 position;\n"
"void main(){\n"
	"	gl_Position.xy = aPosition.xy * scaleBias.xy + scaleBias.zw;\n"
//"	gl_Position.z=0;\n"
//	"	//gl_Position.xyz = aPosition.xyz + translation.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"
	
	"}\n"
"[Fragment shader]"
"//varying vec4 position;\n"
"uniform vec4 colorRGBA=vec4(1,1,1,1);\n"
"void main(): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
	else if(stx_strcmp(aShaderName,"plainSHD2")==0)
				return "[Vertex shader]"
//"vec4 translation=vec4(0,0,0,1);\n"
"attribute vec3 aPosition;\n"
"void main(){\n"
	"	gl_Position.xyz = aPosition.xyz;\n"
	"	gl_Position.w= WSIGN 1;\n"	
	"}\n"
"[Fragment shader]"
"uniform vec4 colorRGBA=vec4(1,1,1,1);\n"
"void main(VsOut In): COLOR{\n"
	"	gl_FragColor = colorRGBA;\n"
	"}\n";
#endif
	//break;
	//};
	return 0;
}

