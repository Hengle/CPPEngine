/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
/**
 *  \file stx_stdinc.h
 *
 *  This is a general header that includes C language support.
 */

#ifndef _stx_stdinc_h
#define _stx_stdinc_h

#include <limits.h>
#if !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
#include <cmath>
#else
#include <math.h>
#endif
#ifndef SWIG
#include <algorithm>
#endif
#ifndef _MSC_VER
#include <stdint.h>
#else
#include <windows.h>
#include <Mmsystem.h>
#endif
#include <stddef.h>
#include <signal.h>

#if defined(ANDROID)
#include <unistd.h>
//Call: sleep(seconds) or usleep(microseconds)
#endif

#if /*!defined(WIN_PHONE_APP) && */!defined (QT_BUILD)
#if defined(ANDROID) || defined(WIN_PHONE_APP) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
    #undef SDL_1_2_15_BUILD
    #undef SDL_2_0_4_BUILD
    //define GL_CODE_BUILD 1

typedef int8_t Sint8;
typedef uint8_t Uint8;
typedef int16_t Sint16;
typedef uint16_t Uint16;

    typedef int32_t Sint32;
    typedef uint32_t Uint32;
    typedef int64_t Sint64;
    typedef uint64_t Uint64;

#elif defined(_MSC_VER) //|| defined(__APPLE__)
    #include <SDL.h>
    #include <SDL_syswm.h>
    #define SDL_2_0_4_BUILD 1
#elif 0//defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
    #include <SDL.h>
    #define SDL_2_0_4_BUILD 1
#elif 1
    #include <SDL2/SDL.h>
#if defined(SDL_VIDEO_DRIVER_X11) || defined(SDL_VIDEO_DRIVER_WINDOWS)
    #include <SDL2/SDL_syswm.h>
#endif
    #define SDL_2_0_4_BUILD 1
#else
    #include <SDL/SDL.h>
#if defined(SDL_VIDEO_DRIVER_X11)
    #include <SDL/SDL_syswm.h>
#endif
    #define SDL_1_2_15_BUILD 1
#endif
#else
#ifdef _MSC_VER
typedef signed __int8		Sint8;
typedef unsigned __int8		Uint8;
typedef signed __int16		Sint16;
typedef unsigned __int16	Uint16;
#endif
typedef int32_t	    		Sint32;
typedef uint32_t		Uint32;
#if HAVE_64
#ifdef _MSC_VER
typedef signed __int64		int64_t;
typedef unsigned __int64	uint64_t;
typedef signed __int64		Sint64;
typedef unsigned __int64	Uint64;
#endif
typedef long long int		int64_t;
typedef long long unsigned int	uint64_t;
typedef long long int	        Sint64;
typedef long long unsigned int	Uint64;
#endif
//elif defined(WIN_PHONE_APP) || defined(WIN8_APP) || defined(WIN_PHONE_APP) || defined(WIN8_APP) || defined(ANDROID) || defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
/**
 * \brief A signed 8-bit integer type.
 */
typedef int8_t Sint8;
/**
 * \brief An unsigned 8-bit integer type.
 */
typedef uint8_t Uint8;
/**
 * \brief A signed 16-bit integer type.
 */
typedef int16_t Sint16;
/**
 * \brief An unsigned 16-bit integer type.
 */
typedef uint16_t Uint16;
/**
 * \brief A signed 32-bit integer type.
 */
typedef int32_t Sint32;
/**
 * \brief An unsigned 32-bit integer type.
 */
typedef uint32_t Uint32;
#if HAVE_64
/**
 * \brief A signed 64-bit integer type.
 */
typedef int64_t Sint64;
/**
 * \brief An unsigned 64-bit integer type.
 */
typedef uint64_t Uint64;
#endif
#endif

/* @} *//* Basic data types */

#if defined(_MSC_VER)
#define APPLICATIONRESOURCES "\\\\IndieLib_resources"
#else
#define APPLICATIONRESOURCES "/IndieLib_resources"
#endif

