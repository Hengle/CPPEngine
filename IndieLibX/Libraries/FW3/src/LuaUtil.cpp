/*
  Copyright (c) 2016 Tommi Roenty   http://www.tommironty.fi/
  Licensed under The GNU Lesser General Public License, version 2.1:
      http://opensource.org/licenses/LGPL-2.1
*/
#include <STX/STX.h>
#include "LuaUtil.h"

lua_State *LuaUtil::SLua::state = 0;

LuaUtil::Lua::Lua() :
    LuaUtil::Lua::ILua()
{
	if(!state)
	{
    state = luaL_newstate();                        /* Create Lua state variable */
    luaL_openlibs(state);                           /* Load Lua libraries */
	}
}

LuaUtil::Lua::~Lua()
{
}

lua_State *LuaUtil::SLua::State(void)
{
    return state;
}

void LuaUtil::SLua::DoFile(const char *afile)
{
    if (luaL_loadfile(state, afile))    /* Load but don't run the Lua script */
	LOG_PRINT("luaL_loadfile(%s) failed\n", afile);  
}
#if 0
#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */

#include <stdlib.h>                             /* For function stx_exit() */
#include <stdio.h>                              /* For input/output */

void bail(lua_State *L, char *msg){
	LOG_PRINT("\nFATAL ERROR:\n  %s: %s\n\n",
		msg, lua_tostring(L, -1));
	stx_exit(1);
}

int main(void)
{
    lua_State *L;

    L = luaL_newstate();                        /* Create Lua state variable */
    luaL_openlibs(L);                           /* Load Lua libraries */

    if (luaL_loadfile(L, "helloscript.lua"))    /* Load but don't run the Lua script */
	bail(L, "luaL_loadfile() failed");      /* Error out if file can't be read */

    LOG_PRINT("In C, calling Lua\n");

    if (lua_pcall(L, 0, 0, 0))                  /* Run the loaded Lua script */
	bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

    LOG_PRINT("Back in C again\n");

    lua_close(L);                               /* Clean up, free the Lua state var */

    return 0;
}
#endif

