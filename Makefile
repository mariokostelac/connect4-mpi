CC = mpic++
CFLAGS = -g -O2 -Wall -std=c++11

game-deps = src/solver.cpp src/game_state.cpp src/communicator_master.cpp src/messages.cpp src/slave.cpp src/timer.cpp
game-deps-h = src/solver.h src/game_state.h src/communicator_master.h src/messages.h src/slave.h src/timer.h

all: game

game: src/game.h src/game.cpp $(game-deps) $(game-deps-h)
	$(CC) $(CFLAGS) $(game-deps)  src/game.cpp -o bin/game
