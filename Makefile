main: 
	gcc main.c src/glad.c -I include/ -lSDL2
clean: 
	rm a.out
