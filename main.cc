#include <GL/glut.h>
#include <ctime>
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "collision.h"
#include "spaceship.h"
#include "asteroid.h"
#include "game.h"

static const size_t CallbackTime = 40;
static const int PanelWidth = 800;
static const int PanelHeight = 600;
static const float MaxVelocity = 10.0f;

Game game;
bool UpKeyPress = false;
bool DownKeyPress = false;
bool LeftKeyPress = false;
bool RightKeyPress = false;

void renderScene (void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glColor3f(1.0f, 1.0f, 1.0f);

  // Draw awesome spaceship.
  SpaceShip& ss = game.spaceship_;
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(ss.position_.x, ss.position_.y, 0.0f);
  glRotatef(ss.direction_, 0.0f, 0.0f, 1.0f);
  glBegin(GL_TRIANGLES);
    glVertex2f(-5.0f, -8.0f);
    glVertex2f(5.0f, -8.0f);
    glVertex2f(0.0f, 12.0f);
  glEnd();
  glPopMatrix();

  // Bounding box for spaceship.
  glColor3f(1.0f, 0.0f, 0.0f);
  glPushMatrix();
  glLoadIdentity();
  std::vector<Point> points;
  computeBoundingBox(ss, &points);
  glBegin(GL_LINE_STRIP);
    for (size_t i = 0; i < points.size(); ++i) {
      glVertex2f(points[i].x, points[i].y);
    }
    glVertex2f(points[0].x, points[0].y);
  glEnd();
  glPopMatrix();


  glColor3f(1.0f, 1.0f, 1.0f);
  // Draw asteroid.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    Asteroid& a = game.asteroids_[i];
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(a.position_.x, a.position_.y, 0.0f);
    glRotatef(a.direction_, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float scale = 10.0f;
      glVertex2f(-1.6 * scale, 0 * scale);
      glVertex2f(-1 * scale, 1 * scale);
      glVertex2f(0.1 * scale, 1.3 * scale);
      glVertex2f(1 * scale, 1.1 * scale);
      glVertex2f(1.5 * scale, 0.1 * scale);
      glVertex2f(1.1 * scale, -0.9 * scale);
      glVertex2f(0 * scale, -1.2 * scale);
      glVertex2f(-1 * scale, -0.9 * scale);
      glVertex2f(-1.6 * scale, 0 * scale);
    glEnd();
  }

  glutSwapBuffers();
}

void timerCallback(int)
{
  SpaceShip& ss = game.spaceship_;

  // Update velocity of the spaceship.
  if (UpKeyPress) {
    if (ss.velocity_.x < MaxVelocity) {
      ss.velocity_.x -= sin(ss.direction_ / 360.0f * 2 * M_PI);
    }
    if (ss.velocity_.y < MaxVelocity) {
      ss.velocity_.y += cos(ss.direction_ / 360.0f * 2 * M_PI);
    }
  }
  if (DownKeyPress) {
    if (ss.velocity_.x > -MaxVelocity) {
      ss.velocity_.x += sin(ss.direction_ / 360.0f * 2 * M_PI);
    }
    if (ss.velocity_.y > -MaxVelocity) {
      ss.velocity_.y -= cos(ss.direction_ / 360.0f * 2 * M_PI);
    }
  }
  if (LeftKeyPress) {
    ss.direction_ += 5.0f;
  }
  if (RightKeyPress) {
    ss.direction_ -= 5.0f;
  }

  // Update spaceship position.
  ss.position_.x = ss.position_.x + ss.velocity_.x;
  ss.position_.y = ss.position_.y + ss.velocity_.y;
  ss.velocity_.x *= .99f;
  ss.velocity_.y *= .99f;

  if (ss.position_.x < 0.0f) {
    ss.position_.x = PanelWidth;
  } else if (ss.position_.x > PanelWidth) {
    ss.position_.x = 0.0f;
  }

  if (ss.position_.y < 0.0f) {
    ss.position_.y = PanelHeight;
  } else if (ss.position_.y > PanelHeight) {
    ss.position_.y = 0.0f;
  }

  // Update asteroids position.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    Asteroid& a = game.asteroids_[i];
    a.position_.x = a.position_.x + a.velocity_.x;
    a.position_.y = a.position_.y + a.velocity_.y;

    if (a.position_.x < 0.0f || a.position_.x > PanelWidth) {
      a.velocity_.x *= -1.0f;
    }

    if (a.position_.y < 0.0f || a.position_.y > PanelHeight) {
      a.velocity_.y *= -1.0f;
    }
  }

  // Collision between spaceship and asteroids.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    SpaceShip& ss = game.spaceship_;
    Asteroid& a = game.asteroids_[i];

    if (collision(ss, a)) {
      std::cout << "collision" << std::endl;
    }
  }


  // Check for collisions between asteroids.
  //for (size_t i = 0; i < game.asteroids_.size(); ++i) {
  //  Asteroid& a = game.asteroids_[i];

  //  for (size_t k = 0; k < game.asteroids_.size(); ++k) {
  //    if (i == k) {
  //      continue;
  //    }

  //    Asteroid& b = game.asteroids_[k];

  //    if (collision(a, b)) {

  //    }
  //  }

  //}



  renderScene();

  glutTimerFunc(CallbackTime, timerCallback, 0);
}

void processSpecialKeys (int key, int, int)
{
  switch (key) {
    case GLUT_KEY_UP:
      UpKeyPress = true;
      break;
   case GLUT_KEY_DOWN:
      DownKeyPress = true;
      break;
    case GLUT_KEY_LEFT:
      LeftKeyPress = true;
      break;
    case GLUT_KEY_RIGHT:
      RightKeyPress = true;
      break;
    default:
      break;
  }
}

void processSpecialKeysUp (int key, int, int)
{
  switch (key) {
    case GLUT_KEY_UP:
      UpKeyPress = false;
      break;
   case GLUT_KEY_DOWN:
      DownKeyPress = false;
      break;
    case GLUT_KEY_LEFT:
      LeftKeyPress = false;
      break;
    case GLUT_KEY_RIGHT:
      RightKeyPress = false;
      break;
    default:
      break;
  }
}

int main (int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(PanelWidth, PanelHeight);
  glutCreateWindow("Asteroids");

  glutDisplayFunc(renderScene);
  glutTimerFunc(CallbackTime, timerCallback, 0);
  glutSpecialFunc(processSpecialKeys);
  glutSpecialUpFunc(processSpecialKeysUp);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, PanelWidth, 0, PanelHeight);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();



  glutMainLoop();

  return 0;
}
