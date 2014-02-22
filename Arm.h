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

#include <iostream>
#include <vector>
#include <cmath>

#include "algebra3.h"
#include "Segment.h"
#include "BallSegment.h"
#include "PrismSegment.h"
#include "Root.h"

class Arm {
private:
	// vector<Segment*> segments;
	// vector<Segment*> endEffectorSegments;
	Root* root;

public:
	Arm(Root* root);

	// void addSegment(Segment* seg);
	void render();
	void ik(std::string mode);
};