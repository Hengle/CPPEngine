/*
 * Copyright (C) 2007 David Adam
 * Copyright (C) 2007 Tony Wasserka
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */
/*
  Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
//include <Framework3/Platform.h>

#ifndef __D3DXFROMWINE9MATH_H__
#define __D3DXFROMWINE9MATH_H__

#include <STX/STX.h>

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

#if defined(_WIN32) && !defined(__MINGW32__)
#if 0
	#include <mmsystem.h>
// ShellExecute()
#include <shellapi.h>
#include <commctrl.h>
// GetOpenFileName()
#include <Commdlg.h>
#elif !defined(WIN8_APP) && !defined(WIN_PHONE_APP) 
#include <shellapi.h>
#include <Winbase.h>//include <windows.h>
#include <Wingdi.h>//include <windows.h>
#include <Winuser.h>
#include <Commctrl.h>
#include <Commdlg.h>//include <windows.h>
#include <Winbase.h>//include <windows.h>
#include <Mmsystem.h>//include <windows.h>
//???include <Shlobj.h>
#endif

#else
#ifndef BASETYPES
#define BASETYPES
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;

#define MAX_PATH          260

#if 0
#ifdef __cplusplus
#define 0    0
#else
#define 0    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#undef far
#undef near
#undef pascal

#define far
#define near
#if (!defined(_MAC)) && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
#define pascal __stdcall
#else
#define pascal
#endif

#if defined(DOSWIN32) || defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL
#endif
#endif

#ifdef _MAC
#define CALLBACK    PASCAL
#define WINAPI      CDECL
#define WINAPIV     CDECL
#define APIENTRY    WINAPI
#define APIPRIVATE  CDECL
#ifdef _68K_
#define PASCAL      __pascal
#else
#define PASCAL
#endif
#elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#else
#define CALLBACK
#define WINAPI
#define WINAPIV
#undef APIENTRY
#define APIENTRY    WINAPI
#define APIPRIVATE
#define PASCAL      pascal
#endif

#ifdef _M_CEE_PURE
#define WINAPI_INLINE  __clrcall
#else
#define WINAPI_INLINE  WINAPI
#endif

#undef FAR
#undef  NEAR
#define FAR                 far
#define NEAR                near
#ifndef CONST
#define CONST               const
#endif

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

//???typedef void* HANDLE;
#ifndef _WIN32
typedef long                 LONG;
typedef struct  tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;

//???typedef HANDLE HWND;
typedef char CHAR;
typedef char TCHAR;
#endif
typedef unsigned char byte;
typedef signed int          INT32;
typedef unsigned int        UINT32;

//
// ANSI (Multi-byte Character) types
//
typedef CHAR* PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;

#ifdef _WIN32
typedef __nullterminated CHAR *NPSTR, *LPSTR, *PSTR;
typedef __nullterminated PSTR *PZPSTR;
typedef __nullterminated CONST PSTR *PCZPSTR;
//typedef __nullterminated CONST CHAR *LPCSTR, *PCSTR;
//typedef __nullterminated PCSTR *PZPCSTR;
#else
typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef PSTR *PZPSTR;
typedef CONST PSTR *PCZPSTR;
//typedef CONST CHAR *LPCSTR, *PCSTR;
//typedef PCSTR *PZPCSTR;
#endif

typedef void    VOID;

#ifndef NOMINMAX

#ifndef stx_max
#define stx_max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef stx_min
#define stx_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */
#endif
#if !defined(HRESULT) && !defined(__MINGW32__)
#define HRESULT long
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define S_OK            ((HRESULT)0L)
#define S_FALSE   	((HRESULT)1L)
#define E_FAIL   	((HRESULT)1L)
#define E_INVALIDARG	(0x80070057L)
#endif

#endif  /* !BASETYPES */


#if defined(QT_BUILD) && !defined(__APPLE__)
#include <qmath.h>
#endif
#include <math.h>

#ifdef _WIN32
#include <float.h>  // for _isnan() on VC++
#define isnan(x) _isnan(x)  // VC++ uses _isnan() instead of isnan()
//#else
//#include <math.h>  // for isnan() everywhere else
#endif

#define D3DXFROMWINE_DEFAULT -1

#define D3DXFROMWINE_PI    ((FLOAT)3.141592654)
#define D3DXFROMWINE_1BYPI ((FLOAT)0.318309886)

#define D3DXFROMWINEToRadian(degree) ((degree) * (D3DXFROMWINE_PI / 180.0f))
#define D3DXFROMWINEToDegree(radian) ((radian) * (180.0f / D3DXFROMWINE_PI))

typedef struct _D3DFROMWINEVIEWPORT9 {
	DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
} D3DFROMWINEVIEWPORT9;

