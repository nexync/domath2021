#include "Solution.h"
#include <iostream>

#include <ctime>
#include <cstdlib>
#include <math.h>

struct opVal {
	float c;
	float* params;
};

Solution::Solution() {
	srand (static_cast <unsigned> (time(0)));
	
	float* defc = new float[10]{0,0,0,0,0,0,0,0,0,0};
	this->curWidth = 1.;
	this->curCenters = defc;
}

void Solution::setWidth(float width) {
	this->curWidth = width;
}

void Solution::setCenters(float* centers) {
	this->curCenters = centers;
}

float* Solution::create_random_params(float* centers, float width) {
	float* ret = new float[10];
	for (int i = 0; i < 10; i++) {
		float LO = centers[i] - width;
		float HI = centers[i] + width;
		ret[i] = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
	}
	return ret;
};


//choose f = a_0cosx + a_1sinx + a_2cos2x + a_3sin2x + a_4cos3x + a_5sin3x + a_6cos4x + a_7sin4x + a_8cos5x + a_9sin5x
float Solution::eval_f(float* params, float x) {
	return params[0]*cos(x) + params[1]*sin(x) + params[2]*cos(2*x) + params[3]*sin(2*x) + params[4]*cos(3*x) + params[5]*sin(3*x) + params[6]*cos(4*x) + params[7]*sin(4*x) + params[8]*cos(5*x) + params[9]*sin(5*x);
}

float Solution::eval_f_prime(float* params, float x) {
	return -params[0]*sin(x) + params[1]*cos(x) - 2*params[2]*sin(2*x) + 2*params[3]*cos(2*x) + -3*params[4]*sin(3*x) + 3*params[5]*cos(3*x) + -4*params[6]*sin(4*x) + 4*params[7]*cos(4*x) + -5*params[8]*sin(5*x) + 5*params[9]*cos(5*x);
}

float Solution::i_fast(float* params, int intervals) {
	float PI = 3.14159;
	float* vals = new float[intervals];
	for (int i = 0; i<intervals; i++) {
		vals[i] = -PI + (2*PI*i)/(intervals-1);
	}

	float rhs = 0;
	float lhs = 0;

	for (int i = 0; i<intervals; i++) {
		float f = eval_f(params, vals[i]);
		float fp = eval_f_prime(params, vals[i]);

		rhs += pow(fp,3);
		lhs += pow((pow(fp, 2) + 1),2) + (2/3)*(pow(f,2))*(pow(fp,2)+1) + pow(f,4);
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
	float* maxvals = new float[10]{0,0,0,0,0,0,0,0,0,0};
	

	for (int i = 0; i<num_starts; i++) {
		if (i % 500 == 0) {
			std::cout << "Finished " << i << " out of " << num_starts << " points." << std::endl;
		}
		float* resetCenters = new float[10]{0,0,0,0,0,0,0,0,0,0};

		setCenters(resetCenters);
		setWidth(1);

		float* temp_max_params = create_random_params(this->curCenters, this->curWidth);
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
				float* params = create_random_params(this->curCenters, this->curWidth);
				float temp_c = i_fast(params, 1000);
				if (temp_c <= 0)	continue;
				else {
					if (temp_c < temp_max_c) {
						float maxdiff = 0;
						for (int l = 0; l<10; l++) {
							if(abs(temp_max_params[l] - params[l] > maxdiff))	maxdiff = abs(temp_max_params[l] - params[l]);
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


