#include "IKMath.h"

double dist(vec3 a, vec3 b) {
	return sqrt(pow(a[VX] - b[VX], 2) + pow(a[VY] - b[VY], 2) + pow(a[VZ] - b[VZ], 2));
}

void matMult(double** a, double** b, double** c, int m, int n, int p) {
	for(int i = 0; i < m; i++) {
		for(int j = 0; j < p; j++)
			c[i][j] = 0;
	}

	for(int i = 0; i < m; i++) {
		for(int j = 0; j < p; j++) {
			c[i][j] = 0.0;
			for(int k = 0; k < n; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

void initializeRotationMatrices(double** xMatrix, double** yMatrix, double** zMatrix, 
	double thetaX, double thetaY, double thetaZ) {

	xMatrix[0][0] = 1.0; xMatrix[0][1] = 0.0; xMatrix[0][2] = 0.0;
	xMatrix[1][0] = 0.0; xMatrix[1][1] = cos(thetaX); xMatrix[1][2] = -sin(thetaX);
	xMatrix[2][0] = 0.0; xMatrix[2][1] = sin(thetaX); xMatrix[2][2] = cos(thetaX);

	yMatrix[0][0] = cos(thetaY); yMatrix[0][1] = 0.0; yMatrix[0][2] = sin(thetaY);
	yMatrix[1][0] = 0.0; yMatrix[1][1] = 1.0; yMatrix[1][2] = 0.0;
	yMatrix[2][0] = -sin(thetaY); yMatrix[2][1] = 0.0; yMatrix[2][2] = cos(thetaY);

	zMatrix[0][0] = cos(thetaZ); zMatrix[0][1] = -sin(thetaZ); zMatrix[0][2] = 0.0;
	zMatrix[1][0] = sin(thetaZ); zMatrix[1][1] = cos(thetaZ); zMatrix[1][2] = 0.0;
	zMatrix[2][0] = 0.0; zMatrix[2][1] = 0.0; zMatrix[2][2] = 1.0;
}

void transpose(double** matrix, double** matrixT, double m, double n) {
	for(int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			matrixT[j][i] = matrix[i][j];
		}
	}
}

void pseudoOneSided(double** matrix, double** inverse, int m, int n) {

	// if(abs(determinant3x3(matrix)) < 0.0001) {
	// 	transpose(matrix, inverse, m, n);
	// 	cout<<"!!!!!!!!!!!!!!!!!!!!transposed instead!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	// 	return;
	// }

	double** mT = new double*[n];
	for(int i = 0; i < n; i++)
		mT[i] = new double[m];
	transpose(matrix, mT, m, n);

	// cout<<"mT:"<<endl;
	// cout<<mT[0][0]<<" "<<mT[0][1]<<" "<<mT[0][2]<<endl;
	// cout<<mT[1][0]<<" "<<mT[1][1]<<" "<<mT[1][2]<<endl;
	// cout<<mT[2][0]<<" "<<mT[2][1]<<" "<<mT[2][2]<<endl;
	// cout<<endl<<endl<<endl;

	double** mTm = new double*[n];
	for(int i = 0; i < n; i++)
		mTm[i] = new double[n];
	matMult(mT, matrix, mTm, n, m, n);

	// cout<<"mTm:"<<endl;
	// cout<<mTm[0][0]<<" "<<mTm[0][1]<<" "<<mTm[0][2]<<endl;
	// cout<<mTm[1][0]<<" "<<mTm[1][1]<<" "<<mTm[1][2]<<endl;
	// cout<<mTm[2][0]<<" "<<mTm[2][1]<<" "<<mTm[2][2]<<endl;
	// cout<<endl<<endl<<endl;

	double** mTmi = new double*[n];
	for(int i = 0; i < n; i++)
		mTmi[i] = new double[n];

	if(determinant3x3(mTm) == 0.0) {
		// cout<<"!!!!!!!!!!!!!!!!!!!!transposed instead!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
		transpose(mTm, mTmi, n, n);
	} else {
		invert3x3(mTm, mTmi);
	}

	// cout<<"mTmi:"<<endl;
	// cout<<mTmi[0][0]<<" "<<mTmi[0][1]<<" "<<mTmi[0][2]<<endl;
	// cout<<mTmi[1][0]<<" "<<mTmi[1][1]<<" "<<mTmi[1][2]<<endl;
	// cout<<mTmi[2][0]<<" "<<mTmi[2][1]<<" "<<mTmi[2][2]<<endl;
	// cout<<endl<<endl<<endl;

	matMult(mTmi, mT, inverse, n, n, m);

	// cout<<"inverse:"<<endl;
	// cout<<inverse[0][0]<<" "<<inverse[0][1]<<" "<<inverse[0][2]<<endl;
	// cout<<inverse[1][0]<<" "<<inverse[1][1]<<" "<<inverse[1][2]<<endl;
	// cout<<inverse[2][0]<<" "<<inverse[2][1]<<" "<<inverse[2][2]<<endl;
	// cout<<endl<<endl<<endl;

	for(int i = 0; i < n; i++) {
		delete[] mT[i];
		delete[] mTm[i];
		delete[] mTmi[i];
	}
	delete[] mT; delete[] mTm; delete[] mTmi;

	cout<<"******************************************************************"<<endl;
}

void pseudoinverse(double** matrix, double** inverse, double m, double n) {

}

double determinant3x3(double** matrix) {
	double a = matrix[0][0]; double b = matrix[0][1]; double c = matrix[0][2];
	double d = matrix[1][0]; double e = matrix[1][1]; double f = matrix[1][2];
	double g = matrix[2][0]; double h = matrix[2][1]; double k = matrix[2][2];

	// double det = a*(e*k-f*h);
	// det -= b*(k*d-f*g);
	// det += c*(d*h-e*g);

	double d1 = e*k; d1 -= f*h; d1 *= a; 
	double d2 = k*d; d2 -= f*g; d2 *= b;
	double d3 = d*h; d3 -= e*g; d3 *= c;

	double det = d1 - d2 + d3;

	// cout<<endl<<"d1   "<<d1<<endl;
	// cout<<"d2   "<<d2<<endl;
	// cout<<"d3   "<<d3<<endl;
	// cout<<"det   "<<det<<endl<<endl<<endl;

	return det;
}

void invert3x3(double** matrix, double** inverse) {
	double a = matrix[0][0]; double b = matrix[0][1]; double c = matrix[0][2];
	double d = matrix[1][0]; double e = matrix[1][1]; double f = matrix[1][2];
	double g = matrix[2][0]; double h = matrix[2][1]; double k = matrix[2][2];

	inverse[0][0]=e*k; inverse[0][0] -=f*h;
	inverse[0][1]=-b*k; inverse[0][1] += c*h;
	inverse[0][2]=b*f; inverse[0][2]-=c*e;
	inverse[1][0]=-d*k; inverse[1][0]+=f*g;
	inverse[1][1]=a*k; inverse[1][1]-=c*g;
	inverse[1][2]=-a*f; inverse[1][2]+=c*d;
	inverse[2][0]=d*h; inverse[2][0]-=e*g;
	inverse[2][1]=-a*h; inverse[2][1]+=b*g;
	inverse[2][2]=a*e; inverse[2][2]-=b*d;

	double det = determinant3x3(matrix);

	// cout<<"DET "<<det<<endl<<endl;

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			/*if(det == 0.0 && inverse[i][j] == 0.0)
				inverse[i][j] = 1.0;
			else*/ if(det != 0.0)
				inverse[i][j] /= det;
		}
	}
}

void printMatrix(double** matrix, int m, int n) {
	cout << endl;
	for(int i = 0; i < m; i++) {
		cout << "  ";
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << "  ";
		}
		cout << endl;
	}
	cout << endl;
}