// G_luaSerialPort.cpp: 定义应用程序的入口点。
//

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#include "libserialport/libserialport.h"
#include <stdlib.h>

#define G_SP_GLOB "sp_port_ptr" //s_port pointer

/// <summary>
/// global "sp_port_ptr" need port_name and baudrate to open serialport
/// </summary>
/// <param name="L">1. port_name 2.baudrate 3.databits</param>
/// <returns>erro 1 error id 2 error msg</returns>
LUA_API int g_sp_open(lua_State* L) 
{
	int top_index=lua_gettop(L);
	if(top_index<1)
	{ 
		lua_pushinteger(L, SP_ERR_ARG);
		lua_pushstring(L, "param size less 1");
		return 2;
	}
	const char * port_name= luaL_checklstring(L, 1,NULL);
	lua_Integer baudrate=   luaL_checkinteger(L, 2);
	struct sp_port* sp_port_;
	enum sp_return sp_rt = sp_get_port_by_name(port_name, &sp_port_);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		lua_pushfstring(L, "get port by name fail:%s \r\n", port_name);
		return 2;
	}
    sp_rt = sp_open(sp_port_, SP_MODE_READ_WRITE);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char * error=sp_last_error_message();
		lua_pushfstring(L, "open fail:%s error message:%s\r\n", port_name,error);
		return 2;
	}
	sp_rt=sp_set_baudrate(sp_port_, baudrate);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set baudrate fail:%s %d error message:%s\r\n", port_name, baudrate, error);
		return 2;
	}
	lua_pushlightuserdata(L, sp_port_);
	lua_setglobal(L, G_SP_GLOB);
	
	lua_pushinteger(L, sp_rt);
	return 1;
}

LUA_API int g_sp_close(lua_State* L)
{
	lua_getglobal(L, G_SP_GLOB);
	struct sp_port *sp_port_= lua_touserdata(L, 1);
	sp_close(sp_port_);
	sp_free_port(sp_port_);
}

LUA_API int g_sp_set_baudrate(lua_State* L)
{
	int top_index = lua_gettop(L);
	lua_Integer baudrate = luaL_checkinteger(L, 1);

	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index + 1);
	enum sp_report	sp_rt = sp_set_baudrate(sp_port_, baudrate);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set baudrate fail:%s %d error message:%s\r\n",baudrate, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}
/// <summary>
/// 
/// </summary>
/// <param name="L"></param>
/// <returns></returns>
LUA_API int g_sp_set_parity(lua_State* L)
{
	int top_index = lua_gettop(L);
	lua_Integer parity = luaL_checkinteger(L, 1);

	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index + 1);
	enum sp_report	sp_rt = sp_set_parity(sp_port_, parity);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set parity fail:%s %d error message:%s\r\n", parity, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}
LUA_API int g_sp_set_rts(lua_State* L)
{
	int top_index = lua_gettop(L);
	lua_Integer rts = luaL_checkinteger(L, 1);

	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index + 1);
	enum sp_report	sp_rt = sp_set_rts(sp_port_, rts);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set rts fail:%s %d error message:%s\r\n", rts, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}
LUA_API int g_sp_set_dtr(lua_State* L)
{
	int top_index = lua_gettop(L);
	lua_Integer dtr = luaL_checkinteger(L, 1);

	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index + 1);
	enum sp_report	sp_rt = sp_set_dtr(sp_port_, dtr);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set dtr fail:%s %d error message:%s\r\n", dtr, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}

/// <summary>
/// write data with timeout
/// </summary>
/// <param name="L">1.char * 2.size 3. timeout (ms)</param>
/// <returns></returns>
LUA_API int g_sp_write(lua_State* L)
{
	int top_index = lua_gettop(L);
	if (top_index <3)
	{
		lua_pushinteger(L, SP_ERR_ARG);
		lua_pushstring(L, "param size less 1");
		return 2;
	}
	char* wdata = luaL_checklstring(L, 1, NULL);
	lua_Integer wsize = luaL_checkinteger(L, 2);
	lua_Integer wtimeout = luaL_checkinteger(L, 3);
	top_index = lua_gettop(L);
	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index+1);

	enum sp_return sp_rt =sp_blocking_write(sp_port_, wdata, wsize, wtimeout);
	if (sp_rt != SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "write data fail:%p %d %s", sp_port_, sp_rt, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}
/// <summary>
/// read data with timeout
/// </summary>
/// <param name="L">1.size 2. timeout (ms)</param>
/// <returns></returns>
LUA_API int g_sp_read(lua_State* L)
{
	int top_index = lua_gettop(L);
	if (top_index < 2)
	{
		lua_pushinteger(L, SP_ERR_ARG);
		lua_pushstring(L, "param size less 1");
		return 2;
	}
	lua_Integer rsize = luaL_checkinteger(L, 1);
	lua_Integer rtimeout = luaL_checkinteger(L, 2);
	top_index = lua_gettop(L);
	lua_getglobal(L, G_SP_GLOB);
	struct sp_port* sp_port_ = lua_touserdata(L, top_index + 1);
	char* tempread = malloc(rsize);
	enum sp_return sp_rt = sp_blocking_read(sp_port_, tempread, rsize, rtimeout);
	if (sp_rt < SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "read data fail:%p %d %s", sp_port_, sp_rt, error);
		return 2;
	}
	luaL_Buffer luabuffer;
	luaL_buffinitsize(L, &luabuffer, sp_rt);
	luaL_addlstring(&luabuffer,tempread, sp_rt);
	luaL_pushresultsize(&luabuffer, sp_rt);
	free(tempread);
	return 1;
}
static luaL_Reg gsp[] = {
	{"g_sp_open",g_sp_open},
	{"g_sp_write",g_sp_write},
	{"g_sp_read",g_sp_read},
	{"g_sp_close",g_sp_close},
	{"g_sp_set_baudrate",g_sp_set_baudrate},
	{"g_sp_set_parity",g_sp_set_parity},
	{"g_sp_set_rts",g_sp_set_rts},
	{"g_sp_set_dtr",g_sp_set_dtr},
	{NULL,NULL}
};
#ifdef _MSC_VER
/* Microsoft Visual C/C++ compiler in use */
#define GSP_API __declspec(dllexport)
#else
#define GSP_API
#endif
/** @endcond */

GSP_API int luaopen_G_luaSerialPort(lua_State* L)
{
	luaL_newlib(L, gsp);
	return 1;
}
