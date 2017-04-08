#Makefile for the bomberman project
EXECUTABLE = bombman

#parts
ENGINE = 					engine/Actor.o \
							engine/Bomberman.o \
							engine/Controllers.o \
							engine/Font.o \
							engine/Room.o \
							engine/ShaderProgram.o \
							engine/TextureCache.o 
ENGINE_BOMERMAN =			engine/bomberman/Button.o \
							engine/bomberman/Level.o \
							engine/bomberman/MainMenu.o
ENGINE_BOMBERMAN_ACTORS =	engine/bomberman/actors/Scenery.o \
							engine/bomberman/actors/Destructables.o
EXTERNAL_OBJECTS = 			extern/lodepng.o \
							extern/gl_core_4_4.o
GEOMETRY_OBJECTS =	 		geometry/Point.o \
							geometry/Rectangle.o
MATH_OBJECTS = 				math/Math.o \
							math/Matrix4x4.o \
							math/Vector3.o \
							math/Vector4.o
UTILITY_OBJECTS =			utility/Settings.o

OBJECTS = 	$(ENGINE) \
			$(ENGINE_BOMERMAN) \
			$(ENGINE_BOMBERMAN_ACTORS) \
			$(EXTERNAL_OBJECTS) \
			$(GEOMETRY_OBJECTS) \
			$(MATH_OBJECTS) \
			$(UTILITY_OBJECTS) \
			Core.o main.o

#build tools
COMPILER = g++
FLAGS = -std=c++14 -Wall -g
#FLAGS = -std=c++14 -Wall -O2

ifeq ($(OS), Windows_NT)
LINKER_SDL = -lmingw32 -lSDL2main -lSDL2
LIBRARIES = $(LINKER_SDL) -lm -lopengl32 -lgdi32 
else 								#usr of user?
LIBRARIES = -lm -lGL -lGLU -lSDL2 -L/user/lib -lpthread
endif

#direcory structure
SOURCE_DIRECTORY = src
BUILD_DIRECTORY = build
STRUCTURE_FLAG = $(BUILD_DIRECTORY)/structure.flag

$(EXECUTABLE): $(STRUCTURE_FLAG) $(addprefix $(BUILD_DIRECTORY)/,$(OBJECTS))
	$(COMPILER) $(FLAGS) $(addprefix $(BUILD_DIRECTORY)/, $(OBJECTS)) -o $@ $(LIBRARIES)

#include dependencies
-include $(addprefix $(BUILD_DIRECTORY)/,$(OBJECTS:.o=.d))

#build folder structure when flag is not set
$(STRUCTURE_FLAG):
	mkdir -p $(filter-out %./,$(addprefix $(BUILD_DIRECTORY)/,$(dir $(OBJECTS))))
	mkdir -p $(filter-out %./,$(addprefix $(BUILD_DIRECTORY)/,$(dir $(OBJECTS))))
	touch $(STRUCTURE_FLAG)

#remove all non-source files
clean:
	rm -rf build/
	rm -rf $(EXECUTABLE) $(EXECUTABLE).exe

rebuild: clean $(EXECUTABLE)

#call this when the folder structure changes
restructure:
	rm $(STRUCTURE_FLAG)

#build and run
run: $(EXECUTABLE)
	@echo "Finalized build, running executable:"
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	@echo running executable through valgrind on default settings
	valgrind --track-origins=yes ./$(EXECUTABLE)

fullvalgrind: $(EXECUTABLE)
	@echo running executable through valgrind with extra checks
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(EXECUTABLE)

#compile objects rule
$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<
	$(COMPILER) -MM -MT $(BUILD_DIRECTORY)/$*.o src/$*.cpp > $(BUILD_DIRECTORY)/$*.d