#ifndef SWIG
#ifndef DBG_HALT
#ifdef _WIN32
#define DBG_HALT __asm{ int 0x10 }
#else
//define DBG_HALT {stx_exit(-1);}
#define DBG_HALT {raise(SIGABRT);}
#endif
#endif
#endif

#ifdef _MSC_VER
#include <functional>
#endif
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <memory.h>

#if defined(__cplusplus)
#include <ctype.h>
#include <string>
#endif

#ifndef _MSC_VER
#include <stddef.h>
#endif

typedef enum _stx_bool
{
    stx_FALSE = 0,
    stx_TRUE = 1
} stx_bool;

#define STXEXTERN
#define STXDECLSPEC
#define STXCALL
#define STXAPI
#define STXAPIENTRY

STXEXTERN STXDECLSPEC std::string STXCALL stx_GetFileExtension(const std::string& path);
STXEXTERN STXDECLSPEC std::string STXCALL stx_GetFileName(const std::string& path);

STXEXTERN STXDECLSPEC std::string STXCALL stx_convertpath(std::string szPath);

STXEXTERN STXDECLSPEC bool STXCALL str_ends_with(const char * str, const char * suffix);

STXEXTERN STXDECLSPEC bool STXCALL str_ends_with(const std::string& s, const std::string& suffix);

#ifdef __cplusplus
extern "C" {
#endif

long getNativeHeapSize();
long getNativeHeapAllocatedSize();
long getNativeHeapFreeSize();

#if defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
const char *iosDocumentsFolder();
void iosLog(const char * msg);
#endif
STXEXTERN STXDECLSPEC const char* STXCALL stx_convertpathC(const char* szPath);
STXEXTERN STXDECLSPEC char * STXCALL stx_getcwd(char * _Path);
STXEXTERN STXDECLSPEC size_t STXCALL stx_strlen(const char *str);
STXEXTERN STXDECLSPEC size_t STXCALL stx_strlcat(char *dst, const char *src, size_t maxlen);
STXEXTERN STXDECLSPEC size_t STXCALL stx_strlcpy(char *dst, const char *src, size_t maxlen);
STXEXTERN STXDECLSPEC int STXCALL stx_strcmp(const char *str1, const char *str2);
STXEXTERN STXDECLSPEC int STXCALL stx_min(int x, int y);
#if 0
STXEXTERN STXDECLSPEC void STXCALL CloseSTXWindow();
STXEXTERN STXDECLSPEC void STXCALL STXSwapBuffers();
STXEXTERN STXDECLSPEC void STXCALL OpenSTXWindow(unsigned int awidth, unsigned int aheight, unsigned int abpp);
#endif

#if 0//defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
namespace std
{
	inline double fabs(double f)
	{
    		if (f < 0) return -f;
    		return f;
	}
};
#endif

#ifdef __cplusplus
}
#endif

STXEXTERN STXDECLSPEC void *STXCALL stx_malloc(size_t size);
STXEXTERN STXDECLSPEC void *STXCALL stx_calloc(size_t nmemb, size_t size);
STXEXTERN STXDECLSPEC void *STXCALL stx_realloc(void *mem, size_t size);
STXEXTERN STXDECLSPEC void STXCALL stx_free(void *mem);

STXEXTERN STXDECLSPEC char *STXCALL stx_getenv(const char *name);
//STXEXTERN STXDECLSPEC int STXCALL stx_setenv(const char *name, const char *value, int overwrite);

STXEXTERN STXDECLSPEC void STXCALL stx_qsort(void *base, size_t nmemb, size_t size, int (*compare) (const void *, const void *));

STXEXTERN STXDECLSPEC int STXCALL stx_abs(int x);

STXEXTERN STXDECLSPEC int STXCALL stx_min(int x, int y);
STXEXTERN STXDECLSPEC int STXCALL stx_max(int x, int y);

STXEXTERN STXDECLSPEC int STXCALL stx_isdigit(int x);
STXEXTERN STXDECLSPEC int STXCALL stx_isspace(int x);
STXEXTERN STXDECLSPEC int STXCALL stx_toupper(int x);
STXEXTERN STXDECLSPEC int STXCALL stx_tolower(int x);

