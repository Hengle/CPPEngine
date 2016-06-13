/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
#include <GUI/GUIUtils.h>

const bool mipmaps=false;
//define CUBEMAPS 1

#if defined(CUBEMAPS)
const unsigned int N=11;
const char* GUILayout="/SkyBox/GUILayout2.xml";
const char* filename[] =
{
"/SkyBox/cubemaps/x/DragonFireDX.dds",
"/SkyBox/cubemaps/x/MountainsDX.dds",
"/SkyBox/cubemaps/x/OlympusDX.dds",
"/SkyBox/cubemaps/x/DragonFireGL.dds",
"/SkyBox/cubemaps/x/MountainsGL.dds",
"/SkyBox/cubemaps/x/OlympusGL.dds",
"/SkyBox/cubemaps/CloudyHillsCubemap2.dds",
"/SkyBox/cubemaps/IncloudsCubemap.dds",
"/SkyBox/cubemaps/nvlobby_cube_mipmap.dds",
"/SkyBox/cubemaps/StPauls_CubeMap.dds",
"/SkyBox/cubemaps/StPauls_CubeMap_Compressed.dds",
0
};
#else
const unsigned int N=10;
const char* GUILayout="/SkyBox/GUILayout.xml";
const char* filename[] =
{
"cube",
"dots",       
"Mountains", 
"Olympus",   
"Terrain",
"Dragonfire",
"nvlobby",
//"Terrain_dark",
"Heightmap",
"StPauls",
//"skybox",
"Terrain2",
0
};
const char* ext[] =
{
"png",
"png",       
"jpg", 
"jpg",   
"png",
"jpg",
"png",
//"png",
"bmp",
"bmp",
//"jpg",
"png",
0
};
#endif

const char* shdTxt="[Vertex shader]\n"
"ROW_MAJOR float4x4 mvp MVPSEMANTIC;\n"
"struct VsOut {\n"
"	float4 pos:       POSITION;\n"
"	float3 cubeCoord: TEXCOORD0;\n"
"};\n"
"VsOut main(float4 pos: POSITION){\n"
"	VsOut Out;\n"
"	Out.pos = mul(mvp, pos);\n"
"	// Nvidia seems to have some kind of weird precision problem here\n"
"	Out.pos.z *= 0.99999;\n"
"	Out.cubeCoord = pos.xyz;\n"
"	return Out;\n"
"}\n"
"[Fragment shader]\n"
"samplerCUBE Env;\n"
"float4 fogColor;\n"
"float2 scaleBias;\n"
"float4 main(float3 cubeCoord: TEXCOORD0) : COLOR {\n"
"	float4 sky = texCUBE(Env, cubeCoord);\n"
"	float fog = saturate(normalize(cubeCoord).y * scaleBias.x + scaleBias.y);\n"
"	fog *= fog * (3 - 2 * fog);\n"
"	return lerp(fogColor, sky, fog);\n"
"}\n";

bool loadSlicedImage(Image3& img, const char **fileNames, const int nImages, const int nArraySlices = 1, uint flags = 0){
	int maxImage = nImages? nImages : 6;
		LOG_FNLN;
		LOG_PRINT("maxImage=%d\n", maxImage);
		LOG_PRINT("nArraySlices=%d\n", nArraySlices);

	Image3 *images = new Image3[maxImage * nArraySlices];

	for (int i = 0; i < maxImage * nArraySlices; i++){
		LOG_FNLN;
		LOG_PRINT("fileNames[%d]=%s\n", i, fileNames[i]);

		//if (!
		images[i].loadImage(fileNames[i], flags);
		if(0){
		LOG_FNLN;
			delete [] images;
			return false;
		}
		LOG_FNLN;
	}
		LOG_FNLN;

	uint nMipMaps = images[0].getMipMapCount();
	ubyte *dest = img.create(images[0].getFormat(), images[0].getWidth(), images[0].getHeight(), nImages, nMipMaps, nArraySlices);

	for (int arraySlice = 0; arraySlice < nArraySlices; arraySlice++){
		int base = arraySlice * maxImage;

		for (uint level = 0; level < nMipMaps; level++){
			int size = images[0].getMipMappedSize(level, 1);
			for (int i = 0; i < maxImage; i++){
				memcpy(dest, images[base + i].getPixels(level), size);
				dest += size;
			}
		}
	}

	delete [] images;
		LOG_FNLN;

	return true;
}

TextureID addCubemap(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState = SS_NONE, const int nArraySlices = 1, uint flags = 0)
{
		LOG_FNLN;
	Image3 img;
		LOG_FNLN;
	if (loadSlicedImage(img, fileNames, 0, nArraySlices))
	{
		LOG_FNLN;
		//if (img.getFormat() == FORMAT_RGBE8) img.unpackImage();
		if (useMipMaps && img.getMipMapCount() <= 1) img.createMipMaps();
		LOG_FNLN;
		TextureID id=IRenderer::GetRendererInstance()->addTexture(&img,true, samplerState, flags);
		LOG_FNLN;
		return id;
	}
	else
	{
		LOG_FNLN;
		char str[1024];
        int n = stx_snprintf(str,1024, "Couldn't open cubemap:\n");
		LOG_FNLN;
		for (int i = 0; i < 6 * nArraySlices; i++)
		{
            n += stx_snprintf(str + n,1024, "%s\n", fileNames[i]);
		}
		LOG_FNLN;
		LOG_PRINT(str);
		return TEXTURE_NONE;
	}
		LOG_FNLN;
}

