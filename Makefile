all: compile link
compile:
	g++ -I src/include -c main.cpp src/Game.cpp src/Table.cpp src/UI.cpp src/Ball.cpp src/Cue.cpp src/Pocket.cpp 
link:
	g++ main.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system