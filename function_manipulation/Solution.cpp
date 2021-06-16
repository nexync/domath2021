#include "solution.h"

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
}

double Solution::evaluate(double** params, int intervals) {
	auto eval = [](std::vector<poly> polys, double** inputs, int intervals) {
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
	
	double right = eval(rhs, params, intervals);
	//std::cout << "right" << right << "\n";
	double left = eval(lhs, params, intervals);
	//std::cout << "left" << left << "\n";
	return right/left;
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