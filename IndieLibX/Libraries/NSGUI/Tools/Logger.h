#ifndef LOGGER_H
#define LOGGER_H

#if 1//def QT_BUILD
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#endif
#ifndef __APPLE__
#include <malloc.h>
#endif

//include <STX/STXCPP.h>
//include <STX/d3dx9mathFromWine.h>
#include <Framework3/IRenderer.h>
#include <STX/CInput.h>

struct Tuple2i;
struct Tuple3i;
struct Tuple4i;

//define __ALL__ 1

//include "MathUtils.h"

#include "NSString.h"

#include "NamedObject.h"

#include "../Parsers/XMLParser.h"

#include "../Parsers/IOXMLObject.h"

//include "../Camera.h"



#define deleteObject(A){ if(A){ delete	 A; A = 0; } }

#define deleteArray(A) { if(A){ delete[] A; A = 0; } }



#define MAX_TEX_UNITS 8


namespace NatureScene
{
class Logger

{

  public:

	  static void push_back(const NSString &info);

    static void writeImmidiateInfoLog(const NSString &info);

    static void writeFatalErrorLog(const NSString &logString);

    static bool writeErrorLog(const NSString &error);

    static void writeInfoLog(const NSString &info);

    static void initialize(const char* logfilename = 0);

    static void flush();



  private:

    static vector<NSString> logStrings;

    static NSString	  logPath;

};



};
#endif

