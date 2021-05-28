#include "Solution.h"
#include <iostream>

#include <ctime>
#include <cstdlib>
#include <math.h>

struct opVal {
	float c;
	float** params;
};

Solution::Solution() {
	srand (static_cast <unsigned> (time(0)));
	float** defc = new float*[2];
	for (int i = 0; i<2; i++) {
		defc[i] = new float[6]{0,0,0,0,0,0};
	}

	this->curWidth = 1.;
	this->curCenters = defc;
}

void Solution::setWidth(float width) {
	this->curWidth = width;
}

void Solution::setCenters(float** centers) {
	this->curCenters = centers;
}

float** Solution::create_random_params(float** centers, float width) {
	float** ret = new float*[2];
	for (int i = 0; i < 2; i++) {
		ret[i] = new float[6];
		for (int j = 0; j < 6; j++) {
			float LO = centers[i][j] - width;
			float HI = centers[i][j] + width;
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

	float rhs = 0;
	float lhs = 0;

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
	// std::cout << "lhs " <<  lhs << std::endl;
	// std::cout << "rhs " << rhs << std::endl;
	if (rhs == 0) {
		return 0;
	}
	return lhs/rhs;
};

opVal Solution::optimize(int iterations, int num_starts, float target) {
	std::cout << "Calling Optimize Function" << std::endl;

	float MAX_C = 9999999;
	float** maxvals = new float*[2];
	for (int i = 0; i<2; i++) {
		maxvals[i] = new float[6]{0,0,0,0,0,0};
	}

	for (int i = 0; i<num_starts; i++) {
		if (i % 500 == 0) {
			std::cout << "Finished " << i << " out of " << num_starts << " points." << std::endl;
		}
		float** resetCenters = new float*[2];
		for (int i = 0; i<2; i++) {
			resetCenters[i] = new float[6]{0,0,0,0,0,0};
		}
		setCenters(resetCenters);
		setWidth(1);

		float** temp_max_params = create_random_params(this->curCenters, this->curWidth);
		float test_c = i_fast(temp_max_params,500);
		float temp_max_c = 0;
		if (test_c > 0 && test_c <= target + 2) {
			temp_max_c = i_fast(temp_max_params,1500);
		}
		else {
			continue;
		}


		//std::cout << "First c Value " << temp_max_c << std::endl;

		if (temp_max_c > 0 && temp_max_c <= target) {
			std::cout << "Beginning to Optimize Point: " << temp_max_c << std::endl;
			setCenters(temp_max_params);
		
			for (int j = 0; j<iterations; j++) {
				float** params = create_random_params(this->curCenters, this->curWidth);
				float temp_c = i_fast(params, 1000);
				if (temp_c <= 0)	continue;
				else {
					if (temp_c < temp_max_c) {
						float maxdiff = 0;
						for (int k = 0; k < 2; k++) {
							for (int l = 0; l<6; l++) {
								if(abs(temp_max_params[k][l] - params[k][l] > maxdiff))	maxdiff = abs(temp_max_params[k][l] - params[k][l] > maxdiff);
							}
						}
						temp_max_c = temp_c;
						temp_max_params = params;
						setCenters(params);
						setWidth(maxdiff);
					}
				}
			}
			std::cout << "Optimized c value " << temp_max_c << std::endl;

			if (temp_max_c < MAX_C) {
				MAX_C = temp_max_c;
				maxvals = temp_max_params;
			}
		}
	}
	opVal ret;
	ret.c = MAX_C;
	ret.params = maxvals;
	return ret;
}


