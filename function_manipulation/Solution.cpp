#include "Solution.h"

#include <iostream>
#include <fstream>			// ifstream and ofstream
#include <sstream> 			// std::istringstream
#include <vector>
#include <string>

struct dummy_var {
	int subscript;
	char letter;

	bool operator<(const dummy_var& rhs) const {
		if (letter != rhs.letter) {
			return letter < rhs.letter;
		}
		else {
			return subscript < rhs.subscript;
		}
		return true;
	}

	bool operator==(const dummy_var& rhs) const {
		if (letter == rhs.letter && subscript == rhs.subscript)	return true;
		else	return false;
	}
};

struct coeff {
	int factor;
	std::vector<dummy_var> variables;
};

struct poly {
	std::vector<coeff> scales;
	double factor;
};

struct param_dict {
	double alpha;
	double** v0;
	double mu;
};

struct ret_triple {
	double** point;
	double res;
	param_dict state;
};

std::ofstream& operator<<(std::ofstream& os, const poly& p) {
	if (p.scales.size() > 0) {
		os << p.factor << " ";
		for (coeff c: p.scales) {
			os << c.factor << " ";
			for (dummy_var v: c.variables) {
				os << v.letter << "_" << v.subscript << "/";
			}
			os << " ";
		}
		os << "\n";
	}
	return os;
}

Solution::Solution() {
	printf("Empty Constructor Called \n");
}

Solution::Solution(int num_functions, int num_params) {
	if (num_functions == 4) {
		std::vector<poly> l,r;

		printf("Reading in Left hand integral \n");
		std::ifstream infile_lhs("lhs.txt");
		if (!infile_lhs) {
			printf("Left hand not found \n");
		}
		std::string temp_left;
		while(std::getline(infile_lhs, temp_left)) {
			poly p = polyFromString(temp_left);
			l.push_back(p);
		}

		printf("Reading in Right hand integral \n");
		std::ifstream infile_rhs("rhs.txt");
		if (!infile_rhs) {
			printf("Right hand not found \n");
		}
		std::string temp_right;
		while(std::getline(infile_rhs, temp_right)) {
			poly p = polyFromString(temp_right);
			r.push_back(p);
		}

		this->lhs = l;
		this->rhs = r;
		this->nf = num_functions;
		this->np = num_params;
	}
	else {
		printf("Use correct starting parameter\n");
	}
}


std::vector<dummy_var> Solution::dummyFromString(std::string inp) {
	std::vector<dummy_var> d;
	const std::string delimiter = "/";

	size_t pos = 0;
	std::string token;
	while ((pos = inp.find(delimiter)) != std::string::npos) {
		token = inp.substr(0, pos);

		char c = token[0];
		int subscript = atoi(token.substr(2).c_str());
		d.push_back({subscript, c	});	

		inp.erase(0, pos + delimiter.length());
	}
	return d;
}

poly Solution::polyFromString(std::string inp) {
	std::vector<coeff> scales;
	std::istringstream stm(inp);
	std::string s;
	stm >> s;
	double factor = atof(s.c_str());
	while(stm >> s) {
		int sc = atoi(s.c_str());
		stm >> s;
		std::vector<dummy_var> d = dummyFromString(s);
		scales.push_back({sc, d});
	}
	struct poly p {
		scales, factor
	};
	return p;
}

double Solution::evaluate(double** params) {
	auto eval = [](std::vector<poly> polys, double** inputs) {
		double ret = 0;
		for (poly p: polys) {
			double temp_sum = 0;
			for (coeff c: p.scales) {
				double temp_prod = 1;
				for (dummy_var v: c.variables) {
					int index;
					switch (v.letter) {
						case 'a':
							index = 0;
							break;
						case 'b':
							index = 1;
							break;
						case 'c':
							index = 2;
							break;
						case 'd':
							index = 3;
							break;
						default:
							index = -1;
							break;
					}
					temp_prod *= inputs[index][v.subscript];
					if (temp_prod == 0) {
						break;
					}
				}
				temp_sum += c.factor*temp_prod;
			}
			ret += p.factor*temp_sum;
		}

		return ret;
	};
	
	double right = eval(rhs, params);
	//std::cout << "right" << right << "\n";
	double left = eval(lhs, params);
	//std::cout << "left" << left << "\n";
	return right/left;
}

double** Solution::gradient(double** params) {
	auto bound = [this](double** params, double width, int function_index, int parameter_index) {
		double** new_params = new double*[nf];
		for (int i = 0; i<nf; i++) {
			new_params[i] = new double[np];
			for (int j = 0; j<np; j++) {
				new_params[i][j] = params[i][j];
			}
		}

		new_params[function_index][parameter_index] += width;
		double right = evaluate(new_params);
		new_params[function_index][parameter_index] -= 2*width;
		double left = evaluate(new_params);

		return (right - left)/(2*width);
	};

	double threshold = 0.001;
	double** ret = new double*[nf];
	double width = 1;

	for (int i = 0; i<nf; i++) {
		ret[i] = new double[np];
		for (int j = 0; j<np; j++) {
			width = 1;
			double slope = bound(params, width, i, j);
			//printf("initial slope: %f \n", slope);
			int k = 0;
			while(true) {
				if (k > 100)	break;
				width = width/2; 
				k++;
				double new_slope = bound(params, width, i, j);
				//printf("new slope at %d iteration: %f, width: %f\n", k, new_slope, width);
				if (abs(slope-new_slope) < threshold)	break;
				else	slope = new_slope;
			}
			//printf("k value: %d\n", k);
			ret[i][j] = slope;
		}
	}
	return ret;
}


ret_triple Solution::steepest_descent(double** initial, double step_tolerance, double grad_tolerance, int iteration_tolerance) {

	auto norm = [this](double** point) {
		double ret = 0;
		for (int i = 0; i<nf; i++) {
			for (int j = 0; j<np; j++) {
				ret += pow(point[i][j], 2);
			}
		}
		return pow(ret, 0.5);
	};

	auto momentum = [this](double** point, double** g, param_dict state) {
		double** v1 = new double*[nf];
		for (int i = 0; i<nf; i++) {
			v1[i] = new double[np];
			for (int j = 0; j<np; j++) {
				v1[i][j] = state.mu * state.v0[i][j] + state.alpha*g[i][j];
			}
		}

		state.v0 = v1;

		double** new_point = new double*[nf];
		for (int i = 0; i<nf; i++) {
			new_point[i] = new double[np];
			for (int j = 0; j<np; j++) {
				new_point[i][j] = point[i][j] + v1[i][j];
			}
		}
		return ret_triple{new_point, evaluate(new_point), state};
	};
	
	int iters = 0;

	double** v0 = new double*[nf];
	for (int i = 0; i<nf; i++) {
		v0[i] = new double[np]{ };
	}

	param_dict initial_state = {0.001, v0, 0.9};
	ret_triple trip = {initial, evaluate(initial), initial_state};

	while(true) {
		double** g = gradient(trip.point);
		if (norm(g) < grad_tolerance) {
			break;
		}

		ret_triple temp_trip = momentum(trip.point, g, trip.state);
		iters ++;  //iteration increment

		double n = 0; //norm calculation
		for (int i = 0; i<nf; i++) {
			for(int j = 0; j<np; j++) {
				n += pow(trip.point[i][j] - temp_trip.point[i][j], 2);
			}
		}

		trip = temp_trip;
		if (pow(n, 0.5) < step_tolerance || iters > iteration_tolerance) {
			break;
		}
	}
	return trip;
}