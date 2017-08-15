# Makefile for bomberman

EXECUTABLE = bomberman

#objects by folders
BOMBERMAN_ENTITIES	=	bomberman/entities/Wall.o

BOMBERMAN_MENUS		=	bomberman/menus/MainMenu.o

BOMBERMAN			=	$(BOMBERMAN_ENTITIES) \
						$(BOMBERMAN_MENUS) \
						bomberman/Bomberman.o \
						bomberman/Bomberman_structors.o \
						bomberman/Bomberman_accessors.o \
						bomberman/Level.o

	CORE			=	core/Exception.o \
						core/Stringable.o

	ENGINE			=	engine/Controllers.o \
						engine/DrawCall.o \
						engine/Entity.o \
						engine/Font.o \
						engine/GameStateController.o \
						engine/MouseController.o \
						engine/RenderableString.o \
						engine/ShaderProgram.o \
						engine/TextureCache.o

	EXTERN			=	extern/gl_core_4_4.o \
						extern/lodepng.o

	GEOMETRY		=	geometry/Point.o \
						geometry/Rectangle.o

	MATH		 	= 	math/Math.o \
						math/Matrix4x4.o \
						math/Vector3.o \
						math/Vector4.o

	UTILITY			=	utility/Settings.o \
						utility/Utility.o


#combine objects
OBJECTS = $(BOMBERMAN) $(CORE) $(ENGINE) $(EXTERN) $(GEOMETRY) $(MATH) $(UTILITY) main.o

#build macros
COMPILER = g++
FLAGS = -std=c++17 -Wall -g
FLAGS_RELEASE = -std=c++17 -Wall -O2
LIBRARIES = -lm

ifeq ($(OS), Windows_NT)
LINKER_SDL = -lmingw32 -lSDL2main -lSDL2
LIBRARIES = $(LINKER_SDL) -lm -lopengl32 -lgdi32 
else 								#usr of user?
LIBRARIES = -lm -lGL -lGLU -lSDL2 -L/user/lib -lpthread
endif

#folders
SOURCEDIR = src
BUILDDIR = build
STRUCTUREFLAG = $(BUILDDIR)/structure.flag

#executable build
$(EXECUTABLE): $(STRUCTUREFLAG) $(addprefix $(BUILDDIR)/,$(OBJECTS))
	$(COMPILER) $(FLAGS) $(addprefix $(BUILDDIR)/,$(OBJECTS)) -o $@ $(LIBRARIES)

#optional include of dependencies
-include $(addprefix $(BUILDDIR)/,$(OBJECTS:.o=.d))

$(STRUCTUREFLAG):
	mkdir -p $(filter-out %./,$(addprefix $(BUILDDIR)/,$(dir $(OBJECTS))))
	touch $(STRUCTUREFLAG)

restructure:
	rm $(STRUCTUREFLAG)

clean:
	rm -rf build/

run: $(EXECUTABLE)
	./$(EXECUTABLE)

rebuild: clean $(EXECUTABLE)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<
	$(COMPILER) -MM -MT $(BUILDDIR)/$*.o src/$*.cpp > $(BUILDDIR)/$*.d