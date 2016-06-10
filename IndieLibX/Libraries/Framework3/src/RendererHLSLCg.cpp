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

#include <STX/STX.h>

//include <Framework3/OpenGLExtensions.h>
#include <Framework3/IRenderer.h>
//using namespace CG;
//include <ResourceReuseCache.h>

#if !defined( _WIN32 ) && !defined(SDL_2_0_4_BUILD) && !defined(DBG_HALT)
#define DBG_HALT
#endif

#if 0//defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define LOG_FNLN_X LOG_FNLN
#define LOG_PRINT_X LOG_PRINT
#else
#define LOG_FNLN_X
#define LOG_PRINT_X
#endif

#if 0//defined(ANDROID)
#ifdef __cplusplus
extern "C" {
#endif
int init(unsigned int width, unsigned int height);
	void init3(unsigned int width, unsigned int height)
{
    IRenderer* r=IRenderer::GetRendererInstance(width, height, "SimpleTexture");		
	IInput*    i=STX_Service::GetInputInstance();
	init(width, height);
}

	void loop3()
{
	while (!STX_Service::GetInputInstance()->OnKeyPress (KEY_ESCAPE) && !STX_Service::GetInputInstance()->Quit())
	{
		STX_Service::GetInputInstance()->Update();
		render();
	}
}
JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeInit_(JNIEnv* env, jclass jcls, jint width, jint height)
{
    init3(width, height);
}
JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeLoop(JNIEnv* env, jclass jcls)
{
    loop3();
}
#ifdef __cplusplus
	};
#endif
#endif



#if 0//defined(ANDROID)
#ifdef __cplusplus
extern "C" {
#endif
static volatile int doterminate;
void closemutex(int sig);
static SDL_threadID mainthread;
int SDLCALL
Run1(void *data)
{
    if (SDL_ThreadID() == mainthread)
        signal(SIGTERM, closemutex);
    while (!doterminate) {
        LOG_PRINT_X("Process %lu ready to work\n", SDL_ThreadID());
        if (SDL_LockMutex(mutex) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
            stx_exit(1);
        }
        LOG_PRINT_X("Process %lu, working!\n", SDL_ThreadID());

	IRenderer* r=IRenderer::GetRendererInstance(640, 512, "NDK_SDL2");
	IInput*    i=STX_Service::GetInputInstance();

        LOG_PRINT_X("Process %lu, done!\n", SDL_ThreadID());
        if (SDL_UnlockMutex(mutex) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            stx_exit(1);
        }
        /* If this sleep isn't done, then threads may starve */
        SDL_Delay(10);
    }
    if (SDL_ThreadID() == mainthread && doterminate) {
        LOG_PRINT_X("Process %lu:  raising SIGTERM\n", SDL_ThreadID());
        raise(SIGTERM);
    }
    return (0);
}
int SDLCALL
Run3(void *data)
{
    if (SDL_ThreadID() == mainthread)
        signal(SIGTERM, closemutex);
    while (!doterminate) {
        LOG_PRINT_X("Process %lu ready to work\n", SDL_ThreadID());
        if (SDL_LockMutex(mutex) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't lock mutex: %s", SDL_GetError());
            stx_exit(1);
        }
        LOG_PRINT_X("Process %lu, working!\n", SDL_ThreadID());
	while (!STX_Service::GetInputInstance()->OnKeyPress (KEY_ESCAPE) && !STX_Service::GetInputInstance()->Quit())
	{
		STX_Service::GetInputInstance()->Update();
		render();
	}
        LOG_PRINT_X("Process %lu, done!\n", SDL_ThreadID());
        if (SDL_UnlockMutex(mutex) < 0) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't unlock mutex: %s", SDL_GetError());
            stx_exit(1);
        }
        /* If this sleep isn't done, then threads may starve */
        SDL_Delay(10);
    }
    if (SDL_ThreadID() == mainthread && doterminate) {
        LOG_PRINT_X("Process %lu:  raising SIGTERM\n", SDL_ThreadID());
        raise(SIGTERM);
    }
    return (0);
}
#ifdef __cplusplus
	};
#endif
#endif

	bool IRenderer::SDL=false;
//namespace STX
//{

//};
// ImageLib lib (for loading images and applying filters)

unsigned int IRenderer::width=800;
unsigned int IRenderer::height=600;
#ifdef _MSCVER
unsigned int IRenderer::bpp=32;
#else
unsigned int IRenderer::bpp=24;
#endif


// --------------------------------------------------------------------------------
//									 Private methods
// --------------------------------------------------------------------------------

/*
*************************
Initialized the input flags
*************************
*/
Shader::~Shader(){}

	IRenderer::~IRenderer()
	{
	};

#include <Framework3/RendererHLSLCg.h>
D3DXFROMWINEVECTOR4 g_scaleBias2D=D3DXFROMWINEVECTOR4(0.0f,0.0f,0.0f,0.0f);
D3DXFROMWINEVECTOR4& RendererHLSLCg::GetscaleBias2D()
{
	return g_scaleBias2D;
}
void RendererHLSLCg::InitTex(ShaderID& s, VertexFormatID& vf)
{
	if(vf==-1)
	{
		const char *texSHD = Framework3ShaderFactory::GetShader("texSHD0");
		s = addHLSLShader(texSHD, "main", "main");
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
			0, TYPE_TEXCOORD, FORMAT_FLOAT, 2
		}
		;
		vf = addVertexFormat(format, elementsOf(format), s);
	}
}
void RendererHLSLCg::InitPlain(ShaderID& s, VertexFormatID& vf)
{
	if(vf==-1)
	{
		const char *plainSHD = Framework3ShaderFactory::GetShader("plainSHD1");
		s = addHLSLShader(plainSHD, "main", "main");
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 2,
		}
		;
		vf = addVertexFormat(format, elementsOf(format), s);
	}
}
void RendererHLSLCg::InitPlain3(ShaderID& s, VertexFormatID& vf)
{
	if(vf==-1)
	{
		const char *plainSHD = Framework3ShaderFactory::GetShader("plainSHD2");
		s = addHLSLShader(plainSHD, "main", "main");
		FormatDesc format[] =
		{
			0, TYPE_VERTEX,   FORMAT_FLOAT, 3,
		}
		;
		vf = addVertexFormat(format, elementsOf(format), s);
	}
}
int IRenderer::fps=0;
#define FPS 60
Uint32 waittime = 1000.0f/FPS;
Uint32 framestarttime = 0;
Sint32 delaytime;

void IRenderer::OnEndScene()
{
	static bool bRender=false;
	if(STX_Service::GetInputInstance()->IsKeyPressed(KEY_p))
		bRender=!bRender;
	if(!bRender)
		return;
	int vpw=GetViewportWidth();
	int vph=GetViewportHeight()/2;
	unsigned int n=GetTextureCount();
	float ss=32.0f;
	
	unsigned int ny=vph/ss;
	unsigned int nx=vpw/ss;
	TextureID c=0;
	for(unsigned int xx=0;xx<nx;xx++)
	for(unsigned int yy=0;yy<ny;yy++)
	{
	if(c>n) continue;
	D3DXFROMWINEVECTOR2 aposition(ss*xx,ss*yy);
	D3DXFROMWINEVECTOR2 asize(ss,ss);
	TexVertex dest[4];
	float x=aposition.x;
	float y=aposition.y;
	dest[0].position = D3DXFROMWINEVECTOR2(x + asize.x, y);
	dest[0].texCoord = D3DXFROMWINEVECTOR2(1.0f, 0.0f);
	dest[1].position = D3DXFROMWINEVECTOR2(x + asize.x, y + asize.y);
	dest[1].texCoord = D3DXFROMWINEVECTOR2(1.0f, 1.0f);
	dest[2].position = D3DXFROMWINEVECTOR2(x, y);
	dest[2].texCoord = D3DXFROMWINEVECTOR2(0.0f, 0.0f);
	dest[3].position = D3DXFROMWINEVECTOR2(x, y + asize.y);
	dest[3].texCoord = D3DXFROMWINEVECTOR2(0.0f, 1.0f);
	drawTextured(PRIM_TRIANGLE_STRIP, dest, 2, c++, GetblendSrcAlpha(), GetnoDepthTest());
	}	
}

void IRenderer::OnPresent(IInput* Input)
{
	//
	static float accTime = 0.1f;
	static int nFrames = 0;
	if (accTime > 0.1f)
	{
		fps = int (nFrames / accTime );
		nFrames = 0;
		accTime = 0;
	}

    static float time00=timeGetTime()/1000.0f;
    float time11=timeGetTime()/1000.0f;
	float frameTime=time11-time00;
	time00=time11;
	accTime += frameTime;
	nFrames++;
	if(Input) Input->Update ();

	static int w=STX_Service::GetWindowInstance()->Getwidth();
	static int h=STX_Service::GetWindowInstance()->GetHeight();
	if((STX_Service::GetWindowInstance()->Getwidth()!=w)||
	   (STX_Service::GetWindowInstance()->Getheight()!=h))
	{
		w=STX_Service::GetWindowInstance()->Getwidth();

		h=STX_Service::GetWindowInstance()->GetHeight();
		s_pRenderer->setViewport(0,0,w,h);
	}
/*
bool done = 0;
while(!done)
{
    while(SDL_PollEvent(&event))
    {
        ...
    }
*/
    // Do stuff

    //SDL_Flip(screen);

    delaytime = waittime - (timeGetTime() - framestarttime);
    if(0)//???delaytime > 0)
        STX_Service::GetTimerInstance()->stx_Delay((Uint32)delaytime);
    framestarttime = timeGetTime();

	if(STX_Service::GetInputInstance()->IsKeyPressed(KEY_x))
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer [80];

		time (&rawtime);
		timeinfo = localtime (&rawtime);

		strftime (buffer,80,"ScreenShot_%F_%r",timeinfo);
		IRenderer::GetRendererInstance()->SaveScreenShot(buffer);
		//IRenderer::GetRendererInstance()->SaveScreenShot(STX_Service::GetWindowInstance()->GetCaption());
	}
/*
	IND_Camera2d mCamera2d (CFrmaework3::Instance()->Window->GetWidth () / 2, CFrmaework3::Instance()->Window->GetHeight() / 2);
	IND_Camera3d mCamera3d (0.0f, 1.4f, -4.0f);
	mCamera3d.SetAspect ((float) CFrmaework3::Instance()->Window->GetWidth () / CFrmaework3::Instance()->Window->GetHeight());

		CFrmaework3::Instance()->Render->SetViewPort2d (0, 0, CFrmaework3::Instance()->Window->GetWidth () , CFrmaework3::Instance()->Window->GetHeight());
		CFrmaework3::Instance()->Render->SetCamera2d (&mCamera2d);
		CFrmaework3::Instance()->Entity2dManager->RenderEntities2d ();

		CFrmaework3::Instance()->Render->SetViewPort3d (0, 0, CFrmaework3::Instance()->Window->GetWidth () , CFrmaework3::Instance()->Window->GetHeight());
		CFrmaework3::Instance()->Render->SetCamera3d (&mCamera3d);
		CFrmaework3::Instance()->LightManager->Update ();
		CFrmaework3::Instance()->Entity3dManager->RenderEntities3d ();
*/
}

#if 0//def _WIN32
OSVERSIONINFOEX m_osver;
#define BUFSIZE 80
int IRenderer::WinVer()//int argc, char *argv[])
{
#if 0
	LOG_PRINT_X("Registry Versions:\n");
   BOOL bOsVersionInfoEx;
   // Try calling GetVersionEx() using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.
   ZeroMemory(&m_osver, sizeof(OSVERSIONINFOEX));
   m_osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) &m_osver)))
   {
      m_osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if(!GetVersionEx((OSVERSIONINFO *) &m_osver))
	return FALSE;
     // else
       //LOG_PRINT_X("Buffer for the structure size is OK.\n");
   }
   //else
     //  LOG_PRINT_X("GetVersionEx() is OK.\n");
   switch (m_osver.dwPlatformId)
   {
      //Test for the Windows NT product family.
      case VER_PLATFORM_WIN32_NT:
	 // Test for the specific product family.
	 if(m_osver.dwMajorVersion == 6 && m_osver.dwMinorVersion == 1)
	    LOG_PRINT_X("Windows 7\n");

	 if(m_osver.dwMajorVersion == 6 && m_osver.dwMinorVersion == 0)
	    LOG_PRINT_X("Windows Server 2008\n");
	 if(m_osver.dwMajorVersion == 6 && m_osver.dwMinorVersion == 0)
	    LOG_PRINT_X("Windows Vista\n");
	 if(m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 2)
	    LOG_PRINT_X("Windows Server 2003 with SP1, Windows XP with SP2\n");
	 if(m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 1)
	    LOG_PRINT_X("Windows Server 2003, Windows XP\n");
	 if(m_osver.dwMajorVersion == 5 && m_osver.dwMinorVersion == 0)
	    LOG_PRINT_X("Windows 2000\n");
	 if(m_osver.dwMajorVersion <= 4)
	    LOG_PRINT_X("Windows NT\n");
	 // Test for specific product on Windows NT 4.0 SP6 and later.
	 if(bOsVersionInfoEx)
	 {
	    // Test for the workstation type.
	    if(m_osver.wProductType == VER_NT_WORKSTATION)
	    {
	       if(m_osver.dwMajorVersion == 4)
		  LOG_PRINT_X("Workstation 4.0\n");
	       else if(m_osver.wSuiteMask & VER_SUITE_PERSONAL)
		  LOG_PRINT_X("Home Edition\n");
	       else
		  LOG_PRINT_X("Professional\n");
	    }
	    // Test for the server type.
	    else if(m_osver.wProductType == VER_NT_SERVER || m_osver.wProductType == VER_NT_DOMAIN_CONTROLLER)
	    {
	       if((m_osver.dwMajorVersion == 5) && (m_osver.dwMinorVersion == 2))
	       {
		  if(m_osver.wSuiteMask & VER_SUITE_DATACENTER)
		     LOG_PRINT_X("Datacenter Edition\n");
		  else if(m_osver.wSuiteMask & VER_SUITE_ENTERPRISE)
		     LOG_PRINT_X("Enterprise Edition\n");
		  else if(m_osver.wSuiteMask == VER_SUITE_BLADE)
		     LOG_PRINT_X("Web Edition\n");
		  else
		     LOG_PRINT_X("Standard Edition\n");
	       }
	       else if((m_osver.dwMajorVersion == 5) && (m_osver.dwMinorVersion == 0))
	       {
		  if(m_osver.wSuiteMask & VER_SUITE_DATACENTER)
		     LOG_PRINT_X("Datacenter Server\n");
		  else if(m_osver.wSuiteMask & VER_SUITE_ENTERPRISE)
		     LOG_PRINT_X("Advanced Server\n");
		  else
		     LOG_PRINT_X("Server\n");
	       }
	       // Windows NT 4.0
	       else
	       {
		  if(m_osver.wSuiteMask & VER_SUITE_ENTERPRISE)
		     LOG_PRINT_X("Server 4.0, Enterprise Edition\n");
		  else
		     LOG_PRINT_X("Server 4.0\n");
	       }
	    }
	 }
	 // Test for specific product on Windows NT 4.0 SP5 and earlier
	 else
	 {
	    HKEY hKey;
	    char szProductType[BUFSIZE];
	    DWORD dwBufLen=BUFSIZE;
	    LONG lRet;
	    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &hKey);
	    if(lRet != ERROR_SUCCESS)
	       return FALSE;
	    else
	      LOG_PRINT_X("RegOpenKeyEx() is OK.\n");
	    lRet = RegQueryValueExA(hKey, "ProductType", 0, 0, (LPBYTE)szProductType, &dwBufLen);
	    if((lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE))
	       return FALSE;
	    else
	      LOG_PRINT_X("RegQueryValueEx() is OK.\n");
	    RegCloseKey(hKey);
	    if(lstrcmpiA("WINNT", szProductType) == 0)
	       LOG_PRINT_X("Workstation ");
	    if(lstrcmpiA("LANMANNT", szProductType) == 0)
	       LOG_PRINT_X("Server ");
	    if(lstrcmpiA("SERVERNT", szProductType) == 0)
	       LOG_PRINT_X("Advanced Server ");
	    LOG_PRINT_X("%d.%d ", m_osver.dwMajorVersion, m_osver.dwMinorVersion);
	 }
	 // Display service pack (if any) and build number.
	 if(m_osver.dwMajorVersion == 4 && lstrcmpiW(m_osver.szCSDVersion, L"Service Pack 6") == 0)
	 {
	    HKEY hKey;
	    LONG lRet;
	    // Test for SP6 versus SP6a.
	    lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Hotfix\\Q246009", 0, KEY_QUERY_VALUE, &hKey);
	    if(lRet == ERROR_SUCCESS)
	    {
	      LOG_PRINT_X("RegOpenKeyEx() is OK.\n");
	      LOG_PRINT_X("Service Pack 6a (Build %d)\n", m_osver.dwBuildNumber & 0xFFFF);
	    }
	    // Windows NT 4.0 prior to SP6a
	    else
	    {
	       LOG_PRINT_X("%s (Build %d)\n", m_osver.szCSDVersion, m_osver.dwBuildNumber & 0xFFFF);
	    }
	     RegCloseKey(hKey);
	 }
	// Windows NT 3.51 and earlier or Windows 2000 and later
	 else
	 {
	    LOG_PRINT_X("%s (Build %d)\n", m_osver.szCSDVersion, m_osver.dwBuildNumber & 0xFFFF);
	 }
	 break;
      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:
	 if(m_osver.dwMajorVersion == 4 && m_osver.dwMinorVersion == 0)
	 {
	     LOG_PRINT_X("Microsoft Windows 95 ");
	     if(m_osver.szCSDVersion[1] == 'C' || m_osver.szCSDVersion[1] == 'B')
		LOG_PRINT_X("OSR2 ");
	 }
	 if(m_osver.dwMajorVersion == 4 && m_osver.dwMinorVersion == 10)
	 {
	     LOG_PRINT_X("Microsoft Windows 98 ");
	     if(m_osver.szCSDVersion[1] == 'A')
		LOG_PRINT_X("SE ");
	 }
	 if((m_osver.dwMajorVersion == 4) && (m_osver.dwMinorVersion == 90))
	 {
	     LOG_PRINT_X("Microsoft Windows Millennium Edition\n");
	 }
	 break;
      case VER_PLATFORM_WIN32s:
	 LOG_PRINT_X("Microsoft Win32s\n");
	 break;
   }
