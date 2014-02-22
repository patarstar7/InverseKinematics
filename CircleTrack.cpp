#include "CircleTrack.h"

CircleTrack::CircleTrack(void) {
  center = vec3(0.0, 1.2, 0.0);
  radius = 1.2;
  angleSpeed = 0.02;
  currentAngle = 0.0;
  orientation = 0;
}

CircleTrack::CircleTrack(vec3 center, double radius, int orientation) {
  this->center = center;
  this->radius = radius;
  this->orientation = orientation;
  angleSpeed = 0.02;
  currentAngle = 0.0;
}

vec3 CircleTrack::getPoint(double angle) {
  vec3 point = vec3();
  if(orientation == 2) {
    point[VX] = center[VX];
    point[VY] = center[VY] + radius * cos(angle);
    point[VZ] = center[VZ] + radius * sin(angle);
  } else if(orientation == 1) {
    point[VX] = center[VX] + radius * cos(angle);
    point[VY] = center[VY] + radius * sin(angle);
    point[VZ] = center[VZ];
  } else {
    point[VX] = center[VX] + radius * cos(angle);
    point[VY] = center[VY];
    point[VZ] = center[VZ] + radius * sin(angle);
  }
  return point;
}

void CircleTrack::render() {
  glColor3f(1.0f, 1.0f, 1.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINE_STRIP);

  for (double i = 0; i < 2 * pi; i+=0.3) {
    vec3 point = getPoint(i);
    glVertex3d(point[VX], point[VY], point[VZ]);
  }
  vec3 point = getPoint(0.0);
  glVertex3d(point[VX], point[VY], point[VZ]);
  
  glEnd();
}

vec3 CircleTrack::next() {
  vec3 next = getPoint(currentAngle);
  currentAngle += angleSpeed;
  if(currentAngle > 2 * pi)
    currentAngle = 0.0;
  return next;
}