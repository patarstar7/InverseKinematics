#pragma once

#include "TargetTrack.h"

class SpiralTrack : public TargetTrack {
private:
	double spRad;
	double minHeight;
	double currY;
	double sAngRate;
	double yRate;
	double sAng;
	double maxSpiral;
public:
	SpiralTrack(void);
	SpiralTrack(double radius, int max);
	vec3 next();
	void render();
};