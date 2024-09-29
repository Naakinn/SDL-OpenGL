main: 
	gcc -Wall -I include/ -fuse-ld=mold -o engine src/main.c src/glad.c src/shader.c -lSDL2 
clean: 
	rm engine
