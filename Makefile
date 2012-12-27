all:
	g++ main.cc game.cc point.cc collision.cc -o asteroids -W -Wall -g -lglut -lGLU

