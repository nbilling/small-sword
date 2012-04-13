# libtcod samples osx makefile
# to build debug version, run "make -f makefile-samples-osx debug"
# to build release version, run "make -f makefile-samples-osx release"
SRCDIR=src
INCDIR=include
OSXDIR=osx
CC=gcc
CPP=g++
TEMP=/tmp
.SUFFIXES: .o .h .c .hpp .cpp

#dependencies
# SDL
SDL_DIR=dependencies/SDL-1.2.14
SDL_FLAGS=-D_GNU_SOURCE=1 -D_REENTRANT -I $(SDL_DIR)/include
SDL_LIBS=$(SDL_DIR)/lib/osx/libSDL.a $(SDL_DIR)/lib/osx/libSDLmain.a -framework Carbon -framework Quartz -framework OpenGL -framework AppKit -framework IOKit -framework AudioUnit -framework Quicktime -framework Cocoa

# ZLIB
ZLIB_DIR=dependencies/zlib-1.2.3
ZLIB_FLAGS=-I $(ZLIB_DIR)/include
ZLIB_LIBS=$(ZLIB_DIR)/lib/osx/libz.a

# PNG
PNG_DIR=dependencies/libpng-1.2.31
PNG_FLAGS=-I $(PNG_DIR)/include
PNG_LIBS=$(PNG_DIR)/lib/osx/libpng.a

#libtcod
TCOD_DIR=dependencies/libtcod
TCOD_FLAGS=-I $(TCOD_DIR)/include -I $(TCOD_DIR)/include/gui
TCOD_LIBS=$(TCOD_DIR)/libtcod.a $(TCOD_DIR)/libtcodxx.a

CFLAGS=$(FLAGS) -I$(INCDIR) -Wall -O0 $(TCOD_FLAGS) $(SDL_FLAGS) $(ZLIB_FLAGS) $(PNG_FLAGS) -fno-strict-aliasing -m32 -g


$(TEMP)/small-sword/release/src/%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -s -o $@ -c $<
$(TEMP)/small-sword/release/src/%.o : $(OSXDIR)/%.m
	$(CC) $(CFLAGS) -s -o $@ -c $<

OBJS_RELEASE=$(TEMP)/small-sword/release/src/small-sword.o \
	$(TEMP)/small-sword/release/src/macsupport.o \
	$(TEMP)/small-sword/release/src/AI.o \
	$(TEMP)/small-sword/release/src/DungeonGenerator.o \
	$(TEMP)/small-sword/release/src/Object.o \
	$(TEMP)/small-sword/release/src/Rect.o \
	$(TEMP)/small-sword/release/src/Abilities.o \
	$(TEMP)/small-sword/release/src/Formulas.o \
	$(TEMP)/small-sword/release/src/TacticalUI.o \
	$(TEMP)/small-sword/release/src/Tile.o \
	$(TEMP)/small-sword/release/src/Zone.o \
	$(TEMP)/small-sword/release/src/Pathfinding.o


all : release

release : small-sword

E%.cpp :
	$(CPP) src/$*.cpp $(CFLAGS) $(TCOD_LIBS) $(ZLIB_LIBS) $(PNG_LIBS) $(SDL_LIBS) -E > preprocessed_$*.cpp

#apps: samples_c.app samples_cpp.app hmtool.app

#$(TEMP)/libtcod/debug/hmtool :
#	\mkdir -p $@

#$(TEMP)/libtcod/release/hmtool :
#	\mkdir -p $@

#$(TEMP)/libtcod/debug/samples :
#	\mkdir -p $@

$(TEMP)/small-sword/release/src :
	\mkdir -p $@

small-sword : $(TEMP)/small-sword/release/src $(OBJS_RELEASE)
	$(CPP) $(OBJS_RELEASE) $(CFLAGS) -o $@ $(TCOD_LIBS) $(ZLIB_LIBS) $(PNG_LIBS) $(SDL_LIBS)

#samples_c.app : samples_c
#	mkdir -p samples_c.app/Contents/MacOS
#	cp osx/samples_c_Info.plist samples_c.app/Contents/Info.plist
#	cp libtcod.cfg samples_c.app/Contents/MacOS/
#	cp samples_c samples_c.app/Contents/MacOS/
#	cp -rf data/ samples_c.app/Contents/MacOS/

#samples_cpp.app : samples_cpp
#	mkdir -p samples_cpp.app/Contents/MacOS
#	cp osx/samples_cpp_Info.plist samples_cpp.app/Contents/Info.plist
#	cp libtcod.cfg samples_cpp.app/Contents/MacOS/
#	cp samples_cpp samples_cpp.app/Contents/MacOS/
#	cp -rf data/ samples_cpp.app/Contents/MacOS/

