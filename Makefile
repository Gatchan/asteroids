all:
	g++ main.cc game.cc point.cc collision.cc asteroid.cc -o asteroids -W -Wall -g -lglut -lGLU

