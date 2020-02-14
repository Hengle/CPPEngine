/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>

ShaderID shd=-1;
TextureID texture=-1;
VertexFormatID vf=-1;

int init(const char* aTitle)
{
#if 0
    {
	Image3 img;
	ubyte *dest = (ubyte *) img.create(FORMAT_RGB8, 256, 256, 256, 1);
	for (int z = 0; z < 256; z++){
		for (int y = 0; y < 256; y++){
		for (int x = 0; x < 256; x++){
            int i=x*y*z;
		dest[3 * i + 0] = x;
		dest[3 * i + 1] = y;
		dest[3 * i + 2] = z;

    }}}
	img.saveDDS("./RGB3D.dds");
	img.savePNG("./RGB3D.png");
    //stx_exit(0);
    }
#endif
	//XFileParser();
//LOG_FNLN;
	shd = IRenderer::GetRendererInstance()->addShaderFromFile("/SimpleTexture/SimpleTexture.shd", "mainVS", "mainPS");
//LOG_FNLN;
	FormatDesc format[] =
	{
		0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
	};
//LOG_FNLN;
	vf = IRenderer::GetRendererInstance()->addVertexFormat(format, elementsOf(format), shd);
//LOG_FNLN;
	texture=IRenderer::GetRendererInstance()->addImageLibTexture("/test.bmp", false, IRenderer::GetRendererInstance()->Getlinear());
//LOG_FNLN;
	//texture=IRenderer::GetRendererInstance()->addImageLibTexture("/treetextures/tree35s.dds", false, ss);

	//stx_OBJFile2XFile("../../IndieLib_resources/blender_3ds/cube.obj");
#if 0//def LINUX
    stx_Effect* pEffect=IRenderer::GetRendererInstance()->addEffectFromFile("/new/x/Island11/Island11.fx");
#endif
	return 0;
}

void render()
{
	float f=128.0f/256.0f;

//LOG_FNLN;
	IRenderer::GetRendererInstance()->Clear(true, true, D3DXFROMWINEVECTOR4 (f, f, f, 1.0f));

//LOG_FNLN;
	IRenderer::GetRendererInstance()->BeginScene();

//LOG_FNLN;
	IRenderer::GetRendererInstance()->setShader(shd);

//LOG_FNLN;
	IRenderer::GetRendererInstance()->setVertexFormat(vf);

//LOG_FNLN;
	IRenderer::GetRendererInstance()->setTexture("Base", texture);

//LOG_FNLN;
	D3DXFROMWINEVECTOR4 color(0.6f, 0.6f, 0.6f, 1.0f);
	IRenderer::GetRendererInstance()->setShaderConstant4f("color", color);

//LOG_FNLN;
	D3DXFROMWINEMATRIX I;
	D3DXFROMWINEMatrixIdentity(&I);

//LOG_FNLN;
	IRenderer::GetRendererInstance()->setShaderConstant4x4f("worldViewProj", I);

//LOG_FNLN;
	IRenderer::GetRendererInstance()->setDepthState(IRenderer::GetRendererInstance()->GetnoDepthTest());

//LOG_FNLN;

	//		           x	 y     tx    ty
	float v[16] =  {  1.0f,-1.0f, 1.0f, 1.0f,
			          1.0f, 1.0f, 1.0f, 0.0f,
			         -1.0f,-1.0f, 0.0f, 1.0f,
			         -1.0f, 1.0f, 0.0f, 0.0f };
	__WORD__ i[6] ={0,1,2,2,1,3};
#if 1
//LOG_FNLN;
	IRenderer::GetRendererInstance()->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &v[0], &v[0], 4*sizeof(float));
#else
	IRenderer::GetRendererInstance()->DrawIndexedPrimitiveUP(PRIM_TRIANGLES, 0, 4, 2, &i[0], &i[0], CONSTANT_INDEX2, &v[0], &v[0], 4*sizeof(float));
#endif
//LOG_FNLN;
	IRenderer::GetRendererInstance()->EndScene();

//LOG_FNLN;
	IRenderer::GetRendererInstance()->Present( );

