#include "PrismSegment.h"

PrismSegment::PrismSegment(double length, vec3 position) {
	this->length = 0.1;
	this->minLength = 0.1;
	this->maxLength = length;
	this->position = position;
	this->direction = vec3(0.0, 1.0, 0.0);
}

void PrismSegment::render() {
	vec3 end = endPosition();

	glPointSize(14.0f);
	glColor3f(0.0f, 0.3f, 1.0f);
	glBegin(GL_POINTS);
    glVertex3d(end[VX], end[VY], end[VZ]);
	glEnd();

	// Make line dashed
	glPushAttrib(GL_ENABLE_BIT); 
	glLineStipple(1, 0xFFAA);
	glEnable(GL_LINE_STIPPLE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0f, 0.5f, 0.0f);
    glLineWidth(4.0f);

    glBegin(GL_LINE_STRIP);
	glVertex3d(position[VX], position[VY], position[VZ]);
	glVertex3d(end[VX], end[VY], end[VZ]);
    glEnd();

    glDisable(GL_LINE_STIPPLE);

    for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->render();
	}
}

void PrismSegment::ik(std::string mode) {
	if(mode == "ccd") {
		for(int i = 0; i < nextSegments.size(); i++) {
			nextSegments[i]->ik(mode);
		}
	}

	vector<vec3> endEffectors;
	vector<vec3> targets;
	getEndEffectors(endEffectors);
	getTargets(targets);

	// vec3 deltaTheta = vec3(0, 0, 0);

	int ee = endEffectors.size();
	int t = targets.size();
	if(ee != t) {
		cout << "ERROR endEffectors size != targets size" << endl;
	}

	double movement = 0.0;
	for(int i = 0; i < ee; i++) {
		// deltaTheta += ikHelper(endEffectors[i], targets[i], mode);
		movement += ikHelper(endEffectors[i], targets[i], mode);
		// cout << "endEffectors[" << i << "]: " << endEffectors[i] << endl;
		// cout << "targets[" << i << "]: " << targets[i] << endl << endl;
	}
	movement /= (double) ee;

	// Move everything for this rotation
	length += movement;
	if(length > maxLength)
		length = maxLength;
	if(length < minLength)
		length = minLength;

	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->movePosition(endPosition());
		if(mode == "transpose")
			nextSegments[i]->ik(mode);
	}
}

double PrismSegment::ikHelper(vec3 endEffector, vec3 pos, std::string mode) {

	// e = target - current
	vec3 e = pos - endEffector;


	if(mode == "ccd") {
		double deltaLength = e * direction;
		return deltaLength;
	}

	// build the jacobian
	double **jacobian = new double*[3];
	jacobian[0] = new double[1];
	jacobian[1] = new double[1];
	jacobian[2] = new double[1];

	vec3 dedl = direction;

	jacobian[0][0] = dedl[VX];
	jacobian[1][0] = dedl[VY];
	jacobian[2][0] = dedl[VZ];

	// printMatrix(jacobian, 3, 1);

	// for now, use transpose of jacobian instead of pseudoinverse
	double **jT = new double*[1];
	jT[0] = new double[3];
	transpose(jacobian, jT, 3, 1);

	// printMatrix(jT, 1, 3);

	// deltaTheta = jacobian^T * e
	double **eMat = new double*[3];
	eMat[0] = new double[1]; eMat[0][0] = e[0];
	eMat[1] = new double[1]; eMat[1][0] = e[1];
	eMat[2] = new double[1]; eMat[2][0] = e[2];

	double **result = new double*[1];
	result[0] = new double[1];
	
	matMult(jT, eMat, result, 1, 3, 1);


	// Move the arm
	
	double increment = result[0][0];
	double speed = 0.003;
	if(abs(increment) < speed)
		increment *= 0.5;
	else
		increment = speed * (increment / abs(increment));

	if(length > maxLength)
		length = maxLength;
	if(length < minLength)
		length = minLength;

	// clean up jacobian and jT arrays
	delete[] jacobian[0]; delete[] jacobian[1]; delete[] jacobian[2];
	delete[] eMat[0]; delete[] eMat[1]; delete[] eMat[2];
	delete[] jT[0];
	delete[] result[0];

	delete[] jacobian;
	delete[] jT;
	delete[] result;
	delete[] eMat;

	return increment;
}
