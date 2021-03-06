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
static const float MaxVelocity = 10.0f;
static const float MaxVelocityAsteroid = 5.0f;

Game game(Size(800.0f, 600.0f));

bool UpKeyPress = false;
bool DownKeyPress = false;
bool LeftKeyPress = false;
bool RightKeyPress = false;
bool ShootKeyPress = false;

bool show_bounding_boxes = false;

void renderScene (void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw bullets.
  for (size_t i = 0; i < game.bullets_.size(); ++i) {
    Bullet& b = game.bullets_[i];
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(b.Position().x, b.Position().y, 0.0f);
    glRotatef(b.Direction(), 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float steps = 10;
      const float scale = b.Bounding().width;
      for (int i = 0; i < steps; ++i) {
        const float angle = 2.0f * M_PI * i / float(steps);
        glVertex2f(scale * sin(angle), scale * cos(angle));
      }

    glEnd();
    glPopMatrix();
  }

  // Draw asteroids.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    glColor3f(0.2f, 0.2f, 0.25f);
    Asteroid& a = game.asteroids_[i];
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(a.Position().x, a.Position().y, 0.0f);
    glRotatef(a.Direction(), 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float x_scale = a.Bounding().width / 2.0f;
      const float y_scale = a.Bounding().height / 2.0f;
      glVertex2f(-1.0f * x_scale, 0.0f * y_scale);
      glVertex2f(-0.8f * x_scale, 0.3f * y_scale);
      glVertex2f(-0.1f * x_scale, 0.7f * y_scale);
      glVertex2f(0.4f * x_scale, 1.0f * y_scale);
      glVertex2f(1.0f * x_scale, 0.6f * y_scale);
      glVertex2f(0.9f * x_scale, -0.3f * y_scale);
      glVertex2f(0.3f * x_scale, -0.7f * y_scale);
      glVertex2f(-0.5f * x_scale, -1.0f * y_scale);
      glVertex2f(-1.0f * x_scale, 0.0f * y_scale);
    glEnd();

    // Bounding boxes.
    if (show_bounding_boxes) {
      glColor3f(1.0f, 0.0f, 0.0f);
      glPushMatrix();
      glLoadIdentity();
      std::vector<Point> points;
      computeBoundingBox(a, &points);
      glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < points.size(); ++i) {
          glVertex2f(points[i].x, points[i].y);
        }
        glVertex2f(points[0].x, points[0].y);
      glEnd();
      glPopMatrix();
    }
  }




  // Draw awesome spaceship.
  if (game.over_) {  // This is actually a crash.
    SpaceShip& ss = game.spaceship_;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ss.Position().x, ss.Position().y, 0.0f);
    glRotatef(ss.Direction(), 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float steps = 100;
      const float scale = std::max(ss.Bounding().width,
                                   ss.Bounding().height) * 2.0f;
      for (int i = 0; i < steps; ++i) {
        const float angle = 2.0f * M_PI * i / float(steps);
        glVertex2f(scale * sin(angle), scale * cos(angle));
      }

    glEnd();
    glPopMatrix();
  } else {
    SpaceShip& ss = game.spaceship_;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ss.Position().x, ss.Position().y, 0.0f);
    glRotatef(ss.Direction(), 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 0.1f, 0.0f);
    glBegin(GL_TRIANGLES);
      const float x_scale = ss.Bounding().width / 2.0f;
      const float y_scale = ss.Bounding().height / 2.0f;
      glVertex2f(-1.0f * x_scale, -1.0f * y_scale);
      glVertex2f(0.0f * x_scale, 1.0f * y_scale);
      glVertex2f(1.0f * x_scale, -1.0f * y_scale);
    glEnd();
    glPopMatrix();
  }

  if (show_bounding_boxes) {
    // Bounding box for spaceship.
    SpaceShip& ss = game.spaceship_;
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
  }

  glutSwapBuffers();
}

