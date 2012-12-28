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
bool ShootKeyPress = false;

bool show_bounding_boxes = false;

void renderScene (void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw asteroids.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    glColor3f(0.2f, 0.2f, 0.25f);
    Asteroid& a = game.asteroids_[i];
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(a.position_.x, a.position_.y, 0.0f);
    glRotatef(a.direction_, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float x_scale = a.bounding_box_.width / 2.0f;
      const float y_scale = a.bounding_box_.height / 2.0f;
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


  // Draw bullets.
  for (size_t i = 0; i < game.bullets_.size(); ++i) {
    Bullet& b = game.bullets_[i];
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(b.position_.x, b.position_.y, 0.0f);
    glRotatef(b.direction_, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float steps = 10;
      const float scale = b.bounding_box_.width;
      for (int i = 0; i < steps; ++i) {
        const float angle = 2.0f * M_PI * i / float(steps);
        glVertex2f(scale * sin(angle), scale * cos(angle));
      }

    glEnd();
    glPopMatrix();
  }


  // Draw awesome spaceship.
  if (game.over_) {  // This is actually a crash.
    SpaceShip& ss = game.spaceship_;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(ss.position_.x, ss.position_.y, 0.0f);
    glRotatef(ss.direction_, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
      const float steps = 100;
      const float scale = std::max(ss.bounding_box_.width,
                                   ss.bounding_box_.height) * 2.0f;
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
    glTranslatef(ss.position_.x, ss.position_.y, 0.0f);
    glRotatef(ss.direction_, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 0.1f, 0.0f);
    glBegin(GL_TRIANGLES);
      const float x_scale = ss.bounding_box_.width / 2.0f;
      const float y_scale = ss.bounding_box_.height / 2.0f;
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


  SpaceShip& ss = game.spaceship_;
  ss.Update();

  // Update velocity of the spaceship.
  if (UpKeyPress) {
    ss.velocity_.x -= sin(ss.direction_ / 360.0f * 2 * M_PI);
    ss.velocity_.y += cos(ss.direction_ / 360.0f * 2 * M_PI);
  }
  if (DownKeyPress) {
    ss.velocity_.x += sin(ss.direction_ / 360.0f * 2 * M_PI);
    ss.velocity_.y -= cos(ss.direction_ / 360.0f * 2 * M_PI);
  }

  if (ss.velocity_.x > MaxVelocity) {
    ss.velocity_.x = MaxVelocity;
  } else if (ss.velocity_.x < -MaxVelocity) {
    ss.velocity_.x = -MaxVelocity;
  }

  if (ss.velocity_.y > MaxVelocity) {
    ss.velocity_.y = MaxVelocity;
  } else if (ss.velocity_.y < -MaxVelocity) {
    ss.velocity_.y = -MaxVelocity;
  }

  if (LeftKeyPress) {
    ss.direction_ += 10.0f;
  }
  if (RightKeyPress) {
    ss.direction_ -= 10.0f;
  }

  // Shooting.
  if (ShootKeyPress) {
    Bullet b(&game);
    if (ss.Shoot(&b)) {
      game.bullets_.push_back(b);
    }
  }

  // Update asteroids position.
  for (size_t i = 0; i < game.asteroids_.size(); ++i) {
    Asteroid& a = game.asteroids_[i];
    a.position_.x = a.position_.x + a.velocity_.x;
    a.position_.y = a.position_.y + a.velocity_.y;
    a.direction_ += a.rotation_speed_;

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
        if (distance(a.position_, b.position_) >
            distance(a.position_ + a.velocity_, b.position_ + b.velocity_)) {
          a.velocity_.x *= -1.0f;
          a.velocity_.y *= -1.0f;
          b.velocity_.x *= -1.0f;
          b.velocity_.y *= -1.0f;
        }
      }
    }
  }

  // Update positions of bullets.
  for (size_t i = 0; i < game.bullets_.size(); ++i) {
    Bullet& b = game.bullets_[i];
    b.position_.x += b.velocity_.x;
    b.position_.y += b.velocity_.y;
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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(PanelWidth, PanelHeight);
  glutCreateWindow("Asteroids");

  glutDisplayFunc(renderScene);
  glutTimerFunc(CallbackTime, timerCallback, 0);
  glutKeyboardFunc(processKeys);
  glutKeyboardUpFunc(processKeysUp);
  glutSpecialFunc(processSpecialKeys);
  glutSpecialUpFunc(processSpecialKeysUp);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, PanelWidth, 0, PanelHeight);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  game.panel_width_ = PanelWidth;
  game.panel_height_ = PanelHeight;

  glutMainLoop();

  return 0;
}
