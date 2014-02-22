#include "Root.h"

Root::Root(void) {
	length = 0;
	position = vec3(0.0, 0.0, 0.0);
	direction = vec3(0.0, 0.0, 0.0);
}

Root::Root(vec3 pos) {
	length = 0;
	position = pos;
	direction = vec3(0.0, 0.0, 0.0);
}

void Root::render() {
    glPointSize(11.0f);
	glColor3f(0.0f, 0.3f, 1.0f);
	glBegin(GL_POINTS);
    glVertex3d(position[VX], position[VY], position[VZ]);
	glEnd();

	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->render();
	}
}

void Root::ik(std::string mode) {
	for(int i = 0; i < nextSegments.size(); i++) {
		nextSegments[i]->ik(mode);
	}
}