#hmtool.app : hmtool
#	mkdir -p hmtool.app/Contents/MacOS
#	cp osx/hmtool_Info.plist hmtool.app/Contents/Info.plist
#	cp libtcod.cfg hmtool.app/Contents/MacOS/
#	cp hmtool hmtool.app/Contents/MacOS/
#	cp -rf data/ hmtool.app/Contents/MacOS/

clean :
	\rm -f $(OBJS_RELEASE) small-sword
#	\rm -rf samples_c.app samples_cpp.app hmtool.app


#OLD OSX FOLLOWS============================
# SRCDIR=.
# INCDIR=include
# CC=gcc
# CPP=g++
# TEMP=/tmp
# .SUFFIXES: .o .h .c .hpp .cpp
# 
# #dependencies
# # SDL
# SDL_DIR=dependencies/SDL-1.2.14
# SDL_FLAGS=-D_GNU_SOURCE=1 -D_REENTRANT -I $(SDL_DIR)/include
# SDL_LIBS=$(SDL_DIR)/lib/osx/libSDL.a $(SDL_DIR)/lib/osx/libSDLmain.a -framework Carbon -framework Quartz -framework OpenGL -framework AppKit -framework IOKit -framework AudioUnit -framework Quicktime -framework Cocoa
# 
# # ZLIB
# ZLIB_DIR=dependencies/zlib-1.2.3
# ZLIB_FLAGS=-I $(ZLIB_DIR)/include
# ZLIB_LIBS=$(ZLIB_DIR)/lib/osx/libz.a
# 
# # PNG
# PNG_DIR=dependencies/libpng-1.2.31
# PNG_FLAGS=-I $(PNG_DIR)/include
# PNG_LIBS=$(PNG_DIR)/lib/osx/libpng.a
# 
# CFLAGS=$(FLAGS) -I$(INCDIR) -I$(INCDIR)/gui -Wall $(SDL_FLAGS) $(ZLIB_FLAGS) $(PNG_FLAGS) -fno-strict-aliasing
# 
# $(TEMP)/%.o : $(SRCDIR)/%.cpp
# 	$(CPP) $(CFLAGS) -o $@ -c $< 
# $(TEMP)/%.o : $(SRCDIR)/%.c
# 	$(CC) $(CFLAGS) -o $@ -c $< 
# $(TEMP)/hmtool/%.o : $(SRCDIR)/src/hmtool/%.cpp
# 	$(CPP) $(CFLAGS) -o $@ -c $< 
# 
# $(TEMP)/hmtool :
# 	mkdir -p $@
# 
# C_OBJS=$(TEMP)/samples_c.o
# CPP_OBJS=$(TEMP)/samples_cpp.o
# 	
# HMTOOLOBJS=$(TEMP)/hmtool/hmtool.o \
# 	$(TEMP)/hmtool/operation.o
# 
# all : samples_c.app samples_cpp.app $(TEMP)/hmtool hmtool.app
# 
# $(TEMP)/hmtool :
# 	mkdir -p $@
# 
# samples_c : $(C_OBJS) libtcod.a
# 	$(CC) $(C_OBJS) $(CFLAGS) -o $@ -L. -ltcod $(ZLIB_LIBS) $(PNG_LIBS) $(SDL_LIBS)
# 
# samples_cpp : $(CPP_OBJS) libtcod++.a
# 	$(CPP) $(CPP_OBJS) -o $@ -L. -ltcod -ltcod++ $(ZLIB_LIBS) $(PNG_LIBS) $(SDL_LIBS)
# 
# hmtool : $(TEMP)/hmtool $(HMTOOLOBJS)
# 	$(CPP) -o $@ $(HMTOOLOBJS) -L. -ltcod -ltcod++ -ltcod-gui $(ZLIB_LIBS) $(PNG_LIBS) $(SDL_LIBS)
# 
# samples_c.app : samples_c
# 	mkdir -p samples_c.app/Contents/MacOS
# 	cp osx/samples_c_Info.plist samples_c.app/Contents/Info.plist
# 	cp samples_c samples_c.app/Contents/MacOS/
# 	cp -r data/ samples_c.app/Contents/MacOS/
# 
# samples_cpp.app : samples_cpp
# 	mkdir -p samples_cpp.app/Contents/MacOS
# 	cp osx/samples_cpp_Info.plist samples_cpp.app/Contents/Info.plist
# 	cp samples_cpp samples_cpp.app/Contents/MacOS/
# 	cp -r data/ samples_cpp.app/Contents/MacOS/
# 
# hmtool.app : hmtool
# 	mkdir -p hmtool.app/Contents/MacOS
# 	cp osx/hmtool_Info.plist hmtool.app/Contents/Info.plist
# 	cp hmtool hmtool.app/Contents/MacOS/
# 
# clean :
# 	\rm -f $(C_OBJS) $(CPP_OBJS) $(HMTOOLOBJS) samples_c samples_cpp hmtool
# 	\rm -rf hmtool.app samples_cpp.app samples_c.app