//LOG_FNLN;
	//stx_exit(0);
}

int ApplicationLogic()
{
//LOG_FNLN;
#if 0
#ifndef _MSC_VER
									//	Ubuntu: Ubuntu:	Darwin:	Windows10:
									//	32-bit  64-bit
	LOG_PRINT("sizeof(char)=%ld\n", sizeof(char));			//	1	1	1	1
	LOG_PRINT("sizeof(unsigned char)=%ld\n", sizeof(unsigned char));//	1	1	1	1
	LOG_PRINT("sizeof(unsigned short)=%ld\n", sizeof(unsigned short));//	2	2	2	2
	LOG_PRINT("sizeof(unsigned int)=%ld\n", sizeof(unsigned int));	//	4	4	4	4
	LOG_PRINT("sizeof(unsigned long)=%ld\n", sizeof(unsigned long));//	4	8	8	4
	LOG_PRINT("sizeof(WORD)=%ld\n", sizeof(WORD));			//	2	2	2	2
	LOG_PRINT("sizeof(DWORD)=%ld\n", sizeof(DWORD));		//	4	4	4	4
	LOG_PRINT("sizeof(UCHAR)=%ld\n", sizeof(UCHAR));		//	1	1	1	1
	LOG_PRINT("sizeof(USHORT)=%ld\n", sizeof(USHORT));		//	2	2	2	2
	LOG_PRINT("sizeof(UINT)=%ld\n", sizeof(UINT));			//	4	4	4	4
	LOG_PRINT("sizeof(ULONG)=%ld\n", sizeof(ULONG));		//	4	4	8	4
	LOG_PRINT("sizeof(float)=%ld\n", sizeof(float));		//	4	4	4	4
	LOG_PRINT("sizeof(double)=%ld\n", sizeof(double));		//	8	8	8	8
#else
  ofstream myfile ("typesizes.txt");
  if (myfile.is_open())
  {
	myfile << "sizeof(char)=%ld\n" << sizeof(char)  << '\n';			//	1	1	1	1
	myfile << "sizeof(unsigned char)=%ld\n" << sizeof(unsigned char)  << '\n';	//	1	1	1	1
	myfile << "sizeof(unsigned short)=%ld\n" << sizeof(unsigned short)  << '\n';	//	2	2	2	2
	myfile << "sizeof(unsigned int)=%ld\n" << sizeof(unsigned int)  << '\n';	//	4	4	4	4
	myfile << "sizeof(unsigned long)=%ld\n" << sizeof(unsigned long)  << '\n';	//	4	8	8	4
	myfile << "sizeof(WORD)=%ld\n" << sizeof(WORD)  << '\n';			//	2	2	2	2
	myfile << "sizeof(DWORD)=%ld\n" << sizeof(DWORD)  << '\n';			//	4	4	4	4
	myfile << "sizeof(UCHAR)=%ld\n" << sizeof(UCHAR)  << '\n';			//	1	1	1	1
	myfile << "sizeof(USHORT)=%ld\n" << sizeof(USHORT)  << '\n';			//	2	2	2	2
	myfile << "sizeof(UINT)=%ld\n" << sizeof(UINT)  << '\n';			//	4	4	4	4
	myfile << "sizeof(ULONG)=%ld\n" << sizeof(ULONG)  << '\n';			//	4	4	8	4
	myfile << "sizeof(float)=%ld\n" << sizeof(float)  << '\n';			//	4	4	4	4
	myfile << "sizeof(double)=%ld\n" << sizeof(double)  << '\n';			//	8	8	8	8
    myfile.close();
  }
#endif	
#endif	
//LOG_FNLN;
	IRenderer* r=IRenderer::GetRendererInstance("SimpleTexture");	
//LOG_FNLN;
	IInput*    i=STX_Service::GetInputInstance();
//LOG_FNLN;
	init("");
//LOG_FNLN;
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{

//LOG_FNLN;
		i->Update();

//LOG_FNLN;
		render();

//LOG_FNLN;
	}
//LOG_FNLN;
	return 0;
}

