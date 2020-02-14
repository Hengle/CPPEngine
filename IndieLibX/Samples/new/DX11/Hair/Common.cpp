/*
	Copyright (c) 2020 Tommi Roenty   http://www.tommironty.fi/
	Licensed under The GNU Lesser General Public License, version 2.1:
		http://opensource.org/licenses/LGPL-2.1
*/
#include <Framework3/IRenderer.h>
// Copyright (c) 2011 NVIDIA Corporation. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, NONINFRINGEMENT,IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA 
// OR ITS SUPPLIERS BE  LIABLE  FOR  ANY  DIRECT, SPECIAL,  INCIDENTAL,  INDIRECT,  OR  
// CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS 
// OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY 
// OTHER PECUNIARY LOSS) ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, 
// EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
// Please direct any bugs or questions to SDKFeedback@nvidia.com

#include <tchar.h>
#include <AtlBase.h>
#include <Strsafe.h>
#include "Common.h"



int WINAPI NVUTFindDXSDKMediaFileCch( char* strDestPath, int cchDest, char strFilename )
{
    int hr = DXUTFindDXSDKMediaFileCch( strDestPath, cchDest, strFilename );

    if (FAILED(hr))
    {
        char strFullError[MAX_PATH] = "The file ";
        StringCchCat( strFullError, MAX_PATH, strFilename );
        StringCchCat( strFullError, MAX_PATH, " was not found in the search path.\nCannot continue - exiting." );

        MessageBoxW(0, strFullError, "Media file not found", MB_OK);
        exit(0);
    }

    return hr;
}

int WINAPI NVUTFindDXSDKMediaFileCchT( LPTSTR strDestPath, int cchDest, std::string strFilename )
{
    int hr(0);
    USES_CONVERSION;

    CT2W wstrFileName(strFilename);
    LPWSTR wstrDestPath = new char[cchDest];
    
    hr = NVUTFindDXSDKMediaFileCch(wstrDestPath, cchDest, wstrFileName);
    
    if(!FAILED(hr))
    {
        LPTSTR tstrDestPath = W2T(wstrDestPath);
        _tcsncpy_s(strDestPath, cchDest, tstrDestPath, cchDest);
    }

    delete[] wstrDestPath;

    return hr;
}


#include <d3dx9shader.h>
int MyCreateEffectFromFile(char *fName, DWORD dwShaderFlags, ID3D11Device *pDevice, EffectID **pEffect)
{
    char fNameChar[1024];
	CharToOem(fName, fNameChar);
	ID3DXBuffer *pShader;
	int hr;
	ID3DXBuffer *pErrors;
	if (FAILED(D3DXFROMWINECompileShaderFromFileA(fNameChar, 0, 0, 0, "fx_5_0", 0, &pShader, &pErrors, 0)))
	{
		MessageBoxA(0, (char *)pErrors->GetBufferPointer(), "Error", MB_OK);
		return E_FAIL;
	}
    (D3DXFROMWINE11CreateEffectFromMemory(pShader->GetBufferPointer(), pShader->GetBufferSize(), dwShaderFlags, pDevice, pEffect));

	//SAFE_RELEASE(pErrors);
	//SAFE_RELEASE(pShader);

    return 0;
}


int MyCreateEffectFromCompiledFile(char *fName, DWORD dwShaderFlags, ID3D11Device *pDevice, EffectID **pEffect)
{
	int hr;
	
	char filename[MAX_PATH];
	wcstombs_s(0,filename,MAX_PATH,fName,MAX_PATH);

	FILE * pFile;
	errno_t error;
	if((error = fopen_s(&pFile,filename,"rb")) != 0)
	{
        MessageBoxA(0,"Could not find compiled effect file","Error",MB_OK);
		return 1;
	}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    int lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    char* buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == 0) return 1; 

    // copy the file into the buffer:
    size_t result = fread (buffer,sizeof(char),lSize,pFile);
    if ((int)result != lSize) return 1;

    //create the effect
    (D3DXFROMWINE11CreateEffectFromMemory((void*) buffer,  result*sizeof(char) , dwShaderFlags, pDevice, pEffect));

    fclose (pFile);

    free (buffer);

    return 0;
}


int CompileShaderFromFile( char* szFileName, char szEntryPoint, char szShaderModel, ID3DBlob** ppBlobOut )
{
    int hr = 0;

    // find the file
    char str[MAX_PATH];
    ( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, szFileName ) );

    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DXFROMWINE11CompileFromFile( str, 0, 0, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, 0, ppBlobOut, &pErrorBlob, 0 );
    if( FAILED(hr) )
    {
        if( pErrorBlob != 0 )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        //SAFE_RELEASE( pErrorBlob );
        return hr;
    }
    //SAFE_RELEASE( pErrorBlob );

    return 0;
}


int LoadComputeShader(ID3D11Device* IRenderer::GetRendererInstance(), char *szFilename, char szEntryPoint, char szShaderModel, ComputeShaderID **ppComputeShader)
{
	int hr;
	ID3DBlob* pBlob = 0;
	( CompileShaderFromFile( szFilename, szEntryPoint, "cs_5_0", &pBlob ) );
	( IRenderer::GetRendererInstance()->CreateComputeShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0, ppComputeShader ) );
	//SAFE_RELEASE( pBlob );
	return hr;
}