STXEXTERN STXDECLSPEC void *STXCALL stx_memset(void *dst, int c, size_t len);

STXEXTERN STXDECLSPEC void *STXCALL stx_memcpy(void *dst, const void *src, size_t len);

STXEXTERN STXDECLSPEC void *STXCALL stx_memmove(void *dst, const void *src, size_t len);
STXEXTERN STXDECLSPEC int STXCALL stx_memcmp(const void *s1, const void *s2, size_t len);

STXEXTERN STXDECLSPEC size_t STXCALL stx_wcslen(const wchar_t *wstr);
STXEXTERN STXDECLSPEC size_t STXCALL stx_wcslcpy(wchar_t *dst, const wchar_t *src, size_t maxlen);
STXEXTERN STXDECLSPEC size_t STXCALL stx_wcslcat(wchar_t *dst, const wchar_t *src, size_t maxlen);

STXEXTERN STXDECLSPEC size_t STXCALL stx_strlen(const char *str);
STXEXTERN STXDECLSPEC size_t STXCALL stx_strlcpy(char *dst, const char *src, size_t maxlen);
STXEXTERN STXDECLSPEC size_t STXCALL stx_utf8strlcpy(char *dst, const char *src, size_t dst_bytes);
STXEXTERN STXDECLSPEC size_t STXCALL stx_strlcat(char *dst, const char *src, size_t maxlen);
STXEXTERN STXDECLSPEC char *STXCALL stx_strdup(const char *str);
STXEXTERN STXDECLSPEC char *STXCALL stx_strrev(char *str);
STXEXTERN STXDECLSPEC char *STXCALL stx_strupr(char *str);
STXEXTERN STXDECLSPEC char *STXCALL stx_strlwr(char *str);
STXEXTERN STXDECLSPEC char *STXCALL stx_strchr(const char *str, int c);
STXEXTERN STXDECLSPEC char *STXCALL stx_strrchr(const char *str, int c);
STXEXTERN STXDECLSPEC char *STXCALL stx_strstr(const char *haystack, const char *needle);

STXEXTERN STXDECLSPEC char *STXCALL stx_itoa(int value, char *str, int radix);
STXEXTERN STXDECLSPEC char *STXCALL stx_uitoa(unsigned int value, char *str, int radix);
STXEXTERN STXDECLSPEC char *STXCALL stx_ltoa(long value, char *str, int radix);
STXEXTERN STXDECLSPEC char *STXCALL stx_ultoa(unsigned long value, char *str, int radix);

STXEXTERN STXDECLSPEC int STXCALL stx_atoi(const char *str);
STXEXTERN STXDECLSPEC double STXCALL stx_atof(const char *str);
STXEXTERN STXDECLSPEC long STXCALL stx_strtol(const char *str, char **endp, int base);
STXEXTERN STXDECLSPEC unsigned long STXCALL stx_strtoul(const char *str, char **endp, int base);
#if HAVE_64
STXEXTERN STXDECLSPEC char *STXCALL stx_lltoa(Sint64 value, char *str, int radix);
STXEXTERN STXDECLSPEC char *STXCALL stx_ulltoa(Uint64 value, char *str, int radix);
STXEXTERN STXDECLSPEC Sint64 STXCALL stx_strtoll(const char *str, char **endp, int base);
STXEXTERN STXDECLSPEC Uint64 STXCALL stx_strtoull(const char *str, char **endp, int base);
#endif
STXEXTERN STXDECLSPEC double STXCALL stx_strtod(const char *str, char **endp);

STXEXTERN STXDECLSPEC int STXCALL stx_strcmp(const char *str1, const char *str2);
STXEXTERN STXDECLSPEC int STXCALL stx_strncmp(const char *str1, const char *str2, size_t maxlen);
STXEXTERN STXDECLSPEC int STXCALL stx_strcasecmp(const char *str1, const char *str2);
STXEXTERN STXDECLSPEC int STXCALL stx_strncasecmp(const char *str1, const char *str2, size_t len);

