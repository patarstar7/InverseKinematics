#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <cmath>
#include "algebra3.h"

class TargetTrack {
public:
	double pi;
	int active;
	TargetTrack(void);
	int toggleActive();
	int isActive();
	virtual vec3 next()=0;
	virtual void render()=0;
};