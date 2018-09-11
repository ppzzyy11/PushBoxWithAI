make:
	#clang++ -o a.out main.cpp -l SDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image
	g++ -o a.out game.cpp -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -std=c++11&&./a.out

clean:
	rm a.out