class SkyBox: public STXGUI
{
D3DXFROMWINEVECTOR4 fogColor;
TextureID env[N];
ShaderID skyBox;
VertexFormatID skyboxVF;
D3DXFROMWINEVECTOR3 camPos;
D3DXFROMWINEMATRIX mvp;
unsigned int m_i;
public:
int init(unsigned int width, unsigned int height)
{
	m_i=0;
	
	//mvp=(1.5f, width, height, 0.001f, 2)
	D3DXFROMWINEMatrixPerspectiveFovLH(&mvp, D3DXFROMWINE_PI / 2.0f, float(IRenderer::GetRendererInstance()->GetViewportWidth())/float(IRenderer::GetRendererInstance()->GetViewportHeight()), 0.001f, 2.0f);

	camPos=D3DXFROMWINEVECTOR3(0.0f, 0.8f, 0.0f);
	fogColor = D3DXFROMWINEVECTOR4(1.0f, 0.5f, 0.1f, 1.0f);

	std::string res=stx_convertpath("/");		
		
	char *fileNames[6];
	fileNames[0]=new char[128];
	fileNames[1]=new char[128];
	fileNames[2]=new char[128];
	fileNames[3]=new char[128];
	fileNames[4]=new char[128];
	fileNames[5]=new char[128];

	unsigned int j=0;
	while(1)
	{
		LOG_FNLN;		
		if(filename[j]==0)
			break;

		LOG_FNLN;
	
		stx_snprintf(fileNames[0], 128, "%sSkyBox/%s/posx.%s", res.c_str(), filename[j], ext[j]);
		stx_snprintf(fileNames[1], 128, "%sSkyBox/%s/negx.%s", res.c_str(), filename[j], ext[j]);
		stx_snprintf(fileNames[2], 128, "%sSkyBox/%s/posy.%s", res.c_str(), filename[j], ext[j]);
		stx_snprintf(fileNames[3], 128, "%sSkyBox/%s/negy.%s", res.c_str(), filename[j], ext[j]);
		stx_snprintf(fileNames[4], 128, "%sSkyBox/%s/posz.%s", res.c_str(), filename[j], ext[j]);
		stx_snprintf(fileNames[5], 128, "%sSkyBox/%s/negz.%s", res.c_str(), filename[j], ext[j]);
		LOG_FNLN;		
		env[j] = addCubemap(fileNames, mipmaps, IRenderer::GetRendererInstance()->Gettrilinear());
		LOG_FNLN;
		j++;
	}
	
	skyBox = IRenderer::GetRendererInstance()->addHLSLShader(shdTxt, "main", "main");
	
	FormatDesc skyboxFmt[] = { 0, TYPE_VERTEX, FORMAT_FLOAT, 3 };
	
	skyboxVF = IRenderer::GetRendererInstance()->addVertexFormat(skyboxFmt, elementsOf(skyboxFmt), skyBox);
	
	STXGUI::init(GUILayout);
}

int render( )
{
	float f=128.0f/256.0f;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));
	IRenderer::GetRendererInstance()->BeginScene();

	float vertices[] = { 0, 2, 0, 0, -2, -1, -1, -2, 1, 1, -2, 1 };
	ushort indices[] = { 0, 1, 2, 3, 0, 1 };

	IRenderer::GetRendererInstance()->setShader(skyBox);
	IRenderer::GetRendererInstance()->setVertexFormat(skyboxVF);
	IRenderer::GetRendererInstance()->setTexture("Env", env[m_i], IRenderer::GetRendererInstance()->Gettrilinear());
	//IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthWrite());
	#if 1
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("mvp", I);
	#else
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("mvp", mvp);
	#endif
	IRenderer::GetRendererInstance()->setShaderConstant4f("fogColor", fogColor);
	IRenderer::GetRendererInstance()->setShaderConstant2f("scaleBias", D3DXFROMWINEVECTOR2(2 + 0.3f * camPos.y, camPos.y * 1.3f - 1.2f));

	IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLE_STRIP, 0, 4, 4, indices, indices, 2, vertices, vertices, sizeof(D3DXFROMWINEVECTOR3));

		char txt[128];
		stx_snprintf(txt, 128, "m_i=%d\n", m_i);
		IRenderer::GetRendererInstance()->drawText(txt, 10, 30, 
			15, 18,
			IRenderer::GetRendererInstance()->GetdefaultFont(), 
			IRenderer::GetRendererInstance()->GetlinearClamp(), 
			IRenderer::GetRendererInstance()->GetblendSrcAlpha(), 
			IRenderer::GetRendererInstance()->GetnoDepthTest());
	
	STXGUI::update();
	IRenderer::GetRendererInstance()->EndScene();
	IRenderer::GetRendererInstance()->Present( );

	return 0;
}
virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int           widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
    GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
    if(radioButton->isPressed() && radioButton->isChecked())
    {
	unsigned int i=0;
	while(1)
	{
		if(filename[i]==0)
			break;
      		if(callbackString == filename[i])
      		{
			m_i = i;
			break;
		}
		i++;
	}
    }
  }
}
};
int ApplicationLogic()
{	
	LOG_PRINT("sizeof(ushort)=%d\n", sizeof(ushort));
	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "SkyBox");		
	IInput*    i=STX_Service::GetInputInstance();
	SkyBox sb;
	sb.init(640, 512);
	while (!i->OnKeyPress (KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		sb.render();
	}
	return 0;
}