STXEXTERN STXDECLSPEC int STXCALL stx_sscanf(const char *text, const char *fmt, ...);
STXEXTERN STXDECLSPEC int STXCALL stx_vsscanf(const char *text, const char *fmt, va_list ap);
STXEXTERN STXDECLSPEC int STXCALL stx_snprintf(char *text, size_t maxlen, const char *fmt, ...);
STXEXTERN STXDECLSPEC int STXCALL stx_vsnprintf(char *text, size_t maxlen, const char *fmt, va_list ap);

#ifndef HAVE_M_PI
#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288   /* pi */
#endif
#endif
/*
STXEXTERN STXDECLSPEC double STXCALL stx_acos(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_asin(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_atan(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_atan2(double x, double y);
STXEXTERN STXDECLSPEC double STXCALL stx_ceil(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_copysign(double x, double y);
STXEXTERN STXDECLSPEC double STXCALL stx_cos(double x);
STXEXTERN STXDECLSPEC float STXCALL stx_cosf(float x);
STXEXTERN STXDECLSPEC double STXCALL stx_fabs(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_floor(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_log(double x);
STXEXTERN STXDECLSPEC double STXCALL stx_pow(double x, double y);
STXEXTERN STXDECLSPEC double STXCALL stx_scalbn(double x, int n);
STXEXTERN STXDECLSPEC double STXCALL stx_sin(double x);
STXEXTERN STXDECLSPEC float STXCALL stx_sinf(float x);
STXEXTERN STXDECLSPEC double STXCALL stx_sqrt(double x);
*/
//define stx_itoa(value, string, radix)	stx_ltoa((long)value, string, radix)
STXEXTERN STXAPI char * STXAPIENTRY stx_ltoa(long value, char *string, int radix);
//define stx_uitoa(value, string, radix)	stx_ultoa((long)value, string, radix)
STXEXTERN STXAPI char * STXAPIENTRY stx_ultoa(unsigned long value, char *string, int radix);

STXEXTERN STXAPI int STXAPIENTRY stx_fileExists(const char* fileName);
STXEXTERN STXAPI int STXAPIENTRY stx_isopened(FILE* _File);
STXEXTERN STXAPI void STXAPIENTRY stx_closeall();
STXEXTERN STXAPI char * STXAPIENTRY stx_getcwd(char * _Path);
STXEXTERN STXAPI int STXAPIENTRY stx_chdir(const char * _Path);
//STXEXTERN STXAPI int STXAPIENTRY stx_mkdir(const char * _Path);
//STXEXTERN STXAPI int STXAPIENTRY stx_rmdir(const char * _Path);

STXEXTERN STXAPI unsigned long STXAPIENTRY stx_randnext(void);
STXEXTERN STXAPI int STXAPIENTRY stx_rand(void);
STXEXTERN STXAPI void STXAPIENTRY stx_srand(unsigned int seed);
STXEXTERN STXAPI void STXAPIENTRY stx_exit(int a);

#ifndef __inttype1__
#define __inttype1__

#ifdef _WIN32
            enum inttype1 {
                DO1_INT,
                DO1_LONG,
                DO1_LONGLONG
            };// _inttype1 = DO_INT;

            enum inttype2 {
                DO2_SHORT,
                DO2_INT,
                DO2_LONG,
                DO2_LONGLONG
            };
//define _INTTYPE1 inttype1
//define _INTTYPE2 inttype2
#else
typedef enum _inttype1 {
                DO1_INT,
                DO1_LONG,
                DO1_LONGLONG
            } inttype1;

typedef            enum _inttype2 {
                DO2_SHORT,
                DO2_INT,
                DO2_LONG,
                DO2_LONGLONG
            } inttype2;
//define _INTTYPE1 _inttype1
//define _INTTYPE2 _inttype2
#endif
#endif

/* Ends C function definitions when using C++ */

#ifndef MAX_PATH
#define MAX_PATH 260
#endif
//define MAX_BUF_SIZE 65535

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef const char*         LPCSTR;
typedef unsigned int        UINT;

