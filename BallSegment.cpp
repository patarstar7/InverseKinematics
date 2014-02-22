#include "BallSegment.h"

BallSegment::BallSegment(double length, vec3 position) {
	this->length = length;
	this->position = position;
	this->direction = vec3(0.0, 1.0, 0.0);
}

void BallSegment::render() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.2f, 0.6f, 0.0f);
    glLineWidth(4.0f);

    glBegin(GL_LINE_STRIP);
	glVertex3d(position[VX], position[VY], position[VZ]);
	vec3 end = endPosition();
	glVertex3d(end[VX], end[VY], end[VZ]);
    glEnd();

    glPointSize(11.0f);
	glColor3f(0.0f, 0.3f, 1.0f);
	glBegin(GL_POINTS);
    glVertex3d(end[VX], end[VY], end[VZ]);
	glEnd();

	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->render();
	}
}

void BallSegment::ik(std::string mode) {
	if(mode == "ccd") {
		for(int i = 0; i < nextSegments.size(); i++) {
			nextSegments[i]->ik(mode);
		}
	}

	vector<vec3> endEffectors;
	vector<vec3> targets;
	getEndEffectors(endEffectors);
	getTargets(targets);

	vec3 deltaTheta = vec3(0, 0, 0);

	int ee = endEffectors.size();
	int t = targets.size();
	if(ee != t)
		cout << "ERROR endEffectors size != targets size" << endl;

	for(int i = 0; i < ee; i++) {
		deltaTheta += ikHelper(endEffectors[i], targets[i], mode);
	}

	deltaTheta /= (double) ee;


	// Move everything for this rotation
	rotateAll(deltaTheta);

	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->movePosition(endPosition());
		if(mode == "transpose")
			nextSegments[i]->ik(mode);
	}
}

