/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef __INTERFACES__
#define __INTERFACES__
/*
typedef enum _tSWIG
{
	eSWIG,
	eNOTSWIG
} tSWIG;
*/
enum tRenderer
{
	eCGGL_1_1=1,
	eGLSLGL_1_1=2,
	eGLES2=4,
	eD3D9=8,
	eD3D10=16,
	eD3D11=32
};

typedef enum _tAudio
{
	eDefault_Audio,
	eSDL_Audio,
	eFMOD_Audio,
	eOpenAL_Audio,
	eDummy_Audio
} tAudio;
typedef enum _tWindow
{
	eDefault_Window,
	eSDL_Window,
	eDummy_Window
} tWindow;
typedef enum _tInput
{
	eDefault_Input,
	eSDL_Input,
	eDummy_Input
} tInput;

#include <stdexcept>
#include <string>
#include <map>
#include <vector>

#define STX_tolower(X)  (((X) >= 'A') && ((X) <= 'Z') ? ('a'+((X)-'A')) : (X))
#define STX_toupper(X)  (((X) >= 'a') && ((X) <= 'z') ? ('A'+((X)-'a')) : (X))
#ifndef SWIG
#ifndef __APPLE__
#include <malloc.h>
#endif
#if defined( LINUX ) && !defined(__APPLE__)
#include <mcheck.h>
#endif

#if 0
inline void stx_free(void *ptr)
{
#ifdef LINUX
  //__free_hook = old_free_hook;
  switch (mprobe(ptr)) {
  case MCHECK_DISABLED:
    printf("MCHECK_DISABLED\n"); break;
  case MCHECK_OK:
    printf("MCHECK_OK at %p\n",ptr); break;
  case MCHECK_HEAD:
    printf("MCHECK_HEAD at %p\n",ptr); break;
  case MCHECK_TAIL:
    printf("MCHECK_TAIL at %p\n",ptr); break;
  case MCHECK_FREE:
    printf("MCHECK_FREE at %p\n",ptr); break;
  }
#endif
  free(ptr);
  //__free_hook=my_free_hook;
}
inline void* stx_malloc(size_t size)
{
  //__malloc_hook = old_malloc_hook;
  void* ptr=malloc(size);
#ifdef LINUX
  switch (mprobe(ptr)) {
  case MCHECK_DISABLED:
    printf("MCHECK_DISABLED\n"); break;
  case MCHECK_OK:
    printf("MCHECK_OK at %p\n",ptr); break;
  case MCHECK_HEAD:
    printf("MCHECK_HEAD at %p\n",ptr); break;
  case MCHECK_TAIL:
    printf("MCHECK_TAIL at %p\n",ptr); break;
  case MCHECK_FREE:
    printf("MCHECK_FREE at %p\n",ptr); break;
  }
#endif
  //__malloc_hook=my_malloc_hook;
  return ptr;
}

#include "Reference_Counting.h"
#endif
#endif

#if 1//defined(_LINUX)
#define STX_INIT_SEGVCATCH
#define STX_TRY
#define STX_CATCH
#else
#include <segvcatch.h>
#define STX_INIT_SEGVCATCH 	segvcatch::init_segv(); \
				segvcatch::init_fpe();

