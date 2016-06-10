/*****************************************************************************************
 * File: Global.h
 * Desc: Global class
 *****************************************************************************************/

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// ----- Includes -----

#include "DebugApi.h"
//#include "basetsd.h"

// ----- Global class -----

extern DebugApi * Debug;

extern int INDIELIBAPI bbb;
extern int INDIELIBAPI mmm;
D3DXFROMWINEMATRIX INDIELIBAPI GetMVP();
class INDIELIBAPI CIndieLib;
extern CIndieLib *	g_pIndieLib;

#endif // _GLOBAL_H_