#endif
   return TRUE;
}
#endif
#if 0
void IRenderer::Init(int awidth, int aheight, const char* atitle, tRenderer rtb, bool InitLogFile
,int argc, char *argv[]
)
{
	;

	//g_pApp->WorkDirectory(InitLogFile);

	//if(g_pApp->screen==0)
	{
#ifdef _WIN32
	LOG_PRINT_X("Platform SDK Versions:\n");
/*
	//Minimum system required	Value for NTDDI_VERSION
	//Windows 7	NTDDI_WIN7
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Server 2008	NTDDI_WS08\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Vista with Service Pack 1 (SP1)	NTDDI_VISTASP1\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Vista NTDDI_VISTA\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Server 2003 with Service Pack 2 (SP2) NTDDI_WS03SP2\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Server 2003 with Service Pack 1 (SP1) NTDDI_WS03SP1\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows Server 2003	NTDDI_WS03\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows XP with Service Pack 3 (SP3)	NTDDI_WINXPSP3\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows XP with Service Pack 2 (SP2)	NTDDI_WINXPSP2\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows XP with Service Pack 1 (SP1)	NTDDI_WINXPSP1\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows XP	NTDDI_WINXP\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows 2000 with Service Pack 4 (SP4)	NTDDI_WIN2KSP4\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows 2000 with Service Pack 3 (SP3)	NTDDI_WIN2KSP3\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows 2000 with Service Pack 2 (SP2)	NTDDI_WIN2KSP2\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows 2000 with Service Pack 1 (SP1)	NTDDI_WIN2KSP1\n");
	if(NTDDI_VERSION>=0x0) LOG_PRINT_X("Windows 2000	NTDDI_WIN2K\n");
*/
	//The following tables describe the legacy macros in use by the Windows header files.
	//Minimum system required	Minimum value for _WIN32_WINNT and WINVER
	if(_WIN32_WINNT>=0x0601) LOG_PRINT_X("Windows 7\n");
	if(_WIN32_WINNT>=0x0600) LOG_PRINT_X("Windows Server 2008\n");
	if(_WIN32_WINNT>=0x0600) LOG_PRINT_X("Windows Vista\n");
	if(_WIN32_WINNT>=0x0502) LOG_PRINT_X("Windows Server 2003 with SP1, Windows XP with SP2\n");
	if(_WIN32_WINNT>=0x0501) LOG_PRINT_X("Windows Server 2003, Windows XP\n");
	if(_WIN32_WINNT>=0x0500) LOG_PRINT_X("Windows 2000\n");
	if(_WIN32_WINNT>=0x0400) LOG_PRINT_X("Windows NT 4.0\n");
/*
	if(_WIN32_WINDOWS>=0x0500) LOG_PRINT_X("Windows ME\n");
	if(_WIN32_WINDOWS>=0x0410) LOG_PRINT_X("Windows 98\n");
	if(_WIN32_WINDOWS>=0x0400) LOG_PRINT_X("Windows 95\n");
*/
	//Minimum version required	Minimum value of _WIN32_IE
	if(_WIN32_IE>=0x0800) LOG_PRINT_X("Internet Explorer 8.0\n");
	if(_WIN32_IE>=0x0700) LOG_PRINT_X("Internet Explorer 7.0\n");
	if(_WIN32_IE>=0x0603) LOG_PRINT_X("Internet Explorer 6.0 SP2\n");
	if(_WIN32_IE>=0x0601) LOG_PRINT_X("Internet Explorer 6.0 SP1\n");
	if(_WIN32_IE>=0x0600) LOG_PRINT_X("Internet Explorer 6.0\n");
	if(_WIN32_IE>=0x0550) LOG_PRINT_X("Internet Explorer 5.5\n");
	if(_WIN32_IE>=0x0501) LOG_PRINT_X("Internet Explorer 5.01\n");
	if(_WIN32_IE>=0x0500) LOG_PRINT_X("Internet Explorer 5.0, 5.0a, 5.0b\n");
	//WinVer();
#endif
;
//LOG_PRINT_X("ST:IRenderer::Init\n");
		IRenderer::GetRendererInstance(awidth, aheight, rtb,InitLogFile,false,argc,argv);

	STX_Service::GetWindowInstance()->SetCaption(
		atitle
		);

#if 0
	s_pRenderer->SetviewportWidth(GetViewportWidth());
	s_pRenderer->SetviewportHeight(GetViewportHeight());
#elif 0//???defined(_WIN32)//???

	s_pRenderer->setViewport(0,0,
		width,
		height);
#endif
//???
;
	//g_pApp->initEffect();
	//	g_pApp->initShader();
//	g_pApp->SetWindow(STX_Service::GetWindowInstance()->GetHWND());
}
#if 0
	if(g_pFrmaework3==0)
	{
		g_pFrmaework3 = CFrmaework3::Instance();
	CFrmaework3::Instance()->Init (rtb);
	D3DXFROMWINEMATRIX mMatView;
	D3DXFROMWINEMatrixIdentity (&mMatView);
	SetTransform (TS_VIEW, &mMatView);
	}
#endif
	}
#endif
int constantTypeSizes[CONSTANT_TYPE_COUNT] =
{
	sizeof(float),
	sizeof(D3DXFROMWINEVECTOR2),
	sizeof(D3DXFROMWINEVECTOR3),
	sizeof(D3DXFROMWINEVECTOR4),
	sizeof(int),
	sizeof(int) * 2,
	sizeof(int) * 3,
	sizeof(int) * 4,
	sizeof(int),
	sizeof(int) * 2,
	sizeof(int) * 3,
	sizeof(int) * 4,
	sizeof(float)*8,
	sizeof(float)*12,
	sizeof(D3DXFROMWINEMATRIX),
}
;


	int IRenderer::ZERO;
	int IRenderer::ONE;
	int IRenderer::SRC_COLOR;
	int IRenderer::ONE_MINUS_SRC_COLOR;
	int IRenderer::DST_COLOR;
	int IRenderer::ONE_MINUS_DST_COLOR;
	int IRenderer::SRC_ALPHA;
	int IRenderer::ONE_MINUS_SRC_ALPHA;
	int IRenderer::DST_ALPHA;
	int IRenderer::ONE_MINUS_DST_ALPHA;
	int IRenderer::SRC_ALPHA_SATURATE;
	int IRenderer::BM_ADD;
	int IRenderer::BM_SUBTRACT;
	int IRenderer::BM_REVERSE_SUBTRACT;
	int IRenderer::BM_MIN;
	int IRenderer::BM_MAX;
	int IRenderer::NEVER;
	int IRenderer::LESS;
	int IRenderer::EQUAL;
	int IRenderer::LEQUAL;
	int IRenderer::GREATER;
	int IRenderer::NOTEQUAL;
	int IRenderer::GEQUAL;
	int IRenderer::ALWAYS;
	int IRenderer::KEEP;
	int IRenderer::SET_ZERO;
	int IRenderer::REPLACE;
	int IRenderer::INVERT;
	int IRenderer::INCR;
	int IRenderer::DECR;
	int IRenderer::INCR_SAT;
	int IRenderer::DECR_SAT;
	int IRenderer::CULL_NONE;
	int IRenderer::CULL_BACK;
	int IRenderer::CULL_FRONT;
	int IRenderer::SOLID;
	int IRenderer::WIREFRAME;
#if 0
	VArray <Quad> RendererHLSLCg::Quads;
	VArray <Cube> RendererHLSLCg::Cubes;
	VArray <FastBuffers> RendererHLSLCg::FastBuffersv;

	VArray <CTextureLoader> RendererHLSLCg::textureLoaders;
	VArray <CVertexBufferLoader> RendererHLSLCg::vertexBufferLoaders;
	VArray <CIndexBufferLoader> RendererHLSLCg::indexBufferLoaders;
#endif
	FontID RendererHLSLCg::defaultFont=-1;
	VArray <TexFont> RendererHLSLCg::fonts;

	float RendererHLSLCg::vv[16];
	float RendererHLSLCg::ww[16];

	std::vector<D3DXFROMWINEMATRIX> RendererHLSLCg::s_pV;
	std::vector<D3DXFROMWINEMATRIX> RendererHLSLCg::s_mvV;

#if 0
	int RendererHLSLCg::viewportWidth=0;
	int	RendererHLSLCg::viewportHeight=0;
	int RendererHLSLCg::viewportX=0;
	int	RendererHLSLCg::viewportY=0;
#else
	D3DFROMWINEVIEWPORT9 RendererHLSLCg::m_viewport;
#endif
	float RendererHLSLCg::m_fYaw;
	float RendererHLSLCg::m_fPitch;
	float RendererHLSLCg::m_fRoll;
	D3DXFROMWINEVECTOR3 RendererHLSLCg::m_eye;
	D3DXFROMWINEVECTOR3 RendererHLSLCg::m_center;
	D3DXFROMWINEVECTOR3 RendererHLSLCg::m_up;
	float IRenderer::m_fieldOfView;
    float IRenderer::m_aspectRatio;
    float IRenderer::m_zNear;
    float IRenderer::m_zFar;




	void RendererHLSLCg::debug(void)
	{
		if(currentShader!=-1) 				LOG_PRINT_X("currentShader		=%x\n", currentShader);
		if(selectedShader!=-1) 				LOG_PRINT_X("selectedShader		=%x\n", selectedShader);
		if(currentVertexFormat!=-1) 			LOG_PRINT_X("currentVertexFormat		=%x\n", currentVertexFormat);
		if(selectedVertexFormat!=-1) 			LOG_PRINT_X("selectedVertexFormat		=%x\n", selectedVertexFormat);

		for(unsigned int i=0;i<MAX_VERTEXSTREAM;i++)
		{
			if(currentVertexBuffers[i]!=-1) 	LOG_PRINT_X("currentVertexBuffers[%d]	=%x\n", i, currentVertexBuffers[i]);
			if(selectedVertexBuffers[i]!=-1) 	LOG_PRINT_X("selectedVertexBuffers[%d]	=%x\n", i, selectedVertexBuffers[i]);
			if(currentOffsets[i]!=-1) 		LOG_PRINT_X("currentOffsets[%d]		=%x\n", i, currentOffsets[i]);
			if(selectedOffsets[i]!=-1) 		LOG_PRINT_X("selectedOffsets[%d]		=%x\n", i, selectedOffsets[i]);
		}
		
		if(currentIndexBuffer!=-1) 			LOG_PRINT_X("currentIndexBuffer		=%x\n", currentIndexBuffer);
		if(selectedIndexBuffer!=-1) 			LOG_PRINT_X("selectedIndexBuffer		=%x\n", selectedIndexBuffer);

		if(currentDepthState!=-1) 			LOG_PRINT_X("currentDepthState		=%x\n", currentDepthState);
		if(selectedDepthState!=-1) 			LOG_PRINT_X("selectedDepthState		=%x\n", selectedDepthState);
	
		if(currentBlendState!=-1) 			LOG_PRINT_X("currentBlendState		=%x\n", currentBlendState);
		if(selectedBlendState!=-1) 			LOG_PRINT_X("selectedBlendState		=%x\n", selectedBlendState);
		if(currentAlphaState!=-1) 			LOG_PRINT_X("currentAlphaState		=%x\n", currentAlphaState);
		if(selectedAlphaState!=-1) 			LOG_PRINT_X("selectedAlphaState		=%x\n", selectedAlphaState);
		if(currentRasterizerState!=-1) 			LOG_PRINT_X("currentRasterizerState	=%x\n", currentRasterizerState);
		if(selectedRasterizerState!=-1) 		LOG_PRINT_X("selectedRasterizerState	=%x\n", selectedRasterizerState);
/*
		uint currentStencilRef, selectedStencilRef;
		uint currentSampleMask, selectedSampleMask;
		TextureID currentColorRT[MAX_MRTS], currentDepthRT;
		int currentColorRTSlice[MAX_MRTS], currentDepthSlice;
		uint nCurrentRenderTargets;
*/
	}

#if 0
	ObjectID RendererHLSLCg::addFastBuffer(ShaderID sid)
	{
		static int id=-1;
			FastBuffers mesh;
			mesh.init(sid);
			id=FastBuffersv.add(mesh);
		return id;
	}
	void RendererHLSLCg::renderFastBuffer(ObjectID id)
	{
		if(!(id<FastBuffersv.size()))return;
		FastBuffersv[id].render();
	}
	ObjectID RendererHLSLCg::addQuad(ShaderID sid)
	{
		static int id=-1;
			Quad quad;
			quad.init(sid);
			id=Quads.add(quad);
		return id;
	}
	void RendererHLSLCg::renderQuad(ObjectID id)
	{
		if(!(id<Quads.size()))return;
		Quads[id].render();
	}
	ObjectID RendererHLSLCg::addCube(ShaderID sid)
	{
		static int id=-1;
			Cube qube;
			qube.init(sid);
			id=Cubes.add(qube);
		return id;
	}
	void RendererHLSLCg::renderCube(ObjectID id)
	{
		if(!(id<Cubes.size()))return;
		Cubes[id].render();
	}