#define STX_TRY try \
{

#define STX_CATCH } \
	catch (std::runtime_error &e) { \
        std::cout << "Caught a runtime_error exception: " \
                  << e.what () << '\n'; \
	STX_STACK_TRACE; \
    } \
    catch (std::exception &e) { \
        std::cout << "Caught an exception of an unexpected type: " \
                  << e.what () << '\n'; \
	STX_STACK_TRACE; \
    }  \
    catch (...) { \
        std::cout << "Caught an unknown exception\n"; \
	STX_STACK_TRACE; \
    }
#endif
#ifndef ANDROID
#if 0
#define STXString std::string
#else
//template<class T, size_t n>
//class STXArray: public std::array<T,n>
#if 0
template<class T,size_t x>
class STXArray: public
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
{
	public:
		STXArray(const T* al):
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>(x)
#else
std::array<T,x>()
#endif
		{
			unsigned int i=0;
			while(i<x)
			{
				T* p=
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
				::data()+i;
				*(p)=*(al+i);
				i++;
			}
		}
		STXArray():
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>(x)
#else
std::array<T,x>()
#endif
		{
			//???fill<T>(0);
		}
		virtual ~STXArray(){}
#ifdef _DEBUG
		T& operator[](int index)
		{
			if(index>=
			#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
			::size()) STX_STACK_TRACE;
			return
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
			::at(index);
		}

		const T& operator[](int index) const
		{
			if(index>=
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
			::size()) STX_STACK_TRACE;
			return
#if defined( _MSC_VER) || defined(QT_BUILD)
std::vector<T>
#else
std::array<T,x>
#endif
			::at(index);
		}
#endif
};
template<class T,size_t x>
class STX1DArray: public
STXArray<T,x>
{
	public:
		STX1DArray():STXArray<T,x>()
		{
			//???STXArray<T,x>::fill(0);
		}
		virtual ~STX1DArray(){}
#ifdef _DEBUG
		T& operator[](int index)
		{
			if(index>=STXArray<T,x>::size()) STX_STACK_TRACE;
			return STXArray<T,x>::at(index);
		}

		const T& operator[](int index) const
		{
			if(index>=STXArray<T,x>::size()) STX_STACK_TRACE;
			return STXArray<T,x>::at(index);
		}
#endif
};
template<class T,size_t x, size_t y>
class STX2DArray
{
	protected:
		STXArray<STXArray<T,x>,y> m_a;
	public:
		unsigned int GetNX(){return x;}
		unsigned int GetNY(){return y;}
		STX2DArray()
		{
			for(unsigned int j=0;j<y;j++)
			for(unsigned int i=0;i<x;i++)
				*(m_a[j].data()+i)=0;

		}
		virtual ~STX2DArray(){}
		T* data()
		{
			return m_a.data();
		}

		STXArray<T,x>& operator[](int index)
		{

			if(index>=m_a.size()) STX_STACK_TRACE;
			return m_a[index];
		}

		const STXArray<T,x>& operator[](int index) const
		{
			if(index>=m_a.size()) STX_STACK_TRACE;
			return m_a[index];
		}
};
template<class T>
class STXVector : public std::vector<T>
{
	public:
		STXVector(const T al[]):std::vector<T>()
		{
			unsigned int i=0;
			unsigned int x=elementsOf(al);
			//LOG_PRINT("x=%d\n",x);
			STX_STACK_TRACE;
			while(i<x)
			{
				std::vector<T>::push_back(*(al+i));
				i++;
			}
		}
		STXVector(size_t n=0):std::vector<T>(n){}
		virtual ~STXVector(){}
#ifdef _DEBUG
		T& operator[](int index)
		{
			if(index>=std::vector<T>::size()) STX_STACK_TRACE;
			return std::vector<T>::at(index);
		}

		const T& operator[](int index) const
		{
			if(index>=std::vector<T>::size()) STX_STACK_TRACE;
			return std::vector<T>::at(index);
		}
#endif
};
template<class T>
class STXList : public std::list<T>
{
	public:
		STXList():std::list<T>(){}
		virtual ~STXList(){}

		T& operator[](int index)
		{
			if(index>=std::list<T>::size()) STX_STACK_TRACE;
			#if defined(QT_BUILD)
			return std::list<T>::at(index);
			#else
			return at(index);
			#endif
		}

		const T& operator[](int index) const
		{
			if(index>=std::list<T>::size()) STX_STACK_TRACE;
			#if defined(QT_BUILD)
			return std::list<T>::at(index);
			#else
			return at(index);
			#endif
		}

		std::
#if defined(QT_BUILD)
			_List_iterator<T>
#elif defined( _MSC_VER)
			list<T>::iterator
#else
			_List_iterator<T>
#endif
			 iter(int index)
		{
			if(index>=std::list<T>::size()) STX_STACK_TRACE;
			int pos=0;
			std::_List_iterator<T> ait2b=std::list<T>::begin();
			std::_List_iterator<T> ait2e=std::list<T>::end();
			std::_List_iterator<T> ii;
			for(ii=ait2b;ii!=ait2e;ii++)
			{
 				if (!(pos < std::list<T>::size()))
					throw std::out_of_range();
					//return std::list<T>::end();//???
				else if(index==pos)
					break;
				pos++;
			}
			return ii;
		}
		T& at(int index) const
		{
			return *iter(index);
		}

void remove( const T& value ){}

template< class UnaryPredicate >
void remove_if( UnaryPredicate p ){}

};
#endif
#endif
#if !defined(SWIG) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
template<

    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T> >
>
class STXMap : public std::map<Key,T,Compare,Allocator>
{
	public:
		STXMap():std::map<Key,T,Compare,Allocator>(){}
		virtual ~STXMap(){}
};
class STXString : public std::string
{
	public:
		STXString(const int n);
		STXString();
		STXString(const char* str);
		STXString(char* str);

		//operator const char *() const;
		STXString(std::string& str);
		STXString(const STXString& rhs);
		//STXString& operator=(const STXString& rhs);
		STXString& operator=(const int& rhs);

		operator const char*() const {return std::string::c_str();}
		operator char*() const {return const_cast<char*>(std::string::c_str());}

		void tolower();
		void toupper();
		~STXString();

	const STXString& operator+=(const STXString& s);
	const int count(const char c) const;

	const STXString find(const STXString& what) const;
	const STXString after(const STXString& what) const;
	const STXString before(const STXString& what) const;
	void append(const STXString& what){std::string::append(what.c_str());}
	char* str() const;
	void replace(const STXString& afrom, const STXString& ato);

};
#endif
#endif

#if defined(linux) && !defined(__APPLE__) && !defined(WIN_PHONE_APP) && !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
#if defined(SDL_VIDEO_DRIVER_X11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#endif
#endif

struct STXAPI IWindow
{
protected:
	int x,y;
	unsigned int bpp,width, height;
	std::string title;
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
	HWND m_hWnd;
	HDC m_hdc;
public:
	HWND GetHWND(){return m_hWnd;}
	HDC GetHDC(){return m_hdc;}
#elif defined(SDL_VIDEO_DRIVER_X11)
	Display* m_dpy;
	Window m_win;
public:
	Display* GetDisplay(){return m_dpy;}
	Window GetWindow(){return m_win;}
#endif
public:
	void UpdateWindowArgs();
	virtual int Getbpp(){return bpp;}
	virtual int Getwidth(){return width;}
	virtual int Getheight(){return height;}
	virtual int GetWidth(){return width;}
	virtual int GetHeight(){return height;}

	virtual void GetWindowPosition(int* x, int* y, unsigned int* z, unsigned int* w)=0;

	virtual void HotKey_ToggleFullScreen(void){}
	virtual void HotKey_ToggleGrab(void){}
	virtual void HotKey_Iconify(void){}
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
	IWindow():m_hWnd(0),title(""){}
#elif defined(SDL_VIDEO_DRIVER_X11)
	IWindow():m_dpy(0), m_win(0),title(""){}
#endif
	virtual ~IWindow(){}
	virtual void ShowCursor (bool pEnable)=0;
	virtual void SetCaption(const char * buf)=0;
	const char* GetCaption(){return title.c_str();}
	virtual void Init(unsigned int awidth, unsigned int aheight, unsigned int abpp, const char* atitle)=0;
	virtual void ReSize(int awidth, int aheight, int abpp)=0;
	//virtual void Init2(int awidth, int aheight, int abpp)=0;
	//virtual int Quit(int returnCode=0)=0;
};
#if 1//ndef SWIG
struct STXAPI DummyWindow : public IWindow
{
public:
	DummyWindow():IWindow(){}
	virtual ~DummyWindow(){}
	virtual void ShowCursor (bool pEnable){}
	virtual void SetCaption(const char * buf){title=buf;}
	const char* GetCaption(){return title.c_str();}
	virtual void Init(unsigned int awidth, unsigned int aheight, unsigned int abpp, const char* atitle)
	{
		width=awidth;
		height=aheight;
		bpp=abpp;
	}

	virtual void GetWindowPosition(int* x, int* y, unsigned int* z, unsigned int* w)
	{
		*x=0;
		*y=0;
		*w=*x+width;
		*z=*y+height;
	}
	virtual void ReSize(int awidth, int aheight, int abpp)
	{
		width=awidth;
		height=aheight;
		bpp=abpp;
	}
};


typedef unsigned int             FMOD_INITFLAGS;
#define FMOD_INIT_NORMAL                  0x00000000 /* All platforms - Initialize normally */
#endif
class STXAPI IAudio
{
public:
	IAudio(){}
	virtual ~IAudio(){}
public:
	//-------------------------------------------------
	// Public Functions for this Class
	//-------------------------------------------------

	virtual void				Update			( ){}
	virtual bool				Init			(int  maxchannels = 64, FMOD_INITFLAGS  flags = FMOD_INIT_NORMAL){return true;}
virtual bool					Load			(const char* file, const char* szName)=0;
	virtual bool				Play			(const char* szName, int loopCount = 1)=0;
	virtual void				Stop			(const char* szName)=0;
	virtual void				StopAll			( )=0;
	virtual bool				IsPlaying		(const char* szName)=0;
	virtual int					GetNumSounds	( )		=0;
};
#if 1//ndef SWIG

class STXAPI DummyAudio : public IAudio
{
public:
	DummyAudio():IAudio(){}
	virtual ~DummyAudio(){}
public:
	//-------------------------------------------------
	// Public Functions for this Class
	//-------------------------------------------------
	virtual bool				Init			(int  maxchannels = 64, FMOD_INITFLAGS  flags = FMOD_INIT_NORMAL){return true;}
	virtual bool				Load			(const char* szFilename, const char* szName){return true;}
	virtual void				Update			( ){}
	virtual bool				Play			(const char* szName, int loopCount = 1){return false;}
	virtual void				Stop			(const char* szName){}
	virtual void				StopAll			( ){}
	virtual bool				IsPlaying		(const char* szName){return false;}
	virtual int				GetNumSounds		( ){return 0;}
};
#endif
struct STXAPI STX_TIMER
{
	virtual void Reset(){}
	STX_TIMER(){}
	virtual ~STX_TIMER(){}
	virtual Uint32 stx_GetTicks(STX_TIMER* _Timer=0)=0;
	virtual void stx_Delay(Uint32 ms,STX_TIMER* _Timer=0)=0;
};

#if 1//def SWIG
#ifndef __APPLE__
struct STXAPI STX_Dummy_TIMER : public  STX_TIMER
{
protected:
#ifdef _MSC_VER
/* The first (low-resolution) ticks value of the application */
 DWORD
#else
/* The first ticks value of the application */
#ifdef HAVE_CLOCK_GETTIME
struct timespec
#else
struct timeval
#endif /* HAVE_CLOCK_GETTIME */
#endif
		m_dwstart;
public:
	virtual void Reset();
	STX_Dummy_TIMER();
	virtual ~STX_Dummy_TIMER();
	virtual Uint32 stx_GetTicks(STX_TIMER* _Timer=0);
	virtual void stx_Delay(Uint32 ms,STX_TIMER* _Timer=0);
};
#endif

#include <Image.h>

#if defined (QT_BUILD)
#define KEY_MACRO(X) KEY_##X=QtSTX::Key_##X
#else
#define KEY_MACRO(X) KEY_##X=SDLK_##X	
#endif

#define KEYTOCHAR(X) "KEY_##X"

typedef enum _eKey
{
#if !defined(ANDROID) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR) && !defined(WIN_PHONE_APP)
	KEY_UNKNOWN	=SDLK_UNKNOWN,
#if defined (SDL_1_2_15_BUILD)
	KEY_NUMLOCK	=SDLK_NUMLOCK,
	KEY_SCROLLOCK	=SDLK_SCROLLOCK,
	KEY_RSUPER	=SDLK_RSUPER,
	KEY_PRINT	=SDLK_PRINT,
	KEY_BREAK	=SDLK_BREAK,
#endif
	KEY_ESCAPE	=SDLK_ESCAPE,
	KEY_KP_EQUALS	=SDLK_KP_EQUALS,
	KEY_UP		=SDLK_UP,
	KEY_DOWN	=SDLK_DOWN,
	KEY_RIGHT	=SDLK_RIGHT,
	KEY_LEFT	=SDLK_LEFT,
	KEY_INSERT	=SDLK_INSERT,
	KEY_HOME	=SDLK_HOME,
	KEY_END		=SDLK_END,
	KEY_PAGEUP	=SDLK_PAGEUP,
	KEY_PAGEDOWN	=SDLK_PAGEDOWN,

	KEY_KP_PERIOD	=SDLK_KP_PERIOD,
	KEY_KP_DIVIDE	=SDLK_KP_DIVIDE,
	KEY_KP_MULTIPLY	=SDLK_KP_MULTIPLY,
	KEY_KP_MINUS	=SDLK_KP_MINUS,
	KEY_KP_PLUS	=SDLK_KP_PLUS,
	KEY_KP_ENTER	=SDLK_KP_ENTER,

	KEY_DELETE	=SDLK_DELETE,
	KEY_CAPSLOCK	=SDLK_CAPSLOCK,
	KEY_RSHIFT	=SDLK_RSHIFT,
	KEY_LSHIFT	=SDLK_LSHIFT,
	KEY_RCTRL	=SDLK_RCTRL,
	KEY_LCTRL	=SDLK_LCTRL,
	KEY_RALT	=SDLK_RALT,
	KEY_LALT	=SDLK_LALT,
	KEY_HELP	=SDLK_HELP,
	KEY_SYSREQ	=SDLK_SYSREQ,
	KEY_MENU	=SDLK_MENU,

	KEY_BACKSPACE	=SDLK_BACKSPACE,
	KEY_TAB		=SDLK_TAB,
	KEY_CLEAR	=SDLK_CLEAR,
	KEY_RETURN	=SDLK_RETURN,
	KEY_PAUSE	=SDLK_PAUSE,
	KEY_SPACE	=SDLK_SPACE,
	KEY_EXCLAIM	=SDLK_EXCLAIM,
	KEY_QUOTEDBL	=SDLK_QUOTEDBL,
	KEY_HASH	=SDLK_HASH,
	KEY_DOLLAR	=SDLK_DOLLAR,
	KEY_AMPERSAND	=SDLK_AMPERSAND,
	KEY_QUOTE	=SDLK_QUOTE,
	KEY_LEFTPAREN	=SDLK_LEFTPAREN,
	KEY_RIGHTPAREN	=SDLK_RIGHTPAREN,
	KEY_ASTERISK	=SDLK_ASTERISK,
	KEY_PLUS	=SDLK_PLUS,
	KEY_COMMA	=SDLK_COMMA,
	KEY_MINUS	=SDLK_MINUS,
	KEY_PERIOD	=SDLK_PERIOD,
	KEY_SLASH	=SDLK_SLASH,
	KEY_COLON	=SDLK_COLON,
	KEY_SEMICOLON	=SDLK_SEMICOLON,
	KEY_LESS	=SDLK_LESS,
	KEY_EQUALS	=SDLK_EQUALS,
	KEY_GREATER	=SDLK_GREATER,
	KEY_QUESTION	=SDLK_QUESTION,
	KEY_AT		=SDLK_AT,
	KEY_LEFTBRACKET	=SDLK_LEFTBRACKET,
	KEY_BACKSLASH	=SDLK_BACKSLASH,
	KEY_RIGHTBRACKET=SDLK_RIGHTBRACKET,
	KEY_CARET	=SDLK_CARET,
	KEY_UNDERSCORE	=SDLK_UNDERSCORE,
	KEY_BACKQUOTE	=SDLK_BACKQUOTE,

	KEY_F1		=SDLK_F1,
	KEY_F2		=SDLK_F2,
	KEY_F3		=SDLK_F3,
	KEY_F4		=SDLK_F4,
	KEY_F5		=SDLK_F5,
	KEY_F6		=SDLK_F6,
	KEY_F7		=SDLK_F7,
	KEY_F8		=SDLK_F8,
	KEY_F9		=SDLK_F9,
	KEY_F10		=SDLK_F10,
	KEY_F11		=SDLK_F11,
	KEY_F12		=SDLK_F12,
	KEY_F13		=SDLK_F13,
	KEY_F14		=SDLK_F14,
	KEY_F15		=SDLK_F15,
#if defined(SDL_2_0_4_BUILD)
	KEY_KP0		=SDLK_KP_0,
	KEY_KP1		=SDLK_KP_1,
	KEY_KP2		=SDLK_KP_2,
	KEY_KP3		=SDLK_KP_3,
	KEY_KP4		=SDLK_KP_4,
	KEY_KP5		=SDLK_KP_5,
	KEY_KP6		=SDLK_KP_6,
	KEY_KP7		=SDLK_KP_7,
	KEY_KP8		=SDLK_KP_8,
	KEY_KP9		=SDLK_KP_9,
#elif defined(SDL_1_2_15_BUILD)
        KEY_KP0		=SDLK_KP0,
    KEY_KP1		=SDLK_KP1,
    KEY_KP2		=SDLK_KP2,
    KEY_KP3		=SDLK_KP3,
    KEY_KP4		=SDLK_KP4,
    KEY_KP5		=SDLK_KP5,
    KEY_KP6		=SDLK_KP6,
    KEY_KP7		=SDLK_KP7,
    KEY_KP8		=SDLK_KP8,
    KEY_KP9		=SDLK_KP9,
#endif
	KEY_0		=SDLK_0,
	KEY_1		=SDLK_1,
	KEY_2		=SDLK_2,
	KEY_3		=SDLK_3,
	KEY_4		=SDLK_4,
	KEY_5		=SDLK_5,
	KEY_6		=SDLK_6,
	KEY_7		=SDLK_7,
	KEY_8		=SDLK_8,
	KEY_9		=SDLK_9,

	KEY_a		=SDLK_a,
	KEY_b		=SDLK_b,
	KEY_c		=SDLK_c,
	KEY_d		=SDLK_d,
	KEY_e		=SDLK_e,
	KEY_f		=SDLK_f,
	KEY_g		=SDLK_g,
	KEY_h		=SDLK_h,
	KEY_i		=SDLK_i,
	KEY_j		=SDLK_j,
	KEY_k		=SDLK_k,
	KEY_l		=SDLK_l,
	KEY_m		=SDLK_m,
	KEY_n		=SDLK_n,
	KEY_o		=SDLK_o,
	KEY_p		=SDLK_p,
	KEY_q		=SDLK_q,
	KEY_r		=SDLK_r,
	KEY_s		=SDLK_s,
	KEY_t		=SDLK_t,
	KEY_u		=SDLK_u,
	KEY_v		=SDLK_v,
	KEY_w		=SDLK_w,
	KEY_x		=SDLK_x,
	KEY_y		=SDLK_y,
	KEY_z		=SDLK_z,
#else
	KEY_UNKNOWN	,
	KEY_ESCAPE	,//=SDLK_ESCAPE,
	KEY_KP_EQUALS	,//=SDLK_KP_EQUALS,
	KEY_UP		,//=SDLK_UP,
	KEY_DOWN	,//=SDLK_DOWN,
	KEY_RIGHT	,//=SDLK_RIGHT,
	KEY_LEFT	,//=SDLK_LEFT,
	KEY_INSERT	,//=SDLK_INSERT,
	KEY_HOME	,//=SDLK_HOME,
	KEY_END		,//=SDLK_END,
	KEY_PAGEUP	,//=SDLK_PAGEUP,
	KEY_PAGEDOWN	,//=SDLK_PAGEDOWN,

	KEY_KP_PERIOD	,//=SDLK_KP_PERIOD,
	KEY_KP_DIVIDE	,//=SDLK_KP_DIVIDE,
	KEY_KP_MULTIPLY	,//=SDLK_KP_MULTIPLY,
	KEY_KP_MINUS	,//=SDLK_KP_MINUS,
	KEY_KP_PLUS	,//=SDLK_KP_PLUS,
	KEY_KP_ENTER	,//=SDLK_KP_ENTER,

	KEY_DELETE	,//=SDLK_DELETE,
	KEY_NUMLOCK	,//=SDLK_NUMLOCK,
	KEY_CAPSLOCK	,//=SDLK_CAPSLOCK,
	KEY_SCROLLOCK	,//=SDLK_SCROLLOCK,
	KEY_RSHIFT	,//=SDLK_RSHIFT,
	KEY_LSHIFT	,//=SDLK_LSHIFT,
	KEY_RCTRL	,//=SDLK_RCTRL,
	KEY_LCTRL	,//=SDLK_LCTRL,
	KEY_RALT	,//=SDLK_RALT,
	KEY_LALT	,//=SDLK_LALT,
	KEY_RSUPER	,//=SDLK_RSUPER,
	KEY_HELP	,//=SDLK_HELP,
	KEY_PRINT	,//=SDLK_PRINT,
	KEY_SYSREQ	,//=SDLK_SYSREQ,
	KEY_BREAK	,//=SDLK_BREAK,
	KEY_MENU	,//=SDLK_MENU,

	KEY_BACKSPACE	,//=SDLK_BACKSPACE,
	KEY_TAB		,//=SDLK_TAB,
	KEY_CLEAR	,//=SDLK_CLEAR,
	KEY_RETURN	,//=SDLK_RETURN,
	KEY_PAUSE	,//=SDLK_PAUSE,
	KEY_SPACE	,//=SDLK_SPACE,
	KEY_EXCLAIM	,//=SDLK_EXCLAIM,
	KEY_QUOTEDBL	,//=SDLK_QUOTEDBL,
	KEY_HASH	,//=SDLK_HASH,
	KEY_DOLLAR	,//=SDLK_DOLLAR,
	KEY_AMPERSAND	,//=SDLK_AMPERSAND,
	KEY_QUOTE	,//=SDLK_QUOTE,
	KEY_LEFTPAREN	,//=SDLK_LEFTPAREN,
	KEY_RIGHTPAREN	,//=SDLK_RIGHTPAREN,
	KEY_ASTERISK	,//=SDLK_ASTERISK,
	KEY_PLUS	,//=SDLK_PLUS,
	KEY_COMMA	,//=SDLK_COMMA,
	KEY_MINUS	,//=SDLK_MINUS,
	KEY_PERIOD	,//=SDLK_PERIOD,
	KEY_SLASH	,//=SDLK_SLASH,
	KEY_COLON	,//=SDLK_COLON,
	KEY_SEMICOLON	,//=SDLK_SEMICOLON,
	KEY_LESS	,//=SDLK_LESS,
	KEY_EQUALS	,//=SDLK_EQUALS,
	KEY_GREATER	,//=SDLK_GREATER,
	KEY_QUESTION	,//=SDLK_QUESTION,
	KEY_AT		,//=SDLK_AT,
	KEY_LEFTBRACKET	,//=SDLK_LEFTBRACKET,
	KEY_BACKSLASH	,//=SDLK_BACKSLASH,
	KEY_RIGHTBRACKET,//=SDLK_RIGHTBRACKET,
	KEY_CARET	,//=SDLK_CARET,
	KEY_UNDERSCORE	,//=SDLK_UNDERSCORE,
	KEY_BACKQUOTE	,//=SDLK_BACKQUOTE,

	KEY_F1		,//=SDLK_F1,
	KEY_F2		,//=SDLK_F2,
	KEY_F3		,//=SDLK_F3,
	KEY_F4		,//=SDLK_F4,
	KEY_F5		,//=SDLK_F5,
	KEY_F6		,//=SDLK_F6,
	KEY_F7		,//=SDLK_F7,
	KEY_F8		,//=SDLK_F8,
	KEY_F9		,//=SDLK_F9,
	KEY_F10		,//=SDLK_F10,
	KEY_F11		,//=SDLK_F11,
	KEY_F12		,//=SDLK_F12,
	KEY_F13		,//=SDLK_F13,
	KEY_F14		,//=SDLK_F14,
	KEY_F15		,//=SDLK_F15,

	KEY_KP0		,//=SDLK_KP0,
	KEY_KP1		,//=SDLK_KP1,
	KEY_KP2		,//=SDLK_KP2,
	KEY_KP3		,//=SDLK_KP3,
	KEY_KP4		,//=SDLK_KP4,
	KEY_KP5		,//=SDLK_KP5,
	KEY_KP6		,//=SDLK_KP6,
	KEY_KP7		,//=SDLK_KP7,
	KEY_KP8		,//=SDLK_KP8,
	KEY_KP9		,//=SDLK_KP9,

	KEY_0		,//=SDLK_0,
	KEY_1		,//=SDLK_1,
	KEY_2		,//=SDLK_2,
	KEY_3		,//=SDLK_3,
	KEY_4		,//=SDLK_4,
	KEY_5		,//=SDLK_5,
	KEY_6		,//=SDLK_6,
	KEY_7		,//=SDLK_7,
	KEY_8		,//=SDLK_8,
	KEY_9		,//=SDLK_9,

	KEY_a		,//=SDLK_a,
	KEY_b		,//=SDLK_b,
	KEY_c		,//=SDLK_c,
	KEY_d		,//=SDLK_d,
	KEY_e		,//=SDLK_e,
	KEY_f		,//=SDLK_f,
	KEY_g		,//=SDLK_g,
	KEY_h		,//=SDLK_h,
	KEY_i		,//=SDLK_i,
	KEY_j		,//=SDLK_j,
	KEY_k		,//=SDLK_k,
	KEY_l		,//=SDLK_l,
	KEY_m		,//=SDLK_m,
	KEY_n		,//=SDLK_n,
	KEY_o		,//=SDLK_o,
	KEY_p		,//=SDLK_p,
	KEY_q		,//=SDLK_q,
	KEY_r		,//=SDLK_r,
	KEY_s		,//=SDLK_s,
	KEY_t		,//=SDLK_t,
	KEY_u		,//=SDLK_u,
	KEY_v		,//=SDLK_v,
	KEY_w		,//=SDLK_w,
	KEY_x		,//=SDLK_x,
	KEY_y		,//=SDLK_y,
	KEY_z		,//=SDLK_z,
#endif
} eKey;

#if defined(QT_BUILD)
/* Key state modifier keys */
#define KEY_NUMLOCK 	KEY_CONTROL
#define KEY_CAPSLOCK 	KEY_CONTROL
#define KEY_SCROLLOCK 	KEY_CONTROL
#define KEY_RSHIFT	KEY_CONTROL
#define KEY_LSHIFT 	KEY_CONTROL
#define KEY_RCTRL 	KEY_CONTROL
#define KEY_LCTRL 	KEY_CONTROL
#define KEY_RALT 	KEY_CONTROL
#define KEY_LALT 	KEY_CONTROL
#define KEY_RMETA 	KEY_CONTROL
#define KEY_LMETA 	KEY_CONTROL
#define KEY_LSUPER 	KEY_CONTROL
#define KEY_RSUPER 	KEY_CONTROL
#endif
	typedef enum _eKeyButton {
		KEY_BUTTON_PRESSED
	} eKeyButton;

	typedef enum _eJoyButton {
		JOYBUTTONDOWN,
		JOYBUTTONUP
	} eJoyButton;

	typedef enum _eMouseButton {
       MOUSE_MOTION,
       MOUSE_BUTTONDOWN,
       MOUSE_BUTTONUP,
	   MBUTTON_LEFT,
	   MBUTTON_RIGHT,
	   MBUTTON_MIDDLE,
	   MBUTTON_WHEELUP,
	   MBUTTON_WHEELDOWN
	} eMouseButton;

#endif
class IInput
	{
		public:
public:
virtual void SetMouseX(int a){}
virtual void SetMouseY(int a){}
virtual void SetMouseLB(int a){}
virtual void SetMouseMB(int a){}
virtual void SetMouseRB(int a){}
virtual void SetKeyDown(eKey a){}
virtual void SetKeyUp(eKey a){}
     static float m_fSpinX;
     static float m_fSpinY;
     static float m_fX;
     static float m_fY;
     static float m_fZ;
     static float  m_fSpinX_L;
     static float  m_fSpinY_L;
     static float  m_fSpinX_R;
     static float  m_fSpinY_R;
			/// Default Constructor
			IInput(){};

			/// Default Destructor
			virtual ~IInput(){};


			/// Tells if a key is inbetween being pressed and released
			virtual bool IsKeyHeld(eKey Key)=0;

			/// Tells if a key is pressed
			virtual bool IsKeyDown(eKey Key)=0;

			/// Tells if a key is released
			virtual bool IsKeyUp(eKey Key)=0;

			/// Updates keyboard events

			virtual bool Update()=0;






	virtual bool OnKeyPress				(eKey pKey)=0;
	virtual bool OnKeyRelease			(eKey pKey)=0;
	virtual bool IsKeyPressed			(eKey pKey)=0;
	virtual bool IsKeyPressed			(eKey pKey, unsigned long pTime)=0;

	virtual bool IsKeyPressed(void)=0;
	virtual bool IsKeyReleased(void)=0;

	virtual bool IsWindowMoved(void)=0;
	virtual bool IsWindowResized(void)=0;
	virtual bool IsWindowMinimised(void)=0;
	virtual bool IsWindowMaximised(void)=0;
	virtual bool IsWindowClosed(void)=0;

	virtual bool IsMouseClick(void)=0;
	virtual bool IsMouseDoubleClick(void)=0;
	virtual bool IsMouseButtonPressed(void)=0;
	virtual bool IsMouseButtonReleased(void)=0;
	virtual bool IsMouseButton			()=0;
	virtual bool IsMouseMotion			()=0;
	virtual int GetMouseX				()=0;
	virtual int GetMouseY				()=0;
	virtual bool OnMouseButtonPress		(eMouseButton pMouseButton)=0;
	virtual bool OnMouseButtonRelease	(eMouseButton pMouseButton)=0;
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton)=0;
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton, unsigned long pTime)=0;

	virtual float& GetSpinX();
	virtual float& GetSpinY();
	virtual float& GetX();
	virtual float& GetY();
	virtual float& GetZ();
	virtual float& GetSpinX_L();
	virtual float& GetSpinY_L();
	virtual float& GetSpinX_R();
	virtual float& GetSpinY_R();

	virtual int GetJoyX()=0;
	virtual int GetJoyY()=0;
	virtual bool IsJoyAxisMoving()=0;
	virtual bool IsJoyBallMoving()=0;
	virtual bool IsJoyHatMoving()=0;
	virtual bool IsJoyButton			()=0;

	virtual Uint8 GetAxisJoystickDeviceIndex()=0;	/**< The joystick device index */
	virtual Uint8 GetAxisIndex()=0;	/**< The joystick axis index */
	virtual Sint16 GetAxisValue()=0;	/**< The axis value (range: -32768 to 32767) */

	virtual Uint8 GetBallJoystickDeviceIndex()=0;	/**< The joystick device index */
	virtual Uint8 GetBallIndex()=0;	/**< The joystick trackball index */
	virtual Sint16 GetBallXRel()=0;	/**< The relative motion in the X direction */
	virtual Sint16 GetBallYRel()=0;	/**< The relative motion in the Y direction */

	virtual Uint8 GetHatJoystickDeviceIndex()=0;	/**< The joystick device index */
	virtual Uint8 GetHatIndex()=0;	/**< The joystick hat index */
	virtual Uint8 GetHatValue()=0;	/**< The hat position value:*/

	virtual Uint8 GetButtonJoystickDeviceIndex()=0;	/**< The joystick device index */
	virtual Uint8 GetButtonIndex()=0;	/**< The joystick button index */
	virtual Uint8 GetButtonState()=0;	/**< SDL_PRESSED or SDL_RELEASED */
	virtual bool Quit ()=0;
	virtual int getKeyID()=0;
	virtual char getKeyChar()=0;
	virtual Sint32 getKeyCode()=0;
	};
