.include "../Makefile.inc"

LUA_PATH=	../../lua

SRCS+=	lutils.c

.PATH:	${LUA_PATH}/src

SRCS+=	lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c lstd.c lauxlib.c lbaselib.c lstrlib.c

CFLAGS+=	-I.
CFLAGS+=	-DBOOT_LUA -ffreestanding -nostdlib -fno-stack-protector


LIB=		lua
INTERNALLIB=
.include <bsd.lib.mk>
