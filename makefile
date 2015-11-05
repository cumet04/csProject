OS = ${shell uname}
VPATH = src header
OBJ = main.o drawing.o camera.o objects.o vector3d.o pointreader.o gui.o world.o

ifeq (${OS}, Linux)
bin_linux.out:	${OBJ}
		g++ -o $@ $^ -lGLU -lGL -lglut -lm -pthread
endif
ifeq (${OS}, Darwin)
bin_mac.out:	${OBJ}
		g++ -o $@ $^ -framework GLUT -framework OpenGL
endif

.cpp.o:
		g++ -I./header -c $<

main.o:			main.hpp drawing.hpp world.hpp camera.hpp gui.hpp
drawing.o:		main.hpp drawing.hpp camera.hpp 
world.o:		main.hpp world.hpp objects.hpp camera.hpp vector3d.hpp
objects.o:		main.hpp world.hpp objects.hpp pointreader.hpp vector3d.hpp
camera.o:		main.hpp objects.hpp camera.hpp vector3d.hpp
gui.o:			main.hpp drawing.hpp camera.hpp gui.hpp vector3d.hpp
pointreader.o:	pointreader.hpp
vector3d.o:		vector3d.hpp