#if 1//def SWIG

class DummyInput : public IInput
	{
		private:
			std::map<int, char> m_Keystates;	/**< Holds the state of the keys */
		protected:
int MouseX;
int MouseY;
int MouseLB;
int MouseMB;
int MouseRB;
		public:
/*
case SDL_KEYUP:
					m_Keystates[SDLKtoKEY(event.key.keysym.sym)] = 'u';
case SDL_KEYDOWN:
					m_Keystates[SDLKtoKEY(event.key.keysym.sym)] = 'd';

 void TextEdit::keyPressEvent(QKeyEvent *e)
 {
     if (c && c->popup()->isVisible()) {
         // The following keys are forwarded by the completer to the widget
        switch (e->key()) {

*/
virtual void SetKeyDown(eKey a){m_Keystates[a]='d';}
virtual void SetKeyUp(eKey a){m_Keystates[a]='u';}
virtual void SetMouseX(int a){MouseX=a;}
virtual void SetMouseY(int a){MouseY=a;}
virtual void SetMouseLB(int a){MouseLB=a;}
virtual void SetMouseMB(int a){MouseMB=a;}
virtual void SetMouseRB(int a){MouseRB=a;}
			/// Default Constructor
                        DummyInput():IInput()
{
MouseX=0;
MouseY=0;
MouseLB=0;
MouseMB=0;
MouseRB=0;
}

			/// Default Destructor
                        virtual ~DummyInput(){}

			/// Updates keyboard events
			virtual bool Update()
{
/*
for( std::map<int, char>::iterator itr = m_Keystates.begin(); itr != m_Keystates.end(); itr++)
		{
			//put no status flag
			if( itr->second == 'u' )
			{
			}
		}
*/
		//check to see if any keys haven't been released but were press
		// ie being held
/*
		for( std::map<int, char>::iterator itr = m_Keystates.begin(); itr != m_Keystates.end(); itr++)
		{
			//put no status flag
			if( itr->second == 'u' )
			{
				bool keyFound = false;
				for(int i = 0; i < keys.size(); i++)
				{
					if(keys[i] == itr->first)
					{
						keyFound = true;
						break;
					}
				}

				if( !keyFound )
				{
					itr->second = 'n';
				}
			}
			else if( itr->second == 'd')
			{
				bool keyFound = false;
				for(int i = 0; i < keys.size(); i++)
				{
					if(keys[i] == itr->first)
					{
						keyFound = true;
						break;
					}
				}

				if( !keyFound )
				{
					itr->second = 'h';
				}
			}
		}
*/

return true;
}

#if 1
virtual bool OnKeyPress	(eKey pKey)
{
if(m_Keystates[pKey] == 'd')
		{
			return true;
		}
	return false;
}

virtual bool OnKeyRelease	(eKey pKey)
{
if(m_Keystates[pKey] == 'u')
		{
			return true;
		}
	return false;
}
	//
	// Tells if a key is inbetween being pressed and released
	//
	virtual bool IsKeyHeld(eKey Key)
	{
		if(m_Keystates[Key] == 'h')
		{
			return true;
		}

		return false;
	}

	//
	// Tells if a key is pressed
	virtual bool IsKeyDown(eKey Key)
	{
		if(m_Keystates[Key] == 'd')
		{
			return true;
		}

		return false;
	}

	//
	// Tells if a key is released
	//
	virtual bool IsKeyUp(eKey Key)
	{
		if(m_Keystates[Key] == 'u')
		{
			return true;
		}

		return false;
	}
	virtual bool IsKeyPressed(eKey a){return IsKeyDown(a);}
	virtual bool IsKeyPressed(eKey a, long unsigned int){return IsKeyDown(a);}
#else			/// Tells if a key is inbetween being pressed and released
			virtual bool IsKeyHeld(eKey Key){return false;}

			/// Tells if a key is pressed
			virtual bool IsKeyDown(eKey Key){return false;}

			/// Tells if a key is released
			virtual bool IsKeyUp(eKey Key){return false;}

	virtual bool OnKeyPress				(eKey pKey){return false;}
	virtual bool OnKeyRelease			(eKey pKey){return false;}
	virtual bool IsKeyPressed			(eKey pKey){return false;}
	virtual bool IsKeyPressed			(eKey pKey, unsigned long pTime){return false;}
#endif
	virtual bool IsKeyPressed(void){return false;}
	virtual bool IsKeyReleased(void){return false;}

	virtual bool IsWindowMoved(void){return false;}
	virtual bool IsWindowResized(void){return false;}
	virtual bool IsWindowMinimised(void){return false;}
	virtual bool IsWindowMaximised(void){return false;}
	virtual bool IsWindowClosed(void){return false;}

	virtual bool IsMouseClick(void){return false;}
	virtual bool IsMouseDoubleClick(void){return false;}
	virtual bool IsMouseButtonPressed(void){return false;}
	virtual bool IsMouseButtonReleased(void){return false;}
	virtual bool IsMouseButton			(){return MouseLB||MouseMB||MouseRB;}
	virtual bool IsMouseMotion			(){return false;}
	virtual int GetMouseX				(){return MouseX;}
	virtual int GetMouseY				(){return MouseY;}
	virtual bool OnMouseButtonPress		(eMouseButton pMouseButton)
{
if((pMouseButton==MBUTTON_LEFT)&&(MouseLB)) return true;
if((pMouseButton==MBUTTON_RIGHT)&&(MouseRB)) return true;
if((pMouseButton==MBUTTON_MIDDLE)&&(MouseMB)) return true;
return false;
}
	virtual bool OnMouseButtonRelease	(eMouseButton pMouseButton){
//if((pMouseButton==MBUTTON_LEFT)&&(!MouseLB)) return true;//???
//if((pMouseButton==MBUTTON_RIGHT)&&(!MouseRB)) return true;
//if((pMouseButton==MBUTTON_MIDDLE)&&(!MouseMB)) return true;
return false;
}
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton){
if((pMouseButton==MBUTTON_LEFT)&&(MouseLB)) return true;
if((pMouseButton==MBUTTON_RIGHT)&&(MouseRB)) return true;
if((pMouseButton==MBUTTON_MIDDLE)&&(MouseMB)) return true;
return false;
}
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton, unsigned long pTime){
if((pMouseButton==MBUTTON_LEFT)&&(MouseLB)) return true;
if((pMouseButton==MBUTTON_RIGHT)&&(MouseRB)) return true;
if((pMouseButton==MBUTTON_MIDDLE)&&(MouseMB)) return true;
return false;
}

	virtual int GetJoyX(){return 0;}
	virtual int GetJoyY(){return 0;}
	virtual bool IsJoyAxisMoving(){return false;}
	virtual bool IsJoyBallMoving(){return false;}
	virtual bool IsJoyHatMoving(){return false;}
	virtual bool IsJoyButton			(){return false;}

	virtual Uint8 GetAxisJoystickDeviceIndex(){return 0;}	/**< The joystick device index */
	virtual Uint8 GetAxisIndex(){return 0;}	/**< The joystick axis index */
	virtual Sint16 GetAxisValue(){return 0;}	/**< The axis value (range: -32768 to 32767) */

	virtual Uint8 GetBallJoystickDeviceIndex(){return 0;}	/**< The joystick device index */
	virtual Uint8 GetBallIndex(){return 0;}	/**< The joystick trackball index */
	virtual Sint16 GetBallXRel(){return 0;}	/**< The relative motion in the X direction */
	virtual Sint16 GetBallYRel(){return 0;}	/**< The relative motion in the Y direction */

	virtual Uint8 GetHatJoystickDeviceIndex(){return 0;}	/**< The joystick device index */
	virtual Uint8 GetHatIndex(){return 0;}	/**< The joystick hat index */
	virtual Uint8 GetHatValue(){return 0;}	/**< The hat position value:*/

	virtual Uint8 GetButtonJoystickDeviceIndex(){return 0;}	/**< The joystick device index */
	virtual Uint8 GetButtonIndex(){return 0;}	/**< The joystick button index */
	virtual Uint8 GetButtonState(){return 0;}	/**< SDL_PRESSED or SDL_RELEASED */
	virtual bool Quit (){return false;}
	virtual int getKeyID(){return 0;}
	virtual char getKeyChar(){return '\0';}
	virtual Sint32 getKeyCode(){return 0;}
    };
