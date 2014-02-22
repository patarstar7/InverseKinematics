#pragma once

#include "TargetTrack.h"

class CircleTrack : public TargetTrack {
private:
	double radius;
	double angleSpeed;
	double currentAngle;
	vec3 center;
	int orientation; // 0 means it's in the xz plane, 1 in xy plane, 2 in yz plane
	int active;
	vec3 getPoint(double angle);
public:
	CircleTrack(void);
	CircleTrack(vec3 center, double radius, int orientation);
	vec3 next();
	void render();
};