#endif
	void RendererHLSLCg::buildLookAtMatrix(D3DXFROMWINEVECTOR3 at, IInput* Input)
	{
#if 0
		D3DXFROMWINEVECTOR3 look(0,0,1);
		D3DXFROMWINEVECTOR3 up(0,1,0);
		D3DXFROMWINEMATRIX R;
	//R.rotateXYZ(Input->GetSpinY_L()/180.0f*D3DXFROMWINE_PI+1.5f*D3DXFROMWINE_PI,-Input->
	//	GetSpinX_L()/180.0f*D3DXFROMWINE_PI+1.5f*D3DXFROMWINE_PI,0.0f);

	m_fYaw=Input->GetSpinY_L()/180.0f*D3DXFROMWINE_PI+1.5f*D3DXFROMWINE_PI;
	m_fPitch=-Input->GetSpinX_L()/180.0f*D3DXFROMWINE_PI+1.5f*D3DXFROMWINE_PI;
	m_fRoll=0.0f;

	D3DXFROMWINEVECTOR3 look2=R*look;
	D3DXFROMWINEVECTOR3 up2=R*up;
	D3DXFROMWINEVECTOR3 look3=at+100.0f*look2;
	D3DXFROMWINEVECTOR3 up3=at+up2;
	//look3=normalize(look3);
	up3=normalize(up3);

	buildLookAtMatrix(
		at,
		look3,
		up3
		);
#elif 0
	mv=
	rotateXY(g_pApp->m_fSpinY_L/180.0f*D3DXFROMWINE_PI, -g_pApp->m_fSpinX_L/180.0f*D3DXFROMWINE_PI)
	//*translate(-at.x,-at.y,-at.z);
	*translate(-at);
	m_fYaw=g_pApp->m_fSpinY_L/180.0f*D3DXFROMWINE_PI;
	m_fPitch=-g_pApp->m_fSpinX_L/180.0f*D3DXFROMWINE_PI;
	m_fRoll=0.0f;
#endif
	}

void RendererHLSLCg::RenderTexture(TextureID id, D3DXFROMWINEVECTOR2 aposition, D3DXFROMWINEVECTOR2 asize, D3DXFROMWINEVECTOR2 texposition, D3DXFROMWINEVECTOR2 texsize)
{
	InitTex(texShader,texVF);
	D3DXFROMWINEVECTOR4 col(1.0f, 1.0f, 1.0f, 1.0f);
	TexVertex dest[4];
	float x=aposition.x;
	float y=aposition.y;
	dest[0].position = D3DXFROMWINEVECTOR2(x + asize.x, y);
	dest[0].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y);
	dest[1].position = D3DXFROMWINEVECTOR2(x + asize.x, y + asize.y);
	dest[1].texCoord = D3DXFROMWINEVECTOR2(texposition.x+texsize.x, texposition.y+texsize.y);
	dest[2].position = D3DXFROMWINEVECTOR2(x, y);
	dest[2].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y);
	dest[3].position = D3DXFROMWINEVECTOR2(x, y + asize.y);
	dest[3].texCoord = D3DXFROMWINEVECTOR2(texposition.x, texposition.y+texsize.y);
	setShader(GettexShader());
	setVertexFormat(texVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", col);

	setTexture("Base", id, (*GetTexture(id)).ss);

	setBlendState(GetblendSrcAlpha());
	setDepthState(GetnoDepthTest());
	DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, &dest[0], &dest[0], sizeof(TexVertex));
}

void RendererHLSLCg::drawTexturedVrtl(const Primitives primitives, TexVertex *vertices, const uint nVertices, const TextureID texture, const BlendStateID blendState, const DepthStateID depthState, const D3DXFROMWINEVECTOR4 *color)
{
	//
	InitTex(texShader,texVF);
	D3DXFROMWINEVECTOR4 col = D3DXFROMWINEVECTOR4(1, 1, 1, 1);
	if (color) col = *color;
	setShader(GettexShader());
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", col);

	setTexture("Base", texture, (*GetTexture(texture)).ss);

	setBlendState(blendState);
	setDepthState(depthState);
	setRasterizerState(cullBack);
	setVertexFormat(GettexVF());
	int pc=getPrimitiveCount(primitives, nVertices);
	int pcR=getPrimitiveCountR(primitives, nVertices);
	//apply();
	DrawPrimitiveUP(primitives,
		//pc
		 nVertices
		, vertices, vertices, 4*sizeof(float));
	//???setVertexFormat(-1);
}
void RendererHLSLCg::drawPlain(const Primitives primitives, D3DXFROMWINEVECTOR2 *vertices, const uint nVertices, const BlendStateID blendState, const DepthStateID depthState, const D3DXFROMWINEVECTOR4 *color){
	//
	InitPlain(plainShader,plainVF);

	D3DXFROMWINEVECTOR4 col = D3DXFROMWINEVECTOR4(1, 1, 1, 1);
	if (color) col = *color;

	//reset();
	setShader(plainShader);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", col);
	setBlendState(blendState);
	setDepthState(depthState);
	setVertexFormat(plainVF);
	//apply();

	DrawPrimitiveUP(
		primitives,
		getPrimitiveCount(primitives, nVertices),
		vertices,vertices, sizeof(D3DXFROMWINEVECTOR2));

}

void RendererHLSLCg::drawTextureVrtl(TextureID texture, int x, int y, int sx, int sy, bool InScene, float tx, float ty, float stx, float sty)
{
	//
	InitTex(texShader,texVF);

	float qx=x;
	float qy=y;
	float dx=sx;
	float dy=sy;

	std::vector<float> vertices(16);
	bool fq=((tx==0.0f)&&(ty==0.0f)&&(stx==1.0f)&&(sty==1.0f));
	{
		if(fq)
		{
			float verts2[] =
			{
				qx+dx,qy,1.0f, 0.0f
				,qx+dx,qy+dy,1.0f, 1.0f
				,qx,qy,0.0f, 0.0f
				,qx,qy+dy,0.0f, 1.0f
			}
			;
			stx_memcpy(&vertices[0],verts2,16*sizeof(float));
		}
		else
		{
			float verts2[] =
			{
				qx+dx,qy,tx+stx,ty
				,qx+dx,qy+dy,tx+stx,ty+sty
				,qx,qy,tx,ty
				,qx,qy+dy,tx,ty+sty
			}
			;
			stx_memcpy(&vertices[0],verts2,16*sizeof(float));

		}
	}
	if(!InScene)
	{
		BeginScene();
	}
	setShader(texShader);
	setVertexFormat(texVF);
	setTexture("Base", texture, (*GetTexture(texture)).ss);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setBlendState(GetblendSrcAlpha());
	setDepthState(GetnoDepthTest());
	setRasterizerState(cullBack);
	//apply();
//	DrawPrimitive(PRIM_TRIANGLE_STRIP, 0, 2);
	DrawPrimitiveUP(
		PRIM_TRIANGLE_STRIP,
		getPrimitiveCount(PRIM_TRIANGLE_STRIP, 4),

		&vertices[0], &vertices[0], 4*sizeof(float));
	if(!InScene)
	{
		EndScene();

	}

}

void RendererHLSLCg::drawFPS()
{
	char str[16];
    stx_snprintf(str,16, "%d", fps);
	drawText(str, 8, GetViewportHeight()-8*10.0f, 30, 38, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);
}

std::map<std::string, TextureID> IRenderer::m_Texture_cache;

TextureID IRenderer::find_from_Texture_cache(const char* fileName)
{
	std::map<std::string, TextureID>::iterator it;
	it = m_Texture_cache.find(fileName);
	if (it != m_Texture_cache.end())
	{
		return it->second;
	}
	return -1;
}

void IRenderer::insert_to_Texture_cache(const char* fileName, TextureID id)
{
	std::map<std::string, TextureID>::iterator it;
	it = m_Texture_cache.find(fileName);
	if (it == m_Texture_cache.end())
		m_Texture_cache.insert ( std::pair<std::string,TextureID>(fileName, id) );
}

TextureID IRenderer::addImageLibTexture(const char *fileName0,
										  //const unsigned int mips
										  const bool useMipMaps
										  , const SamplerStateID samplerState, uint flags, float3 alpha)
{

	std::string f=fileName0;

	std::string f2=stx_convertpath(f);

    const char* fileName=f2.c_str();


    stx_fileExists(fileName);

	LOG_FNLN_X;
	LOG_PRINT_X("fileName \"%s\"\n", fileName);

	TextureID id=TEXTURE_NONE;
	std::map<std::string, TextureID>::iterator it;
	it = m_Texture_cache.find(fileName);
	if (it != m_Texture_cache.end())
	{
		id=it->second;
	}
	else
	{

	Image3 img;

	if (img.loadImageLibImage(fileName, useMipMaps))
	{
		//???img.setAlpha(alpha);

		id=addTexture(&img,useMipMaps, samplerState, flags,alpha);
	}
	else
	{
        	LOG_PRINT_X("Couldn't open \"%s\"\n", fileName);
        	id=TEXTURE_NONE;
	}
	}
	if (it == m_Texture_cache.end())
		m_Texture_cache.insert ( std::pair<std::string,TextureID>(fileName, id) );
	return id;
}
TextureID RendererHLSLCg::addTexture(const char *fileName0, const bool useMipMaps, const SamplerStateID samplerState, uint flags, float3 alpha)
{
return IRenderer::GetRendererInstance()->addImageLibTexture(fileName0, useMipMaps,
				samplerState, flags, alpha);
}

#if 0
TextureID RendererHLSLCg::addCubemapVrtl(const char **fileNames, const bool useMipMaps, const SamplerStateID samplerState, const int nArraySlices, uint flags)
{
	Image3 img;
	if (img.loadSlicedImage(fileNames, 0, nArraySlices))
	{
		//if (img.getFormat() == FORMAT_RGBE8) img.unpackImage();
		if (useMipMaps && img.getMipMapCount() <= 1) img.createMipMaps();
		return IRenderer::GetRendererInstance()->addTexture(&img,true, samplerState, flags);
	}
	else
	{
		char str[1024];
        int n = stx_snprintf(str,1024, "Couldn't open cubemap:\n");
		for (int i = 0; i < 6 * nArraySlices; i++)
		{
            n += stx_snprintf(str + n,1024, "%s\n", fileNames[i]);
		}
		ErrorMsg(str);
		return TEXTURE_NONE;
	}
}
TextureID RendererHLSLCg::addNormalMap(const char *fileName, const FORMAT destFormat, const bool useMipMaps, const SamplerStateID samplerState, float sZ, float mipMapScaleZ, uint flags)
{
	Image3 img;
	uint loadFlags = 0;
	if (!useMipMaps) loadFlags |= DONT_LOAD_MIPMAPS;
	if (img.loadImage(fileName, loadFlags))

	{


		if (useMipMaps && img.getMipMapCount() <= 1) img.createMipMaps();
		if (img.toNormalMap(destFormat, sZ, mipMapScaleZ))
		{
			return addTexture(&img,
				true, samplerState, flags);
		}
	}
	else
	{
		char str[256];
        stx_snprintf(str,256, "Couldn't open \"%s\"", fileName);
		ErrorMsg(str);
	}
	return TEXTURE_NONE;
}
#endif
ShaderID IRenderer::addShaderFromFileVrtl(const char *fileName, const char *vsName, const char *psName,  const char **attributeNames, const int nAttributes, const char *extra, const uint flags)
{
	std::string f=fileName;
	std::string fn=stx_convertpath(f);
	LOG_FNLN;
	LOG_PRINT("fileName=%s\n", fn.c_str());
	std::string contents;
	std::ifstream in(fn.c_str(), std::ios::in | std::ios::binary);
  	if (in)
  	{
    		contents=(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
  	}
	const char* c=contents.c_str();
	//LOG_PRINT_X("\n<8:>\n%s\n%s\n%s\n</8:>\n", c, vsName, psName);
	ShaderID id=addHLSLShaderVrtl(c, vsName, psName, 0, 0, 0, 0);
	return id;
}
ShaderID IRenderer::addHLSLShaderVrtl(const char *shaderTextp, const char *vsName, const char *psName,  const char **attributeNames, const int nAttributes, const char *extra, const uint flags)
{
LOG_FNLN_X;
	
	if(!shaderTextp) DBG_HALT;
LOG_FNLN_X;
	//LOG_PRINT_X("\n<5:>\n%s\n</5:>\n", shaderTextp);
	std::string shaderText=std::string(shaderTextp);
LOG_FNLN_X;
	ShaderID res = SHADER_NONE;
LOG_FNLN_X;
	char *vss = (char *)strstr(shaderText.c_str(), "[Vertex shader]");
LOG_FNLN_X;
	//char *gs = (char *)strstr(shaderText.c_str(), "[Geometry shader]");
	char *fs = (char *)strstr(shaderText.c_str(), "[Fragment shader]");
LOG_FNLN_X;
	char *header = (((char *)shaderText.c_str())[0] != '[')? (char *)shaderText.c_str() : 0;
LOG_FNLN_X;
	int vsLine = 0;
LOG_FNLN_X;
	
	if (vss != 0)
	{
		*vss = '\0';
		vss += 15;
		while (*vss == '\r' || *vss == '\n') vss++;
		char *str = (char *)shaderText.c_str();
		while (str < vss)
		{
			if (*str == '\n') vsLine++;
			str++;
		}
	}
LOG_FNLN_X;
	int fsLine = 0;
LOG_FNLN_X;
	
	if (fs != 0)
	{
		*fs = '\0';
		fs += 17;
		while (*fs == '\r' || *fs == '\n') fs++;
		char *str = (char *)shaderText.c_str();
		while (str < fs)
		{
			if (*str == '\n') fsLine++;
			str++;
		}
	}
LOG_FNLN_X;

	//LOG_PRINT_X("\n<6:>\n%s\n%s\n</6:>\n", vsName, vss);
	//LOG_PRINT_X("\n<7:>\n%s\n%s\n</7:>\n", psName, fs);
	
	res = addHLSLShader(vss, 0, fs, vsName, psName, vsLine, 0, fsLine, header, extra, shaderText.c_str(), attributeNames, nAttributes, flags);
LOG_FNLN_X;


	
	if(res==-1)
		DBG_HALT;
LOG_FNLN_X;

	
	return res;
}
int RendererHLSLCg__formatSize[] =
	{
		sizeof(float), sizeof(float)/2, sizeof(ubyte)
	}
	;
int RendererHLSLCg::getFormatSize(const AttributeFormat format) const
{
	return RendererHLSLCg__formatSize[format];
}
FontID RendererHLSLCg::addFont(const char *textureFile, const char *fontFile, const SamplerStateID samplerState)
{/*
		static FileCache fileCache;
		int i=fileCache.begin(fontFile);//???
		if(i!=-1)
			return i;
		else
		{*/



	std::string f=fontFile;

        std::string fn=stx_convertpath(f);



	LOG_PRINT_X("RendererHLSLCg::addFont:fopen=%s\n",fn.c_str());
        FILE *file = fopen(fn.c_str(), "rb");

	if (file == 0) return FONT_NONE;

	TexFont afont;
	TexFont* font=&afont;

	uint version = 0;
    fread(&version, sizeof(version), 1, file);
    fread(font->chars, sizeof(font->chars), 1, file);
    fclose(file);

	LOG_PRINT_X("textureFile=%s\n",textureFile);

		font->texture = IRenderer::GetRendererInstance()->addImageLibTexture(textureFile, false, samplerState);

	LOG_PRINT_X("font->texture=%d\n",font->texture);

	if (font->texture == TEXTURE_NONE) return FONT_NONE;

	RendererHLSLCg::fonts.add(afont);

//	 fileCache.end(font->texture);//???
//		}
	return RendererHLSLCg::fonts.size()-1;
}

//CResourceReuseCache*	    RendererHLSLCg::g_pResourceReuseCache=0;
#if 0
#include <zip.h>

zip* APKArchive;
static void loadAPK (const char* apkPath) {
  LOG_PRINT_X("Loading APK %s\n", apkPath);
  APKArchive = zip_open(apkPath, 0, 0);
  if (APKArchive == 0) {
    LOG_PRINT_X("Error loading APK");
    return;
  }

  //Just for debug, print APK contents
  int numFiles = zip_get_num_files(APKArchive);
  for (int i=0; i<numFiles; i++) {
    const char* name = zip_get_name(APKArchive, i, 0);
    if (name == 0) {
      LOG_PRINT_X("Error reading zip file name at index %i : %s", zip_strerror(APKArchive));
      return;
    }
	if (name[stx_strlen(name)-1]=='/') continue;

	char buf[1024];
	stx_strlcpy(buf,name,1024);
    char* fc1=stx_strrchr(buf,'/');
    char* fc2=stx_strrchr(buf,'\\');
    char* fc=stx_max(fc1,fc2);
    buf[int(fc-buf)]='\0';
    if (fc)
	{
    stx_mkdirs(buf);
	}
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(APKArchive, name, 0, &st);
    char *contents = new char[st.size];

        int err = 0;
    zip_file *f = zip_fopen(APKArchive, name, 0);

    FILE* ff=fopen(name, "wb");
    zip_fread(f, contents, st.size);
    fwrite(contents, st.size,1,ff);
    zip_fclose(f);
    fclose(ff);
	delete[] contents;

  }
}

int zxz(const char* apkPath)
{
    //Open the ZIP archive
    int err = 0;
    zip *z = zip_open(apkPath, 0, &err);

    //Search for the file of given name
    const char *name = "beetleship.png";
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, name, 0, &st);

    //Alloc memory for its uncompressed contents
    char *contents = new char[st.size];

    //Read the compressed file
    zip_file *f = zip_fopen(z, "beetleship.png", 0);
    zip_fread(f, contents, st.size);
    zip_fclose(f);

    //And close the archive
    zip_close(z);
	delete[] contents;
}
#endif
int RendererHLSLCg::WorkDirectory(int awidth, int aheight, bool InitLogFile,
	int argc, char *argv[])
{
	STX_INIT_SEGVCATCH;

	width=awidth;//IRenderer::width;
	height=aheight;//IRenderer::height;
	bpp=IRenderer::bpp;

#if 0
	if(miscParams)
	{

		if((*miscParams)["width"].length())
		{
		width=atoi((*miscParams)["width"].c_str());
		}
		if((*miscParams)["height"].length())
		{
		height=atoi((*miscParams)["height"].c_str());
		}
		if((*miscParams)["bpp"].length())
		{
		bpp=atoi((*miscParams)["bpp"].c_str());
		}
		if((*miscParams)["window"].length())
		{
			std::string str=((*miscParams)["window"]).c_str();
			char* c=(char*)str.c_str();
            char* s=stx_strchr(c,':');
			if(s)c[int(s-str.c_str())]='\0';
		m_dpy=(Display*)atoi(c);
		m_win=*(Window*)atoi(s+1);;

		}
for(int i=0;i<argc;i++)
{
	std::string buf=argv[i];

	buf=buf.substr(0,5);
	if(strcmp(buf.c_str(),"-width")==0)
		{
			std::string str=argv[i];
			char* c=(char*)str.c_str();
            char* s=stx_strchr(c,'=');
			width=atoi(s+1);;
		}
	buf=argv[i];
	buf=buf.substr(0,6);
	if(strcmp(buf.c_str(),"-height")==0)
		{
			std::string str=argv[i];
			char* c=(char*)str.c_str();
            char* s=stx_strchr(c,'=');
			width=atoi(s+1);;
		}
	buf=argv[i];
	buf=buf.substr(0,3);
	if(strcmp(buf.c_str(),"-bpp")==0)
		{
			std::string str=argv[i];
			char* c=(char*)str.c_str();
            char* s=stx_strchr(c,'=');
			width=atoi(s+1);;
		}
	buf=argv[i];
	buf=buf.substr(0,6);
	if(strcmp(buf.c_str(),"-window")==0)
		{
			std::string str=argv[i];
			char* c=(char*)str.c_str();
            char* s=stx_strchr(c,'=');
			if(s)
			{
			std::string str2=s+1;
			char* c=(char*)str2.c_str();
            char* s=stx_strchr(c,':');
			if(s)c[int(s-str2.c_str())]='\0';
		m_dpy=(Display*)atoi(c);
		m_win=*(Window*)atoi(s+1);;
			}
		}

}

	}
#endif

	STX_Service::GetWindowInstance()->Init(width,height,bpp,"");
#if !defined(WIN_PHONE_APP) && !defined(ANDROID) && !defined(IPHONE_SIMULATOR) && !defined(OS_IPHONE)
	if(
#if defined(_WIN32)
		STX_Service::GetWindowInstance()->GetHWND()==0
#elif defined(QT_BUILD) || defined(SDL_2_0_4_BUILD)
		0
#else
		STX_Service::GetWindowInstance()->GetDisplay()==0
#endif
		){


	}
#endif
	return 0;
}

