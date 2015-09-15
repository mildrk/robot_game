CC = g++
CFLAGS = -g -Wall -Wno-deprecated
OS = $(shell uname)
DEPS = Mesh
OBJ = Mesh.o
ifeq ($(OS), Darwin)
	LIBS = -framework OpenGL -framework GLUT
else ifeq ($(OS), Linux)
	LIBS = -lglut -lGLU -lGL
else
#	CC=i686-pc-mingw32-g++
#	CC=i686-w64-mingw32-g++
	CC=x86_64-w64-mingw32-g++
	LIBS = -lglut32 -lglu32 -lopengl32
#	LIBS = -lglut -lGLU -lGL
endif

game.o: game.cpp
	$(CC) $(CFLAGS) -c $<

Mesh.o: Mesh.cpp
	$(CC) $(CFLAGS) -c $<

game: game.o Mesh.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) Mesh.o

clean: FORCE
	-rm -f *.o *~ game *core*

FORCE: