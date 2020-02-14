import os, sys, re

def replaceString(outtext):

    outtext=re.sub(r'\bMessageBoxA\s*\(\s*0\s*\,\s*\"', 'LOG_PRINT(\"', outtext)
    outtext=re.sub(r'\bS_FALSE\b', '1', outtext)
    outtext=re.sub(r'\bLPCWSTR\b', 'char', outtext)
    outtext=re.sub(r'\bLPCSTR\b', 'char', outtext)
    outtext=re.sub(r'\bm_pD3DDevice\b', 'IRenderer::GetRendererInstance()', outtext)
    outtext=re.sub(r'\bm_pD3DContext\b', 'IRenderer::GetRendererInstance()', outtext)

    outtext=re.sub(r'\bD3DX', 'D3DXFROMWINE', outtext)
    outtext=re.sub(r'\bD3DXFROMWINEFROMWINE', 'D3DXFROMWINE', outtext)
    outtext=re.sub(r'\bTEXT\b', '',outtext)
    outtext=re.sub(r'\bTCHAR\b', 'char', outtext)
    outtext=re.sub(r'\bWCHAR\b', 'char', outtext)
    outtext=re.sub(r'\bNULL\b', '0', outtext)
    outtext=re.sub(r'\bHRESULT\b', 'int', outtext)
    outtext=re.sub(r'\bS_OK\b', '0', outtext)
    outtext=re.sub(r'\bV\s*\(', '(', outtext)
    outtext=re.sub(r'\bV_RETURN\s*\(', '(', outtext)
    outtext=re.sub(r'\bV_GET_VARIABLE_RETBOOL\s*\(', '(', outtext)
    outtext=re.sub(r'\bV_GET_VARIABLE_RET\s*\(', '(', outtext)

    outtext=re.sub(r'\bLPCTSTR\b', 'std::string', outtext)
    outtext=re.sub(r'\bL\"', '\"', outtext)
    outtext=re.sub(r'\s*CALLBACK\s*', ' ', outtext)
    outtext=re.sub(r'\bSAFE_RELEASE\b', '//SAFE_RELEASE', outtext)
    #outtext=re.sub(r'\bSAFE_DELETE\b', '', outtext)

    outtext=re.sub(r'\bUINT\b', 'unsigned int', outtext)

    cr='''/*
\tCopyright (c) 2020 Tommi Roenty   http://www.tommironty.fi/
\tLicensed under The GNU Lesser General Public License, version 2.1:
\t\thttp://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
'''
    if outtext.find("wWinMain")>0:
        cr+='''
class App: public STXGUI
{
public:
    App(){}
    virtual ~App(){}
    int init(const char* aTitle){STXGUI::init("/GUILayout.xml");}
    void render( ){STXGUI::update();}
'''
    main=''
    if outtext.find("wWinMain")>0:
        main+='''
virtual void actionPerformed(GUIEvent &evt)
{
  const NSString &callbackString  = evt.getCallbackString();
  GUIRectangle *sourceRectangle = evt.getEventSource();
  int widgetType      = sourceRectangle->getWidgetType();

  if(widgetType == RADIO_BUTTON)
  {
    GUIRadioButton   *radioButton = (GUIRadioButton*)sourceRectangle;
    if(radioButton->isPressed() && radioButton->isChecked())
    {
		if(callbackString == "")
      	{
		}
	}
	}
}
};
int ApplicationLogic()
{
	IRenderer* r=IRenderer::GetRendererInstance("");
	IInput*    i=STX_Service::GetInputInstance();
	App app;
	app.init("");
	while (!i->OnKeyPress (STX_KEY_ESCAPE) && !i->Quit())
	{
		i->Update();
		app.render();
	}
	return 0;
}
    '''

    txt=cr+outtext+main
    txt = txt.replace("wWinMain", "")

    return txt