#if defined(SDL_1_2_15_BUILD) || defined(SDL_2_0_4_BUILD)

	class SDLInput : public IInput
	{
		public:
			/// Default Constructor
			SDLInput(){m_quit=false;}

			/// Default Destructor
			virtual ~SDLInput(){}

			/// Tells if a key is inbetween being pressed and released
			virtual bool IsKeyHeld(eKey Key);

			/// Tells if a key is pressed
			virtual bool IsKeyDown(eKey Key);

			/// Tells if a key is released
			virtual bool IsKeyUp(eKey Key);

			/// Updates keyboard events
			virtual bool Update();

	virtual bool OnKeyPress				(eKey pKey);
	virtual bool OnKeyRelease			(eKey pKey);
	virtual bool IsKeyPressed			(eKey pKey);
	virtual bool IsKeyPressed			(eKey pKey, unsigned long pTime);

	virtual bool IsKeyPressed(void);
	virtual bool IsKeyReleased(void);

	virtual bool IsWindowMoved(void);
	virtual bool IsWindowResized(void);
	virtual bool IsWindowMinimised(void);
	virtual bool IsWindowMaximised(void);
	virtual bool IsWindowClosed(void);

	virtual bool IsMouseClick(void);
	virtual bool IsMouseDoubleClick(void);

	virtual bool IsMouseButtonPressed(void);
	virtual bool IsMouseButtonReleased(void);
	virtual bool IsMouseButton			();
	virtual bool IsMouseMotion			();
	virtual int GetMouseX				();
	virtual int GetMouseY				();
	virtual bool MouseButtonPressed		(eMouseButton pMouseButton);
	virtual bool OnMouseButtonPress		(eMouseButton pMouseButton);
	virtual bool OnMouseButtonRelease	(eMouseButton pMouseButton);
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton);
	virtual bool IsMouseButtonPressed	(eMouseButton pMouseButton, unsigned long pTime);


	virtual int GetJoyX();
	virtual int GetJoyY();
	virtual bool IsJoyAxisMoving();
	virtual bool IsJoyBallMoving();
	virtual bool IsJoyHatMoving();
	virtual bool IsJoyButton			();

	virtual Uint8 GetAxisJoystickDeviceIndex();	/**< The joystick device index */
	virtual Uint8 GetAxisIndex();	/**< The joystick axis index */
	virtual Sint16 GetAxisValue();	/**< The axis value (range: -32768 to 32767) */

	virtual Uint8 GetBallJoystickDeviceIndex();	/**< The joystick device index */
	virtual Uint8 GetBallIndex();	/**< The joystick trackball index */
	virtual Sint16 GetBallXRel();	/**< The relative motion in the X direction */
	virtual Sint16 GetBallYRel();	/**< The relative motion in the Y direction */

	virtual Uint8 GetHatJoystickDeviceIndex();	/**< The joystick device index */
	virtual Uint8 GetHatIndex();	/**< The joystick hat index */
	virtual Uint8 GetHatValue();	/**< The hat position value:*/

	virtual Uint8 GetButtonJoystickDeviceIndex();	/**< The joystick device index */
	virtual Uint8 GetButtonIndex();	/**< The joystick button index */
	virtual Uint8 GetButtonState();	/**< SDL_PRESSED or SDL_RELEASED */
	virtual bool Quit ();
	virtual int getKeyID();
	virtual char getKeyChar();
	virtual Sint32 getKeyCode();

		private:

			std::map<int, char> m_Keystates;	/**< Holds the state of the keys */
			int m_MouseX;
			int m_MouseY;
			std::vector< char > m_MouseButtons;
			bool m_quit;


	};
