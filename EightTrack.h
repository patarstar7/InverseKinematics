#pragma once

#include "TargetTrack.h"
#include <vector>

class EightTrack : public TargetTrack {
private:
	double a;
	double angle;
	double angleSpd;
	int angleDirection;
	int side;
	int count;
	vec3 center;
	int orientation;
	vector<vec3> vertices;
	vec3 getPoint(double angle, int side, double r);
public:
	EightTrack(vec3 center, int orientation);
	vec3 next();
	void render();
};