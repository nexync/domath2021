#include "Solution.h"
#include <iostream>

#include <ctime>
#include <cstdlib>
#include <math.h>
#include <vector>

struct opVal {
	double c;
	double** params;
};

struct retStruct {
	int size;
  opVal* values; 
};

Solution::Solution() {
	srand (static_cast <unsigned> (time(0)));
	double** defc = new double*[2];
	for (int i = 0; i<2; i++) {
		defc[i] = new double[10]{0,0,0,0,0,0,0,0,0,0};
	}

	this->curWidth = 1.;
	this->curCenters = defc;
}

void Solution::setWidth(double width) {
	this->curWidth = width;
}

void Solution::setCenters(double** centers) {
	this->curCenters = centers;
}

double** Solution::create_random_params(double** centers, double width) {
	double** ret = new double*[2];
	for (int i = 0; i < 2; i++) {
		ret[i] = new double[10];
		for (int j = 0; j < 10; j++) {
			double LO = centers[i][j] - width;
			double HI = centers[i][j] + width;
			ret[i][j] = LO + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HI-LO)));
		}
	}
	return ret;
};


//choose f = a_0cosx + a_1sinx + a_2cos2x + a_3sin2x + a_4cos3x + a_5sin3x + a_6cos4x + a_7sin4x + a_8cos5x + a_9sin5x
double Solution::eval_func(double* params, double x) {
	return params[0]*cos(x) + params[1]*sin(x) + params[2]*cos(2*x) + params[3]*sin(2*x) + params[4]*cos(3*x) + params[5]*sin(3*x) + params[6]*cos(4*x) + params[7]*sin(4*x) + params[8]*cos(5*x) + params[9]*sin(5*x);
}

double Solution::eval_func_prime(double* params, double x) {
	return -params[0]*sin(x) + params[1]*cos(x) - 2*params[2]*sin(2*x) + 2*params[3]*cos(2*x) + -3*params[4]*sin(3*x) + 3*params[5]*cos(3*x) + -4*params[6]*sin(4*x) + 4*params[7]*cos(4*x) + -5*params[8]*sin(5*x) + 5*params[9]*cos(5*x);
}

double Solution::a_x(double* params, double x, double y) {
	return eval_func_prime(params, x) * cos(y);
};

double Solution::a_y(double* params, double x, double y) {
	return -1*eval_func(params,x) * sin(y);
};

double Solution::b_x(double* params, double x, double y) {
	return eval_func_prime(params,x)*sin(y);
};

double Solution::b_y(double* params, double x, double y) {
	return eval_func(params,x)*cos(y);
};

