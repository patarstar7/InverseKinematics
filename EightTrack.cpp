#include "EightTrack.h"

EightTrack::EightTrack(vec3 center, int orientation) {
	a = 1.2;
	angle = 0.0;
	angleSpd = 0.007;
	angleDirection = 1;
	side = 0;
	this->orientation = orientation;
	this->center = center;

	int tempSide = 0;
	int tempDirection = 1;
	double tempAngle = 0.0;
	int notDone = 1;
	while(notDone) {
		double a2 = pow(a, 2);
		double cosTerm = cos(2 * tempAngle);
		double secTerm = pow(cos(tempAngle), 4);
		double r2 = a2 * cosTerm * secTerm;
		double r = sqrt(r2);

		vec3 point = getPoint(tempAngle, tempSide, r);
		vertices.push_back(point);

		tempAngle += ((double) tempDirection) * angleSpd;

		if(tempAngle > pi / 4 || tempAngle < 0.0) {
			tempDirection *= -1;
			tempAngle += angleSpd * ((double) tempDirection);
			tempSide++;
			if(tempSide > 3)
				// tempSide = 0;
				notDone = !notDone;
		}
	}
}

vec3 EightTrack::getPoint(double angle, int side, double r) {
	vec3 point = vec3();
	if(orientation == 2) {
		point[VX] = center[VX];

		point[VZ] = r * cos(angle);
		if(side == 1 || side == 2)
			point[VZ] *= -1;
		point[VZ] += center[VZ];

		point[VY] = r * sin(angle);
		if(side == 1 || side == 3)
			point[VY] *= -1;
		point[VY] += center[VY];
	} else if(orientation == 1) {
		point[VX] = r * cos(angle);
		if(side == 1 || side == 2)
			point[VX] *= -1;
		point[VX] += center[VX];

		point[VY] = r * sin(angle);
		if(side == 1 || side == 3)
			point[VY] *= -1;
		point[VY] += center[VY];

		point[VZ] = center[VZ];
	} else {
		point[VX] = r * cos(angle);
		if(side == 1 || side == 2)
			point[VX] *= -1;
		point[VX] += center[VX];

		point[VY] = center[VY];

		point[VZ] = r * sin(angle);
		if(side == 1 || side == 3)
			point[VZ] *= -1;
		point[VZ] += center[VZ];
	}
	return point;
}

vec3 EightTrack::next() {
	double a2 = pow(a, 2);
	double cosTerm = cos(2 * angle);
	double secTerm = pow(cos(angle), 4);
	double r2 = a2 * cosTerm * secTerm;
	double r = sqrt(r2);

	vec3 point = getPoint(angle, side, r);

	angle += ((double) angleDirection) * angleSpd;

	if(angle > pi / 4 || angle < 0.0) {
		angleDirection *= -1;
		angle += angleSpd * ((double) angleDirection);
		side++;
		if(side > 3)
			side = 0;
	}

	return point;
}

void EightTrack::render() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < vertices.size(); i++) {
		glVertex3d(vertices[i][VX], vertices[i][VY], vertices[i][VZ]);
	}
	glEnd();
}