RendererHLSLCg::RendererHLSLCg( int awidth, int aheight, bool InitLogFile
		,int argc, char *argv[]):IRenderer()
{
	//LOG_PRINT_X("ST:RendererHLSLCg::RendererHLSLCg\n");
	WorkDirectory(awidth, aheight,InitLogFile,argc,argv);
    // Create the texture reuse cache
   //if(g_pResourceReuseCache==0)
//	   g_pResourceReuseCache=new CResourceReuseCache(this  );
//    if( !g_pResourceReuseCache )
//	return E_OUTOFMEMORY;

	nImageUnits = 1;
	nMRTs = 1;
	maxAnisotropic = 1;

	m_viewport=D3DFROMWINEVIEWPORT9();
	m_viewport.Width  = width;
	m_viewport.Height = height;
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	D3DXFROMWINEMatrixIdentity((D3DXFROMWINEMATRIX*)pp);
	D3DXFROMWINEMatrixIdentity((D3DXFROMWINEMATRIX*)vv);
	D3DXFROMWINEMatrixIdentity((D3DXFROMWINEMATRIX*)ww);
	fontBuffer.resize(0);
	fontBufferCount = 0;
	#if 0//???def PROFILE
	GLprofileFrame = false;
	GLprofileReset = true;
	GLprofileString[0] = '\0';
	#endif
/*
	texShader = SHADER_NONE;
			texVF = VF_NONE;
*/
	/*
		FormatDesc pcAttribs[] = {
		0,TYPE_VERTEX,   FORMAT_FLOAT,    3,
		0,TYPE_TEXCOORD,    FORMAT_FLOAT, 4,
	};
	FormatDesc pctAttribs[] = {
		0,TYPE_VERTEX,   FORMAT_FLOAT,    3,
		0,TYPE_TEXCOORD,    FORMAT_FLOAT, 4,
		0,TYPE_TEXCOORD, FORMAT_FLOAT,    2,
	};

	pcVertexFormat  = addVertexFormat(pcAttribs,  elementsOf(pcAttribs));
	pctVertexFormat = addVertexFormat(pctAttribs, elementsOf(pctAttribs));
*/
	texShader=-1;
	texVF=-1;
	plainShader=-1;
	plainVF=-1;
	plainShader3=-1;
	plainVF3=-1;

	m_VertexPointer=0;
   m_NormalPointer=0;
   m_TexCoordPointer=0;
   m_VertexOff=0;
   m_NormalOff=0;
   m_TexCoordOff=0;
   m_VertexW=0;
   m_NormalW=0;
   m_TexCoordW=0;

	//ililuInit();
//	m_pInput=0;

	RendererHLSLCg::vb=-1;
	RendererHLSLCg::ib=-1;
		RendererHLSLCg::defaultFont=-1;
		RendererHLSLCg::fonts.clear();
		RendererHLSLCg::fonts.empty();

#if 0
	Entity2dManager=new FW3_Entity2dManager();
	Entity3dManager=new FW3_Entity3dManager();
	AnimationManager=new FW3_AnimationManager();
	SurfaceManager=new FW3_SurfaceManager();
	FontManager=new FW3_FontManager();
	LightManager=new FW3_LightManager();
	MeshManager=new FW3_MeshManager();
#endif

}
RendererHLSLCg::~RendererHLSLCg()
{
	fontBuffer.resize(0);
//	delete g_pResourceReuseCache;

}
	D3DXFROMWINEMATRIX RendererHLSLCg::Getpp()
	{
		return pp;
	}
	D3DXFROMWINEMATRIX RendererHLSLCg::Getvv()
	{
		return vv;
	}

	D3DXFROMWINEMATRIX RendererHLSLCg::Getww()
	{
		return ww;
	}

