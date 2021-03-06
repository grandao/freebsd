/*-
* Copyright (c) 2014 Pedro Souza <pedrosouza@freebsd.org>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*/

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>		/* to pick up __FreeBSD_version */
#include <machine/stdarg.h>


#include "bootstrap.h"
#include "interp.h"

#define lua_c

#include "lua.h"
#include "ldebug.h"
#include "lauxlib.h"
#include "lualib.h"
#include "lutils.h"


struct interp_lua_softc {
	lua_State	*luap;
};
struct interp_lua_softc	lua_softc = { 0 };

#define	LDBG(...)	do {			\
	printf("%s(%d): ", __func__, __LINE__);	\
	printf(__VA_ARGS__);			\
	printf("\n");				\
} while (0)


void
interp_lua_init(void *ctx)
{
	lua_State *luap;
	struct bootblk_command **cmdp;
	struct interp_lua_softc	*softc;
	struct env_var *ev;
	const char *name_str, *val_str;
	char buf[16];

	softc = ctx;
	luap = lua_create();
	if (luap == NULL) {
		LDBG("problem with initializing Lua interpreter\n");
	}
	softc->luap = luap;
	register_utils(luap);
	luaopen_base(luap);

	luaL_requiref(luap, LUA_STRLIBNAME, luaopen_string, 1);
	lua_pop(luap, 1);
}

int
interp_lua_run(void *data, const char *line)
{
	lua_State *luap;
	struct interp_lua_softc	*softc;
	int argc, ret;
	char **argv;

	softc = data;
	luap = softc->luap;

	if (ldo_string(luap, line, strlen(line)) != 0)
		printf("[LUA]Failed to execure \'%s\'\n", line);

	return (0);
}

int
interp_lua_incl(void *ctx, const char *filename)
{
	struct interp_lua_softc *softc;

	softc = ctx;

	return ldo_file(softc->luap, filename);
}

/*
* To avoid conflicts lua uses loader.lua instead of 
* loader.rc/boot.conf  to load its configurations.
*/
int
interp_lua_load_config(void *ctx)
{
	return interp_lua_incl(ctx, "/boot/loader.lua");
}


struct interp boot_interp_lua = {
	.init = interp_lua_init,
	.run = interp_lua_run,
	.incl = interp_lua_incl,
	.load_configs = interp_lua_load_config,
	.context = &lua_softc
};
