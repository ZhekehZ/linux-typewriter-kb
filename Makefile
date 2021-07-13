
all:
	g++ -std=c++17 src/*.cpp src/keyboard/*.cpp src/sound/*.cpp -Iinclude -lSDL2 -lSDL2_mixer