void RendererHLSLCg::reset0ToDefaults()
{
	currentShader = SHADER_NONE;
	currentVertexFormat = VF_NONE;
	for (uint i = 0; i < MAX_VERTEXSTREAM; i++)
	{
		currentVertexBuffers[i] = VB_NONE;
		currentOffsets[i] = 0;
	}
	currentIndexBuffer = IB_NONE;
	currentDepthState = DS_NONE;
	currentStencilRef = 0;
	currentBlendState = BS_NONE;
	currentAlphaState = -1;
	currentSampleMask = ~0;
	currentRasterizerState = RS_NONE;
	currentColorRT[0] = FB_COLOR;
	currentColorRTSlice[0] = NO_SLICE;
	for (uint i = 1; i < nMRTs; i++)
	{
		currentColorRT[i] = TEXTURE_NONE;
		currentColorRTSlice[i] = NO_SLICE;
	}

	currentDepthRT = FB_DEPTH;
	currentDepthSlice = NO_SLICE;
	nCurrentRenderTargets = 1;
	reset(RESET_ALL);//???
	#ifdef DEBUG
	wasReset = true;
	#endif
	nearestClamp = addSamplerState(NEAREST, CLAMP, CLAMP, CLAMP);
	linearClamp = addSamplerState(LINEAR, CLAMP, CLAMP, CLAMP);
	bilinearClamp = addSamplerState(BILINEAR, CLAMP, CLAMP, CLAMP);
	trilinearClamp = addSamplerState(TRILINEAR, CLAMP, CLAMP, CLAMP);
	nearest = addSamplerState(NEAREST, WRAP, WRAP, WRAP);
	linearAniso = addSamplerState(LINEAR, WRAP, WRAP, WRAP);
	bilinearAniso = addSamplerState(BILINEAR, WRAP, WRAP, WRAP);
	trilinearAniso = addSamplerState(TRILINEAR, WRAP, WRAP, WRAP);

	noDepthTest  = addDepthState(false, false);
	noDepthWrite = addDepthState(true,  false);
	cullNone  = addRasterizerState(CULL_NONE);
	cullBack  = addRasterizerState(CULL_BACK);
	cullFront = addRasterizerState(CULL_FRONT);
	fillSolid = addRasterizerState(CULL_FRONT,SOLID);
	fillWireFrame = addRasterizerState(CULL_FRONT,WIREFRAME);
	blendSrcAlpha = -1;
	blendOneOne = -1;
	blendOneOneReverseSubstract = -1;
	//ifndef USE_D3D10_1
//if(!dynamic_cast<RendererHLSLD3D11*>(IRenderer::GetRendererInstance()))//???
{
	blendSrcAlpha = addBlendState(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
	blendOneOne = addBlendState(ONE, ONE,BM_ADD);
	blendOneOneReverseSubstract=addBlendState(ONE, ONE, BM_REVERSE_SUBTRACT);
}
    enableAlpha = addAlphaState();
    //disableAlpha = addAlphaState(false);

	//endif



}

#include <Framework3/rend_device_state.h>
void RendererHLSLCg::apply()
{
	#ifdef DEBUG
	//debug();
	if (!wasReset)
	{
		LOG_PRINT_X("%s", "Warning: reset() not called before apply()");
	}
	wasReset = false;
	#endif
	viewport(m_viewport.X,m_viewport.Y,m_viewport.Width,m_viewport.Height);
	if (selectedShader != DONTCARE)
	{
		changeShader(selectedShader);
		applyConstants();
	}
	applyTextures();
	applySamplerStates();
	changeVertexFormat(selectedVertexFormat);
	for (uint i = 0; i < MAX_VERTEXSTREAM; i++)
	{
		if (selectedVertexBuffers[i] != DONTCARE)
		{
			changeVertexBuffer(i, selectedVertexBuffers[i], selectedOffsets[i]);
		}
	}
	/*
{
	int nvertices=0;//???
	int s=0;
	if(m_VertexPointer) s+=m_VertexW*sizeof(float);
	if(m_NormalPointer) s+=m_NormalW*sizeof(float);
	if(m_TexCoordPointer) s+=m_TexCoordW*sizeof(float);
	BYTE* vertices=new BYTE[nvertices*s];
	for(unsigned int i=0;i<nvertices;i++)
	{
		int o=0;
		if(m_VertexPointer)
		{
		stx_memcpy(&vertices[i*s+o],&((BYTE*)m_VertexPointer)[i*s],m_VertexW*sizeof(float));
		o+=m_VertexW*sizeof(float);
		}
		if(m_NormalPointer)
		{
		stx_memcpy(&vertices[i*s+o],&((BYTE*)m_NormalPointer)[i*s],m_VertexW*sizeof(float));
		o+=m_NormalW*sizeof(float);
		}
		if(m_TexCoordPointer)
		{
		stx_memcpy(&vertices[i*s+o],&((BYTE*)m_TexCoordPointer)[i*s],m_VertexW*sizeof(float));
		o+=m_TexCoordW*sizeof(float);
		}

	}
	static VertexBufferID vb=-1;
	vb=addVertexBuffer(nvertices*s,STATIC,vertices,vb);
	delete[] vertices;

	changeVertexBuffer(0, vb, s);
		m_VertexPointer=0;
   m_NormalPointer=0;
   m_TexCoordPointer=0;
   m_VertexOff=0;
   m_NormalOff=0;
   m_TexCoordOff=0;
   m_VertexW=0;
   m_NormalW=0;
   m_TexCoordW=0;
}
	*/
	if (selectedIndexBuffer != DONTCARE) changeIndexBuffer(selectedIndexBuffer);
	if (selectedDepthState != DONTCARE)
	{

		changeDepthState(selectedDepthState, selectedStencilRef);
	}
 #if 1
	if (selectedAlphaState != DONTCARE)
	{
        //changeAlphaState(selectedAlphaState);
        BlendingAlphaTest::GetInstance()->SetAlphaTest(true);
	}
	if (selectedBlendState != DONTCARE)
	{
        changeBlendState(selectedBlendState, selectedSampleMask);
        //BlendingAlphaTest::GetInstance()->SetBlending( true );
	}
#endif
	if (selectedRasterizerState != DONTCARE) changeRasterizerState(selectedRasterizerState);
}
void RendererHLSLCg::reset0(const uint flags)
{
#if 0
	if(m_pInput)
	{
		bool bLoop=true;
		while(bLoop)

		{
			m_pInput->Update();

			//if(m_pInput->IsKeyPressed(IND_M)) m_pInput=0;
			if(m_pInput->IsKeyPressed(IND_N)) bLoop=false;
			if(m_pInput->IsKeyPressed(IND_B)) DBG_HALT;
			//if(m_pInput->IsKeyPressed(IND_S)) m_pInput=0;
		}

//		EndScene(true);
		Present(true);
		LOG_PRINT_X("Present\n");
//		BeginScene(true);
		Clear(  true,false,D3DXFROMWINEVECTOR4(0.35f, 0.53f, 0.7, 1.0f));
	}
#endif
	if (flags & RESET_SHADER)
	{
		selectedShader = SHADER_NONE;
	}
	if (flags & RESET_VF)
	{
		selectedVertexFormat = VF_NONE;
	}
	if (flags & RESET_VB)
	{
		for (uint i = 0; i < MAX_VERTEXSTREAM; i++)
		{
			selectedVertexBuffers[i] = VB_NONE;
			selectedOffsets[i] = 0;
		}
	}
	if (flags & RESET_IB) selectedIndexBuffer = IB_NONE;
	if (flags & RESET_DS)
	{
		selectedDepthState = DS_NONE;
		selectedStencilRef = 0;
	}
	if (flags & RESET_BS)
	{
		selectedBlendState = BS_NONE;
		selectedSampleMask = ~0;
	}
	if (flags & RESET_AS)selectedAlphaState = -1;
	if (flags & RESET_RS) selectedRasterizerState = RS_NONE;
	#ifdef DEBUG
	wasReset = true;
	#endif

	m_VertexPointer=0;
   m_NormalPointer=0;
   m_TexCoordPointer=0;
   m_VertexOff=0;
   m_NormalOff=0;
   m_TexCoordOff=0;
   m_VertexW=0;
   m_NormalW=0;
   m_TexCoordW=0;

}

TextureID RendererHLSLCg::addTextureVrtl(int Width, int Height, bool MipLevels, FORMAT format, const SamplerStateID samplerState, float3 alpha,const uint flags)
{
		//int bpp=getBytesPerPixel(format);
		//int is=getImageSize(format, Width, Height, 1, MipLevels);
		Image3 img;
		//int size = getMipMappedSize(0, MipLevelss);
		img.create(format,Width, Height, 1, MipLevels);
		//.setPixels( new ubyte[is]);
		return addTexture(&img,MipLevels,samplerState,flags);
}

void RendererHLSLCg::DrawPrimitiveUP(Primitives PrimitiveType,
UINT PrimitiveCount,
CONST void* pVertexStreamZeroData0,
CONST void* pVertexStreamZeroData,
UINT VertexStreamZeroStride, const uint flags)
{
	//
	int NumVertices=getPrimitiveCountR(PrimitiveType, PrimitiveCount);
	int StartVertex=int((((BYTE*)pVertexStreamZeroData)-((BYTE*)pVertexStreamZeroData0))/VertexStreamZeroStride);
	//static VertexBufferID vb = -1;

#if defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
	LOG_FNLN;
	LOG_PRINT("vb_=%x\n", vb);
#endif

	if(vb==-1)
	{
	vb = addVertexBuffer(
		(NumVertices+StartVertex) * VertexStreamZeroStride,
		DYNAMIC,
		pVertexStreamZeroData0);
	} else {
	vb = addVertexBuffer(
		(NumVertices+StartVertex) * VertexStreamZeroStride,
		DYNAMIC,
		pVertexStreamZeroData0,
		vb);
	}

	setVertexBufferVrtl(0,vb);

	DrawPrimitive(PrimitiveType,StartVertex,
	PrimitiveCount,flags);

#if defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
	LOG_FNLN;
	//LOG_PRINT_X("nv=%d\n",NumVertices);
	LOG_PRINT("PrimitiveCount=%d\n",PrimitiveCount);
	LOG_PRINT("NumVertices=%d\n", NumVertices);
	LOG_PRINT("StartVertex=%d\n", StartVertex);
	LOG_PRINT("VertexStreamZeroStride=%d\n", VertexStreamZeroStride);
	LOG_PRINT("pVertexStreamZeroData=%x\n", pVertexStreamZeroData);
	LOG_PRINT("pVertexStreamZeroData0=%x\n", pVertexStreamZeroData0);
	LOG_PRINT("flags=%d\n", flags);
	LOG_PRINT("vb=%d\n", vb);
#endif
}

VertexBufferID RendererHLSLCg::vb = -1;
IndexBufferID RendererHLSLCg::ib = -1;

void RendererHLSLCg::DrawIndexedPrimitiveUP(Primitives PrimitiveType,
UINT MinVertexIndex,
UINT NumVertices,
UINT PrimitiveCount,
CONST void* pIndexData0,
CONST void* pIndexData,
int IndexDataFormat,
CONST void* pVertexStreamZeroData0,
CONST void* pVertexStreamZeroData,
UINT VertexStreamZeroStride, const uint flags)
{
	if(!VertexStreamZeroStride) return;//???
	int StartVertex=int((((BYTE*)pVertexStreamZeroData)-((BYTE*)pVertexStreamZeroData0))/VertexStreamZeroStride);
	int startIndex=int((((BYTE*)pIndexData)-((BYTE*)pIndexData0))/sizeof(DWORD));
	if(IndexDataFormat==2) startIndex=int((((BYTE*)pIndexData)-((BYTE*)pIndexData0))/sizeof(WORD));

	//static VertexBufferID vb = -1;
	//static IndexBufferID ib = -1;
	vb = addVertexBuffer(
		(NumVertices+StartVertex) * VertexStreamZeroStride,
		STATIC,
		pVertexStreamZeroData0,
		vb);
	ib = addIndexBuffer(
		startIndex+3*PrimitiveCount ,
		(IndexDataFormat==4)?sizeof(unsigned int):sizeof(WORD),
		STATIC,
		pIndexData0,
		ib);

	setVertexBufferVrtl(0,vb);
	setIndexBuffer(ib);
	DrawIndexedPrimitive(
		PrimitiveType,StartVertex,
	MinVertexIndex,
	NumVertices,startIndex,
	PrimitiveCount,flags);
}
	void RendererHLSLCg::DumpRenderer()
	{
		LOG_PRINT_X("%s\n",getString());
	}
void RendererHLSLCg::DrawFPSVrtl(int x, int y)
{

	char str[16];
    stx_snprintf(str,16, "%d", fps);
	drawText(str, x, GetViewportHeight()-y*10.0f, 30, 38,
	defaultFont,
	GetlinearClamp(),
	GetblendSrcAlpha(),
	GetnoDepthTest());
}
	void RendererHLSLCg::getExtents(FontID font,char *string, float x, float y, const float charWidth, const float charHeight, const float skewX , const float skewY ,
		//const FONT_HADJUST hAdjust , const FONT_VADJUST vAdjust ,
		float *left , float *right , float *top , float *bottom )
	{
	}
float RendererHLSLCg::getStringWidth(FontID font,const char* str, const float charWidth)

{
	if(!RendererHLSLCg::fonts.size()) return 0.0f;
	float startx = 0.0f;
	float x = 0.0f;
	while (*str)
	{
		if (*str == '\n')
		{
			//y += charHeight;
			x = startx;
		}
		else
		{
//			LOG_PRINT_X("RendererHLSLCg::fonts.size=%d\n",RendererHLSLCg::fonts.size());
			//TexFont f=;
			Character chr = RendererHLSLCg::fonts[font].chars[*(unsigned char *) str];
			float charWidth=RendererHLSLCg::fonts[font].chars[*(unsigned char *) str].x1-
				RendererHLSLCg::fonts[font].chars[*(unsigned char *) str].x0;
			float cw = charWidth * chr.ratio;
			x += cw;
		}
		str++;
	}
	return x;
}
void RendererHLSLCg::fillTextBuffer(TexVertex *dest, const char *str, float x, float y, const float charWidth, const float charHeight, const FontID font)
{
	float startx = x;
	while (*str)
	{
		if (*str == '\n')
		{
			y += charHeight;
			x = startx;
		}
		else
		{
//			LOG_PRINT_X("RendererHLSLCg::fonts.size=%d\n",RendererHLSLCg::fonts.size());
			//TexFont f=;
			Character chr = RendererHLSLCg::fonts[font].chars[*(unsigned char *) str];
			float cw = charWidth * chr.ratio;
#if 0
			dest[0].position = D3DXFROMWINEVECTOR2(x, y);
			dest[0].texCoord = D3DXFROMWINEVECTOR2(chr.x0, chr.y0);
			dest[1].position = D3DXFROMWINEVECTOR2(x + cw, y);
			dest[1].texCoord = D3DXFROMWINEVECTOR2(chr.x1, chr.y0);
			dest[2].position = D3DXFROMWINEVECTOR2(x, y + charHeight);
			dest[2].texCoord = D3DXFROMWINEVECTOR2(chr.x0, chr.y1);
			dest[3].position = D3DXFROMWINEVECTOR2(x, y + charHeight);
			dest[3].texCoord = D3DXFROMWINEVECTOR2(chr.x0, chr.y1);
			dest[4].position = D3DXFROMWINEVECTOR2(x + cw, y);
			dest[4].texCoord = D3DXFROMWINEVECTOR2(chr.x1, chr.y0);
			dest[5].position = D3DXFROMWINEVECTOR2(x + cw, y + charHeight);
			dest[5].texCoord = D3DXFROMWINEVECTOR2(chr.x1, chr.y1);
#else
			dest[0].position = D3DXFROMWINEVECTOR2(x + cw, y);
			dest[0].texCoord = D3DXFROMWINEVECTOR2(chr.x1, chr.y0);
			dest[1].position = D3DXFROMWINEVECTOR2(x + cw, y + charHeight);
			dest[1].texCoord = D3DXFROMWINEVECTOR2(chr.x1, chr.y1);
			dest[2].position = D3DXFROMWINEVECTOR2(x, y);
			dest[2].texCoord = D3DXFROMWINEVECTOR2(chr.x0, chr.y0);
			dest[3].position = D3DXFROMWINEVECTOR2(x, y + charHeight);
			dest[3].texCoord = D3DXFROMWINEVECTOR2(chr.x0, chr.y1);
#endif
			dest += 4;
			x += cw;
		}
		str++;
	}
}
bool RendererHLSLCg::drawText(const char *str, float x, float y, const float charWidth,
						const float charHeight,
						const FontID& font,
						const SamplerStateID samplerState, const BlendStateID blendState, const DepthStateID depthState)
{
//	if(strcmp(str,"Visibility")==0)DBG_HALT;
//	if(strcmp(str,"FPS Monitor")==0)DBG_HALT;
	FontID ffont= font;
	if (font == FONT_NONE)
	if (defaultFont == FONT_NONE)
	{
		//return false;
#if 0
		defaultFont = addFont(FONT_PATH"Future.dds", FONT_PATH"Future.font", linearClamp);
#else
		std::string fn1="/Framework3/Future.dds";
		std::string fn2="/Framework3/Future.font";
		std::string buf =stx_convertpath(fn1);
		std::string buf2=stx_convertpath(fn2);


		defaultFont = addFont(buf.c_str(), buf2.c_str(), linearClamp);
#endif
		ffont=defaultFont;

    }else{ffont=font;}

	uint n = 4 * getTextQuads(str);

	if (n == 0) return true;

	if (n > fontBufferCount)
	{

		fontBuffer.resize( n );

		fontBufferCount = n;
	}

	fillTextBuffer(
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
		fontBuffer.data()

#else
		&fontBuffer[0]
#endif
		, str, x, y, charWidth, charHeight, ffont);

	/*
	drawTextured(PRIM_TRIANGLES,
#ifndef _MSC_VER
		fontBuffer.data()
#else
		&fontBuffer[0]
#endif
	, n, RendererHLSLCg::fonts[ffont]->texture, samplerState, blendState, depthState);
	*/
/*
drawTextured(
	const Primitives primitives, 
	TexVertex *vertices, 
	const uint nVertices, 
	const TextureID texture, 
	const BlendStateID blendState = -1, 
	const DepthStateID depthState = -1, 
	const D3DXFROMWINEVECTOR4 *color = 0
*/
	drawTextured(PRIM_TRIANGLE_STRIP,
#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
		fontBuffer.data()
#else
		&fontBuffer[0]
#endif
		, n-2, RendererHLSLCg::fonts[ffont].texture, blendState, depthState);

	return true;
}
#define MAX_BUF_SIZE 1024
void RendererHLSLCg::DrawText(unsigned int x, unsigned int y, const char *pszFormat, ...)
{
	char	buf[MAX_BUF_SIZE]={0};
	buf[0]='\0';
	va_list arglist;
	va_start(arglist, pszFormat);
    vsprintf(&buf[stx_strlen(buf)], pszFormat, arglist);
	va_end(arglist);


		drawText(buf, x, y, 15, 18, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);
}
void RendererHLSLCg::DrawSmallText(unsigned int x, unsigned int y, const char *pszFormat, ...)
{
	char	buf[MAX_BUF_SIZE]={0};
	buf[0]='\0';
	va_list arglist;
	va_start(arglist, pszFormat);
    vsprintf(&buf[stx_strlen(buf)], pszFormat, arglist);
	va_end(arglist);
		drawText(buf, x, y, 7, 9, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);
}
extern "C"
{
int rgb2gifMain(char *FileName0, unsigned int Width, unsigned int Height, unsigned char* Data);
};
void RendererHLSLCg::OpenILToGIF(Image3& img, const char* fn)
{
				FORMAT formats=img.getFormat();
				char bbuf[1024];
				sprintf(bbuf,"%s.gif",fn);
				//rgb2gifMain(bbuf,img.getWidth(),img.getHeight(),(unsigned char*)img.getPixels());
}

uint RendererHLSLCg::getTextQuads(const char *str) const
{
	uint n = 0;
	while (*str)
	{
		if (*str != '\n') n++;
		str++;
	}
	return n;
}
void RendererHLSLCg::setShaderConstant1i(const char *name, const int constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, &constant, sizeof(constant));
}
void RendererHLSLCg::setShaderConstant1f(const char *name, const float constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, &constant, sizeof(constant));
}
void RendererHLSLCg::setShaderConstant2f(const char *name, const D3DXFROMWINEVECTOR2 &constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, &constant, sizeof(constant));
}
void RendererHLSLCg::setShaderConstant3f(const char *name, const D3DXFROMWINEVECTOR3 &constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, &constant, sizeof(constant));
}
void RendererHLSLCg::setShaderConstant4f(const char *name, const D3DXFROMWINEVECTOR4 &constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, &constant, sizeof(constant));
}
void RendererHLSLCg::setShaderConstant4x4f(const char *name, const D3DXFROMWINEMATRIX &constant)
{
	//ASSERT(selectedShader != SHADER_NONE);
#if 0
	D3DXFROMWINEMATRIX constant2=constant;
	constant2.setTranspose();//???

	setShaderConstantRaw(name, &constant2, sizeof(constant2));
#else
	setShaderConstantRaw(name, &constant, sizeof(constant));
	//LOG_PRINT_X("%s\n",name);
#endif
}