vec3 BallSegment::ikHelper(vec3 endEffector, vec3 pos, std::string mode) {
	// pos is the target

	if(mode == "ccd") {
		vec3 deltaTheta = vec3(0, 0, 0);

		vec3 vec = vec3(pos - position);
		vec3 dir = vec3(endEffector - position);

		// cout << "vec:  " << vec << endl;
		// cout << "dir:  " << dir << endl;
		// cout << "pos:  " << pos << endl;
		// cout << "position:  " << position << endl;
		// cout << "ee:  " << endEffector << endl;

		vec3 xVec = vec3(0.0, vec[VY], vec[VZ]); if(xVec.length() > 0) xVec.normalize();
		vec3 xDir = vec3(0.0, dir[VY], dir[VZ]); if(xDir.length() > 0) xDir.normalize();

		vec3 yVec = vec3(vec[VX], 0.0, vec[VZ]); if(yVec.length() > 0) yVec.normalize();
		vec3 yDir = vec3(dir[VX], 0.0, dir[VZ]); if(yDir.length() > 0) yDir.normalize();

		vec3 zVec = vec3(vec[VX], vec[VY], 0.0); if(zVec.length() > 0) zVec.normalize();
		vec3 zDir = vec3(dir[VX], dir[VY], 0.0); if(zDir.length() > 0) zDir.normalize();

		vec3 xCross = xDir ^ xVec;
		vec3 yCross = yDir ^ yVec;
		vec3 zCross = zDir ^ zVec;

		// cout << "yVec:    " << yVec << endl << "yVec length:   " << yVec.length() << endl;
		// cout << "yDir:    " << yDir << endl << "yDir length:   " << yDir.length() << endl;
		// cout << "yCross:    " << yCross << endl << "yCross length:   " << yCross.length() << endl << endl;

		deltaTheta[VX] = asin(xCross.length());
		deltaTheta[VY] = asin(yCross.length());
		deltaTheta[VZ] = asin(zCross.length());

		if(xCross[VX] < 0)
			deltaTheta[VX] *= -1.0;
		if(yCross[VY] < 0)
			deltaTheta[VY] *= -1.0;
		if(zCross[VZ] < 0)
			deltaTheta[VZ] *= -1.0;

		return deltaTheta;
	}

	// deltaTheta = SVD(jacobian) * (target config - current config)
	vec3 deltaTheta = vec3();

	// e = target - current
	vec3 e = pos - endEffector;

	// set step size
	// double step = 0.02;
	// if(e.length() > step) {
	// 	e = e.normalize() * step;
	// }

	// build the jacobian
	double **jacobian = new double*[3];
	jacobian[0] = new double[3];
	jacobian[1] = new double[3];
	jacobian[2] = new double[3];

	vec3 x = vec3(1.0, 0.0, 0.0);
	vec3 y = vec3(0.0, 1.0, 0.0);
	vec3 z = vec3(0.0, 0.0, 1.0);

	// vec3 x = vec3(1.0, 0.1, 0.1);
	// vec3 y = vec3(0.1, 1.0, 0.1);
	// vec3 z = vec3(0.1, 0.1, 1.0);

	vec3 dedx = x ^ (endEffector - position);
	vec3 dedy = y ^ (endEffector - position);
	vec3 dedz = z ^ (endEffector - position);

	jacobian[0][0] = dedx[VX]; jacobian[0][1] = dedy[VX]; jacobian[0][2] = dedz[VX];
	jacobian[1][0] = dedx[VY]; jacobian[1][1] = dedy[VY]; jacobian[1][2] = dedz[VY];
	jacobian[2][0] = dedx[VZ]; jacobian[2][1] = dedy[VZ]; jacobian[2][2] = dedz[VZ];

	// cout<<"jacobian: "<<endl;
	// printMatrix(jacobian, 3, 3);

	// for now, use transpose of jacobian instead of pseudoinverse
	double **jT = new double*[3];
	for(int i = 0; i < 3; i++) {
		jT[i] = new double[3];
	}

	if(mode == "pseudo") {
		pseudoOneSided(jacobian, jT, 3, 3);
	} else {
		transpose(jacobian, jT, 3, 3);	
	}

	// cout<<"jT:"<<endl;
	// cout<<jT[0][0]<<" "<<jT[0][1]<<" "<<jT[0][2]<<endl;
	// cout<<jT[1][0]<<" "<<jT[1][1]<<" "<<jT[1][2]<<endl;
	// cout<<jT[2][0]<<" "<<jT[2][1]<<" "<<jT[2][2]<<endl;
	// cout<<endl<<endl<<endl;

	// deltaTheta = jacobian^T * e
	double **eMat = new double*[3];
	eMat[0] = new double[1]; eMat[0][0] = e[0];
	eMat[1] = new double[1]; eMat[1][0] = e[1];
	eMat[2] = new double[1]; eMat[2][0] = e[2];

	double **result = new double*[3];
	result[0] = new double[1];
	result[1] = new double[1];
	result[2] = new double[1];
	
	matMult(jT, eMat, result, 3, 3, 1);

	// move everything from result to deltaTheta
	deltaTheta[VX] = result[0][0];
	deltaTheta[VY] = result[1][0];
	deltaTheta[VZ] = result[2][0];

	// for(int i = 0; i < segments; i++) {
	// 	cout << "move joint " << i << " by    xy: " << deltaThetaXY[i] << 
		// "   yz: " << deltaThetaYZ[i] << "   zx: " << deltaThetaZX[i] << endl;
	// }

	// scale deltaTheta. scale grows exponentially as the e approaches the target
	// so the arm's movement doesn't slow down
	double elen = e.length();
	double scale = 0.01;
	if(elen < 1.2)
		scale = pow(elen - 1.2, 2) * (0.2 / pow(1.2, 2)) + 0.01;
		// scale = (-0.19/1.2) * elen + 0.2;
	// double scale = 0.1;
	// if(e.length() > 1.2)
	// 	scale = 0.02;
	// if(e.length() < 0.5)
	// 	scale = 0.2;
	deltaTheta[VX] *= scale;
	deltaTheta[VY] *= scale;
	deltaTheta[VZ] *= scale;

	// clean up jacobian and jT arrays
	delete[] jacobian[0]; delete[] jacobian[1]; delete[] jacobian[2];
	delete[] eMat[0]; delete[] eMat[1]; delete[] eMat[2];
	delete[] jT[0]; delete[] jT[1]; delete[] jT[2];
	delete[] result[0]; delete[] result[1]; delete[] result[2];

	delete[] jacobian;
	delete[] jT;
	delete[] result;
	delete[] eMat;

	return deltaTheta;
}