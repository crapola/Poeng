# Executable name.
exe=Debug\PoengSDL
warnings=-Wall -Weffc++ -pedantic -Wextra
# Includes.
includes=-IF:\Libraries\SDL2-2.0.10\x86_64-w64-mingw32\include\SDL2 \
-IF:\Libraries\SDL2_mixer-2.0.4\x86_64-w64-mingw32\include\SDL2
# Extra gcc options.
options=-g
# Linker options.
LDFLAGS=-LF:\Libraries\SDL2-2.0.10\x86_64-w64-mingw32\lib \
-LF:\Libraries\SDL2_mixer-2.0.4\x86_64-w64-mingw32\lib
# Libs to link against. Most dependant first.
# Example: -lgdi32 -luser32 -lkernel32 -lwinmm
LDLIBS=-lmingw32 -lSDL2main -lSDL2.dll -luser32 -lgdi32 -lwinmm -ldxguid -lsdl2 -lsdl2_mixer
#-------------------------------------------------------------------------------
src_folders=src
CXXFLAGS=-std=c++17 $(warnings) $(includes) $(options) -MMD
src_files=\
	$(wildcard src/*.cpp)\
	$(wildcard src/*/*.cpp)
#   $(wildcard src/*/*/*.cpp) etc...
src_files:=$(filter-out src/web/main.cpp,$(src_files))
objdir=bin
objs=$(src_files:src/%.cpp=$(objdir)/%.o)
target=$(objdir)/$(exe).exe

# Target.
all: $(target)
	@echo Done.
# Link.
$(target): $(objs)	
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
	$(CXX) -o $(target) $(objs) $(LDFLAGS) $(LDLIBS)
#   windres.exe -J rc -O coff -i res/resources.rc -o $(objdir)/resources.res
#   $(CXX) -o $(target) $(objs) $(objdir)/resources.res $(LDFLAGS) $(LDLIBS)

# Compile object files.
$(objdir)/%.o: src/%.cpp
$(objdir)/%.o: src/%.cpp $(objdir)/%.d
#	Create folder when needed.
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Dependencies.
# Based on http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#tldr
depfiles:=$(src_files:src/%.cpp=$(objdir)/%.d)

$(depfiles):

include $(wildcard $(depfiles))

# Clean temp folder.
clean:
	del $(objdir)\*.o