void RendererHLSLCg::setShaderConstantArray1f(const char *name, const float *constant, const uint count)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, constant, count * sizeof(float));
}
void RendererHLSLCg::setShaderConstantArray2f(const char *name, const D3DXFROMWINEVECTOR2 *constant, const uint count)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, constant, count * sizeof(D3DXFROMWINEVECTOR2));
}
void RendererHLSLCg::setShaderConstantArray3f(const char *name, const D3DXFROMWINEVECTOR3 *constant, const uint count)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, constant, count * sizeof(D3DXFROMWINEVECTOR3));
}
void RendererHLSLCg::setShaderConstantArray4f(const char *name, const D3DXFROMWINEVECTOR4 *constant, const uint count)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, constant, count * sizeof(D3DXFROMWINEVECTOR4));
}
void RendererHLSLCg::setShaderConstantArray4x4f(const char *name, const D3DXFROMWINEMATRIX *constant, const uint count)
{
	//ASSERT(selectedShader != SHADER_NONE);
	setShaderConstantRaw(name, constant, count * sizeof(D3DXFROMWINEMATRIX));
}
#if 0
// Transform (x, y, z, 0) by matrix.  If you transforming a normal by a
// non-affine matrix, the matrix you pass to this function should be the
// transpose of the inverse of the matrix you would use to transform a coord.
D3DXFROMWINEVECTOR3* D3DXFROMWINEVECTOR3TransformNormal
    ( D3DXFROMWINEVECTOR3 *pOut, CONST D3DXFROMWINEVECTOR3 *pV, CONST D3DXFROMWINEMATRIX *pM )
{
    D3DXFROMWINEVECTOR4 X;
    D3DXFROMWINEVECTOR4 Y;
    D3DXFROMWINEVECTOR4 Z;
	D3DXFROMWINEVECTOR4 pOut0;
#if 0
    Z = XMVectorSplatZ(V);
    Y = XMVectorSplatY(V);
    X = XMVectorSplatX(V);
#else
	D3DXFROMWINEVECTOR3 V=*pV;
	Z[0]=V.z;
	Z[1]=V.z;
	Z[2]=V.z;

	Y[0]=V.y;
	Y[1]=V.y;
	Y[2]=V.y;

	X[0]=V.x;
	X[1]=V.x;
	X[2]=V.x;
#endif
    pOut0 = Z * ((*pM).rows[2]);//XMVectorMultiply(Z, M.r[2]);
    pOut0 = Y*((*pM).rows[1])+ pOut0;//XMVectorMultiplyAdd(Y, M.r[1], Result);
    pOut0 = X*((*pM).rows[0])+ pOut0;//XMVectorMultiplyAdd(X, M.r[0], Result);

	pOut->x=pOut0.x;
	pOut->y=pOut0.y;
	pOut->z=pOut0.z;

	return pOut;

}
#endif
VOID RendererHLSLCg::UpdateViewer(
                                                   D3DXFROMWINEVECTOR3 &pos, float* quat)
{
#if 0
        D3DXFROMWINEVECTOR3 v;
        m_eye = pos;
		D3DXFROMWINEMATRIX m_matRot;
		Quaternion2Mat4(m_matRot, quat);

        v = D3DXFROMWINEVECTOR3(0.0f, 0.0f, -1.0f);
        m_center=m_matRot*v;
        v = D3DXFROMWINEVECTOR3(0.0f, 1.0f, 0.0f);
        m_up=m_matRot*v;

        D3DXFROMWINEMATRIX m1, m2;
        m1.setScales( 1.0f, 1.0f, -1.0f);
        m2.rotateX( D3DXFROMWINE_PI / 2.0f);
        m2=m1*m2;

        m_center=m2*m_center;
        m_up=m2*m_up;

        m_center = normalize(m_center);
        m_up = normalize(m_up);
//        m_vCross = normalize(cross(m_up, m_center));
const FLOAT NEAR_CLIP = 0.001f;//150.0f;

const FLOAT FAR_CLIP = 1200000.0f;

const FLOAT FOV = D3DXFROMWINE_PI / 4.0f;

	buildPerspectiveMatrix(

		180.0f/4,

		float(GetViewportWidth())/GetViewportHeight(),

		0.1f,//NEAR_CLIP,

		FAR_CLIP

		);


#if 1
	D3DXFROMWINEVECTOR3 c=m_eye+100.0f*m_center;
#else
	static D3DXFROMWINEVECTOR3 m_eye0=m_eye;
	D3DXFROMWINEVECTOR3 c=m_eye+100.0f*(m_eye-m_eye0);
	m_up=D3DXFROMWINEVECTOR3(0,1,0);
#endif
		buildLookAtMatrix(
			m_eye.x,
			m_eye.y,
			m_eye.z,
			c.x,
			c.y,
			c.z,
			m_up.x,
			m_up.y,
			m_up.z
			);
#endif
}
#if 0
HRESULT RendererHLSLCg::LoadTexture(LoaderID pLoader)
{

	return textureLoaders[pLoader].Load();
}
HRESULT RendererHLSLCg::DecompressTexture(LoaderID  pLoader, void** pLocalData, SIZE_T* Bytes )
{
	return textureLoaders[pLoader].Decompress(pLocalData, Bytes);
}
HRESULT RendererHLSLCg::LoadVertexBuffer(LoaderID pLoader)
{
	return vertexBufferLoaders[pLoader].Load();
}
HRESULT RendererHLSLCg::DecompressVertexBuffer(LoaderID  pLoader, void** pLocalData, SIZE_T* Bytes )
{
	return vertexBufferLoaders[pLoader].Decompress(pLocalData, Bytes);
}
HRESULT RendererHLSLCg::LoadIndexBuffer(LoaderID pLoader)
{
	return indexBufferLoaders[pLoader].Load();
}
HRESULT RendererHLSLCg::DecompressIndexBuffer(LoaderID  pLoader, void** pLocalData, SIZE_T* Bytes )
{
	return indexBufferLoaders[pLoader].Decompress(pLocalData, Bytes);
}








LoaderID RendererHLSLCg::addTextureLoader( CHAR* szFileName, CPackedFile* g_PackFile )
	{
		static int id=-1;

			CTextureLoader tl( szFileName, g_PackFile );
			id=textureLoaders.add(tl);
		return id;
	}
LoaderID RendererHLSLCg::addVertexBufferLoader()
	{
		static int id=-1;
			CVertexBufferLoader tl;
			id=vertexBufferLoaders.add(tl);
		return id;
	}
LoaderID RendererHLSLCg::addIndexBufferLoader()
	{
		static int id=-1;
			CIndexBufferLoader tl;
			id=indexBufferLoaders.add(tl);
		return id;
	}
void RendererHLSLCg::SetProcessorResourceError(ProcessorID pProcessor){IMPLEMENT;}
void RendererHLSLCg::DestroyProcessor(ProcessorID pProcessor){IMPLEMENT;}
void RendererHLSLCg::DestroyLoader(ProcessorID pLoader){IMPLEMENT;}
#endif
void D3DXFROMWINECreateTextureFromFile(
	const char* n,
	TextureID* id,
	bool cm,
	SamplerStateID si, float3 alpha)
{
	if(si==-1) si=IRenderer::GetRendererInstance()->Getlinear();
	*id=IRenderer::GetRendererInstance()->addImageLibTexture(
		n,
		cm,




		si,0,alpha);

}
struct PCVertex {
	D3DXFROMWINEVECTOR2 vertex;
	//D3DXFROMWINEVECTOR4 color;
};

struct PCTVertex {
	D3DXFROMWINEVECTOR2 vertex;

	//D3DXFROMWINEVECTOR4 color;
	D3DXFROMWINEVECTOR2 texCoord;
};
#if 1
void RendererHLSLCg::drawLineVrtl(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){
	D3DXFROMWINEVECTOR2 no;
	D3DXFROMWINEVECTOR2 ni(y1 - y0, x1 - x0);
	D3DXFROMWINEVec2Normalize(&no,&ni);
	D3DXFROMWINEVECTOR2 d = 0.5f * lineWidth * no;
	D3DXFROMWINEVECTOR2 v0(x0, y0);
	D3DXFROMWINEVECTOR2 v1(x1, y1);

	uint col = toBGRA(color);
	PCVertex vertices[] = {
		D3DXFROMWINEVECTOR2(v0 + d),// color,
		D3DXFROMWINEVECTOR2(v0 - d),// color,
		D3DXFROMWINEVECTOR2(v1 + d),// color,
		D3DXFROMWINEVECTOR2(v1 - d),// color,
	};


	InitPlain(plainShader, plainVF);
	setShader(plainShader);
	setVertexFormat(plainVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, vertices, vertices, sizeof(PCVertex));
}
	TextureID RendererHLSLCg::addRenderDepth(const int width, const int height, const int depthBits){
		return addRenderDepth(width, height, 1, depthBits <= 16? FORMAT_D16 : FORMAT_D24);
	}

void RendererHLSLCg::drawRectVrtl(const float x0, const float y0, const float x1, const float y1, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){
	uint col = toBGRA(color);

	InitPlain(plainShader, plainVF);
	setShader(plainShader);
	setVertexFormat(plainVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	if (lineWidth > 0){
		float d = 0.5f * lineWidth;
		PCVertex vertices[] = {
			D3DXFROMWINEVECTOR2(x0 - d, y0 - d),// color,
			D3DXFROMWINEVECTOR2(x0 + d, y0 + d),// color,
			D3DXFROMWINEVECTOR2(x1 + d, y0 - d),// color,
			D3DXFROMWINEVECTOR2(x1 - d, y0 + d),// color,
			D3DXFROMWINEVECTOR2(x1 + d, y1 + d),// color,
			D3DXFROMWINEVECTOR2(x1 - d, y1 - d),// color,
			D3DXFROMWINEVECTOR2(x0 - d, y1 + d),// color,
			D3DXFROMWINEVECTOR2(x0 + d, y1 - d),// color,

			D3DXFROMWINEVECTOR2(x0 - d, y0 - d),// color,
			D3DXFROMWINEVECTOR2(x0 + d, y0 + d),// color,
		};

		DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 8, vertices, vertices, sizeof(PCVertex));
	} else {
		PCVertex vertices[] = {
			D3DXFROMWINEVECTOR2(x0, y0), //color,
			D3DXFROMWINEVECTOR2(x1, y0), //color,
			D3DXFROMWINEVECTOR2(x1, y1), //color,
			D3DXFROMWINEVECTOR2(x0, y1), //color,
		};

		DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 2, vertices, vertices, sizeof(PCVertex));
	}
}

void RendererHLSLCg::drawRoundRectVrtl(const float x0, const float y0, const float x1, const float y1, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){
	uint col = toBGRA(color);

	bool lines = (lineWidth > 0);
	float d = 0.5f * lineWidth;

	PCVertex vertices[42];

	int k = 0;
	// Four arcs
	for (int j = 0; j < 4; j++){
		// Arc center
		float x = (j >= 1 && j <= 2)? x0 + r : x1 - r;
		float y = (j > 1)? y0 + r : y1 - r;

		// Arc
		for (int i = 0; i <= 4; i++){
			float ang = (4 * j + i) * 2 * D3DXFROMWINE_PI / 16;
			float cosA = cosf(ang);
			float sinA = sinf(ang);

			if (lines){
				vertices[2 * k    ].vertex = D3DXFROMWINEVECTOR2(x + (r + d) * cosA, y + (r + d) * sinA);
				vertices[2 * k + 1].vertex = D3DXFROMWINEVECTOR2(x + (r - d) * cosA, y + (r - d) * sinA);
				//vertices[2 * k    ].color = color;
				//vertices[2 * k + 1].color = color;
			} else {
				vertices[k].vertex = D3DXFROMWINEVECTOR2(x + r * cosA, y + r * sinA);
				//vertices[k].color = color;
			}
			k++;
		}
	}

	InitPlain(plainShader, plainVF);
	setShader(plainShader);
	setVertexFormat(plainVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	if (lines){
		// Full loop
		vertices[40] = vertices[0];
		vertices[41] = vertices[1];
		DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 40, vertices, vertices, sizeof(PCVertex));
	} else {
		DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 18, vertices, vertices, sizeof(PCVertex));
	}
}
Primitives g_prim=PRIM_TRIANGLES;
D3DXFROMWINEVECTOR4 g_color(1.0f,1.0f,1.0f,1.0f);
//PCTVertex* g_vertices=0;
std::vector<D3DXFROMWINEVECTOR4> g_vertices;
std::vector<D3DXFROMWINEVECTOR2> g_texCoords;

void RendererHLSLCg::Color3f(float x,float  y, float  z){g_color=D3DXFROMWINEVECTOR4(x,y,z,1.0f);}
void RendererHLSLCg::Color4f(float x,float  y, float  z ,float  w){g_color=D3DXFROMWINEVECTOR4(x,y,z,1.0f);}//w);}
void RendererHLSLCg::Color3fv(const float* v){g_color=D3DXFROMWINEVECTOR4(v[0],v[1],v[2],1.0f);}
void RendererHLSLCg::Color4fv(const float* v){g_color=D3DXFROMWINEVECTOR4(v[0],v[1],v[2],v[3]);}//v[3]);}

void RendererHLSLCg::Vertex2fv(const float* v){g_vertices.push_back(D3DXFROMWINEVECTOR4(v[0],v[1],0.0f,1.0f));}
void RendererHLSLCg::Vertex3fv(const float* v){g_vertices.push_back(D3DXFROMWINEVECTOR4(v[0],v[1],v[2],1.0f));}
void RendererHLSLCg::Vertex4fv(const float* v){g_vertices.push_back(D3DXFROMWINEVECTOR4(v[0],v[1],v[2],v[3]));}


void RendererHLSLCg::Begin(Primitives prim)
{
	g_prim=prim;
}
void RendererHLSLCg::Vertex2i(int x,int y)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,0,1));
}
void RendererHLSLCg::Vertex3i(int x,int y, int z)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,x,1));
}
void RendererHLSLCg::Vertex4i(int x,int y, int z ,int w)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,z,w));
}
void RendererHLSLCg::Vertex2f(float x,float  y)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,0,1));
}
void RendererHLSLCg::Vertex3f(float x,float  y, float  z)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,z,1));
}
void RendererHLSLCg::Vertex4f(float x,float  y, float  z ,float  w)
{
	g_vertices.push_back(D3DXFROMWINEVECTOR4(x,y,z,w));
}
void RendererHLSLCg::TexCoord2f(float x,float  y)
{
	g_texCoords.push_back(D3DXFROMWINEVECTOR2(x,y));
}
unsigned int RendererHLSLCg::getIndicesCount(const Primitives primitives){
	switch (primitives){
		case PRIM_POINTS:
			return 1;
		case PRIM_LINES:
			return 2;
				case PRIM_LINE_STRIP:
			return 2;
		case PRIM_TRIANGLES:
			return 3;
		case PRIM_TRIANGLE_STRIP:
			return 3;
				case PRIM_TRIANGLE_FAN:
                        return 3;
    case PRIM_QUADS:
    return 0;
    case PRIM_LINE_LOOP:
    return 0;
		};
	return 0;
}
unsigned int RendererHLSLCg::getPrimitiveCount(const Primitives primitives, const int count){
	switch (primitives){
		case PRIM_POINTS:
			return count;
		case PRIM_LINES:
			return count / 2;
				case PRIM_LINE_STRIP:
			return count - 1;
		case PRIM_TRIANGLES:
			return count / 3;
		case PRIM_TRIANGLE_STRIP:
			return count - 2;
				case PRIM_TRIANGLE_FAN:
                        return count - 2;
    case PRIM_QUADS:
    return 0;
    case PRIM_LINE_LOOP:
    return 0;
		};
	return 0;
}
unsigned int RendererHLSLCg::getPrimitiveCountR(const Primitives primitives, const int count){
	switch (primitives){
		case PRIM_POINTS:
			return count;
		case PRIM_LINES:
			return count * 2;
				case PRIM_LINE_STRIP:
			return count + 1;
		case PRIM_TRIANGLES:
			return count * 3;
		case PRIM_TRIANGLE_STRIP:
			return count +  2;
				case PRIM_TRIANGLE_FAN:
						return count + 2;
        case PRIM_QUADS:
        return 0;
        case PRIM_LINE_LOOP:
        return 0;
		};
	return 0;
}
unsigned int RendererHLSLCg::getVertexCount(const Primitives primitives, const int  NumPrim)
{
	switch(primitives)
	{
		case PRIM_POINTS:		return NumPrim;

		case PRIM_LINES:			return NumPrim * 2;
		case PRIM_LINE_STRIP:		return NumPrim + 1;
		case PRIM_TRIANGLES:	return NumPrim * 3;
		case PRIM_TRIANGLE_STRIP:	return NumPrim + 2;
		case PRIM_TRIANGLE_FAN: 	return NumPrim + 2;
	default:					return 0;
	}
	return 0;
}


