COMPILER=clang
NOWARNING=-Weverything -Wno-weak-vtables -Wno-padded -Wno-unused-parameter -Wno-sign-conversion -Wno-c++98-compat -Wno-conversion -Wno-missing-noreturn -Wno-reorder
COMPFLAGS=-std=c++11 -stdlib=libc++ $(NOWARNING)
FRAMEWORKS=OpenGL Cocoa IOKit
LINKFRAMEWORKS=$(addprefix -framework ,$(FRAMEWORKS))
LINKFLAGS=-std=c++11 -stdlib=libc++ -lstdc++ -lglfw $(LINKFRAMEWORKS)

BINDIR=bin
SOURCEDIR=src
OBJDIR=build

DRAWABLENAMES=FullscreenQuad Torus Model
OBJECTNAMES=FrameCounter Shader RunLoop FramebufferObject Printer TextureUnit RenderInfo $(DRAWABLENAMES)
DRAWABLEOBJECTS=$(addsuffix .o,$(DRAWABLENAMES))
ALLOBJECTS=$(addsuffix .o,$(OBJECTNAMES))
OBJECTHEADERS=$(addprefix $(SOURCEDIR)/,$(addsuffix .h,$(OBJECTNAMES)))

DRAWABLEOBJECTFILES=$(addprefix $(OBJDIR)/,$(DRAWABLEOBJECTS))
ALLOBJECTFILES=$(addprefix $(OBJDIR)/,$(ALLOBJECTS))

# Metatargets
.DEFAULT:
	echo "You forgot one of your targets, dumbass"

.PHONY: shortcut all clean 

shortcut: $(BINDIR)/FunGL
	./$(BINDIR)/FunGL

all: $(BINDIR)/FunGL

clean:
	rm -f  $(BINDIR)/FunGL
	rm -fr $(BINDIR)/*.dSYM
	rm -f  $(OBJDIR)/*.o


# Main targets
$(BINDIR)/FunGL: $(OBJDIR)/main.o $(ALLOBJECTFILES) | $(BINDIR)
	$(COMPILER) $(LINKFLAGS) $(OBJDIR)/main.o $(ALLOBJECTFILES) -g -o $(BINDIR)/FunGL

$(OBJDIR)/main.o: $(SOURCEDIR)/main.cpp $(OBJECTHEADERS)
	$(COMPILER) $(COMPFLAGS) $(SOURCEDIR)/main.cpp -c -o $(OBJDIR)/main.o

# Directory targets
$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(ALLOBJECTFILES): | $(OBJDIR)

# Drawables need these headers
$(DRAWABLEOBJECTFILES): $(SOURCEDIR)/Drawable.h $(SOURCEDIR)/Shader.h

$(OBJDIR)/Torus.o: $(SOURCEDIR)/Geometry.h

$(OBJDIR)/Printer.o: $(SOURCEDIR)/Shader.h $(SOURCEDIR)/TextureUnit.h

$(OBJDIR)/FramebufferObject.o: $(SOURCEDIR)/TextureUnit.h

# Wildcard object rule
$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp $(SOURCEDIR)/%.h
	$(COMPILER) $(COMPFLAGS) $< -c -o $@