typedef struct  D3DXFROMWINEVECTOR2
{
#ifdef __cplusplus
	D3DXFROMWINEVECTOR2& set(FLOAT fx, FLOAT fy)
	{
    		x = fx;
    		y = fy;
		return *this;
	}

    D3DXFROMWINEVECTOR2();
    D3DXFROMWINEVECTOR2(CONST FLOAT *pf);
    D3DXFROMWINEVECTOR2(FLOAT fx, FLOAT fy);
#if !defined(SWIG)
    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEVECTOR2& operator += (CONST D3DXFROMWINEVECTOR2&);
    D3DXFROMWINEVECTOR2& operator -= (CONST D3DXFROMWINEVECTOR2&);
    D3DXFROMWINEVECTOR2& operator *= (FLOAT);
    D3DXFROMWINEVECTOR2& operator /= (FLOAT);

    D3DXFROMWINEVECTOR2 operator + () const;
    D3DXFROMWINEVECTOR2 operator - () const;

    D3DXFROMWINEVECTOR2 operator + (CONST D3DXFROMWINEVECTOR2&) const;
    D3DXFROMWINEVECTOR2 operator - (CONST D3DXFROMWINEVECTOR2&) const;
    D3DXFROMWINEVECTOR2 operator * (FLOAT) const;
    D3DXFROMWINEVECTOR2 operator / (FLOAT) const;
#if !defined(SWIG)
    friend D3DXFROMWINEVECTOR2 operator * (FLOAT, CONST D3DXFROMWINEVECTOR2&);
#endif

    BOOL operator == (CONST D3DXFROMWINEVECTOR2&) const;
    BOOL operator != (CONST D3DXFROMWINEVECTOR2&) const;
#endif /* __cplusplus */
    FLOAT x, y;
} D3DXFROMWINEVECTOR2, *LPD3DXFROMWINEVECTOR2;

#ifndef D3DFROMWINEVECTOR_DEFINED
typedef struct  _D3DFROMWINEVECTOR {
    float x;
    float y;
    float z;
} D3DFROMWINEVECTOR;
#define D3DFROMWINEVECTOR_DEFINED
#endif


#ifdef __cplusplus
typedef struct  D3DXFROMWINEVECTOR3 : public D3DFROMWINEVECTOR
{
	D3DXFROMWINEVECTOR3& set(FLOAT fx, FLOAT fy, FLOAT fz)
	{
    		x = fx;
    		y = fy;
    		z = fz;
		return *this;
	}

	D3DXFROMWINEVECTOR3& normalize()
{
	CONST D3DXFROMWINEVECTOR3 pv1=*this;
	CONST D3DXFROMWINEVECTOR3 pv2=*this;
   	FLOAT n=sqrtf((pv1.x) * (pv2.x) + (pv1.y) * (pv2.y) + (pv1.z) * (pv2.z));
    		x /= n;
    		y /= n;
    		z /= n;
	return *this;
}

D3DXFROMWINEVECTOR3& crossProduct(CONST D3DXFROMWINEVECTOR3& pv1, CONST D3DXFROMWINEVECTOR3& pv2)
{
	D3DXFROMWINEVECTOR3 pout=*this;
    pout.x = (pv1.y) * (pv2.z) - (pv1.z) * (pv2.y);
    pout.y = (pv1.z) * (pv2.x) - (pv1.x) * (pv2.z);
    pout.z = (pv1.x) * (pv2.y) - (pv1.y) * (pv2.x);
    return *this;
}
	FLOAT getDotProduct(CONST D3DXFROMWINEVECTOR3& a)
{
	CONST D3DXFROMWINEVECTOR3 pv1=a;
	CONST D3DXFROMWINEVECTOR3 pv2=a;
   	return FLOAT(sqrtf((pv1.x) * (pv2.x) + (pv1.y) * (pv2.y) + (pv1.z) * (pv2.z)));
}

    D3DXFROMWINEVECTOR3();
    D3DXFROMWINEVECTOR3(CONST FLOAT *pf);
    D3DXFROMWINEVECTOR3(CONST D3DFROMWINEVECTOR& v);
    D3DXFROMWINEVECTOR3(FLOAT fx, FLOAT fy, FLOAT fz);
#if !defined(SWIG)

    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEVECTOR3& operator += (CONST D3DXFROMWINEVECTOR3&);
    D3DXFROMWINEVECTOR3& operator -= (CONST D3DXFROMWINEVECTOR3&);
    D3DXFROMWINEVECTOR3& operator *= (FLOAT);
    D3DXFROMWINEVECTOR3& operator /= (FLOAT);

    D3DXFROMWINEVECTOR3 operator + () const;
    D3DXFROMWINEVECTOR3 operator - () const;

    D3DXFROMWINEVECTOR3 operator + (CONST D3DXFROMWINEVECTOR3&) const;
    D3DXFROMWINEVECTOR3 operator - (CONST D3DXFROMWINEVECTOR3&) const;
    D3DXFROMWINEVECTOR3 operator * (FLOAT) const;
    D3DXFROMWINEVECTOR3 operator / (FLOAT) const;
#if !defined(SWIG)

    friend D3DXFROMWINEVECTOR3 operator * (FLOAT, CONST struct  D3DXFROMWINEVECTOR3&);
#endif

    BOOL operator == (CONST D3DXFROMWINEVECTOR3&) const;
    BOOL operator != (CONST D3DXFROMWINEVECTOR3&) const;
} D3DXFROMWINEVECTOR3, *LPD3DXFROMWINEVECTOR3;
#else /* !__cplusplus */
typedef struct  _D3DVECTOR D3DXFROMWINEVECTOR3, *LPD3DXFROMWINEVECTOR3;
#endif /* !__cplusplus */

