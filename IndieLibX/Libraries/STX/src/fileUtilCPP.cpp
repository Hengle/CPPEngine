/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
/*
ILboolean ILAPIENTRY ilutD3D10TexFromFileInMemory(ID3D10Device *Device, void *Lump, ILuint Size, ID3D10Texture2D **Texture)
{
	iBindImageTemp();
	if (!ilLoadL(IL_TYPE_UNKNOWN, Lump, Size))
		return IL_FALSE;

	*Texture = ilutD3D10Texture(Device);

	return IL_TRUE;
}
*/
/*
ASSIMP_API const C_STRUCT aiScene* aiImportFileFromMemory(
    const char* pBuffer,
    unsigned int pLength,
    unsigned int pFlags,
    const char* pHint);

ASSIMP_API const C_STRUCT aiScene* aiImportFileFromMemoryWithProperties(
    const char* pBuffer,
    unsigned int pLength,
    unsigned int pFlags,
    const char* pHint,
    const C_STRUCT aiPropertyStore* pProps);
*/
#include <STX/STX.h>
using namespace std;

#if 0//defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define LOG_FNLN_X LOG_FNLN
#define LOG_PRINT_X LOG_PRINT
#else
#define LOG_FNLN_X
#define LOG_PRINT_X
#endif

#if defined(_MSC_VER)
#define search 	'/'
#define repl 	'\\'
#else
#define search 	'\\'
#define repl	'/'
#endif

#if defined(ANDROID)
#elif defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#elif defined(__APPLE__)
#include <stdio.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

#include <sstream>

std::string replaceAll_( const std::string& s, const std::string& f, const std::string& r )
{
if (
	s.empty() ||
	f.empty() ||
	f == r ||
	s.find(f) == std::string::npos
	)
{
return s;
}
ostringstream build_it;
size_t i = 0;
for ( size_t pos; ( pos = s.find( f, i ) ) != string::npos; ) {
build_it.write( &s[i], pos - i );
build_it << r;
i = pos + f.size();
}
if ( i != s.size() ) {
build_it.write( &s[i], s.size() - i );
}
return build_it.str();
}

// LNX: $HOME/Asiakirjat/IndieLib_resources
// OSX: $HOME/Documents/IndieLib_resources
// MSC: C:\\Users\\Tommi\\Documents\\IndieLib_resources
// NDK: IndieLib_resources
const char* pathIL()
{
	// Make sure we're running in the exe's path
	static char path[MAX_PATH];
	path[0]='\0';
	char *slash = 0;
#if defined(ANDROID)
	//stx_strlcpy(path, "/mnt/sdcard/", MAX_PATH);
#elif defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
    	stx_strlcpy(path, iosDocumentsFolder(), MAX_PATH);
	printf("iosDocumentsFolder=%s\n", path);
#elif defined(__APPLE__)
	uint32_t bufsize = MAX_PATH;
	_NSGetExecutablePath(path, &bufsize);
	chdir(path);
	path[0]='\0';
	stx_strlcpy(path, "./Documents", MAX_PATH);
#elif defined(LINUX)
	// Make sure we're running in the exe's directory
	if (realpath("/proc/self/exe", path)){
		char *slash = stx_strrchr(path, '/');
		if (slash) *slash = '\0';
		chdir(path);
		path[0]='\0';
	}
	stx_strlcpy(path, "../..", MAX_PATH);
#elif defined(_MSC_VER)
	if (GetModuleFileName(NULL, path, sizeof(path)))
	{
		char *slash = stx_strrchr(path, '\\');
		if (slash) *slash = '\0';
        	chdir(path);
		path[0]='\0';
	}
	stx_strlcpy(path, "..\\\\..", MAX_PATH);
#endif
#if !defined(ANDROID)
	stx_strlcat(path, "/IndieLib_resources", MAX_PATH);
#else
	stx_strlcat(path, "IndieLib_resources", MAX_PATH);
#endif
	if (path) 
	{
#if defined(ANDROID)
		LOG_PRINT_X("File=%s\n", path);
#endif
		return path;
	}
	return 0;
}
std::string stx_convertpath(std::string szPath)
{
	static std::string tmp;
	tmp="";
	tmp.append(pathIL());
	tmp.append(szPath);

	std::string from;
	std::string to;
#if defined(_MSC_VER)
	from="/";
	to="\\\\";
#else
	from="\\\\";
	to="/";
#endif
	tmp=replaceAll_(tmp, from, to);	

	from=pathIL();
	from.append(pathIL());
	to=pathIL();
	tmp=replaceAll_(tmp, from, to);
#if defined(_MSC_VER)
	from="\\\\.\\\\";
	to="\\\\";
#else
	from="/./";
	to="/";
#endif
	tmp=replaceAll_(tmp, from, to);

	printf("%s:\n%s:%d\n", __FILE__,__FUNCTION__, __LINE__);
	printf("%s:\n", tmp.c_str());

	return tmp;
}

const char* STXCALL stx_convertpathC(const char* szPath)
{
	printf("%s:\n%s:%d\n", __FILE__,__FUNCTION__, __LINE__);
	printf("%s:\n", szPath);
	std::string szPath2=szPath;
	std::string szPath3=stx_convertpath(szPath2);
	if(stx_strcmp(szPath3.c_str(),"")==0)
	{
		szPath3="../IndieLib_resources/";
		szPath3.append(szPath);
	}
	printf("%s:\n%s:%d\n", __FILE__,__FUNCTION__, __LINE__);
	printf("%s:\n", szPath3.c_str());
	return szPath3.c_str();
}