STXEXTERN STXAPI const char STXAPIENTRY stx_getOsSeparator();
STXEXTERN STXAPI int STXAPIENTRY stx_fileExists(const char* fileName);
//STXEXTERN STXAPI size_t STXAPIENTRY stx_fwrite(const void * _Str,  size_t _Size,  size_t _Count, FILE* _File);


#define stxEPSILON     0.0001f

#define STX_SEEK_CUR    1
#define STX_SEEK_END    2
#define STX_SEEK_SET    0

#if !defined(SWIG)
#ifdef __cplusplus
   extern "C" 
#endif
int main(int argc, char *argv[]);

#ifdef __cplusplus
   extern "C" 
#endif
int SDL_main(int argc, char *argv[]);

#if defined(ANDROID)
#define ApplicationLogic() mainX(int argc, char *argv[])
#elif defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
#define ApplicationLogic() mainX(int argc, char *argv[])
#elif defined(WIN_PHONE_APP)
#define ApplicationLogic() mainX(int argc, char *argv[])
#elif defined(_MSC_VER) && !defined(WIN_PHONE_APP)
//pragma comment(linker, "/ENTRY:WinMainCRTStartup")
//pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#define ApplicationLogic() WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#elif !defined(_MSC_VER)
#define ApplicationLogic() main(int argc, char *argv[])
#endif 
#endif

#define LOG_TRACE_ON 1
#if defined(_DEBUG) && defined(LOG_TRACE_ON)
#if defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
// See in file /home/tronty/IndieLibX/Libraries/STX/src/fileUtilM.m function iosLog
#define LOG_START iosLog("")
#define LOG_PRINT(...) do { \
    std::string iosbuf; \
    iosbuf.resize(1024); \
    int num_bytes = snprintf(&iosbuf[0], 1024, __VA_ARGS__); \
    if(num_bytes < 1024) iosbuf.resize(num_bytes); \
    iosLog(iosbuf.c_str()); \
} while (0)
#define LOG_FNLN do { \
    std::string iosbuf; \
    iosbuf.resize(1024); \
  	int num_bytes = snprintf(&iosbuf[0], 1024, "%s:%s:%d\n", __FILE__,__FUNCTION__, __LINE__); \
	if(num_bytes < 1024) iosbuf.resize(num_bytes); \
    iosLog(iosbuf.c_str()); \
} while (0)
#elif defined(ANDROID)

#if 0
#define LOG_TAG "libIndieLibX"
#define LOG_START
#define LOG_PRINT
#define LOG_FNLN
#else
#include <jni.h>
#include <android/log.h>
#define LOG_TAG "libIndieLibX"
#define LOG_START
#define LOG_PRINT(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
//define LOG_PRINT(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);
#define LOG_FNLN __android_log_print(ANDROID_LOG_INFO,LOG_TAG, "%s:%s:%d\n", __FILE__,__FUNCTION__, __LINE__);
#endif
#elif defined(WIN_PHONE_APP)
#define LOG_START
#define LOG_PRINT
#define LOG_FNLN
#elif defined(_MSC_VER)
#define LOG_START
#define LOG_PRINT
#define LOG_FNLN
#else
#define LOG_START do { \
} while (0)
#define LOG_PRINT(...) do { \
  	printf(__VA_ARGS__); \
} while (0)
#define LOG_FNLN  do { \
  	printf("%s:%s:%d\n", __FILE__, __FUNCTION__, __LINE__); \
} while (0)
#endif
#endif

#define ErrorMsg LOG_PRINT

#ifdef __cplusplus
STXEXTERN STXAPI std::string& replaceAll(std::string& context, const std::string& from, const std::string& to);
#endif

STXEXTERN STXAPI bool STXAPIENTRY stx_isBigEndian();
STXEXTERN STXAPI bool STXAPIENTRY stx_isLittleEndian();
STXEXTERN STXAPI void STXAPIENTRY stx_exit(int a);
STXEXTERN STXAPI void * STXAPIENTRY stx_malloc(size_t size);

