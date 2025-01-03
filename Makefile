build:
	g++ -Wall ./src/*.cpp ./include/glad/glad.c -I"./include/" -lglfw3 -o opengl
run:
	./opengl
clean:
	rm opengl
