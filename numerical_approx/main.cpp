#define _USE_MATH_DEFINES
#include "main.h"
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <vector>

Approximator::Approximator() {
	std::cout << "Constructor called" << "\n";
}

double Approximator::parabola_eval(double x, double y, double m) {
	return (1-pow(x,2*m))*(1-pow(y,2*m));
}

std::vector<double> Approximator::parabola_grad(double x, double y, double m) {
	return std::vector<double> { -2*m*pow(x,2*m-1)*(1-pow(y,2*m)), -2*m*pow(y,2*m-1)*(1-pow(x,2*m)) };
}

std::vector<double> Approximator::oscillate_eval(double x, double epsilon, double delta) {
	double k = ceil(delta/(2*pow(epsilon,2)));

	//delta is the slope
	//k is the number of peaks
	//d/2k is height of each peak
	
	//approximation with sin and arcsin
	//double m = k/delta;
	//return std::vector<double> {abs(asin(sin(k*M_PI*x)))/(m*M_PI), 0}; 
	double l = delta/(2*k);
	double newx = x*k - floor(x*k);
	return std::vector<double> { -abs(2*l*newx - l) + l, 0 };
}

std::vector<double> Approximator::oscillate_grad(double x, double epsilon, double delta) {
	double k = ceil(delta/(2*pow(epsilon,2)));
	if (x*k - floor(x*k) <= 0.5) {
		return std::vector<double> {delta, 0};
	} 
	else {
		return std::vector<double> {-1*delta, 0};
	}
}

std::vector<double> Approximator::func_eval(double x, double y, double epsilon, double delta) {
	double m = 1/epsilon;
	std::vector<double> oscillate_out = oscillate_eval(x, epsilon, delta);
	double parabola_out = parabola_eval(x,y,m);
	return std::vector<double> {oscillate_out[0]*parabola_out, 0};
}

std::vector<double> Approximator::func_grad(double x, double y, double epsilon, double delta) {
	double m = 1/epsilon;
	std::vector<double> oscillate_grad_out = oscillate_grad(x,epsilon,delta);
	//std::cout << oscillate_grad_out[0] << " " << oscillate_grad_out[1] << " " <<  oscillate_grad_out[2] << " " << oscillate_grad_out[3] << " " << "\n";

	std::vector<double> oscillate_out = oscillate_eval(x,epsilon,delta);
	double parabola_out = parabola_eval(x,y,m);
	std::vector<double> parabola_grad_out = parabola_grad(x,y,m);


	//Returns a 2x2 matrix flattened to a vector in R4. 
	//For x = (x1,x2,x3,x4) as an output, correponds to
	// the matrix  x = (x1  x2)
	//                 (x3  x4)


	// x3 is zero here since second parameter of oscillate functions returns 0
	// same reasoning for x4
	return std::vector<double> {
		parabola_grad_out[0]*oscillate_out[0] + oscillate_grad_out[0]*parabola_out, parabola_grad_out[1]*oscillate_out[0],
		0, 0
	};
}

double Approximator::f(std::vector<double> xi) {
	double EPSILON = 0.0001;
	double norm_sq = 0;
	for (int i = 0; i<4; i++) {
		norm_sq += pow(xi[i],2);
	}
	double det_xi = xi[0]*xi[3] - xi[1]*xi[2];

	return norm_sq*(norm_sq - (4./pow(3,0.5) + EPSILON)*det_xi);
}

//dont use something like 10^k + 1 for even splitting
double Approximator::integrate(double epsilon, double delta, int intervals) {
	double* vals = new double[intervals];
	for (int i = 0; i<intervals; i++) {
		vals[i] = (1.*i)/(intervals-1);
	}

	int positives = 0;
	int negatives = 0;
	int total = 0;
	double ret = 0;
	for (unsigned int i = 0; i<intervals-1; i++) {
		//std::vector<double> test = func_grad(vals[i], 0, epsilon, delta);
		for (unsigned int j = 0; j<intervals-1; j++) {
			std::vector<double> grad_out = func_grad(vals[i], vals[j], epsilon, delta);
			if (grad_out[0] > 0) {
				positives++;
			}
			else {
				negatives++;
			}
			total++;
			grad_out[0] += 1;
			grad_out[3] += pow(3,0.5);
			ret += f(grad_out);
		}
	}
	printf("Positives: %d, Negatives: %d, Total: %d\n", positives, negatives, total);
	return ret/(pow(intervals-1,2));
}

int main() {
	Approximator a = Approximator();

	double epsilon = 0.0001;
	double delta = 0.001;
	double k = ceil(delta/(2*pow(epsilon,2)));
	printf("Number of Intervals: %d\n", (int)k);
	double d = a.integrate(epsilon, delta, 23815);
	printf("Integral of f(xi + grad_phi): %f\n", d);

	double f_out = a.f(std::vector<double>{1,0,0,pow(3,0.5)});
	printf("f(xi): %f\n", f_out);

	printf("Difference %f: ", d-f_out);
}