STXEXTERN STXAPI void * STXAPIENTRY stx_calloc(size_t nmemb, size_t size);
STXEXTERN STXAPI void * STXAPIENTRY stx_realloc(void *mem, size_t size);
STXEXTERN STXAPI void STXAPIENTRY stx_free(void *mem);
STXEXTERN STXAPI void STXAPIENTRY stx_closeall();
STXEXTERN STXAPI char * STXAPIENTRY stx_getenv(const char *name);
STXEXTERN STXAPI int STXAPIENTRY stx_putenv(const char *variable);
STXEXTERN STXAPI void STXAPIENTRY stx_qsort(void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));
STXEXTERN STXAPI void * STXAPIENTRY stx_memset(void *dst, int c, size_t len);
STXEXTERN STXAPI void * STXAPIENTRY stx_revcpy(void *dst, const void *src, size_t len);
STXEXTERN STXAPI void * STXAPIENTRY stx_memmove(void * _Dst, const void * _Src, size_t _Size);
STXEXTERN STXAPI int STXAPIENTRY stx_memcmp(const void *s1, const void *s2, size_t len);
STXEXTERN STXAPI float STXAPIENTRY stxAbs(float f);
STXEXTERN STXAPI int STXAPIENTRY stxEq(float fValue1, float fValue2);
STXEXTERN STXAPI void * STXAPIENTRY stx_memcpy(void *d, const void *s, size_t l);
STXEXTERN STXAPI char * STXAPIENTRY stx_strtok(char * _Str, const char * _Delim);
STXEXTERN STXAPI char * STXAPIENTRY stx_getcwd(char * _Path);
STXEXTERN STXAPI unsigned long STXAPIENTRY stx_randnext(void);
STXEXTERN STXAPI int STXAPIENTRY stx_rand(void);
STXEXTERN STXAPI void STXAPIENTRY stx_srand(unsigned int seed);
STXEXTERN STXAPI int STXAPIENTRY stx_isopened(FILE* _File);
STXEXTERN STXAPI int STXAPIENTRY stx_fileExists(const char* fileName);
STXEXTERN STXAPI const char STXAPIENTRY stx_getOsSeparator();
STXEXTERN STXAPI const char* STXAPIENTRY stx_getOsSeparatorString();

#ifndef SWIG
#if defined(LINUX) && !defined(__APPLE__)

#include <assert.h>
 #include <execinfo.h>
     #include <stdio.h>
     #include <stdlib.h>
#include <mcheck.h>

     /* Obtain a backtrace and print it to stdout. */
inline void print_trace(const char *file, int line)
{
    const size_t max_depth = 100;
    size_t stack_depth;
    void *stack_addrs[max_depth];
    char **stack_strings;

    stack_depth = backtrace(stack_addrs, max_depth);
    stack_strings = backtrace_symbols(stack_addrs, stack_depth);

    printf("Call stack from %s:%d:\n", file, line);

    size_t i = 1;
    for (i = 1; i < stack_depth; i++) {
        printf("    %s\n", stack_strings[i]);
    }
    free(stack_strings); // malloc()ed by backtrace_symbols
    //fflush(out);
}
#define STX_STACK_TRACE print_trace(__FILE__,__LINE__);
#define STX_BACKTRACE print_trace(__FILE__,__LINE__);
#else
inline void print_trace(const char *file, int line)
{
    printf("%s:%d:\n", file, line);
}
#define STX_STACK_TRACE
#define STX_BACKTRACE
#endif
#endif

#define STXASSERT(str) {assert(str);}

#ifdef stx_max
#undef stx_max
#endif
#define stx_max(x, y)	(((x) > (y)) ? (x) : (y))
//define max std::max

#ifdef stx_min
#undef stx_min
#endif
#define stx_min(x, y)	(((x) < (y)) ? (x) : (y))
//define min std::min

typedef enum _STX_bool
{
	STX_FALSE=0,
	STX_TRUE =1
} STX_bool;

#include "Console.h"

#if !defined(_MSC_VER) && !defined(__MINGW32__)
DWORD timeGetTime();
#endif

#endif

