COMPILER=clang
NOWARNING=-Weverything -Wno-padded -Wno-unused-parameter -Wno-sign-conversion -Wno-c++98-compat -Wno-conversion -Wno-missing-noreturn
COMPFLAGS=-std=c++11 -stdlib=libc++ $(NOWARNING)
LINKFRAMEWORKS=-framework OpenGL -framework Cocoa -framework IOKit
LINKFLAGS=-lstdc++ -lglfw $(LINKFRAMEWORKS)

BINDIR=bin
SOURCEDIR=src
OBJDIR=build

DRAWABLEOBJECTS=FullscreenQuad.o Torus.o
ALLOBJECTS=FrameCounter.o Shader.o RunLoop.o FramebufferObject.o $(DRAWABLEOBJECTS)

DRAWABLES=$(addprefix $(OBJDIR)/,$(DRAWABLEOBJECTS))
ALLOBJECTFILES=$(addprefix $(OBJDIR)/,$(ALLOBJECTS))




.DEFAULT:
	echo "You forgot one of your targets, dumbass"

# Metatargets
.PHONY: shortcut all clean 

shortcut: $(BINDIR)/GLFWTest
	./$(BINDIR)/GLFWTest

all: $(BINDIR)/GLFWTest

clean:
	rm -f $(BINDIR)/GLFWTest
	rm -f build/*.o
	rm -rf $(BINDIR)/*.dSYM

# Normal targets
$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/GLFWTest: $(SOURCEDIR)/main.cpp $(ALLOBJECTFILES) | $(BINDIR)
	$(COMPILER) $(COMPFLAGS) $(LINKFLAGS) $(SOURCEDIR)/main.cpp $(ALLOBJECTFILES) -g -o $(BINDIR)/GLFWTest

# Drawables need these headers
$(DRAWABLES): $(SOURCEDIR)/Drawable.h

$(OBJDIR)/Torus.o: $(SOURCEDIR)/Geometry.h $(SOURCEDIR)/Matrix.h

# Object rules
$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(COMPILER) $(COMPFLAGS) $< -c -o $@

$(ALLOBJECTFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
