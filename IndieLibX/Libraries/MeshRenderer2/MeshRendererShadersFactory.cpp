/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
/*
    aiProcess_CalcTangentSpace
    aiProcess_GenNormals
    aiProcess_GenSmoothNormals
*/
#include <stdio.h>
#include <stdlib.h>
#include <Framework3/IRenderer.h>
#include <assimp_view.h>

std::map<std::string, ShaderID> MeshRendererShadersFactory::m_MeshRendererShadersFactory_Shadercache;
std::map<std::string, VertexFormatID> MeshRendererShadersFactory::m_MeshRendererShadersFactory_VertexFormatcache;

VertexFormatID MeshRendererShadersFactory::find_from_MeshRendererShadersFactory_VertexFormatcache(const char* fileName)
{
	std::map<std::string, VertexFormatID>::iterator it;
	it = m_MeshRendererShadersFactory_VertexFormatcache.find(std::string(fileName));
	if (it != m_MeshRendererShadersFactory_VertexFormatcache.end())
		return it->second;
	return -1;
}
void MeshRendererShadersFactory::insert_to_MeshRendererShadersFactory_VertexFormatcache(const char* fileName, VertexFormatID id)
{
	std::map<std::string, VertexFormatID>::iterator it;
	it = m_MeshRendererShadersFactory_VertexFormatcache.find(std::string(fileName));
	if (it == m_MeshRendererShadersFactory_VertexFormatcache.end())
		m_MeshRendererShadersFactory_VertexFormatcache.insert ( std::pair<std::string,VertexFormatID>(std::string(fileName), id) );
}
VertexFormatID MeshRendererShadersFactory::GetVertexFormat(const char* aShaderName, const ShaderID shd)
{
	VertexFormatID newvf;
	std::map<std::string, VertexFormatID>::iterator it;
	it = m_MeshRendererShadersFactory_VertexFormatcache.find(std::string(aShaderName));
	if (it != m_MeshRendererShadersFactory_VertexFormatcache.end())
		newvf=it->second;
	else
	{
		if((stx_strcmp(aShaderName,"NormalsEffect")==0)
			|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
			|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
			|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
		{
			FormatDesc format[] =
			{
				0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
				0, TYPE_NORMAL,   FORMAT_FLOAT, 3
			};
			newvf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
		}
else if((stx_strcmp(aShaderName,"CompleteEffect")==0))
{
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
			0, TYPE_NORMAL,   FORMAT_FLOAT, 3,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 4, // dColorDiffuse
			0, TYPE_TANGENT,  FORMAT_FLOAT, 3, // vTangent
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 3, // vBitangent
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
		};
		newvf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
}
else 
{
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
		};
		newvf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
}
		m_MeshRendererShadersFactory_VertexFormatcache.insert ( std::pair<std::string,VertexFormatID>(std::string(aShaderName),newvf) );
		}
		return newvf;
}

	ShaderID MeshRendererShadersFactory::find_from_MeshRendererShadersFactory_Shadercache(const char* fileName)
	{
		std::map<std::string, ShaderID>::iterator it;
		it = m_MeshRendererShadersFactory_Shadercache.find(std::string(fileName));
		if (it != m_MeshRendererShadersFactory_Shadercache.end())
			return it->second;
		return -1;
	}

	void MeshRendererShadersFactory::insert_to_MeshRendererShadersFactory_Shadercache(const char* fileName, ShaderID id)
	{
		std::map<std::string, ShaderID>::iterator it;
		it = m_MeshRendererShadersFactory_Shadercache.find(std::string(fileName));
		if (it == m_MeshRendererShadersFactory_Shadercache.end())
			m_MeshRendererShadersFactory_Shadercache.insert ( std::pair<std::string,ShaderID>(std::string(fileName), id) );
	}

	ShaderID MeshRendererShadersFactory::GetShader(const char* aShaderName, const char *vsName, const char *psName)
	{
		LOG_FNLN;
		LOG_PRINT("ShaderName%s\nvsName=%s\npsName=%s\n", aShaderName, vsName, psName);
		ShaderID newshd;
		std::map<std::string, ShaderID>::iterator it;
		it = m_MeshRendererShadersFactory_Shadercache.find(std::string(aShaderName));
		if (it != m_MeshRendererShadersFactory_Shadercache.end())
			newshd=it->second;
		else
		{
			const char* shdTxt=GetShaderTxt(aShaderName);
			newshd = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, vsName, psName);
			m_MeshRendererShadersFactory_Shadercache.insert ( std::pair<std::string,ShaderID>(std::string(aShaderName),newshd) );
		}
		return newshd;
	}
