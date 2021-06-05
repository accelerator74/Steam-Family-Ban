# Makefile
HX_SOURCEMOD = ../sourcemod
#
# familyban.ext.so
#
HX_INCLUDE = -I. \
	-Isdk/public/steam \
	-I$(HX_SOURCEMOD)/public \
	-I$(HX_SOURCEMOD)/public/amtl \
	-I$(HX_SOURCEMOD)/public/amtl/amtl \
	-I$(HX_SOURCEMOD)/sourcepawn/include
#
HX_QWERTY = -D_LINUX \
	-Dstricmp=strcasecmp \
	-D_stricmp=strcasecmp \
	-D_strnicmp=strncasecmp \
	-Dstrnicmp=strncasecmp \
	-D_snprintf=snprintf \
	-D_vsnprintf=vsnprintf \
	-D_alloca=alloca \
	-Dstrcmpi=strcasecmp \
	-Wall \
	-Werror \
	-Wno-switch \
	-Wno-unused \
	-msse \
	-DSOURCEMOD_BUILD \
	-DHAVE_STDINT_H \
	-m32 \
	-DNDEBUG \
	-O3 \
	-funroll-loops \
	-pipe \
	-fno-strict-aliasing \
	-fvisibility=hidden \
	-mfpmath=sse

CPP_FLAGS = -Wno-non-virtual-dtor \
	-Wno-delete-non-virtual-dtor \
	-fvisibility-inlines-hidden \
	-fno-exceptions \
	-fno-rtti
#
HX_SO = Release/smsdk_ext.o \
	Release/extension.o
#
all:
	mkdir -p Release
#
	clang $(HX_INCLUDE) $(HX_QWERTY) $(CPP_FLAGS) -o Release/smsdk_ext.o -c $(HX_SOURCEMOD)/public/smsdk_ext.cpp
	clang $(HX_INCLUDE) $(HX_QWERTY) $(CPP_FLAGS) -o Release/extension.o -c extension.cpp
#
	clang $(HX_SO) sdk/redistributable_bin/linux32/libsteam_api.so -static-libgcc -shared -m32 -lm -ldl -o Release/familyban.ext.so
#
	rm -rf Release/*.o
