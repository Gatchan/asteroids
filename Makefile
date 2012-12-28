all:
	g++ -g main.cc game.cc point.cc collision.cc asteroid.cc spaceship.cc bullet.cc gameobject.cc -o asteroids -W -Wall -g -lglut -lGLU -lGL

