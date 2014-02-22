#include "TargetTrack.h"

TargetTrack::TargetTrack(void) {
	pi = 3.14159265359;
	active = 0;
}

int TargetTrack::toggleActive() {
	active = !active;
	return active;
}

int TargetTrack::isActive() {
	return active;
}