void timerCallback(int)
{
  if (game.over_) {
    return;
  }

  // Operate spaceship.
  SpaceShip& ss = game.spaceship_;

  if (UpKeyPress) {
    ss.Thrust(Forward);
  }
  if (DownKeyPress) {
    ss.Thrust(Backward);
  }
  if (LeftKeyPress) {
    ss.Turn(Left);
  }
  if (RightKeyPress) {
    ss.Turn(Right);
  }

  ss.Update();

  // Shooting.
  if (ShootKeyPress) {
    Bullet bullet(&game);
    if (ss.Shoot(&bullet)) {
      game.bullets_.push_back(bullet);
    }
  }

  // Update positions of bullets.
  for (size_t i = 0; i < game.bullets_.size(); ++i) {
    Bullet& b = game.bullets_[i];
    b.SetPosition(b.Position() + b.Velocity());
  }

  // Remove bullets when they are out of the screen.
  typedef std::vector<Bullet>::iterator it_bullets;
  for (it_bullets it = game.bullets_.begin(); it != game.bullets_.end(); ) {
    if (!it->OnScreen()) {
      it = game.bullets_.erase(it);
    } else {
      ++it;
    }
  }

  for (size_t k = 0; k < game.bullets_.size(); ++k) {
      for (size_t i = 0; i < game.asteroids_.size(); ++i) {
        if (collision(game.bullets_[k], game.asteroids_[i])) {
          Vector velocity = game.asteroids_[i].Velocity() +
                            game.bullets_[k].Velocity() * 0.1f;
          velocity.x = std::max(std::min(velocity.x, MaxVelocityAsteroid),
                                -MaxVelocityAsteroid);
          velocity.y = std::max(std::min(velocity.y, MaxVelocityAsteroid),
                                -MaxVelocityAsteroid);
          game.asteroids_[i].SetVelocity(velocity);

          game.bullets_.erase(game.bullets_.begin() + k);
          break;
        }
      }
  }

  // Update asteroids position.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    Asteroid& a = game.asteroids_[i];
    a.Update();
  }

  // Collision between spaceship and asteroids.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    SpaceShip& ss = game.spaceship_;
    Asteroid& a = game.asteroids_[i];

    if (collision(ss, a)) {
      game.over_ = true;
    }
  }

  // Check for collisions between asteroids.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    Asteroid& a = game.asteroids_[i];

    for (size_t k = 0; k < game.asteroids_.size(); ++k) {
      if (i == k) {
        continue;
      }

      Asteroid& b = game.asteroids_[k];

      if (collision(a, b)) {
        // Make them bounce only if in the next iteration they will be closer.
        if (distance(a.Position(), b.Position()) >
            distance(a.Position() + a.Velocity(), b.Position() + b.Velocity())) {

          Vector tmp = a.Velocity();
          a.SetVelocity(b.Velocity());
          b.SetVelocity(tmp);

          // Every time that asteroids collide they slow down.
          a.Velocity() *= 0.9f;
          b.Velocity() *= 0.9f;
        }
      }
    }
  }



  renderScene();

  glutTimerFunc(CallbackTime, timerCallback, 0);
}

void processKeys (unsigned char key, int, int)
{
  switch (key) {
    case 'd':
      show_bounding_boxes = !show_bounding_boxes;
      break;
    case ' ':
      ShootKeyPress = true;
      break;
    default:
      break;
    }
}

void processKeysUp (unsigned char key, int, int)
{
  switch (key) {
    case 'd':
      break;
    case ' ':
      ShootKeyPress = false;
      break;
    default:
      break;
    }
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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  glEnable(GL_MULTISAMPLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(game.WindowSize().width, game.WindowSize().height);
  glutCreateWindow("Asteroids");

  glutDisplayFunc(renderScene);
  glutTimerFunc(CallbackTime, timerCallback, 0);
  glutKeyboardFunc(processKeys);
  glutKeyboardUpFunc(processKeysUp);
  glutSpecialFunc(processSpecialKeys);
  glutSpecialUpFunc(processSpecialKeysUp);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, game.WindowSize().width, 0, game.WindowSize().height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glutMainLoop();

  return 0;
}
