#Makefile for the bomberman project
EXECUTABLE = Bomberman

#parts
EXTERNAL_OBJECTS = extern/lodepng.o
OBJECTS = $(EXTERNAL_OBJECTS) main.o

#build tools
COMPILER = g++
FLAGS = -std=c++14 -Wall -g
LIBRARIES = -lm -lopengl32 -lgdi32

#direcory structure
SOURCE_DIRECTORY = src
BUILD_DIRECTORY = build
DEPENDENCY_DIRECTORY = $(BUILD_DIRECTORY)/dependencies
STRUCTURE_FLAG = $(DEPENDENCY_DIRECTORY)/structure.flag

$(EXECUTABLE): $(STRUCTURE_FLAG) $(addprefix $(BUILD_DIRECTORY)/,$(OBJECTS))
	$(COMPILER) $(FLAGS) $(addprefix $(BUILD_DIRECTORY)/, $(OBJECTS)) -o $@ $(LIBRARIES)

#include dependencies
-include $(addprefix $(BUILD_DIRECTORY)/,$(OBJECTS:.o=.d))

#build folder structure when flag is not set
$(STRUCTURE_FLAG):
	mkdir -p $(filter-out %./,$(addprefix $(BUILD_DIRECTORY)/,$(dir $(OBJECTS))))
	mkdir -p $(filter-out %./,$(addprefix $(DEPENDENCY_DIRECTORY)/,$(dir $(OBJECTS))))
	touch $(STRUCTURE_FLAG)

#call this when the folder structure changes
restructure:
	rm $(STRUCTURE_FLAG)

#remove all non-source files
clean:
	rm -rf build/

#build and run
run: $(EXECUTABLE)
	@echo "Finalized build, running executable:"
	./$(EXECUTABLE)

#compile objects rule
$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp
	#build file
	$(COMPILER) $(FLAGS) -o $@ -c $<
	#generate dependencies
	$(COMPILER) -MM -MT $(BUILD_DIRECTORY)/$*.o src/$*.cpp > $(DEPENDENCY_DIRECTORY)/$*.d