typedef struct  D3DXFROMWINEVECTOR4
{
#ifdef __cplusplus

	D3DXFROMWINEVECTOR4& set(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw)
	{
    		x = fx;
    		y = fy;
    		z = fz;
    		w = fw;
		return *this;
	}

	D3DXFROMWINEVECTOR4& normalize()
{
	CONST D3DXFROMWINEVECTOR4 *pv1=this;
	CONST D3DXFROMWINEVECTOR4 *pv2=this;
   	FLOAT n=(pv1->x) * (pv2->x) + (pv1->y) * (pv2->y) + (pv1->z) * (pv2->z)
		 + (pv1->w) * (pv2->w);
    		x /= n;
    		y /= n;
    		z /= n;
    		w /= n;
	return *this;
}

    D3DXFROMWINEVECTOR4();
    D3DXFROMWINEVECTOR4(CONST FLOAT *pf);
    D3DXFROMWINEVECTOR4(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw);
#if !defined(SWIG)

    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEVECTOR4& operator += (CONST D3DXFROMWINEVECTOR4&);
    D3DXFROMWINEVECTOR4& operator -= (CONST D3DXFROMWINEVECTOR4&);
    D3DXFROMWINEVECTOR4& operator *= (FLOAT);
    D3DXFROMWINEVECTOR4& operator /= (FLOAT);

    D3DXFROMWINEVECTOR4 operator + () const;
    D3DXFROMWINEVECTOR4 operator - () const;

    D3DXFROMWINEVECTOR4 operator + (CONST D3DXFROMWINEVECTOR4&) const;
    D3DXFROMWINEVECTOR4 operator - (CONST D3DXFROMWINEVECTOR4&) const;
    D3DXFROMWINEVECTOR4 operator * (FLOAT) const;
    D3DXFROMWINEVECTOR4 operator / (FLOAT) const;
#if !defined(SWIG)

    friend D3DXFROMWINEVECTOR4 operator * (FLOAT, CONST D3DXFROMWINEVECTOR4&);
#endif

    BOOL operator == (CONST D3DXFROMWINEVECTOR4&) const;
    BOOL operator != (CONST D3DXFROMWINEVECTOR4&) const;
#endif /* __cplusplus */
    FLOAT x, y, z, w;
} D3DXFROMWINEVECTOR4, *LPD3DXFROMWINEVECTOR4;

#ifdef __cplusplus


typedef struct  D3DFROMWINEMATRIX {
    /*union {
        struct  {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        } DUMMYSTRUCTNAME;*/
        float m[4][4];
   // } DUMMYUNIONNAME;
} D3DFROMWINEMATRIX;


typedef struct  D3DXFROMWINEMATRIX : public D3DFROMWINEMATRIX
{
    D3DXFROMWINEMATRIX();
    D3DXFROMWINEMATRIX(CONST FLOAT *pf);
    D3DXFROMWINEMATRIX(CONST D3DFROMWINEMATRIX& mat);
    D3DXFROMWINEMATRIX(FLOAT f11, FLOAT f12, FLOAT f13, FLOAT f14,
               FLOAT f21, FLOAT f22, FLOAT f23, FLOAT f24,
               FLOAT f31, FLOAT f32, FLOAT f33, FLOAT f34,
               FLOAT f41, FLOAT f42, FLOAT f43, FLOAT f44);

    FLOAT& operator () (UINT row, UINT col);
#if !defined(SWIG)
    FLOAT operator () (UINT row, UINT col) const;

    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEMATRIX& operator *= (CONST D3DXFROMWINEMATRIX&);
    D3DXFROMWINEMATRIX& operator += (CONST D3DXFROMWINEMATRIX&);
    D3DXFROMWINEMATRIX& operator -= (CONST D3DXFROMWINEMATRIX&);
    D3DXFROMWINEMATRIX& operator *= (FLOAT);
    D3DXFROMWINEMATRIX& operator /= (FLOAT);

    D3DXFROMWINEMATRIX operator + () const;
    D3DXFROMWINEMATRIX operator - () const;

    D3DXFROMWINEMATRIX operator * (CONST D3DXFROMWINEMATRIX&) const;
    D3DXFROMWINEMATRIX operator + (CONST D3DXFROMWINEMATRIX&) const;
    D3DXFROMWINEMATRIX operator - (CONST D3DXFROMWINEMATRIX&) const;
    D3DXFROMWINEMATRIX operator * (FLOAT) const;
    D3DXFROMWINEMATRIX operator / (FLOAT) const;
#if !defined(SWIG)

    friend D3DXFROMWINEMATRIX operator * (FLOAT, CONST D3DXFROMWINEMATRIX&);
#endif

    BOOL operator == (CONST D3DXFROMWINEMATRIX&) const;
    BOOL operator != (CONST D3DXFROMWINEMATRIX&) const;
	float& index(const unsigned int i, const unsigned int j) const {return *const_cast<float*>(&m[i][j]);}
} D3DXFROMWINEMATRIX, *LPD3DXFROMWINEMATRIX;
#else /* !__cplusplus */
typedef struct  _D3DMATRIX D3DXFROMWINEMATRIX, *LPD3DXFROMWINEMATRIX;
#endif /* !__cplusplus */

