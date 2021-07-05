#ifndef MAIN_H
#define MAIN_H

#include <vector>

class Approximator {
	public:
		Approximator();
		double parabola_eval(double, double, double);
		std::vector<double> parabola_grad(double, double, double);
		std::vector<double> oscillate_eval(double, double, double);
		std::vector<double> oscillate_grad(double, double, double);
		std::vector<double> func_eval(double, double, double, double);
		std::vector<double> func_grad(double, double, double, double);
		double f(std::vector<double>);
		double integrate(double, double, int);
};

#endif