void RendererHLSLCg::EndVrtl(TextureID id)
{
	std::vector<PCTVertex> PCTvertices;
	std::vector<PCVertex> PCvertices;

	int tcs=g_texCoords.size();
	int vs=g_vertices.size();

	if(g_texCoords.size())
	{
		PCTvertices.resize(g_vertices.size());
	InitTex(texShader, texVF);
		setShader(texShader);
		setVertexFormat(texVF);
	for(unsigned int i=0;i<g_vertices.size();i++)
	{
		//LOG_PRINT_X("evx=%f\n",g_vertices.at(i).x);
		//LOG_PRINT_X("evy=%f\n",g_vertices.at(i).y);

		PCTvertices[i].vertex=D3DXFROMWINEVECTOR2(
			fabs(g_vertices.at(i).x),
			fabs(g_vertices.at(i).y)
			);
		if(i<g_texCoords.size())PCTvertices[i].texCoord=D3DXFROMWINEVECTOR2(
			g_texCoords.at(i).x,
			1.0f-g_texCoords.at(i).y//???-1
			);
	}
	}
	else
	{
		PCvertices.resize(g_vertices.size());
		InitPlain(plainShader, plainVF);
		setShader(plainShader);
		setVertexFormat(plainVF);
	for(unsigned int i=0;i<g_vertices.size();i++)
	{
		//LOG_PRINT_X("evx=%f\n",g_vertices.at(i).x);
		//LOG_PRINT_X("evy=%f\n",g_vertices.at(i).y);

		PCvertices[i].vertex=D3DXFROMWINEVECTOR2(
			fabs(g_vertices.at(i).x),
			fabs(g_vertices.at(i).y)
			);
	}
	}

	int nn=g_vertices.size();
/*
	if(g_prim==PRIM_TRIANGLE_STRIP)
		nn=g_vertices.size()-2;
*/

	int count=g_vertices.size();
	switch (g_prim){
		case PRIM_POINTS:
			nn=count;
			break;
		case PRIM_LINES:
			nn=count / 2;
			break;
				case PRIM_LINE_STRIP:
			nn=count - 1;
			break;
		case PRIM_TRIANGLES:
			nn=count / 3;
			break;
		case PRIM_QUADS:
			g_prim=PRIM_TRIANGLE_STRIP;//???
			nn=2;
			break;
		case PRIM_TRIANGLE_STRIP:
			nn=count - 2;
			break;
				case PRIM_TRIANGLE_FAN:
						nn=count - 2;
						break;
        case PRIM_LINE_LOOP:
        break;
		};

	/*getPrimitiveCountR(g_prim,
		g_vertices.size());
	if(g_prim==PRIM_LINES)
		nn=g_vertices.size();
*/
	/*
	switch (g_prim){
		case PRIM_POINTS:
			return g_vertices.size();
		case PRIM_LINES:
			return g_vertices.size() * 2;
				case PRIM_LINE_STRIP:
			return g_vertices.size() + 1;
		case PRIM_TRIANGLES:
			return g_vertices.size() * 3;
		case PRIM_TRIANGLE_STRIP:
			return g_vertices.size();// - 2;
				case PRIM_TRIANGLE_FAN:
						return g_vertices.size() + 2;
		};

*/
/*
	switch(g_prim)
	{
		case PRIM_LINES:
			LOG_PRINT_X("\n",);
			break;
	}:
*/

	g_vertices.clear();
	g_texCoords.clear();

	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", g_color);
	if(id!=-1)setTexture("Base",id,(*GetTexture(id)).ss);

	//setBlendState(GetblendSrcAlpha());
	setDepthState(GetnoDepthTest());
		if(PCTvertices.size())
		DrawPrimitiveUP(g_prim,
		nn, &PCTvertices[0], &PCTvertices[0], sizeof(PCTVertex));
		else
		DrawPrimitiveUP(g_prim,
		nn, &PCvertices[0], &PCvertices[0], sizeof(PCVertex));

	/*
	int i1=getPrimitiveCount(g_prim,g_vertices.size());
	int i2=getPrimitiveCountR(g_prim,g_vertices.size());
	LOG_PRINT_X("PC=%d\n",getPrimitiveCount(g_prim,g_vertices.size()));
	LOG_PRINT_X("PCR=%d\n",getPrimitiveCountR(g_prim,g_vertices.size()));
	*/

}
void RendererHLSLCg::drawCircleVrtl(const float x, const float y, const float r, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){
	uint col = toBGRA(color);

	bool lines = (lineWidth > 0);
	float d = 0.5f * lineWidth;

	PCVertex vertices[34];

	for (int i = 0; i < 16; i++){
		float ang = i * 2 * D3DXFROMWINE_PI / 16;
		float cosA = cosf(ang);
		float sinA = sinf(ang);

		if (lines){
			vertices[2 * i    ].vertex = D3DXFROMWINEVECTOR2(x + (r + d) * cosA, y + (r + d) * sinA);
			vertices[2 * i + 1].vertex = D3DXFROMWINEVECTOR2(x + (r - d) * cosA, y + (r - d) * sinA);
			//vertices[2 * i    ].color = color;
			//vertices[2 * i + 1].color = color;
		} else {
			vertices[i].vertex = D3DXFROMWINEVECTOR2(x + r * cosA, y + r * sinA);
			//vertices[i].color = color;
		}
	}

	InitPlain(plainShader, plainVF);
		setShader(plainShader);
		setVertexFormat(plainVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	if (lines){
		// Full loop
		vertices[32] = vertices[0];
		vertices[33] = vertices[1];
		DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 32, vertices, vertices, sizeof(PCVertex));
	} else {
		DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 14, vertices, vertices, sizeof(PCVertex));
	}
}

void RendererHLSLCg::drawTriangleVrtl(const float x0, const float y0, const float x1, const float y1, const float x2, const float y2, const D3DXFROMWINEVECTOR4 &color, const float lineWidth){
	uint col = toBGRA(color);

	if (lineWidth > 0){
		drawLine(x0, y0, x1, y1, color, lineWidth);
		drawLine(x1, y1, x2, y2, color, lineWidth);
		drawLine(x2, y2, x0, y0, color, lineWidth);
	} else {
		PCVertex vertices[3] = {
			D3DXFROMWINEVECTOR2(x0, y0),// color,
			D3DXFROMWINEVECTOR2(x1, y1),// color,
			D3DXFROMWINEVECTOR2(x2, y2),// color,
		};
	InitPlain(plainShader, plainVF);
	setShader(plainShader);
	setVertexFormat(plainVF);
	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	DrawPrimitiveUP(PRIM_TRIANGLES, 1, vertices, vertices, sizeof(PCVertex));
	}
}

void RendererHLSLCg::drawQuadVrtl(const float x0, const float y0, const float x1, const float y1, const float s0, const float t0, const float s1, const float t1, const D3DXFROMWINEVECTOR4 &color){
	uint col = toBGRA(color);

	PCTVertex vertices[4] = {
		D3DXFROMWINEVECTOR2(x0, y0),
		//color,
		D3DXFROMWINEVECTOR2(s0, t0),
		D3DXFROMWINEVECTOR2(x1, y0),
		//color,
		D3DXFROMWINEVECTOR2(s1, t0),
		D3DXFROMWINEVECTOR2(x1, y1),
		//color,
		D3DXFROMWINEVECTOR2(s1, t1),
		D3DXFROMWINEVECTOR2(x0, y1),
		//color,
		D3DXFROMWINEVECTOR2(s0, t1),
	};
#if 0
	InitPixel(texShader, texVF);
		setShader(texShader);
		setVertexFormat(texVF);
#else
	InitPlain(plainShader, plainVF);
		setShader(plainShader);
		setVertexFormat(plainVF);
#endif

	setup2DMode(0, (float) GetViewportWidth(), 0, (float) GetViewportHeight());
	setShaderConstant4f("scaleBias", GetscaleBias2D());
	setShaderConstant4f("colorRGBA", color);
	setDepthState(GetnoDepthTest());
	DrawPrimitiveUP(PRIM_TRIANGLE_FAN, 2, vertices, vertices, sizeof(PCTVertex));
}

#endif


static FORMAT formats3[] =
{
	FORMAT_NONE	 ,
	FORMAT_R8	 ,
	FORMAT_RG8	 ,
	FORMAT_RGB8	 ,
	FORMAT_RGBA8	 ,
	FORMAT_R16	 ,
	FORMAT_RG16	 ,
	FORMAT_RGB16	 ,
	FORMAT_RGBA16	 ,
	FORMAT_R8S	 ,
	FORMAT_RG8S	 ,
	FORMAT_RGB8S	 ,
	FORMAT_RGBA8S	 ,
	FORMAT_R16S	 ,
	FORMAT_RG16S	 ,
	FORMAT_RGB16S	 ,
	FORMAT_RGBA16S	 ,
	FORMAT_R16f	 ,
	FORMAT_RG16f	 ,
	FORMAT_RGB16f	 ,



	FORMAT_RGBA16f	 ,
	FORMAT_R32f	 ,
	FORMAT_RG32f	 ,
	FORMAT_RGB32f	 ,
	FORMAT_RGBA32f	 ,
	FORMAT_R16I	 ,
	FORMAT_RG16I	 ,
	FORMAT_RGB16I	 ,
	FORMAT_RGBA16I	 ,
	FORMAT_R32I	 ,
	FORMAT_RG32I	 ,
	FORMAT_RGB32I	 ,
	FORMAT_RGBA32I	 ,
	FORMAT_R16UI	 ,
	FORMAT_RG16UI	 ,
	FORMAT_RGB16UI	 ,
	FORMAT_RGBA16UI  ,
	FORMAT_R32UI	 ,
	FORMAT_RG32UI	 ,
	FORMAT_RGB32UI	 ,
	FORMAT_RGBA32UI  ,
	FORMAT_RGBE8	 ,
	FORMAT_RGB9E5	 ,
	FORMAT_RG11B10F  ,
	FORMAT_RGB565	 ,
	FORMAT_RGBA4	 ,
	FORMAT_RGB10A2	 ,
	FORMAT_D16	 ,
	FORMAT_D24	 ,
	FORMAT_D24S8	 ,
	FORMAT_D32F	 ,

	FORMAT_DXT1	 ,
	FORMAT_DXT3	 ,
	FORMAT_DXT5	 ,
	FORMAT_ATI1N	 ,
	FORMAT_ATI2N   ,


	FORMAT_RGB8	 ,
	FORMAT_RGB8	 ,
	FORMAT_RGBA8	 ,

}
;
static D3DFORMAT formats3d3d9[] =
{
	D3DFMT_UNKNOWN,
	D3DFMT_L8,
	D3DFMT_A8L8,
	D3DFMT_X8R8G8B8,
	D3DFMT_A8R8G8B8,
	D3DFMT_L16,
	D3DFMT_G16R16,
	D3DFMT_UNKNOWN, D3DFMT_A16B16G16R16,
	D3DFMT_UNKNOWN,
	D3DFMT_V8U8,
	D3DFMT_UNKNOWN,
	D3DFMT_Q8W8V8U8,
	D3DFMT_UNKNOWN,
	D3DFMT_V16U16,
	D3DFMT_UNKNOWN,
	D3DFMT_Q16W16V16U16,
	D3DFMT_R16F,
	D3DFMT_G16R16F,
	D3DFMT_UNKNOWN, D3DFMT_A16B16G16R16F,
	D3DFMT_R32F,
	D3DFMT_G32R32F,
	D3DFMT_UNKNOWN, D3DFMT_A32B32G32R32F,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN, D3DFMT_UNKNOWN, D3DFMT_UNKNOWN, D3DFMT_R5G6B5,
	D3DFMT_A4R4G4B4,
	D3DFMT_A2B10G10R10,
	D3DFMT_D16,
	D3DFMT_D24X8,
	D3DFMT_D24S8,
	D3DFMT_D32F_LOCKABLE,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
#ifdef _WIN32
	(D3DFORMAT) '1ITA',
	(D3DFORMAT) '2ITA',
#else
	D3DFMT_UNKNOWN,
	D3DFMT_UNKNOWN,
#endif
	D3DFMT_R8G8B8,
//	D3DFMT_B8G8R8,
	D3DFMT_X8B8G8R8,
	D3DFMT_A8B8G8R8,

}
;
FORMAT FORMAT3FromD3D (UINT D3DFORMAT )

{
	for(unsigned int i=0;i<59;i++)

		if(D3DFORMAT==formats3d3d9[i]) return formats3[i];
	return FORMAT_NONE;
}

/*






	PRIM_TRIANGLES	    = 0,
	PRIM_TRIANGLE_FAN   = 1,
	PRIM_TRIANGLE_STRIP = 2,
	PRIM_QUADS	    = 3,
	PRIM_LINES	    = 4,
	PRIM_LINE_STRIP     = 5,
	PRIM_LINE_LOOP	    = 6,
	PRIM_POINTS	    = 7,


	GL_TRIANGLES,
	GL_TRIANGLE_FAN,
	GL_TRIANGLE_STRIP,
	GL_QUADS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_POINTS,

		pt=D3DPT_TRIANGLELIST;
		pt=D3DPT_TRIANGLEFAN;
		pt=D3DPT_TRIANGLESTRIP;
		pt=(D3DPRIMITIVETYPE)0;
		pt=D3DPT_LINELIST;
		pt=D3DPT_LINESTRIP;
		pt=(D3DPRIMITIVETYPE)0;
		pt=D3DPT_POINTLIST;

		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D10_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,

		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
*/
#if 0
void RendererHLSLCg::XXX(Primitives prim,Primitives& rprim, int& rnv,BYTE& rb, int& rvs)
{
		BYTE* bi=0;
		int is=0;
		int ni=0;
			if(selectedIndexBuffer!=-1)
			{
			bi=indexBuffers[selectedIndexBuffer].Lock();
			is=indexBuffers[selectedIndexBuffer].indexSize;
			ni=indexBuffers[selectedIndexBuffer].nIndices;
			}

		BYTE* bv=vertexBuffers[selectedVertexBuffer].Lock();//all vertex buffers???
		vs=vertexBuffers[selectedVertexBuffer].indexSize;
		nv=vertexBuffers[selectedVertexBuffer].nIndices;
		VertexBufferID vb;
		if(prim==PRIM_QUADS)
		{
		if(!bi)
		{
			b=new BYTE[vs*nv];
			stx_memcpy(b,bv,vs*nv);
			rnv=nv;
		}
		else
		{
			int ic=getIndicesCount(prim);
			rnv=getVertexCount(prim,getPrimitiveCount(ni))*getPrimitiveCount(prim,ni);
			b=new BYTE[vs*rnv];
			DWORD dwi=0;
			WORD wi=0;
			BYTE* d;
			for(unsigned int i=0;i<ni;i=i+ic)
			{
				if(is==4)
				{
					d=&bv[vs*dwi];
					stx_memcpy(b+dwi*vs,d,vs*ic);
					dwi++;
				}
				else
				{
					d=&bv[vs*wi];
					stx_memcpy(b+wi*vs,d,vs*ic);
					wi++;
				}
							}
		}
			rprim=PRIM_TRIANGLE_STRIP;
			rb=b;
			rvs=vs;
		}
		else if(prim=PRIM_LINE_LOOP)
		{
		if(!bi)
		{
			b=new BYTE[(vs+1)*nv];
			stx_memcpy(b,bv,vs*nv);
			stx_memcpy(b+vs*nv,bv,vs);
			rnv=nv+1;
		}
		else
		{
			int ic=getIndicesCount(prim);
			rnv=1+getVertexCount(prim,getPrimitiveCount(ni))*getPrimitiveCount(prim,ni);
			b=new BYTE[vs*rnv];
			DWORD dwi=0;
			WORD wi=0;
			BYTE* d;

			for(unsigned int i=0;i<ni;i=i+ic)
			{
				if(is==4)
				{
					d=&bv[vs*dwi];
					stx_memcpy(b+dwi*vs,d,vs*ic);
					dwi++;
				}
				else
				{
					d=&bv[vs*wi];
					stx_memcpy(b+wi*vs,d,vs*ic);
					wi++;
				}
			stx_memcpy(b+vs*nv,bv,vs*is);
		}
		}
}

void Rendererdraw(Primitives prim)
{
			BYTE* b;
		int vs;
		int nv;
		Primitives& rprim;
		int rnv;
		BYTE rb;
		int rvs;
			if((!dynamic_cast<RendererGL_1_1*>(this))&&(prim==PRIM_LINE_LOOP))
			{
				XXX(prim,rprim, rnv,rb, rvs);
			}
		if((!dynamic_cast<RendererGL_1_1*>(this))&&(prim==PRIM_QUADS))
		{
			XXX(prim,rprim, rnv,rb, rvs);
							DrawPrimitiveUP(PRIM_TRIANGLE_STRIP,nv,b,b,vs);
		}
		//else
		//DrawIndexedPrimitiveUP(PRIM_TRIANGLE_STRIP,0,nv,np,bi,bi,f,b,b,vs);
		/*
    void DrawIndexedPrimitiveUP(Primitives PrimitiveType,
		UINT MinVertexIndex,
		UINT NumVertices,
		UINT PrimitiveCount,
		CONST void* pIndexData0,
		CONST void* pIndexData,
		int IndexDataFormat,
		CONST void* pVertexStreamZeroData0,
		CONST void* pVertexStreamZeroData,
		UINT VertexStreamZeroStride, const uint flags=RESET_ALL);
		*/
		}
}