#ifndef SWIG
typedef struct  D3DXFROMWINEQUATERNION
{
#ifdef __cplusplus
    D3DXFROMWINEQUATERNION();
    D3DXFROMWINEQUATERNION(CONST FLOAT *pf);
    D3DXFROMWINEQUATERNION(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw);
#if !defined(SWIG)

    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEQUATERNION& operator += (CONST D3DXFROMWINEQUATERNION&);
    D3DXFROMWINEQUATERNION& operator -= (CONST D3DXFROMWINEQUATERNION&);
    D3DXFROMWINEQUATERNION& operator *= (CONST D3DXFROMWINEQUATERNION&);
    D3DXFROMWINEQUATERNION& operator *= (FLOAT);
    D3DXFROMWINEQUATERNION& operator /= (FLOAT);

    D3DXFROMWINEQUATERNION  operator + () const;
    D3DXFROMWINEQUATERNION  operator - () const;

    D3DXFROMWINEQUATERNION operator + (CONST D3DXFROMWINEQUATERNION&) const;
    D3DXFROMWINEQUATERNION operator - (CONST D3DXFROMWINEQUATERNION&) const;
    D3DXFROMWINEQUATERNION operator * (CONST D3DXFROMWINEQUATERNION&) const;
    D3DXFROMWINEQUATERNION operator * (FLOAT) const;
    D3DXFROMWINEQUATERNION operator / (FLOAT) const;
#if !defined(SWIG)

    friend D3DXFROMWINEQUATERNION operator * (FLOAT, CONST D3DXFROMWINEQUATERNION&);
#endif

    BOOL operator == (CONST D3DXFROMWINEQUATERNION&) const;
    BOOL operator != (CONST D3DXFROMWINEQUATERNION&) const;
#endif /* __cplusplus */
    FLOAT x, y, z, w;
} D3DXFROMWINEQUATERNION, *LPD3DXFROMWINEQUATERNION;
#endif

typedef struct  D3DXFROMWINEPLANE
{
#ifdef __cplusplus
    D3DXFROMWINEPLANE();
    D3DXFROMWINEPLANE(CONST FLOAT *pf);
    D3DXFROMWINEPLANE(FLOAT fa, FLOAT fb, FLOAT fc, FLOAT fd);
#if !defined(SWIG)

    operator FLOAT* ();
    operator CONST FLOAT* () const;
#endif

    D3DXFROMWINEPLANE operator + () const;
    D3DXFROMWINEPLANE operator - () const;

    BOOL operator == (CONST D3DXFROMWINEPLANE&) const;
    BOOL operator != (CONST D3DXFROMWINEPLANE&) const;
#endif /* __cplusplus */
    FLOAT a, b, c, d;
} D3DXFROMWINEPLANE, *LPD3DXFROMWINEPLANE;

#ifndef D3DFROMWINECOLORVALUE_DEFINED
typedef struct  _D3DFROMWINECOLORVALUE {
    float r;
    float g;
    float b;
    float a;
} D3DFROMWINECOLORVALUE;
#define D3DFROMWINECOLORVALUE_DEFINED
#endif

typedef struct  D3DXFROMWINECOLOR
{
#ifdef __cplusplus
    D3DXFROMWINECOLOR();
    D3DXFROMWINECOLOR(DWORD col);
    D3DXFROMWINECOLOR(CONST FLOAT *pf);
    D3DXFROMWINECOLOR(CONST D3DFROMWINECOLORVALUE& col);
    D3DXFROMWINECOLOR(FLOAT fr, FLOAT fg, FLOAT fb, FLOAT fa);
#if !defined(SWIG)

    operator DWORD () const;

    operator FLOAT* ();
    operator CONST FLOAT* () const;

    operator D3DFROMWINECOLORVALUE* ();
    operator CONST D3DFROMWINECOLORVALUE* () const;

    operator D3DFROMWINECOLORVALUE& ();
    operator CONST D3DFROMWINECOLORVALUE& () const;
#endif

    D3DXFROMWINECOLOR& operator += (CONST D3DXFROMWINECOLOR&);
    D3DXFROMWINECOLOR& operator -= (CONST D3DXFROMWINECOLOR&);
    D3DXFROMWINECOLOR& operator *= (FLOAT);
    D3DXFROMWINECOLOR& operator /= (FLOAT);

    D3DXFROMWINECOLOR operator + () const;
    D3DXFROMWINECOLOR operator - () const;

    D3DXFROMWINECOLOR operator + (CONST D3DXFROMWINECOLOR&) const;
    D3DXFROMWINECOLOR operator - (CONST D3DXFROMWINECOLOR&) const;
    D3DXFROMWINECOLOR operator * (FLOAT) const;
    D3DXFROMWINECOLOR operator / (FLOAT) const;
#if !defined(SWIG)

    friend D3DXFROMWINECOLOR operator * (FLOAT, CONST D3DXFROMWINECOLOR&);
#endif

    BOOL operator == (CONST D3DXFROMWINECOLOR&) const;
    BOOL operator != (CONST D3DXFROMWINECOLOR&) const;
#endif /* __cplusplus */
    FLOAT r, g, b, a;
} D3DXFROMWINECOLOR, *LPD3DXFROMWINECOLOR;