double Solution::check_integral(double** params, int intervals) {

	double* params_a = params[0];
	double* params_b = params[1];
	double PI = 3.14159;
	double* vals = new double[intervals];
	for (int i = 0; i<intervals; i++) {
		vals[i] = -PI + (2*PI*i)/(intervals-1);
	}

	double rhs = 0;
	double lhs = 0;

	for (int i = 0; i<intervals; i++) {
		for (int j = 0; j<intervals; j++) {
			double dax = a_x(params_a, vals[i], vals[j]);
			double day = a_y(params_a, vals[i], vals[j]);
			double dbx = b_x(params_b, vals[i], vals[j]);
			double dby = b_y(params_b, vals[i], vals[j]);

			double ng2 = pow(dax,2) + pow(day,2) + pow(dbx,2) + pow(dby,2);
			double ng4 = pow(ng2,2);
			double dg = dax*dby - day*dbx;
			
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

double Solution::i_fast(double** params, int intervals) {
	double* params_a = params[0];
	double* params_b = params[1];
	double PI = 3.14159;
	double* vals = new double[intervals];
	for (int i = 0; i<intervals; i++) {
		vals[i] = -PI + (2*PI*i)/(intervals-1);
	}

	double rhs = 0;
	double lhs = 0;

	for (int i = 0; i<intervals; i++) {
		double f = eval_func(params_a, vals[i]);
		double fp = eval_func_prime(params_a, vals[i]);

		double g = eval_func(params_b, vals[i]);
		double gp = eval_func_prime(params_b, vals[i]);

		rhs += pow(fp,3)*g + fp*pow(g,3) + pow(f,3)*gp + f*pow(gp,3) + (1./3.)*( (pow(f,2)*fp*g)+ (pow(gp,2)*fp*g) + (pow(fp,2)*f*gp) + (pow(g,2)*f*gp) );
		lhs += pow(fp,4) + 2*pow(fp,2)*pow(g,2) + pow(g,4) + pow(f,4) + 2*pow(f,2)*pow(gp,2) + pow(gp,4) + (2./3.)*( (pow(fp,2)*pow(f,2)) + (pow(f,2)*pow(g,2)) + (pow(fp,2)*pow(gp,2)) + (pow(g,2)*pow(gp,2)) );
	}
	// std::cout << "lhs " <<  lhs << std::endl;
	// std::cout << "rhs " << rhs << std::endl;
	if (rhs == 0) {
		return 0;
	}
	return lhs/rhs;
};

retStruct Solution::optimize(int iterations, int num_starts, double target) {
	std::cout << "Calling Optimize Function" << std::endl;
	std::vector<opVal> ret_vals;
	// double MAX_C = 9999999;
	// double** maxvals = new double*[2];
	// for (int i = 0; i<2; i++) {
	// 	maxvals[i] = new double[10]{0,0,0,0,0,0,0,0,0,0};
	// }

	for (int i = 0; i<num_starts; i++) {
		if (i % 500 == 0) {
			std::cout << "Finished " << i << " out of " << num_starts << " points." << std::endl;
		}
		double** resetCenters = new double*[2];
		for (int i = 0; i<2; i++) {
			resetCenters[i] = new double[10]{0,0,0,0,0,0,0,0,0,0};
		}
		setCenters(resetCenters);
		setWidth(1);

		double** temp_max_params = create_random_params(this->curCenters, this->curWidth);
		double test_c = i_fast(temp_max_params,1000);
		double temp_max_c = 0;
		if (test_c > 0 && test_c <= target + 2) {
			temp_max_c = i_fast(temp_max_params,3000);
		}
		else {
			continue;
		}


		//std::cout << "First c Value " << temp_max_c << std::endl;

		if (temp_max_c > 0 && temp_max_c <= target) {
			//std::cout << "Beginning to Optimize Point: " << temp_max_c << std::endl;
			setCenters(temp_max_params);
		
			for (int j = 0; j<iterations; j++) {
				double** params = create_random_params(this->curCenters, this->curWidth);
				double temp_c = i_fast(params, 2000);
				if (temp_c <= 0.5)	continue;
				else {
					if (temp_c < temp_max_c) {
						// double maxdiff = 0;
						// for (int k = 0; k < 2; k++) {
						// 	for (int l = 0; l<10; l++) {
						// 		if(abs(temp_max_params[k][l] - params[k][l] > maxdiff)) {
						// 			std::cout << abs(temp_max_params[k][l] - params[k][l] > maxdiff) << std::endl;
						// 			maxdiff = abs(temp_max_params[k][l] - params[k][l] > maxdiff);
						// 		}
						// 	}
						// }
						temp_max_c = temp_c;
						temp_max_params = params;
						setCenters(params);
						//setWidth((9./10.)*this->curWidth);
					}
				}
			}
			//std::cout << "Optimized c value " << temp_max_c << std::endl;

			// if (temp_max_c < MAX_C) {
			// 	MAX_C = temp_max_c;
			// 	maxvals = temp_max_params;
			// }
			if (temp_max_c < 5 && temp_max_c > 0.5) {
				struct opVal temp {
					temp_max_c, temp_max_params
				};
				ret_vals.push_back(temp);
			}
		}
	}
	// opVal ret;
	// ret.c = MAX_C;
	// ret.params = maxvals;

	struct retStruct ret {
		ret_vals.size(), ret_vals.data()
	};
	return ret;
}

opVal Solution::efficient_optimize(double** params, int max_iters, double threshold, double initial_width) {
	double** new_params = params;
	double cval = i_fast(params, 2000);
	double width = initial_width;
	int iters = 0;
	while (true) {	
		double** temp_params = new_params;

		for (int i = 0; i<2; i++) {
			for (int j = 0; j<10; j++) {
				int randomval = rand() % 2;
				if (randomval == 1) {
					temp_params[i][j] += width;
				}
				else {
					temp_params[i][j] -= width;
				}
			}
		}
		double temp_cval = i_fast(temp_params, 2000);

		if (0.5 < temp_cval < cval) {
			new_params = temp_params;
			cval = temp_cval;
		}
		else {
			width *= 2;
		}
		iters++;
		if (iters > max_iters || temp_cval < threshold) {
			break;
		}
	}
	struct opVal ret {
		cval, new_params
	};
	return ret;
}