#endif

#if defined(USE_GLES_SHADERS) || defined(IPHONE_SIMULATOR) || defined(OS_IPHONE) || defined(ANDROID)
#include <Framework3/RendererGLES2.h>
#else
#include <Framework3/RendererHLSLD3D11.h>
#include <Framework3/RendererHLSLD3D10.h>
#include <Framework3/RendererHLSLD3D9.h>
#include <Framework3/RendererGLSLGL_1_1.h>
#include <Framework3/RendererCgGL_1_1.h>
#endif
IRenderer* IRenderer::GetRendererInstance(int awidth, int aheight,
		const char* atitle, tRenderer art, bool InitLogFile,
			bool fullScreen
		,int argc, char *argv[])
{
	if(IRenderer::s_pRenderer)
	{
		return IRenderer::s_pRenderer;
	}

	IRenderer::width=awidth;
	IRenderer::height=aheight;

	LOG_START;
	unsigned int c=1;
	while(c<2)
	{
#ifdef  __FrameworkGLES2_H__
	IRenderer::s_pRenderer=new RendererGLES2(awidth, aheight,InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
#if defined(__Framework3D3D11_H__)		
	IRenderer::s_pRenderer=new RendererHLSLD3D11(awidth, aheight,InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
#if defined(__Framework3D3D10_H__)		
	IRenderer::s_pRenderer=new RendererHLSLD3D10(awidth, aheight,InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
#if defined(__Framework3D3D9_H__)		
	IRenderer::s_pRenderer=new RendererHLSLD3D9(awidth, aheight,InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
#ifdef __FrameworkCgGL_1_1_H__
	IRenderer::s_pRenderer=new RendererCgGL_1_1(awidth, aheight, InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
#ifdef  __FrameworkGLSLGL_1_1_H__
	IRenderer::s_pRenderer=new RendererGLSLGL_1_1(awidth, aheight,InitLogFile,fullScreen,argc,argv);
	if(IRenderer::s_pRenderer->IsOK()) break;
#endif
	c++;
	}
	char buf[1024] = {0};
	stx_strlcpy(buf, "Framework3 - ", 1024);
	stx_strlcat(buf, atitle, 1024);
	stx_strlcat(buf, " - ", 1024);
	stx_strlcat(buf, IRenderer::s_pRenderer->getString(), 1024);

	if(sizeof(void *) == 4)
		stx_strlcat(buf, " - 32 bit", 1024);
	else if(sizeof(void *) == 8)
		stx_strlcat(buf, " - 64 bit", 1024);

	STX_Service::GetWindowInstance()->SetCaption(buf);
	IRenderer::s_pRenderer->setViewport(0, 0, awidth, aheight);

	return IRenderer::s_pRenderer;
}
#if 0
	const char* IRenderer::getString()
	{
		static char buf[256] = {0};
		static char os[10] = {0};
#if defined(WIN_PHONE_APP)
        stx_snprintf(os,256,"Windows Phone\0");
#elif defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
        stx_snprintf(os,256,"iPhone\0");
#elif defined(ANDROID)
        stx_snprintf(os,256,"android\0");
#elif defined(QT_BUILD)
        stx_snprintf(os,256,"Symbian\0");
#elif defined(_WIN32) || defined(WIN8_APP)
        stx_snprintf(os,256,"Windows\0");
#elif defined(__GNUC__) && !defined(__MINGW32__)
        stx_snprintf(os,256,"Linux\0");
#endif

#if 0
#if !defined(SDL_2_0_4_BUILD)
#if defined( __Framework3D3D9_H__)
    if(dynamic_cast<RendererHLSLD3D9*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s D3D9\0", os);
#elif defined(__Framework3D3D10_H__)
    if(dynamic_cast<RendererHLSLD3D10*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s D3D10\0", os);
#elif defined(__Framework3D3D11_H__)
    if(dynamic_cast<RendererHLSLD3D11*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s D3D11\0", os);
#elif defined(__FrameworkGLSLGL_1_1_H__)
    if(dynamic_cast<RendererGLSLGL_1_1*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s GLSLGL_1_1\0", os);
#elif defined(__FrameworkGLES2_H__)
    if(dynamic_cast<RendererGLES2*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s GLES2\0", os);
#elif defined(__FrameworkCgGL_1_1_H__)
    if(dynamic_cast<RendererCgGL_1_1*>(IRenderer::GetRendererInstance())) stx_snprintf(buf,256,"%s CgGL_1_1\0", os);
	#endif

#else
stx_snprintf(buf,256,"%s GLSLGL_1_1\0", os);
#endif
#endif
	return buf;
	}
#endif

IRenderer* IRenderer::s_pRenderer=0;

int RendererHLSLCg::addAnimation(char* afile){return -1;}
int RendererHLSLCg::addMesh(char* afile){return -1;}
int RendererHLSLCg::addSound(char* afile, char* aname){return -1;}

void RendererHLSLCg::deleteAnimation(char* afile){}
void RendererHLSLCg::deleteMesh(char* afile){}

void RendererHLSLCg::deleteSound(char* afile, char* aname){}

void RendererHLSLCg::renderAnimation(int id, int x, int y, int sx, int sy){}
void RendererHLSLCg::renderMesh(int id, D3DXFROMWINEMATRIX mvp){}
void RendererHLSLCg::playSound(int id){}
void RendererHLSLCg::stopSound(int id){}

/*
GL_LINE_LOOP,
GL_QUADS,
D3DPT_TRIANGLEFAN

     DWORD   MaxPrimitiveCount;          // max number of primitives per DrawPrimitive call
    DWORD   MaxVertexIndex;
    DWORD   MaxStreams;
    DWORD   MaxStreamStride;            // max stride for SetStreamSource

GL_MAX_ELEMENTS_INDICES
GL_MAX_ELEMENTS_VERTICES

# GL_LINE_STRIP— Use this primitive to draw a sequence of connected line segments. OpenGL renders a line segment between the first and second vertices, between the second and third, between the third and fourth, and so on. If the application specifies n vertices, OpenGL renders n–1 line segments.
# GL_LINE_LOOP— Use this primitive to close a line strip. OpenGL renders this primitive like a GL_LINE_STRIP with the addition of a closing line segment between the final and first vertices.

A triangle fan is similar to a triangle strip, except that all the triangles share one vertex, as shown in the following illustration.
The system uses vertices v2, v3, and v1 to draw the first triangle; v3, v4, and v1 to draw the second triangle; v4, v5, and v1 to draw the third triangle; and so on. When flat shading is enabled, the system shades the triangle with the color from its first vertex.

	GL_TRIANGLES,
	GL_TRIANGLE_FAN,
	GL_TRIANGLE_STRIP,
	GL_QUADS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_POINTS,

		pt=D3DPT_TRIANGLELIST;
		pt=D3DPT_TRIANGLEFAN;
		pt=D3DPT_TRIANGLESTRIP;
		pt=(D3DPRIMITIVETYPE)0;
		pt=D3DPT_LINELIST;
		pt=D3DPT_LINESTRIP;
		pt=(D3DPRIMITIVETYPE)0;
		pt=D3DPT_POINTLIST

		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, 	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, 			D3D10_PRIMITIVE_TOPOLOGY_LINELIST,
		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
		D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,
*/
/*
GL_MAX_3D_TEXTURE_SIZE





                            params returns one value,
                            a rough estimate of the largest 3D texture that the GL can handle.
                            The value must be at least 16.
                            If the GL version is 1.2 or greater, use
                            GL_PROXY_TEXTURE_3D to determine if a texture is too large.
                            See glTexImage3D.

GL_MAX_CLIENT_ATTRIB_STACK_DEPTH





                            params returns one value indicating the maximum supported depth
                            of the client attribute stack.
                            See glPushClientAttrib.

GL_MAX_ATTRIB_STACK_DEPTH





                            params returns one value,
                            the maximum supported depth of the attribute stack. The value must be
                            at least 16.
                            See glPushAttrib.

GL_MAX_CLIP_PLANES





                            params returns one value,
                            the maximum number of application-defined clipping planes. The value must be at least 6.
                            See glClipPlane.

GL_MAX_COLOR_MATRIX_STACK_DEPTH





                            params returns one value, the maximum supported depth of the color matrix
                            stack.  The value must be at least 2.
                            See glPushMatrix.

GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS





                            params returns one value, the maximum supported texture image units that
                            can be used to access texture maps from the vertex shader and the fragment processor combined.
                            If both the vertex shader and the fragment processing stage access the same texture image
                            unit, then that counts as using two texture image units against this limit.
                            The value must be at least 2.
                            See glActiveTexture.

GL_MAX_CUBE_MAP_TEXTURE_SIZE





                            params returns one value.
                            The value gives a rough estimate of the largest cube-map texture that
                            the GL can handle. The value must be at least 16.
                            If the GL version is 1.3 or greater, use GL_PROXY_TEXTURE_CUBE_MAP
                            to determine if a texture is too large.
                            See glTexImage2D.

GL_MAX_DRAW_BUFFERS





                            params returns one value, the maximum number
                            of simultaneous output colors allowed from a fragment shader using the
                            gl_FragData built-in array.  The value must be at least 1.
                            See glDrawBuffers.

GL_MAX_ELEMENTS_INDICES





                            params returns one value,
                            the recommended maximum number of vertex array indices.
                            See glDrawRangeElements.

GL_MAX_ELEMENTS_VERTICES





                            params returns one value,
                            the recommended maximum number of vertex array vertices.
                            See glDrawRangeElements.

GL_MAX_EVAL_ORDER





                            params returns one value,
                            the maximum equation order supported by 1D and 2D
                            evaluators. The value must be at least 8.

                            See glMap1 and glMap2.

GL_MAX_FRAGMENT_UNIFORM_COMPONENTS





                            params returns one value,
                            the maximum number of individual floating-point, integer, or boolean values that can be held
                            in uniform variable storage for a fragment shader. The value must be at least 64.
                            See glUniform.

GL_MAX_LIGHTS





                            params returns one value,
                            the maximum number of lights. The value must be at least 8.
                            See glLight.

GL_MAX_LIST_NESTING





                            params returns one value,
                            the maximum recursion depth allowed during display-list
                            traversal. The value must be at least 64.
                            See glCallList.

GL_MAX_MODELVIEW_STACK_DEPTH





                            params returns one value,
                            the maximum supported depth of the modelview matrix stack. The value must
                            be at least 32.
                            See glPushMatrix.

GL_MAX_NAME_STACK_DEPTH





                            params returns one value,
                            the maximum supported depth of the selection name stack. The value must be at least 64.
                            See glPushName.

GL_MAX_PIXEL_MAP_TABLE





                            params returns one value,
                            the maximum supported size of a glPixelMap lookup table.
                            The value must be at least 32.
                            See glPixelMap.

GL_MAX_PROJECTION_STACK_DEPTH





                            params returns one value, the maximum supported depth of the projection
                            matrix stack. The value must be at least 2.
                            See glPushMatrix.

GL_MAX_TEXTURE_COORDS





                            params returns one value,
                            the maximum number of texture coordinate sets available to vertex and fragment shaders.
                            The value must be at least 2.
                            See glActiveTexture and
                            glClientActiveTexture.

GL_MAX_TEXTURE_IMAGE_UNITS





                            params returns one value, the maximum supported texture image units that
                            can be used to access texture maps from the fragment shader.
                            The value must be at least 2.
                            See glActiveTexture.

GL_MAX_TEXTURE_LOD_BIAS





                            params returns one value,
                            the maximum, absolute value of the texture level-of-detail bias.  The
                            value must be at least 4.

GL_MAX_TEXTURE_SIZE





                            params returns one value.
                            The value gives a rough estimate of the largest texture that
                            the GL can handle. The value must be at least 64.
                            If the GL version is 1.1 or greater, use
                            GL_PROXY_TEXTURE_1D or GL_PROXY_TEXTURE_2D
                            to determine if a texture is too large.
                            See glTexImage1D and glTexImage2D.

GL_MAX_TEXTURE_STACK_DEPTH





                            params returns one value,
                            the maximum supported depth of the texture matrix stack. The value must be at least 2.
                            See glPushMatrix.

GL_MAX_TEXTURE_UNITS





                            params returns a single value indicating the number of conventional
                            texture units supported. Each conventional texture unit includes both a texture coordinate set
                            and a texture image unit.  Conventional texture units may be used for fixed-function (non-shader)
                            rendering.  The value must be at least 2.  Additional texture coordinate sets and texture
                            image units may be accessed from vertex and fragment shaders.
                            See glActiveTexture and
                            glClientActiveTexture.

GL_MAX_VARYING_FLOATS





                            params returns one value,
                            the maximum number of interpolators available for processing varying variables used by
                            vertex and fragment shaders. This value represents the number of individual floating-point
                            values that can be interpolated; varying variables declared as vectors, matrices, and arrays
                            will all consume multiple interpolators. The value must be at least 32.

GL_MAX_VERTEX_ATTRIBS





                            params returns one value,
                            the maximum number of 4-component generic vertex attributes accessible to a vertex shader.
                            The value must be at least 16.
                            See glVertexAttrib.

GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS





                            params returns one value, the maximum supported texture image units that
                            can be used to access texture maps from the vertex shader. The value may be 0.
                            See glActiveTexture.

GL_MAX_VERTEX_UNIFORM_COMPONENTS





                            params returns one value,
                            the maximum number of individual floating-point, integer, or boolean values that can be held
                            in uniform variable storage for a vertex shader. The value must be at least 512.
                            See glUniform.

GL_MAX_VIEWPORT_DIMS





                            params returns two values:
                            the maximum supported width and height of the viewport.
                            These must be at least as large as the visible dimensions of the display
                            being rendered to.
                            See glViewport.

*/

