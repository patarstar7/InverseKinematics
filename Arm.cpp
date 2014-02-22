#include "Arm.h"

Arm::Arm(Root* root) {
	this->root = root;
	// segments.push_back(root);
}

// void Arm::addSegment(Segment* seg) {
// 	segments.push_back(seg);
// }

void Arm::render() {
	root->render();
}

void Arm::ik(std::string mode) {
	root->ik(mode);
}

// #include "Arm.h"

// Arm::Arm(vec3 root, vector<double> links) {
// 	numSegments = links.size();

// 	segments = new Segment*[numSegments+1];
// 	segments[0] = new Root(root);

// 	for(int i = 0; i < numSegments; i++) {
// 		double l = links[i];
// 		vec3 pos = segments[i]->endPosition();
// 		if(l > 0.0) {
// 			segments[i+1] = new BallSegment(l, pos);
// 		} else if (l < 0.0) {
// 			segments[i+1] = new PrismSegment(-l, pos);
// 		}
// 	}

// 	for(int i = 0; i < numSegments + 1; i++) {
// 		segments[i]->setEndEffector(getEndEffector());
// 	}
// }

// vec3 Arm::getEndEffector() {
// 	// cout<<segments[numSegments]->position<<endl;
// 	vec3 e = segments[numSegments]->endPosition();
// 	// cout << "getEndEffector returns: " << e << endl;
// 	return e;
// }

// void Arm::render() {
// 	for(int i = 0; i < numSegments + 1; i++) {
// 		segments[i]->render();
// 	}
// }

// void Arm::ik(vec3 pos, std::string mode) {
// 	if(mode == "transpose" || mode == "pseudo" || mode == "") {
// 		vec3 cumulativeRotate = vec3(0, 0, 0);
// 		for(int i = 0; i < numSegments + 1; i++) {
// 			// cout << "joint " << i << endl;
// 			vec3 r = segments[i]->ik(pos, mode);
// 			// cout << "r" << i << ":  " << r << endl << endl;
// 			cumulativeRotate = cumulativeRotate + r;
// 			segments[i]->rotate(cumulativeRotate);
// 			if(i < numSegments)
// 				segments[i+1]->movePosition(segments[i]->endPosition());
// 		}
// 		vec3 ee = getEndEffector();
// 		for(int i = 0; i < numSegments + 1; i++) {
// 			segments[i]->setEndEffector(ee);
// 		}
// 	} else if(mode == "ccd") {
// 		for(int i = numSegments; i > 0; i--) {
// 			vec3 r = segments[i]->ik(pos, mode);
// 			segments[i]->rotate(r);
// 			if(i < numSegments)
// 				segments[i+1]->movePosition(segments[i]->endPosition());
// 			for(int j = i + 1; j < numSegments + 1; j++) {
// 				segments[j]->rotate(r);
// 				if(j < numSegments)
// 					segments[j+1]->movePosition(segments[j]->endPosition());
// 			}
// 			vec3 ee = getEndEffector();
// 			for(int i = 0; i < numSegments + 1; i++) {
// 				segments[i]->setEndEffector(ee);
// 			}
// 		}
// 	}
// }