#include "Solution.h"
#include <iostream>

#include <ctime>
#include <cstdlib>
#include <math.h>

Solution::Solution() {
	srand (static_cast <unsigned> (time(0)));
}

float** Solution::create_random_params() {
	float LO = -1;
	float HI = 1;

	float** ret = new float*[2];
	for (int i = 0; i < 2; i++) {
		ret[i] = new float[6];
		for (int j = 0; j < 6; j++) {
			ret[i][j] = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
		}
	}
	return ret;
};

float Solution::f_x(float* params, float x, float y) {
	return -params[0]*sin(x) + params[1]*cos(x) + params[4]*cos(x)*(pow(cos(y),2)) - params[5]*sin(y)*sin(2*x);
};

float Solution::f_y(float* params, float x, float y) {
	return -params[2]*sin(y) + params[3]*cos(y) - params[4]*sin(x)*sin(2*y) + params[5]*cos(y)*(pow(cos(x),2));
};

float Solution::i_fast(float** params, int intervals) {
	float* params_a = params[0];
	float* params_b = params[1];
	float PI = 3.14159;
	float* vals = new float[intervals];
	for (int i = 0; i<intervals; i++) {
		vals[i] = -PI + (2*PI*i)/(intervals-1);
	}

	float rhs,lhs = 0;

	for (int i = 0; i<intervals; i++) {
		for (int j = 0; j<intervals; j++) {
			float dax = f_x(params_a, vals[i], vals[j]);
			float day = f_y(params_a, vals[i], vals[j]);
			float dbx = f_x(params_b, vals[i], vals[j]);
			float dby = f_y(params_b, vals[i], vals[j]);

			float ng2 = pow(dax,2) + pow(day,2) + pow(dbx,2) + pow(dby,2);
			float ng4 = pow(ng2,2);
			float dg = dax*dby - day*dbx;
			
			lhs += ng4;
			rhs += ng2*dg;
		}
	}

	if (rhs == 0) {
		return 0;
	}
	return lhs/rhs;
};


