main: 
	gcc -Wall -I include/ -o engine src/main.c src/glad.c src/shader.c -lSDL2 
debug: 
	gcc -ggdb -O0 -Wall -I include/ -o engine src/main.c src/glad.c src/shader.c -lSDL2 
clean: 
	rm engine
