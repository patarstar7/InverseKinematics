#pragma once;

#include <cmath>
#include "algebra3.h"

double dist(vec3 a, vec3 b);
void matMult(double** a, double** b, double** c, int m, int n, int p);
void initializeRotationMatrices(double** xMatrix, double** yMatrix, double** zMatrix, double thetaX, double thetaY, double thetaZ);
void transpose(double** matrix, double** matrixT, double m, double n);
void pseudoOneSided(double** matrix, double** inverse, int m, int n);
void pseudoinverse(double** matrix, double** inverse, double m, double n);

double determinant3x3(double** matrix);
void invert3x3(double** matrix, double** inverse);
void printMatrix(double** matrix, int m, int n);