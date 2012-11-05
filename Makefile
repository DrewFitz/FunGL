COMPILER=clang
NOWARNING=-Weverything -Wno-weak-vtables -Wno-padded -Wno-unused-parameter -Wno-sign-conversion -Wno-c++98-compat -Wno-conversion -Wno-missing-noreturn -Wno-reorder
COMPFLAGS=-std=c++11 -stdlib=libc++ $(NOWARNING)
FRAMEWORKS=OpenGL Cocoa IOKit
LINKFRAMEWORKS=$(addprefix -framework ,$(FRAMEWORKS))
LINKFLAGS=-lstdc++ -lglfw $(LINKFRAMEWORKS)

BINDIR=bin
SOURCEDIR=src
OBJDIR=build

DRAWABLENAMES=FullscreenQuad Torus Model
OBJECTNAMES=FrameCounter Shader RunLoop FramebufferObject Printer TextureUnit $(DRAWABLENAMES)
DRAWABLEOBJECTS=$(addsuffix .o,$(DRAWABLENAMES))
ALLOBJECTS=$(addsuffix .o,$(OBJECTNAMES))

DRAWABLEOBJECTFILES=$(addprefix $(OBJDIR)/,$(DRAWABLEOBJECTS))
ALLOBJECTFILES=$(addprefix $(OBJDIR)/,$(ALLOBJECTS))




.DEFAULT:
	echo "You forgot one of your targets, dumbass"

# Metatargets
.PHONY: shortcut all clean 

shortcut: $(BINDIR)/FunGL
	./$(BINDIR)/FunGL

all: $(BINDIR)/FunGL

clean:
	rm -f $(BINDIR)/FunGL
	rm -f build/*.o
	rm -rf $(BINDIR)/*.dSYM

# Normal targets
$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/FunGL: $(SOURCEDIR)/main.cpp $(ALLOBJECTFILES) $(SOURCEDIR)/TextureUnit.h | $(BINDIR)
	$(COMPILER) $(COMPFLAGS) $(LINKFLAGS) $(SOURCEDIR)/main.cpp $(ALLOBJECTFILES) -g -o $(BINDIR)/FunGL

# Drawables need these headers
$(DRAWABLEOBJECTFILES): $(SOURCEDIR)/Drawable.h $(SOURCEDIR)/Shader.h

$(OBJDIR)/Torus.o: $(SOURCEDIR)/Geometry.h $(SOURCEDIR)/Matrix.h

$(OBJDIR)/Printer.o: $(SOURCEDIR)/Shader.h $(SOURCEDIR)/TextureUnit.h

$(OBJDIR)/FramebufferObject.o: $(SOURCEDIR)/TextureUnit.h

# Object rules
$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h
	$(COMPILER) $(COMPFLAGS) $< -c -o $@

$(ALLOBJECTFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
