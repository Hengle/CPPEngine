/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#ifndef _LUAUTIL_
#define _LUAUTIL_
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
//include "LuaPlus.h"
//using namespace LuaPlus;
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
namespace    LuaUtil
{
class   ILua
{

public:

    ILua()
    {
    };

    virtual ~ILua()
    {
    };
    virtual lua_State    *State(void) = 0;
    virtual void        DoFile(const char *afile) = 0;
};
class   SLua
{

public:
    static lua_State *state;

public:
    static lua_State *State(void);
    static void     DoFile(const char *afile);
};
class Lua :
    public ILua,
    SLua
{

public:
    Lua();
    ~Lua();

    lua_State *State(void)
    {
        return SLua::State();
    }

    void DoFile(const char *afile)
    {
        SLua::DoFile(afile);
    }
};
}
#endif

