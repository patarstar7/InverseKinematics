#include "SpiralTrack.h"

SpiralTrack::SpiralTrack(void) {
	spRad = 1.2;
	minHeight = 0.5;
	currY = minHeight;
	sAngRate = 0.05;
	yRate = sAngRate / 20.0;
	sAng = 0.0;
	maxSpiral = 12 * pi;
}

SpiralTrack::SpiralTrack(double radius, int max) {
	spRad = radius;
	minHeight = 0.5;
	currY = minHeight;
	sAngRate = 0.05;
	yRate = sAngRate / 20.0;
	sAng = 0.0;
	maxSpiral = ((double) max) * pi;
}

vec3 SpiralTrack::next() {
	vec3 next = vec3();
	next[VX] = spRad * cos(sAng);
	next[VZ] = spRad * sin(sAng);
	next[VY] = currY;

	sAng += sAngRate;
	currY += yRate;

	if(sAng > maxSpiral || sAng < 0.0) {
		sAngRate *= -1;
		yRate *= -1;
	}
	return next;
}

void SpiralTrack::render() {
	double x;
	double y = minHeight;
	double z;
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for(double t = 0.0; t < maxSpiral; t+= abs(sAngRate)) {
		x = spRad * cos(t);
		z = spRad * sin(t);
		glVertex3d(x, y, z);
		y += abs(yRate);
	}
	glEnd();
}