#ifdef __cplusplus
extern "C" {
#endif

 D3DXFROMWINECOLOR* WINAPI D3DXFROMWINEColorAdjustContrast(D3DXFROMWINECOLOR *pout, CONST D3DXFROMWINECOLOR *pc, FLOAT s);
 D3DXFROMWINECOLOR* WINAPI D3DXFROMWINEColorAdjustSaturation(D3DXFROMWINECOLOR *pout, CONST D3DXFROMWINECOLOR *pc, FLOAT s);

 FLOAT WINAPI D3DXFROMWINEFresnelTerm(FLOAT costheta, FLOAT refractionindex);

#ifndef SWIG
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixAffineTransformation(D3DXFROMWINEMATRIX *pout, FLOAT scaling, CONST D3DXFROMWINEVECTOR3 *rotationcenter, CONST D3DXFROMWINEQUATERNION *rotation, CONST D3DXFROMWINEVECTOR3 *translation);
#endif
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixAffineTransformation2D(D3DXFROMWINEMATRIX *pout, FLOAT scaling, CONST D3DXFROMWINEVECTOR2 *protationcenter, FLOAT rotation, CONST D3DXFROMWINEVECTOR2 *ptranslation);
#ifndef SWIG
 HRESULT WINAPI D3DXFROMWINEMatrixDecompose(D3DXFROMWINEVECTOR3 *poutscale, D3DXFROMWINEQUATERNION *poutrotation, D3DXFROMWINEVECTOR3 *pouttranslation, CONST D3DXFROMWINEMATRIX *pm);
#endif
 FLOAT WINAPI D3DXFROMWINEMatrixDeterminant(CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixInverse(D3DXFROMWINEMATRIX *pout, FLOAT *pdeterminant, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixLookAtLH(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR3 *peye, CONST D3DXFROMWINEVECTOR3 *pat, CONST D3DXFROMWINEVECTOR3 *pup);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixLookAtRH(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR3 *peye, CONST D3DXFROMWINEVECTOR3 *pat, CONST D3DXFROMWINEVECTOR3 *pup);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixMultiply(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEMATRIX *pm1, CONST D3DXFROMWINEMATRIX *pm2);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixMultiplyTranspose(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEMATRIX *pm1, CONST D3DXFROMWINEMATRIX *pm2);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixOrthoLH(D3DXFROMWINEMATRIX *pout, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixOrthoOffCenterLH(D3DXFROMWINEMATRIX *pout, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixOrthoOffCenterRH(D3DXFROMWINEMATRIX *pout, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixOrthoLH(D3DXFROMWINEMATRIX *pout, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixOrthoRH(D3DXFROMWINEMATRIX *pout, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveFovLH(D3DXFROMWINEMATRIX *pout, FLOAT fovy, FLOAT aspect, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveFovRH(D3DXFROMWINEMATRIX *pout, FLOAT fovy, FLOAT aspect, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveLH(D3DXFROMWINEMATRIX *pout, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveOffCenterLH(D3DXFROMWINEMATRIX *pout, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveOffCenterRH(D3DXFROMWINEMATRIX *pout, FLOAT l, FLOAT r, FLOAT b, FLOAT t, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixPerspectiveRH(D3DXFROMWINEMATRIX *pout, FLOAT w, FLOAT h, FLOAT zn, FLOAT zf);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixReflect(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEPLANE *pplane);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationAxis(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR3 *pv, FLOAT angle);
#ifndef SWIG
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationQuaternion(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEQUATERNION *pq);
#endif
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationX(D3DXFROMWINEMATRIX *pout, FLOAT angle);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationY(D3DXFROMWINEMATRIX *pout, FLOAT angle);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationYawPitchRoll(D3DXFROMWINEMATRIX *pout, FLOAT yaw, FLOAT pitch, FLOAT roll);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixRotationZ(D3DXFROMWINEMATRIX *pout, FLOAT angle);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixScaling(D3DXFROMWINEMATRIX *pout, FLOAT sx, FLOAT sy, FLOAT sz);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixShadow(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR4 *plight, CONST D3DXFROMWINEPLANE *pPlane);
#ifndef SWIG
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixTransformation(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR3 *pscalingcenter, CONST D3DXFROMWINEQUATERNION *pscalingrotation, CONST D3DXFROMWINEVECTOR3 *pscaling, CONST D3DXFROMWINEVECTOR3 *protationcenter, CONST D3DXFROMWINEQUATERNION *protation, CONST D3DXFROMWINEVECTOR3 *ptranslation);
#endif
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixTransformation2D(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEVECTOR2 *pscalingcenter, FLOAT scalingrotation, CONST D3DXFROMWINEVECTOR2 *pscaling, CONST D3DXFROMWINEVECTOR2 *protationcenter, FLOAT rotation, CONST D3DXFROMWINEVECTOR2 *ptranslation);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixTranslation(D3DXFROMWINEMATRIX *pout, FLOAT x, FLOAT y, FLOAT z);
 D3DXFROMWINEMATRIX* WINAPI D3DXFROMWINEMatrixTranspose(D3DXFROMWINEMATRIX *pout, CONST D3DXFROMWINEMATRIX *pm);

 D3DXFROMWINEPLANE* WINAPI D3DXFROMWINEPlaneFromPointNormal(D3DXFROMWINEPLANE *pout, CONST D3DXFROMWINEVECTOR3 *pvpoint, CONST D3DXFROMWINEVECTOR3 *pvnormal);
 D3DXFROMWINEPLANE* WINAPI D3DXFROMWINEPlaneFromPoints(D3DXFROMWINEPLANE *pout, CONST D3DXFROMWINEVECTOR3 *pv1, CONST D3DXFROMWINEVECTOR3 *pv2, CONST D3DXFROMWINEVECTOR3 *pv3);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEPlaneIntersectLine(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEPLANE *pp, CONST D3DXFROMWINEVECTOR3 *pv1, CONST D3DXFROMWINEVECTOR3 *pv2);
 D3DXFROMWINEPLANE* WINAPI D3DXFROMWINEPlaneNormalize(D3DXFROMWINEPLANE *pout, CONST D3DXFROMWINEPLANE *pp);
 D3DXFROMWINEPLANE* WINAPI D3DXFROMWINEPlaneTransform(D3DXFROMWINEPLANE *pout, CONST D3DXFROMWINEPLANE *pplane, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEPLANE* WINAPI D3DXFROMWINEPlaneTransformArray(D3DXFROMWINEPLANE *pout, UINT outstride, CONST D3DXFROMWINEPLANE *pplane, UINT pstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
#ifndef SWIG

 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionBaryCentric(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq1, CONST D3DXFROMWINEQUATERNION *pq2, CONST D3DXFROMWINEQUATERNION *pq3, FLOAT f, FLOAT g);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionExp(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionInverse(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionLn(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionMultiply(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq1, CONST D3DXFROMWINEQUATERNION *pq2);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionNormalize(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionRotationAxis(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEVECTOR3 *pv, FLOAT angle);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionRotationMatrix(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionRotationYawPitchRoll(D3DXFROMWINEQUATERNION *pout, FLOAT yaw, FLOAT pitch, FLOAT roll);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionSlerp(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq1, CONST D3DXFROMWINEQUATERNION *pq2, FLOAT t);
 D3DXFROMWINEQUATERNION* WINAPI D3DXFROMWINEQuaternionSquad(D3DXFROMWINEQUATERNION *pout, CONST D3DXFROMWINEQUATERNION *pq1, CONST D3DXFROMWINEQUATERNION *pq2, CONST D3DXFROMWINEQUATERNION *pq3, CONST D3DXFROMWINEQUATERNION *pq4, FLOAT t);
void WINAPI D3DXFROMWINEQuaternionToAxisAngle(CONST D3DXFROMWINEQUATERNION *pq, D3DXFROMWINEVECTOR3 *paxis, FLOAT *pangle);
#endif
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2BaryCentric(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv1, CONST D3DXFROMWINEVECTOR2 *pv2, CONST D3DXFROMWINEVECTOR2 *pv3, FLOAT f, FLOAT g);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2CatmullRom(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv0, CONST D3DXFROMWINEVECTOR2 *pv1, CONST D3DXFROMWINEVECTOR2 *pv2, CONST D3DXFROMWINEVECTOR2 *pv3, FLOAT s);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2Hermite(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv1, CONST D3DXFROMWINEVECTOR2 *pt1, CONST D3DXFROMWINEVECTOR2 *pv2, CONST D3DXFROMWINEVECTOR2 *pt2, FLOAT s);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2Normalize(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec2Transform(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR2 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec2TransformArray(D3DXFROMWINEVECTOR4 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR2 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2TransformCoord(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2TransformCoordArray(D3DXFROMWINEVECTOR2 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR2 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2TransformNormal(D3DXFROMWINEVECTOR2 *pout, CONST D3DXFROMWINEVECTOR2 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR2* WINAPI D3DXFROMWINEVec2TransformNormalArray(D3DXFROMWINEVECTOR2 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR2 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);



 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3BaryCentric(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv1, CONST D3DXFROMWINEVECTOR3 *pv2, CONST D3DXFROMWINEVECTOR3 *pv3, FLOAT f, FLOAT g);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3CatmullRom( D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv0, CONST D3DXFROMWINEVECTOR3 *pv1, CONST D3DXFROMWINEVECTOR3 *pv2, CONST D3DXFROMWINEVECTOR3 *pv3, FLOAT s);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3Hermite(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv1, CONST D3DXFROMWINEVECTOR3 *pt1, CONST D3DXFROMWINEVECTOR3 *pv2, CONST D3DXFROMWINEVECTOR3 *pt2, FLOAT s);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3Normalize(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3Project(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv, CONST D3DFROMWINEVIEWPORT9 *pviewport, CONST D3DXFROMWINEMATRIX *pprojection, CONST D3DXFROMWINEMATRIX *pview, CONST D3DXFROMWINEMATRIX *pworld);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3ProjectArray(D3DXFROMWINEVECTOR3 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR3 *pv, UINT vstride, CONST D3DFROMWINEVIEWPORT9 *pviewport, CONST D3DXFROMWINEMATRIX *pprojection, CONST D3DXFROMWINEMATRIX *pview, CONST D3DXFROMWINEMATRIX *pworld, UINT n);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec3Transform(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR3 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec3TransformArray(D3DXFROMWINEVECTOR4 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR3 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3TransformCoord(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3TransformCoordArray(D3DXFROMWINEVECTOR3 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR3 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3TransformNormal(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3TransformNormalArray(D3DXFROMWINEVECTOR3 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR3 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3Unproject(D3DXFROMWINEVECTOR3 *pout, CONST D3DXFROMWINEVECTOR3 *pv, CONST D3DFROMWINEVIEWPORT9 *pviewport, CONST D3DXFROMWINEMATRIX *pprojection, CONST D3DXFROMWINEMATRIX *pview, CONST D3DXFROMWINEMATRIX *pworld);
 D3DXFROMWINEVECTOR3* WINAPI D3DXFROMWINEVec3UnprojectArray(D3DXFROMWINEVECTOR3 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR3 *pv, UINT vstride, CONST D3DFROMWINEVIEWPORT9 *pviewport, CONST D3DXFROMWINEMATRIX *pprojection, CONST D3DXFROMWINEMATRIX *pview, CONST D3DXFROMWINEMATRIX *pworld, UINT n);

 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4BaryCentric(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv1, CONST D3DXFROMWINEVECTOR4 *pv2, CONST D3DXFROMWINEVECTOR4 *pv3, FLOAT f, FLOAT g);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4CatmullRom(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv0, CONST D3DXFROMWINEVECTOR4 *pv1, CONST D3DXFROMWINEVECTOR4 *pv2, CONST D3DXFROMWINEVECTOR4 *pv3, FLOAT s);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4Cross(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv1, CONST D3DXFROMWINEVECTOR4 *pv2, CONST D3DXFROMWINEVECTOR4 *pv3);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4Hermite(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv1, CONST D3DXFROMWINEVECTOR4 *pt1, CONST D3DXFROMWINEVECTOR4 *pv2, CONST D3DXFROMWINEVECTOR4 *pt2, FLOAT s);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4Normalize(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4Transform(D3DXFROMWINEVECTOR4 *pout, CONST D3DXFROMWINEVECTOR4 *pv, CONST D3DXFROMWINEMATRIX *pm);
 D3DXFROMWINEVECTOR4* WINAPI D3DXFROMWINEVec4TransformArray(D3DXFROMWINEVECTOR4 *pout, UINT outstride, CONST D3DXFROMWINEVECTOR4 *pv, UINT vstride, CONST D3DXFROMWINEMATRIX *pm, UINT n);

#ifdef __cplusplus
}
#endif

#if 0
typedef interface ID3DXFROMWINEMatrixStack *LPD3DXFROMWINEMATRIXSTACK;

DEFINE_GUID(IID_ID3DXFROMWINEMatrixStack,
0xc7885ba7, 0xf990, 0x4fe7, 0x92, 0x2d, 0x85, 0x15, 0xe4, 0x77, 0xdd, 0x85);

#undef INTERFACE
#define INTERFACE ID3DXFROMWINEMatrixStack

DECLARE_INTERFACE_(ID3DXFROMWINEMatrixStack, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;
    STDMETHOD(Pop)(THIS) PURE;
    STDMETHOD(Push)(THIS) PURE;
    STDMETHOD(LoadIdentity)(THIS) PURE;
    STDMETHOD(LoadMatrix)(THIS_ CONST D3DXFROMWINEMATRIX* pM ) PURE;
    STDMETHOD(MultMatrix)(THIS_ CONST D3DXFROMWINEMATRIX* pM ) PURE;
    STDMETHOD(MultMatrixLocal)(THIS_ CONST D3DXFROMWINEMATRIX* pM ) PURE;
    STDMETHOD(RotateAxis)(THIS_ CONST D3DXFROMWINEVECTOR3* pV, FLOAT Angle) PURE;
    STDMETHOD(RotateAxisLocal)(THIS_ CONST D3DXFROMWINEVECTOR3* pV, FLOAT Angle) PURE;
    STDMETHOD(RotateYawPitchRoll)(THIS_ FLOAT Yaw, FLOAT Pitch, FLOAT Roll) PURE;
    STDMETHOD(RotateYawPitchRollLocal)(THIS_ FLOAT Yaw, FLOAT Pitch, FLOAT Roll) PURE;
    STDMETHOD(Scale)(THIS_ FLOAT x, FLOAT y, FLOAT z) PURE;
    STDMETHOD(ScaleLocal)(THIS_ FLOAT x, FLOAT y, FLOAT z) PURE;
    STDMETHOD(Translate)(THIS_ FLOAT x, FLOAT y, FLOAT z ) PURE;
    STDMETHOD(TranslateLocal)(THIS_ FLOAT x, FLOAT y, FLOAT z) PURE;
    STDMETHOD_(D3DXFROMWINEMATRIX*, GetTop)(THIS) PURE;
};

#undef INTERFACE

#if !defined(__cplusplus) || defined(CINTERFACE)

#define ID3DXFROMWINEMatrixStack_QueryInterface(p,a,b)            (p)->lpVtbl->QueryInterface(p,a,b)
#define ID3DXFROMWINEMatrixStack_AddRef(p)                        (p)->lpVtbl->AddRef(p)
#define ID3DXFROMWINEMatrixStack_Release(p)                       (p)->lpVtbl->Release(p)
#define ID3DXFROMWINEMatrixStack_Pop(p)                           (p)->lpVtbl->Pop(p)
#define ID3DXFROMWINEMatrixStack_Push(p)                          (p)->lpVtbl->Push(p)
#define ID3DXFROMWINEMatrixStack_LoadIdentity(p)                  (p)->lpVtbl->LoadIdentity(p)
#define ID3DXFROMWINEMatrixStack_LoadMatrix(p,a)                  (p)->lpVtbl->LoadMatrix(p,a)
#define ID3DXFROMWINEMatrixStack_MultMatrix(p,a)                  (p)->lpVtbl->MultMatrix(p,a)
#define ID3DXFROMWINEMatrixStack_MultMatrixLocal(p,a)             (p)->lpVtbl->MultMatrixLocal(p,a)
#define ID3DXFROMWINEMatrixStack_RotateAxis(p,a,b)                (p)->lpVtbl->RotateAxis(p,a,b)
#define ID3DXFROMWINEMatrixStack_RotateAxisLocal(p,a,b)           (p)->lpVtbl->RotateAxisLocal(p,a,b)
#define ID3DXFROMWINEMatrixStack_RotateYawPitchRoll(p,a,b,c)      (p)->lpVtbl->RotateYawPitchRoll(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_RotateYawPitchRollLocal(p,a,b,c) (p)->lpVtbl->RotateYawPitchRollLocal(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_Scale(p,a,b,c)                   (p)->lpVtbl->Scale(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_ScaleLocal(p,a,b,c)              (p)->lpVtbl->ScaleLocal(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_Translate(p,a,b,c)               (p)->lpVtbl->Translate(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_TranslateLocal(p,a,b,c)          (p)->lpVtbl->TranslateLocal(p,a,b,c)
#define ID3DXFROMWINEMatrixStack_GetTop(p)                        (p)->lpVtbl->GetTop(p)

#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif

//HRESULT WINAPI D3DXFROMWINECreateMatrixStack(DWORD flags, LPD3DXFROMWINEMATRIXSTACK* ppstack);

#ifdef __cplusplus
}
#endif
#ifndef D3DFROMWINECOLORVALUE_DEFINED
typedef struct _D3DFROMWINECOLORVALUE {
    float r;
    float g;
    float b;
    float a;
} D3DFROMWINECOLORVALUE;
#define D3DFROMWINECOLORVALUE_DEFINED
#endif

typedef struct _D3DFROMWINEMATERIAL9 {
    D3DFROMWINECOLORVALUE   Diffuse;        /* Diffuse color RGBA */
    D3DFROMWINECOLORVALUE   Ambient;        /* Ambient color RGB */
    D3DFROMWINECOLORVALUE   Specular;       /* Specular 'shininess' */
    D3DFROMWINECOLORVALUE   Emissive;       /* Emissive color RGB */
    float           Power;          /* Sharpness if specular highlight */
} D3DFROMWINEMATERIAL9;

typedef enum _D3DFROMWINELIGHTTYPE {
    D3DFROMWINELIGHT_POINT          = 1,
    D3DFROMWINELIGHT_SPOT           = 2,
    D3DFROMWINELIGHT_DIRECTIONAL    = 3,
    D3DFROMWINELIGHT_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
} D3DFROMWINELIGHTTYPE;/*
typedef struct _D3DFROMWINEVECTOR {
    float x;
    float y;
    float z;
} D3DFROMWINEVECTOR;*/
typedef struct _D3DFROMWINELIGHT9 {
    D3DFROMWINELIGHTTYPE    Type;            /* Type of light source */
    D3DFROMWINECOLORVALUE   Diffuse;         /* Diffuse color of light */
    D3DFROMWINECOLORVALUE   Specular;        /* Specular color of light */
    D3DFROMWINECOLORVALUE   Ambient;         /* Ambient color of light */
    D3DFROMWINEVECTOR       Position;         /* Position in world space */
    D3DFROMWINEVECTOR       Direction;        /* Direction in world space */
    float           Range;            /* Cutoff range */
    float           Falloff;          /* Falloff */
    float           Attenuation0;     /* Constant attenuation */
    float           Attenuation1;     /* Linear attenuation */
    float           Attenuation2;     /* Quadratic attenuation */
    float           Theta;            /* Inner angle of spotlight cone */
    float           Phi;              /* Outer angle of spotlight cone */
} D3DFROMWINELIGHT9;
#ifndef SWIG
#include "d3dx9mathFromWine.inl"
#endif

typedef D3DXFROMWINEVECTOR2 vec2;
typedef D3DXFROMWINEVECTOR3 vec3;
typedef D3DXFROMWINEVECTOR4 vec4;
typedef D3DXFROMWINEVECTOR2 float2;
typedef D3DXFROMWINEVECTOR3 float3;
typedef D3DXFROMWINEVECTOR4 float4;
typedef D3DXFROMWINEMATRIX  mat4;

#endif /* __D3DXFROMWINE9MATH_H__ */

