#include "Segment.h"

void Segment::addNextSegment(Segment* next) {
	nextSegments.push_back(next);
}

void Segment::setTarget(vec3* t) {
	target = t;
}

void Segment::rotate(vec3 rotation) {

	double** xMatrix = new double*[3];
	xMatrix[0] = new double[3];
	xMatrix[1] = new double[3];
	xMatrix[2] = new double[3];

	double** yMatrix = new double*[3];
	yMatrix[0] = new double[3];
	yMatrix[1] = new double[3];
	yMatrix[2] = new double[3];

	double** zMatrix = new double*[3];
	zMatrix[0] = new double[3];
	zMatrix[1] = new double[3];
	zMatrix[2] = new double[3];

	initializeRotationMatrices(xMatrix, yMatrix, zMatrix, rotation[VX], rotation[VY], rotation[VZ]);

	double** rotationMatrix = new double*[3];
	rotationMatrix[0] = new double[3];
	rotationMatrix[1] = new double[3];
	rotationMatrix[2] = new double[3];

	double** tempMatrix = new double*[3];
	tempMatrix[0] = new double[3];
	tempMatrix[1] = new double[3];
	tempMatrix[2] = new double[3];

	matMult(xMatrix, yMatrix, tempMatrix, 3, 3, 3);
	matMult(tempMatrix, zMatrix, rotationMatrix, 3, 3, 3);

	double** current = new double*[3];
	current[0] = new double[1]; current[0][0] = direction[VX];
	current[1] = new double[1]; current[1][0] = direction[VY];
	current[2] = new double[1]; current[2][0] = direction[VZ];

	double** result = new double*[3];
	result[0] = new double[1];
	result[1] = new double[1];
	result[2] = new double[1];

	matMult(rotationMatrix, current, result, 3, 3, 1);

	vec3 dir = vec3(result[0][0], result[1][0], result[2][0]);
	dir.normalize(); // to be extra safe
	
	direction = dir;

	delete[] xMatrix[0]; delete[] xMatrix[1]; delete[] xMatrix[2]; delete[] xMatrix;
	delete[] yMatrix[0]; delete[] yMatrix[1]; delete[] yMatrix[2]; delete[] yMatrix;
	delete[] zMatrix[0]; delete[] zMatrix[1]; delete[] zMatrix[2]; delete[] zMatrix;
	delete[] rotationMatrix[0]; delete[] rotationMatrix[1]; delete[] rotationMatrix[2]; delete[] rotationMatrix;
	delete[] tempMatrix[0]; delete[] tempMatrix[1]; delete[] tempMatrix[2]; delete[] tempMatrix;
	delete[] current[0]; delete[] current[1]; delete[] current[2]; delete[] current;
	delete[] result[0]; delete[] result[1]; delete[] result[2]; delete[] result;
}

void Segment::rotateAll(vec3 rotation) {
	rotate(rotation);
	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->movePosition(vec3(endPosition()));
		nextSegments[i]->rotateAll(rotation);
	}
}

void Segment::movePosition(vec3 pos) {
	position = pos;
}

vec3 Segment::endPosition() {
	if(length == 0.0)
		return position;
	return vec3(position + (direction * length));
}

void Segment::getEndEffectors(vector<vec3> &endEffectors) {
	int segs = nextSegments.size();
	if(segs == 0)
		endEffectors.push_back(vec3(endPosition()));
	for(int i = 0; i < segs; i++) {
		nextSegments[i]->getEndEffectors(endEffectors);
	}
}

void Segment::getTargets(vector<vec3> &targets) {
	int segs = nextSegments.size();
	if(segs == 0) {
		// cout<<*target<<endl;
		targets.push_back(vec3(*target));
		// cout << targets.back() << endl;
	}
	for(int i = 0; i < segs; i++) {
		nextSegments[i]->getTargets(targets);
	}
}