LDFLAGS = -lGL -lGLU -lglut
CFLAGS= -w -g -Wall -std=c++11
CC=g++
EXEEXT=
RM=rm
CXXFLAGS=-std=c++11

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
    # RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32
else
# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	endif
endif

PROGRAM_NAME= main

run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEEXT)

$(PROGRAM_NAME): main.o math/Vector3.o nodes/GroupNode.o nodes/Node.o nodes/NodeType.o nodes/MaterialNode.o nodes/ShapeNode.o nodes/TransformationNode.o nodes/TextureNode.o scene/HitBox.o scene/SceneObject.o scene/SceneGraph.o util/Camera.o util/Color.o util/Light.o util/Material.o util/Shape.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -g

clean:
	$(RM) math/*.o nodes/*.o scene/*.o util/*.o *.o $(PROGRAM_NAME)$(EXEEXT)