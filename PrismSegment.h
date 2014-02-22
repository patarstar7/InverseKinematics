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

// Segment with a prismatic joint
class PrismSegment : public Segment {
private:
	double maxLength;
	double minLength;
public:
	PrismSegment(double length, vec3 position);
	void render();
	void ik(std::string mode);
	double ikHelper(vec3 endEffector, vec3 pos, std::string mode);
};