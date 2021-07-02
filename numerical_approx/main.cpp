#include "main.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
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
	double m = k/delta;
	return std::vector<double> {abs(asin(sin(k*M_PI*x)))/(m*M_PI), 0};
}

std::vector<double> Approximator::oscillate_grad(double x, double epsilon, double delta) {
	double k = ceil(delta/(2*pow(epsilon,2)));
	if (x*k - floor(x*k) <= 0.5) {
		return std::vector<double> {delta, 0};
	} 
	else {
		return std::vector<double> {-delta, 0};
	}
}

std::vector<double> Approximator::func_eval(double x, double y, double epsilon, double delta) {
	double m = 1/epsilon;
	std::vector<double> oscillate_out = oscillate_eval(x, epsilon, delta);
	double parabola_out = parabola_eval(x,y,m);
	return std::vector<double> {oscillate_out[0]*parabola_out, 0};
}