def replaceStringInshdFile(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()
    outtext=replaceString(outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def myfunshd(dummy, dirr, filess):
	#print dummy, dirr, filess
	for child in filess:
	    #print child
	    
	    if '.shd' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
	    	    replaceStringInshdFile(dirr+'/'+child)

def replaceStringInDX9File(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()
    outtext=replaceString(outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def replaceStringInDX10File(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()
    outtext=replaceString(outtext)

    #outtext=re.sub(r'\b\b', '', outtext)
    '''
( ID3D10Device* IRenderer::GetRendererInstance(), 
LPCWSTR file,LPCSTR shaderTextureName, ID3D10Device* IRenderer::GetRendererInstance()
(IRenderer::GetRendererInstance(), 
(ID3D10Device* IRenderer::GetRendererInstance())
(ID3D11Device* IRenderer::GetRendererInstance(), ID3D11DeviceContext* IRenderer::GetRendererInstance(), 
(ID3D11Device* IRenderer::GetRendererInstance(), ID3D11DeviceContext *IRenderer::GetRendererInstance())
ID3DX10MeshBuffer\b', 'exBuffer', outtext)
ID3DX11EffectTechnique
(ID3D11Device* IRenderer::GetRendererInstance())
(ID3D11Device* IRenderer::GetRendererInstance(), ID3D11DeviceContext* IRenderer::GetRendererInstance())
    '''

    outtext=re.sub(r'\bID3D10Device* pd3dDevice\s*\)', ')', outtext)
    outtext=re.sub(r'\(\s*ID3D10DeviceContext\s*\*\s*pd3dContext\s*\,\s*', '(', outtext)
    outtext=re.sub(r'\bpd3dDevice\b', 'IRenderer::GetRendererInstance()', outtext)
    outtext=re.sub(r'\bpd3dContext\b', 'IRenderer::GetRendererInstance()', outtext)
    outtext=re.sub(r'\(ID3D10Device* pd3dDevice, ID3D10DeviceContext* pd3dContext\b', '(', outtext)
    outtext=re.sub(r'\, void* pUserContext\)', ')', outtext)

    #outtext=re.sub(r'\bIASetPrimitiveTopology\b', '', outtext)

    outtext=re.sub(r'\bIASetVertexBuffers\b', 'setVertexBuffer', outtext)
    outtext=re.sub(r'\bIASetIndexBuffer\b', 'setIndexBuffer', outtext)

    outtext=re.sub(r'\bCModelViewerCamera\s+g_Camera\s*\;', 'D3DXFROMWINEMATRIX g_CameraEyePt;\nD3DXFROMWINEMATRIX g_CameraProj;\nD3DXFROMWINEMATRIX g_CameraView;\nD3DXFROMWINEMATRIX g_CameraWorld;\n', outtext)

    outtext=re.sub(r'\bCDXUTSDKMesh\b', 'MeshRenderer2', outtext)
    outtext=re.sub(r'\bCDXUTMesh10\b', 'MeshRenderer2', outtext)

	#PRIM_TRIANGLE_FAN
	#PRIM_QUADS
	#PRIM_LINE_LOOP

    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ\b', 'PRIM_LINES /* _ADJ */', outtext)
    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST\b', 'PRIM_TRIANGLES', outtext)
    #outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_UNDEFINED\b', 'PRIM_', outtext)
    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP\b', 'PRIM_TRIANGLE_STRIP', outtext)
    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_LINELIST\b', 'PRIM_LINES', outtext)
    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_LINESTRIP\b', 'PRIM_LINE_STRIP', outtext)
    outtext=re.sub(r'\bD3D10_PRIMITIVE_TOPOLOGY_POINTLIST\b', 'PRIM_POINTS', outtext)

    outtext=re.sub(r'\bID3D10RasterizerState\b', 'RasterizerStateID', outtext)
    #outtext=re.sub(r'\bD3DX10_PASS_DESC\b', '', outtext)
    outtext=re.sub(r'\bID3D10RenderTargetView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D10DepthStencilView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D10Buffer\b', 'exBufferID', outtext)
    outtext=re.sub(r'\bID3D10ShaderResourceView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D10SamplerState\b', 'SamplerStateID', outtext)
    outtext=re.sub(r'\bD3D10_RENDER_TARGET_VIEW_DESC\b', 'SURFACE_DESC', outtext)
    outtext=re.sub(r'\bID3D10ComputeShader\b', 'ComputeShaderID', outtext)

    #outtext=re.sub(r'\bID3D10UnorderedAccessView\b', '', outtext)
    #outtext=re.sub(r'\bD3D10_INPUT_ELEMENT_DESC\b', '', outtext)

    outtext=re.sub(r'\bID3DX10Effect\b', 'EffectID', outtext)
    outtext=re.sub(r'\bID3D10Effect\b', 'EffectID', outtext)

    outtext=re.sub(r'\b ID3DX10EffectTechnique\b', 'stx_Technique', outtext)
    outtext=re.sub(r'\bID3DX10EffectScalarVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX10EffectVectorVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX10EffectShaderResourceVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX10EffectMatrixVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3D10Texture1D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D10Texture2D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D10Texture3D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bD3D10_VIEWPORT\b', 'D3DFROMWINEVIEWPORT9', outtext)
    outtext=re.sub(r'\bD3D10_TEXTURE2D_DESC\b', 'SURFACE_DESC', outtext)
    outtext=re.sub(r'\bID3D10InputLayout*\b', 'VertexFormatID', outtext)

    outtext=re.sub(r'\bID3D10EffectScalarVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3D10EffectVectorVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3D10EffectMatrixVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3D10EffectTechnique\b', 'stx_Technique', outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def replaceStringInDX11File(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()
    outtext=replaceString(outtext)

    #outtext=re.sub(r'\b\b', '', outtext)

    outtext=re.sub(r'\bID3D11Device* pd3dDevice\s*\)', ')', outtext)
    outtext=re.sub(r'\(\s*ID3D11DeviceContext\s*\*\s*pd3dContext\s*\,\s*', '(', outtext)
    outtext=re.sub(r'\(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dContext\b', '(', outtext)
    outtext=re.sub(r'\bpd3dDevice\b', 'IRenderer::GetRendererInstance()', outtext)
    outtext=re.sub(r'\bpd3dContext\b', 'IRenderer::GetRendererInstance()', outtext)
    outtext=re.sub(r'\, void* pUserContext\)', ')', outtext)

    #outtext=re.sub(r'\bIASetPrimitiveTopology\b', '', outtext)

    outtext=re.sub(r'\bIASetVertexBuffers\b', 'setVertexBuffer', outtext)
    outtext=re.sub(r'\bIASetIndexBuffer\b', 'setIndexBuffer', outtext)

    outtext=re.sub(r'\bCModelViewerCamera\s+g_Camera\s*\;', 'D3DXFROMWINEMATRIX g_CameraEyePt;\nD3DXFROMWINEMATRIX g_CameraProj;\nD3DXFROMWINEMATRIX g_CameraView;\nD3DXFROMWINEMATRIX g_CameraWorld;\n', outtext)

    outtext=re.sub(r'\bCDXUTSDKMesh\b', 'MeshRenderer2', outtext)
    outtext=re.sub(r'\bCDXUTMesh11\b', 'MeshRenderer2', outtext)

	#PRIM_TRIANGLE_FAN
	#PRIM_QUADS
	#PRIM_LINE_LOOP

    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ\b', 'PRIM_LINES /* _ADJ */', outtext)
    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST\b', 'PRIM_TRIANGLES', outtext)
    #outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_UNDEFINED\b', 'PRIM_', outtext)
    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP\b', 'PRIM_TRIANGLE_STRIP', outtext)
    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_LINELIST\b', 'PRIM_LINES', outtext)
    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_LINESTRIP\b', 'PRIM_LINE_STRIP', outtext)
    outtext=re.sub(r'\bD3D11_PRIMITIVE_TOPOLOGY_POINTLIST\b', 'PRIM_POINTS', outtext)

    outtext=re.sub(r'\bID3D11RasterizerState\b', 'RasterizerStateID', outtext)
    #outtext=re.sub(r'\bD3DX11_PASS_DESC\b', '', outtext)
    outtext=re.sub(r'\bID3D11RenderTargetView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D11DepthStencilView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D11Buffer\b', 'exBufferID', outtext)
    outtext=re.sub(r'\bID3D11ShaderResourceView\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D11SamplerState\b', 'SamplerStateID', outtext)
    outtext=re.sub(r'\bD3D11_RENDER_TARGET_VIEW_DESC\b', 'SURFACE_DESC', outtext)
    outtext=re.sub(r'\bID3D11ComputeShader\b', 'ComputeShaderID', outtext)

    #outtext=re.sub(r'\bID3D11UnorderedAccessView\b', '', outtext)
    #outtext=re.sub(r'\bD3D11_INPUT_ELEMENT_DESC\b', '', outtext)

    outtext=re.sub(r'\bID3DX11Effect\b', 'EffectID', outtext)
    outtext=re.sub(r'\bID3D11Effect\b', 'EffectID', outtext)

    outtext=re.sub(r'\b ID3DX11EffectTechnique\b', 'stx_Technique', outtext)
    outtext=re.sub(r'\bID3DX11EffectScalarVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX11EffectVectorVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX11EffectShaderResourceVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3DX11EffectMatrixVariable\b', 'stx_Variable', outtext)
    outtext=re.sub(r'\bID3D11Texture1D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D11Texture2D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bID3D11Texture3D\b', 'TextureID', outtext)
    outtext=re.sub(r'\bD3D11_VIEWPORT\b', 'D3DFROMWINEVIEWPORT9', outtext)
    outtext=re.sub(r'\bD3D11_TEXTURE2D_DESC\b', 'SURFACE_DESC', outtext)
    outtext=re.sub(r'\bID3D11InputLayout*\b', 'VertexFormatID', outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def replaceStringInGLFile(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()
    outtext=replaceString(outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def replaceStringInFile(filePath):
    "replaces all string by a regex substitution"
    tempName=filePath+'~~~'
    input = open(filePath)
    outtext=input.read()
    input.close()

    outtext=re.sub(r'\bTEXT\b', '',outtext)

    output = open(tempName,'w')
    output.write(outtext)
    output.close()
    os.unlink(filePath)
    os.rename(tempName,filePath)

def myfunDX9(dummy, dirr, filess):
	for child in filess:
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX9File(dirr+'/'+child)
			print child
	for child in filess:
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX9File(dirr+'/'+child)
			print child

def myfunDX10(dummy, dirr, filess):
	for child in filess:
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX10File(dirr+'/'+child)
			print child
	for child in filess:
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX10File(dirr+'/'+child)
			print child

def myfunDX11(dummy, dirr, filess):
	for child in filess:
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX11File(dirr+'/'+child)
			print child
	for child in filess:
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInDX11File(dirr+'/'+child)
			print child

def myfunGL(dummy, dirr, filess):
	for child in filess:
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInGLFile(dirr+'/'+child)
			print child
	for child in filess:
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInGLFile(dirr+'/'+child)
			print child

def myfun(dummy, dirr, filess):
	for child in filess:
		if '.cpp' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInFile(dirr+'/'+child)
			print child
		if '.h' == os.path.splitext(child)[1] and os.path.isfile(dirr+'/'+child):
			replaceStringInFile(dirr+'/'+child)
			print child

os.system("rm -fr ../Samples/new/DX11/Hair")
os.system("rm -fr ../Samples/new/DX10/FurShellsAndFins")
os.system("cp -fr ../Samples/new/DX11/Hair_ ../Samples/new/DX11/Hair")
os.system("cp -fr ../Samples/new/DX10/FurShellsAndFins_ ../Samples/new/DX10/FurShellsAndFins")
DX11=['../Samples/new/DX11/Hair']#['../Samples/new/DX11/DetailTessellation11', '../Samples/new/DX11/OceanCS', '../Samples/new/new/DX11/nBodyCS', '../Samples/new/new/DX11/NBodyGravityCS11']
DX10=['../Samples/new/DX10/FurShellsAndFins']#['../Samples/new/DX10/Global_Illumination', '../Samples/new/DX10/GrassInstancing', '../Samples/new/DX10/Skinning10', '../Samples/new/new/DX10/Rain', '../Samples/new/new/DX10/NBodyGravity', '../Samples/new/DX10/SkinAnimation']
DX9=[]#['../Samples/new/DX9/HDR', '../Samples/new/DX9/HDRCubeMap', '../Samples/new/DX9/HDRFormats', '../Samples/new/DX9/HDRLighting', '../Samples/new/DX9/HDRPipeline']
GL=[]#['../Samples/new_/R2VB_VTF/18', '../Samples/new/GL/ParallaxMappingOpenGL', '../Samples/new/GL/hdr']
'''
Files=['../Libraries/Framework3']
Files=['../Libraries/IndieLib/IndieLib/Common']
v=['../../IndieLib_resources/www.shadertoy.com']
for i in v:
	os.path.walk(i, myfunshd, 13)
exit(0)

for i in Files:
	os.path.walk(i, myfun, 13)
for i in GL:
	os.path.walk(i, myfunGL, 13)
for i in DX9:
	os.path.walk(i, myfunDX9, 13)
'''
for i in DX10:
	os.path.walk(i, myfunDX10, 13)
for i in DX11:
	os.path.walk(i, myfunDX11, 13)

