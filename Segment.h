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
#include "IKMath.h"

// Segment represents a joint and the attached segment of an arm.
class Segment {
public:
	// Basic info
	double length;
	vec3 direction;
	vec3 position;

	// Array of pointers to this segment's next segments
	vector<Segment*> nextSegments;

	// If this segment has an end effector attached to it, then this points to the end effector's target.
	// Constraint: This must point to a vec3 if nextSegments is empty, and should be ignored if nextSegments
	// is non-empty.
	vec3 *target;

	// Render the segment
	virtual void render()=0;

	// Do ik for the target position pos.
	virtual void ik(std::string mode)=0;

	// Add an element to nextSegments
	void addNextSegment(Segment* next);

	// Set target of this segment, must follow the constraints outlined for the target field.
	void setTarget(vec3* t);

	// Rotate the segment. rotation parameter is [angle about x-axis, angle about y-axis, angle about z-axis].
	void rotate(vec3 rotation);

	// Rotate the segment and all segments attached to it
	void rotateAll(vec3 rotation);

	// Moves the position to the argument coordinates
	void movePosition(vec3 pos);

	// Get the position of the end of this segment
	vec3 endPosition();

	// Get the positions of the end effectors on this segment's branch.
	void getEndEffectors(vector<vec3> &endEffectors);

	// Get the positions of the targets of this segment's branch.
	void getTargets(vector<vec3> &targets);
};