#endif

struct STX_Service
{
#if 0
static bool m_ReplayOn;
static unsigned int m_ReplayIndex;
static bool IsReplayOn(){return m_ReplayOn;}
static bool ReplayOn(){return m_ReplayOn=true;ResetReplayIndex();}
static bool ReplayOff(){return m_ReplayOn=false;ResetReplayIndex();}
static unsigned int GetReplayIndex(){return m_ReplayIndex;}
static void ResetReplayIndex(){m_ReplayIndex=0;}
static unsigned int IncrementReplayIndex(){return m_ReplayIndex++;}
#endif
#if 0
virtual int renderReplay()
{
list::pop_front
list::push_back
list::pop_back
struct ReplayFrame
{
	IND_Camera2d Camera2d;
	IND_Camera3d Camera3d;
	D3DFROMWINEVIEWPORT9 ViewPort2d;
	D3DFROMWINEVIEWPORT9 ViewPort3d;

	std::vector <IND_Entity2d> mListEntities2d  [NUM_LAYERS];
	std::vector <IND_Entity3d> mListEntities3d;
};
std::list<ReplayFrame> frames;
	ReplayFrame frame;
	frame.Camera2d=CIndieLib::Instance()->Render->GetCamera2d;
	frame.Camera3d=CIndieLib::Instance()->Render->GetCamera3d;
	frame.ViewPort2d=CIndieLib::Instance()->Render->GetViewPort2d;
	frame.ViewPort3d=CIndieLib::Instance()->Render->GetViewPort3d;
	for(unsigned int i=0;i<NUM_LAYERS;i++)
	frame.mListEntities2d  [i]=CIndieLib::Instance()->Entity2dManager->GetEntityVector(i);
	frame.mListEntities3d  =CIndieLib::Instance()->Entity3dManager->GetEntityVector();
		if(frames.size()) frames.pop_front();
		frames.push_back(frame);

		// ----- Render  -----
	vector <IND_Entity2d*> *mListEntities2d  [NUM_LAYERS];
	vector <IND_Entity3d*> *mListEntities3d;

		mI->Render->BeginScene ();

		mI->Render->SetViewPort2d (frames[i].ViewPort2d.X, frames[i].ViewPort2d.Y, frames[i].ViewPort2d.Width, frames[i].ViewPort2d.Height);
		mI->Render->SetCamera2d (&frames[i].Camera2d);
		mI->Entity2dManager->RenderEntities2d ();

		mI->Render->SetViewPort3d (frames[i].ViewPort3d.X, frames[i].ViewPort3d.Y, frames[i].ViewPort3d.Width, frames[i].ViewPort3d.Height);
		mI->Render->SetCamera3d (&frames[i].Camera3d);
		mI->Entity3dManager->RenderEntities3d ();

		mI->Render->EndScene ();


}
#endif

static void CloseWindow();
static void CloseInput();
static void CloseAudio();
static void CloseTimer();

static void CloseAll();
#if !defined(SWIG)
static IWindow* s_pWindow;
static IInput* s_pInput;
static IAudio * s_pAudio;
static STX_TIMER* s_pTimer;
#endif
public:
static Uint32 GetDelay();
static void PrintDelay(const char* file="", const char* function="", int line=0);
#if 0
static IWindow* GetWindowInstance(tWindow a=eDummy_Window);
#elif defined(SDL_1_2_15_BUILD) || defined(SDL_2_0_4_BUILD)
static IWindow* GetWindowInstance(tWindow a=eSDL_Window);
#else
static IWindow* GetWindowInstance(tWindow a=eDummy_Window);
#endif

#if 0
static IInput* GetInputInstance(tInput a=eDummy_Input);
#elif defined(SDL_1_2_15_BUILD) || defined(SDL_2_0_4_BUILD)
static IInput* GetInputInstance(tInput a=eSDL_Input);
#else
static IInput* GetInputInstance(tInput a=eDummy_Input);
#endif

#if defined(SDL_1_2_15_BUILD) || defined(SDL_2_0_4_BUILD)
static IAudio * GetAudioInstance(tAudio a=eSDL_Audio);
#else
static IAudio * GetAudioInstance(tAudio a=eOpenAL_Audio);
#endif


static STX_TIMER* GetTimerInstance();
//static STX_STDINC* GetSTDINCInstance();
};

#if defined(SDL_1_2_15_BUILD) || defined(SDL_2_0_4_BUILD)
struct STXAPI STX_SDL_TIMER : public STX_TIMER
{
	STX_SDL_TIMER():STX_TIMER(){}
	virtual ~STX_SDL_TIMER(){}
	virtual Uint32 stx_GetTicks(STX_TIMER* _Timer);
	virtual void stx_Delay(Uint32 ms,STX_TIMER* _Timer);
};
#endif
#endif
#if defined(SDL_2_0_4_BUILD) && !defined(SWIG)
extern SDL_Window* g_SDL_window;
extern SDL_GLContext g_SDL_context;
#endif
#endif

