// G_luaSerialPort.cpp: 定义应用程序的入口点。
//

#include "lua/src/lua.h"
#include "lua/src/lualib.h"
#include "lua/src/lauxlib.h"
#include "libserialport/libserialport.h"
#include <stdlib.h>

#define G_SP_METATABLE "GSP" //s_port pointer

typedef struct sp_port_fake sp_port_fake;
struct sp_port_fake
{
	struct	sp_port* sp_;
};
static sp_port_fake* check_sp_port(lua_State* L) {
	void* ud = luaL_checkudata(L, 1, G_SP_METATABLE);
	luaL_argcheck(L, ud != NULL, 1, G_SP_METATABLE " expected");
	return (sp_port_fake*)ud;
}

LUA_API int g_sp_new(lua_State* L)
{
	sp_port_fake* sp_fake = lua_newuserdata(L, sizeof(sp_port_fake));
	luaL_getmetatable(L, G_SP_METATABLE);
	lua_setmetatable(L, -2);//sp_fake userdata mark metatable 
	return 1;
}
/// <summary>
/// global "sp_port_ptr" need port_name and baudrate to open serialport
/// </summary>
/// <param name="L">1.sp_port_fake 2. port_name 3.baudrate </param>
/// <returns>erro 1 error id 2 error msg</returns>
LUA_API int g_sp_open(lua_State* L) 
{
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	int top_index=lua_gettop(L);
	
	luaL_argcheck(L, top_index == 3, 1, "param size unequal 3(1.sp_port_fake 2. port_name 3.baudrate)");
	const char * port_name= luaL_checklstring(L, 2,NULL);
	lua_Integer baudrate=   luaL_checkinteger(L, 3);
	enum sp_return sp_rt = sp_get_port_by_name(port_name, &sp_port_fake_->sp_);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	if (sp_rt < SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		lua_pushfstring(L, "get port by name fail:%s \r\n", port_name);
		return 2;
	}
    sp_rt = sp_open(sp_port_, SP_MODE_READ_WRITE);
	if (sp_rt < SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char * error=sp_last_error_message();
		lua_pushfstring(L, "open fail:%s error message:%s\r\n", port_name,error);
		return 2;
	}
	sp_rt=sp_set_baudrate(sp_port_, baudrate);
	if (sp_rt < SP_OK)
	{
		lua_pushinteger(L, sp_rt);
		const char* error = sp_last_error_message();
		lua_pushfstring(L, "set baudrate fail:%s %d error message:%s\r\n", port_name, baudrate, error);
		return 2;
	}
	lua_pushinteger(L, sp_rt);
	return 1;
}

LUA_API int g_sp_close(lua_State* L)
{
	//printf("%s \n", __func__);
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	if (sp_port_fake_->sp_ != NULL)
	{
		sp_close(sp_port_fake_->sp_);
		sp_free_port(sp_port_fake_->sp_);
		sp_port_fake_->sp_ = NULL;
	}
	return 0;
}

LUA_API int g_sp_set_baudrate(lua_State* L)
{
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);
	lua_Integer baudrate = luaL_checkinteger(L, 2);
	enum sp_report	sp_rt = sp_set_baudrate(sp_port_, baudrate);
	if (sp_rt < SP_OK)
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
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);
	lua_Integer parity = luaL_checkinteger(L, 2);
	enum sp_report	sp_rt = sp_set_parity(sp_port_, parity);
	if (sp_rt < SP_OK)
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
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);
	lua_Integer rts = luaL_checkinteger(L, 2);

	enum sp_report	sp_rt = sp_set_rts(sp_port_, rts);
	if (sp_rt < SP_OK)
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
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);
	lua_Integer dtr = luaL_checkinteger(L, 2);

	enum sp_report	sp_rt = sp_set_dtr(sp_port_, dtr);
	if (sp_rt < SP_OK)
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
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);

	luaL_argcheck(L, top_index == 3, 1, "param size unequal 3 (1.sp_port_fake 2.write buffer 3. write timeout)");
	lua_Integer wsize = 0;
	char* wdata = luaL_checklstring(L, 2, &wsize);
	lua_Integer wtimeout = luaL_checkinteger(L, 3);

	enum sp_return sp_rt =sp_blocking_write(sp_port_, wdata, wsize, wtimeout);
	if (sp_rt < SP_OK)
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
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	struct sp_port* sp_port_ = sp_port_fake_->sp_;
	int top_index = lua_gettop(L);

	luaL_argcheck(L, top_index == 3, 1, "param size unequal 2 (1.sp_port_fake 2.read size 3. read timeout)");
	lua_Integer rsize = luaL_checkinteger(L, 2);
	lua_Integer rtimeout = luaL_checkinteger(L, 3);
	top_index = lua_gettop(L);

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
	luaL_buffinit(L, &luabuffer);
	luaL_addlstring(&luabuffer,tempread, sp_rt);
	luaL_pushresult(&luabuffer);
	free(tempread);
	return 1;
}

//
LUA_API int g_sp_get_sp_port(lua_State* L)
{
	//printf("%s \n", __func__);
	struct sp_port_fake* sp_port_fake_ = check_sp_port(L);
	if (sp_port_fake_->sp_ != NULL)
	{
		lua_pushlightuserdata(L, sp_port_fake_->sp_);
		return 1;
	}
	return 0;
}
static luaL_Reg gsp[] = {
	{"__close",g_sp_close},
	{"__gc",g_sp_close},
	{"open",g_sp_open},
	{"write",g_sp_write},
	{"read",g_sp_read},
	{"close",g_sp_close},
	{"set_baudrate",g_sp_set_baudrate},
	{"set_parity",g_sp_set_parity},
	{"set_rts",g_sp_set_rts},
	{"set_dtr",g_sp_set_dtr},
	{"get",g_sp_get_sp_port},
	{NULL,NULL}
};
static luaL_Reg reg[] = {
	{"new",g_sp_new },
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
	luaL_newmetatable(L, G_SP_METATABLE);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");//table  index set vlue and pop top stack
	luaL_setfuncs(L, gsp, 0);
	luaL_newlib(L, reg);
	return 1;
}