const char* MeshRendererShadersFactory::GetShaderTxt(const char* aShaderName)
{
/*
	switch(a)
	{
	case eGLES_Shader_android:
*/
#if defined(USE_GLES_SHADERS) && (defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR))
if((stx_strcmp(aShaderName,"DefaultEffect")==0)
|| (stx_strcmp(aShaderName,"PassThroughEffect")==0)
|| (stx_strcmp(aShaderName,"NormalsEffect")==0)
|| (stx_strcmp(aShaderName,"SimpleShader")==0)
|| (stx_strcmp(aShaderName,"lightning1")==0)
|| (stx_strcmp(aShaderName,"lightning2")==0)
|| (stx_strcmp(aShaderName,"AmbientLighting")==0)
|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
	return "[Vertex shader]\n"
	"//ROW_MAJOR mediump float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"uniform mediump mat4 WorldViewProjection;\n"
	"attribute mediump vec3 aPosition;\n"
	"attribute mediump vec2 auv;\n"
	"varying mediump vec2 vuv;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 objPos = aPosition;\n"
		"gl_Position = WorldViewProjection * vec4( objPos, 1.0f);\n"
		"vuv=auv;\n"
	"}\n"
"[Fragment shader]\n"
	"//varying mediump vec4 vPosition;\n"
	"varying mediump vec2 vuv;\n"
	"uniform sampler2D DIFFUSE_SAMPLER;\n"
	"void main()\n"
	"{\n"
		"//gl_FragColor = mediump vec4(0.0f,1.0f,0.0f,1.0f);\n"
		"gl_FragColor = texture2D(DIFFUSE_SAMPLER,vuv);\n"
	"}\n";
/*
		break;
	case eGLES_Shader_Ubuntu:
*/
#elif defined(USE_GLES_SHADERS) && defined(_LINUX)
if((stx_strcmp(aShaderName,"DefaultEffect")==0)
|| (stx_strcmp(aShaderName,"PassThroughEffect")==0)
|| (stx_strcmp(aShaderName,"NormalsEffect")==0)
|| (stx_strcmp(aShaderName,"SimpleShader")==0)
|| (stx_strcmp(aShaderName,"lightning1")==0)
|| (stx_strcmp(aShaderName,"lightning2")==0)
|| (stx_strcmp(aShaderName,"AmbientLighting")==0)
|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
	return "[Vertex shader]\n"
	"//ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"uniform mat4 WorldViewProjection;\n"
	"attribute vec3 aPosition;\n"
	"attribute vec2 auv;\n"
	"varying vec2 vuv;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 objPos = aPosition;\n"
		"gl_Position = WorldViewProjection * vec4( objPos, 1.0f);\n"
		"vuv=auv;\n"
	"}\n"
"[Fragment shader]\n"
	"//varying vec4 vPosition;\n"
	"varying vec2 vuv;\n"
	"uniform sampler2D DIFFUSE_SAMPLER;\n"
	"void main()\n"
	"{\n"
		"//gl_FragColor = vec4(0.0f,1.0f,0.0f,1.0f);\n"
		"gl_FragColor = texture2D(DIFFUSE_SAMPLER,vuv);\n"
	"}\n";
/*
		break;
		case eHLSL_Shader:
*/
#elif defined(USE_HLSL_SHADERS)
if((stx_strcmp(aShaderName,"DefaultEffect")==0)
|| (stx_strcmp(aShaderName,"PassThroughEffect")==0)
|| (stx_strcmp(aShaderName,"NormalsEffect")==0)
|| (stx_strcmp(aShaderName,"SimpleShader")==0)
|| (stx_strcmp(aShaderName,"lightning1")==0)
|| (stx_strcmp(aShaderName,"lightning2")==0)
//|| (stx_strcmp(aShaderName,"AmbientLighting")==0)
//|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
//|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
//|| (stx_strcmp(aShaderName,"PhongLighting2")==0)
)
	return "[Vertex shader]"
	"ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Position : POSITION;\n"
		"//float3 Normal : NORMAL;\n"
		"//float4 Diffuse : TEXCOORD0;\n"
		"//float3 Tangent : TANGENT;\n"
		"//float3 Bitangent : TEXCOORD1;\n"
		"float2 uv : TEXCOORD0;\n"
		"//float2 uv2 : TEXCOORD3;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Position : POSITION;\n"
		"float2 uv : TEXCOORD0;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
		"float3 objPos = IN.Position;\n"
		"Out.Position = mul( float4( objPos, 1.0f), WorldViewProjection);\n"
		"Out.uv=IN.uv;\n"
		"return Out;\n"
	"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Position : POSITION;\n"
		"float2 uv : TEXCOORD0;\n"
		"};\n"
		"sampler2D DIFFUSE_SAMPLER;\n"
	"float4 main(VS_OUTPUT IN) : COLOR\n"
	"{\n"
		"float4 OUT = float4(0.0f,1.0f,0.0f,1.0f);\n"
		"OUT=tex2D(DIFFUSE_SAMPLER,IN.uv);\n"
		"return OUT;\n"
	"}\n";
	if(stx_strcmp(aShaderName,"AmbientLighting")==0)
		return "[Vertex shader]"
	"ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Pos : POSITION;\n"
		"float2 uv : TEXCOORD2;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
	"float4 Pos: POSITION;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
	"Out.Pos = mul(float4(IN.Pos,1), WorldViewProjection); // transform Position\n"
	"return Out;\n"
	"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
	"float4 Pos: POSITION;\n"
	"};\n"
	"float4 main(VS_OUTPUT IN) : COLOR\n"
	"{\n"
	"return float4(0.5, 0.075, 0.075, 1.0);\n"
	"}";
	if(stx_strcmp(aShaderName,"DiffuseLighting")==0)
		return "[Vertex shader]"
	"ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"float4x4 World;\n"
	"float4 vecLightDir;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Pos : POSITION;\n"
		"float3 Normal : NORMAL;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
"Out.Pos = mul(float4(IN.Pos, 1), WorldViewProjection);\n"
"Out.Light = vecLightDir.xyz;\n"
"Out.Norm = normalize(mul(float4(IN.Normal, 1), World)).xyz;\n"
"return Out;\n"
"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
	"};\n"
"float4 main(VS_OUTPUT IN) : COLOR\n"
"{\n"
"float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f};\n"
"float4 ambient = {0.1, 0.0, 0.0, 1.0};\n"
"return ambient + diffuse * saturate(dot(IN.Light, IN.Norm));\n"
"}";
	if(stx_strcmp(aShaderName,"PhongLighting1")==0)
		return "[Vertex shader]"
	"ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
"float4x4 World;\n"
"float4 vecLightDir;\n"
"float4 vecEye;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Pos : POSITION;\n"
		"float3 Normal : NORMAL;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
		"float3 View : TEXCOORD2;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
"Out.Pos = mul(float4(IN.Pos,1), WorldViewProjection);\n"
"Out.Light = vecLightDir.xyz;\n"
"float3 PosWorld = normalize(mul(float4(IN.Pos,1), World)).xyz;\n"
"Out.View = vecEye.xyz - PosWorld;\n"
"Out.Norm = mul(float4(IN.Normal,1), World).xyz;\n"
"return Out;\n"
"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
		"float3 View : TEXCOORD2;\n"
	"};\n"
"float4 main(VS_OUTPUT IN) : COLOR\n"
"{\n"
"float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f};\n"
"float4 ambient = { 0.1f, 0.0f, 0.0f, 1.0f};\n"
"float3 Normal = normalize(IN.Norm);\n"
"float3 LightDir = normalize(IN.Light);\n"
"float3 ViewDir = normalize(IN.View);\n"
"float4 diff = saturate(dot(float4(Normal,1), LightDir));\n"
"float3 Reflect = normalize(2 * diff.xyz * Normal - LightDir);\n"
"float4 specular = float4(pow(saturate(dot(Reflect, ViewDir)), 8),1);\n"
"return ambient + diffuse * diff + specular;\n"
"}\n";
	if(stx_strcmp(aShaderName,"PhongLighting2")==00)
		return "[Vertex shader]"
	"ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
"float4x4 World;\n"
"float4 vecLightDir;\n"
"float4 vecEye;\n"
	"struct VS_INPUT\n"
	"{\n"
		"float3 Pos : POSITION;\n"
		"float3 Normal : NORMAL;\n"
	"};\n"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
		"float3 View : TEXCOORD2;\n"
	"};\n"
	"VS_OUTPUT main(VS_INPUT IN)\n"
	"{\n"
		"VS_OUTPUT Out = (VS_OUTPUT)0;\n"
"Out.Pos = mul(float4(IN.Pos,1), WorldViewProjection).xyz;\n"
"Out.Light = vecLightDir.xyz;\n"
"float3 PosWorld = normalize(mul(float4(IN.Pos,1), World)).xyz;\n"
"Out.View = vecEye.xyz - PosWorld;\n"
"Out.Norm = mul(float4(IN.Normal,1), World).xyz;\n"
"return Out;\n"
"}\n"
"[Fragment shader]"
	"struct VS_OUTPUT\n"
	"{\n"
		"float4 Pos : POSITION;\n"
		"float3 Light : TEXCOORD0;\n"
		"float3 Norm : TEXCOORD1;\n"
		"float3 View : TEXCOORD2;\n"
	"};\n"
"float4 main(VS_OUTPUT IN) : COLOR\n"
"{\n"
"float4 diffuse = { 1.0f, 0.0f, 0.0f, 1.0f};\n"
"float4 ambient = {0.1, 0.0, 0.0, 1.0};\n"
"float3 Normal = normalize(IN.Norm);\n"
"float3 LightDir = normalize(IN.Light);\n"
"float3 ViewDir = normalize(IN.View);\n"
"float4 diff = float4(saturate(dot(Normal, LightDir)),1);\n"
"float shadow = saturate(4* diff);\n"
"float3 Reflect = normalize(2 * diff.xyz * Normal - LightDir); // R\n"
"float4 specular = float4(pow(saturate(dot(Reflect, ViewDir)), 8),1);\n"
"return ambient + shadow * (diffuse * diff + specular);\n"
"}";
/*
		break;
	case eGLSL_Shader:
*/
#else
if((stx_strcmp(aShaderName,"DefaultEffect")==0)
|| (stx_strcmp(aShaderName,"PassThroughEffect")==0)
|| (stx_strcmp(aShaderName,"NormalsEffect")==0)
|| (stx_strcmp(aShaderName,"SimpleShader")==0)
|| (stx_strcmp(aShaderName,"lightning1")==0)
|| (stx_strcmp(aShaderName,"lightning2")==0)
|| (stx_strcmp(aShaderName,"AmbientLighting")==0)
|| (stx_strcmp(aShaderName,"DiffuseLighting")==0)
|| (stx_strcmp(aShaderName,"PhongLighting1")==0)
|| (stx_strcmp(aShaderName,"PhongLighting2")==0))
	return "[Vertex shader]\n"
	"//ROW_MAJOR float4x4 WorldViewProjection	MVPSEMANTIC;\n"
	"uniform mat4 WorldViewProjection;\n"
	"attribute vec3 aPosition;\n"
	"attribute vec2 auv;\n"
	"varying vec2 vuv;\n"
	"void main()\n"
	"{\n"
		"vec3 objPos = aPosition;\n"
		"gl_Position = WorldViewProjection * vec4( objPos, 1.0f);\n"
		"vuv=auv;\n"
	"}\n"
"[Fragment shader]\n"
	"//varying vec4 vPosition;\n"
	"varying vec2 vuv;\n"
	"uniform sampler2D DIFFUSE_SAMPLER;\n"
	"void main()\n"
	"{\n"
		"//gl_FragColor = vec4(0.0f,1.0f,0.0f,1.0f);\n"
		"gl_FragColor = texture2D(DIFFUSE_SAMPLER,vuv);\n"
	"}\n";
#endif
	//break;
	//};
	return 0;
}

