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

#include "Segment.h"
#include "IKMath.h"

// Root of an arm. Does nothing
class Root : public Segment {
public:
	Root(void);
	Root(vec3 pos);
	void render();
	void ik(std::string mode);
};